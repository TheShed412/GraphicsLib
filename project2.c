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

int total_world_vertices = 36;

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

vec4* ground() {
    vec4* starting_cube = single_cube();
    starting_cube = scale_vertices(starting_cube, VERTS_IN_CUBE, 0.8, 0.1, 0.8);
    starting_cube = rotate_vertices(starting_cube, VERTS_IN_CUBE, 0.1, Y);
    starting_cube = rotate_vertices(starting_cube, VERTS_IN_CUBE, 0.1, X);

    return starting_cube;
}

pos_tex* ground_with_tex() {
    pos_tex* starting_cube = single_cube_texture(GRASS);
    vec4* starting_cube_pos = get_pos_verts(starting_cube, VERTS_IN_CUBE);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, 0.8, 0.1, 0.8);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, 20, 20, 20);

    for (int i = 0; i < VERTS_IN_CUBE; i++) {
        starting_cube[i].pos_vert = starting_cube_pos[i];
    }

    return starting_cube;
}

pos_tex* wall_with_tex() {
    int scale_amt = 3;
    pos_tex* starting_cube = single_cube_texture(BRICK);
    vec4* starting_cube_pos = get_pos_verts(starting_cube, VERTS_IN_CUBE);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, 0.1, 0.8, 0.8);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, scale_amt, scale_amt, scale_amt);

    for (int i = 0; i < VERTS_IN_CUBE; i++) {
        starting_cube[i].pos_vert = starting_cube_pos[i];
    }

    return starting_cube;
}

pos_tex* pillar_with_tex() {
    int scale_amt = 3;
    pos_tex* starting_cube = single_cube_texture(PILLAR);
    vec4* starting_cube_pos = get_pos_verts(starting_cube, VERTS_IN_CUBE);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, 0.15, 0.85, 0.15);
    starting_cube_pos = scale_vertices(starting_cube_pos, VERTS_IN_CUBE, scale_amt, scale_amt, scale_amt);

    for (int i = 0; i < VERTS_IN_CUBE; i++) {
        starting_cube[i].pos_vert = starting_cube_pos[i];
    }

    return starting_cube;
}

pos_tex* translate_pos_verts(pos_tex* tex_pos, int num_verts, GLfloat x, GLfloat y, GLfloat z) {
    vec4* pos_verts = get_pos_verts(tex_pos, num_verts);
    pos_verts = translate_vertices(pos_verts, num_verts, x, y, z);

    for (int i = 0; i < num_verts; i++) {
        tex_pos[i].pos_vert = pos_verts[i];
    }

    return tex_pos;
}

pos_tex* wall_with_pillar() {
    pos_tex* wall_pillar = empty_cube_arr(2);
    pos_tex* wall = wall_with_tex();
    pos_tex* pillar = pillar_with_tex();

    pillar = translate_pos_verts(pillar, VERTS_IN_CUBE, 0, 0, 1.35);

    for (int i = 0; i < VERTS_IN_CUBE; i ++) {
        wall_pillar[i] = wall[i];
    }

    for (int i = 36; i < VERTS_IN_CUBE * 2; i ++) {
        wall_pillar[i] = pillar[i - 36];
    }

    return wall_pillar;
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

    pos_tex* ground_tex_pos = wall_with_pillar();
    total_world_vertices *= 2;
    vec2* tex_coords = get_tex_verts(ground_tex_pos, total_world_vertices);
    vec4 *ground_vertices = get_pos_verts(ground_tex_pos, total_world_vertices);

    vec4 eyes = {0.0, 1.5, -3, 1};
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
        , sizeof(vec4) * total_world_vertices + sizeof(vec4)*total_world_vertices + sizeof(vec2) *total_world_vertices 
        , NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * total_world_vertices, ground_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * total_world_vertices , sizeof(vec2) *total_world_vertices, tex_coords);


    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * total_world_vertices));

    GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
    glEnableVertexAttribArray(vTexCoord);
    glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0 + (sizeof(vec4) * total_world_vertices));

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

    glDrawArrays(GL_TRIANGLES, 0, total_world_vertices);

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