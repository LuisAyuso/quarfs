#pragma once

#include <iostream>
#include <GL/glew.h> 
#include "utils/fileHandler.h"

namespace quarfs {

class Shader : private FileListener{
    std::string name;
    unsigned id;

    // from the file listener
    void notifiyModification(const std::string&);

public:

    /**
     * constructs a shader object, loads the data from a file
     * compiles and validates
     */
    Shader(const std::string& path, const std::string& name);

    Shader(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader operator=(const Shader&) = delete;
    Shader operator=(Shader&&) = delete;

    ~Shader();

    unsigned getId()const ;
    std::string getName()const ;
};



std::string getShaderTypeName (GLenum t);
bool is_compiled(unsigned shader, GLenum type);
bool is_valid (unsigned prog) ;


unsigned compileShader(const char* buffer, GLenum type);

namespace{

    unsigned linkShader2Program (unsigned program){
        return program;
    }
    template <typename ... ARGS>
    unsigned linkShader2Program (unsigned program, unsigned shader, ARGS... shaders){
        if (shader) glAttachShader (program, shader);
        return linkShader2Program (program, shaders...);
    }
}

template <typename ... ARGS>
unsigned linkProgram (ARGS... shaders){

    // link program
    unsigned id = glCreateProgram ();
    linkShader2Program(id, shaders...);
    glLinkProgram (id);
    return id;
}


 } //namespace quarfs {



