#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Instance{

    std::vector<glm::vec3> pos;

    Instance();

    void addCube(float x, float y, float z);
    void update();
    void draw()const;
};


