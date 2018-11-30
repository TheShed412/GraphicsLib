#ifndef SHAPES
#define SHAPES
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include "matrix_lib.h"

vec4* torus(int num_vertices, GLfloat twist, int axis);

vec4* cube();

vec4* sphere(vec4 origin, float radius);

vec4* uv_sphere(float radius, int stacks, int slices);

#endif