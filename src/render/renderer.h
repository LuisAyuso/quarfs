#pragma once

#include <glm/glm.hpp>

#include "shader.h"


class Renderer{

    // shaders
    Shader shader_program;
    
    // other objects
    glm::mat4 projection;
    glm::mat4 camera;

    public:

        Renderer();
        Renderer(const Renderer&) = delete;
        Renderer(const Renderer&&) = delete;
        ~Renderer();

        /**
         * dont execute the initialization until context is ready
         */
        void init(float w, float h);

        void setPerspective (float w, float h);
        
        void beginDraw()const;

        void updateCamera (const glm::mat4& camera);

        /**
         * performs a transformation int the matrix
         *  acording to the formula:  Perspective*camera*transform*
         */
        void applyCorrection (const glm::vec3& pos);

        void configureRender(float w, float h);
};
