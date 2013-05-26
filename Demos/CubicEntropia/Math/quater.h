// Definizione di una classe quaternion
// By [Turbo]

#ifndef _QUATER_H_
#define _QUATER_H_

#include "matrix.h"

typedef struct
{
  float4 x, y, z, w; // w rappresenta un angolo in radianti
} AD_Quaternion;


void quat_set(AD_Quaternion *q, float4 qx, float4 qy, float4 qz, float4 qw);
void quat_copy(AD_Quaternion *q1, AD_Quaternion *q2);
void quat_add(AD_Quaternion *q1, AD_Quaternion *q2, AD_Quaternion *q3);
void quat_sub(AD_Quaternion *q1, AD_Quaternion *q2, AD_Quaternion *q3);
void quat_mul(AD_Quaternion *, AD_Quaternion *, AD_Quaternion *);
void quat_scale(AD_Quaternion *q1, float s, AD_Quaternion *q2);
void quat_inverse(AD_Quaternion *q1, AD_Quaternion *q2);
void quat_neg(AD_Quaternion *q1, AD_Quaternion *q2);
void quat_log(AD_Quaternion *q1, AD_Quaternion *q2);
void quat_logdif(AD_Quaternion *q1, AD_Quaternion *q2, AD_Quaternion *q3);
void quat_exp(AD_Quaternion *q1, AD_Quaternion *q2);

float4 quat_lenght(AD_Quaternion *q);
float4 quat_dot(AD_Quaternion *q1, AD_Quaternion *q2);
void quat_normalize(AD_Quaternion *q);
void quat_swap_3DS(AD_Quaternion *q);
void quat_quat_to_rotquat(AD_Quaternion *q);
void quat_rotquat_to_quat(AD_Quaternion *q);
void quat_rotquat_to_matrix(AD_Quaternion *q, AD_Matrix *m);
void quat_slerp(AD_Quaternion *q1, AD_Quaternion *q2, float4 t, float4 spin, AD_Quaternion *q3);
void quat_slerp_flip(AD_Quaternion *q1, AD_Quaternion *q2, float4 t, float4 spin, AD_Quaternion *q3);

#endif