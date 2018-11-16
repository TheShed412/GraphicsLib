#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <time.h>

#include "headers/initShader.h"
#include "headers/matrix_lib.h"
#include "headers/transformations.h"
#include "headers/maze_helper.h"
#include "headers/view.h"

/* numTriangles = 360/degreePerVertex */
/* for the circle, vertices are 3 x numTriangles */
/* for the cone, vertices are 6 x numTriangles */
#define NUM_VERTICES 36
#define DELTA 0.01
//#define DEBUG
//#define AUTO_SOLVER

enum direction {FORWARD, BACKWARDS, RIGHT, LEFT};

typedef struct {
    int i;
    int j;
    enum direction anim;
} dir;

int whole_maze_cubes = 337;

vec4 origin = {0, 0, 0, 0};

GLuint ctm_location;
GLuint perspective_shift;
GLuint cam_shit;
mat4 projection;
mat4 model_view;
GLuint perspective_shift;
mat4 ctm =             {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

mat4 id =             {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

vec4 eyes_maze_start = {-11.5, 7, -10, 1};
vec4 look_at_maze_start = {10, 7, -10, 1};
vec4 orbit_stop = {20, 20, 0, 1};
#ifndef DEBUG
vec4 eyes = {0, 20, -20, 1};// starting point {-10, 11, -10, 1}
vec4 look_at_pos = {0, 0, 0, 1};// starting point {0, 11, -10, 1}
#else
// orbit stop point: {20, 20, 0, 1}
vec4 eyes = {-11.5, 7, -10, 1};// starting point {-10, 11, -10, 1}
vec4 look_at_pos = {100, 7, -10, 1};
#endif
vec4 up_vec = {0, 1, 0, 1};
vec4 to_maze_look;

enum state* solve_states;

enum state curr_state = ORBITING;

vec4* bottom(int num_vertices, GLfloat twist, int axis);
void init(void);
void display(void);
void keyboard(unsigned char key, int mousex, int mousey);
void idle(int);
pos_tex* all_pillars();

int total_vertices = 0;

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Maze");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(1, idle, 0);
    glutMainLoop();

    return 0;
}

void print_maze(cell** maze)
{
    int i, j;
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            printf("+");
            if (maze[i][j].north_wall) {
                printf("---");
            } else {
                printf("   ");
            }
        }
        printf("+\n");
        for (i = 0; i < 8; i++) {
            if (maze[i][j].west_wall) {
                printf("|");
            } else {
                printf(" ");
            }
            printf("   ");
            if (i == 7) {
                if (maze[i][j].east_wall) {
                    printf("|");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
        if (j == 7) {
            for (int i = 0; i < 8; i++) {
                printf("+");
                if (maze[i][j].south_wall) {
                    printf("---");
                } else {
                    printf("   ");
                }
            }
            printf("+\n");
        }
    }
}

pos_tex* cell_walls(const cell* walls) {
    pos_tex* grnd_pillar = empty_cube_arr(8);
    pos_tex* wp1 = wall_with_pillar(walls->west_wall);
    pos_tex* wp2 = wall_with_pillar(walls->south_wall);
    pos_tex* wp3 = wall_with_pillar(walls->east_wall);
    pos_tex* wp4 = wall_with_pillar(walls->north_wall);

    wp2 = translate_pos_verts(wp2, VERTS_IN_CUBE*2, -1.35, 0, 1.35);
    wp2 = rotate_pos_verts(wp2, VERTS_IN_CUBE*2, M_PI/2);

    wp3 = rotate_pos_verts(wp3, VERTS_IN_CUBE*2, M_PI);
    wp3 = translate_pos_verts(wp3, VERTS_IN_CUBE*2, 1.35 * 2, 0, 0);
    
    wp4 = translate_pos_verts(wp4, VERTS_IN_CUBE*2, -1.35, 0, -1.35);
    wp4 = rotate_pos_verts(wp4, VERTS_IN_CUBE*2, -M_PI/2);

    for (int i = 0; i < VERTS_IN_CUBE * 2; i ++) {
        grnd_pillar[i] = wp1[i];
    }

    for (int i = VERTS_IN_CUBE * 2; i < VERTS_IN_CUBE * 4; i ++) {
        grnd_pillar[i] = wp2[i - VERTS_IN_CUBE * 2];
    }

    for (int i = VERTS_IN_CUBE * 4; i < VERTS_IN_CUBE * 6; i ++) {
        grnd_pillar[i] = wp3[i - VERTS_IN_CUBE * 4];
    }

    for (int i = VERTS_IN_CUBE * 6; i < VERTS_IN_CUBE * 8; i ++) {
        grnd_pillar[i] = wp4[i - VERTS_IN_CUBE * 6];
    }

    return grnd_pillar;
}

pos_tex* cell_walls_no_pillars(const cell* walls) {
    pos_tex* grnd_pillar = empty_cube_arr(4);
    pos_tex* wp1;
    pos_tex* wp2;
    pos_tex* wp3;
    pos_tex* wp4;

    wp1 = wall_with_tex();
    for (int i = 0; i < VERTS_IN_CUBE; i ++) {
        if (walls->west_wall) {
            grnd_pillar[i] = wp1[i]; 
        }
    }

    wp2 = wall_with_tex();
    wp2 = translate_pos_verts(wp2, VERTS_IN_CUBE*2, -1.35, 0, 1.35);
    wp2 = rotate_pos_verts(wp2, VERTS_IN_CUBE*2, M_PI/2);
    for (int i = VERTS_IN_CUBE; i < VERTS_IN_CUBE * 2; i ++) {
        if(walls->south_wall) {
            grnd_pillar[i] = wp2[i - VERTS_IN_CUBE];
        }

    }


    wp3 = wall_with_tex();
    wp3 = rotate_pos_verts(wp3, VERTS_IN_CUBE*2, M_PI);
    wp3 = translate_pos_verts(wp3, VERTS_IN_CUBE*2, 1.35 * 2, 0, 0);
    for (int i = VERTS_IN_CUBE * 2; i < VERTS_IN_CUBE * 3; i ++) {
        if (walls->east_wall) {
            grnd_pillar[i] = wp3[i - VERTS_IN_CUBE * 2];
        }
    }

    wp4 = wall_with_tex();
    wp4 = translate_pos_verts(wp4, VERTS_IN_CUBE*2, -1.35, 0, -1.35);
    wp4 = rotate_pos_verts(wp4, VERTS_IN_CUBE*2, -M_PI/2);
    for (int i = VERTS_IN_CUBE * 3; i < VERTS_IN_CUBE * 4; i ++) {
        if (walls->north_wall) {
            grnd_pillar[i] = wp4[i - VERTS_IN_CUBE * 3];
        }
    }

    return grnd_pillar;
}

pos_tex* whole_maze(cell** maze_cells) {
    pos_tex* maze_shapes = empty_cube_arr(whole_maze_cubes);
    pos_tex* pillars = all_pillars();

    GLfloat trans_x = 0;
    GLfloat trans_z = 0;
    int flip = 1;
    int inner = 0;
    int bound = VERTS_IN_CUBE * 4;
    int diff = 0;
    int size = 8;
    int total_wall_verts = 0;
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j++) {
            pos_tex* curr_pillar = cell_walls_no_pillars(&maze_cells[i][j]);
            curr_pillar = translate_pos_verts(curr_pillar, VERTS_IN_CUBE*4, 
                trans_x, 0, trans_z);
            for (; inner < bound; inner++) {
                maze_shapes[inner] = curr_pillar[inner - diff];
                total_wall_verts++;
            }
            inner = bound;
            diff = bound;
            bound += VERTS_IN_CUBE*4;
            trans_z += ((1.35*2));
        }
        trans_x += (1.35*2);
        trans_z = 0;
    }

    int pillar_end_index = total_wall_verts + VERTS_IN_CUBE*81;
    for(int i=total_wall_verts; i < pillar_end_index; i ++) {
        maze_shapes[i] = pillars[i - total_wall_verts];
    }

    return maze_shapes;
}

enum direction get_anim(enum cardinal forward, enum cardinal new_dir) {
    if (forward == EAST) {
        switch(new_dir) {
            case EAST:
                return FORWARD;
            break;
            case WEST:
                return BACKWARDS;
            break;
            case SOUTH:
                return RIGHT;
            break;
            case NORTH:
                return LEFT;
            break;
        }
    } else if (forward == SOUTH) {
        switch(new_dir) {
            case EAST:
                return LEFT;
            break;
            case WEST:
                return RIGHT;
            break;
            case SOUTH:
                return FORWARD;
            break;
            case NORTH:
                return BACKWARDS;
            break;
        }
    } else if (forward == WEST) {
        switch(new_dir) {
            case EAST:
                return BACKWARDS;
            break;
            case WEST:
                return FORWARD;
            break;
            case SOUTH:
                return LEFT;
            break;
            case NORTH:
                return RIGHT;
            break;
        }
    } else {//north
        switch(new_dir) {
            case EAST:
                return RIGHT;
            break;
            case WEST:
                return LEFT;
            break;
            case SOUTH:
                return FORWARD;
            break;
            case NORTH:
                return BACKWARDS;
            break;
        }
    }
}

int max_dirs = 128;
dir* solver(const cell** maze) {
    dir* dirs = calloc(max_dirs, max_dirs*sizeof(dir));

    int i = 0;
    int j = 0;
    int turns = 0;
    GLboolean just_turned_right = GL_FALSE;
    enum cardinal curr_forward = EAST;
    while((i <= 7 && j <= 7)) {
        if(turns >= max_dirs) {
            return NULL;
        }
        cell curr_cell = maze[i][j];

        if (!curr_cell.south_wall) {
            dir new_dir;
            new_dir.i = i;
            new_dir.j = j;
            new_dir.anim = get_anim(curr_forward, SOUTH);
            curr_forward = SOUTH;
            dirs[turns] = new_dir;
        } else if (!curr_cell.east_wall) {
            dir new_dir;
            new_dir.i = i;
            new_dir.j = j;
            new_dir.anim = get_anim(curr_forward, EAST);
            curr_forward = EAST;
            dirs[turns] = new_dir;
        } else if (!curr_cell.north_wall) {
            dir new_dir;
            new_dir.i = i;
            new_dir.j = j;
            new_dir.anim = get_anim(curr_forward, NORTH);
            curr_forward = NORTH;
            dirs[turns] = new_dir;
        } else if (!curr_cell.north_wall) {
            dir new_dir;
            new_dir.i = i;
            new_dir.j = j;
            new_dir.anim = get_anim(curr_forward, NORTH);
            curr_forward = NORTH;
            dirs[turns] = new_dir;
        }

        turns ++;
    }

    return dirs;
}

pos_tex* all_pillars() {
    pos_tex* pillars = empty_cube_arr(81);

    GLfloat trans_x = 0;
    GLfloat trans_z = 0;
    int flip = 1;
    int inner = 0;
    int bound = VERTS_IN_CUBE;
    int diff = 0;
    int size = 8;
    for (int i = 0; i < 9; i ++) {
        for (int j = 0; j < 9; j++) {
            pos_tex* curr_pillar = pillar_with_tex();
            curr_pillar = translate_pos_verts(curr_pillar, VERTS_IN_CUBE, 
                trans_x, 0, trans_z);
            for (; inner < bound; inner++) {
                pillars[inner] = curr_pillar[inner - diff];
            }
            inner = bound;
            diff = bound;
            bound += VERTS_IN_CUBE;

            trans_z += ((1.35*2));
        }
        trans_x += (1.35*2);
        trans_z = 0;
    }

    pillars = translate_pos_verts(pillars, VERTS_IN_CUBE*81, 0, 0, -1.35);

    return pillars;
}

pos_tex* whole_maze_and_ground(cell** maze_cells) {
    pos_tex* whole_shabang = empty_cube_arr(whole_maze_cubes + 1);

    pos_tex* the_ground = ground_with_tex();
    pos_tex* the_maze = whole_maze(maze_cells);

    the_maze = translate_pos_verts(the_maze, VERTS_IN_CUBE*whole_maze_cubes, -10, 6.5, -10);

    for (int i=0; i < VERTS_IN_CUBE*whole_maze_cubes; i++) {
        whole_shabang[i] = the_maze[i];
    }

    for(int i=VERTS_IN_CUBE*whole_maze_cubes; i < VERTS_IN_CUBE*(whole_maze_cubes+1); i++) {
        whole_shabang[i] = the_ground[i - (VERTS_IN_CUBE*whole_maze_cubes)];
    }
    to_maze_look = the_maze[0].pos_vert;
    return whole_shabang;
}

void print_states(enum state* states) {

    for (int i = 0; i < 256; i ++) {
        if(states[i] == MOVING_FORWARD) {
            printf("MOVING FORWARD\n");
        } else if (states[i] == TURNING_LEFT) {
            printf("TURNING LEFT\n");
        } else if (states[i] == TURNING_RIGHT) {
            printf("TURNING RIGHT\n");
        }  else if (states[i] == SOLVED) {
            printf("SOLVED\n");
        }
    }
}

/**
 * From the circle.c file with a couple small changes
*/
void init(void)
{
    GLubyte*** grass_tex = get_textures();  
    cell** maze = make_maze();
    print_maze(maze);
    solve_states = right_hand_rule(maze, 8);
    print_states(solve_states);
    GLuint program = initShader("shaders/vshader_proj2.glsl", "shaders/fshader_proj2.glsl");
    glUseProgram(program);

    pos_tex* ground_tex_pos = whole_maze_and_ground(maze);
    total_vertices = get_total_verts();
    vec2* tex_coords = get_tex_verts(ground_tex_pos, total_vertices);
    vec4 *ground_vertices = get_pos_verts(ground_tex_pos, total_vertices);

    int f = 1;
    projection = *frustum(-f, f, f, -f, -f, f);
    model_view = *look_at(&eyes, &look_at_pos, &up_vec);

    GLuint mytex[1];
    glGenTextures(1, mytex);
    glBindTexture(GL_TEXTURE_2D, mytex[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, grass_tex);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    int param;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &param);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER
        , sizeof(vec4) * total_vertices + sizeof(vec4)*total_vertices + sizeof(vec2) * total_vertices 
        , NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * total_vertices, ground_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * total_vertices , sizeof(vec2) *total_vertices, tex_coords);


    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * total_vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vec4) * total_vertices));

    GLuint texture_location = glGetUniformLocation(program, "texture");
    glUniform1i(texture_location, 0);

    ctm_location = glGetUniformLocation(program, "ctm");
    perspective_shift = glGetUniformLocation(program, "projection");
    cam_shit = glGetUniformLocation(program, "model_view");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
    glClearColor(0.0, 0.0, 0.5, 1.0);
    glDepthRange(1,0);
}

/**
 * From the circle.c file with a couple small changes
*/
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
    glUniformMatrix4fv(perspective_shift, 1, GL_FALSE, (GLfloat *) &projection);
    glUniformMatrix4fv(cam_shit, 1, GL_FALSE, (GLfloat *) &model_view);

    glDrawArrays(GL_TRIANGLES, 0, total_vertices);

    glutSwapBuffers();
}

GLfloat vec_mag(const vec4* vec) {
    GLfloat x = vec->vec[X];
    GLfloat y = vec->vec[Y];
    GLfloat z = vec->vec[Z];
    GLfloat w = vec->vec[W];
    return sqrt(x*x + y*y + z*z);
}

GLfloat vec_angle_btw(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2) {

    if(x1 == x2 && z1==z2) return 0.0;

    GLfloat mag_vec1 = sqrt(x1*x1 + z1*z1);
    GLfloat mag_vec2 =  sqrt(x2*x2 + z2*z2);

    GLfloat dot_prod = x1*x2 + z1*z2;

    GLfloat val = dot_prod/(mag_vec1 * mag_vec2);

    return acos(val);
}

GLfloat spin = 0.015;
GLfloat spin_total = 0.0;
GLfloat left_spin_total = 0.0;
GLboolean start_solve = GL_FALSE;
GLfloat move = 0.01;
GLfloat east_move_tracker = 0;
GLfloat south_move_tracker = 0;


GLfloat east_dist = 0.17;
GLboolean done = GL_FALSE;
GLfloat move_dist = 0;
GLfloat fwd_dist = 1.35 * 2;
GLfloat mv_counter = 0.0;
GLfloat rot_amnt = 0.0;
int state_counter = 0;
vec4 old_look_at = {100, 7, -10, 1};
enum cardinal curr_forward = EAST;

int test = 0;
void idle(int value) 
{
    glutTimerFunc(1, idle, 0);
    vec4* dist_vec = &eyes_maze_start;
    #ifndef AUTO_SOLVER
    enum state states[] = {MOVING_FORWARD, MOVING_FORWARD, MOVING_FORWARD, 
    MOVING_FORWARD, TURNING_RIGHT, MOVING_FORWARD, TURNING_RIGHT, MOVING_FORWARD, TURNING_LEFT, MOVING_FORWARD,
    TURNING_LEFT, MOVING_FORWARD, TURNING_RIGHT, MOVING_FORWARD, MOVING_FORWARD, TURNING_RIGHT, MOVING_FORWARD,
    MOVING_FORWARD, TURNING_LEFT, MOVING_FORWARD, TURNING_RIGHT, MOVING_FORWARD, TURNING_LEFT, MOVING_FORWARD,
    MOVING_FORWARD, TURNING_LEFT, MOVING_FORWARD, MOVING_FORWARD, MOVING_FORWARD, MOVING_FORWARD, MOVING_FORWARD,
    MOVING_FORWARD, MOVING_FORWARD, MOVING_FORWARD,
    SOLVED};

    solve_states = states;
    #endif
    
    #ifndef DEBUG
    switch(curr_state) {
        case ORBITING:
            {
                mat4* rotation_matrix = get_rotation_matrix(spin, Y);
                eyes = *mat_mult_vec(rotation_matrix, &eyes);
                model_view = *look_at(&eyes, &look_at_pos, &up_vec);
                if (start_solve) 
                    curr_state = ORBIT_STOPPED;
            }
        break;
        case ORBIT_STOPPED:
            {
                if(!(eyes.vec[Z] >= -0.07 && eyes.vec[Z] <= 0.13 && eyes.vec[X] < 0)) {
                    mat4* rotation_matrix = get_rotation_matrix(spin, Y);
                    eyes = *mat_mult_vec(rotation_matrix, &eyes);
                    model_view = *look_at(&eyes, &look_at_pos, &up_vec);
                } else {
                    curr_state = MOVING_TO_MAZE;
                    vec4* dist_vec = vec_sub(&eyes_maze_start, &eyes);
                    move_dist = vec_mag(dist_vec)*2;
                }
            }
        break;
        // vec4 eyes = {0, 20, -20, 1}; start
        // vec4 eyes = {-11.5, 7, -10, 1}; end
        case MOVING_TO_MAZE:
            {   
                int finished = 0;
                look_at_pos = look_at_maze_start;
                mat4* trans_mat;
                if(!(eyes.vec[Y] < 7.1 && eyes.vec[Y] > 6.9)) {
                    trans_mat = get_translation_matrix(0, -dist_vec->vec[Y]/move_dist, 0);
                } else if(!(eyes.vec[Z] < -9.9 && eyes.vec[Z] > -10.2)) {
                    trans_mat = get_translation_matrix(0, 0, dist_vec->vec[Z]/move_dist);
                } else if (!(eyes.vec[X] > -11.7 && eyes.vec[X] < -11.3)) {
                    trans_mat = get_translation_matrix(-dist_vec->vec[X]/move_dist, 0, 0);
                } else {
                    trans_mat = get_translation_matrix(0, 0, 0);
                    finished = 1;
                }
                eyes = *mat_mult_vec(trans_mat, &eyes);

                if(finished) {
                    eyes = eyes_maze_start;
                    look_at_pos = old_look_at;
                    curr_state = solve_states[state_counter];
                }

                model_view = *look_at(&eyes, &look_at_pos, &up_vec);
            }
        break;
        case MOVING_FORWARD:
            {
                mat4* trans_mat;
                int finished = 0;

                if(curr_forward == EAST) {
                    
                    if(mv_counter >= fwd_dist) {
                        finished = 1;
                    } else {
                        trans_mat = get_translation_matrix(0.05, 0, 0);
                        mv_counter += 0.05;
                    }
                } else if (curr_forward == WEST) {
                    if(mv_counter >= fwd_dist) {
                        finished = 1;
                    } else {
                        trans_mat = get_translation_matrix(-0.05, 0, 0);
                        mv_counter += 0.05;
                    }
                } else if (curr_forward == NORTH) {
                    if(mv_counter >= fwd_dist) {
                        finished = 1;
                    } else {
                        trans_mat = get_translation_matrix(0, 0, -0.05);
                        mv_counter += 0.05;
                    }
                } else if (curr_forward == SOUTH) {
                    if(mv_counter >= fwd_dist) {
                        finished = 1;
                    } else {
                        trans_mat = get_translation_matrix(0, 0, 0.05);
                        mv_counter += 0.05;
                    }
                }

                if(!finished) {
                    eyes = *mat_mult_vec(trans_mat, &eyes);
                    look_at_pos = *mat_mult_vec(trans_mat, &look_at_pos);
                } else {
                    state_counter++;
                    curr_state = solve_states[state_counter];
                    old_look_at = look_at_pos;
                    mv_counter = 0.0;
                }

                model_view = *look_at(&eyes, &look_at_pos, &up_vec);
            }
        break;
        case TURNING_RIGHT:
            {
                mat4* trans_mat;
                int finished = 0;

                GLfloat theta = vec_angle_btw(old_look_at.vec[X], old_look_at.vec[Z], look_at_pos.vec[X], look_at_pos.vec[Z]);

                if (theta <= (M_PI/2 + DELTA)) {
                    trans_mat = get_rotation_matrix(-0.02, Y);
                } else {
                    finished = 1;
                }

                if(!finished) {
                    look_at_pos = *mat_mult_vec(trans_mat, &look_at_pos);
                } else {
                    enum cardinal temp_fwd;
                    if(curr_forward == EAST) temp_fwd = SOUTH;
                    else if(curr_forward == SOUTH) temp_fwd = WEST;
                    else if(curr_forward == WEST) temp_fwd = NORTH;
                    else if(curr_forward == NORTH) temp_fwd = EAST;
                    state_counter++;
                    curr_state = solve_states[state_counter];
                    old_look_at = look_at_pos;
                    rot_amnt = 0.0;
                    curr_forward = temp_fwd;
                }

                model_view = *look_at(&eyes, &look_at_pos, &up_vec);

            }
        break;
        case TURNING_LEFT:
            {
                mat4* trans_mat;
                int finished = 0;

                GLfloat theta = vec_angle_btw(old_look_at.vec[X], old_look_at.vec[Z], look_at_pos.vec[X], look_at_pos.vec[Z]);

                if (theta <= M_PI/2+DELTA) {
                    trans_mat = get_rotation_matrix(0.02, Y);;
                } else {
                    finished = 1;
                }

                if(!finished) {
                    look_at_pos = *mat_mult_vec(trans_mat, &look_at_pos);
                } else {
                    enum cardinal temp_fwd;
                    if(curr_forward == EAST) temp_fwd = NORTH;
                    else if(curr_forward == SOUTH) temp_fwd = EAST;
                    else if(curr_forward == WEST) temp_fwd = SOUTH;
                    else if(curr_forward == NORTH) temp_fwd = WEST;
                    state_counter++;
                    curr_state = solve_states[state_counter];
                    old_look_at = look_at_pos;
                    rot_amnt = 0.0;
                    curr_forward = temp_fwd;
                }

                model_view = *look_at(&eyes, &look_at_pos, &up_vec);

            }
        break;
        case SOLVED:
            {
                mat4* trans_mat;
                int finished = 0;

                GLfloat theta = vec_angle_btw(old_look_at.vec[X], old_look_at.vec[Z], look_at_pos.vec[X], look_at_pos.vec[Z]);

                if (theta <= M_PI - 0.05) {
                    trans_mat = get_rotation_matrix(-0.02, Y);
                } else {
                    finished = 1;
                }

                if(!finished) {
                    look_at_pos = *mat_mult_vec(trans_mat, &look_at_pos);
                } else {
                    look_at_pos = *mat_mult_vec(&id, &look_at_pos);
                }

                model_view = *look_at(&eyes, &look_at_pos, &up_vec);

            }
        break;
    }
    #else
        mat4* trans_mat;
        int finished = 0;
        if(start_solve) {
            GLfloat theta = vec_angle_btw(old_look_at.vec[X], old_look_at.vec[Z], look_at_pos.vec[X], look_at_pos.vec[Z]);

            if (theta <= M_PI/2) {
                trans_mat = get_rotation_matrix(-0.02, Y);
            } else {
                finished = 1;
            }

            if(!finished) {
                look_at_pos = *mat_mult_vec(trans_mat, &look_at_pos);
            }

            model_view = *look_at(&eyes, &look_at_pos, &up_vec);
        }
    #endif
    glutPostRedisplay();
}

/**
 * From the circle.c file
*/
void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);
    
    if(key == ' '){
        start_solve = GL_TRUE;
    }

    glutPostRedisplay();
}