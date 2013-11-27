#pragma once

#include "utils/matrix.h"
#include "world.h"



class Camera{

    vec3 pos, inclination, lookAt;
    const mat4& perspectiveMatrix;
    int transformationMatrixBuffer;
    mat4 camMatrix;

public:
    Camera (const vec3& pos, const vec3& inclination, const vec3& lookAt, const mat4& perspectiveMatrix, int matrixBuffer);
    ~Camera();

    void move  (const vec3& pos);
    void rotate(const vec3& pos);
    void transform(const vec3& pos);


    // we make friends, so we can draw stuff
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
    
    // if new matrix, send to GPU
    
    // draw the object (this object, if is composed, will recursivelly call back)
    elem.draw();

    // restore old matrix
}
