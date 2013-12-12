
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GL/glfw3.h> // GLFW helper library

#include <iostream>

#include <glm/glm.hpp>
#include "render/window_mgr.h"
#include "world/surface.h"
#include "world/world.h"

#include "world/instance.h"

#include "utils/fileHandler.h"


int main () {

    WindowManager window(800, 600, "Quarfs!");

    //////////////////////////////////////////////
    // get a camera
    Camera cam = window.getCamera(glm::vec3(40,40,10),
                                  glm::vec3(0,0,0), // and looks at the origin
                                  glm::vec3(0.0,1.0,0.0));
    window.registerInputListener(&cam);

    //////////////////////////////////////////////
    // build the world
    World w;
    //Surface s;
    //Instance w;

    ///////////////////////////////////////////////
    // draw loop
    while (!window.isFinish()) {

        // some management updates
        updateFileHandler();

        // some logical updates
        
        // visual setup and things
        window.setupFrame();
        cam.update();

        // draw!
        shotFrame(cam, w.getTree());
        shotFrame(cam, w.getTree());
        shotFrame(cam, w.getTree());
        shotFrame(cam, w.getTree());
        shotFrame(cam, w.getTree());
    //    shotFrame(cam, s);
    //
     //   w.update();
     //   w.draw();

        // finish the hole picture
        window.finishFrame();
    }

    return 0;
}

