#pragma once

#include "renderer.h"
#include "utils/matrix.h"
#include "world.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows

class Camera{

    vec3 pos, inclination, lookAt;
    const Renderer& renderer;

    mat4 camMat;

public:
    Camera (const vec3& pos, const vec3& inclination, const vec3& lookAt, const Renderer& renderer);
    ~Camera();

    void move  (const vec3& pos);
    void rotate(const vec3& pos);
    void transform(const vec3& pos);

    // we make friends, so we can use the insides
template <typename T>
friend void shotFrame(const Camera& cam, const T& elem);
};


template <typename T>
void shotFrame(const Camera&, const T&);

template<>
inline void shotFrame(const Camera& cam, const TreeNode& tree) {
    struct DrawVisitor : public DrawingVisitor <DrawVisitor, DrawNode>{
        const Camera& cam;
        DrawVisitor(const Camera& cam) : cam(cam) {}
        void visitElem (const DrawNode& elem){
            shotFrame(cam, elem);
        }
    } vis(cam);
    vis.traverseTree(tree);
}


template <typename T>
void shotFrame(const Camera& cam, const T& elem){
    // retrieve possition and update transform matrix
    const vec3& pos = elem.getPos();
    std::cout << "print: " << pos << std::endl;

    cam.renderer.applyCorrection (cam.camMat, translate(pos));// getIdentity4()); // translate(pos));
    elem.draw();
}
