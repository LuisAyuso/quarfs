# include "renderer.h"
#include <assert.h>


namespace {

    const char* vertex_shader =
        "#version 400\n"
        "layout(location = 0) in vec3 vtx_pos;"
        "layout(location = 1) in vec3 vtx_color;"
        "uniform mat4 modelMatrix;"
        "out vec3 color;"
        "void main () {"
        "  color = vtx_color;"
        "  gl_Position = modelMatrix * vec4 (vtx_pos, 1.0);"
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
: perspectiveMatrix(getIdentity4()){
}

Renderer::~Renderer(){
    // clean up renders?
}

void Renderer::init(){
    compileShaders();
    configureRender();
}

void Renderer::setPerspective (float  fovy,  float  aspect,  float  zNear,  float  zFar){
}

void Renderer::beginDraw()const{
    // wipe the drawing surface clear
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup Matrix
    int matrix_location = glGetUniformLocation (shader_program, "modelMatrix");
    glUseProgram (shader_program);
    glUniformMatrix4fv (matrix_location, 1, GL_FALSE, getIdentity4());
            //translate(vec3(-0.5f,0.0f,0.0f)));
    glUseProgram (shader_program);
}

void Renderer::applyCorrection (const mat4& camera, const mat4& transform) const{
    
    mat4 matrix = transform; //transform*(camera*perspective);

    if (matrix != lastUsedMatrix){
        int matrix_location = glGetUniformLocation (shader_program, "modelMatrix");
        glUseProgram (shader_program);
        glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
    }
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

void Renderer::configureRender(){
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

    glEnable (GL_CULL_FACE); // cull face
    glCullFace (GL_BACK); // cull back face
    glFrontFace (GL_CW); // GL_CCW for counter clock-wise

    // do some nasty matrix stuff and build a nice perspective one
    perspectiveMatrix = getIdentity4();
}
