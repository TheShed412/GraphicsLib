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
#define NUM_VERTICES 15552
#define TIMER 1
#define WIDTH 1024
#define HEIGHT 1024

GLuint ctm_location;
mat4 ctm =             {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

GLuint scale_ctm_location;
mat4 scale_ctm =             {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};


vec4* genRandomTriangleColors(int num_vertices);
vec4* bottom(int num_vertices, GLfloat twist, int axis);
void init(void);
void display(void);
void keyboard(unsigned char key, int mousex, int mousey);
void idle(int);
vec4* tor_band(GLfloat end, GLfloat start);
void translate_vertices(vec4* vertices, GLfloat x, GLfloat y, GLfloat z);
void rotate_vertices(vec4* vertices, GLfloat twist, int axis);
void scale_vertices(vec4* vertices, GLfloat x, GLfloat y, GLfloat z);
void rotate_n_vertices(vec4* vertices, GLfloat twist, int axis, int num_vertices);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
vec4* get_ball_vec(int x, int y);

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Peoject 1");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(TIMER, idle, 0);
    glutMotionFunc(motion);
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
    GLfloat radius = 3.5;
    int num_bands = NUM_VERTICES/216;
    GLfloat delta_theta = 5;

    GLfloat theta, theta_r;
    int index = 0;
    vec4 *vertices = (vec4 *) malloc (sizeof(vec4) * num_vertices);
    theta_r = delta_theta * M_PI / 180.0;
    GLfloat dist = radius * tan(theta_r);
    GLfloat start, end;
    for (theta = 0; theta <= 360-delta_theta; theta+=delta_theta) {
        theta_r = theta * M_PI / 180.0;
        end = dist;
        start = end - dist;
        vec4* band = tor_band(end, start);
        translate_vertices(band, -1*(radius-1.25), 0, 0);
        rotate_n_vertices(band, theta_r, Y, 216);
    
        for(int i=0; i<216; i++) {
            vertices[index + i] = band[i];
        }
        index += 216;
        free(band);
    }


    GLfloat scale_factor = 0.20;
    //rotate_vertices(vertices, 0.5, axis);
    //translate_vertices(vertices, x_trans, y_trans, 0);
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

void rotate_n_vertices(vec4* vertices, GLfloat twist, int axis, int num_vertices) {
    vec4 temp;
    for (int i=0; i < num_vertices; i++) {
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

vec4* tor_band(GLfloat end, GLfloat start) {
    float theta_r, theta10_r, theta, z_diff;
    z_diff = 0;
    int index = 0;
    vec4* temp_vec_arr = calloc(216, sizeof(vec4) * 216);
    for(theta = 0; theta <= 350; theta += 10) {
        theta_r = theta * M_PI / 180.0;
        theta10_r = (theta + 10) * M_PI / 180.0;

        /* the vectors for the slopes of the cone */
        vec4 first = (vec4){cos(theta_r), sin(theta_r), start-z_diff, 1};
        vec4 second = (vec4){cos(theta_r), sin(theta_r), end+z_diff, 1};
        vec4 third = (vec4){cos(theta10_r), sin(theta10_r), end+z_diff, 1};
        temp_vec_arr[index + 0] = third;
        temp_vec_arr[index + 1] = second;
        temp_vec_arr[index + 2] = first;

        /* the vectors for the base of the cone */
        vec4 first_base = (vec4){cos(theta10_r), sin(theta10_r), end+z_diff, 1.0};
        vec4 second_base = (vec4){cos(theta_r), sin(theta_r), start-z_diff, 1.0};
        vec4 third_base = (vec4){cos(theta10_r), sin(theta10_r), start-z_diff, 1.0};
        temp_vec_arr[index + 3] = first_base;
        temp_vec_arr[index + 4] = second_base;
        temp_vec_arr[index + 5] = third_base;
	    index += 6;
    }
    return temp_vec_arr;
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
    scale_ctm_location =  glGetUniformLocation(program, "scale_ctm");

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
    glUniformMatrix4fv(scale_ctm_location, 1, GL_FALSE, (GLfloat *) &scale_ctm);

    glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

    glutSwapBuffers();
}

int curr_x = 0;
int curr_y = 0;
int prev_x = 0;
int prev_y = 0;
int ball_grabbed = 0;
GLfloat theta = 0.0;

void idle(int value) 
{   
    glutTimerFunc(TIMER, idle, 0);

    // if the torus moved
    if(curr_x != prev_x || curr_y != prev_y) {
        // get the start and ending vectors
        vec4* start_vec = get_ball_vec(prev_x, prev_y);
        vec4* end_vec = get_ball_vec(curr_x, curr_y);
        GLfloat dot_prod = vec_mult(start_vec, end_vec);
        theta = acos((dot_prod > 1.0) ? 1.0 : dot_prod);
        prev_x = curr_x;
        prev_y = curr_y;
    }

    mat4* rotation_matrix = get_rotation_matrix(theta, Y);
    ctm = *rotation_matrix;
    // spin += 0.01;
    glutPostRedisplay();
}

/**
 * From the circle.c file
*/
GLfloat scale_up = 1.02;
GLfloat scale_dn = 1/1.02;
mat4* scale_mat = NULL;
void keyboard(unsigned char key, int mousex, int mousey)
{
    if(scale_mat == NULL)
        scale_mat = get_scaling_matrix_con(1);
    mat4* temp;
    switch(key) {
        case 'w':
            temp = get_scaling_matrix_con(scale_up);
            scale_mat = mat4_mult(scale_mat, temp);
        break;
        case 's':
            temp = get_scaling_matrix_con(scale_dn);
            scale_mat = mat4_mult(scale_mat, temp);
        break;
        default:
            scale_mat = get_scaling_matrix_con(1);
    }
    scale_ctm = *scale_mat;
    if(key == 'q')
    	exit(0);

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    
    if(scale_mat == NULL)
        scale_mat = get_scaling_matrix_con(1);
    mat4* temp;
    switch(button) {
        case 3:
            temp = get_scaling_matrix_con(scale_up);
            scale_mat = mat4_mult(scale_mat, temp);
        break;
        case 4:
            temp = get_scaling_matrix_con(scale_dn);
            scale_mat = mat4_mult(scale_mat, temp);
        break;
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN) {
                ball_grabbed = 1;
                curr_x = x;
                curr_y = y;
                prev_x = curr_x;
                prev_y = curr_y;
            } else {
                ball_grabbed = 0;
            }
        break;
        default:
            scale_mat = get_scaling_matrix_con(1);
    }
    scale_ctm = *scale_mat;
    glutPostRedisplay();
}

void motion(int x, int y) {
    if(ball_grabbed) {
        curr_x = x;
        curr_y = y;
    }
}

vec4* get_ball_vec(int x, int y) {
    vec4* ball_vec = calloc(1, sizeof(vec4));

    GLfloat proj_x = (x/(WIDTH*1.0)) * 2.0 - 1.0;
    GLfloat proj_y = -1*((y/(HEIGHT*1.0)) * 2.0 - 1.0);
    GLfloat sqr_dist = proj_x*proj_x + proj_y*proj_y;
    GLfloat proj_z;
    if(sqr_dist <= 1.0) {
        proj_z = sqrt(1 - sqr_dist);
    } else {
        proj_z = 0;
        proj_x = ((proj_x > 1.0) ? 1.0 : proj_x);
        proj_x = ((proj_x < -1.0) ? -1.0 : proj_x);
        proj_y = ((proj_y > 1.0) ? 1.0 : proj_y);
        proj_y = ((proj_y < -1.0) ? -1.0 : proj_y);
    }

    ball_vec->vec[X] = proj_x;
    ball_vec->vec[Y] = proj_y;
    ball_vec->vec[Z] = proj_z;

    return ball_vec;
}