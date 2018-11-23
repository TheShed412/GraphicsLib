#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <time.h>

#include "../headers/initShader.h"
#include "../headers/matrix_lib.h"
#include "../headers/transformations.h"

#define VERTS_IN_CUBE 36
#define VERTS_IN_SPHERE 1// placeholder for now

static void rotate_n_vertices(vec4* vertices, GLfloat twist, int axis, int num_vertices) {
    vec4 temp;
    for (int i=0; i < num_vertices; i++) {
        temp = vertices[i];
        vertices[i] = *rotation(&temp, twist, axis);
    }
}

static void translate_vertices_p1(vec4* vertices, int num_vertices, GLfloat x, GLfloat y, GLfloat z) {
    vec4 temp;
    for (int i=0; i < num_vertices; i++) {
        temp = vertices[i];
        vertices[i] = *translation(&temp, x, y, z);
    }
}

static void scale_vertices_p1(vec4* vertices, int num_vertices, GLfloat x, GLfloat y, GLfloat z) {
    vec4 temp;
    for (int i=0; i < num_vertices; i++) {
        temp = vertices[i];
        vertices[i] = *scale(&temp, x, y, z);
    }
}

static vec4* tor_band(GLfloat end, GLfloat start) {
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

vec4* torus(int num_vertices, GLfloat twist, int axis)
{   
    GLfloat radius = 3.5;
    int num_bands = num_vertices/216;
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
        translate_vertices_p1(band, num_vertices, -1*(radius-1.25), 0, 0);
        rotate_n_vertices(band, theta_r, Y, 216);
    
        for(int i=0; i<216; i++) {
            vertices[index + i] = band[i];
        }
        index += 216;
        free(band);
    }


    GLfloat scale_factor = 0.20;
    scale_vertices_p1(vertices, num_vertices, scale_factor, scale_factor, scale_factor);
    return vertices;
}

vec4* cube(){
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
    cube[30].vec[X] = -x, cube[30].vec[Y] = y,  cube[30].vec[Z] = z;//2
    cube[31].vec[X] = x,  cube[31].vec[Y] = y,  cube[31].vec[Z] = z;//3
    cube[32].vec[X] = x,  cube[32].vec[Y] = y, cube[32].vec[Z] = -z;//7

    cube[33].vec[X] = -x,  cube[33].vec[Y] = y,cube[33].vec[Z] = z;//2
    cube[34].vec[X] = x, cube[34].vec[Y] = y, cube[34].vec[Z] = -z;//7
    cube[35].vec[X] = -x,  cube[35].vec[Y] = y, cube[35].vec[Z] = -z;//6

    return cube;
}

vec4* sphere() {
    vec4* sphere_verts = calloc(VERTS_IN_SPHERE, VERTS_IN_SPHERE*sizeof(vec4));

    return sphere_verts;
}