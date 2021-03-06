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
#include "headers/lighting.h"

#define SPHERE_VERTS 6144
#define CUBE_VERTS 36
#define TIMER 1
#define WIDTH 1024
#define HEIGHT 1024

GLuint ctm_location;
GLuint ball_ctm_location;
GLuint perspective_shift;
GLuint cam_shit;
GLuint is_shadow_location;
GLuint light_point_location;
GLuint eye_location;

GLuint ap_location;
GLuint dp_location;
GLuint sp_location;

GLuint ac_location;
GLuint al_location;
GLuint aq_location;

GLuint shininess_location;

mat4 projection;
mat4 model_view;
mat4 ctm =             {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

mat4 ball1_ctm =       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

mat4 ball2_ctm =       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

mat4 ball3_ctm =       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

mat4 ball4_ctm =       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

mat4 ball5_ctm =       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

mat4 light_ctm =       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

GLuint scale_ctm_location;
mat4 scale_ctm =       {1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1};

vec4 origin = {0, 0, 0, 0};
vec4 light_point = {0, 3.0, 0, 1};

int num_vertices = 0;

vec4 eyes = {0, 5, 5, 1};// starting point {-10, 11, -10, 1}
vec4 look_at_pos = {0, 0, 0, 1};// starting point {0, 11, -10, 1}
vec4 up_vec = {0, 1, 0, 1};

vec4 light_diffuse = {1.0, 1.0, 1.0, 1.0};
vec4 light_specular = {1.0, 1.0, 1.0, 1.0};
vec4 light_ambient = {0.2, 0.2, 0.2, 1.0};

GLfloat shininess = 0.2;

material ball_materials[5] = {
    {{1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10},
    {{0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10},
    {{0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10},
    {{1.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10},
    {{1.0, 0.5, 0.0, 1.0}, {1.0, 0.5, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 10}
};

material table_material = {{0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 1};
material light_material = {{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, 1};

vec4* ball1_normals;
vec4* ball2_normals;
vec4* ball3_normals;
vec4* ball4_normals;
vec4* ball5_normals;
vec4* light_normals;
vec4* cube_normals;

vec4* genRandomTriangleColors(int num_vertices);
void init(void);
void display(void);
void idle(int);
void keyboard(unsigned char key, int mousex, int mousey);
void handle_key_up(unsigned char key, int x, int y) ;
void special_keyboard(int key, int mousex, int mousey);
void handle_special_key_up(int key, int x, int y);

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Peoject 3");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(handle_key_up);
    glutSpecialFunc(special_keyboard);
    glutSpecialUpFunc(handle_special_key_up);
    glutTimerFunc(TIMER, idle, 0);
    glutMainLoop();

    return 0;
}

vec4* vec4_product(const vec4* vec1, const vec4* vec2) {
    vec4* result = (vec4*)calloc(1, sizeof(vec4));

    result->vec[X] = vec1->vec[X] * vec2->vec[X];
    result->vec[Y] = vec1->vec[Y] * vec2->vec[Y];
    result->vec[Z] = vec1->vec[Z] * vec2->vec[Z];
    result->vec[W] = vec1->vec[W] * vec2->vec[W];

    return result;
}

mat4* arbitrary_rotate(GLfloat z_theta, vec4* axis) {
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

/* assuming the thing is at origin */
vec4* normal_verts(const vec4* shape_verts, int num_vertices) {
    int i;
    vec4 *normals = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    for(i = 0; i < num_vertices; i++)
    {
        vec4 vert1 = shape_verts[i];

        vec4* vector1 = vec_sub(&vert1, &origin);
        vector1->vec[W] = 0;
        
        normals[i] = *vector1;
    }

    return normals;
}

vec4* colored_verts(int num_vertices, GLfloat r, GLfloat g, GLfloat b) {
    int index = 0, i;
    vec4 *colors = (vec4 *) malloc(sizeof(vec4) * num_vertices);

    for(i = 0; i < num_vertices / 3; i++)
    {
        // r = r*300;
        // b = b*300;
        // g = g*300;
        colors[index] = (vec4){r*300, g*300, b*300, 1.0};
        colors[index + 1] = (vec4){r*300, g*300, b*300, 1.0};
        colors[index + 2] = (vec4){r*300, g*300, b*300, 1.0};
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
    num_vertices = SPHERE_VERTS*11 + CUBE_VERTS;
    GLuint program = initShader("shaders/vshader_proj3.glsl", "shaders/fshader_proj3.glsl");
    glUseProgram(program);
    
    // 20X20 cube that has y=0
    vec4* ball1 = uv_sphere(0.5, 32, 32);
    ball1_normals = normal_verts(ball1, SPHERE_VERTS);
    ball1 = translate_vertices(ball1, SPHERE_VERTS, 0, 0.5, 0);
    ball1_normals = translate_vertices(ball1_normals, SPHERE_VERTS, 0, 0.5, 0);
    vec4* ball1_color = colored_verts(SPHERE_VERTS, 1, 0, 0);
    //vec4* ball1_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* ball2 = uv_sphere(0.5, 32, 32);
    ball2_normals = normal_verts(ball2, SPHERE_VERTS);
    ball2 = translate_vertices(ball2, SPHERE_VERTS, 1, 0.5, 0);
    ball2_normals = translate_vertices(ball2_normals, SPHERE_VERTS, 1, 0.5, 0);
    vec4* ball2_color = colored_verts(SPHERE_VERTS, 0, 1, 0);
    //vec4* ball2_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* ball3 = uv_sphere(0.5, 32, 32);
    ball3_normals = normal_verts(ball1, SPHERE_VERTS);
    ball3 = translate_vertices(ball3, SPHERE_VERTS, 2, 0.5, 0);
    ball3_normals = translate_vertices(ball3_normals, SPHERE_VERTS, 2, 0.5, 0);
    vec4* ball3_color = colored_verts(SPHERE_VERTS, 0, 0, 1);
    //vec4* ball3_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* ball4 = uv_sphere(0.5, 32, 32);
    ball4_normals = normal_verts(ball4, SPHERE_VERTS);
    ball4 = translate_vertices(ball4, SPHERE_VERTS, 3, 0.5, 0);
    ball4_normals = translate_vertices(ball4_normals, SPHERE_VERTS, 3, 0.5, 0);
    vec4* ball4_color = colored_verts(SPHERE_VERTS, 1, 1, 0);
    //vec4* ball4_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* ball5 = uv_sphere(0.5, 32, 32);
    ball5_normals = normal_verts(ball5, SPHERE_VERTS);
    ball5 = translate_vertices(ball5, SPHERE_VERTS, 4, 0.5, 0);
    ball5_normals = translate_vertices(ball5_normals, SPHERE_VERTS, 4, 0.5, 0);
    vec4* ball5_color = colored_verts(SPHERE_VERTS, 0.5, 0.2, 0);
    //vec4* ball5_color = genRandomTriangleColors(SPHERE_VERTS);

    vec4* light = uv_sphere(0.1, 32, 32);
    light_normals = normal_verts(light, SPHERE_VERTS);
    light = translate_vertices(light, SPHERE_VERTS, 
        light_point.vec[X], light_point.vec[Y], light_point.vec[Z]);
    vec4* light_color = colored_verts(SPHERE_VERTS, 1.0, 1.0, 1.0);
    //vec4* liht_color = genRandomTriangleColors(SPHERE_VERTS);

    /* table cube */
    vec4* cube_verts = cube();
    cube_normals = normal_verts(cube_verts, 36);
    cube_verts = scale_vertices(cube_verts, CUBE_VERTS, 80, 40, 80);
    cube_verts = translate_vertices(cube_verts, CUBE_VERTS, 0.0, -20, 0.0);
    cube_normals = scale_vertices(cube_normals, CUBE_VERTS, 80, 40, 80);
    cube_normals = translate_vertices(cube_normals, CUBE_VERTS, 0.0, -20, 0.0);
    vec4* cube_colors = colored_verts(CUBE_VERTS, 0, 0.4, 0);

    vec4* scene_verts = calloc(num_vertices, sizeof(vec4));
    vec4* scene_colors = calloc(num_vertices, sizeof(vec4));
    vec4* scene_normals = calloc(SPHERE_VERTS*6 + CUBE_VERTS, sizeof(vec4));
    int normal_num = SPHERE_VERTS*6 + CUBE_VERTS;


    int curr_vert_size = add_to_scene(scene_verts, cube_verts, CUBE_VERTS, 0);
    curr_vert_size += add_to_scene(scene_verts, ball1, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, ball2, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, ball3, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, ball4, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, ball5, SPHERE_VERTS, curr_vert_size);
    curr_vert_size += add_to_scene(scene_verts, light, SPHERE_VERTS, curr_vert_size);
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
    curr_col_size += add_to_scene(scene_colors, light_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball1_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball2_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball3_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball4_color, SPHERE_VERTS, curr_col_size);
    curr_col_size += add_to_scene(scene_colors, ball5_color, SPHERE_VERTS, curr_col_size);

    int curr_norm_size = add_to_scene(scene_normals, cube_normals, CUBE_VERTS, 0);
    curr_norm_size += add_to_scene(scene_normals, ball1_normals, SPHERE_VERTS, curr_norm_size);
    curr_norm_size += add_to_scene(scene_normals, ball2_normals, SPHERE_VERTS, curr_norm_size);
    curr_norm_size += add_to_scene(scene_normals, ball3_normals, SPHERE_VERTS, curr_norm_size);
    curr_norm_size += add_to_scene(scene_normals, ball4_normals, SPHERE_VERTS, curr_norm_size);
    curr_norm_size += add_to_scene(scene_normals, ball5_normals, SPHERE_VERTS, curr_norm_size);
    curr_norm_size += add_to_scene(scene_normals, light_normals, SPHERE_VERTS, curr_norm_size);

    int f = 1;
    projection = *frustum(-f, f, f, -f, -f, f);
    model_view = *look_at(&eyes, &look_at_pos, &up_vec);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * (2 * num_vertices + curr_norm_size), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices, scene_verts);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices, sizeof(vec4) * num_vertices, scene_colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * num_vertices * 2, sizeof(vec4) * curr_norm_size, scene_normals);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * num_vertices * 2));

    ctm_location = glGetUniformLocation(program, "ctm");
    scale_ctm_location =  glGetUniformLocation(program, "scale_ctm");
    perspective_shift = glGetUniformLocation(program, "projection");
    cam_shit = glGetUniformLocation(program, "model_view");
    eye_location = glGetUniformLocation(program, "EyePoint");
    is_shadow_location = glGetUniformLocation(program, "isShadow");
    light_point_location = glGetUniformLocation(program, "light_point");
    ap_location = glGetUniformLocation(program, "AmbientProduct");
    sp_location = glGetUniformLocation(program, "SpecularProduct");
    dp_location = glGetUniformLocation(program, "DiffuseProduct");
    ac_location = glGetUniformLocation(program, "attenuation_constant");
    al_location = glGetUniformLocation(program, "attenuation_linear");
    aq_location = glGetUniformLocation(program, "attenuation_quadratic");
    shininess_location = glGetUniformLocation(program, "shininess");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

    GLfloat ac = 1;
    GLfloat al = 0.1;
    GLfloat aq = 0.1;

    glUniformMatrix4fv(scale_ctm_location, 1, GL_FALSE, (GLfloat *) &scale_ctm);
    glUniformMatrix4fv(perspective_shift, 1, GL_FALSE, (GLfloat *) &projection);
    glUniformMatrix4fv(cam_shit, 1, GL_FALSE, (GLfloat *) &model_view);
    glUniform4fv(light_point_location, 1, (GLfloat *) &light_point);
    
    // vec4 light_diffuse = {1.0, 1.0, 1.0, 1.0};
    // vec4 light_specular = {1.0, 1.0, 1.0, 1.0};
    // vec4 light_ambient = {0.2, 0.2, 0.2, 1.0};

    // GLfloat shininess = 0.2;

    glUniform4fv(eye_location, 1, (GLfloat *) &eyes);

    //table
    glUniform1i(is_shadow_location, 0);
    vec4* AmbientProductT = vec4_product(&table_material.reflect_ambient, &light_ambient);
    vec4* DiffuseProductT = vec4_product(&table_material.reflect_diffuse, &light_diffuse);
    vec4* SpecularProductT = vec4_product(&table_material.reflect_specular, &light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) AmbientProductT);
    glUniform4fv(dp_location, 1, (GLfloat *) DiffuseProductT);
    glUniform4fv(sp_location, 1, (GLfloat *) SpecularProductT);
    glUniform1fv(shininess_location, 1, &table_material.shine);
    glUniform1fv(ac_location, 1, &ac);
    glUniform1fv(al_location, 1, &al);
    glUniform1fv(aq_location, 1, &aq);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ctm);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glUniform1i(is_shadow_location, 0);
    vec4* AmbientProduct1 = vec4_product(&ball_materials[0].reflect_ambient, &light_ambient);
    vec4* DiffuseProduct1 = vec4_product(&ball_materials[0].reflect_diffuse, &light_diffuse);
    vec4* SpecularProduct1 = vec4_product(&ball_materials[0].reflect_specular, &light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) AmbientProduct1);
    glUniform4fv(dp_location, 1, (GLfloat *) DiffuseProduct1);
    glUniform4fv(sp_location, 1, (GLfloat *) SpecularProduct1);
    glUniform1fv(shininess_location, 1, &ball_materials[0].shine);
    glUniform1fv(ac_location, 1, &ac);
    glUniform1fv(al_location, 1, &al);
    glUniform1fv(aq_location, 1, &aq);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball1_ctm);
    glDrawArrays(GL_TRIANGLES, 36, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 0);
    vec4* AmbientProduct2 = vec4_product(&ball_materials[1].reflect_ambient, &light_ambient);
    vec4* DiffuseProduct2 = vec4_product(&ball_materials[1].reflect_diffuse, &light_diffuse);
    vec4* SpecularProduct2 = vec4_product(&ball_materials[1].reflect_specular, &light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) AmbientProduct2);
    glUniform4fv(dp_location, 1, (GLfloat *) DiffuseProduct2);
    glUniform4fv(sp_location, 1, (GLfloat *) SpecularProduct2);
    glUniform1fv(shininess_location, 1, &ball_materials[1].shine);
    glUniform1fv(ac_location, 1, &ac);
    glUniform1fv(al_location, 1, &al);
    glUniform1fv(aq_location, 1, &aq);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball2_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 0);
    vec4* AmbientProduct3 = vec4_product(&ball_materials[2].reflect_ambient, &light_ambient);
    vec4* DiffuseProduct3 = vec4_product(&ball_materials[2].reflect_diffuse, &light_diffuse);
    vec4* SpecularProduct3 = vec4_product(&ball_materials[2].reflect_specular, &light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) AmbientProduct3);
    glUniform4fv(dp_location, 1, (GLfloat *) DiffuseProduct3);
    glUniform4fv(sp_location, 1, (GLfloat *) SpecularProduct3);
    glUniform1fv(shininess_location, 1, &ball_materials[2].shine);
    glUniform1fv(ac_location, 1, &ac);
    glUniform1fv(al_location, 1, &al);
    glUniform1fv(aq_location, 1, &aq);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball3_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*2, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 0);
    vec4* AmbientProduct4 = vec4_product(&ball_materials[3].reflect_ambient, &light_ambient);
    vec4* DiffuseProduct4 = vec4_product(&ball_materials[3].reflect_diffuse, &light_diffuse);
    vec4* SpecularProduct4 = vec4_product(&ball_materials[3].reflect_specular, &light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) AmbientProduct4);
    glUniform4fv(dp_location, 1, (GLfloat *) DiffuseProduct4);
    glUniform4fv(sp_location, 1, (GLfloat *) SpecularProduct4);
    glUniform1fv(shininess_location, 1, &ball_materials[3].shine);
    glUniform1fv(ac_location, 1, &ac);
    glUniform1fv(al_location, 1, &al);
    glUniform1fv(aq_location, 1, &aq);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball4_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*3, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 0);
    vec4* AmbientProduct5 = vec4_product(&ball_materials[4].reflect_ambient, &light_ambient);
    vec4* DiffuseProduct5 = vec4_product(&ball_materials[4].reflect_diffuse, &light_diffuse);
    vec4* SpecularProduct5 = vec4_product(&ball_materials[4].reflect_specular, &light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) AmbientProduct5);
    glUniform4fv(dp_location, 1, (GLfloat *) DiffuseProduct5);
    glUniform4fv(sp_location, 1, (GLfloat *) SpecularProduct5);
    glUniform1fv(shininess_location, 1, &ball_materials[4].shine);
    glUniform1fv(ac_location, 1, &ac);
    glUniform1fv(al_location, 1, &al);
    glUniform1fv(aq_location, 1, &aq);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball5_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*4, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 0);
    vec4* AmbientProductL = vec4_product(&light_material.reflect_ambient, &light_ambient);
    vec4* DiffuseProductL = vec4_product(&light_material.reflect_diffuse, &light_diffuse);
    vec4* SpecularProductL = vec4_product(&light_material.reflect_specular, &light_specular);
    glUniform4fv(ap_location, 1, (GLfloat *) AmbientProductL);
    glUniform4fv(dp_location, 1, (GLfloat *) DiffuseProductL);
    glUniform4fv(sp_location, 1, (GLfloat *) SpecularProductL);
    glUniform1fv(shininess_location, 1, &light_material.shine);
    glUniform1fv(ac_location, 1, &ac);
    glUniform1fv(al_location, 1, &al);
    glUniform1fv(aq_location, 1, &aq);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &light_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*5, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 1);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball1_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*6, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 1);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball2_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*7, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 1);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball3_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*8, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 1);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball4_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*9, SPHERE_VERTS);

    glUniform1i(is_shadow_location, 1);
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &ball5_ctm);
    glDrawArrays(GL_TRIANGLES, 36 + SPHERE_VERTS*10, SPHERE_VERTS);

    glutSwapBuffers();
}

GLfloat spin = 0.0;
GLfloat ball1_spin = 0.0;
GLfloat ball2_spin = 0.0;
GLfloat ball3_spin = 0.0;
GLfloat ball4_spin = 0.0;
GLfloat ball5_spin = 0.0;
GLboolean y_spin = GL_FALSE;
GLboolean other_spin = GL_FALSE;
int spin_dir = Y;

GLfloat light_move_hor = 0.0;
GLfloat light_move_vert = 0.0;
GLfloat light_move_height = 0.0;

void idle(int value) 
{   
    glutTimerFunc(TIMER, idle, 0);
    mat4* ball_rotation_matrix1 = get_rotation_matrix(ball1_spin, Y);
    ball1_ctm = *ball_rotation_matrix1;

    mat4* ball_rotation_matrix2 = get_rotation_matrix(ball2_spin, Y);
    ball2_ctm = *ball_rotation_matrix2;

    mat4* ball_rotation_matrix3 = get_rotation_matrix(ball3_spin, Y);
    ball3_ctm = *ball_rotation_matrix3;

    mat4* ball_rotation_matrix4 = get_rotation_matrix(ball4_spin, Y);
    ball4_ctm = *ball_rotation_matrix4;

    mat4* ball_rotation_matrix5 = get_rotation_matrix(ball5_spin, Y);
    ball5_ctm = *ball_rotation_matrix5;
    ball1_spin += 0.00;
    ball2_spin += 0.0125;
    ball3_spin += 0.015;
    ball4_spin += 0.02;
    ball5_spin += 0.0225;
    
    mat4* rotation_matrix  =  get_rotation_matrix(spin, spin_dir);
    if(other_spin) {
        vec4* vector = vec_sub(&look_at_pos, &eyes);
        vec4* axis = vec_cross(&up_vec, vector);
        rotation_matrix = arbitrary_rotate(spin, axis);
    } else if (y_spin){
        rotation_matrix =  get_rotation_matrix(spin, spin_dir);
    }
    eyes = *mat_mult_vec(rotation_matrix, &eyes);
    model_view = *look_at(&eyes, &look_at_pos, &up_vec);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    	exit(0);

    switch(key) {
        case 'd':
        y_spin = GL_TRUE;
        spin = 0.015;
        spin_dir = Y;
        break;
        case 'a':
        y_spin = GL_TRUE;
        spin = -0.015;
        spin_dir = Y;
        break;
        case 'w':
        other_spin = GL_TRUE;
        spin = -0.015;
        spin_dir = X;
        break;
        case 's':
        other_spin = GL_TRUE;
        spin = 0.015;
        spin_dir = X;
        break;
    }

    glutPostRedisplay();
}

void special_keyboard(int key, int mousex, int mousey)
{
    
    switch(key) {
        case GLUT_KEY_RIGHT:
           light_move_hor += 0.15;
        break;
        case GLUT_KEY_LEFT:
            light_move_hor -= 0.15;
        break;
        case GLUT_KEY_UP:
            light_move_vert -= 0.15;
        break;
        case GLUT_KEY_DOWN:
            light_move_vert += 0.15;
        break;
        case GLUT_KEY_CTRL_R:
            light_move_height += 0.15; 
        break;
        case GLUT_KEY_ALT_R:
            light_move_height -= 0.15; 
        break;
    }

    mat4* temp = get_translation_matrix(light_move_hor, light_move_height, light_move_vert);
    light_ctm = *mat4_mult(temp, &light_ctm);
    light_point = *mat_mult_vec(&light_ctm, &light_point);

    glutPostRedisplay();
}

void handle_key_up(unsigned char key, int x, int y) {
    switch(key) {
        default:
        other_spin = GL_FALSE;
        spin = 0.0;
        break;
    }
    glutPostRedisplay();
}

void handle_special_key_up(int key, int x, int y) {
    switch(key) {
        default:
        light_move_hor = 0.0;
        light_move_vert = 0.0;
        light_move_height = 0.0;
        break;
    }
    glutPostRedisplay();
}