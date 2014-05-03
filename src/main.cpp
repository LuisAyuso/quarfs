
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <iostream>

#include <glm/glm.hpp>
#include "render/window_mgr.h"
#include "world/surface.h"
#include "world/world.h"
#include "world/world_cache.h"

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

	WorldCache cache(w);


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
      //  shotFrame(w.getTree());
	 	shotFrame(cache.getWhatToDraw());

        // finish the hole picture
        window.finishFrame();
	
		std::cout << "====================================" << std::endl;
    }

    return 0;
}

