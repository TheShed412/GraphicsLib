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
#define NUM_VERTICES 5000
#define TIMER 1

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
void idle(int);
int tor_band(vec4* vertices, GLfloat twist, int axis, GLfloat end, GLfloat start, int index, float taper);
void translate_vertices(vec4* vertices, GLfloat x, GLfloat y, GLfloat z);
void rotate_vertices(vec4* vertices, GLfloat twist, int axis);
void scale_vertices(vec4* vertices, GLfloat x, GLfloat y, GLfloat z);

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Peoject 1");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(TIMER, idle, 0);
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
vec4* bottom(int num_vertices, GLfloat twist, int axis)
{
    GLfloat x_trans = 0.0;
    GLfloat y_trans = 0.5;
    int num_bands = NUM_VERTICES/216;
    float theta, theta_r, theta10_r;
    int index = 0;
    vec4 *vertices = (vec4 *) malloc (sizeof(vec4) * num_vertices);
    GLfloat dist = 0.5;
    GLfloat start, end;
    for (int i=1; i <= num_bands; i ++) {
        end = dist * i;
        start = end - dist;
        index = tor_band(vertices, twist, axis, end, start, index, 0.01);
    }


    GLfloat scale_factor = 0.20;
    rotate_vertices(vertices, twist, axis);
    translate_vertices(vertices, x_trans, y_trans, 0);
    scale_vertices(vertices, scale_factor, scale_factor, scale_factor);
    return vertices;
}

void rotate_vertices(vec4* vertices, GLfloat twist, int axis) {
    vec4 temp;
    for (int i=0; i < NUM_VERTICES; i++) {
        temp = vertices[i];
        vertices[i] = *rotation(&temp, twist, axis);
    }
}

void translate_vertices(vec4* vertices, GLfloat x, GLfloat y, GLfloat z) {
    vec4 temp;
    for (int i=0; i < NUM_VERTICES; i++) {
        temp = vertices[i];
        vertices[i] = *translation(&temp, x, y, z);
    }
}

void scale_vertices(vec4* vertices, GLfloat x, GLfloat y, GLfloat z) {
    vec4 temp;
    for (int i=0; i < NUM_VERTICES; i++) {
        temp = vertices[i];
        vertices[i] = *scale(&temp, x, y, z);
    }
}

int tor_band(vec4* vertices, GLfloat twist, int axis, GLfloat end, GLfloat start, int index, float taper) {
    float theta_r, theta10_r, theta, z_diff;
    for(theta = 0; theta <= 350; theta += 10) {
        theta_r = theta * M_PI / 180.0;
        theta10_r = (theta + 10) * M_PI / 180.0;
        GLfloat x_trans = 0.0;
        GLfloat y_trans = 0.5;


        /* the vectors for the slopes of the cone */
        vec4 first = (vec4){cos(theta_r), sin(theta_r), start-z_diff, 1};
        vec4 second = (vec4){cos(theta_r), sin(theta_r), end+z_diff, 1};
        vec4 third = (vec4){cos(theta10_r), sin(theta10_r), end+z_diff, 1};

        /* the vectors for the base of the cone */
        vec4 first_base = (vec4){cos(theta10_r), sin(theta10_r), end+z_diff, 1.0};
        vec4 second_base = (vec4){cos(theta_r), sin(theta_r), start-z_diff, 1.0};
        vec4 third_base = (vec4){cos(theta10_r), sin(theta10_r), start-z_diff, 1.0};

        /* add all vertecies */
        vertices[index + 0] = third;
        vertices[index + 1] = second;
        vertices[index + 2] = first;
        vertices[index + 5] = third_base;
        vertices[index + 4] = second_base;
        vertices[index + 3] = first_base;
	    index += 6;
        if (theta < 180) {
            z_diff -= taper;
        } else {
            z_diff += taper;
        }
        
    }

        return index;
}

/**
 * From the circle.c file with a couple small changes
*/
void init(void)
{
    GLuint program = initShader("shaders/vshader_lab4.glsl", "shaders/fshader_lab4.glsl");
    glUseProgram(program);

    vec4 *circle_vertices = bottom(NUM_VERTICES, 0, X);
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

void idle(int value) 
{
    glutTimerFunc(TIMER, idle, 0);
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