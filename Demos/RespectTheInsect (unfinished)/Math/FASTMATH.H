// Fast Math Library, made by [AD]Turbo

#ifndef _FASTMATH_H_
#define _FASTMATH_H_

#include "..\types.h"
#include "..\utility.h"
#include "vect3D.h"

// sono in fastmath.cpp
extern float Scale16, Scale5, half;
extern unsigned int fast_sqrt_table[256];
// ###########################################################
// ################  FAST MATH PROCEDURES  ###################
// ###########################################################


// prima di utilizzare qualsiasi funzione chiamare questa init
void init_fast_math_library(void);
void build_sqrt_tables(void);


inline int ceil16(int value)
{ return ((value+0xffff)>>16); }


inline float fceil(float value)
{
	float result;
	_asm
	{
		fld     value
        fadd    dword ptr half
        frndint
        fstp    result
	}
	return(result);
}


inline float mysqrt(float value)
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


inline int myfist(float4 fvalue)
{
// converte in intero il valore float
  int result;
	_asm
	{
	  fld fvalue
	  fistp dword ptr result
	}
	return(result);
}


inline int myfist16 (float4 fvalue)
{
// converte in fixed point 16.16 il valore float passato
  int result;

 _asm
 {
   fld fvalue
   fmul Scale16
   fistp dword ptr result
 }
 return(result);
}


inline int myfist5 (float4 fvalue)
// converte in fixed point 27.5 il valore float passato
{
  int result;

 _asm
 {
   fld fvalue
   fmul Scale5
   fistp dword ptr result
 }
 return(result);
}


void my2fist16 (float4 *fvalues, int *ivalues);
int imul16(int x, int y);  // (x * y) >> 16
int imul14(int x, int y);  // (x * y) >> 14
int idiv16(int x, int y);  // (x << 16) / y



inline float fast_sqrt(float n)
{
    unsigned int *num = (unsigned *)&n;
    short e;

//    if (n == 0) return (0); 
    e = (*num >> 23) - 127; 
    *num &= 0x7fffff;  
    if (e & 0x01) *num |= 0x800000;
    e >>= 1;
    *num = ((fast_sqrt_table[*num >> 16])) + ((e + 127) << 23);
    return(n);
}


inline float vect_fast_lenght(AD_Vect3D *a)
{ return (fast_sqrt(a->x*a->x+a->y*a->y+a->z*a->z)); }


float fast_rcpsqrt(float n);

void vect_fast_normalize(AD_Vect3D *a);

#define FP_BITS(fp) (*(int *)&(fp))
#define fast_float_abs(fp) (FP_BITS(fp)&0x7FFFFFFF)
#define is_float_negative(fp) (FP_BITS(fp) & 0x80000000)
#define is_float_positive(fp) ((FP_BITS(fp) & 0x80000000) ^ 0x80000000)

#endif