#include <stdio.h>
#include <stdlib.h>
#include "headers/matrix_lib.h"

int main(int argc, const char* argv[]) {
    mat4 matrix;
    matrix.col1.x = 0.1;
    matrix.col1.y = 0.2;
    matrix.col1.z = 0.3;
    matrix.col1.w = 0.4;
    matrix.col4.w = 0.16;

    mat4* result = scalar_mult(&matrix, 2);

    printf("result col1 x: %f\n", result->col1.x);
    printf("result col1 y: %f\n", result->col1.y);
    printf("result col1 z: %f\n", result->col1.z);
    printf("result col1 w: %f\n", result->col1.w);
    printf("result col4 w: %f\n", result->col4.w);

    free(result);
}