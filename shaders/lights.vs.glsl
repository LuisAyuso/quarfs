#version 400

// not interpolated normals

// inputs
layout(location = 0) in vec3 vtx_pos;
layout(location = 1) in vec3 vtx_color;
layout(location = 2) in vec3 vtx_normal;
layout(location = 3) in vec3 vtx_trans; // instanciated

// uniforms
uniform mat4 PV;  // perspective*view
uniform vec3 lightPos;  // perspective*view

// outputs
out vec3 color;
out vec4 normal;  // flat, do not interpolate

void main () {
  color = vtx_color;
  normal = vec4(vtx_normal, 0.0);
  gl_Position =  PV* vec4 (vtx_pos+vtx_trans, 1.0);
}
