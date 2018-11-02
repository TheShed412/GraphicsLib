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

/* numTriangles = 360/degreePerVertex */
/* for the circle, vertices are 3 x numTriangles */
/* for the cone, vertices are 6 x numTriangles */
#define NUM_VERTICES 108

GLuint ctm_location;
mat4 ctm =             {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

vec4 all_cube_vertices[108];
vec4 all_cube_colors[108];

vec4 twin_cube_location = {0.0, 0.5, 0.0, 1.0};
vec4 left_cube_location = {-0.5, -0.5, 0.0, 1.0};
vec4 right_cube_location = {0.5, -0.5, 0.0, 1.0};

GLfloat twin_cube_degree = 0.0, left_cube_degree = 0.0, right_cube_degree = 0.0;

mat4 twin_cube_ctm =   {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};
mat4 left_cube_ctm =   {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};
mat4 right_cube_ctm =  {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

void init(void);
void display(void);
void keyboard(unsigned char key, int mousex, int mousey);
void idle(void);
vec4* single_cube();

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lab6 Cubes");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}


/* assuming the size is 36 cause I don't think I'll use any other size */
vec4* set_cube_colors(){
    vec4* cube_colors = calloc(NUM_VERTICES, NUM_VERTICES*sizeof(vec4));
    int index = 0;
    GLfloat r, g, b;
    for(int i=0; i<NUM_VERTICES/6; i++) {
        int rem = i%6;
        switch(rem) {
            case 0:
                r = 1.0;
                g = 0.0;
                b = 0.0;
            break;
            case 1:
                r = 0.0;
                g = 1.0;
                b = 0.0;
            break;
            case 2:
                r = 0.0;
                g = 0.0;
                b = 1.0;
            break;
            case 3:
                r = 1.0;
                g = 1.0;
                b = 0.0;
            break;
            case 4:
                r = 1.0;
                g = 0.0;
                b = 1.0;
            break;
            case 5:
                r = 0.0;
                g = 1.0;
                b = 1.0;
            break;
        }

        cube_colors[index + 0] = (vec4){r, g, b, 1.0};
        cube_colors[index + 1] = (vec4){r, g, b, 1.0};
        cube_colors[index + 2] = (vec4){r, g, b, 1.0};
        cube_colors[index + 3] = (vec4){r, g, b, 1.0};
        cube_colors[index + 4] = (vec4){r, g, b, 1.0};
        cube_colors[index + 5] = (vec4){r, g, b, 1.0};
        index += 6;
    }
    return cube_colors;
}

vec4* twin_cubes(){
    vec4* cubes = calloc(72, 72*sizeof(vec4));
    GLfloat scale_half = 0.5;
    vec4 up_half = {0, 0.5, 0, 0};
    vec4 right_half = {0.5, 0, 0, 0};
    vec4 left_half = {-0.5, 0, 0, 0};

    vec4* cube1 = const_scale(single_cube(), 36, scale_half);
    cube1 = translate_vertices(cube1, 36, up_half.vec[X], up_half.vec[Y], up_half.vec[Z]);
    cube1 = translate_vertices(cube1, 36, left_half.vec[X], left_half.vec[Y], left_half.vec[Z]);

    for (int i=0; i < 36; i ++)
        cubes[i] = cube1[i];

    vec4* cube2 = const_scale(single_cube(), 36, scale_half);
    cube2 = translate_vertices(cube2, 36, up_half.vec[X], up_half.vec[Y], up_half.vec[Z]);
    cube2 = translate_vertices(cube2, 36, right_half.vec[X], right_half.vec[Y], right_half.vec[Z]);

    for (int i=36; i < 72; i++)
        cubes[i] = cube2[i-36];

    return cubes;
}

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

/**
 * From the circle.c file with a couple small changes
*/
void init(void)
{
    GLuint program = initShader("shaders/vshader_lab6.glsl", "shaders/fshader_lab6.glsl");
    glUseProgram(program);

    vec4* cube_vertices = calloc(NUM_VERTICES, NUM_VERTICES*sizeof(vec4));

    vec4* twin_verts = twin_cubes();
    vec4* other_cubes = single_cube();
    vec4* circle_colors = set_cube_colors();

    int i;
    for (i=0; i < 72; i ++) {
        cube_vertices[i] = twin_verts[i];
    }

    for (i=72; i<NUM_VERTICES; i++) {
        cube_vertices[i] = other_cubes[i-72];
    }
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * NUM_VERTICES, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * NUM_VERTICES, cube_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * NUM_VERTICES, sizeof(vec4) * NUM_VERTICES, circle_colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * NUM_VERTICES));

    ctm_location = glGetUniformLocation(program, "ctm");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    //glPolygonMode(GL_BACK, GL_LINE);

    /* twin cubes */
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &twin_cube_ctm);
    glDrawArrays(GL_TRIANGLES, 0, 72);

    /* left cube */
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &left_cube_ctm);
    glDrawArrays(GL_TRIANGLES, 72, 36);

    /* right cube */
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &right_cube_ctm);
    glDrawArrays(GL_TRIANGLES, 72, 36);

    glutSwapBuffers();
}

void idle(void) 
{   
    vec4 down_left = {-0.5, -0.5, 0, 1};
    vec4 down_right = {0.5, -0.5, 0, 1};

    mat4* down_left_mat = get_translation_matrix(down_left.vec[X], down_left.vec[Y], down_left.vec[Z]);
    mat4* down_right_mat = get_translation_matrix(down_right.vec[X], down_right.vec[Y], down_right.vec[Z]);
    mat4* left_cube_rot_mat = get_rotation_matrix(left_cube_degree, Z);
    mat4* right_cube_rot_mat = get_rotation_matrix(right_cube_degree, X);
    mat4* scale_mat = get_scaling_matrix_con(0.5);

    mat4* left_trans_mat = mat4_mult(down_left_mat, mat4_mult(left_cube_rot_mat, scale_mat));
    mat4* right_trans_mat = mat4_mult(down_right_mat, mat4_mult(right_cube_rot_mat, scale_mat));
    mat4* twin_cube_rotation = get_rotation_matrix(twin_cube_degree, Y);
    right_cube_ctm = *right_trans_mat;
    left_cube_ctm = *left_trans_mat;
    twin_cube_ctm = *twin_cube_rotation;

    twin_cube_degree += 0.01;
    right_cube_degree += 0.01;
    left_cube_degree += 0.01;
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