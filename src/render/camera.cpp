#include "camera.h"


Camera::Camera (const vec3& pos, const vec3& inclination, const vec3& lookAt, const mat4& perspectiveMatrix, int matrixBuffer)
    : pos(pos), inclination(inclination), lookAt(lookAt), 
      perspectiveMatrix(perspectiveMatrix), transformationMatrixBuffer(matrixBuffer) {

    // build the right camera possition matrix

}
Camera::~Camera(){
}


void Camera::move  (const vec3& pos){
}
void Camera::rotate(const vec3& pos){
}
void Camera::transform(const vec3& pos){
}

