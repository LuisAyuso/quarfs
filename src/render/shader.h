#pragma once

#include <iostream>




class Shader{
    std::string name;
    unsigned id;

public:

    Shader();
    /**
     * constructs a shader object, loads the data from a file
     * compiles and validates
     */
    Shader(const std::string& name);
    Shader(const Shader&);
    ~Shader();

    unsigned getId()const ;
    std::string getName()const ;

};
