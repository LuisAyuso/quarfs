#pragma once

#include <GL/glew.h> // include GLEW and new version of GL on Windows

#include "utils/matrix.h"




class Renderer{

    // shaders
    unsigned shader_program;
    
    // other objects
    mat4 perspectiveMatrix;
    mutable mat4 lastUsedMatrix;

    public:

        Renderer();
        Renderer(const Renderer&) = delete;
        Renderer(const Renderer&&) = delete;
        ~Renderer();

        /**
         * dont execute the initialization until context is ready
         */
        void init();

        /**
         * just like the old gluprespective
         */
        void setPerspective (float  fovy,  float  aspect,  float  zNear,  float  zFar);
        
        void beginDraw()const;
        /**
         * performs a transformation int the matrix
         *  acording to the formula:  Perspective*camera*transform*
         */
        void applyCorrection (const mat4& camera, const mat4& transform) const;


    private:
        // setup
        void compileShaders();
        void configureRender();
};
