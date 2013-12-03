#version 400

// inputs
in  vec3 color;
flat in vec4 normal;

// outputs
out vec4 frag_color;

void main () {
  vec4 tmp = normalize(normal);
  vec3 N = tmp.xyz; 
  frag_color = vec4 ( abs(N/2.0), 1.0);
}
