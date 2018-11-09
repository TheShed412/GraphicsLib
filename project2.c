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

int whole_maze_cubes = 512;

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

// pos_tex* pillar_test() {
//     pos_tex* grnd_pillar = empty_cube_arr(3);
//     pos_tex* ground_tex_pos = ground_with_tex();
//     pos_tex* pillar_tex_pos = wall_with_pillar();
//     vec4 grndo = ground_tex_pos[30].pos_vert;

//     pillar_tex_pos = 
//         translate_pos_verts(pillar_tex_pos, VERTS_IN_CUBE*2, grndo.vec[X]+1, grndo.vec[Y]+1.2, grndo.vec[Z]-1);

    
//     for (int i = 0; i < VERTS_IN_CUBE; i ++) {
//         grnd_pillar[i] = ground_tex_pos[i];
//     }

//     for (int i = 36; i < VERTS_IN_CUBE * 3; i ++) {
//         grnd_pillar[i] = pillar_tex_pos[i - 36];
//     }

//     return grnd_pillar;
// }

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



pos_tex* whole_maze() {
    pos_tex* maze_shapess = empty_cube_arr(whole_maze_cubes);
    cell** maze_cells = make_maze();
    print_maze(maze_cells);

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

/**
 * From the circle.c file with a couple small changes
*/
void init(void)
{
    GLubyte*** grass_tex = get_textures();  
    cell** maze = make_maze();
    GLuint program = initShader("shaders/vshader_proj2.glsl", "shaders/fshader_proj2.glsl");
    glUseProgram(program);

    pos_tex* ground_tex_pos = whole_maze();
    total_vertices = get_total_verts();
    vec2* tex_coords = get_tex_verts(ground_tex_pos, total_vertices);
    vec4 *ground_vertices = get_pos_verts(ground_tex_pos, total_vertices);

    vec4 eyes = {0.0, 15, -15, 1};
    vec4 look_at_pos = {0, 1.2, 0, 1};
    vec4 up_vec = {0, 1, 0, 1};

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

GLfloat spin = 0.0;

void idle(void) 
{
    mat4* rotation_matrix = get_rotation_matrix(spin, Y);
    ctm = *rotation_matrix;
    spin += 0.01;
    glutPostRedisplay();
}

/**
 * From the circle.c file
*/
void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);

    glutPostRedisplay();
}