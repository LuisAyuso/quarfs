
#include "shader.h"

#include <iostream>
#include <fstream>
#include <GL/glew.h> 
#include <boost/filesystem.hpp>
#include <assert.h>

namespace fs = boost::filesystem;

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
        "  normal = vec4(vtx_normal, 0.0);"
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

    unsigned linkProgram (unsigned vs, unsigned fs){

        // link program
        unsigned id = glCreateProgram ();
        if (vs) glAttachShader (id, fs);
        if (fs) glAttachShader (id, vs);
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

    const char* loadShader(const fs::path& p){
        std::ifstream myfile;
        myfile.open (p.string(), std::ios::in);

        // find file size
        size_t begin = myfile.tellg();
        myfile.seekg (0, std::ios::end);
        size_t end = myfile.tellg();
        myfile.seekg (0, std::ios::beg);
        size_t size = end-begin;
        char* buffer = new char[size+1];
        myfile.read(buffer, size);
        myfile.close();
        buffer[size] = '\0';

     //   std::cout << "load Shader: " << p.string() <<" \n" <<
     //       buffer << std::endl;

        return buffer;
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

    unsigned vs(0), fs(0);
    if (name == "default" || name.empty()){
        vs = compileShader (vertex_shader, GL_VERTEX_SHADER);
        fs = compileShader (fragment_shader, GL_FRAGMENT_SHADER);
        id = linkProgram(vs, fs); 
    }
    else {
        fs::path p("./shaders");
        assert (fs::exists(p) && fs::is_directory(p));
        fs::path fraFile = "./shaders/"+name+".fs.glsl";
        assert (fs::exists(fraFile) && "no fragments shader");
        const char* buff = loadShader(fraFile);
        fs = compileShader(buff, GL_FRAGMENT_SHADER);
        delete[] buff;

        fs::path veFile = "./shaders/"+name+".vs.glsl";
        assert (fs::exists(veFile) && "no fragments shader");
        buff = loadShader(veFile);
        vs = compileShader(buff, GL_VERTEX_SHADER);
        delete[] buff;
        
        // create program
        id = linkProgram(vs, fs); 
        std::cout << "loaded shader set: " << name << std::endl;

    }
}

Shader::Shader(const Shader& o)
    :name(o.name), id(o.id){
}

Shader::~Shader(){
}

unsigned Shader::getId()const {
    return id;
}
