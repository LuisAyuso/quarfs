
#include <iostream>


 #include<X11/X.h>
 #include<X11/Xlib.h>
 #include<GL/gl.h>
 #include<GL/glx.h>
 #include<GL/glu.h>


#include "world.h"
#include "joystick.h"

void resizeGLScene(unsigned int width, unsigned int height) {
    if (height == 0)    /* Prevent A Divide By Zero If The Window Is Too Small */
        height = 1;
    glViewport(0, 0, width, height);    /* Reset The Current Viewport And Perspective Transformation */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void initGL(unsigned int width, unsigned int height) {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    /* we use resizeGLScene once to set up our initial perspective */
    resizeGLScene(width, height);
    /* Reset the rotation angles of our objects */
    glFlush();
}


int main(int argc, char *argv[]) {

    Display                 *display;
    Window                  root;
    GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo             *vi;
    Colormap                cmap;
    XSetWindowAttributes    swa;
    Window                  win;
    GLXContext              glc;
    XWindowAttributes       gwa;
    XEvent                  xev;


	display = XOpenDisplay(NULL);

	if(display == NULL) {
        printf("\n\tcannot connect to X server\n\n");
        exit(0);
	}

	root = DefaultRootWindow(display);

	vi = glXChooseVisual(display, 0, att);

	if(vi == NULL) {
        printf("\n\tno appropriate visual found\n\n");
        exit(-1);
	} 

	cmap = XCreateColormap(display, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;

	win = XCreateWindow(display, root, 0, 0, 600, 480, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	XMapWindow(display, win);
	XStoreName(display, win, "cubeLand");

    // register the close window button
    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(display, win, &wmDeleteMessage, 1);

	glc = glXCreateContext(display, vi, NULL, GL_TRUE);
	glXMakeCurrent(display, win, glc);

    // now we have a window, lets set some ogl on it
    int x,y;
    unsigned width, height, border, depth;
    Window dummy;
    XGetGeometry(display, win, &dummy, &x, &y, &width, &height, &border, &depth);
    initGL(width,height);

    // create a world to draw
    World world;

    // some interaction variables
    Joystick* joy;
    joy = new Joystick();

    // render loop
    bool running = true;
	while(running) {

        // attend first the window events
        while (XPending(display) > 0) {
            XNextEvent(display, &xev);

            std::cout << "event " << xev.type << std::endl;

            if(xev.type == Expose) {
                // expose is ignored, we are updating the screen anyway
                break;
            }

            if(xev.type == ConfigureNotify) {
                // something changed in the window, lets consider what is it
                if (width != xev.xconfigure.width || height != xev.xconfigure.height){
                    // looks like resize
                    width = xev.xconfigure.width;
                    height = xev.xconfigure.height;
                    resizeGLScene(width,height);
                }
            }

            else if(xev.type == ClientMessage){
                if (xev.xclient.data.l[0] == wmDeleteMessage)
                    running = false;
            }

            else if(xev.type == KeyPress ){
                //Vif (XLookupKeysym(&xev.xkey, 0) == XK_Escape)
                //    running = false;
                //if (XLookupKeysym(&xev.xkey, 0) == XK_Spacebar)
                    
                    std::cout << joy->get_num_buttons() << std::endl;
                    std::cout << joy->get_num_axes() << std::endl;
                    std::cout << joy->get_name() << std::endl;
            }
        }

        // upodate scene
        if (world.update()){
            // set camera position and projection
            
            // draw hud

            // draw world
            world.draw();

            // done
            glXSwapBuffers(display, win);
        }
	}

    if (joy) delete joy;

    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, glc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    exit(0);
} 
