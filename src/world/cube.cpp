
#include <iostream> // include GLEW and new version of GL on Windows
#include <GL/glew.h> // include GLEW and new version of GL on Windows

#include "world/cube.h"


namespace {

    unsigned int initializeCube(){
    std::cout << "alocate the cube! " << std::endl;
    float points[] = {
       -0.5f,-0.5f,-0.5f,
       -0.5f, 0.5f,-0.5f,
        0.5f, 0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
    
       -0.5f,-0.5f, 0.5f,
       -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f,-0.5f, 0.5f,
    };

    // 12 normals, pass as flat, not interpolated and transmited b the (first/last) vertex
    float normals[] = {
        0.0,  0.0, -1.0,
        0.0,  0.0, -1.0,

        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,

        1.0,  0.0,  0.0,
        1.0,  0.0,  0.0,

        0.0, -1.0,  0.0,
        0.0, -1.0,  0.0,

       -1.0,  0.0,  0.0,
       -1.0,  0.0,  0.0,

        0.0,  1.0,  0.0,
        0.0,  1.0,  0.0,

    };

    float colours[] = {
        159.0/255.0,182.0/255.0,205.0/255.0,
        159.0/255.0,182.0/255.0,205.0/255.0,
        159.0/255.0,182.0/255.0,205.0/255.0,
        159.0/255.0,182.0/255.0,205.0/255.0,

        159.0/255.0,182.0/255.0,205.0/255.0,
        159.0/255.0,182.0/255.0,205.0/255.0,
        159.0/255.0,182.0/255.0,205.0/255.0,
        159.0/255.0,182.0/255.0,205.0/255.0,
    };

    unsigned indexes[]{
        // front
        0,1,2,  
        2,3,0,  

        // back
        4,7,5,  
        5,7,6,

        3,2,6,
        6,7,3,

        0,3,7,
        7,4,0,

        0,4,5,
        5,1,0,

        1,5,2,
        2,5,6
    };

    // alocate in memory the vertex, then copy to gpu and forget them
    // if order changes, then the thing is not the same

    //////////////////////////////////////////////
    // buffers to print
    unsigned int quad_vbo = 0;
    glGenBuffers (1, &quad_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, quad_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3*8 * sizeof (float), points, GL_STATIC_DRAW);

    unsigned int color_vbo = 0;
    glGenBuffers (1, &color_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, color_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3*8 * sizeof (float), colours, GL_STATIC_DRAW);

    unsigned int normals_vbo = 0;
    glGenBuffers (1, &normals_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3*12 * sizeof (float), normals, GL_STATIC_DRAW);

    unsigned int index_ibo = 0;
    glGenBuffers (1, &index_ibo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_ibo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indexes), indexes, GL_STATIC_DRAW);

    unsigned int vao = 0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);

    glBindBuffer (GL_ARRAY_BUFFER, quad_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glBindBuffer (GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
    glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_ibo);
    glVertexAttribPointer (3, 3, GL_UNSIGNED_INT, GL_FALSE, 0, (GLubyte*)NULL);

    glEnableVertexAttribArray (0); // optional, first buffer is active by default
    glEnableVertexAttribArray (1); // second buffer is not active by default
    glEnableVertexAttribArray (2);
    glEnableVertexAttribArray (3);

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
    glDrawElements (GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray (0);
}
