#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera (const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, const Renderer& renderer)
    : pos(pos), up(up), lookAt(lookAt), renderer(renderer), camMat (){

    // build the right camera possition matrix
    camMat = glm::lookAt (pos, lookAt, up);
    //camMat[12] = -pos[0];
    //camMat[13] = -pos[1];
    //camMat[14] = -pos[2];
}
Camera::~Camera(){
}


void Camera::move  (const glm::vec3& p){
    pos += p;
    camMat = glm::lookAt (pos, lookAt, up);
}
void Camera::rotate(const glm::vec3& r){
}
