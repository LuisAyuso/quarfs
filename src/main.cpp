
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GL/glfw3.h> // GLFW helper library

#include <iostream>

#include "render/window_mgr.h"
#include "utils/maths_funcs.h"
#include "world.h"

unsigned int dumm(){

    std::cout << "alocate the cube! " << std::endl;

    // alocate in memory the vertex, then copy to gpu and forget them
    // if order changes, then the thing is not the same
    float points[] = {
        -0.5f, 0.0f,  0.0f,
        .0f, 0.5f,  0.0f,
        .5f, 0.0f,  0.0f
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
    glBufferData (GL_ARRAY_BUFFER, 3*4 * sizeof (float), points, GL_STATIC_DRAW);

    unsigned int color_vbo = 0;
    glGenBuffers (1, &color_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, color_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3*4 * sizeof (float), colours, GL_STATIC_DRAW);

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
    Camera cam = window.getCamera(vec3(0.5, 0.0, 0.0),
                                  vec3(0.5, 0.0, -10.0),
                                  vec3(0.0, 1.0,0.0));

    unsigned dummy = dumm();

    ///////////////////////////////////////////////
    // draw loop
    while (!window.isFinish()) {
        window.setupFrame();

        shotFrame(cam, w.getTree());

  //  glBindVertexArray (dummy);
  //  glDrawArrays (GL_TRIANGLE_STRIP, 0, 3);

        window.finishFrame();
        //assert(false);
    }

    return 0;
}

