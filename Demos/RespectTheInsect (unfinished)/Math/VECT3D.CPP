// Metodi e operatori della classe AD_Vect3D
// By [Turbo]
#include <math.h>
#include "matrix.h"
#include "vect3D.h"

float uno=1.0f;

inline float mysqrt_2(float value)
{
	float result;
	_asm
	{
            fld value
			fsqrt
			fstp result

	}
	return(result);
}


// REALIZZAZIONE C DEGLI OPERATORI

void vect_copy(AD_Vect3D *a, AD_Vect3D *b)
{
   b->x=a->x;
   b->y=a->y;
   b->z=a->z;
}


void vect_set(AD_Vect3D *a, float4 fx, float4 fy, float4 fz)
{
  a->x=fx;
  a->y=fy;
  a->z=fz;
}


void vect_add(AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
/*
  c->x=a->x + b->x;
  c->y=a->y + b->y;
  c->z=a->z + b->z;
*/

  __asm
  {
	 mov esi, [a]
	 mov ebx, [c]
	 mov edi, [b]
     add ebx, 0

	 fld dword ptr [esi]
	 fadd dword ptr [edi]
	 fld dword ptr [esi+4]
	 fadd dword ptr [edi+4]
	 fxch
	 fld dword ptr [esi+8]
	 fadd dword ptr [edi+8]
	 fxch
	 fstp dword ptr [ebx]
	 fstp dword ptr [ebx+8]
	 fstp dword ptr [ebx+4]
  }
}


void vect_sub(AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
/*
  c->x=a->x - b->x;
  c->y=a->y - b->y;
  c->z=a->z - b->z;
*/
  __asm
  {
	 mov esi, [a]
	 mov ebx, [c]
	 mov edi, [b]
     add ebx, 0

	 fld dword ptr [esi]
	 fsub dword ptr [edi]
	 fld dword ptr [esi+4]
	 fsub dword ptr [edi+4]
	 fxch
	 fld dword ptr [esi+8]
	 fsub dword ptr [edi+8]
	 fxch
	 fstp dword ptr [ebx]
	 fstp dword ptr [ebx+8]
	 fstp dword ptr [ebx+4]
  }
}


void vect_neg(AD_Vect3D *a, AD_Vect3D *b)
{
  b->x=-a->x;
  b->y=-a->y;
  b->z=-a->z;
}


void vect_scale(AD_Vect3D *a, float4 s, AD_Vect3D *b)
{
  b->x=s*a->x;
  b->y=s*a->y;
  b->z=s*a->z;
}


float4 vect_dot(AD_Vect3D *a, AD_Vect3D *b)
{ 
//  float result;

  _asm
  {
	mov esi, [a]
	mov edi, [b]
	
	fldz

	fld dword ptr [esi]
	fmul dword ptr [edi]
	fld dword ptr [esi+4]
	fmul dword ptr [edi+4]

	fxch
	faddp st(2), st

	fld dword ptr [esi+8]
	fmul dword ptr [edi+8]

	fxch
	faddp st(2), st

	fadd
//	fstp result  // il vc ritorna il risultato sullo stack
  }

//	return(a->x*b->x + a->y*b->y + a->z*b->z);
//  return(result);
}


void vect_cross(AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
  c->x=a->y*b->z - b->y*a->z;
  c->y=b->x*a->z - a->x*b->z;
  c->z=a->x*b->y - b->x*a->y;
}


float4 vect_lenght(AD_Vect3D *a)
{
  return (mysqrt_2(a->x*a->x+a->y*a->y+a->z*a->z));
}


void vect_normalize(AD_Vect3D *a)
{

  float4 aux;
/*
  aux=1.0f/mysqrt_2(a->x*a->x+a->y*a->y+a->z*a->z);
  a->x=a->x*aux;
  a->y=a->y*aux;
  a->z=a->z*aux;
*/

  _asm
  {
	mov esi, [a]
	
	fld dword ptr [esi]
	fmul dword ptr [esi]
	fld dword ptr [esi+4]
	fmul dword ptr [esi+4]

	fxch
	faddp st(1), st

	fld dword ptr [esi+8]
	fmul dword ptr [esi+8]

	fxch
	faddp st(1), st

	fsqrt
	fdivr dword ptr [uno]

	fstp dword ptr [aux]

	fld dword ptr [esi]
	fmul dword ptr [aux]
	fld dword ptr [esi+4]
	fmul dword ptr [aux]
	fld dword ptr [esi+8]
	fmul dword ptr [aux]
	fstp  dword ptr [esi+8]
	fstp  dword ptr [esi+4]
	fstp  dword ptr [esi]
  }
}


void vect_swap_3DS(AD_Vect3D *a)
{
  float4 tmp;

  tmp=a->y;
  a->y=a->z;
  a->z=tmp;
}


void print_to_file (AD_Vect3D *a, FILE *dove)
{
   fprintf(dove, "x=%f  y=%f  z=%f \n", a->x, a->y, a->z);
}


// VETTORI MULTIDIMENSIONALI

void nvect_make(nVector *a, int dim)
{
  int t;

  if (dim<0) return;

  a->dimension=dim;
  a->v=new float[dim];
  for (t=0; t<dim; t++) a->v[t]=0;
}


void nvect_free(nVector *a)
{
  delete [] a->v;
  a->dimension=0;
}


void nvect_copy(nVector *a, nVector *b)
{
  int t;

  for (t=0; t<a->dimension; t++)  b->v[t]=a->v[t];
}


void nvect_add(nVector *a, nVector *b, nVector *c)
{
  int t;

  for (t=0; t<a->dimension; t++)  c->v[t]=a->v[t] + b->v[t];
}


void nvect_sub(nVector *a, nVector *b, nVector *c)
{
  int t;

  for (t=0; t<a->dimension; t++)  c->v[t]=a->v[t] - b->v[t];
}


void nvect_neg(nVector *a, nVector *b)
{
  int t;

  for (t=0; t<a->dimension; t++)  b->v[t]=-a->v[t];
}


void nvect_scale(nVector *a, float4 s, nVector *b)
{
  int t;

  for (t=0; t<a->dimension; t++)  b->v[t]=s*a->v[t];
}


float nvect_dot(nVector *a, nVector *b)
{
  int t;
  float faux=0;

  for (t=0; t<a->dimension; t++)  faux=faux + b->v[t]*a->v[t];
  return(faux);
}


float nvect_lenght(nVector *a)
{
  int t;
  float faux=0;

  for (t=0; t<a->dimension; t++)  faux=faux + a->v[t]*a->v[t];
  return(mysqrt_2(faux));
}


void nvect_normalize(nVector *a)
{
  int t;
  float faux=0;

  for (t=0; t<a->dimension; t++)  faux=faux + a->v[t]*a->v[t];
  
  faux=1.0f/faux;
  for (t=0; t<a->dimension; t++)  a->v[t]=a->v[t]*faux;
}