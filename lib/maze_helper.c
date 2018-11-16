#include "../headers/maze_helper.h"
#include "../headers/matrix_lib.h"
#include "../headers/transformations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

enum ort {HORZ, VERT};

static int total_world_vertices = 0;

#define MAX_REC_INDEX 8

static void rec_maze_builder(cell** maze, int start_vert, int end_vert, int start_hor, int end_hor);
static void rec_maze_builder_vert(cell** maze, int start_vert, int end_vert, int start_hor, int end_hor);
static void rec_maze_builder_hor(cell** maze, int start_vert, int end_vert, int start_hor, int end_hor);
static void divide(cell** maze, int x, int y, int w, int h, enum ort orientation);

int get_total_verts() {
    return total_world_vertices;
}

static enum ort choose_ort(int w, int h) {
    if(w < h) {
        return HORZ;
    } else if(h < w) {
        return VERT;
    } else {
        int pick = rand() % 2;
        return (pick) ? VERT : HORZ;
    }
}

vec4* single_cube(){
    vec4* cube = calloc(36, 36*sizeof(vec4));

    total_world_vertices += 36;

    for(int i=0; i<36; i++) {
        cube[i].vec[W] = 1;
    }

    GLfloat x, y, z;    
    x = 0.5;
    y = 0.5;
    z = 0.5;

    /* face 1 */
    cube[0].vec[X] = -x, cube[0].vec[Y] = -y, cube[0].vec[Z] = z;//0
    cube[1].vec[X] = x, cube[1].vec[Y] = -y, cube[1].vec[Z] = z;//1
    cube[2].vec[X] = x, cube[2].vec[Y] = y, cube[2].vec[Z] = z;//3

    cube[3].vec[X] = -x, cube[3].vec[Y] = -y, cube[3].vec[Z] = z;//0
    cube[4].vec[X] = x, cube[4].vec[Y] = y, cube[4].vec[Z] = z;//3
    cube[5].vec[X] = -x, cube[5].vec[Y] = y, cube[5].vec[Z] = z;//2

    /* face 2: +x, changing z */
    cube[6].vec[X] = x, cube[6].vec[Y] = -y,  cube[6].vec[Z] = z;//1
    cube[7].vec[X] = x,  cube[7].vec[Y] = -y,  cube[7].vec[Z] = -z;//5
    cube[8].vec[X] = x,  cube[8].vec[Y] = y, cube[8].vec[Z] = -z;//7

    cube[9].vec[X] = x,  cube[9].vec[Y] = -y,   cube[9].vec[Z] = z;//1
    cube[10].vec[X] = x, cube[10].vec[Y] = y,  cube[10].vec[Z] = -z;//7
    cube[11].vec[X] = x,  cube[11].vec[Y] = y, cube[11].vec[Z] = z;//3

    /* face 3: -z */
    cube[12].vec[X] = x, cube[12].vec[Y] = -y,  cube[12].vec[Z] = -z;//5
    cube[13].vec[X] = -x,  cube[13].vec[Y] = -y,  cube[13].vec[Z] = -z;//4
    cube[14].vec[X] = -x,  cube[14].vec[Y] = y, cube[14].vec[Z] = -z;//6

    cube[15].vec[X] = x,  cube[15].vec[Y] = -y,   cube[15].vec[Z] = -z;//5
    cube[16].vec[X] = -x, cube[16].vec[Y] = y,  cube[16].vec[Z] = -z;//6
    cube[17].vec[X] = x,  cube[17].vec[Y] = y, cube[17].vec[Z] = -z;//7

    /* face 4: const -x, changing z */
    cube[18].vec[X] = -x, cube[18].vec[Y] = -y,  cube[18].vec[Z] = -z;//4
    cube[19].vec[X] = -x,  cube[19].vec[Y] = -y,  cube[19].vec[Z] = z;//0
    cube[20].vec[X] = -x,  cube[20].vec[Y] = y, cube[20].vec[Z] = z;//2

    cube[21].vec[X] = -x,  cube[21].vec[Y] = -y,cube[21].vec[Z] = -z;//4
    cube[22].vec[X] = -x, cube[22].vec[Y] = y, cube[22].vec[Z] = z;//2
    cube[23].vec[X] = -x,  cube[23].vec[Y] = y, cube[23].vec[Z] = -z;//6

    /* bottom: const -y */
    cube[24].vec[X] = -x, cube[24].vec[Y] = -y,  cube[24].vec[Z] = -z;//4
    cube[25].vec[X] = x,  cube[25].vec[Y] = -y,  cube[25].vec[Z] = -z;//5
    cube[26].vec[X] = x,  cube[26].vec[Y] = -y, cube[26].vec[Z] = z;//1

    cube[27].vec[X] = -x,  cube[27].vec[Y] = -y,cube[27].vec[Z] = -z;//4
    cube[28].vec[X] = x, cube[28].vec[Y] = -y, cube[28].vec[Z] = z;//1
    cube[29].vec[X] = -x,  cube[29].vec[Y] = -y, cube[29].vec[Z] = z;//0

    /* top: const y */
    cube[30].vec[X] = -x, cube[30].vec[Y] = y,  cube[30].vec[Z] = z;//2
    cube[31].vec[X] = x,  cube[31].vec[Y] = y,  cube[31].vec[Z] = z;//3
    cube[32].vec[X] = x,  cube[32].vec[Y] = y, cube[32].vec[Z] = -z;//7

    cube[33].vec[X] = -x,  cube[33].vec[Y] = y,cube[33].vec[Z] = z;//2
    cube[34].vec[X] = x, cube[34].vec[Y] = y, cube[34].vec[Z] = -z;//7
    cube[35].vec[X] = -x,  cube[35].vec[Y] = y, cube[35].vec[Z] = -z;//6

    return cube;
}

pos_tex* empty_cube_arr(int num_cubes) {
    int total_verts = num_cubes * VERTS_IN_CUBE;

    pos_tex* tcube = calloc(total_verts, total_verts * sizeof(pos_tex));

    for(int i=0; i<total_verts; i++) {
        tcube[i].pos_vert.vec[W] = 1;
    }

    return tcube;
}

pos_tex* single_cube_texture(enum texture tex_type){
    pos_tex* tcube = calloc(36, 36*sizeof(pos_tex));

    total_world_vertices += 36;

    for(int i=0; i<36; i++) {
        tcube[i].pos_vert.vec[W] = 1;
    }

    GLfloat x, y, z;    
    x = 0.5;
    y = 0.5;
    z = 0.5;

    vec2 bot_rt, bot_lt, top_rt, top_lt;

    switch(tex_type) {
        case GRASS:
            top_lt.x = 0.0, top_lt.y = 0.5;
            top_rt.x = 0.5, top_rt.y = 0.5;
            bot_lt.x = 0.0, bot_lt.y = 1.0;
            bot_rt.x = 0.5, bot_rt.y = 1.0; 
        break;
        
        case PILLAR:
            top_lt.x = 0.5, top_lt.y = 0.0;
            top_rt.x = 1.0, top_rt.y = 0.0;
            bot_lt.x = 0.5, bot_lt.y = 0.5;
            bot_rt.x = 1.0, bot_rt.y = 0.5;
        break;

        case BRICK:
            top_lt.x = 0.0, top_lt.y = 0.0;
            top_rt.x = 0.5, top_rt.y = 0.0;
            bot_lt.x = 0.0, bot_lt.y = 0.5;
            bot_rt.x = 0.5, bot_rt.y = 0.5; 
        break;
    }

    /* face 1 */
    tcube[0].pos_vert.vec[X] = -x, tcube[0].pos_vert.vec[Y] = -y, tcube[0].pos_vert.vec[Z] = z;//0
    tcube[0].tex_vert = bot_lt;
    tcube[1].pos_vert.vec[X] = x, tcube[1].pos_vert.vec[Y] = -y, tcube[1].pos_vert.vec[Z] = z;//1
    tcube[1].tex_vert = bot_rt;
    tcube[2].pos_vert.vec[X] = x, tcube[2].pos_vert.vec[Y] = y, tcube[2].pos_vert.vec[Z] = z;//3
    tcube[2].tex_vert = top_rt;

    tcube[3].pos_vert.vec[X] = -x, tcube[3].pos_vert.vec[Y] = -y, tcube[3].pos_vert.vec[Z] = z;//0
    tcube[3].tex_vert = bot_lt;
    tcube[4].pos_vert.vec[X] = x, tcube[4].pos_vert.vec[Y] = y, tcube[4].pos_vert.vec[Z] = z;//3
    tcube[4].tex_vert = top_rt;
    tcube[5].pos_vert.vec[X] = -x, tcube[5].pos_vert.vec[Y] = y, tcube[5].pos_vert.vec[Z] = z;//2
    tcube[5].tex_vert = top_lt;

    /* face 2: +x, changing z */
    tcube[6].pos_vert.vec[X] = x, tcube[6].pos_vert.vec[Y] = -y,  tcube[6].pos_vert.vec[Z] = z;//1
    tcube[6].tex_vert = bot_lt;
    tcube[7].pos_vert.vec[X] = x,  tcube[7].pos_vert.vec[Y] = -y,  tcube[7].pos_vert.vec[Z] = -z;//5
    tcube[7].tex_vert = bot_rt;
    tcube[8].pos_vert.vec[X] = x,  tcube[8].pos_vert.vec[Y] = y, tcube[8].pos_vert.vec[Z] = -z;//7
    tcube[8].tex_vert = top_rt;

    tcube[9].pos_vert.vec[X] = x,  tcube[9].pos_vert.vec[Y] = -y,   tcube[9].pos_vert.vec[Z] = z;//1
    tcube[9].tex_vert = bot_lt;
    tcube[10].pos_vert.vec[X] = x, tcube[10].pos_vert.vec[Y] = y,  tcube[10].pos_vert.vec[Z] = -z;//7
    tcube[10].tex_vert = top_rt;
    tcube[11].pos_vert.vec[X] = x,  tcube[11].pos_vert.vec[Y] = y, tcube[11].pos_vert.vec[Z] = z;//3
    tcube[11].tex_vert = top_lt;

    /* face 3: -z */
    /* this triangle is acting weird  */
    tcube[12].pos_vert.vec[X] = x, tcube[12].pos_vert.vec[Y] = -y,  tcube[12].pos_vert.vec[Z] = -z;//5
    tcube[12].tex_vert = bot_lt;
    tcube[13].pos_vert.vec[X] = -x,  tcube[13].pos_vert.vec[Y] = -y,  tcube[13].pos_vert.vec[Z] = -z;//4
    tcube[13].tex_vert = bot_rt;
    tcube[14].pos_vert.vec[X] = -x,  tcube[14].pos_vert.vec[Y] = y, tcube[14].pos_vert.vec[Z] = -z;//6
    tcube[14].tex_vert = top_lt;
    /* ^^^ this triangle is acting weird ^^^ */

    tcube[15].pos_vert.vec[X] = x,  tcube[15].pos_vert.vec[Y] = -y,   tcube[15].pos_vert.vec[Z] = -z;//5
    tcube[15].tex_vert = bot_lt;
    tcube[16].pos_vert.vec[X] = -x, tcube[16].pos_vert.vec[Y] = y,  tcube[16].pos_vert.vec[Z] = -z;//6
    tcube[16].tex_vert = top_rt;
    tcube[17].pos_vert.vec[X] = x,  tcube[17].pos_vert.vec[Y] = y, tcube[17].pos_vert.vec[Z] = -z;//7
    tcube[17].tex_vert = top_lt;

    /* face 4: const -x, changing z */
    tcube[18].pos_vert.vec[X] = -x, tcube[18].pos_vert.vec[Y] = -y,  tcube[18].pos_vert.vec[Z] = -z;//4
    tcube[18].tex_vert = bot_lt;
    tcube[19].pos_vert.vec[X] = -x,  tcube[19].pos_vert.vec[Y] = -y,  tcube[19].pos_vert.vec[Z] = z;//0
    tcube[19].tex_vert = bot_rt;
    tcube[20].pos_vert.vec[X] = -x,  tcube[20].pos_vert.vec[Y] = y, tcube[20].pos_vert.vec[Z] = z;//2
    tcube[20].tex_vert = top_rt;

    tcube[21].pos_vert.vec[X] = -x,  tcube[21].pos_vert.vec[Y] = -y,tcube[21].pos_vert.vec[Z] = -z;//4
    tcube[21].tex_vert = bot_lt;
    tcube[22].pos_vert.vec[X] = -x, tcube[22].pos_vert.vec[Y] = y, tcube[22].pos_vert.vec[Z] = z;//2
    tcube[22].tex_vert = top_rt;
    tcube[23].pos_vert.vec[X] = -x,  tcube[23].pos_vert.vec[Y] = y, tcube[23].pos_vert.vec[Z] = -z;//6
    tcube[23].tex_vert = top_lt;

    /* bottom: const -y */
    tcube[24].pos_vert.vec[X] = -x, tcube[24].pos_vert.vec[Y] = -y,  tcube[24].pos_vert.vec[Z] = -z;//4
    tcube[24].tex_vert = bot_lt;
    tcube[25].pos_vert.vec[X] = x,  tcube[25].pos_vert.vec[Y] = -y,  tcube[25].pos_vert.vec[Z] = -z;//5
    tcube[25].tex_vert = bot_rt;
    tcube[26].pos_vert.vec[X] = x,  tcube[26].pos_vert.vec[Y] = -y, tcube[26].pos_vert.vec[Z] = z;//1
    tcube[26].tex_vert = top_rt;

    tcube[27].pos_vert.vec[X] = -x,  tcube[27].pos_vert.vec[Y] = -y,tcube[27].pos_vert.vec[Z] = -z;//4
    tcube[27].tex_vert = bot_lt;
    tcube[28].pos_vert.vec[X] = x, tcube[28].pos_vert.vec[Y] = -y, tcube[28].pos_vert.vec[Z] = z;//1
    tcube[28].tex_vert = top_rt;
    tcube[29].pos_vert.vec[X] = -x,  tcube[29].pos_vert.vec[Y] = -y, tcube[29].pos_vert.vec[Z] = z;//0
    tcube[29].tex_vert = top_lt;

    /* top: const y */
    tcube[30].pos_vert.vec[X] = -x, tcube[30].pos_vert.vec[Y] = y,  tcube[30].pos_vert.vec[Z] = z;//2
    tcube[30].tex_vert = bot_lt;
    tcube[31].pos_vert.vec[X] = x,  tcube[31].pos_vert.vec[Y] = y,  tcube[31].pos_vert.vec[Z] = z;//3
    tcube[31].tex_vert = bot_rt;
    tcube[32].pos_vert.vec[X] = x,  tcube[32].pos_vert.vec[Y] = y, tcube[32].pos_vert.vec[Z] = -z;//7
    tcube[32].tex_vert = top_rt;

    tcube[33].pos_vert.vec[X] = -x,  tcube[33].pos_vert.vec[Y] = y,tcube[33].pos_vert.vec[Z] = z;//2
    tcube[33].tex_vert = bot_lt;
    tcube[34].pos_vert.vec[X] = x, tcube[34].pos_vert.vec[Y] = y, tcube[34].pos_vert.vec[Z] = -z;//7
    tcube[34].tex_vert = top_rt;
    tcube[35].pos_vert.vec[X] = -x,  tcube[35].pos_vert.vec[Y] = y, tcube[35].pos_vert.vec[Z] = -z;//6
    tcube[35].tex_vert = top_lt;

    return tcube;
}

vec4* get_pos_verts(const pos_tex* tex_pos, int size) {
    vec4* pos_verts = calloc(size, size*sizeof(vec4));

    for(int i=0; i < size; i ++) {
        pos_verts[i] = tex_pos[i].pos_vert;
    }

    return pos_verts;
}

vec2* get_tex_verts(const pos_tex* tex_pos, int size) {
    vec2* tex_verts = calloc(size, size*sizeof(vec2));

    for(int i=0; i < size; i ++) {
        tex_verts[i] = tex_pos[i].tex_vert;
    }

    return tex_verts;
}

/* makes an 8x8 maze */
cell** make_maze() {
    int time_rand = 1542258418;
    srand(time_rand);
    printf("time: %d\n", time_rand);

    cell** maze_arr = calloc(8, sizeof(cell*));
    for(int i=0; i < 8; i++) {
        maze_arr[i] = calloc(8, sizeof(cell));
    }

    /* set boundaries for maze */
    int i;
    for (i = 0; i < 8; i++) {
        maze_arr[0][i].west_wall = GL_TRUE;
    }
     maze_arr[0][0].west_wall = GL_FALSE;
    for (i = 0; i < 8; i++) {
        maze_arr[i][0].north_wall = GL_TRUE;
    }
    for (i = 7; i >= 0; i--) {
        maze_arr[7][i].east_wall = GL_TRUE;
    }
    maze_arr[7][7].east_wall = GL_FALSE;
    for (i = 7; i >= 0; i--) {
        maze_arr[i][7].south_wall = GL_TRUE;
    }
    /* entrance and exit */

    /* uncomment when other things are done */
    divide(maze_arr, 0, 0, MAX_REC_INDEX, MAX_REC_INDEX, choose_ort(MAX_REC_INDEX, MAX_REC_INDEX));

    return maze_arr;
}

static void divide(cell** maze, int x, int y, int w, int h, enum ort orientation) {
    if (w <= 1 || h <= 1) 
        return;

    GLboolean is_horizontal = orientation == HORZ;

    int wall_x = (is_horizontal) ? 0 : rand() % (w-1);
    int wall_y = (is_horizontal) ? rand() % (h-1) : 0;
    wall_x += x;
    wall_y += y;

    int hole_x = (is_horizontal) ? rand() % (w) : 0;
    int hole_y = (is_horizontal) ? 0 : rand() % (h);
    hole_x += wall_x;
    hole_y += wall_y;

    int dir_x = (is_horizontal) ? 1 : 0;
    int dir_y = (is_horizontal) ? 0 : 1;

    int wall_len = (is_horizontal) ? w : h;

    if (is_horizontal) { // do the east wall

        for (int i = 0; i < wall_len; i++) {
            maze[wall_y][wall_x].east_wall = GL_TRUE;
            wall_y += dir_y;
            wall_x += dir_x;
        }
        maze[hole_y][hole_x].east_wall = GL_FALSE;

    } else { // do the south wall
        for (int i = 0; i < wall_len; i++) {
            maze[wall_y][wall_x].south_wall = GL_TRUE;
            wall_y += dir_y;
            wall_x += dir_x;
        }
        maze[hole_y][hole_x].south_wall = GL_FALSE;
    } 


    int next_x = x;
    int next_y = y;
    int next_w = (is_horizontal) ? w : wall_x-x+1;
    int next_h = (is_horizontal) ? wall_y-y+1 : h;
    divide(maze, next_x, next_y, next_w, next_h, choose_ort(w, h));

    next_x = (is_horizontal) ? x : wall_x+1;
    next_y = (is_horizontal) ? wall_y+1 : y;
    next_w = (is_horizontal) ? w : x+w-wall_x-1;
    next_h = (is_horizontal) ? y+h-wall_y-1 : h;
    divide(maze, next_x, next_y, next_w, next_h, choose_ort(w, h));
}

void print_cell(const cell* cell_print) {
    //char* top_filled = "*---*\n|   |\n*---*\n";
    char* cell_str = calloc(1, 19 * sizeof(char));

    char* top_filled = (cell_print->north_wall) ? "*---*\n" : "*   *\n";
    char* bot_filled = (cell_print->south_wall) ? "*---*\n" : "*   *\n";
    char* left_right = "";

    if(cell_print->east_wall && cell_print->west_wall) {
        left_right = "|   |\n";
    } else if (cell_print->east_wall && !cell_print->west_wall) {
        left_right = "|    \n";
    } else if (!cell_print->east_wall && cell_print->west_wall) {
        left_right = "    |\n";
    } else {
        left_right = "     \n";
    }

    //char* result = calloc(1, 19 * sizeof(char));
    strcat(cell_str, top_filled);
    strcat(cell_str, left_right);
    strcat(cell_str, bot_filled);
    printf("%s", cell_str);
}

/* returning triple array. yikes. */
GLubyte*** get_texture(const char* file_name) {
    GLubyte*** tex_array = (GLubyte***) calloc(400, 400*sizeof(GLubyte**));

    for (int i=0; i < 400; i++) {
        tex_array[i] = (GLubyte**) calloc(400, 400*sizeof(GLubyte*));

        for(int j=0; j < 4; j++) {
            tex_array[i][j] = (GLubyte*) calloc(4, 4*sizeof(GLubyte));
        }
    }
    FILE* fp = fopen(file_name, "r");
    fread(tex_array, 400 * 400 * 3, 1, fp);
    fclose(fp);
    return tex_array;
}

GLubyte*** get_textures() {
    GLubyte*** tex_array = (GLubyte***) calloc(800, 800*sizeof(GLubyte**));

    for (int i=0; i < 800; i++) {
        tex_array[i] = (GLubyte**) calloc(800, 800*sizeof(GLubyte*));

        for(int j=0; j < 4; j++) {
            tex_array[i][j] = (GLubyte*) calloc(4, 4*sizeof(GLubyte));
        }
    }
    FILE* fp = fopen("textures/p2texture04.raw", "r");
    fread(tex_array, 800 * 800 * 3, 1, fp);
    fclose(fp);
    return tex_array;
}

vec4* ground() {
    vec4* starting_cube = single_cube();
    starting_cube = scale_vertices(starting_cube, VERTS_IN_CUBE, 0.9, 0.1, 0.9);
    starting_cube = rotate_vertices(starting_cube, VERTS_IN_CUBE, 0.1, Y);
    starting_cube = rotate_vertices(starting_cube, VERTS_IN_CUBE, 0.1, X);

    return starting_cube;
}

pos_tex* ground_with_tex() {
    pos_tex* starting_cube = single_cube_texture(GRASS);
    vec4* starting_cube_pos = get_pos_verts(starting_cube, VERTS_IN_CUBE);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, 0.8, 0.1, 0.8);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, 110, 110, 110);

    for (int i = 0; i < VERTS_IN_CUBE; i++) {
        starting_cube[i].pos_vert = starting_cube_pos[i];
    }

    return starting_cube;
}

pos_tex* wall_with_tex() {
    int scale_amt = 3;
    pos_tex* starting_cube = single_cube_texture(BRICK);
    vec4* starting_cube_pos = get_pos_verts(starting_cube, VERTS_IN_CUBE);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, 0.1, 0.8, 0.8);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, scale_amt, scale_amt, scale_amt);

    for (int i = 0; i < VERTS_IN_CUBE; i++) {
        starting_cube[i].pos_vert = starting_cube_pos[i];
    }

    return starting_cube;
}

pos_tex* pillar_with_tex() {
    int scale_amt = 3;
    pos_tex* starting_cube = single_cube_texture(PILLAR);
    vec4* starting_cube_pos = get_pos_verts(starting_cube, VERTS_IN_CUBE);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, 0.15, 0.85, 0.15);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, scale_amt, scale_amt, scale_amt);

    for (int i = 0; i < VERTS_IN_CUBE; i++) {
        starting_cube[i].pos_vert = starting_cube_pos[i];
    }

    return starting_cube;
}

pos_tex* translate_pos_verts(pos_tex* tex_pos, int num_verts, GLfloat x, GLfloat y, GLfloat z) {
    vec4* pos_verts = get_pos_verts(tex_pos, num_verts);
    pos_verts = translate_vertices(pos_verts, num_verts, x, y, z);

    for (int i = 0; i < num_verts; i++) {
        tex_pos[i].pos_vert = pos_verts[i];
    }

    return tex_pos;
}

pos_tex* rotate_pos_verts(pos_tex* tex_pos, int num_verts, GLfloat rot) {
    vec4* pos_verts = get_pos_verts(tex_pos, num_verts);
    pos_verts = rotate_vertices(pos_verts, num_verts, rot, Y);

    for (int i = 0; i < num_verts; i++) {
        tex_pos[i].pos_vert = pos_verts[i];
    }

    return tex_pos;
}

pos_tex* wall_with_pillar(GLboolean make_wall) {
    pos_tex* wall_pillar = empty_cube_arr(2);
    pos_tex* pillar = pillar_with_tex();
    pillar = translate_pos_verts(pillar, VERTS_IN_CUBE, 0, 0, 1.35);

    pos_tex* wall;

    if(make_wall) {
        wall = wall_with_tex();
    } else {
        wall = pillar_with_tex();
        wall = translate_pos_verts(wall, VERTS_IN_CUBE, 0, 0, 1.35);
    }

    for (int i = 0; i < VERTS_IN_CUBE; i ++) {
        wall_pillar[i] = wall[i];
    }

    for (int i = 36; i < VERTS_IN_CUBE * 2; i ++) {
        wall_pillar[i] = pillar[i - 36];
    }

    return wall_pillar;
}

cell* get_cell(const cell** maze, int x, int y, int size) {
    cell* ret_cell = calloc(1, sizeof(cell));

    cell curr_cell = maze[x][y];

    ret_cell->east_wall = curr_cell.east_wall;
    ret_cell->west_wall = curr_cell.west_wall;
    ret_cell->south_wall = curr_cell.south_wall;
    ret_cell->north_wall = curr_cell.north_wall;

    if(!ret_cell->east_wall && x > 0) {
        ret_cell->east_wall = maze[x-1][y].west_wall;
    }
    if(!ret_cell->west_wall && x < size-1) {
        ret_cell->west_wall = maze[x+1][y].east_wall;
    }

    if(!ret_cell->south_wall && y > 0) {
        ret_cell->south_wall = maze[x][y-1].north_wall;
    }
    if(!ret_cell->north_wall && y < size-1) {
        ret_cell->north_wall = maze[x][y+1].south_wall;
    }

    return ret_cell;
}
