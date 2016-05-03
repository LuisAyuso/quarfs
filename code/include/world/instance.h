#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace quarfs{

struct Instance{

    std::vector<glm::vec3> pos;

    Instance();

    void addCube(float x, float y, float z);
    void update();
    void draw()const;
};


} //namespace quarfs{
