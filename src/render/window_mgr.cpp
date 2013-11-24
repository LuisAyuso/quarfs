
#include "render/window_mgr.h"
#include <iostream>

Display* WindowManager::createAndInitialzeWindow(){

    GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo             *vi;
    Colormap                cmap;
    XSetWindowAttributes    swa;
    XWindowAttributes       gwa;

	display = XOpenDisplay(NULL);

	if(display == NULL) {
        std::cerr << "\n\tcannot connect to X server\n\n";
        exit(0);
	}

	root = DefaultRootWindow(display);

	vi = glXChooseVisual(display, 0, att);

	if(vi == NULL) {
        std::cerr <<"\n\tno appropriate visual found\n\n";
        exit(-1);
	} 

	cmap = XCreateColormap(display, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;

	win = XCreateWindow(display, root, 0, 0, 800, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(display, win);
	XStoreName(display, win, "cubeLand");

    // register the close window button
    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, win, &wmDeleteMessage, 1);

    // create gl context
	glc = glXCreateContext(display, vi, NULL, GL_TRUE);
	glXMakeCurrent(display, win, glc);

    // now we have a window, lets set some ogl on it
    int x,y;
    unsigned width, height, border, depth;
    Window dummy;
    XGetGeometry(display, win, &dummy, &x, &y, &width, &height, &border, &depth);
    initGL(width,height);
    return display;
}

void WindowManager::resizeGLScene(unsigned int width, unsigned int height) {

    if (height == 0)    /* Prevent A Divide By Zero If The Window Is Too Small */
        height = 1;
    glViewport(0, 0, width, height);    /* Reset The Current Viewport And Perspective Transformation */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void WindowManager::initGL(unsigned int width, unsigned int height) {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { 0.0, 40.0, 0.0, 1.0 };
    glClearColor (0.0, 0.0, 0.0, 0.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /* we use resizeGLScene once to set up our initial perspective */
    resizeGLScene(width, height);
    /* Reset the rotation angles of our objects */
    glFlush();
}

void WindowManager::destroyContext(){
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, glc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
}
