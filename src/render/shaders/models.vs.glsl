#version 400

layout(location = 0) in vec3 vtx_pos; 
layout(location = 1) in vec3 vtx_color; 
layout(location = 2) in vec3 vtx_normal; 

uniform mat4 PV;   // perspective*view
uniform mat4 M;    // model

// OUTPUTS
out vec3 color; 
flat out vec4 normal;   // flat means no interpolation

void main () { 
  color = vtx_color; 
  normal = M * vec4(vtx_normal, 0.0); 
  gl_Position =  PV*M* vec4 (vtx_pos, 1.0); 
}";
