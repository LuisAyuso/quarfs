#include "render/window_mgr.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GL/glfw3.h> // GLFW helper library

#include <iostream>
#include <assert.h>
#include <math.h>

#include <glm/glm.hpp>
#include "renderer.h"

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


    bool mouseDown = false;
    void mouseCallback(GLFWwindow *w, int b, int a) {
        mouseDown = a==GLFW_PRESS;
        // save press possition mouse
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
    glfwSetMouseButtonCallback (window, mouseCallback);

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
    // mouse
    glfwGetCursorPos (window, &mouseX, &mouseY);
    lastmX = mouseX;
    lastmY = mouseY;

    //////////////////////////////////////////////
    // initialize renderer
    renderer.init(w,h);

    registerInputListener(&renderer);
}

WindowManager::~WindowManager(){
    // close GL context and any other GLFW resources
    glfwTerminate();
}
        
void WindowManager::setupFrame(){

    static int count =0; //  60 frames?  30 will be half a second

    // measure fps
    update_fps_counter();
    // update other events like input handling 
    glfwPollEvents ();

    glfwGetCursorPos (window, &mouseX, &mouseY);

    // ESC key handling
    if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose (window, 1);
    }
    if (glfwGetKey (window, GLFW_KEY_UP))    { for(auto i : inputListeners) i->keyUp();}
    if (glfwGetKey (window, GLFW_KEY_DOWN))  { for(auto i : inputListeners) i->keyDown();}
    if (glfwGetKey (window, GLFW_KEY_LEFT))  { for(auto i : inputListeners) i->keyLeft();}
    if (glfwGetKey (window, GLFW_KEY_RIGHT)) { for(auto i : inputListeners) i->keyRight();}
    if (glfwGetKey (window, GLFW_KEY_SPACE)) { if(!count){
                                                    for(auto i : inputListeners) i->spacebar();
                                                    count = 30;
                                                  }
                                              }
    if (mouseDown){ for(auto i : inputListeners) i->mouseDiff((lastmX-mouseX)/10.0, (lastmY-mouseY)/10.); }
    lastmX=mouseX; 
    lastmY=mouseY;
    if (count) count --;

    static int oldW, oldH;
    if (oldW !=  g_gl_width || oldH != g_gl_height){
        renderer.setPerspective(g_gl_width, g_gl_height);
        oldW=g_gl_width; 
        oldH=g_gl_height;
    }
    glViewport (0, 0, g_gl_width, g_gl_height);
    renderer.beginDraw();
}

void WindowManager::registerInputListener(InputListener* lis){
    inputListeners.push_back(lis);
}

Camera WindowManager::getCamera(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up) {
    return Camera(pos, lookAt, up, renderer);
}

void WindowManager::finishFrame(){
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers (window);
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
