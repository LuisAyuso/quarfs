#include "matrix.h"


mat4 getIdentity4(){
    return mat4 (1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

mat3 getIdentity3(){
    return mat3 (1.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 1.0f);
}

mat4 translate (const vec3& v){
    auto&& m = getIdentity4();
    return m;
}

mat3 translate (const vec2& v){
    auto&& m = getIdentity3();
    return m;
}
