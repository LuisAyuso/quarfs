#pragma once

#include <iostream>
#include "utils/fileHandler.h"




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
