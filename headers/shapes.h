#ifndef SHAPES
#define SHAPES
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include "matrix_lib.h"

vec4* torus(int num_vertices, GLfloat twist, int axis);

vec4* cube();

#endif