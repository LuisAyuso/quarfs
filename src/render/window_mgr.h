
#pragma once

#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

class WindowManager{

    Display*                display;
    Window                  root;
    Window                  win;
    GLXContext              glc;

    public:

    WindowManager(){
    }

    ~WindowManager(){
        destroyContext();
    }

    Display* createAndInitialzeWindow();

    void resizeGLScene(unsigned int width, unsigned int height) ;

    void initGL(unsigned int width, unsigned int height) ;

    void destroyContext();

    void flushWindow(){
        glXSwapBuffers(display, win);
    }
};
