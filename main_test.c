#include <stdio.h>
#include <stdlib.h>
#include "headers/matrix_lib.h"

#define FAIL_STR ">_< Oooo daddy made a fucky wuck!"
#define PASS_STR "UwU Good Daddy passed his test!"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void assert_float(char*, float, float);

int main(int argc, const char* argv[]) {
    mat4 matrix;

    matrix.cols[COL1].vec[X] = 0.1;
    matrix.cols[COL1].vec[Y] = 0.2;
    matrix.cols[COL1].vec[Z] = 0.3;
    matrix.cols[COL1].vec[W] = 0.4;
    matrix.cols[COL4].vec[W] = 0.16;

    mat4* result = scalar_mult(&matrix, 2);

    assert_float("scalar_mult", 0.2, result->cols[0].vec[0]);
    assert_float("scalar_mult", 0.4, result->cols[0].vec[1]);
    assert_float("scalar_mult", 0.6, result->cols[0].vec[2]);
    assert_float("scalar_mult", 0.8, result->cols[0].vec[3]);
    assert_float("scalar_mult", 0.32, result->cols[3].vec[3]);
    free(result);

    vec4 left;
    vec4 right;

    left.vec[X] = 1.0;
    left.vec[Y] = 2.0;
    left.vec[Z] = 1.0;
    left.vec[W] = 1.0;

    right.vec[X] = 2.0;
    right.vec[Y] = 2.0;
    right.vec[Z] = 2.0;
    right.vec[W] = 2.0;

    float vec_result = vec_mult(&left, &right);

    assert_float("vec_mult", 10.0, vec_result);

    mat4 matrix2;
    matrix2.cols[COL1].vec[X] = 1.0;
    matrix2.cols[COL2].vec[X] = 2.0;
    matrix2.cols[COL3].vec[X] = 3.0;
    matrix2.cols[COL4].vec[X] = 4.0;

    vec4* result_vec = vec_from_row(&matrix2, X);

    assert_float("vec_from_row", result_vec->vec[X], 1.0);
    assert_float("vec_from_row", result_vec->vec[Y], 2.0);
    assert_float("vec_from_row", result_vec->vec[Z], 3.0);
    assert_float("vec_from_row", result_vec->vec[W], 4.0);

    mat4 matrix3 = (*make_mat4());
    //set_mat_zeroes(&matrix3);

    matrix3.cols[COL1].vec[X] = 2.0;
    matrix3.cols[COL2].vec[X] = 2.0;
    matrix3.cols[COL3].vec[X] = 2.0;
    matrix3.cols[COL4].vec[X] = 2.0;

    matrix3.cols[COL1].vec[W] = 2.0;
    matrix3.cols[COL2].vec[W] = 2.0;
    matrix3.cols[COL3].vec[W] = 2.0;
    matrix3.cols[COL4].vec[W] = 2.0;

    mat4* result_mat = mat4_mult(&matrix3, &matrix3);

    assert_float("mat4_mult", 8.0, result_mat->cols[COL1].vec[X]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL2].vec[X]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL3].vec[X]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL4].vec[X]);

    assert_float("mat4_mult", 0.0, result_mat->cols[COL1].vec[Y]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL2].vec[Y]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL3].vec[Y]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL4].vec[Y]);

    assert_float("mat4_mult", 0.0, result_mat->cols[COL1].vec[Z]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL2].vec[Z]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL3].vec[Z]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL4].vec[Z]);

    assert_float("mat4_mult", 8.0, result_mat->cols[COL1].vec[W]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL2].vec[W]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL3].vec[W]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL4].vec[W]);

    return 0;
}

void assert_float(char* print_string, float expected, float actual) {
    if (expected == actual){
        printf("%s%s%s: %s\n", KGRN, PASS_STR, KWHT, print_string);
    } else {
        printf("%s%s%s: %s --- expected: %f != actual: %f\n", 
        KRED, FAIL_STR, KWHT, print_string, expected, actual);
    }
}