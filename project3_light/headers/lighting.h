#ifndef LIGHT
#define LIGHT

typedef struct {
    vec4 reflect_ambient;
    vec4 reflect_diffuse;
    vec4 reflect_specular;
    GLfloat shine;
} material;

#endif