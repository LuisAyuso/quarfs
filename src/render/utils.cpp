#include "utils.h"

#include <GL/glew.h> 
#include <iostream> 


bool allAllright(){
    auto err = glGetError();
    if (err == GL_NO_ERROR){
        return true;
    }

    switch(err){
        case GL_INVALID_ENUM:
            std::cerr << "An unacceptable value is specified for an enumerated argument." <<
                         "The offending command is ignored " <<
                         "and has no other side effect than to set the error flag." << std::endl;
            break;
        case GL_INVALID_VALUE:
            std::cerr << "A numeric argument is out of range. " <<
                         "The offending command is ignored " << 
                         "and has no other side effect than to set the error flag." << std::endl;
            break;
        case GL_INVALID_OPERATION:
            std::cerr << "The specified operation is not allowed in the current state. " <<
                         "The offending command is ignored " << 
                         "and has no other side effect than to set the error flag." << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "The framebuffer object is not complete. The offending command " <<
                         "is ignored and has no other side effect than to set the error flag." << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            std::cerr << "There is not enough memory left to execute the command. " <<
                         "The state of the GL is undefined, " << 
                         "except for the state of the error flags, " << 
                         "after this error is recorded." << std::endl;
            break;
        case GL_STACK_UNDERFLOW:
            std::cerr << "An attempt has been made to perform an operation that "  <<
                         "would cause an internal stack to underflow." << std::endl;
            break;
        case GL_STACK_OVERFLOW:
                std::cerr << "An attempt has been made to perform an operation " <<
                             "that would cause an internal stack to overflow. " << std::endl;
            break;
    }
    return false;
}

