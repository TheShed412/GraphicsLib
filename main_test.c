#include <stdio.h>
#include <stdlib.h>
#include "headers/matrix_lib.h"
#include "headers/transformations.h"

#define FAIL_STR ">_< Oooo daddy made a fucky wuck!"
#define PASS_STR "UwU Good Daddy passed his test!"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void assert_float(char*, GLfloat, GLfloat);

int main(int argc, const char* argv[]) {
    
    vec4 vector = {1, 1, 1, 1};
    vec4* result = translation(&vector, 1.0, 3.0, 1.0);

    return 0;
}

void assert_float(char* print_string, GLfloat expected, GLfloat actual) {
    if (expected == actual){
        printf("%s%s%s: %s\n", KGRN, PASS_STR, KWHT, print_string);
    } else {
        printf("%s%s%s: %s --- expected: %f != actual: %f\n", 
        KRED, FAIL_STR, KWHT, print_string, expected, actual);
    }
}

void matrix_lib_tests() {
    mat4 matrix;

    matrix.cols[COL1].vec[X] = 0.1;
    matrix.cols[COL1].vec[Y] = 0.2;
    matrix.cols[COL1].vec[Z] = 0.3;
    matrix.cols[COL1].vec[W] = 0.4;
    matrix.cols[COL4].vec[W] = 0.16;

    mat4* result = scalar_mult(&matrix, 2);

    assert_float("scalar_mult", 0.2, result->cols[0].vec[0]);
    assert_float("scalar_mult", 0.4, result->cols[0].vec[1]);
    assert_float("scalar_mult", 0.6, result->cols[0].vec[2]);
    assert_float("scalar_mult", 0.8, result->cols[0].vec[3]);
    assert_float("scalar_mult", 0.32, result->cols[3].vec[3]);
    free(result);

    vec4 left;
    vec4 right;

    left.vec[X] = 1.0;
    left.vec[Y] = 2.0;
    left.vec[Z] = 1.0;
    left.vec[W] = 1.0;

    right.vec[X] = 2.0;
    right.vec[Y] = 2.0;
    right.vec[Z] = 2.0;
    right.vec[W] = 2.0;

    GLfloat vec_result = vec_mult(&left, &right);

    assert_float("vec_mult", 10.0, vec_result);

    mat4 matrix2;
    matrix2.cols[COL1].vec[X] = 1.0;
    matrix2.cols[COL2].vec[X] = 2.0;
    matrix2.cols[COL3].vec[X] = 3.0;
    matrix2.cols[COL4].vec[X] = 4.0;

    vec4* result_vec = vec_from_row(&matrix2, X);

    assert_float("vec_from_row", result_vec->vec[X], 1.0);
    assert_float("vec_from_row", result_vec->vec[Y], 2.0);
    assert_float("vec_from_row", result_vec->vec[Z], 3.0);
    assert_float("vec_from_row", result_vec->vec[W], 4.0);

    mat4 matrix3 = (*make_mat4());
    //set_mat_zeroes(&matrix3);

    matrix3.cols[COL1].vec[X] = 2.0;
    matrix3.cols[COL2].vec[X] = 2.0;
    matrix3.cols[COL3].vec[X] = 2.0;
    matrix3.cols[COL4].vec[X] = 2.0;

    matrix3.cols[COL1].vec[W] = 2.0;
    matrix3.cols[COL2].vec[W] = 2.0;
    matrix3.cols[COL3].vec[W] = 2.0;
    matrix3.cols[COL4].vec[W] = 2.0;

    mat4* result_mat = mat4_mult(&matrix3, &matrix3);

    assert_float("mat4_mult", 8.0, result_mat->cols[COL1].vec[X]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL2].vec[X]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL3].vec[X]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL4].vec[X]);

    assert_float("mat4_mult", 0.0, result_mat->cols[COL1].vec[Y]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL2].vec[Y]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL3].vec[Y]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL4].vec[Y]);

    assert_float("mat4_mult", 0.0, result_mat->cols[COL1].vec[Z]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL2].vec[Z]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL3].vec[Z]);
    assert_float("mat4_mult", 0.0, result_mat->cols[COL4].vec[Z]);

    assert_float("mat4_mult", 8.0, result_mat->cols[COL1].vec[W]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL2].vec[W]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL3].vec[W]);
    assert_float("mat4_mult", 8.0, result_mat->cols[COL4].vec[W]);

    GLfloat float_arr[ROW_SIZE][COL_SIZE] = {{2, 2, 2, 2}, 
                                           {0, 0, 0, 0},
                                           {0, 0, 0, 0},
                                           {2, 2, 2, 2}};
    
    mat4 arr_to_mat;
    arr_to_mat4(float_arr, &arr_to_mat);

    GLfloat float_arr2[ROW_SIZE][COL_SIZE];
    mat4_to_arr(&arr_to_mat, float_arr2);

    mat4* result_mat2 = mat4_mult(&arr_to_mat, &arr_to_mat);

    for (int i = 0; i < COL_SIZE; i++) {
        for (int j = 0; j < ROW_SIZE; j++) {
            assert_float("mat4_to_arr", float_arr[i][j], float_arr2[i][j]);
        }
    }

    assert_float("arr_to_mat4", 8.0, result_mat2->cols[COL1].vec[X]);
    assert_float("arr_to_mat4", 8.0, result_mat2->cols[COL2].vec[X]);
    assert_float("arr_to_mat4", 8.0, result_mat2->cols[COL3].vec[X]);
    assert_float("arr_to_mat4", 8.0, result_mat2->cols[COL4].vec[X]);

    assert_float("arr_to_mat4", 0.0, result_mat2->cols[COL1].vec[Y]);
    assert_float("arr_to_mat4", 0.0, result_mat2->cols[COL2].vec[Y]);
    assert_float("arr_to_mat4", 0.0, result_mat2->cols[COL3].vec[Y]);
    assert_float("arr_to_mat4", 0.0, result_mat2->cols[COL4].vec[Y]);

    assert_float("arr_to_mat4", 0.0, result_mat2->cols[COL1].vec[Z]);
    assert_float("arr_to_mat4", 0.0, result_mat2->cols[COL2].vec[Z]);
    assert_float("arr_to_mat4", 0.0, result_mat2->cols[COL3].vec[Z]);
    assert_float("arr_to_mat4", 0.0, result_mat2->cols[COL4].vec[Z]);

    assert_float("arr_to_mat4", 8.0, result_mat2->cols[COL1].vec[W]);
    assert_float("arr_to_mat4", 8.0, result_mat2->cols[COL2].vec[W]);
    assert_float("arr_to_mat4", 8.0, result_mat2->cols[COL3].vec[W]);
    assert_float("arr_to_mat4", 8.0, result_mat2->cols[COL4].vec[W]);

    GLfloat** test_arr = deter3_from_array(float_arr2, Y);

    GLfloat thing[3][3] = {{2, 2, 2}, 
                        {0, 0, 0}, 
                        {2, 2, 2}};

    for(int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++) {
            assert_float("deter3_from_array", thing[i][j], test_arr[i][j]);
        }
    }
    GLfloat deter_tst[ROW_SIZE][COL_SIZE] = {{1, 0, 0, 0}, 
                                           {2, 3, 5, 0},
                                           {0, 4, 0, 0},
                                           {0, 4, 5, 3}};

    GLfloat deter_tst2[ROW_SIZE][COL_SIZE] = {{0, 0, 0, 0}, 
                                           {2, 3, 5, 0},
                                           {0, 4, 0, 0},
                                           {0, 4, 5, 3}};

    GLfloat deter_tst3[ROW_SIZE][COL_SIZE] = {{0, 9, 0, 0}, 
                                           {0, 3, 5, 0},
                                           {0, 4, 0, 0},
                                           {0, 4, 5, 3}};

    mat4 deter_mat_test;
    arr_to_mat4(deter_tst, &deter_mat_test);
    mat4 deter_mat_test2;
    arr_to_mat4(deter_tst2, &deter_mat_test2);
    mat4 deter_mat_test3;
    arr_to_mat4(deter_tst3, &deter_mat_test3);

    GLfloat deter_result = determinant(&deter_mat_test);
    GLfloat deter_result2 = determinant(&deter_mat_test2);
    GLfloat deter_result3 = determinant(&deter_mat_test3);

    assert_float("determinant", -60, deter_result);
    assert_float("determinant2", 0, deter_result2);
    assert_float("determinant3", 0, deter_result3); 

    /* MINOR MATRIX TEST */
    GLfloat before_minor_matrix_arr[4][4] = {{1, 1, 1, 1}, 
                                           {2, 2, 2, 2},
                                           {3, 3, 3, 3},
                                           {4, 4, 4, 4}};
    
    GLfloat expected_minor_matrix[3][3] =  {{1, 1, 1}, 
                                          {3, 3, 3}, 
                                          {4, 4, 4}};

    GLfloat expected_minor_matrix2[3][3] =  {{1, 1, 1}, 
                                          {2, 2, 2}, 
                                          {3, 3, 3}};

    GLfloat expected_minor_matrix3[3][3] =  {{2, 2, 2}, 
                                          {3, 3, 3}, 
                                          {4, 4, 4}};


    mat4 before_minor_matrix;
    arr_to_mat4(before_minor_matrix_arr, &before_minor_matrix);

    GLfloat** minor_matrix_actual = minor_matrix(&before_minor_matrix, COL2, COL2);
    GLfloat** minor_matrix_actual2 = minor_matrix(&before_minor_matrix, COL4, COL4);
    GLfloat** minor_matrix_actual3 = minor_matrix(&before_minor_matrix, COL1, COL1); 

    for(int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++) {
            assert_float("minor_matrix", expected_minor_matrix[i][j], minor_matrix_actual[i][j]);
        }
    }

    for(int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++) {
            assert_float("minor_matrix2", expected_minor_matrix2[i][j], minor_matrix_actual2[i][j]);
        }
    }

    for(int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++) {
            assert_float("minor_matrix3", expected_minor_matrix3[i][j], minor_matrix_actual3[i][j]);
        }
    }

    /* mat4_of_minors TEST */
    GLfloat cofactor_test_arr[4][4] =      {{3, 1, 9, 2}, 
                                           {4, 5, 6, 9},
                                           {3, 4, 3, 7},
                                           {7, 8, 6, 4}};

    GLfloat cofactor_test_exp[4][4] =      {{90, -75, -11, 9}, 
                                           {-330, 237, 91, -33},
                                           {402, -297, -115, 63},
                                           {-6, 24, 2, -12}};

    mat4 cofactor_test_actual;
    arr_to_mat4(cofactor_test_arr, &cofactor_test_actual);
    mat4* mat4_of_minors_actual = cofactor(&cofactor_test_actual);
    mat4_to_arr(mat4_of_minors_actual, cofactor_test_arr);

    for(int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j ++) {
            assert_float("cofactor", cofactor_test_exp[i][j], cofactor_test_arr[i][j]);
        }
    }

    GLfloat deter_3x3[3][3] = {{5, 6, 9}, 
                            {4, 3, 7}, 
                            {8, 6, 4}};

    GLfloat** deter_3x3_ptrs = (GLfloat**) malloc(3 * sizeof(GLfloat*));
    for(int i = 0; i < 3; i++) {
        deter_3x3_ptrs[i] = (GLfloat*) calloc(3, sizeof(GLfloat));
    }

    for(int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            deter_3x3_ptrs[i][j] = deter_3x3[i][j];
        }
    }

    GLfloat determinant = determinant3x3(deter_3x3_ptrs);
    assert_float("determinant3x3", 90, determinant);

        GLfloat transpose_test_arr[4][4] =      {{1, 1, 1, 1}, 
                                                 {2, 2, 2, 2},
                                                 {3, 3, 3, 3},
                                                 {4, 4, 4, 4}};
        
        GLfloat transpose_expected_arr[4][4] =      {{1, 2, 3, 4}, 
                                                     {1, 2, 3, 4},
                                                     {1, 2, 3, 4},
                                                     {1, 2, 3, 4}};

        mat4 transpose_test;
        arr_to_mat4(transpose_test_arr, &transpose_test);
        mat4 transpose_expected;
        arr_to_mat4(transpose_expected_arr, &transpose_expected);

        mat4* actual_transpose = transpose_mat4(&transpose_test);

        for (int i=0; i < 4; i++) {
            for (int j=0; j < 4; j++) {
                assert_float("transpose", transpose_expected.cols[i].vec[j], actual_transpose->cols[i].vec[j]);
            }
        }

        GLfloat inverse_test_arr[4][4] =   {{1, 1, 2, 1}, 
                                            {4, 2, 2, 2},
                                            {3, 3, 5, 3},
                                            {4, 9, 4, 4}};
        
        GLfloat identity_arr[4][4] =       {{1, 0, 0, 0}, 
                                            {0, 1, 0, 0},
                                            {0, 0, 1, 0},
                                            {0, 0, 0, 1}};
        mat4 inverse_test;
        arr_to_mat4(inverse_test_arr, &inverse_test);
        mat4 identity;
        arr_to_mat4(identity_arr, &identity);

        mat4* inverse_actual = inverse_mat4(&inverse_test);

        mat4* should_be_identity = mat4_mult(&inverse_test, inverse_actual);

        for (int i=0; i < 4; i++) {
            for (int j=0; j < 4; j++) {
                assert_float("invert", identity.cols[i].vec[j], should_be_identity->cols[i].vec[j]);
            }
        }
}