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

vec4 origin = {0, 0, 0, 0};


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
mat4* arbitrary_rotate(GLfloat z_theta, const vec4* axis);

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

GLfloat mag(const vec4* vec) {

    GLfloat x_sqr = vec->vec[X] * vec->vec[X];
    GLfloat y_sqr = vec->vec[Y] * vec->vec[Y];
    GLfloat z_sqr = vec->vec[Z] * vec->vec[Z];

    return sqrt(x_sqr + y_sqr + z_sqr);
}

int curr_x = 0;
int curr_y = 0;
int prev_x = 0;
int prev_y = 0;
int ball_grabbed = 0;
GLfloat theta = 0.0;
mat4* rot_mat = NULL;

void idle(int value) 
{   
    glutTimerFunc(TIMER, idle, 0);
    if (rot_mat == NULL) {
        rot_mat = get_rotation_matrix(theta, Y);
    }

    // if the torus moved
    if(curr_x != prev_x || curr_y != prev_y) {
        // get the start and ending vectors
        vec4* start_vec = get_ball_vec(prev_x, prev_y);
        vec4* end_vec = get_ball_vec(curr_x, curr_y);
        vec4* axis = vec_cross(end_vec, start_vec);
        GLfloat dot_prod = vec_mult(start_vec, end_vec);
        // dot_prod/mag_a * mag_b
        theta = acos((dot_prod > 1.0) ? 1.0 : dot_prod);

        prev_x = curr_x;
        prev_y = curr_y;
        free(start_vec);
        free(end_vec);
        
        rot_mat = mat4_mult(arbitrary_rotate(theta, axis), rot_mat);
    }
    ctm = *rot_mat;
    glutPostRedisplay();
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
    }

    ball_vec->vec[X] = proj_x;
    ball_vec->vec[Y] = proj_y;
    ball_vec->vec[Z] = proj_z;

    return ball_vec;
}