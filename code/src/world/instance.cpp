
#include "world/instance.h"

namespace quarfs {

Instance::Instance(){
}

void Instance::addCube(float x, float y, float z){
    pos.push_back(glm::vec3(x,y,z));
}

void Instance::update(){
}

void Instance::draw()const{
}

} //namespace quarfs {
