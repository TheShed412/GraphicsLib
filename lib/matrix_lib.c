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
    set_vec_zeros(&row_vec);
    set_vec_zeros(&col_vec);

    /* loop is iterating through the rows */
    for(int row_i = 0; row_i < ROW_SIZE; row_i++) {
        row_vec = (*vec_from_row(left_op, row_i));

        for(int col_j = 0; col_j < COL_SIZE; col_j++) {
            col_vec = right_op->cols[col_j];
            float point = vec_mult(&row_vec, &col_vec);
            result->cols[col_j].vec[row_i] = point;
        }
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

void set_vec_zeros(vec4* dirty_vec) {
    for (int i = 0; i < COL_SIZE; i ++) {
        dirty_vec->vec[i] = 0.0;
    }
}

void set_mat_zeroes(mat4* dirty_mat) {
    
    for (int i = 0; i < ROW_SIZE; i++){
        set_vec_zeros(&dirty_mat->cols[i]);
    }
}

mat4* make_mat4() {
    mat4* result = (mat4*) malloc(sizeof(mat4));
    set_mat_zeroes(result);
    return result;
}