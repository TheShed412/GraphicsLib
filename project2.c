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

enum direction {FORWARD, BACKWARDS, RIGHT, LEFT};

enum direction solver_dir[] = {FORWARD, RIGHT, FORWARD, FORWARD, FORWARD, FORWARD, FORWARD, FORWARD, FORWARD, LEFT, FORWARD, FORWARD, 
                               FORWARD, FORWARD, FORWARD, FORWARD, FORWARD, FORWARD, LEFT, LEFT};

typedef struct {
    int i;
    int j;
    enum direction anim;
} dir;

int whole_maze_cubes = 512;

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

vec4 eyes_maze_start = {-10, 11, -10, 1};
vec4 look_at_maze_start = {0, 11, -10, 1};
vec4 eyes = {0, 20, -20, 1};// starting point {-10, 11, -10, 1}
vec4 look_at_pos = {0, 0, 0, 1};// starting point {0, 11, -10, 1}
vec4 up_vec = {0, 1, 0, 1};
vec4 to_maze_look;

vec4* bottom(int num_vertices, GLfloat twist, int axis);
void init(void);
void display(void);
void keyboard(unsigned char key, int mousex, int mousey);
void idle(void);

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
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}

mat4* arbitrary_rotate(GLfloat z_theta, const vec4* axis) {
    mat4* final_mat = calloc(1, sizeof(mat4));

    /* step 1: translate to origin */
    vec4* to_origin = vec_sub(&origin, axis);
    //mat4* trans_to_origin = get_translation_matrix(to_origin->vec[X], to_origin->vec[Y], to_origin->vec[Z]);
    //mat4* trans_from_origin = get_translation_matrix(axis->vec[X], axis->vec[Y], axis->vec[Z]);
    GLfloat size_of_axis = sqrt(axis->vec[X]*axis->vec[X] + axis->vec[Z]*axis->vec[Z] + axis->vec[Y]*axis->vec[Y]);
    vec4* at_origin = scalar_mult_vec4(axis, 1/size_of_axis);

    /* I might need to check which way to spin on the Y and X axes */
    /* step 2: rotate around Y axis to the Z-Y plane */
    GLfloat vx = at_origin->vec[X];
    GLfloat vz = at_origin->vec[Z];
    GLfloat vy = at_origin->vec[Y];
    GLfloat d = sqrt(vy*vy + vz*vz);
    mat4 rot_to_yz = {1, 0, 0, 0,
                 0, vz/d, vy/d, 0,
                 0, -vy/d, vz/d, 0,
                 0, 0, 0, 1};
    vec4* at_yz = mat_mult_vec(&rot_to_yz, at_origin);

    /* step 3: rotate around X-axis to the Z,-axis */
    GLfloat z_adj = sqrt(vz*vz + vx*vx);
    mat4 rot_to_z = {d, 0, vx, 0,
                 0, 1, 0, 0,
                 -vx, 0, d, 0,
                 0, 0, 0, 1};
    vec4* at_z = mat_mult_vec(&rot_to_z, at_yz);

    /* step 4: rotate around the Z-axis */
    mat4* rot_around_z = get_rotation_matrix(-1*z_theta, Z);

    /* step 5: undo everything and multiply to make the mother of all matrices */
    //mat4* do_rotate_mat = mat4_mult(rot_to_yz, trans_to_origin);
    mat4*do_rotate_mat = mat4_mult(&rot_to_z, &rot_to_yz);
    do_rotate_mat = mat4_mult(rot_around_z, do_rotate_mat);

    mat4* rot_from_z = inverse_mat4(&rot_to_z);
    mat4* rot_from_yz = inverse_mat4(&rot_to_yz);

    mat4* undo_rotate = mat4_mult(rot_from_yz, rot_from_z);
    //undo_rotate = mat4_mult(trans_from_origin, undo_rotate);

    final_mat = mat4_mult(undo_rotate, do_rotate_mat);
    return final_mat;
}

pos_tex* cell_3d() {
    pos_tex* grnd_pillar = empty_cube_arr(8);
    pos_tex* wp1 = wall_with_pillar(GL_TRUE);
    pos_tex* wp2 = wall_with_pillar(GL_FALSE);
    pos_tex* wp3 = wall_with_pillar(GL_TRUE);
    pos_tex* wp4 = wall_with_pillar(GL_TRUE);

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

pos_tex* whole_maze() {
    pos_tex* maze_shapess = empty_cube_arr(whole_maze_cubes);
    cell** maze_cells = make_maze();

    GLfloat trans_x = 0;
    GLfloat trans_z = 0;
    int flip = 1;
    int inner = 0;
    int bound = VERTS_IN_CUBE*8;
    int diff = 0;
    int size = 8;
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j++) {
            cell curr_cell = maze_cells[i][j];
            pos_tex* curr_shape_cell = cell_walls(&curr_cell);
            curr_shape_cell = translate_pos_verts(curr_shape_cell, VERTS_IN_CUBE*8, 
                trans_x, 0, trans_z);
            
            for (; inner < bound; inner++) {
                maze_shapess[inner] = curr_shape_cell[inner - diff];
            }
            inner = bound;
            diff = bound;
            bound += VERTS_IN_CUBE*8;

            trans_z += ((1.35*2));
        }
        trans_x += (1.35*2);
        trans_z = 0;
    }

    return maze_shapess;
}

pos_tex* whole_maze_and_ground() {
    pos_tex* whole_shabang = empty_cube_arr(whole_maze_cubes + 1);

    pos_tex* the_ground = ground_with_tex();
    pos_tex* the_maze = whole_maze();

    the_maze = translate_pos_verts(the_maze, VERTS_IN_CUBE*whole_maze_cubes, -10, 10, -10);

    for (int i=0; i < VERTS_IN_CUBE*512; i++) {
        whole_shabang[i] = the_maze[i];
    }

    for(int i=VERTS_IN_CUBE*512; i < VERTS_IN_CUBE*513; i++) {
        whole_shabang[i] = the_ground[i - (VERTS_IN_CUBE*512)];
    }
    to_maze_look = the_maze[0].pos_vert;
    return whole_shabang;
}

/**
 * From the circle.c file with a couple small changes
*/
void init(void)
{
    GLubyte*** grass_tex = get_textures();  
    cell** maze = make_maze();
    GLuint program = initShader("shaders/vshader_proj2.glsl", "shaders/fshader_proj2.glsl");
    glUseProgram(program);

    pos_tex* ground_tex_pos = whole_maze_and_ground();
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

GLfloat spin = 0.015;
GLfloat spin_total = 0.0;
GLfloat left_spin_total = 0.0;
GLboolean move_to_maze = GL_FALSE;
GLboolean start_solve = GL_FALSE;
GLfloat move = 0.01;
GLfloat east_move_tracker = 0;
GLfloat south_move_tracker = 0;

GLboolean move_east(GLfloat dist) {
    if (east_move_tracker < dist) {
        mat4* forward_mat = get_translation_matrix(east_move_tracker, 0, 0);
        eyes = *mat_mult_vec(forward_mat, &eyes);  
        look_at_maze_start = *mat_mult_vec(forward_mat, &look_at_maze_start);
        model_view = *look_at(&eyes, &look_at_maze_start, &up_vec);
        east_move_tracker += move;
    } else {
        //move_tracker = 0.0;
        return GL_TRUE;
    }
    return GL_FALSE;
}

GLboolean move_south() {
    if (south_move_tracker < 0.61) {
        mat4* forward_mat = get_translation_matrix(0, 0, south_move_tracker);
        eyes = *mat_mult_vec(forward_mat, &eyes);  
        look_at_maze_start = *mat_mult_vec(forward_mat, &look_at_maze_start);
        model_view = *look_at(&eyes, &look_at_maze_start, &up_vec);
        south_move_tracker += move;
    } else {
        //move_tracker = 0.0;
        return GL_TRUE;
    }
    return GL_FALSE;
}

GLboolean turn_right(GLfloat amt) {
    if (spin_total < amt) {
        mat4* rot_mat = arbitrary_rotate(spin_total, &up_vec);
        look_at_maze_start = *mat_mult_vec(rot_mat, &look_at_maze_start);  
        model_view = *look_at(&eyes, &look_at_maze_start, &up_vec);
        spin_total += spin/10;
    } else {
        //spin_total = 0.0;
        return GL_TRUE;
    }
    return GL_FALSE;
}

GLboolean turn_left() {
    if (left_spin_total < 0.016) {
        mat4* rot_mat = arbitrary_rotate(-spin, &up_vec);
        look_at_maze_start = *mat_mult_vec(rot_mat, &look_at_maze_start);  
        model_view = *look_at(&eyes, &look_at_maze_start, &up_vec);
        left_spin_total += spin/100;
    } else {
        //spin_total = 0.0;
        return GL_TRUE;
    }

    return GL_FALSE;
}

GLfloat east_dist = 0.17;
GLboolean done = GL_FALSE;

void idle(void) 
{
    if (move_to_maze) {
        // mat4* trans_mat = &id;
        // //if (eyes.vec[Y] >= to_maze_look.vec[Y]) {
        //     trans_mat = get_translation_matrix(to_maze_look.vec[X], to_maze_look.vec[Y], to_maze_look.vec[Z]);
        //     eyes = *mat_mult_vec(trans_mat, &eyes);
        // } else {
        //    // eyes = to_maze_look;
        // }
        //mat4* rotation_matrix = get_rotation_matrix(spin, Y); 
        
        if (start_solve) {
            if (done) {
                if(move_east(east_dist)){
                    turn_right(0.09);
                }
                
                //east_move_tracker = 1222;
            }
            if(!done && move_east(east_dist)) {
                if(turn_right(0.11)) {
                    if(move_south()) {
                        if (turn_left()) {
                            east_dist = 0.65;
                            east_move_tracker  = 0;
                            done = GL_TRUE;
                            spin_total = 0;
                        }
                    }
                }
            }
        } else {
            eyes = eyes_maze_start;
            model_view = *look_at(&eyes, &look_at_maze_start, &up_vec);
        }
    }else {
        mat4* rotation_matrix = get_rotation_matrix(spin, Y);
        eyes = *mat_mult_vec(rotation_matrix, &eyes);
        model_view = *look_at(&eyes, &look_at_pos, &up_vec);
    }
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
        move_to_maze = GL_TRUE;
    }

    if(move_to_maze) {
        if(key == 's'){
            start_solve = GL_TRUE;
        }
    }

    glutPostRedisplay();
}