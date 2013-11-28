#pragma once

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>



class Renderer{

    // shaders
    unsigned shader_program;
    
    // other objects
    glm::mat4 projection;
    mutable glm::mat4 lastUsedMatrix;

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
        /**
         * performs a transformation int the matrix
         *  acording to the formula:  Perspective*camera*transform*
         */
        void applyCorrection (const glm::mat4& camera, const glm::vec3& pos) const;

        void configureRender(float w, float h);

    private:
        // setup
        void compileShaders();
};
