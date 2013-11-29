#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera (const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, Renderer& renderer)
    : pos(pos), up(up), lookAt(lookAt), renderer(renderer), camMat (){
        update();
}
Camera::~Camera(){
}

void Camera::update(){
    camMat = glm::lookAt (pos, lookAt, up);
}

void Camera::keyUp(){
    pos.y += 0.1;
    lookAt.y += 0.1;
    update();
}
void Camera::keyDown(){
    pos.y -= 0.1;
    lookAt.y -= 0.1;
    update();
}
void Camera::keyRight(){
    pos.x += 0.1;
    lookAt.x += 0.1;
    update();
}
void Camera::keyLeft(){
    pos.x -= 0.1;
    lookAt.x -= 0.1;
    update();
}
void Camera::mouseDiff(double x, double y){
}
