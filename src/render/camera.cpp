#include "camera.h"


Camera::Camera (const vec3& pos, const vec3& lookAt, const vec3& up, const Renderer& renderer)
    : pos(pos), up(up), lookAt(lookAt), renderer(renderer), camMat (identity_mat4()){

    // build the right camera possition matrix
    //
    //
    camMat = look_at (pos, lookAt, up);
    //camMat[12] = -pos[0];
    //camMat[13] = -pos[1];
    //camMat[14] = -pos[2];
}
Camera::~Camera(){
}


void Camera::move  (const vec3& p){
    pos += p;
    camMat = look_at (pos, lookAt, up);
}
void Camera::rotate(const vec3& r){
}
