#pragma once
#include <vector>


struct DrawNode{
    float x,y,z;
    DrawNode(float x, float y, float z);
    void draw ()const;
};
std::ostream& operator<<(std::ostream& out, const DrawNode& dn);


