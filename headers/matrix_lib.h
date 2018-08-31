#ifndef MAT_LIB
#define MAT_LIB

#define COL_SIZE 4
#define ROW_SIZE 4
#define MAT_LENGTH 16

typedef struct {
    float vec[COL_SIZE];
} vec4;

typedef struct {
    vec4 cols[ROW_SIZE];
} mat4;

mat4* scalar_mult(const mat4* matrix, float scalar);

#endif