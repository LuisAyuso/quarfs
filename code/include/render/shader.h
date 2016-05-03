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
    Shader(const std::string& name);

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
unsigned linkProgram (unsigned vs, unsigned fs);
unsigned compileShader(const char* buffer, GLenum type);


 } //namespace quarfs {



