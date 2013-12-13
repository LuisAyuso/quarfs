#include <iostream>
#include <assert.h>
#include <GL/glew.h> 

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "light.h"
#include "shader.h"
#include "utils.h"


namespace{
    char shadowCasterVS[] = 
        "#version 400\n"
        "layout(location = 0) in vec3 vtx_pos;"
        "layout(location = 1) in vec3 vtx_color;"
        "layout(location = 2) in vec3 vtx_normal;"
        "layout(location = 3) in vec3 vtx_trans; // instanciated"
        "uniform mat4 PV;"
        "void main () {"
        "        gl_Position = PV * vec4 (vtx_pos+vtx_trans, 1.0);"
        "}";

    char shadowCasterFS[] = 
        "#version 400\n"
        "void main () {"
        "}";
}


ShadowCaster::ShadowCaster() : shadow_size(256){
    unsigned vs = compileShader(shadowCasterVS, GL_VERTEX_SHADER);
    assert( is_compiled(vs, GL_VERTEX_SHADER));
    unsigned fs = compileShader(shadowCasterFS, GL_FRAGMENT_SHADER);
    assert( is_compiled(fs, GL_FRAGMENT_SHADER));
    shaderPogram = linkProgram (vs, fs);
    assert( is_valid (shaderPogram) );

    // create framebuffer
    glGenFramebuffers (1, &fb);
    glBindFramebuffer (GL_FRAMEBUFFER, fb);
    
    // create texture for framebuffer
    glGenTextures (1, &fb_tex);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, fb_tex);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                    shadow_size, shadow_size, 0,
                    GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glBindFramebuffer (GL_FRAMEBUFFER, 0);
    assert(allAllright());
}

void ShadowCaster::setupFrame(const glm::vec3& sun){
    // generate V P
    //view is sun to the center of the universe
    glm::mat4 camera = glm::lookAt (sun, glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,1.0,0.0));

    // since is the sun lets make it orthographic
    glm::mat4 projection = glm::ortho(-400.0f,400.0f, -400.0f, 400.0f);
    glm::mat4 PV    = projection * camera; 

    // bind framebuffer that renders to texture instead of screen
    glBindFramebuffer (GL_FRAMEBUFFER, fb);
    // set the viewport to the size of the shadow map
    glViewport (0, 0, shadow_size, shadow_size);
    // clear the shadow map to black (or white)
    glClearColor (0.0, 0.0, 0.0, 1.0);
    // no need to clear the colour buffer
    glClear (GL_DEPTH_BUFFER_BIT);

    // bind out shadow-casting shader from the previous section
    glUseProgram (shaderPogram);
    // send in the view and projection matrices from the light
    int matrix_location = glGetUniformLocation (shaderPogram, "PV");
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, glm::value_ptr(PV));
    assert(allAllright());
}
void ShadowCaster::endFrame(){
}
