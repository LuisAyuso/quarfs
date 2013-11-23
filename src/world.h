

#pragma once


class World{

    float rotTri;
    float rotQuad;

    public:

    World():
        rotTri(0.f), rotQuad(0.f)
    {}

    bool update();
    void draw() const;
};
