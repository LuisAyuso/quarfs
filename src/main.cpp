
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GL/glfw3.h> // GLFW helper library

#include <iostream>

#include <glm/glm.hpp>
#include "render/window_mgr.h"
#include "world/world.h"


unsigned int dumm(){

    std::cout << "alocate the dummy! " << std::endl;

    // alocate in memory the vertex, then copy to gpu and forget them
    // if order changes, then the thing is not the same
    float points[] = {
         -1.0f, -1.0f, 0.0f,
          1.0f, -1.0f, 0.0f,
          0.0f,  1.0f, 0.0f,
    };

    float colours[] = {
        1.0f, 0.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 0.0f,  1.0f
    };

    //////////////////////////////////////////////
    // buffers to print
    unsigned int quad_vbo = 0;
    glGenBuffers (1, &quad_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, quad_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3*3 * sizeof (float), points, GL_STATIC_DRAW);

    unsigned int color_vbo = 0;
    glGenBuffers (1, &color_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, color_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3*3* sizeof (float), colours, GL_STATIC_DRAW);

    unsigned int vao = 0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);

    glBindBuffer (GL_ARRAY_BUFFER, quad_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glBindBuffer (GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

    glEnableVertexAttribArray (0); // optional, first buffer is active by default
    glEnableVertexAttribArray (1); // second buffer is not active by default

    return vao;
}

int main () {

    WindowManager window(800, 600, "Quarfs!");

  //  //////////////////////////////////////////////
  //  // setup render
  //  Renderer& renderer = window.getRenderer();
  //  renderer.compileShaders();
  //  renderer.configureRender();

    //////////////////////////////////////////////
    // build the world
    World w;

    //////////////////////////////////////////////
    // get a camera
    Camera cam = window.getCamera(glm::vec3(4,3,3),
                                  glm::vec3(0,0,0), // and looks at the origin
                                  glm::vec3(0.0, 1.0,0.0));

    ///////////////////////////////////////////////
    // dummy
    struct A{
        unsigned vao;
        A():  vao (dumm()) {} 
        glm::vec3 getPos() const {return glm::vec3(-1, 0, 0); }
        void draw()const {
            glBindVertexArray (vao);
            glDrawArrays (GL_TRIANGLE_STRIP, 0, 3);
        }
    } dummy;

    ///////////////////////////////////////////////
    // draw loop
    while (!window.isFinish()) {
        window.setupFrame();

        shotFrame(cam, w.getTree());
     //   shotFrame(cam, dummy);

        window.finishFrame();
    }

    return 0;
}

