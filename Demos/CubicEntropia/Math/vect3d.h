// Definizione di una classe AD_Vect3D
// By [Turbo]

#ifndef _VECT3D_H_
#define _VECT3D_H_

#include <stdio.h>
#include "types.h"

// *******************************************************************
//                      Definisce un vertice texture
// *******************************************************************
typedef struct
{
  float4 u, v;
} AD_VectUV;


// *******************************************************************
// Definizione di vettore tridimensionale: questo tipo di definizione
// è compatibile con la D3DVECTOR !!!
// *******************************************************************
typedef struct
{
  float4 x, y, z;
} AD_Vect3D;


typedef struct
{
  AD_Vect3D S, T, SxT;
} BUMPSPACE;


// *******************************************************************
// typedef void (__cdecl *DrawTriangleProc)(AD_Vertex2D *, AD_Vertex2D *, AD_Vertex2D *);
typedef void (*VectOP3)(AD_Vect3D *, AD_Vect3D *, AD_Vect3D *);
typedef void (*VectOP2)(AD_Vect3D *, AD_Vect3D *);
typedef void (*VectOP1)(AD_Vect3D *);
typedef void (*VectScale)(AD_Vect3D *, float4, AD_Vect3D *);
typedef void (*VectAutoScale)(AD_Vect3D *, float4);
typedef float4 (*VectDot)(AD_Vect3D *, AD_Vect3D *);
typedef float4 (*VectLength)(AD_Vect3D *);


// *******************************************************************
//     Le procedure chimate dai programmi: saranno istanziate alla
//          realizzazione piu' efficiente per la propria CPU
// *******************************************************************
extern VectOP1 vect_auto_neg;
extern VectOP1 vect_auto_normalize;
extern VectOP2 vect_neg;
extern VectOP2 vect_auto_add;
extern VectOP2 vect_auto_sub;
extern VectOP2 vect_normalize;
extern VectOP3 vect_add;
extern VectOP3 vect_sub;
extern VectOP3 vect_cross;
extern VectScale vect_scale;
extern VectAutoScale vect_auto_scale;
extern VectDot vect_dot;
extern VectLength vect_length;
extern VectLength vect_length2;


// *******************************************************************
//                   Implementazioni in C
// *******************************************************************
void   vect_add_C (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
void   vect_auto_add_C (AD_Vect3D *a, AD_Vect3D *b);
void   vect_sub_C (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
void   vect_auto_sub_C (AD_Vect3D *a, AD_Vect3D *b);
void   vect_neg_C (AD_Vect3D *a, AD_Vect3D *b);
void   vect_auto_neg_C (AD_Vect3D *a);
void   vect_scale_C (AD_Vect3D *a, float4 s, AD_Vect3D *b);
void   vect_auto_scale_C (AD_Vect3D *a, float4 s);
float4 vect_dot_C (AD_Vect3D *a, AD_Vect3D *b);
void   vect_cross_C (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
float4 vect_length_C (AD_Vect3D *a);
float4 vect_length2_C (AD_Vect3D *a);
void   vect_normalize_C (AD_Vect3D *a, AD_Vect3D *b);
void   vect_auto_normalize_C (AD_Vect3D *a);

// *******************************************************************
//                   Implementazioni in Asm Pemtium
// *******************************************************************
void   vect_add_asm (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
void   vect_sub_asm (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
float4 vect_dot_asm (AD_Vect3D *a, AD_Vect3D *b);
void   vect_normalize_asm (AD_Vect3D *a, AD_Vect3D *b);
void   vect_auto_normalize_asm (AD_Vect3D *a);

// *******************************************************************
//                      Implementazioni in 3DNow!
// *******************************************************************
void   vect_add_3DNow (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
void   vect_auto_add_3DNow (AD_Vect3D *a, AD_Vect3D *b);
void   vect_sub_3DNow (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
void   vect_auto_sub_3DNow (AD_Vect3D *a, AD_Vect3D *b);
void   vect_neg_3DNow (AD_Vect3D *a, AD_Vect3D *b);
void   vect_auto_neg_3DNow (AD_Vect3D *a);
void   vect_scale_3DNow (AD_Vect3D *a, float4 s, AD_Vect3D *b);
void   vect_auto_scale_3DNow (AD_Vect3D *a, float4 s);
float4 vect_dot_3DNow (AD_Vect3D *a, AD_Vect3D *b);
void   vect_cross_3DNow (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
float4 vect_length_3DNow (AD_Vect3D *a);
float4 vect_length2_3DNow (AD_Vect3D *a);
void   vect_normalize_3DNow (AD_Vect3D *a, AD_Vect3D *b);
void   vect_auto_normalize_3DNow (AD_Vect3D *a);


// *******************************************************************
//    Queste implementazioni sono fisse in C perchè non sono usate
//    mai in porzioni time-critical
// *******************************************************************
void vect_set(AD_Vect3D *a, float4, float4, float4);
void vect_copy(AD_Vect3D *a, AD_Vect3D *b);
void vect_swap_3DS(AD_Vect3D *a);
void print_to_file (AD_Vect3D *a, FILE *dove);


// Definizione di un vettore multidimensionale
typedef struct
{
   int dimension;
   float *v;
} nVector;

void nvect_make(nVector *a, int dim);
void nvect_free(nVector *a);
void nvect_copy(nVector *a, nVector *b);
void nvect_add(nVector *a, nVector *b, nVector *c);
void nvect_sub(nVector *a, nVector *b, nVector *c);
void nvect_neg(nVector *a, nVector *b);
void nvect_scale(nVector *a, float4 s, nVector *b);
float nvect_dot(nVector *a, nVector *b);
float nvect_lenght(nVector *a);
void nvect_normalize(nVector *a);


// -------------------------------------------------------------------
//                    strutture psecifiche per D3D8
// -------------------------------------------------------------------

// rappresenta il sovrainsieme completo del vertice di input
// (sia alla pipeline fissa che al vertex shader)
typedef struct 
{ 
  AD_Vect3D point;
  AD_Vect3D normal;
  uint32 ARGB;
  AD_VectUV uv1;
  AD_VectUV uv2;
  BUMPSPACE bumpspace;
} DriverVertex, *DriverVertexPtr;


#endif