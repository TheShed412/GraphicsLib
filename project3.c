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
#include "headers/shapes.h"
#include "headers/view.h"

#define NUM_VERTICES 6144
#define SPHERE_VERTS 6144
#define CUBE_VERTS 36
#define TIMER 1
#define WIDTH 1024
#define HEIGHT 1024

GLuint ctm_location;
GLuint perspective_shift;
GLuint cam_shit;
mat4 projection;
mat4 model_view;
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

int num_vertices = 0;

vec4 eyes = {0, 4, 4, 1};// starting point {-10, 11, -10, 1}
vec4 look_at_pos = {0, 0, 0, 1};// starting point {0, 11, -10, 1}
vec4 up_vec = {0, 1, 0, 1};

vec4* genRandomTriangleColors(int num_vertices);
void init(void);
void display(void);
void idle(int);
void keyboard(unsigned char key, int mousex, int mousey);


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
        // r = 0;
        // g = 0.4;
        // b = 0;

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

vec4* colored_verts(int num_vertices, GLfloat r, GLfloat g, GLfloat b) {
    int index = 0, i;
    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    for(i = 0; i < num_vertices / 3; i++)
    {
        colors[index] = (vec4){r, g, b, 1.0};
        colors[index + 1] = (vec4){r, g, b, 1.0};
        colors[index + 2] = (vec4){r, g, b, 1.0};
        index += 3;
    }

    return colors;
}

int add_to_scene(vec4* scene_verts, vec4* shape_verts, int shape_size, int offset) {
    int size = 0;

    for (int i=0; i < shape_size; i++) {
        scene_verts[offset + i] = shape_verts[i];
    }

    return shape_size;
}

void init(void)
{
    num_vertices = SPHERE_VERTS*5 + CUBE_VERTS;
    GLuint program = initShader("shaders/vshader_proj3.glsl", "shaders/fshader_proj3.glsl");
    glUseProgram(program);
    
    // 20X20 cube that has y=0
    vec4* ball1 = uv_sphere(0.5, 32, 32);
    ball1 = translate_vertices(ball1, SPHERE_VERTS, 0, 0.5, 0);
    //vec4* ball1_color = colored_verts(SPHERE_VERTS, 0.4, 0, 0);
    vec4* ball1_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* ball2 = uv_sphere(0.5, 32, 32);
    ball2 = translate_vertices(ball2, SPHERE_VERTS, 1, 0.5, 0);
    //vec4* ball1_color = colored_verts(SPHERE_VERTS, 0.4, 0, 0);
    vec4* ball2_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* ball3 = uv_sphere(0.5, 32, 32);
    ball3 = translate_vertices(ball3, SPHERE_VERTS, 2, 0.5, 0);
    //vec4* ball1_color = colored_verts(SPHERE_VERTS, 0.4, 0, 0);
    vec4* ball3_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* ball4 = uv_sphere(0.5, 32, 32);
    ball4 = translate_vertices(ball4, SPHERE_VERTS, 3, 0.5, 0);
    //vec4* ball1_color = colored_verts(SPHERE_VERTS, 0.4, 0, 0);
    vec4* ball4_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* ball5 = uv_sphere(0.5, 32, 32);
    ball5 = translate_vertices(ball5, SPHERE_VERTS, 4, 0.5, 0);
    //vec4* ball1_color = colored_verts(SPHERE_VERTS, 0.4, 0, 0);
    vec4* ball5_color = genRandomTriangleColors(SPHERE_VERTS);

    /* table cube */
    vec4* cube_verts = cube();
    cube_verts = scale_vertices(cube_verts, CUBE_VERTS, 40, 40, 40);
    cube_verts = translate_vertices(cube_verts, CUBE_VERTS, 0.0, -20, 0.0);
    vec4* cube_colors = colored_verts(CUBE_VERTS, 0, 0.4, 0);

    vec4* scene_verts = calloc(num_vertices, sizeof(vec4));
    vec4* scene_colors = calloc(num_vertices, sizeof(vec4));

    int curr_vert_size = add_to_scene(scene_verts, cube_verts, CUBE_VERTS, 0);
    curr_vert_size += add_to_scene(scene_verts, ball1, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, ball2, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, ball3, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, ball4, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, ball5, SPHERE_VERTS, curr_vert_size);

    int curr_col_size = add_to_scene(scene_colors, cube_colors, CUBE_VERTS, 0);
    curr_col_size += add_to_scene(scene_colors, ball1_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball2_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball3_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball4_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball5_color, SPHERE_VERTS, curr_col_size);

    int f = 1;
    projection = *frustum(-f, f, f, -f, -f, f);
    model_view = *look_at(&eyes, &look_at_pos, &up_vec);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * num_vertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, scene_verts);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, scene_colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices));

    ctm_location = glGetUniformLocation(program, "ctm");
    scale_ctm_location =  glGetUniformLocation(program, "scale_ctm");
    perspective_shift = glGetUniformLocation(program, "projection");
    cam_shit = glGetUniformLocation(program, "model_view");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
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
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
    glUniformMatrix4fv(perspective_shift, 1, GL_FALSE, (GLfloat *) &projection);
    glUniformMatrix4fv(cam_shit, 1, GL_FALSE, (GLfloat *) &model_view);

    glDrawArrays(GL_TRIANGLES, 0, num_vertices);

    glutSwapBuffers();
}

GLfloat spin = 0.0;

void idle(int value) 
{   
    glutTimerFunc(TIMER, idle, 0);
    // mat4* rotation_matrix = get_rotation_matrix(spin, Y);
    // ctm = *rotation_matrix;
    // spin += 0.01;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);

    glutPostRedisplay();
}