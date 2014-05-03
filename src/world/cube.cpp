
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

			// 8 normals, pass as flat, not interpolated and transmited by the (first/last) vertex
			float normals[] = {
				0.0 ,-1.0, 0.0,   
				0.0 , 0.0, 0.0,  // no use
				0.0 , 0.0,-1.0, 
				0.0 , 0.0, 0.0,  // no use

			   -1.0 , 0.0, 0.0, 
				0.0 , 1.0, 0.0, 
				1.0 , 0.0, 0.0, 
				0.0 , 0.0, 1.0, 
			};

			float colours[] = {
		  //    1.0 , 0.0, 0.0,   // red
		  //    0.0 , 1.0, 0.0,   // green
		  //    0.0 , 0.0, 1.0,   // blue
		  //    0.0 , 0.0, 0.0, 

				139.0/256.0,  90.0/256.0,  0.0,
						0.0, 150.0/256.0, 70.0/256.0,
						0.0, 150.0/256.0, 70.0/256.0,
				139.0/256.0,  90.0/256.0,  0.0,

				139.0/256.0,  90.0/256.0,  0.0,
						0.0, 150.0/256.0, 70.0/256.0,
						0.0, 150.0/256.0, 70.0/256.0,
				139.0/256.0,  90.0/256.0,  0.0,
			};

			// here is the tick, each corner has a single normal
			// flat buffers are passed without interpolation along the GPU 
			// with the value of the first/last vertex which trigered the rutine
			unsigned indexes[]{
				// front
				0,1,2,  // a
				3,0,2,  // b

				// back
				5,4,7,  // c
				6,5,7,  // d

				3,2,6,  // e
				7,3,6,  // f

				3,7,0,  // g
				7,4,0,  // h

				5,1,4,  // i
				1,0,4,  // j

				2,1,5,  // k
				6,2,5   // l
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
			glBufferData (GL_ARRAY_BUFFER, 3*8 * sizeof (float), normals, GL_STATIC_DRAW);

			unsigned int index_ibo = 0;
			glGenBuffers (1, &index_ibo);
			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_ibo);
			glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indexes), indexes, GL_STATIC_DRAW);

			unsigned int vao = 0;
			glGenVertexArrays (1, &vao);
			glBindVertexArray (vao);

			glBindBuffer (GL_ARRAY_BUFFER, quad_vbo);
			glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
			glBindBuffer (GL_ARRAY_BUFFER, color_vbo);
			glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
			glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
			glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

			glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_ibo);

			glEnableVertexAttribArray (0); // vertex
			glEnableVertexAttribArray (1); // color
			glEnableVertexAttribArray (2); // normals
		  
			return vao;
		}

} // anonimous namespace

unsigned DrawNode::getNewCubeVAO(){
    return initializeCube();
}

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
