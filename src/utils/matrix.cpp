#include "matrix.h"


mat3 getIdentity3(){
    return mat3 (1.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 1.0f);
}

mat4 getIdentity4(){
    return mat4 (1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f);
}

mat3 translate (const vec2& v){
    auto&& m = getIdentity3();
    m[6] = v[0];
    m[7] = v[1];
    return m;
}

mat4 translate (const vec3& v){
    auto&& m = getIdentity4();
    m[12] = v[0];
    m[13] = v[1];
    m[14] = v[2];
    return m;
}
