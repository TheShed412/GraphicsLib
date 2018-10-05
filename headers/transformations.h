#ifndef TRANS
#define TRANS

#include "matrix_lib.h"
#include <math.h>

vec4* translation(const vec4* vector, GLfloat x, GLfloat y, GLfloat z);

vec4* rotation(const vec4* vector, GLfloat theta, int axis);

mat4* get_rotation_matrix(GLfloat theta, int axis);

#endif