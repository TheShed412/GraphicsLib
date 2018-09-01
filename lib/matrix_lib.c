#include <stdio.h>
#include <stdlib.h>
#include "../headers/matrix_lib.h"

float vec_mult(const vec4* left, const vec4* right){
    float result = 0;

    for(int i = 0; i < COL_SIZE; i++) {
        result += left->vec[i] * right->vec[i]; 
    }

    return result;
}

vec4* vec_from_row(const mat4* matrix, int row){
    vec4* result = (vec4*) malloc(sizeof(vec4));

    /* looping across the columns to get the rows */
    for(int i = 0; i < COL_SIZE; i ++) {
        result->vec[i] = matrix->cols[i].vec[row];
    }

    return result;
}

mat4* mat4_mult(const mat4* left_op, const mat4* right_op){
    mat4* result = (mat4*) malloc(sizeof(mat4));
    vec4 row_vec; // vec from the left_op
    vec4 col_vec; // vec from the right_op

    /* loop is iterating through the rows */
    for(int i = 0; i < ROW_SIZE; i++) {

    }

    return result;
}

mat4* scalar_mult(const mat4* matrix, float scalar){
    mat4* result = (mat4*) malloc(sizeof(mat4));

    for (int i = 0; i < COL_SIZE; i ++){
        for (int j = 0; j < ROW_SIZE; j++){
            result->cols[i].vec[j] = matrix->cols[i].vec[j] * scalar;
        }
    }

    return result;
}