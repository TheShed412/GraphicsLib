#ifndef MAT_LIB
#define MAT_LIB

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#define COL_SIZE 4
#define ROW_SIZE 4
#define MAT_LENGTH 16

#define X 0
#define Y 1
#define Z 2
#define W 3

#define COL1 0
#define COL2 1
#define COL3 2
#define COL4 3

typedef struct {
    GLfloat vec[COL_SIZE];
} vec4;

typedef struct {
    vec4 cols[ROW_SIZE];
} mat4;

mat4* mat4_mult(const mat4* left_op, const mat4* right_op);

mat4* scalar_mult(const mat4* matrix, GLfloat scalar);

GLfloat vec_mult(const vec4* left, const vec4* right);

vec4* vec_from_row(const mat4* matrix, int row);

void set_vec_zeros(vec4* dirty_vec);

void set_mat_zeroes(mat4* dirty_mat);

mat4* make_mat4();

void arr_to_mat4(GLfloat float_arr[4][4], mat4* dest_mat);

GLfloat determinant(const mat4* matrix);

void mat4_to_arr(const mat4* mat, GLfloat dest_arr[4][4]);

GLfloat** deter3_from_array(const GLfloat mat4_arr[4][4], int skip_row);

GLfloat determinant3x3(GLfloat** deter3);

mat4* mat4_of_minors(const mat4* matrix);

GLfloat** minor_matrix(const mat4* matrix, int row_skip, int col_skip);

mat4* cofactor(const mat4* matrix);

#endif