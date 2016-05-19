#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include "render/shader.h"


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
}

TEST(Shader, compile){

    EXPECT_TRUE(quarfs::compileShader(vertex_shader, GL_VERTEX_SHADER));
    EXPECT_FALSE(quarfs::compileShader(vertex_shader, GL_FRAGMENT_SHADER));

}
