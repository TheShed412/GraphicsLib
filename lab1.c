#include <stdio.h>
#include <stdlib.h>
#include "headers/matrix_lib.h"

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
                

    return 0;
}