#include <stdio.h>
#include <stdlib.h>
#include "../headers/matrix_lib.h"

mat4* scalar_mult(const mat4* matrix, float scalar){
    mat4* result = (mat4*) malloc(sizeof(mat4));

    for (int i = 0; i < COL_SIZE; i ++){
        for (int j = 0; j < ROW_SIZE; j++){
            result->cols[i].vec[j] = matrix->cols[i].vec[j] * scalar;
        }
    }

    // result->col1.x = matrix->col1.x * scalar;
    // result->col1.y = matrix->col1.y * scalar;
    // result->col1.z = matrix->col1.z * scalar;
    // result->col1.w = matrix->col1.w * scalar;

    // result->col2.x = matrix->col2.x * scalar;
    // result->col2.y = matrix->col2.y * scalar;
    // result->col2.z = matrix->col2.z * scalar;
    // result->col2.w = matrix->col2.w * scalar;

    // result->col3.x = matrix->col3.x * scalar;
    // result->col3.y = matrix->col3.y * scalar;
    // result->col3.z = matrix->col3.z * scalar;
    // result->col3.w = matrix->col3.w * scalar;

    // result->col4.x = matrix->col4.x * scalar;
    // result->col4.y = matrix->col4.y * scalar;
    // result->col4.z = matrix->col4.z * scalar;
    // result->col4.w = matrix->col4.w * scalar;

    return result;
}