

#pragma once
#include <iostream>
#include "drawable_node.h"



struct DrawNode{
    float x,y,z;
    DrawNode(float x, float y, float z)
     :   x(x), y(y), z(z)
    {}
    void draw ()const;
};


class Storage : public OcNode<DrawNode>{

    public: 
        Storage ()
            : OcNode<DrawNode>()
        {}

};

class World{

    Storage nodeTree;

public:

    World();
    ~World();

    bool update();
    void draw() const;

};
