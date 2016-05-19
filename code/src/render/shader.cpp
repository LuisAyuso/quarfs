
#include "render/shader.h"
#include "render/utils.h"

#include <iostream>
#include <fstream>
#include <GL/glew.h> 
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <cassert>

namespace fs = boost::filesystem;


namespace {

    const char* vertex_shader =
        "#version 400\n"
        "layout(location = 0) in vec3 vtx_pos;"
        "layout(location = 1) in vec3 vtx_color;"
        "layout(location = 2) in vec3 vtx_normal;"
        "layout(location = 3) in vec3 vtx_trans;"
        "uniform mat4 PV;"  // perspective*view
        "out vec3 color;"
        "flat out vec4 normal;"
        "void main () {"
        "  color = vtx_color;"
        "  normal = vec4(vtx_normal, 0.0);"
        "  gl_Position =  PV* vec4 (vtx_pos+vtx_trans, 1.0);"
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
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
namespace quarfs {

Shader::Shader(const std::string& shader_path, const std::string& name)
:name(name){

    unsigned vs(0), fs(0);
    if (name == "default" || name.empty()){
        vs = compileShader (vertex_shader, GL_VERTEX_SHADER);
        fs = compileShader (fragment_shader, GL_FRAGMENT_SHADER);
        id = linkProgram(vs, fs); 
    }
    else {

        // find and compile vertex shader
        fs::path veFile = shader_path + "/" + name+".vs.glsl";
        assert (fs::exists(veFile) && "no fragments shader");
        const char* buff = loadShader(veFile);
        vs = compileShader(buff, GL_VERTEX_SHADER);
        assert(is_compiled(vs,GL_VERTEX_SHADER));
        delete[] buff;

        // find and compile fragment shader
        fs::path fraFile = shader_path + "/" + name+".fs.glsl";
        assert (fs::exists(fraFile) && "no fragments shader");
        buff = loadShader(fraFile);
        fs = compileShader(buff, GL_FRAGMENT_SHADER);
        assert(is_compiled(fs,GL_FRAGMENT_SHADER));
        delete[] buff;
        
        // create program
        id = linkProgram(vs, fs); 
        assert(is_valid(id));

        // register files to update modifications
        FileHandler::getInstance().registerHandle(fraFile.string(), this);
        FileHandler::getInstance().registerHandle(veFile.string(), this);
    }

    int shnum;
    glGetProgramiv(id, GL_ATTACHED_SHADERS, &shnum);
    assert(allAllright());
    std::cout << "loaded shader set: " << name << " with " << shnum << " shaders" << std::endl;
    int uniforms;
    GLint size; 
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniforms);
    assert(allAllright());
    for (int i =0; i < uniforms; i+=size){
        char name[128];
        GLenum type;
        glGetActiveUniform (id, i, 128, NULL, &size, &type, name);
    assert(allAllright());
        std::cout << "\tuniform: " << name << " size: " << size << std::endl;
    }
    assert(allAllright());
//    int attirbutes;
//    glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &attirbutes);
//    for (unsigned i =0; i < attirbutes; i++){
//        char name[128];
//        GLenum type;
//        glGetActiveUniform (id, i, 128, NULL, &size, &type, name);
//        std::cout << "\tattribute: " << name << " size " << size << std::endl;
//    }
//    assert(allAllright());

}

Shader::~Shader(){
}

unsigned Shader::getId()const {
    return id;
}

std::string Shader::getName()const {
    return name;
}

void Shader::notifiyModification(const std::string& fname){
    std::cout << "file " << fname << " modified! " << std::endl;
    // what kind of shader is it?  ignore, recompose both paths and compile again
    
    std::string verFile = fname;
    std::string fraFile = fname;
    //"vs.glsl" has 7 chars
    verFile.replace (verFile.end()-7, verFile.end(), std::string("vs.glsl"));
    fraFile.replace (fraFile.end()-7, fraFile.end(), std::string("fs.glsl"));

    std::cout <<  verFile << std::endl;
    std::cout <<  fraFile << std::endl;
    
    // load, compile... 
    fs::path verPath = verFile;
    assert (fs::exists(verPath) && "no fragments shader");
    const char* buff = loadShader(verPath);
    unsigned vs = compileShader(buff, GL_VERTEX_SHADER);
    delete[] buff;

    fs::path fraPath = fraFile;
    assert (fs::exists(fraPath) && "no fragments shader");
    buff = loadShader(fraPath);
    unsigned fs = compileShader(buff, GL_FRAGMENT_SHADER);
    delete[] buff;
    
    // create program
    unsigned newid = linkProgram(vs, fs); 

    // we load and compile the others as well (could be cached)
    if (is_valid(newid)){
    
        // link, if everithing alright, change and delete previous.
        unsigned sh[8];
        int size;
        glGetAttachedShaders (id, 8, &size, sh);
        for (int i =0; i< size; ++i) glDeleteShader(sh[i]);
        glDeleteProgram(id);

        id = newid;
    }
}

////////////////////////////////////////////////////////////////////////////////////
/////////////////////////   Shader tools  /////////////////////////
////////////////////////////////////////////////////////////////////////////////////

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
    return id;
}

unsigned compileShader(const char* buffer, GLenum type){
    // compile shaders
    unsigned int sh = glCreateShader (type);
    glShaderSource (sh, 1, &buffer, NULL);
    glCompileShader (sh);
    is_compiled (sh, type);
    return sh;
}

} //namespace quarfs {
