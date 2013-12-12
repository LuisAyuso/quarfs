#include <iostream>
#include <assert.h>


#include <GL/glew.h> 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.h"
#include "utils.h"




Renderer::Renderer() 
:  currentShader(0), projection(glm::mat4()){
}

Renderer::~Renderer(){
    // clean up renders?
}

void Renderer::init(float w, float h){
    currentCount = 0;
    shader_programs.push_back(std::shared_ptr<Shader>(new Shader("color")));
    shader_programs.push_back(std::shared_ptr<Shader>(new Shader("normals")));
    shader_programs.push_back(std::shared_ptr<Shader>(new Shader("normals2")));
    shader_programs.push_back(std::shared_ptr<Shader>(new Shader("lights")));
    assert(allAllright());
    configureRender(w, h);
    assert(allAllright());
}

void Renderer::setPerspective (float w, float h){
    projection = glm::perspective(45.0f, w / h, 0.1f, 1000.0f);
}

void Renderer::beginDraw()const{
    // wipe the drawing surface clear
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    assert(allAllright());
//    // setup Matrix
//    int matrix_location = glGetUniformLocation (shader_programs[currentShader].getId(), "MPV");
//    glUseProgram (shader_programs[currentShader].getId());
//    glUniformMatrix4fv (matrix_location, 1, GL_FALSE,  glm::value_ptr(glm::mat4()));
//    glUseProgram (shader_programs[currentShader].getId());
//    assert(allAllright());

    currentCount ++;
}


void Renderer::updateCamera (const glm::mat4& cam){
    camera = cam;
    glm::mat4 PV    = projection * camera; 

    int matrix_location = glGetUniformLocation (shader_programs[currentShader]->getId(), "PV");
    assert(allAllright());
    glUseProgram (shader_programs[currentShader]->getId());
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, glm::value_ptr(PV));

    // load light pos
    matrix_location = glGetUniformLocation (shader_programs[currentShader]->getId(), "lightPos");
    float lightStep = currentCount/600.0;
    glUniform3f (matrix_location, sin(lightStep)*500.0, cos(lightStep)*500.0, 0.0);
    assert(allAllright());
}

void Renderer::configureRender(float w, float h){
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    assert(allAllright());

    glEnable (GL_CULL_FACE); // cull face
    glCullFace (GL_BACK); // cull back face
    glFrontFace (GL_CCW); // is default CCW 

    projection = glm::perspective(45.0f, w / h, 0.1f, 100.0f);
    assert(allAllright());
}

void Renderer::spacebar(){
    currentShader ++;
    if (currentShader == shader_programs.size())
        currentShader = 0;
    std::cout << "usign shader: " << shader_programs[currentShader]->getName() << std::endl;

    updateCamera(camera);
}
