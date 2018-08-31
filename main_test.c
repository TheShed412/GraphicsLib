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
    matrix.col1.x = 0.1;
    matrix.col1.y = 0.2;
    matrix.col1.z = 0.3;
    matrix.col1.w = 0.4;
    matrix.col4.w = 0.16;

    mat4* result = scalar_mult(&matrix, 2);

    assert_float("scalar_mult", 0.2, result->col1.x);
    assert_float("scalar_mult", 0.4, result->col1.y);
    assert_float("scalar_mult", 0.6, result->col1.z);
    assert_float("scalar_mult", 0.8, result->col1.w);
    assert_float("scalar_mult", 0.32, result->col4.w);
    free(result);

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