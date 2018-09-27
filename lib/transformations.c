#include <stdio.h>
#include <stdlib.h>
#include "../headers/transformations.h"

mat4 IDENTITY_MATRIX = {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};


static mat4* copy_id(){
    mat4* ret_matrix = (mat4*)calloc(1, sizeof(mat4));
    for(int i=0; i < COL_SIZE; i ++) {
        for(int j=0; j<ROW_SIZE; j ++) {
            ret_matrix->cols[i].vec[j] = IDENTITY_MATRIX.cols[i].vec[j];
        }
    }

    return ret_matrix;
}

vec4* translation(const vec4* vector, GLfloat x, GLfloat y, GLfloat z) {
    
    mat4* trans_mat = copy_id();
    trans_mat->cols[COL4].vec[X] = x;
    trans_mat->cols[COL4].vec[Y] = y;
    trans_mat->cols[COL4].vec[Z] = z;

    vec4* result_vec = mat_mult_vec(trans_mat, vector);

    free(trans_mat);
    return result_vec;
}