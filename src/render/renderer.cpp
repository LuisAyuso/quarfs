#include <iostream>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.h"

namespace {

    const char* vertex_shader =
        "#version 400\n"
        "layout(location = 0) in vec3 vtx_pos;"
        "layout(location = 1) in vec3 vtx_color;"
        "uniform mat4 MPV;"
        "out vec3 color;"
        "void main () {"
        "  color = vtx_color;"
        "  gl_Position =  MPV * vec4 (vtx_pos, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 400\n"
        "in  vec3 color;"
        "out vec4 frag_color;"
        "void main () {"
        "  frag_color = vec4 (color, 1.0);"
        "}";

    bool is_compiled(unsigned shader){
        int params = -1;
        glGetShaderiv (shader, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params) {
            std::cout << " == shader compilation failed == " << std::endl;
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

} // anonimous namespace


Renderer::Renderer() 
: projection(glm::mat4()){
}

Renderer::~Renderer(){
    // clean up renders?
}

void Renderer::init(float w, float h){
    compileShaders();
    configureRender(w, h);
}

void Renderer::setPerspective (float w, float h){
    projection = glm::perspective(45.0f, w / h, 0.1f, 100.0f);
}

void Renderer::beginDraw()const{
    // wipe the drawing surface clear
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup Matrix
    int matrix_location = glGetUniformLocation (shader_program, "MPV");
    glUseProgram (shader_program);
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE,  glm::value_ptr(glm::mat4()));
    glUseProgram (shader_program);
}


void Renderer::updateCamera (const glm::mat4& cam){
    camera = cam;
}

void Renderer::applyCorrection (const glm::vec3& pos){

    glm::mat4 model  = glm::translate(glm::mat4(), pos);
    glm::mat4 MVP    = projection * camera * model; 

    int matrix_location = glGetUniformLocation (shader_program, "MPV");
    glUseProgram (shader_program);
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, glm::value_ptr(MVP));
}


void Renderer::compileShaders(){
    unsigned int vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
    assert(is_compiled(vs));
    unsigned int fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);
    assert(is_compiled(fs));

    shader_program = glCreateProgram ();
    glAttachShader (shader_program, fs);
    glAttachShader (shader_program, vs);
    glLinkProgram (shader_program);
    assert(is_valid(shader_program));
}

void Renderer::configureRender(float w, float h){
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

    glEnable (GL_CULL_FACE); // cull face
    glCullFace (GL_BACK); // cull back face
    glFrontFace (GL_CCW); // is default CCW 

    projection = glm::perspective(45.0f, w / h, 0.1f, 100.0f);
}

