#include "../headers/lighting.h"

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
#include "../headers/shapes.h"
#include "../headers/view.h"

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