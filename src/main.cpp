
#include <iostream>

#include "input/joystick.h"
#include "render/window_mgr.h"

#include "world.h"


int main(int argc, char *argv[]) {

    WindowManager wmgr;
    Display *display = wmgr.createAndInitialzeWindow();

    // create a world to draw
    World world;

    // some interaction variables
    Joystick* joy;
    joy = new Joystick();

    // event handler
    XEvent   xev;
    unsigned width, height; 

    // render loop
    bool running = true;
    bool repaint = true;
	while(running) {

        // attend first the window events
        while (XPending(display) > 0) {
            XNextEvent(display, &xev);
           // std::cout << "event " << xev.type << std::endl;

            if(xev.type == Expose) {
                repaint = true;
                break;
            }

            if(xev.type == ConfigureNotify) {
                // something changed in the window, lets consider what is it
                if (width != xev.xconfigure.width || height != xev.xconfigure.height){
                    // looks like resize
                    width = xev.xconfigure.width;
                    height = xev.xconfigure.height;
                    wmgr.resizeGLScene(width,height);
                }
            }

            else if(xev.type == ClientMessage){
                if (xev.xclient.data.l[0] == XInternAtom(display, "WM_DELETE_WINDOW", false))
                    running = false;
            }

            else if(xev.type == KeyPress ){
                if (XLookupKeysym(&xev.xkey, 0) == XK_Escape){
                    running = false;
                }else{
                //if (XLookupKeysym(&xev.xkey, 0) == XK_Spacebar)
               //     std::cout << joy->get_num_buttons() << std::endl;
               //     std::cout << joy->get_num_axes() << std::endl;
               //     std::cout << joy->get_name() << std::endl;
                }
            }
        }

        // upodate scene
        if (world.update() || repaint){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // set camera position and projection
            glLoadIdentity();
            gluLookAt(0, 10, -35, /* look from camera XYZ */ 
                      0, 0, 0, /* look at the origin */ 
                      0, 1, 0); /* positive Y up vector */
            
            // draw hud

            // draw world
            world.draw();

            // done
            wmgr.flushWindow();
            repaint = false;
        }
	}
    if (joy) delete joy;

      exit(0);
} 
