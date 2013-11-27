#pragma once
#include <vector>
#include "utils/matrix.h"

struct DrawNode{
    vec3 pos;
    unsigned int vao;

    DrawNode(const vec3& pos);
    DrawNode(float a, float b, float c);
    const vec3& getPos() const;
    void draw ()const;

    float x() const { return pos[0];}
    float y() const { return pos[1];}
    float z() const { return pos[2];}

    private:
    void initThis();
};
std::ostream& operator<<(std::ostream& out, const DrawNode& dn);

