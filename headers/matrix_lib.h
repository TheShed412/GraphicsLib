#ifndef MAT_LIB
#define MAT_LIB

#define COL_SIZE 4
#define ROW_SIZE 4
#define MAT_LENGTH 16

typedef struct {
    float x, y, z, w;
} vec4;

typedef struct {
    vec4 col1, col2, col3, col4;
} mat4;

mat4* scalar_mult(const mat4* matrix, float scalar);

#endif