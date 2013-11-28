
#include <iostream> // include GLEW and new version of GL on Windows
#include <GL/glew.h> // include GLEW and new version of GL on Windows

#include "world/cube.h"


namespace {

    unsigned int initializeCube(){

    std::cout << "alocate the cube! " << std::endl;

    // alocate in memory the vertex, then copy to gpu and forget them
    // if order changes, then the thing is not the same
   // float points[] = {
   //     0.0f, 0.0f,  0.0f,
   //     0.0f, 1.0f,  0.0f,
   //     1.0f, 0.0f,  0.0f,
   //     1.0f, 1.0f,  0.0f
   // };
    float points[] = {
        0.0f, 0.0f,  0.0f,
        0.0f, 0.5f,  0.0f,
        0.5f, 0.0f,  0.0f,
        0.5f, 0.5f,  0.0f
    };

    float colours[] = {
        1.0f, 0.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 0.0f,  1.0f,
        1.0f, 0.0f,  1.0f
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

} // anonimous namespace


void DrawNode::initThis(){
    static unsigned singleVao = initializeCube();
    vao = singleVao;
}

DrawNode::DrawNode(float a, float b, float c)
    : pos(a,b,c)
{ 
    initThis();
}

DrawNode::DrawNode(const glm::vec3& pos)
:   pos(pos)
{   
    initThis();
}

const glm::vec3& DrawNode::getPos() const{
    return pos;
}

std::ostream& operator<<(std::ostream& out, const DrawNode& dn){
    out << "DN[" << dn.pos[0] << "," << dn.pos[1] << "," << dn.pos[2] << "]";
    return out;
}

void DrawNode::draw ()const {
    glBindVertexArray (vao);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray (0);
}
