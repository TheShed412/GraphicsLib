#include <stdio.h>
#include <stdlib.h>
#include "headers/matrix_lib.h"

void print_vec4(const vec4* vector) {
    GLfloat v_x = vector->vec[X];
    GLfloat v_y = vector->vec[Y];
    GLfloat v_z = vector->vec[Z];
    GLfloat v_w = vector->vec[W];
    printf("| %.1f |\n", v_x);
    printf("| %.1f |\n", v_y);
    printf("| %.1f |\n", v_z);
    printf("| %.1f |\n", v_w);
}

void print_mat4(const mat4* matrix) {
    for (int i=0; i < COL_SIZE; i ++) {
        for (int j=0; j < ROW_SIZE; j++) {
            printf("%.1f\t", matrix->cols[j].vec[i]);
        }
        printf("\n");
    }
}

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

    mat4* mat_sub_result = mat_sub(&m1, &m2);

    mat4* mat_mult_result = mat4_mult(&m1, &m2);

    mat4* mat_inverse_reslut = inverse_mat4(&m1);

    mat4* mat_transpose_result = transpose_mat4(&m1);

    vec4* mat_mult_vec_result = mat_mult_vec(&m1, &v1);

    printf("Scalar times vector:\n");
    print_vec4(vec_scalar_mult_result);
    printf("\n\n Vector Addition:\n");
    print_vec4(vec_add_result);
    printf("\n\n Vector Subtraction:\n");
    print_vec4(vec_sub_result);
    printf("\n\n Vector Dot Product:\n");
    printf("%.1f\n", vec_mult_result);
    printf("\n\n Vector Cross Product:\n");
    print_vec4(vec_cross_result);
    printf("\n\n Matrix Scalar Multiplication:\n");
    print_mat4(mat_scalar_mult);
    printf("\n\n Matrix Addition:\n");
    print_mat4(mat_add_result);
    printf("\n\n Matrix Subtraction:\n");
    print_mat4(mat_sub_result);
    printf("\n\n Matrix Multiplication:\n");
    print_mat4(mat_mult_result);
    printf("\n\n Matrix Inverse:\n");
    print_mat4(mat_inverse_reslut);
    printf("\n\n Matrix Transpose:\n");
    print_mat4(mat_transpose_result);
    printf("\n\n Matrix Vector Multiplication:\n");
    print_vec4(mat_mult_vec_result);

    return 0;
}
