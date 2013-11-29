#pragma once

struct InputListener {
    virtual void keyUp(){}
    virtual void keyDown() {}
    virtual void keyLeft() {}
    virtual void keyRight() {}

    virtual void spacebar(){}

    virtual void mouseDiff(double x, double y) {}
};
