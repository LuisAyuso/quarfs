#pragma once
#include <vector>
#include <glm/glm.hpp>

struct DrawNode{
    glm::vec3 pos;
    unsigned int vao;

    DrawNode(const glm::vec3& pos);
    DrawNode(float a, float b, float c);
    const glm::vec3& getPos() const;
    void draw ()const;

    float x() const { return pos[0];}
    float y() const { return pos[1];}
    float z() const { return pos[2];}

    private:
    void initThis();
};
std::ostream& operator<<(std::ostream& out, const DrawNode& dn);


