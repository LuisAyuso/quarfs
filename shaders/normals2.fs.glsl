#version 400

// not interpolated normals

// inputs
in  vec3 color;
in vec4 normal;

// outputs
out vec4 frag_color;

void main () {
  vec4 tmp = normalize(normal);
  vec3 N = tmp.xyz; 
  frag_color = vec4 ( N, 1.0);
}
