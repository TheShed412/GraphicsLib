#ifndef TRANS
#define TRANS

#include "matrix_lib.h"
#include <math.h>

vec4* translation(const vec4* vector, GLfloat x, GLfloat y, GLfloat z);

vec4* rotation(const vec4* vector, GLfloat theta, int axis);

mat4* get_rotation_matrix(GLfloat theta, int axis);

vec4* scale(const vec4* vector, GLfloat x, GLfloat y, GLfloat z);

mat4* get_scaling_matrix(const vec4* vector, GLfloat x, GLfloat y, GLfloat z);

mat4* get_scaling_matrix_con(GLfloat scale_factor);

mat4* get_translation_matrix(GLfloat x, GLfloat y, GLfloat z);

mat4* get_rotation_matrix2(const vec4* dest, int axis, GLfloat theta);

vec4* translate_vertices(vec4* vertices, int num_vertices, GLfloat x, GLfloat y, GLfloat z);

vec4* scale_vertices(vec4* vertices, int num_vertices, GLfloat x, GLfloat y, GLfloat z);

vec4* const_scale(vec4* vertices, int num_vertices, GLfloat scale_fac);

vec4* rotate_vertices(vec4* vertices, int num_vertices, GLfloat theta, int axis);

vec4* my_normalize(const vec4* vector);

#endif