#pragma once

#include "render/renderer.h"
#include "world/world.h"
#include "input/listener.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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



template <typename T>
void shotFrame(const Camera& cam, const T& elem){
    // retrieve possition and update transform matrix
    const glm::vec3& pos = elem.getPos();
    cam.renderer.applyCorrection (pos);
    elem.draw();
}




//template<>
//inline void shotFrame(const Camera& cam, const TreeNode& tree) {
//
//
//    struct DrawVisitor : public DrawingVisitor <DrawVisitor, DrawNode>{
//        const Camera& cam;
//        DrawVisitor(const Camera& cam) : cam(cam) {}
//        void visitElem (const DrawNode& elem){
//            shotFrame(cam, elem);
//        }
//    } vis(cam);
//    vis.traverseTree(tree);
//}
//
