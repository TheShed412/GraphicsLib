#include <stdio.h>
#include <stdlib.h>
#include "headers/matrix_lib.h"



int main(int argc, const char* argv[]) {

    vec4 v1 = {1, 2, 3, 4};
    vec4 v2 = {5, 6, 7, 8};

    /* each row is actually a column */
    mat4 m1 = {1, -5,  9,   13,
               2,  6, -10,  14,
               3,  7,  11,  15,
               4,  8,  12, -16};
    
    mat4 m2 = {4,  8,  12,  16,
               3,  7,  11,  15,
               2,  6,  10,  14,
               1,  5,  9,   13};

    GLfloat s = 3.0;

    vec4* vec_scalar_mult_result = scalar_mult_vec4(&v1, s);
    
    vec4* vec_add_result = vec_add(&v1, &v2);

    vec4* vec_sub_result = vec_sub(&v1, &v2);

    GLfloat vec_mult_result = vec_mult(&v1, &v2);

    vec4* vec_cross_result = vec_cross(&v1, &v2);

    mat4* mat_scalar_mult = scalar_mult(&m1, s);

    mat4* mat_add_result = mat_add(&m1, &m2);

    return 0;
}