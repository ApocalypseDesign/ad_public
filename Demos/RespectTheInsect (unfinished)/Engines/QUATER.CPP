// Definizione dei metodi ed operatori della classe quaternion
// By [Turbo]

#include <math.h>
#include "quater.h"
#include "shared.h"

void quat_set(AD_Quaternion *q, float4 qx, float4 qy, float4 qz, float4 qw)
{
  q->x=qx;
  q->y=qy;
  q->z=qz;
  q->w=qw;
}


void quat_copy(AD_Quaternion *q1, AD_Quaternion *q2)
{
  q2->x=q1->x;
  q2->y=q1->y;
  q2->z=q1->z;
  q2->w=q1->w;
}


float4 quat_lenght(AD_Quaternion *q)
{
  return((float)sqrt(q->x*q->x+q->y*q->y+q->z*q->z+q->w*q->w));
}


void quat_normalize(AD_Quaternion *q)
{
  float4 aux;

  aux=1.0f/quat_lenght(q);
  q->x=q->x*aux;
  q->y=q->y*aux;
  q->z=q->z*aux;
  q->w=q->w*aux;
}


void quat_swap_3DS(AD_Quaternion *q)
{
  float4 tmp;

  tmp=q->y;
  q->y=q->z;
  q->z=tmp;
}


void quat_quat_to_rotquat(AD_Quaternion *q)
// trasforma il quaternione in un quaternione di rotazione
{
  float4 n, fi, sinfi;

  n=sqrtf(q->x*q->x+q->y*q->y+q->z*q->z);
  if (n==0) n=1;

  fi=q->w*0.5f;
  sinfi=sinf(fi)/n;
  q->w=cosf(fi);
  q->x=sinfi*q->x;
  q->y=sinfi*q->y;
  q->z=sinfi*q->z;
}


void quat_rotquat_to_quat(AD_Quaternion *q)
{
  float4 fi, sinfi;

  fi=acosf(q->w);
  sinfi=1.0f/sinf(fi);
  q->w=fi*2.0f;
  q->x=q->x*sinfi;
  q->y=q->y*sinfi;
  q->z=q->z*sinfi;
}


void quat_rotquat_to_matrix(AD_Quaternion *q, AD_Matrix *m)
{
  float qxy, qwz, qwy, qxz, qwx, qyz;
  float xx, yy, zz;

  qxy=2.0f * q->x * q->y;
  qwz=2.0f * q->w * q->z;
  qwy=2.0f * q->w * q->y;
  qxz=2.0f * q->x * q->z;
  qwx=2.0f * q->w * q->x;
  qyz=2.0f * q->y * q->z;
  xx=q->x * q->x;
  yy=q->y * q->y;
  zz=q->z * q->z;

  m->a[0][0]=1.0f-2.0f*(yy + zz);
  m->a[0][1]=qxy - qwz;
  m->a[0][2]=qwy + qxz;
  m->a[0][3]=0.0;

  m->a[1][0]=qxy + qwz;
  m->a[1][1]=1.0f-2.0f*(xx + zz);
  m->a[1][2]=qyz - qwx;
  m->a[1][3]=0.0;

  m->a[2][0]=qxz - qwy;
  m->a[2][1]=qwx + qyz;
  m->a[2][2]=1.0f-2.0f*(xx + yy);
  m->a[2][3]=0.0;

  m->a[3][0]=0.0;
  m->a[3][1]=0.0;
  m->a[3][2]=0.0;
  m->a[3][3]=1.0;
}


void quat_add(AD_Quaternion *q1, AD_Quaternion *q2, AD_Quaternion *q3)
{
  q3->x=q1->x + q2->x;
  q3->y=q1->y + q2->y;
  q3->z=q1->z + q2->z;
  q3->w=q1->w + q2->w;
}


void quat_sub(AD_Quaternion *q1, AD_Quaternion *q2, AD_Quaternion *q3)
{
  q3->x=q1->x - q2->x;
  q3->y=q1->y - q2->y;
  q3->z=q1->z - q2->z;
  q3->w=q1->w - q2->w;
}


void quat_mul(AD_Quaternion *q1, AD_Quaternion *q2, AD_Quaternion *q3)
{
  AD_Quaternion qtmp;

  qtmp.x=q1->w*q2->x + q1->x*q2->w + q1->y*q2->z - q1->z*q2->y;
  qtmp.y=q1->w*q2->y - q1->x*q2->z + q1->y*q2->w + q1->z*q2->x;
  qtmp.z=q1->w*q2->z + q1->x*q2->y - q1->y*q2->x + q1->z*q2->w;
  qtmp.w=q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z;
  quat_copy(&qtmp, q3);
}


void quat_scale(AD_Quaternion *q1, float s, AD_Quaternion *q2)
{
   q2->w=q1->w*s;
   q2->x=q1->x*s;
   q2->y=q1->y*s;
   q2->z=q1->z*s;
}


void quat_neg(AD_Quaternion *q1, AD_Quaternion *q2)
{
  q2->w=q1->w;
  q2->x=-q1->x;
  q2->y=-q1->y;
  q2->z=-q1->z;
}


void quat_inverse(AD_Quaternion *q1, AD_Quaternion *q2)
{
  float mag;

  mag = (q1->w*q1->w + q1->x*q1->x + q1->y*q1->y + q1->z*q1->z);
  if (mag != 1.0f) mag = 1.0f / mag; else mag = 1.0f;
  q2->w = q1->w *  mag;
  q2->x = q1->x * -mag;
  q2->y = q1->y * -mag;
  q2->z = q1->z * -mag;
}


void quat_log(AD_Quaternion *q1, AD_Quaternion *q2)
{
  float  len;

  len = sqrtf (q1->x*q1->x + q1->y*q1->y + q1->z*q1->z);
  if (q1->w != 0.0) len = atanf (len / q1->w); else len = (float)(M_PI/2.0f);
  q2->w = 0.0;
  q2->x = q1->x * len;
  q2->y = q1->y * len;
  q2->z = q1->z * len;
}


void quat_logdif(AD_Quaternion *q1, AD_Quaternion *q2, AD_Quaternion *q3)
{
  AD_Quaternion inv, dif;
  float len, len1, s;

  quat_inverse (q1, &inv);
  quat_mul (&inv, q2, &dif);
  len = sqrtf (dif.x*dif.x + dif.y*dif.y + dif.z*dif.z);
  s = quat_dot (q1, q2);
  if (s != 0.0) len1 = atanf (len / s); else len1 = (float)(M_PI/2.0f);
  if (len != 0.0) len1 /= len;
  q3->w = 0.0;
  q3->x = dif.x * len1;
  q3->y = dif.y * len1;
  q3->z = dif.z * len1;
}


void quat_exp(AD_Quaternion *q1, AD_Quaternion *q2)
{
  float  len, len1;

  len = sqrtf (q1->x*q1->x + q1->y*q1->y + q1->z*q1->z);
  if (len > 0.0) len1 = sinf(len) / len; else len1 = 1.0;
  q2->w = cosf(len);
  q2->x = q1->x * len1;
  q2->y = q1->y * len1;
  q2->z = q1->z * len1;
}


float4 quat_dot(AD_Quaternion *q1, AD_Quaternion *q2)
{
  return(q1->x*q2->x + q1->y*q2->y + q1->z*q2->z + q1->w*q2->w);
}


void quat_slerp(AD_Quaternion *q1, AD_Quaternion *q2, float4 t, float4 spin, AD_Quaternion *q3)
// Spherical Linear Interpolation
{
  float4 EPSILON = 1E-6f;    // questo epsilon e' OK !
  float4 alpha, cosalpha, k1, k2;
  float4 sinalpha, anglespin;
  float4 flip=1;

  cosalpha=quat_dot(q1, q2);  // prodotto scalare

 // caso di rotazioni tra quaternioni "molto" vicini fra loro; in questo caso
 // si ottengono coefficenti k1 e k2 pari allo sviluppo in serie troncato al
 // termine di grado 1; praticamente si approssima sin(a*x)=a*sin(x) e nei
 // rapporti (quando si e' a monte della formula) si ottiene semplificazione
 // tra seni; questo permette di evitare l'instabilita' numerica

  if ((1.0-fabs(cosalpha))<EPSILON)
  {
    k1=1.0f-t;
    k2=t;
  }
  else
  {
    alpha=acosf(cosalpha);
    sinalpha=1.0f/sinf(alpha);
    anglespin=(float)(alpha+spin*M_PI);
    k1=sinf(alpha-t*anglespin)*sinalpha;
    k2=sinf(t*anglespin)*sinalpha;
  }
  
  q3->x = k1*q1->x + k2*q2->x;
  q3->y = k1*q1->y + k2*q2->y;
  q3->z = k1*q1->z + k2*q2->z;
  q3->w = k1*q1->w + k2*q2->w;
}


void quat_slerp_flip(AD_Quaternion *q1, AD_Quaternion *q2, float4 t, float4 spin, AD_Quaternion *q3)
// Spherical Linear Interpolation
{
  float4 EPSILON = 1E-6f;    // questo epsilon e' OK !
  float4 alpha, cosalpha, k1, k2;
  float4 sinalpha, anglespin;
  float4 flip=1;

  cosalpha=quat_dot(q1, q2);  // prodotto scalare
  if (cosalpha < 0.0) cosalpha = -cosalpha, flip = -1; else flip = 1;

 // caso di rotazioni tra quaternioni "molto" vicini fra loro; in questo caso
 // si ottengono coefficenti k1 e k2 pari allo sviluppo in serie troncato al
 // termine di grado 1; praticamente si approssima sin(a*x)=a*sin(x) e nei
 // rapporti (quando si e' a monte della formula) si ottiene semplificazione
 // tra seni; questo permette di evitare l'instabilita' numerica

  if ((1.0-fabs(cosalpha))<EPSILON)
  {
    k1=1.0f-t;
    k2=t;
  }
  else
  {
    alpha=acosf(cosalpha);
    sinalpha=1.0f/sinf(alpha);
    anglespin=(float)(alpha+spin*M_PI);
    k1=sinf(alpha-t*anglespin)*sinalpha;
    k2=sinf(t*anglespin)*sinalpha;
  }
  
  k2*=flip;
  q3->x = k1*q1->x + k2*q2->x;
  q3->y = k1*q1->y + k2*q2->y;
  q3->z = k1*q1->z + k2*q2->z;
  q3->w = k1*q1->w + k2*q2->w;
}