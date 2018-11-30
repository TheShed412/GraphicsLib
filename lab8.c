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

#define NUM_VERTICES 6180
#define CUBE_VERTS 36
#define SPHERE_VERTS 6144
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

GLfloat eyex, eyey, eyez;
GLfloat atx, aty, atz;
GLfloat lightx, lighty, lightz;

vec4 eyes = {0, 3, -3.5, 1};
vec4 look_at_pos = {0, 0, 0, 1};
vec4 up_vec = {0, 1, 0, 1};

vec4* genRandomTriangleColors(int num_vertices);
void init(void);
void display(void);
void idle(int);
void keyboard(unsigned char key, int mousex, int mousey);

int main(int argc, char **argv) 
{
    printf("Enter eye point: ");
    scanf("%f,%f,%f", &eyex, &eyey, &eyez);
    printf("Enter at point: ");
    scanf("%f,%f,%f", &atx, &aty, &atz);
    printf("Enter light point: ");
    scanf("%f,%f,%f", &lightx, &lighty, &lightz);
    eyes.vec[X] = eyex;
    eyes.vec[Y] = eyey;
    eyes.vec[Z] = eyez;
    look_at_pos.vec[X] = atx;
    look_at_pos.vec[Y] = aty;
    look_at_pos.vec[Z] = atz;
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

vec4* cube_colors(int num_vertices)
{
    GLfloat r, g, b;
    int index = 0, i;

    srand(time(0));

    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    for(i = 0; i < num_vertices / 6; i++)
    {
        // r = 0;
        // g = 0.4;
        // b = 0;

        r = rand() / (float) RAND_MAX;
        g = rand() / (float) RAND_MAX;
        b = rand() / (float) RAND_MAX;

        colors[index + 0] = (vec4){r, g, b, 1.0};
        colors[index + 1] = (vec4){r, g, b, 1.0};
        colors[index + 2] = (vec4){r, g, b, 1.0};
        colors[index + 3] = (vec4){r, g, b, 1.0};
        colors[index + 4] = (vec4){r, g, b, 1.0};
        colors[index + 5] = (vec4){r, g, b, 1.0};
        index += 6;
    }

    return colors;
}

vec4* vert_shadow(vec4* cube_verts, const vec4* light_pos, int size) {
    vec4* shadow = calloc(size, sizeof(vec4));

    GLfloat zs;
    GLfloat xs;

    GLfloat xl = light_pos->vec[X];
    GLfloat yl = light_pos->vec[Y];
    GLfloat zl = light_pos->vec[Z];

    GLfloat x;
    GLfloat y;
    GLfloat z;

    for (int i = 0; i < size; i++) {
        x = cube_verts[i].vec[X];
        y = cube_verts[i].vec[Y];
        z = cube_verts[i].vec[Z];

        xs = xl - (yl * ((xl-x)/(yl-y)));
        zs = zl - (yl * ((zl-z)/(yl-y)));
        shadow[i].vec[X] = xs;
        shadow[i].vec[Z] = zs;
        shadow[i].vec[W] = 1;
    }
    return shadow;
}

vec4* shadow_colors(int num_vertices)
{
    GLfloat r, g, b;
    int index = 0, i;

    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    for(i = 0; i < num_vertices / 3; i++)
    {
        r = 0;
        g = 0;
        b = 0;

        colors[index + 0] = (vec4){r, g, b, 1.0};
        colors[index + 1] = (vec4){r, g, b, 1.0};
        colors[index + 2] = (vec4){r, g, b, 1.0};
        index += 3;
    }

    return colors;
}
void init(void)
{
    GLuint program = initShader("shaders/vshader_lab8.glsl", "shaders/fshader_lab8.glsl");
    glUseProgram(program);
    vec4* cube_verts = cube();
    cube_verts = translate_vertices(cube_verts, CUBE_VERTS, 0.5, 0.5, 0);
    vec4* sphere_verts = uv_sphere(0.5, 32, 32);
    sphere_verts = translate_vertices(sphere_verts, SPHERE_VERTS, -0.5, 0.25, 0);

    vec4* total_vertices = calloc(NUM_VERTICES*2, sizeof(vec4));
    vec4* total_colors = calloc(NUM_VERTICES*2, sizeof(vec4));

    for (int i = 0; i < SPHERE_VERTS; i++) {
        total_vertices[i] = sphere_verts[i];
    }

    for (int i = 0; i < 36; i++) {
        total_vertices[SPHERE_VERTS+i] = cube_verts[i];
    }

    vec4* sphere_colors = genRandomTriangleColors(SPHERE_VERTS);
    vec4* cube_color = cube_colors(CUBE_VERTS);

    for (int i = 0; i < SPHERE_VERTS; i++) {
        total_colors[i] = sphere_colors[i];
    }

    for (int i = 0; i < 36; i++) {
        total_colors[SPHERE_VERTS+i] = cube_color[i];
    }

    int f = 1;
    projection = *frustum(-f, f, f, -f, -f, f);
    model_view = *look_at(&eyes, &look_at_pos, &up_vec);

    vec4 light_vec = {lightx, lighty, lightz, 1};

    vec4* cube_shadow = vert_shadow(cube_verts, &light_vec, CUBE_VERTS);
    vec4* sphere_shadow = vert_shadow(sphere_verts, &light_vec, SPHERE_VERTS);
    vec4* shadow_color = shadow_colors(NUM_VERTICES);

    for(int i = 0; i < NUM_VERTICES; i++) {
        total_colors[NUM_VERTICES+i] = shadow_color[i];
    }

    for (int i = 0; i < SPHERE_VERTS; i++) {
        total_vertices[NUM_VERTICES+i] = sphere_shadow[i];
    }

    for (int i = 0; i < CUBE_VERTS; i++) {
        total_vertices[NUM_VERTICES + SPHERE_VERTS + i] = cube_shadow[i];
    }
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * 2 * NUM_VERTICES*2, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * NUM_VERTICES*2, total_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * NUM_VERTICES*2, sizeof(vec4) * NUM_VERTICES*2, total_colors);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * NUM_VERTICES*2));

    ctm_location = glGetUniformLocation(program, "ctm");
    scale_ctm_location =  glGetUniformLocation(program, "scale_ctm");
    perspective_shift = glGetUniformLocation(program, "projection");
    cam_shit = glGetUniformLocation(program, "model_view");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
    glClearColor(1.0, 1.0, 1.0, 1.0);
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

    glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES*2);

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