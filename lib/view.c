#include "../headers/view.h"
#include "../headers/matrix_lib.h"
#include "../headers/transformations.h"

mat4* look_at(const vec4* eye_pos, const vec4* look_at_pos, const vec4* up_vector) {
    mat4* ret_mat = calloc(1, sizeof(mat4));
    ret_mat->cols[COL4].vec[W] = 1;

    vec4* up_normal = my_normalize(up_vector);
    vec4* look_dir_norm = my_normalize(vec_sub(look_at_pos, eye_pos));//f
    vec4* right_vector = my_normalize(vec_cross(look_dir_norm, up_normal));//s
    up_normal = vec_cross(right_vector, look_dir_norm);//u

    ret_mat->cols[COL1].vec[X] = right_vector->vec[X];
    ret_mat->cols[COL1].vec[Y] = right_vector->vec[Y];
    ret_mat->cols[COL1].vec[Z] = right_vector->vec[Z];

    ret_mat->cols[COL2].vec[X] = up_normal->vec[X];
    ret_mat->cols[COL2].vec[Y] = up_normal->vec[Y];
    ret_mat->cols[COL2].vec[Z] = up_normal->vec[Z];

    ret_mat->cols[COL3].vec[X] = -look_dir_norm->vec[X];
    ret_mat->cols[COL3].vec[Y] = -look_dir_norm->vec[Y];
    ret_mat->cols[COL3].vec[Z] = -look_dir_norm->vec[Z];

    ret_mat->cols[COL4].vec[X] = -vec_mult(right_vector, eye_pos);
    ret_mat->cols[COL4].vec[Y] = -vec_mult(up_normal, eye_pos);
    ret_mat->cols[COL4].vec[Z] = vec_mult(look_dir_norm, eye_pos);

    return ret_mat;
}
