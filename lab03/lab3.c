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
#define NUM_VERTICES 216

vec4* genRandomTriangleColors(int num_vertices);
vec4* bottom(int num_vertices, GLfloat twist, int axis);
void init(void);
void display(void);
void keyboard(unsigned char key, int mousex, int mousey);

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Cone");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
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
    float theta, theta_r, theta10_r;
    int index = 0;
    vec4 *vertices = (vec4 *) malloc (sizeof(vec4) * num_vertices);
    for(theta = 0; theta <= 350; theta += 10)
    {
        theta_r = theta * M_PI / 180.0;
        theta10_r = (theta + 10) * M_PI / 180.0;
        GLfloat og_z = -1.25;
        GLfloat x_trans = 0.0;
        GLfloat y_trans = 0.5;

        /* the vectors for the slopes of the cone */
        vec4 first = (vec4){0.0, 0.0, og_z, 1.0};
        vec4 second = (vec4){cos(theta_r), sin(theta_r), 0.0, 1.0};
        vec4 third = (vec4){cos(theta10_r), sin(theta10_r), 0.0, 1.0};

        /* the vectors for the base of the cone */
        vec4 first_base = (vec4){0.0, 0.0, 0.0, 1.0};
        vec4 second_base = (vec4){cos(theta_r), sin(theta_r), 0.0, 1.0};
        vec4 third_base = (vec4){cos(theta10_r), sin(theta10_r), 0.0, 1.0};

        /* rotating slope vectors  */
        vec4* rot_first = rotation(&first, twist, axis);
        vec4* rot_second = rotation(&second, twist, axis);
        vec4* rot_third = rotation(&third, twist, axis);

        /* rotating base vectors */
        vec4* rot_first_base = rotation(&first_base, twist, axis);
        vec4* rot_second_base = rotation(&second_base, twist, axis);
        vec4* rot_third_base = rotation(&third_base, twist, axis);

        /* moving slopes along x axis to keep cone in view */
        vec4* tran_rot_first = translation(rot_first, x_trans, y_trans, 0.0);
        vec4* tran_rot_second = translation(rot_second, x_trans, y_trans, 0.0);
        vec4* tran_rot_third = translation(rot_third, x_trans, y_trans, 0.0);

        /* moving base along x axis to keep it in view */
        vec4* tran_rot_first_base = translation(rot_first_base, x_trans, y_trans, 0.0);
        vec4* tran_rot_second_base = translation(rot_second_base, x_trans, y_trans, 0.0);
        vec4* tran_rot_third_base = translation(rot_third_base, x_trans, y_trans, 0.0);

        /* add all vertecies */
        vertices[index + 0] = *tran_rot_third;
        vertices[index + 1] = *tran_rot_second;
        vertices[index + 2] = *tran_rot_first;
        vertices[index + 5] = *tran_rot_third_base;
        vertices[index + 4] = *tran_rot_second_base;
        vertices[index + 3] = *tran_rot_first_base;

        // vertices[index] = *tran_rot_first;
        // vertices[index + 1] = *tran_rot_third;
        // vertices[index + 2] = *tran_rot_third_base;
        // vertices[index + 3] = *tran_rot_second_base;
        // vertices[index + 4] = *tran_rot_second;
        // vertices[index + 5] = *tran_rot_first_base;
	    index += 6;

        // vec4 first = (vec4){0.0, 0.0, og_z, 1.0};
        // vec4 second = (vec4){cos(theta_r), sin(theta_r), og_z, 1.0};
        // vec4 third = (vec4){cos(theta10_r), sin(theta10_r), og_z, 1.0};

        // vec4 fourth = (vec4){0.0, 0.0, -10.0, 1.0};
        // vec4 fifth = (vec4){cos(theta_r), sin(theta_r), og_z, 1.0};
        // vec4 sixth = (vec4){cos(theta10_r), sin(theta10_r), og_z, 1.0};

        // vertices[index] = *(rotation(&first, twist, axis));
        // vertices[index + 1] = *(rotation(&second, twist, axis));
        // vertices[index + 2] = *(rotation(&third, twist, axis));
        // vertices[index + 3] = *(rotation(&fourth, twist, axis));
        // vertices[index + 4] = *(rotation(&fifth, twist, axis));
        // vertices[index + 5] = *(rotation(&sixth, twist, axis));
	    // index += 6;
    }

    return vertices;
}

/**
 * From the circle.c file with a couple small changes
*/
void init(void)
{
    GLuint program = initShader("shaders/vshader.glsl", "shaders/fshader.glsl");
    glUseProgram(program);

    vec4 *circle_vertices = bottom(NUM_VERTICES, 5, X);
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
    glDrawArrays(GL_TRIANGLES, 0, 72*3);

    glutSwapBuffers();
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