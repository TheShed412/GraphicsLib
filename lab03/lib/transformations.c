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

static mat4* x_rotate(GLfloat theta) {
    mat4* rot_mat = copy_id();

    GLfloat cos_theta = cos(theta);
    GLfloat sin_theta = sin(theta);

    rot_mat->cols[COL2].vec[Y] = cos_theta;
    rot_mat->cols[COL3].vec[Y] = sin_theta * -1;
    rot_mat->cols[COL2].vec[Z] = sin_theta;
    rot_mat->cols[COL3].vec[Z] = cos_theta;

    return rot_mat;
}

static mat4* y_rotate(GLfloat theta) {
    mat4* rot_mat = copy_id();

    GLfloat cos_theta = cos(theta);
    GLfloat sin_theta = sin(theta);

    rot_mat->cols[COL1].vec[X] = cos_theta;
    rot_mat->cols[COL1].vec[Z] = sin_theta * -1;
    rot_mat->cols[COL3].vec[X] = sin_theta;
    rot_mat->cols[COL3].vec[Z] = cos_theta;

    return rot_mat;
}

static mat4* z_rotate(GLfloat theta) {
    mat4* rot_mat = copy_id();

    GLfloat cos_theta = cos(theta);
    GLfloat sin_theta = sin(theta);

    rot_mat->cols[COL1].vec[X] = cos_theta;
    rot_mat->cols[COL2].vec[X] = sin_theta * -1;
    rot_mat->cols[COL1].vec[Y] = sin_theta;
    rot_mat->cols[COL2].vec[Y] = cos_theta;

    return rot_mat;
}

vec4* rotation(const vec4* vector, GLfloat theta, int axis) {
    mat4* rot_mat;
    vec4* ret_vec;

    switch(axis) {
        case X:
            rot_mat = x_rotate(theta);
            ret_vec = mat_mult_vec(rot_mat, vector);
            break;
        case Y:
            rot_mat = y_rotate(theta);
            ret_vec = mat_mult_vec(rot_mat, vector);
            break;
        case Z:
            rot_mat = z_rotate(theta);
            ret_vec = mat_mult_vec(rot_mat, vector);
            break;
    }

    return ret_vec;
}