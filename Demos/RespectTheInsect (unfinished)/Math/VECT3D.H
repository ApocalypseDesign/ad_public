// Definizione di una classe AD_Vect3D
// By [Turbo]

#ifndef _VECT3D_H_
#define _VECT3D_H_

#include <stdio.h>
#include "ADrender.h"
#include "..\types.h"

// Definizione di vettore tridimensionale

typedef struct
{
  float4 x, y, z;
} AD_Vect3D;

void vect_copy(AD_Vect3D *a, AD_Vect3D *b);
void vect_set(AD_Vect3D *a, float4, float4, float4);
void vect_add(AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
void vect_sub(AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
void vect_neg(AD_Vect3D *a, AD_Vect3D *b);
void vect_scale(AD_Vect3D *a, float4 s, AD_Vect3D *b);
void vect_swap_3DS(AD_Vect3D *a);
float4 vect_dot(AD_Vect3D *a, AD_Vect3D *b);
void vect_cross(AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c);
float4 vect_lenght(AD_Vect3D *a);
void vect_normalize(AD_Vect3D *a);
void print_to_file (AD_Vect3D *a, FILE *dove);

// alcune delle stesse funzioni in formato inline
inline void vect_scale_inline(AD_Vect3D *a, float4 s, AD_Vect3D *b)
{
  b->x=s*a->x;
  b->y=s*a->y;
  b->z=s*a->z;
}

inline void vect_add_inline(AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
  c->x=a->x + b->x;
  c->y=a->y + b->y;
  c->z=a->z + b->z;
}

inline void vect_sub_inline(AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
  c->x=a->x - b->x;
  c->y=a->y - b->y;
  c->z=a->z - b->z;
}


typedef struct
{
  float4 u, v;
} AD_VectUV;


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


#define VERTEX_IS_TO_CLIP 0x80000000

typedef struct 
{ 
  int flags;  // il bit 0 e' a 1 se il vertice e' da clippare; 
              // il bit 1 e' 0 e non va usato! (perche' serve
              // che sia zero quando si sommano i flags dei 3
              // vertici di un triangolo per vedere se esso e' 
              // clippato in z
              // i bit dal 31..2 contengono il valore di
              // counter_for_resetting_vertex
              // che viene incrementato di 4 alla volta

  AD_Vect3D point,    // punto originale
	        tpoint,   // trasformato in camera space
	        pjpoint;  // proiettato
  float     w;        // 1/z del punto proiettato
} AD_Vertex3D;

#endif