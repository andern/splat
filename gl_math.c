#include <math.h>
#include <stdio.h>

#include "gl_math.h"

float rad(float deg)
{
        return 3.14159265359f / 180 * deg;
}

void vec3_cross(vec3 vec, const vec3 a, const vec3 b)
{
        vec[0] = a[1]*b[2] - a[2]*b[1];
        vec[1] = a[2]*b[0] - a[0]*b[2];
        vec[2] = a[0]*b[1] - a[1]*b[0];
}

void vec3_normalize(vec3 vec, const vec3 a)
{
        float m = sqrtf(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
        vec[0] /= m;
        vec[1] /= m;
        vec[2] /= m;
}

void vec3_sub(vec3 vec, const vec3 a, const vec3 b)
{
        vec[0] = a[0] - b[0];
        vec[1] = a[1] - b[1];
        vec[2] = a[2] - b[2];
}

void mat_diag(mat4 mat, float a, float b, float c, float d)
{
        mat[0] = a;
        mat[5] = b;
        mat[10] = c;
        mat[15] = d;
}

void mat_identity(mat4 mat)
{
        for (int i = 0; i < 16; i++) mat[i] = 0;
        mat_diag(mat, 1.0f, 1.0f, 1.0f, 1.0f);
}

void mat_look_at(mat4 mat, const vec3 camera_pos,
const vec3 target_pos, const vec3 up_dir)
{
        vec3 forward_dir;
        vec3_sub(forward_dir, target_pos, camera_pos); /* Distance */
        vec3_normalize(forward_dir, forward_dir); /* Normalized forward dir */

        vec3 right_dir;
        vec3_cross(right_dir, forward_dir, up_dir);

        vec3 up_dir_new;
        vec3_cross(up_dir_new, right_dir, forward_dir);

        mat[0] = right_dir[0];
        mat[1] = right_dir[1];
        mat[2] = right_dir[2];
        mat[3] = 0;

        mat[4] = up_dir_new[0];
        mat[5] = up_dir_new[1];
        mat[6] = up_dir_new[2];
        mat[7] = 0;

        mat[8] = -forward_dir[0];
        mat[9] = -forward_dir[1];
        mat[10] = -forward_dir[2];
        mat[11] = 0;

        mat[12] = 0;
        mat[13] = 0;
        mat[14] = 0;
        mat[15] = 1;

        mat4 translate = {0};
        mat_diag(translate, 1.0f, 1.0f, 1.0f, 1.0f);
        translate[3] = -camera_pos[0];
        translate[7] = -camera_pos[1];
        translate[11] = -camera_pos[2];

        mat_mul(mat, mat, translate);
}

void mat_mul(mat4 mat, const mat4 a, const mat4 b)
{ 
        mat[0] = a[0]*b[0] + a[1]*b[4] + a[2]*b[8]  + a[3]*b[12];
        mat[1] = a[0]*b[1] + a[1]*b[5] + a[2]*b[9]  + a[3]*b[13];
        mat[2] = a[0]*b[2] + a[1]*b[6] + a[2]*b[10] + a[3]*b[14];
        mat[3] = a[0]*b[3] + a[1]*b[7] + a[2]*b[11] + a[3]*b[15];

        mat[4] = a[4]*b[0] + a[5]*b[4] + a[6]*b[8]  + a[7]*b[12];
        mat[5] = a[4]*b[1] + a[5]*b[5] + a[6]*b[9]  + a[7]*b[13];
        mat[6] = a[4]*b[2] + a[5]*b[6] + a[6]*b[10] + a[7]*b[14];
        mat[7] = a[4]*b[3] + a[5]*b[7] + a[6]*b[11] + a[7]*b[15];

        mat[8] = a[8]*b[0] + a[9]*b[4] + a[10]*b[8]  + a[11]*b[12];
        mat[9] = a[8]*b[1] + a[9]*b[5] + a[10]*b[9]  + a[11]*b[13];
        mat[10] = a[8]*b[2] + a[9]*b[6] + a[10]*b[10] + a[11]*b[14];
        mat[11] = a[8]*b[3] + a[9]*b[7] + a[10]*b[11] + a[11]*b[15];

        mat[12] = a[12]*b[0] + a[13]*b[4] + a[14]*b[8]  + a[15]*b[12];
        mat[13] = a[12]*b[1] + a[13]*b[5] + a[14]*b[9]  + a[15]*b[13];
        mat[14] = a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14];
        mat[15] = a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15];
}


void print_mat(mat4 mat)
{
        for (int i = 0; i < 16; i++) {
                if (i % 4 == 0) printf("\n");
                printf("%-5.4f ", mat[i]);
        }
        printf("\n");
}

