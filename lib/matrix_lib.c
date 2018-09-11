#include <stdio.h>
#include <stdlib.h>
#include "../headers/matrix_lib.h"

static void free_arr(float*** _arr, int size) {
    float** arr = *_arr;
    for(int i = 0; i < size; i++) {
        free(arr[i]);
    }
    free(arr);
}

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
    mat4* result = (mat4*) calloc(1, sizeof(mat4));
    return result;
}

void arr_to_mat4(float float_arr[4][4], mat4* dest_mat) {

    for(int col_i = 0; col_i < COL_SIZE; col_i++) {
        for(int row = 0; row < ROW_SIZE; row++) {
            dest_mat->cols[col_i].vec[row] = float_arr[row][col_i];
        }
    }

}

void mat4_to_arr(const mat4* mat, float dest_arr[4][4]) {

    for(int col_i = 0; col_i < COL_SIZE; col_i++) {
        for(int row = 0; row < ROW_SIZE; row++) {
            dest_arr[row][col_i] = mat->cols[col_i].vec[row];
        }
    }

}

float** deter3_from_array(const float mat4_arr[4][4], int skip_row){
    
    float** deter3_arr = (float**) malloc(3 * sizeof(float*));
    for(int i = 0; i < 3; i++) {
        deter3_arr[i] = (float*) calloc(3, sizeof(float));
    }

    int d_j;
    for(int i = 1; i < ROW_SIZE; i++) {
        d_j = 0;
        for(int j = 0; j < COL_SIZE; j++) {
            if (j != skip_row) {
                deter3_arr[d_j][i-1] = mat4_arr[j][i];
                d_j++;
            }
        }
    }

    return deter3_arr;
}

/* something is fucky with this */
float determinant3x3(float** deter3) {

    float a11 = deter3[0][0];
    float a12 = deter3[0][1];
    float a13 = deter3[0][2];

    float a21 = deter3[1][0];
    float a22 = deter3[1][1];
    float a23 = deter3[1][2];

    float a31 = deter3[2][0];
    float a32 = deter3[2][1];
    float a33 = deter3[2][2];

    float positive = (a11*a22*a33) + (a12*a23*a31) + (a21*a32*a13);
    float negative = (a13*a22*a31) + (a21*a12*a33) + (a23*a32*a11);

    return positive - negative;
}

float determinant(const mat4* matrix) {

    /* values infront of the 3x3 determinants */
    float val1 = matrix->cols[COL1].vec[X];
    float val2 = matrix->cols[COL1].vec[Y];
    float val3 = matrix->cols[COL1].vec[Z];
    float val4 = matrix->cols[COL1].vec[W];

    float matrix_array[4][4];
    mat4_to_arr(matrix, matrix_array);

    float** deter1_arr = deter3_from_array(matrix_array, X);
    float** deter2_arr = deter3_from_array(matrix_array, Y);
    float** deter3_arr = deter3_from_array(matrix_array, Z);
    float** deter4_arr = deter3_from_array(matrix_array, W);

    float deter1 = determinant3x3(deter1_arr);
    float deter2 = determinant3x3(deter2_arr);
    float deter3 = determinant3x3(deter3_arr);
    float deter4 = determinant3x3(deter4_arr);

    float deter;
    deter = (val1*deter1) - (val2*deter2) + (val3*deter3) - (val4*deter4);

    free_arr(&deter1_arr, 3);
    free_arr(&deter2_arr, 3);
    free_arr(&deter3_arr, 3);
    free_arr(&deter4_arr, 3);
    return deter;
}

float** minor_matrix(const mat4* mat4_matrix, int row_skip, int col_skip) {
    float** minor = (float**) malloc(3 * sizeof(float*));
    for(int i = 0; i < 3; i++) {
        minor[i] = (float*) calloc(3, sizeof(float));
    }

    int m_j;
    int m_i = 0;
    for(int i = 0; i < ROW_SIZE; i++) {
        m_j = 0;
        for(int j = 0; j < COL_SIZE; j++) {
            if (j != row_skip && i != col_skip) {
                minor[m_j][m_i] = mat4_matrix->cols[i].vec[j];
                m_j++;
            }
        }
        if (i != col_skip) {
            m_i++;
        }
    }

    return minor;
}

mat4* mat4_of_minors(const mat4* matrix) {
    mat4* result = make_mat4();
    float** minor3;
    float deter3;

    for (int col_i = 0; col_i < COL_SIZE; col_i++){
        for (int row_i = 0; row_i < ROW_SIZE; row_i++){
            minor3 = minor_matrix(matrix, row_i, col_i);
            deter3 = determinant3x3(minor3);
            result->cols[col_i].vec[row_i] = deter3;
        }
    }

    return result;
}

mat4* cofactor(const mat4* matrix) {
    mat4* result = make_mat4();

    result = mat4_of_minors(matrix);

    int invert = 1;
    for (int i = 0; i < COL_SIZE; i++){
        for (int j = 0; j < ROW_SIZE; j++) {
            result->cols[i].vec[j] *= invert;
            invert *= -1;
        }
        invert *= -1;
    }

    return result;
}