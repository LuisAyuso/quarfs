#pragma once



#include <glm/glm.hpp>

class Light{

};




struct ShadowCaster{
    int shadow_size;
    unsigned int fb;
    unsigned int fb_tex;
    unsigned shaderPogram;

    ShadowCaster();
    void setupFrame(const glm::vec3& sun);
    void endFrame();
};
