#pragma once

#include <glm/glm.hpp>



class Surface{

    unsigned vao;
    unsigned indexCount;

public:

    Surface();

    glm::vec3 getPos() const {return glm::vec3(0.0,0.0,0.0); }
    void draw() const;
};
