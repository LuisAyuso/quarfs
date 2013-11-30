#version 400

// inputs
in  vec3 color;

// normals come in a flat buffer, which means they dont get interpolated,
// first(last) vertex to create the shape give it a value
flat in vec4 normal;

// outputs
out vec4 frag_color;

void main () {
  vec4 tmp = normalize(normal);
  vec3 N = tmp.xyz; 
  frag_color = vec4 ( (N+color)/2.0, 1.0);
};
