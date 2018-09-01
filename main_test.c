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