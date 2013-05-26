// Metodi e operatori della classe AD_Vect3D
// By [Turbo]
#include <math.h>
#include "vect3D.h"

#ifdef  _MSC_VER
#include <mm3dnow.h>
#define _m_from_floats(f) (*(__m64 *)(f))
#define _m_to_float(f)  ((f).m64_f32[0])
#define _m_from_float(f)  _m_from_int (*(int *)&(f))
#endif

VectOP1 vect_auto_neg;
VectOP1 vect_auto_normalize;
VectOP2 vect_neg;
VectOP2 vect_auto_add;
VectOP2 vect_auto_sub;
VectOP2 vect_normalize;
VectOP3 vect_add;
VectOP3 vect_sub;
VectOP3 vect_cross;
VectScale vect_scale;
VectAutoScale vect_auto_scale;
VectDot vect_dot;
VectLength vect_length;
VectLength vect_length2;
float uno=1.0f;

#ifdef  _MSC_VER
inline float mysqrt_2(float4 value)
{
	float4 result;
	_asm
	{
            fld value
			fsqrt
		    fstp result
	}
	return(result);
}
#else
inline float4 mysqrt_2(float4 value)
{
	return((float4)sqrt(value));
}
#endif


// *******************************************************************
//    Queste implementazioni sono fisse in C perchè non sono usate
//    mai in porzioni time-critical
// *******************************************************************

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



// *******************************************************************
//                   Implementazioni in C/Asm Pentium
// *******************************************************************

void vect_add_C (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
  c->x=a->x + b->x;
  c->y=a->y + b->y;
  c->z=a->z + b->z;
}


void vect_auto_add_C (AD_Vect3D *a, AD_Vect3D *b)
{

  a->x=a->x + b->x;
  a->y=a->y + b->y;
  a->z=a->z + b->z;
/*
  __asm
  {
	 mov esi, [a]
	 mov edi, [b]

	 fld dword ptr [esi]
	 fadd dword ptr [edi]
	 fld dword ptr [esi+4]
	 fadd dword ptr [edi+4]
	 fxch
	 fld dword ptr [esi+8]
	 fadd dword ptr [edi+8]
	 fxch
	 fstp dword ptr [esi]
	 fstp dword ptr [esi+8]
	 fstp dword ptr [esi+4]
  }*/
}


void vect_sub_C (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
  c->x=a->x - b->x;
  c->y=a->y - b->y;
  c->z=a->z - b->z;
}


void vect_auto_sub_C (AD_Vect3D *a, AD_Vect3D *b)
{
  a->x=a->x - b->x;
  a->y=a->y - b->y;
  a->z=a->z - b->z;
/*
  __asm
  {
	 mov esi, [a]
	 mov edi, [b]

	 fld dword ptr [esi]
	 fsub dword ptr [edi]
	 fld dword ptr [esi+4]
	 fsub dword ptr [edi+4]
	 fxch
	 fld dword ptr [esi+8]
	 fsub dword ptr [edi+8]
	 fxch
	 fstp dword ptr [esi]
	 fstp dword ptr [esi+8]
	 fstp dword ptr [esi+4]
  }*/
}


void vect_neg_C (AD_Vect3D *a, AD_Vect3D *b)
{
  b->x=-a->x;
  b->y=-a->y;
  b->z=-a->z;
}


void vect_auto_neg_C (AD_Vect3D *a)
{
  a->x=-a->x;
  a->y=-a->y;
  a->z=-a->z;
}


void vect_scale_C (AD_Vect3D *a, float4 s, AD_Vect3D *b)
{
  b->x=s*a->x;
  b->y=s*a->y;
  b->z=s*a->z;
}


void vect_auto_scale_C (AD_Vect3D *a, float4 s)
{
  a->x=s*a->x;
  a->y=s*a->y;
  a->z=s*a->z;
}


float4 vect_dot_C (AD_Vect3D *a, AD_Vect3D *b)
{ 
  return(a->x*b->x + a->y*b->y + a->z*b->z);
}


void vect_cross_C (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
  c->x=a->y*b->z - b->y*a->z;
  c->y=b->x*a->z - a->x*b->z;
  c->z=a->x*b->y - b->x*a->y;
}


float4 vect_length_C (AD_Vect3D *a)
{
  return (mysqrt_2(a->x*a->x+a->y*a->y+a->z*a->z));
}


float4 vect_length2_C (AD_Vect3D *a)
{
  return (a->x*a->x+a->y*a->y+a->z*a->z);
}


void vect_auto_normalize_C (AD_Vect3D *a)
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


void vect_normalize_C (AD_Vect3D *a, AD_Vect3D *b)
{
  float4 aux;

  aux=1.0f/mysqrt_2(a->x*a->x+a->y*a->y+a->z*a->z);
  b->x=a->x*aux;
  b->y=a->y*aux;
  b->z=a->z*aux;
}


// *******************************************************************
//                   Implementazioni in Asm Pemtium
// *******************************************************************
void vect_add_asm (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
  __asm
  {
	 mov esi, [a]
     xor ebx, ebx
	 mov edi, [b]
	 mov ebx, [c]
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


void vect_sub_asm (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
  __asm
  {
	 mov esi, [a]
     xor ebx, ebx
	 mov ebx, [c]
	 mov edi, [b]
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


float4 vect_dot_asm (AD_Vect3D *a, AD_Vect3D *b)
{
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
}


void vect_normalize_asm (AD_Vect3D *a, AD_Vect3D *b)
{
  float4 aux;

  _asm
  {
	mov esi, [a]
	mov edi, [b]
	
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

	fstp dword ptr [aux]  // salvo 1/sqrt() in aux

	fld dword ptr [esi]
	fmul dword ptr [aux]
	fld dword ptr [esi+4]
	fmul dword ptr [aux]
	fld dword ptr [esi+8]
	fmul dword ptr [aux]
	fstp  dword ptr [edi+8]
	fstp  dword ptr [edi+4]
	fstp  dword ptr [edi]
  }
}


void vect_auto_normalize_asm (AD_Vect3D *a)
{
  float4 aux;

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

	fstp dword ptr [aux]  // salvo 1/sqrt() in aux

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

// *******************************************************************
//                      Implementazioni in 3DNow!
// *******************************************************************
#ifdef  _MSC_VER
void vect_add_3DNow (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
    __asm
    {
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[b]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        pfadd       mm0,[edx]
        mov         eax,[c]
        pfadd       mm1,[edx+8]
        movq        [eax],mm0
        movd        [eax+8],mm1
        femms
    }
}
void vect_auto_add_3DNow (AD_Vect3D *a, AD_Vect3D *b)
{
    __asm
    {
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[b]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        mov         eax,[a]
        movd        mm2,[edx+8]
        pfadd       mm0,[edx]
        pfadd       mm1,mm2
        movq        [eax],mm0
        movd        [eax+8],mm1
        femms
    }
}
void vect_sub_3DNow (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
    __asm
    {
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[b]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        mov         eax,[c]
        pfsub       mm0,[edx]
        movd        mm2,[edx+8]
        pfsub       mm1,mm2
        movq        [eax],mm0
        movd        [eax+8],mm1
        femms
    }
}
void vect_auto_sub_3DNow (AD_Vect3D *a, AD_Vect3D *b)
{
    __asm
    {
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[b]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        pfsub       mm0,[edx]
        pfsub       mm1,[edx+8]
        movq        [eax],mm0
        movd        [eax+8],mm1
        femms
    }
}
void vect_neg_3DNow (AD_Vect3D *a, AD_Vect3D *b)
{
    __asm
	{
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[b]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
		pfsub       mm2, mm2
		pfsub       mm3, mm3
		pfsub       mm2, mm0
		pfsub       mm3, mm1
		movq        [edx], mm2
		movd       [edx+8], mm3
        femms
	}
}
void vect_auto_neg_3DNow (AD_Vect3D *a)
{
    __asm
	{
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[a]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
		pfsub       mm2, mm2
		pfsub       mm3, mm3
		pfsub       mm2, mm0
		pfsub       mm3, mm1
		movq        [edx], mm2
		movd        [edx+8], mm3
        femms
	}
}
void vect_scale_3DNow (AD_Vect3D *a, float4 s, AD_Vect3D *b)
{
    __asm
    {
        ALIGN       16
        femms
        mov         eax,[a]
        movd        mm0,[s]
        movq        mm3,[eax]
        punpckldq   mm0,mm0
        movd        mm2,[eax+8]
        movq        mm1,mm0
        pfmul       mm0,mm3
        mov         eax,[b] 
        pfmul       mm1,mm2 
        movq        [eax],mm0
        movd        [eax+8],mm1
        femms
    }
}
void vect_auto_scale_3DNow (AD_Vect3D *a, float4 s)
{
    __asm
    {
        ALIGN       16
        femms
        mov         eax,[a]
        movd        mm0,[s]
        movq        mm3,[eax]
        punpckldq   mm0,mm0
        movd        mm2,[eax+8]
        movq        mm1,mm0
        pfmul       mm0,mm3
        pfmul       mm1,mm2 
        movq        [eax],mm0
        movd        [eax+8],mm1
        femms
    }
}
float4 vect_dot_3DNow (AD_Vect3D *a, AD_Vect3D *b)
{
    float result;

    __asm {
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[b]
        movq        mm0,[eax]
        movq		mm3,[edx]
        pfmul       mm0,mm3
        movd		mm2,[eax+8]
        movd        mm1,[edx+8]
        pfacc       mm0,mm0
        pfmul       mm1,mm2
        pfadd       mm0,mm1
        movd        result,mm0
        femms
    }
    return result;
}
void vect_cross_3DNow (AD_Vect3D *a, AD_Vect3D *b, AD_Vect3D *c)
{
    __asm
    {
        ALIGN       16
        femms
        mov         edx,[b]
        mov         eax,[a]
        movd        mm0,[edx+8]
        movd        mm1,[eax+8]
        punpckldq   mm0,mm0
        movd        mm3,[eax+4]
        punpckldq   mm1,mm1
        pfmul       mm0,[eax]
        movq        mm5,[edx]
        movd        mm4,[edx+4]
        pfmul       mm1,mm5
        movq        mm2,mm0
        pfmul       mm3,[edx]
        pfsub       mm0,mm1
        pfmul       mm4,[eax]
        mov         eax,[c]
        pfsub       mm1,mm2
        punpckhdq   mm0,mm0
        pfsub       mm4,mm3
        punpckldq   mm0,mm1
        movd        [eax],mm0
        movd        [eax+4],mm1
        movd        [eax+8],mm4
        femms
    }
}
float4 vect_length_3DNow (AD_Vect3D *a)
{
    float result;

    __asm {
        ALIGN       16
        femms
        mov         eax,[a]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        pfmul       mm0,mm0
        pfmul       mm1,mm1
        pfacc       mm0,mm0
        pfadd       mm0,mm1
        pfrsqrt     mm1,mm0
        movq        mm2,mm1
        pfmul       mm1,mm1
        pfrsqit1    mm1,mm0
        pfrcpit2    mm1,mm2
        pfmul       mm0,mm1
        movd        result,mm0
        femms
    }
    return result;
}
float4 vect_length2_3DNow (AD_Vect3D *a)
{
    float result;

    __asm {
        ALIGN       16
        femms
        mov         eax,[a]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        pfmul       mm0,mm0
        pfmul       mm1,mm1
        pfacc       mm0,mm0
        pfadd       mm0,mm1
        movd        result,mm0
        femms
    }
    return result;
}
void vect_normalize_3DNow (AD_Vect3D *a, AD_Vect3D *b)
{
    __asm
    {
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[b]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        movq        mm4,mm0
        movq        mm3,mm1
        pfmul       mm0,mm0 
        pfmul       mm1,mm1 
        pfacc       mm0,mm0 
        pfadd       mm0,mm1 
        movd        eax,mm0
        pfrsqrt     mm1,mm0 
        movq        mm2,mm1
        pfmul       mm1,mm1
        pfrsqit1    mm1,mm0
        pfrcpit2    mm1,mm2
        punpckldq   mm1,mm1
        pfmul       mm3,mm1
        pfmul       mm4,mm1
        movd        [edx+8],mm3
        movq        [edx],mm4
        femms
    }
}
void vect_auto_normalize_3DNow (AD_Vect3D *a)
{
    __asm
    {
        ALIGN       16
        femms
        mov         eax,[a]
        mov         edx,[a]
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        movq        mm4,mm0
        movq        mm3,mm1
        pfmul       mm0,mm0 
        pfmul       mm1,mm1 
        pfacc       mm0,mm0 
        pfadd       mm0,mm1 
        movd        eax,mm0
        pfrsqrt     mm1,mm0 
        movq        mm2,mm1
        pfmul       mm1,mm1
        pfrsqit1    mm1,mm0
        pfrcpit2    mm1,mm2
        punpckldq   mm1,mm1
        pfmul       mm3,mm1
        pfmul       mm4,mm1
        movd        [edx+8],mm3
        movq        [edx],mm4
        femms
    }
}
#endif

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