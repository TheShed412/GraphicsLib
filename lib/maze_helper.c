#include "../headers/maze_helper.h"

vec4* single_cube(){
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

