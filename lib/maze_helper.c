#include "../headers/maze_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

//enum wall {HORZ, VERT};

#define MAX_REC_INDEX 7

static void rec_maze_builder(cell** maze, int start_vert, int end_vert, int start_hor, int end_hor);

vec4* single_cube(){
    vec4* cube = calloc(36, 36*sizeof(vec4));

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

pos_tex* single_cube_texture(){
    pos_tex* tcube = calloc(36, 36*sizeof(pos_tex));

    for(int i=0; i<36; i++) {
        tcube[i].pos_vert.vec[W] = 1;
    }

    GLfloat x, y, z;    
    x = 0.5;
    y = 0.5;
    z = 0.5;

    vec2 bot_rt, bot_lt, top_rt, top_lt;
    bot_lt.x = 0, bot_lt.y = 0;
    bot_rt.x = 1, bot_rt.y = 0;
    top_rt.x = 1, top_lt.y = 1;
    top_lt.x = 0, top_lt.y = 1;

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
    tcube[12].pos_vert.vec[X] = x, tcube[12].pos_vert.vec[Y] = -y,  tcube[12].pos_vert.vec[Z] = -z;//5
    tcube[12].tex_vert = bot_lt;
    tcube[13].pos_vert.vec[X] = -x,  tcube[13].pos_vert.vec[Y] = -y,  tcube[13].pos_vert.vec[Z] = -z;//4
    tcube[13].tex_vert = bot_lt;
    tcube[14].pos_vert.vec[X] = -x,  tcube[14].pos_vert.vec[Y] = y, tcube[14].pos_vert.vec[Z] = -z;//6
    tcube[14].tex_vert = top_lt;

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

/* makes an 8x8 maze */
cell** make_maze() {
    srand(time(0));

    cell** maze_arr = calloc(8, sizeof(cell*));
    for(int i=0; i < 8; i++) {
        maze_arr[i] = calloc(8, sizeof(cell));
    }

    /* set boundaries for maze */
    int i;
    for (i = 0; i < 8; i++) {
        maze_arr[0][i].north_wall = GL_TRUE;
    }
    for (i = 0; i < 8; i++) {
        maze_arr[i][0].east_wall = GL_TRUE;
    }
    for (i = 7; i >= 0; i--) {
        maze_arr[7][i].south_wall = GL_TRUE;
    }
    for (i = 7; i >= 0; i--) {
        maze_arr[i][7].west_wall = GL_TRUE;
    }
    /* entrance and exit */
    maze_arr[0][0].east_wall = GL_FALSE;
    maze_arr[7][7].west_wall = GL_FALSE;

    /* uncomment when other things are done */
    //rec_maze_builder(maze_arr, 0, MAX_REC_INDEX, 0, MAX_REC_INDEX);

    return maze_arr;
}

/*  I need a recursive function that will make the maze using division
    rules:
        1. generating vert lines:
            a. arr from 0 to 6
            b. only spawn east_wall
        2. generating horizontal lines
            a. arr from 0 to 6
            b. only spawning south_walls
    This is to avoid generating an adjacent wall and not generating to matching adjacent wall
 */

static void rec_maze_builder(cell** maze, int start_vert, int end_vert, int start_hor, int end_hor) {

    int hor_diff = end_hor-start_hor;
    int vert_diff = end_vert-start_vert;

    if(hor_diff <= 0 && (vert_diff <= 0)) {
        return;
    }

    /* do the vertical line first */
    int vert_index;
    int hole_index;
    if (vert_diff > 0) {
        vert_index = (rand() % (vert_diff)) + start_vert;// find the index im walling up
        hole_index = (rand() % (vert_diff)) + start_vert;// find the index where the hole is going
        for(int i=start_vert; i < end_vert; i++) {
            maze[i][vert_index].east_wall = GL_TRUE;
        }
        maze[hole_index][vert_index].east_wall = GL_FALSE;
    } else {
        vert_index = start_vert;
    }


    /* then do the horizontal line */
    int hor_index;
    if (hor_diff > 0) {
        hor_index = start_hor + (rand() % (hor_diff));// find the index im walling up
        hole_index = start_hor + (rand() % (hor_diff));// find the index where the hole is going
        for(int i=start_hor; i < end_hor; i++) {
            maze[hor_index][i].south_wall = GL_TRUE;
        }
        maze[hor_index][hole_index].south_wall = GL_FALSE;
    } else {
        hor_index = start_hor;
    }

    /* bottom left quad */
    rec_maze_builder(maze, start_vert, vert_index-1, hor_index+1, end_hor);

    /* top right quad */
    rec_maze_builder(maze, vert_index+1, end_vert, start_hor, hor_index-1);

    /* bottom right quad */
    rec_maze_builder(maze, vert_index+1, end_vert, hor_index+1, end_hor);

    /* top left quad */
    rec_maze_builder(maze, start_vert, vert_index-1, start_hor, hor_index-1);
    // rec_maze_builder(maze, vert_index-1, end_vert, hor_index+1, end_hor);
    // rec_maze_builder(maze, start_vert, vert_index-1, start_hor, hor_index-1);

    // rec_maze_builder(maze, vert_index+1, end_vert, start_hor, hor_index+1);
    // rec_maze_builder(maze, start_vert, vert_index-1, hor_index-1, end_hor);
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