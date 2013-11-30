
#include "shader.h"

#include <GL/glew.h> 

#include <assert.h>

namespace {

    const char* vertex_shader =
        "#version 400\n"
        "layout(location = 0) in vec3 vtx_pos;"
        "layout(location = 1) in vec3 vtx_color;"
        "layout(location = 2) in vec3 vtx_normal;"
        "uniform mat4 PV;"  // perspective*view
        "uniform mat4 M;"   // model
        "out vec3 color;"
        "flat out vec4 normal;"
        "void main () {"
        "  color = vtx_color;"
        "  normal = M * vec4(vtx_normal, 0.0);"
        "  gl_Position =  PV*M* vec4 (vtx_pos, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 400\n"
        "in  vec3 color;"
        "flat in vec4 normal;"
        "out vec4 frag_color;"
        "void main () {"
        "  vec4 tmp = normalize(normal);"
        "  vec3 N = tmp.xyz; "
        "  frag_color = vec4 ( (N+color)/2.0, 1.0);"
        "}";

    std::string getShaderTypeName (GLenum t){
        switch(t){
            case GL_COMPUTE_SHADER:
                return std::string("GL_COMPUTE_SHADER");

            case GL_VERTEX_SHADER:
                return std::string("GL_VERTEX_SHADER");

            case GL_TESS_CONTROL_SHADER:
                return std::string("GL_TESS_CONTROL_SHADER");

            case GL_TESS_EVALUATION_SHADER:
                return std::string("GL_TESS_EVALUATION_SHADER");

            case GL_GEOMETRY_SHADER:
                return std::string("GL_GEOMETRY_SHADER");

            case GL_FRAGMENT_SHADER:
                return std::string("GL_FRAGMENT_SHADER");
        }
        return "unknow";
    }

    bool is_compiled(unsigned shader, GLenum type){
        int params = -1;
        glGetShaderiv (shader, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params) {
            std::cout << " == " << getShaderTypeName(type) << " compilation failed == " << std::endl;
            int max_length = 2048;
            int actual_length = 0;
            char log[2048];
            glGetShaderInfoLog (shader, max_length, &actual_length, log);
            std::cout << log << std::endl;
            return false; 
        }
        return true;
    }

    bool is_valid (unsigned prog) {
        glValidateProgram (prog);
        int params = -1;
        glGetProgramiv (prog, GL_VALIDATE_STATUS, &params);
        if (GL_TRUE != params) {
            std::cout << " == shader program not valid == " << std::endl;
            int max_length = 2048;
            int actual_length = 0;
            char log[2048];
            glGetProgramInfoLog (prog, max_length, &actual_length, log);
            std::cout << log << std::endl;
            return false;
        }
        return true;
    }

    unsigned compileProgram (unsigned vs, unsigned fs){

        // link program
        unsigned id = glCreateProgram ();
        glAttachShader (id, fs);
        glAttachShader (id, vs);
        glLinkProgram (id);
        assert(is_valid(id));

        return id;
    }

    unsigned compileShader(const char* buffer, GLenum type){
        // compile shaders
        unsigned int sh = glCreateShader (type);
        glShaderSource (sh, 1, &buffer, NULL);
        glCompileShader (sh);
        assert(is_compiled(sh,type));
        return sh;
    }

} // anonimous namespace
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

Shader::Shader()
    :name ("uninitialized"){
}
Shader::Shader(const std::string& name)
:name(name){

    if (name == "default"){
        unsigned vs = compileShader (vertex_shader, GL_VERTEX_SHADER);
        unsigned fs = compileShader (fragment_shader, GL_FRAGMENT_SHADER);
        id = compileProgram(vs, fs); 
    }
    else {

    }
}

Shader::Shader(const Shader& o)
    :name(o.name), id(id){
}

Shader::~Shader(){
}

unsigned Shader::getId()const {
    return id;
}
