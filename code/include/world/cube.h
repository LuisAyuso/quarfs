#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace quarfs{

struct DrawNode{

    glm::vec3 pos;
    unsigned int vao;

    DrawNode(const glm::vec3& pos);
    DrawNode(float a, float b, float c);
    const glm::vec3& getPos() const;
    void draw ()const;
    unsigned getVao() const { return vao;}

    float x() const { return pos[0];}
    float y() const { return pos[1];}
    float z() const { return pos[2];}

    private:
    void initThis();
};

unsigned getNewCubeVAO();

} //namespace quarfs{

namespace std{
    ostream& operator<<(ostream& out, const quarfs::DrawNode& dn);
}


