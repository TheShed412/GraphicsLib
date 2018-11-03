#ifndef MAZ_HLP
#define MAZ_HLP

#include "matrix_lib.h"

#define VERTS_IN_CUBE 36

typedef struct {
    GLboolean north_wall;
    GLboolean south_wall;
    GLboolean east_wall;
    GLboolean west_wall;
} cell;

vec4* single_cube();

#endif