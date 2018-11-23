#ifndef MAZ_HLP
#define MAZ_HLP

#include "matrix_lib.h"

#define VERTS_IN_CUBE 36

enum cardinal{NORTH, SOUTH, EAST, WEST};

enum state {ORBITING, ORBIT_STOPPED, MOVING_TO_MAZE, MOVING_FORWARD, TURNING_RIGHT, TURNING_LEFT, SOLVED};

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

enum texture{GRASS, BRICK, PILLAR};

vec4* single_cube();

GLubyte*** get_texture(const char* file_name);

void print_cell(const cell* cell_print);

cell** make_maze();

pos_tex* single_cube_texture(enum texture tex_type);

vec2* get_tex_verts(const pos_tex* tex_pos, int size);

vec4* get_pos_verts(const pos_tex* tex_pos, int size);

GLubyte*** get_textures();

pos_tex* empty_cube_arr(int num_cubes);

vec4* ground();

pos_tex* ground_with_tex();

pos_tex* wall_with_tex();

pos_tex* pillar_with_tex();

pos_tex* translate_pos_verts(pos_tex* tex_pos, int num_verts, GLfloat x, GLfloat y, GLfloat z);

pos_tex* wall_with_pillar(GLboolean make_wall) ;

int get_total_verts();

pos_tex* rotate_pos_verts(pos_tex* tex_pos, int num_verts, GLfloat rot);

cell* get_cell(cell** maze, int x, int y, int size);

enum state* right_hand_rule(cell** maze, int size);

#endif