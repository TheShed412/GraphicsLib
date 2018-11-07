#ifndef MAZ_HLP
#define MAZ_HLP

#include "matrix_lib.h"

#define VERTS_IN_CUBE 36

typedef struct {
    GLboolean north_wall;
    GLboolean south_wall;
    GLboolean east_wall;
    GLboolean west_wall;
} cell;

typedef struct
{
    GLfloat x;
    GLfloat y;
} vec2;

typedef struct {
    vec4 pos_vert;
    vec2 tex_vert;
} pos_tex;

vec4* single_cube();

GLubyte*** get_texture(const char* file_name);

void print_cell(const cell* cell_print);

cell** make_maze();

#endif