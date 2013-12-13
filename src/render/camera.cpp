#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


Camera::Camera (const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, Renderer& renderer)
    : pos(pos), up(up), lookAt(lookAt), renderer(renderer), camMat (){
        update();
}
Camera::~Camera(){
}

void Camera::update(){
    camMat = glm::lookAt (pos, lookAt, up);
    renderer.updateCamera(camMat);
}

void Camera::keyUp(){
    glm::vec3 to = lookAt-pos;
    to = glm::normalize(to);
    pos.z += 0.1;
    lookAt.z += 0.1;
    update();
}
void Camera::keyDown(){
    pos.z -= 0.1;
    lookAt.z -= 0.1;
    update();
}
void Camera::keyRight(){
    pos.x -= 0.1;
    lookAt.x -= 0.1;
    update();
}
void Camera::keyLeft(){
    pos.x += 0.1;
    lookAt.x += 0.1;
    update();
}
void Camera::mouseDiff(float x, float y){

    glm::vec3 to = lookAt-pos;
    to = glm::normalize(to);
    to = glm::rotateX(to, y);
    to = glm::rotateY(to, x);

    lookAt = to+pos;
    update();

   // std::cout << x << "," << y << std::endl;
}
