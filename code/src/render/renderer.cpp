#include <iostream>
#include <assert.h>


#include <GL/glew.h> 

#define GLM_FORCE_RADIANS 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render/renderer.h"
#include "render/utils.h"
#include "render/shader.h"


namespace{
    char debugvs[] = 
        "#version 400\n"
        "in vec2 vp, vt;"
        "out vec2 st;"
        "void main () {"
        "    st = vt;"
        "    gl_Position = vec4 (vp, 0.0, 1.0);"
        "}";
    char debugfs[] = 
        "#version 400\n"
        "in vec2 st;"
  //      "uniform sampler2D depth_tex;"
        "out vec4 frag_colour;"
        "void main () {"
        //"   frag_colour = texture (depth_tex, st);"
        "   frag_colour = vec4 (0.0, 0.0, 0.0, 1.0);"
        "}";
}

namespace quarfs{

    struct DebugQuad{
        unsigned shaderPogram;
        unsigned vao;

        DebugQuad(){
            // compile shader:
            unsigned vs = compileShader(debugvs, GL_VERTEX_SHADER);
            assert( is_compiled(vs, GL_VERTEX_SHADER));
            unsigned fs = compileShader(debugfs, GL_FRAGMENT_SHADER);
            assert( is_compiled(fs, GL_FRAGMENT_SHADER));
            shaderPogram = linkProgram (vs, fs);
            assert( is_valid (shaderPogram) );

            // create display surface
            float quad_screen_cordinates[] = {
                0.5, 0.5,
                1.0, 0.5,
                1.0, 1.0,
                1.0, 1.0,
                0.5, 1.0,
                0.5, 0.5
            };

            float texture_map[] = {
                0.0, 0.0,
                1.0, 0.0,
                1.0, 1.0,
                1.0, 1.0,
                0.0, 1.0,
                0.0, 0.0
            };
            
            unsigned int ver_vbo = 0;
            glGenBuffers (1, &ver_vbo);
            glBindBuffer (GL_ARRAY_BUFFER, ver_vbo);
            glBufferData (GL_ARRAY_BUFFER, 2*6 * sizeof (float), quad_screen_cordinates, GL_STATIC_DRAW);

            unsigned int tex_vbo = 0;
            glGenBuffers (1, &tex_vbo);
            glBindBuffer (GL_ARRAY_BUFFER, tex_vbo);
            glBufferData (GL_ARRAY_BUFFER, 2*6 * sizeof (float), texture_map, GL_STATIC_DRAW);

            glGenVertexArrays (1, &vao);
            glBindVertexArray (vao);

            glBindBuffer (GL_ARRAY_BUFFER, ver_vbo);
            glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
            glBindBuffer (GL_ARRAY_BUFFER, tex_vbo);
            glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

            glEnableVertexAttribArray (0); // vertex
            glEnableVertexAttribArray (1); // texture
        }

        void draw(){
            glUseProgram (shaderPogram);
            glBindVertexArray (vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray (0);
        }

        ~DebugQuad(){
            // cleanup
        }
    };



Renderer::Renderer() 
:  currentShader(0), projection(glm::mat4()){
}

Renderer::~Renderer(){
    // clean up renders?
    std::cout << "delete renderer" << std::endl;
    delete debugScreen;
  //	delete shadowCaster;
}

void Renderer::init(float w, float h){
    currentCount = 0;
    shader_programs.push_back(std::shared_ptr<Shader>(new Shader("lights")));
  //  shader_programs.push_back(std::shared_ptr<Shader>(new Shader("color")));
  //  shader_programs.push_back(std::shared_ptr<Shader>(new Shader("normals")));
  //  shader_programs.push_back(std::shared_ptr<Shader>(new Shader("normals2")));
    assert(allAllright());
    configureRender(w, h);
    assert(allAllright());

    debugScreen = new DebugQuad();
  //  shadowCaster = new ShadowCaster();
}

void Renderer::setPerspective (float w, float h){
    projection = glm::perspective(45.0f, w / h, 0.1f, 1000.0f);
}

void Renderer::beginDraw()const{
    // wipe the drawing surface clear
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    currentCount ++;
}

void Renderer::endDraw()const{

    // this woudl be the place to draw everithing else

    // draw the debug quad
    debugScreen->draw();
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

} //namespace quarfs{
