#include "render/window_mgr.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GL/glfw3.h> // GLFW helper library

#include <iostream>
#include <assert.h>
#include <math.h>

#include "renderer.h"
#include "utils/matrix.h"

namespace {
    void error_callback (int error, const char* description) {
        std::cout << "Error code [" << error << "] " << description << std::endl;
    }

    // keep track of window size for things like the viewport and the mouse cursor
    int g_gl_width = 800;
    int g_gl_height = 600;

    // a call-back function
    void window_size_callback (GLFWwindow* window, int width, int height) {
        g_gl_width = width;
        g_gl_height = height;

        //persperctive changes go here
    }
} // anonimous namespace

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

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

    /////////////////////////////////////////////
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

    /////////////////////////////////////////////
    // get version info
    const GLubyte* rendererStr = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    std::cout << "Renderer: "  << rendererStr << std::endl;
    std::cout << "OpenGL version supported " <<  version << std::endl;

    //////////////////////////////////////////////
    // initialize renderer
    renderer.init();
}

WindowManager::~WindowManager(){
    // close GL context and any other GLFW resources
    glfwTerminate();
}
        
void WindowManager::setupFrame(){
    // measure fps
    update_fps_counter();
    glViewport (0, 0, g_gl_width, g_gl_height);
    renderer.beginDraw();
}

Camera WindowManager::getCamera(const vec3& pos, const vec3& inclination, const vec3& lookAt) const{
    return Camera(pos, inclination, lookAt, renderer);
}

void WindowManager::finishFrame(){
    // update other events like input handling 
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

double WindowManager::getFrameRate ()const{
    return fps;
}
