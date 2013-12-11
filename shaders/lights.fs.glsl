#version 400

// not interpolated normals

// inputs
in vec3 color;
in vec4 normal;

// outputs
out vec4 frag_color;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

uniform vec3 lightPos;  // perspective*view

void main () {

  vec4 tmp = normalize(normal);

  // Cosine of the angle between the normal and the light direction,
  // clamped above 0
  //  - light is at the vertical of the triangle -> 1
  //  - light is perpendicular to the triangle -> 0
  //  - light is behind the triangle -> 0
    

  float cosTheta = clamp( dot( tmp.xyz,lightPos ), 0,1 );
  frag_color =  vec4( cosTheta*color, 1.0);
}
