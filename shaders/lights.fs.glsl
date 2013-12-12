#version 400

// not interpolated normals

// inputs
in vec4 pos;
in vec3 color;
flat in vec4 normal;

// outputs
out vec4 frag_color;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

uniform vec3 lightPos;  // perspective*view

void main () {
    
    vec4 SUN = vec4(lightPos, 1.0);
    vec4 N = normalize(normal);

    vec4 L = normalize (SUN-N);
    vec4 E = normalize (-pos);
    vec4 R = reflect (L,N);
    
    vec3 ambient = 0.4*color * ((normalize(SUN).y+2)/2.0);
    ambient = clamp (ambient, 0, 1);

    vec3 difuse = 0.6*color * max(dot(N,L), 0.0);
    difuse = clamp(difuse,0,1);

    vec3 specular = color * pow(max(dot(R,E),0.0), 0.3);
  //  if (SUN.y > 0) specular = clamp(specular,0,1);
  //  else specular = vec3(0,0,0);
    
    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    float cosTheta = clamp( dot( N,L ), 0,1 );
    frag_color =  vec4(difuse + ambient, 1.0);
}
