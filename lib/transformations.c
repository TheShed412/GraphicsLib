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

vec4* scale(const vec4* vector, GLfloat x, GLfloat y, GLfloat z) {
    mat4* scale_mat = copy_id();
    scale_mat->cols[COL1].vec[X] = x;
    scale_mat->cols[COL2].vec[Y] = y;
    scale_mat->cols[COL3].vec[Z] = z;

    vec4* result_vec = mat_mult_vec(scale_mat, vector);
    free(scale_mat);
    return result_vec;
}

mat4* get_translation_matrix(GLfloat x, GLfloat y, GLfloat z) {
    
    mat4* trans_mat = copy_id();
    trans_mat->cols[COL4].vec[X] = x;
    trans_mat->cols[COL4].vec[Y] = y;
    trans_mat->cols[COL4].vec[Z] = z;

    return trans_mat;
}

mat4* get_scaling_matrix_con(GLfloat scale_factor) {
    mat4* scale_mat = copy_id();
    scale_mat->cols[COL1].vec[X] = scale_factor;
    scale_mat->cols[COL2].vec[Y] = scale_factor;
    scale_mat->cols[COL3].vec[Z] = scale_factor;

    return scale_mat;
}

mat4* get_scaling_matrix(const vec4* vector, GLfloat x, GLfloat y, GLfloat z) {
    mat4* scale_mat = copy_id();
    scale_mat->cols[COL1].vec[X] = x;
    scale_mat->cols[COL2].vec[Y] = y;
    scale_mat->cols[COL3].vec[Z] = z;

    return scale_mat;
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

static mat4* y_rotate2(GLfloat vy, GLfloat vz) {
    mat4* rot_mat = copy_id();
    GLfloat d = sqrt(vy*vy + vz*vz);

    mat4 temp_mat = {1,     0,    0, 0,
                     0,  vz/d, vy/d, 0,
                     0, -vy/d, vz/d, 0,
                     0,     0,    0, 1};
    
    for(int i=0; i < COL_SIZE; i++) {
        for(int j=0; j < ROW_SIZE; j++) {
            rot_mat->cols[i].vec[j] = temp_mat.cols[i].vec[j];
        }
    }

    return rot_mat;
}

static mat4* x_rotate2(GLfloat vx, GLfloat vy, GLfloat vz) {
    mat4* rot_mat = copy_id();
    GLfloat d = sqrt(vy*vy + vz*vz);

    mat4 temp_mat = {d,  0, vx, 0,
                     0,  1,  0, 0,
                    -vx, 0,  d, 0,
                     0,  0,  0, 1};
    
    for(int i=0; i < COL_SIZE; i++) {
        for(int j=0; j < ROW_SIZE; j++) {
            rot_mat->cols[i].vec[j] = temp_mat.cols[i].vec[j];
        }
    }

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

mat4* get_rotation_matrix(GLfloat theta, int axis) {
    mat4* rot_mat;

        switch(axis) {
        case X:
            rot_mat = x_rotate(theta);
            break;
        case Y:
            rot_mat = y_rotate(theta);
            break;
        case Z:
            rot_mat = z_rotate(theta);
            break;
    }

    return rot_mat;
}

mat4* get_rotation_matrix2(const vec4* dest, int axis, GLfloat theta) {
    mat4* rot_mat;
    GLfloat vx = dest->vec[X];
    GLfloat vz = dest->vec[Z];
    GLfloat vy = dest->vec[Y];

    switch(axis) {
        case X:
            rot_mat = x_rotate2(vx, vy, vz);
            break;
        case Y:
            rot_mat = y_rotate2(vy, vz);
            break;
        case Z:
            rot_mat = z_rotate(theta);
            break;
    }

    return rot_mat;
}

vec4* translate_vertices(vec4* vertices, int num_vertices, GLfloat x, GLfloat y, GLfloat z) {
    vec4 temp;
    for (int i=0; i < num_vertices; i++) {
        temp = vertices[i];
        vertices[i] = *translation(&temp, x, y, z);
    }

    return vertices;
}

vec4* scale_vertices(vec4* vertices, int num_vertices, GLfloat x, GLfloat y, GLfloat z) {
    vec4 temp;
    for (int i=0; i < num_vertices; i++) {
        temp = vertices[i];
        vertices[i] = *scale(&temp, x, y, z);
    }

    return vertices;
}

vec4* const_scale(vec4* vertices, int num_vertices, GLfloat scale_fac) {
    return scale_vertices(vertices, num_vertices, scale_fac, scale_fac, scale_fac);
}