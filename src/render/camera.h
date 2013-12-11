#pragma once

#include "render/renderer.h"
#include "world/world.h"
#include "input/listener.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <set>
#include <vector>


class Camera : public InputListener{

    glm::vec3 pos, up, lookAt;
    Renderer& renderer;

    glm::mat4 camMat;

public:
    Camera (const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, Renderer& renderer);
    ~Camera();

    // from input listener
    void keyUp();
    void keyDown();
    void keyLeft();
    void keyRight();
    void mouseDiff(float x, float y);
    void update();

private:

    // we make friends, so we can use the insides
template <typename T>
friend void shotFrame(const Camera& cam, const T& elem);
};


//
//template <typename T>
//void shotFrame(const Camera& cam, const T& elem){
//    // retrieve possition and update transform matrix
//    const glm::vec3& pos = elem.getPos();
//    cam.renderer.applyCorrection (pos);
//    elem.draw();
//}
//
//


//template<>
//inline void shotFrame(const Camera& cam, const TreeNode& tree) {
//    struct DrawVisitor : public DrawingVisitor <DrawVisitor, DrawNode>{
//        const Camera& cam;
//        DrawVisitor(const Camera& cam) : cam(cam) {}
//        void visitElem (const DrawNode& elem){
//            shotFrame(cam, elem);
//        }
//    } vis(cam);
//    vis.traverseTree(tree);
//}


/// instanciated drawwing of the tree
template<>
inline void shotFrame(const Camera& cam, const TreeNode& tree) {

    static unsigned numElems = 0;
    static unsigned vao = 0;

    if (!numElems){
        std::vector<glm::vec3> posList;
        std::set<unsigned> vaos;
        struct DrawVisitor : public DrawingVisitor <DrawVisitor, DrawNode>{
            std::vector<glm::vec3>& list;
            std::set<unsigned>& vaos;
            DrawVisitor(std::vector<glm::vec3>& list, std::set<unsigned>& vaos) 
                : list(list), vaos(vaos) {}
            void visitElem (const DrawNode& elem){
                list.push_back(elem.getPos());
                vaos.insert(elem.getVao());
            }
        } vis(posList, vaos);
        vis.traverseTree(tree);

        numElems =  posList.size();

        float positions[numElems*3];
        float* index = positions;
        for(auto e: posList){
            index[0] = e.x;
            index[1] = e.y;
            index[2] = e.z;
            index +=3;
        }

        // this might be smarter in the future, but now, only one figure
        assert(vaos.size()==1);

        // need to load the vao
        vao = *vaos.begin();

        glBindVertexArray (vao);

        unsigned ibo;
        glGenBuffers(1, &ibo);  // instances buffer object
        glBindBuffer(GL_ARRAY_BUFFER, ibo);

        // fill with data
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*numElems, positions, GL_STATIC_DRAW);

        // set up generic attrib pointers
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));

        // a attrib divisor of 1 means that attribute 2 will advance once
        // every instance (0 would mean once per vertex)
        glVertexAttribDivisor(3, 1);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        assert(glGetError() == GL_NO_ERROR);
    }

    assert(vao);
    glBindVertexArray (vao);
    glDrawElementsInstanced(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0, numElems);
}

