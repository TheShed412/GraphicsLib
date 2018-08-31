#include <stdio.h>
#include <stdlib.h>

#define COL_SIZE 4
#define ROW_SIZE 4
#define MAT_LENGTH 16

typedef struct {
    float x, y, z, w;
} vec4;

typedef struct {
    vec4 col1, col2, col3, col4;
} mat4;

mat4* pointer_func(const mat4* matrix) {
    mat4* result = (mat4*) malloc(sizeof(mat4));

    result->col1.x = matrix->col1.x;
    result->col1.y = matrix->col1.y;
    result->col1.z = matrix->col1.z;
    result->col1.w = matrix->col1.w;

    return result;
}

int main(int argc, const char* argv[]) {
    mat4 matrix;
    matrix.col1.x = 0.1;
    matrix.col1.y = 0.2;
    matrix.col1.z = 0.3;
    matrix.col1.w = 0.4;

    mat4* result = pointer_func(&matrix);

    printf("result col1 x: %f\n", result->col1.x);
    printf("result col1 y: %f\n", result->col1.y);
    printf("result col1 z: %f\n", result->col1.z);
    printf("result col1 w: %f\n", result->col1.w);
}