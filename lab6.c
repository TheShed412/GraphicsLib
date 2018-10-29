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
#define NUM_VERTICES 72

GLuint ctm_location;
mat4 ctm =             {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

vec4 all_cube_vertices[108];
vec4 all_cube_colors[108];

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
    vec4* cube_colors = calloc(72, 72*sizeof(vec4));
    int index = 0;
    GLfloat r, g, b;
    for(int i=0; i<72/6; i++) {
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
    cube[24].vec[X] = -x, cube[24].vec[Y] = y,  cube[24].vec[Z] = z;//2
    cube[25].vec[X] = x,  cube[25].vec[Y] = y,  cube[25].vec[Z] = z;//3
    cube[26].vec[X] = x,  cube[26].vec[Y] = y, cube[26].vec[Z] = -z;//7

    cube[27].vec[X] = -x,  cube[27].vec[Y] = y,cube[27].vec[Z] = z;//2
    cube[28].vec[X] = x, cube[28].vec[Y] = y, cube[28].vec[Z] = -z;//7
    cube[29].vec[X] = -x,  cube[29].vec[Y] = y, cube[29].vec[Z] = -z;//6

    return cube;
}

/**
 * From the circle.c file with a couple small changes
*/
void init(void)
{
    GLuint program = initShader("shaders/vshader_lab6.glsl", "shaders/fshader_lab6.glsl");
    glUseProgram(program);

    vec4 *circle_vertices = twin_cubes();
    vec4 *circle_colors = set_cube_colors();
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * NUM_VERTICES, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * NUM_VERTICES, circle_vertices);
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

/**
 * From the circle.c file with a couple small changes
*/
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);

    glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

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