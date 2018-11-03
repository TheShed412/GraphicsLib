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

/* numTriangles = 360/degreePerVertex */
/* for the circle, vertices are 3 x numTriangles */
/* for the cone, vertices are 6 x numTriangles */
#define NUM_VERTICES 36

GLuint ctm_location;
mat4 ctm =             {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

vec4* genRandomTriangleColors(int num_vertices);
vec4* bottom(int num_vertices, GLfloat twist, int axis);
void init(void);
void display(void);
void keyboard(unsigned char key, int mousex, int mousey);
void idle(void);

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

/**
 * From the circle.c file with a couple small changes
*/
vec4* genRandomTriangleColors(int num_vertices)
{
    GLfloat r, g, b;
    int index = 0, i;

    srand(time(0));

    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    for(i = 0; i < num_vertices / 3; i++)
    {
        r = rand() / (float) RAND_MAX;
        g = rand() / (float) RAND_MAX;
        b = rand() / (float) RAND_MAX;

        colors[index] = (vec4){r, g, b, 1.0};
        colors[index + 1] = (vec4){r, g, b, 1.0};
        colors[index + 2] = (vec4){r, g, b, 1.0};
        index += 3;
    }

    return colors;
}

/**
 * From the circle.c file with a couple small changes
*/
void init(void)
{
    GLuint program = initShader("shaders/vshader_lab4.glsl", "shaders/fshader_lab4.glsl");
    glUseProgram(program);

    vec4 *circle_vertices = single_cube();
    vec4 *circle_colors = genRandomTriangleColors(NUM_VERTICES);
    
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