#include "render/window_mgr.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GL/glfw3.h> // GLFW helper library

#include <iostream>
#include <assert.h>
#include <math.h>

#include "utils/maths_funcs.h"

namespace {

    const char* vertex_shader =
        "#version 400\n"
        "layout(location = 0) in vec3 vtx_pos;"
        "layout(location = 1) in vec3 vtx_color;"
        "uniform mat4 matrix;"
        "out vec3 color;"
        "void main () {"
        "  color = vtx_color;"
        "  gl_Position = matrix * vec4 (vtx_pos, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 400\n"
        "in  vec3 color;"
        "out vec4 frag_color;"
        "void main () {"
        "  frag_color = vec4 (color, 1.0);"
        "}";

    void error_callback (int error, const char* description) {
        std::cout << "Error code [" << error << "] " << description << std::endl;
    }

    // keep track of window size for things like the viewport and the mouse cursor
    int g_gl_width = 640;
    int g_gl_height = 480;

    // a call-back function
    void window_size_callback (GLFWwindow* window, int width, int height) {
        g_gl_width = width;
        g_gl_height = height;

        //persperctive changes go here
    }

    bool is_compiled(unsigned shader){
        int params = -1;
        glGetShaderiv (shader, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params) {
            std::cout << " == shader compilation failed == " << std::endl;
            int max_length = 2048;
            int actual_length = 0;
            char log[2048];
            glGetShaderInfoLog (shader, max_length, &actual_length, log);
            std::cout << log << std::endl;
            return false; 
        }
        return true;
    }

    bool is_valid (unsigned prog) {
        glValidateProgram (prog);
        int params = -1;
        glGetProgramiv (prog, GL_VALIDATE_STATUS, &params);
        if (GL_TRUE != params) {
            std::cout << " == shader program not valid == " << std::endl;
            int max_length = 2048;
            int actual_length = 0;
            char log[2048];
            glGetProgramInfoLog (prog, max_length, &actual_length, log);
            std::cout << log << std::endl;
            return false;
        }
        return true;
    }

    // http://antongerdelan.net/teaching/3dprog1/maths_cheat_sheet.pdf

float matrix[] = {
     1.0f, 0.0f, 0.0f, 0.0f, // first column
     0.0f, 1.0f, 0.0f, 0.0f, // second column
     0.0f, 0.0f, 1.0f, 0.0f, // third column
     0.5f, 0.0f, 0.0f, 1.0f  // fourth column
};


} // anonimous namespace

WindowManager::WindowManager(unsigned w, unsigned h, const char* name){

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
        std::cerr << "ERROR: could not start GLFW3\n";
        throw -1;
    } 


    window = glfwCreateWindow (w, h, name, NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: could not open window with GLFW3\n";
        glfwTerminate();
        throw -1;
    }
    glfwMakeContextCurrent (window);

    glfwSetErrorCallback (error_callback);
    glfwSetWindowSizeCallback (window, window_size_callback);

    // start GLEW extension handler
    glewInit ();

    //////////////////////////////////////////////
    //  minimun version to use
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /////////////////////////////////////////////
    // antialiassing
    glfwWindowHint (GLFW_SAMPLES, 4);

    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string

    std::cout << "Renderer: "  << renderer << std::endl;
    std::cout << "OpenGL version supported " <<  version << std::endl;

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

    glEnable (GL_CULL_FACE); // cull face
    glCullFace (GL_BACK); // cull back face
    glFrontFace (GL_CW); // GL_CCW for counter clock-wise

    //////////////////////////////////////////////
    // compile shaders
    unsigned int vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
    assert(is_compiled(vs));
    unsigned int fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);
    assert(is_compiled(fs));

    shader_program = glCreateProgram ();
    glAttachShader (shader_program, fs);
    glAttachShader (shader_program, vs);
    glLinkProgram (shader_program);

    assert(is_valid(shader_program));

    int matrix_location = glGetUniformLocation (shader_program, "matrix");
    glUseProgram (shader_program);
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
}

WindowManager::~WindowManager(){
    // close GL context and any other GLFW resources
    glfwTerminate();
}

void WindowManager::setupFrame(){

    update_fps_counter();

    static float speed = 0.1;
    static float last_position = 0.0f;
    static double previous_seconds = glfwGetTime ();
    double current_seconds = glfwGetTime ();
    double elapsed_seconds = current_seconds - previous_seconds;
    previous_seconds = current_seconds;
    
    // reverse direction when going to far left or right
    if (fabs(last_position) > 1.0f) {
        speed = -speed;
    }
    
    // update the matrix
    matrix[12] = elapsed_seconds * speed + last_position;
    last_position = matrix[12];
    int matrix_location = glGetUniformLocation (shader_program, "matrix");
    glUseProgram (shader_program);
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
              

    // wipe the drawing surface clear
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport (0, 0, g_gl_width, g_gl_height);

    glUseProgram (shader_program);
}

void WindowManager::finishFrame(){
    // update other events like input handling 
    //
    glfwPollEvents ();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers (window);

    // ESC key handling
    if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose (window, 1);
    }
}

bool WindowManager::isFinish() const{
    return glfwWindowShouldClose (window);
}

void WindowManager::update_fps_counter(){
    static double previous_seconds = glfwGetTime ();
    static int frame_count;
    double current_seconds = glfwGetTime ();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf (tmp, "Quarfs @ fps: %.2lf", fps);
        glfwSetWindowTitle (window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

double WindowManager::getFrameRate (){
    return fps;
}
