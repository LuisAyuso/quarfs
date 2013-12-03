#include <iostream>
#include <assert.h>


#include <GL/glew.h> 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.h"




Renderer::Renderer() 
:  currentShader(0), projection(glm::mat4()){
}

Renderer::~Renderer(){
    // clean up renders?
}

void Renderer::init(float w, float h){
    shader_programs.push_back(Shader("color"));
    shader_programs.push_back(Shader("normals"));
    shader_programs.push_back(Shader("normals2"));
    configureRender(w, h);
}

void Renderer::setPerspective (float w, float h){
    projection = glm::perspective(45.0f, w / h, 0.1f, 100.0f);
}

void Renderer::beginDraw()const{
    // wipe the drawing surface clear
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup Matrix
    int matrix_location = glGetUniformLocation (shader_programs[currentShader].getId(), "MPV");
    glUseProgram (shader_programs[currentShader].getId());
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE,  glm::value_ptr(glm::mat4()));
    glUseProgram (shader_programs[currentShader].getId());
}


void Renderer::updateCamera (const glm::mat4& cam){
    camera = cam;
    glm::mat4 PV    = projection * camera; 
    int matrix_location = glGetUniformLocation (shader_programs[currentShader].getId(), "PV");
    assert(glGetError() == GL_NO_ERROR);
    glUseProgram (shader_programs[currentShader].getId());
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, glm::value_ptr(PV));
}

void Renderer::applyCorrection (const glm::vec3& pos){
    glm::mat4 model  = glm::translate(glm::mat4(), pos);
    int matrix_location = glGetUniformLocation (shader_programs[currentShader].getId(), "M");
    glUseProgram (shader_programs[currentShader].getId());
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, glm::value_ptr(model));
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

void Renderer::spacebar(){
    currentShader ++;
    if (currentShader == shader_programs.size())
        currentShader = 0;
    std::cout << "usign shader: " << shader_programs[currentShader].getName() << std::endl;
    glUseProgram (shader_programs[currentShader].getId());
    glm::mat4 PV    = projection * camera; 
    int matrix_location = glGetUniformLocation (shader_programs[currentShader].getId(), "PV");
    assert(glGetError() == GL_NO_ERROR);
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, glm::value_ptr(PV));
}
