
#include "surface.h"
#include "noise.h"
#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h> // include GLEW and new version of GL on Windows



#define MAT(m,x,y) m[x+ (mapSide*y)]

namespace {
    struct vert_t{
        float x,y,z;
        vert_t (): x(0), y(0), z(0) {}
        vert_t (float x, float y, float z): x(x), y(y), z(z) {}
        void operator= (const glm::vec3& v){
            x = v.x;
            y = v.y;
            z = v.z;
        }
        bool operator== (const vert_t& o){
            return (x==o.x && z==o.z);
        }
    };

    std::ostream& operator << (std::ostream& out, const vert_t& v){
        out << "[" << v.x << "," << v.y << ","<< v.z << "]";
        return out;
    }

    /**
     * look for vertex in the list 
     */
    unsigned getIndex(vert_t l[], unsigned size, const vert_t& v){
        for (unsigned i=0; i< size; ++i){
            if (l[i] == v)
                return i;
        }
        std::cout << v << std::endl;
        assert(false&& "look for weird");
        return 0;
    }

}

///////////////////////////////////////////////////////////////
//////  THIS SHIT ONLY DRAWS A PLANE OR CONTINUOS, can not make cubes
///////////////////////////////////////////////////////////////
Surface::Surface(){

    unsigned mapSide = 200;
    unsigned numVerts = mapSide*mapSide;
    float halfSize = mapSide/2.0;

    vert_t points[numVerts];
    assert(sizeof(vert_t) == 3*sizeof(float));

    //////////////////////////////////////////////////////////////////////////
    //  Buid vertex

    // build cloud of points
    for (unsigned i = 0; i < mapSide; ++i){
        for (unsigned j = 0; j < mapSide; ++j){
            MAT(points, i,j) = glm::vec3((i-halfSize),  //-mapSide/2)-(step/2.0),
                                          perlin(i,j),
                                         (j-halfSize));  //-mapSide/2)-(step/2.0),
        }
    }

    //for (auto& a : points)
    //    std::cout << a << std::endl;
    //std::cout << " ========== "<< std::endl;
    
    //////////////////////////////////////////////////////////////////////////
    //  Index triangles (brute force, who cares!)
    
    // build indexes to render them from avobe (just cubes, 2 triangles each)
    //   number is (i-1)*(j-1)*2 and each triangle has 3 vertex
    indexCount = (mapSide-1)*(mapSide-1)*2*3;
    std::vector<unsigned> indexVect;
    // first 2 loops make the (i-1)*(j-1)
    for (unsigned i = 0; i < mapSide-1; ++i){
        for (unsigned j = 0; j < mapSide-1; ++j){
            // push 6 vertex (2 triangles)
            
            // counter clock wise
            indexVect.push_back(getIndex(points, numVerts, vert_t(i-halfSize,   7.0, j-halfSize  )));
            indexVect.push_back(getIndex(points, numVerts, vert_t(i-halfSize,   7.0, j+1-halfSize)));
            indexVect.push_back(getIndex(points, numVerts, vert_t(i+1-halfSize, 7.0, j-halfSize  )));
                                                                                   
            indexVect.push_back(getIndex(points, numVerts, vert_t(i-halfSize,   7.0, j+1-halfSize)));
            indexVect.push_back(getIndex(points, numVerts, vert_t(i+1-halfSize, 7.0, j+1-halfSize)));
            indexVect.push_back(getIndex(points, numVerts, vert_t(i+1-halfSize, 7.0, j-halfSize  )));

            // Clock Wise
         // indexVect.push_back(getIndex(points, numVerts, vert_t(i-halfSize,   0.0, j-halfSize  )));
         // indexVect.push_back(getIndex(points, numVerts, vert_t(i+1-halfSize, 0.0, j-halfSize  )));
         // indexVect.push_back(getIndex(points, numVerts, vert_t(i-halfSize,   0.0, j+1-halfSize)));
         //                                                                         
         // indexVect.push_back(getIndex(points, numVerts, vert_t(i-halfSize,   0.0, j+1-halfSize)));
         // indexVect.push_back(getIndex(points, numVerts, vert_t(i+1-halfSize, 0.0, j-halfSize  )));
         // indexVect.push_back(getIndex(points, numVerts, vert_t(i+1-halfSize, 0.0, j+1-halfSize)));
        }
    }

    assert(indexVect.size() == indexCount);
    unsigned indexes[indexCount];
    for (unsigned i=0; i<indexCount; ++i)
        indexes[i] = indexVect[i];

    //std::cout << " ========== "<< std::endl;
    //for (auto& a : indexVect)
    //    std::cout << a << std::endl;

    //////////////////////////////////////////////////////////////////////////
    //  normals 
    vert_t normals[numVerts];
    for( auto& a : normals)
        a = vert_t(0.0,1.0,0.0);

    //////////////////////////////////////////////////////////////////////////
    //  color 
    vert_t colors[numVerts];
    for( auto& a : colors)
        a = vert_t(159.0/255.0,182.0/255.0,205.0/255.0);

    assert(sizeof (colors) == sizeof(normals));
    assert(sizeof (points) == sizeof(normals));
    assert(sizeof(normals) == 3*numVerts*sizeof(float));

    std::cout << "created a Surface " << mapSide << "x" << mapSide << std::endl;
   // std::cout << " Points " << std::endl;
   // for (auto& a : points)
   //     std::cout << "\t" << a << std::endl;
   // std::cout << " index " << std::endl;
   // for (auto& a : indexes)
   //     std::cout << "\t" << a << std::endl;

    //////////////////////////////////////////////////////////////////////////
    //  load to GPU
    unsigned int vert_vbo = 0;
    glGenBuffers (1, &vert_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vert_vbo);
    glBufferData (GL_ARRAY_BUFFER, 3*numVerts*sizeof(float), &points, GL_STATIC_DRAW);

    unsigned int color_vbo = 0;
    glGenBuffers (1, &color_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, color_vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (colors), &colors, GL_STATIC_DRAW);

    unsigned int normals_vbo = 0;
    glGenBuffers (1, &normals_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
    glBufferData (GL_ARRAY_BUFFER, sizeof (normals), &normals, GL_STATIC_DRAW);

    unsigned int index_ibo = 0;
    glGenBuffers (1, &index_ibo);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, index_ibo);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indexes), &indexes, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);

    vao = 0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);

    glBindBuffer (GL_ARRAY_BUFFER, vert_vbo);
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
    //glEnableVertexAttribArray (3);
    assert(glGetError() == GL_NO_ERROR);

    glBindVertexArray (0);
}


void Surface::draw()const{
    glBindVertexArray (vao);
    glDrawElements (GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray (0);
}
