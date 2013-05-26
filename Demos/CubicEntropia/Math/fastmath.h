// Fast Math Library, made by [AD]Turbo

#ifndef _FASTMATH_H_
#define _FASTMATH_H_

#include "Utility.h"
#include "Vect3D.h"

// sono in fastmath.cpp
extern float4 Scale16, Scale5, half;
extern uint32 fast_sqrt_table[256];
extern float4 Pi__;
extern float4 Pi2__;
extern float4 InvPi__;

// ###########################################################
// ################  FAST MATH PROCEDURES  ###################
// ###########################################################


// prima di utilizzare qualsiasi funzione chiamare questa init
void init_fast_math_library(void);
void build_sqrt_tables(void);


inline int32 ceil16(int32 value)
{ return ((value+0xffff)>>16); }

#ifdef  _MSC_VER
inline float4 fceil(float4 value)
{
	float4 result;
	_asm
	{
		fld     value
        fadd    dword ptr half
        frndint
        fstp    result
	}
	return(result);
}
#else
inline float4 fceil(float4 value)
{
	return(ceil(value));
}
#endif


#ifdef  _MSC_VER
inline float4 mysqrt(float4 value)
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
inline float4 mysqrt(float4 value)
{
	return((float4)sqrt(value));
}
#endif



#ifdef  _MSC_VER
inline int32 myfist(float4 fvalue)
{
// converte in intero il valore float
  int32 result;
	_asm
	{
	  fld fvalue
	  fistp dword ptr result
	}
	return(result);
}
#else
inline int32 myfist(float4 fvalue)
{
  return((int32)fvalue);
}
#endif



#ifdef  _MSC_VER
inline int32 myfist16 (float4 fvalue)
{
  // converte in fixed point 16.16 il valore float passato
  int32 result;

  _asm
  {
    fld fvalue
    fmul Scale16
    fistp dword ptr result
  }
  return(result);
}
#else
inline int32 myfist16 (float4 fvalue)
{
  return((int32)(fvalue*Scale16));
}
#endif



#ifdef  _MSC_VER
inline int32 myfist5 (float4 fvalue)
// converte in fixed point 27.5 il valore float passato
{
  int32 result;

  _asm
  {
    fld fvalue
    fmul Scale5
    fistp dword ptr result
  }
  return(result);
}
#else
inline int32 myfist5 (float4 fvalue)
// converte in fixed point 27.5 il valore float passato
{
  return((int32)(fvalue*Scale5));
}
#endif



void my2fist16 (float4 *fvalues, int32 *ivalues);
int32 imul16(int32 x, int32 y);  // (x * y) >> 16
int32 imul14(int32 x, int32 y);  // (x * y) >> 14
int32 idiv16(int32 x, int32 y);  // (x << 16) / y



inline float4 fast_sqrt(float4 n)
{
    uint32 *num = (unsigned *)&n;
    short e;

//    if (n == 0) return (0); 
    e = (*num >> 23) - 127; 
    *num &= 0x7fffff;  
    if (e & 0x01) *num |= 0x800000;
    e >>= 1;
    *num = ((fast_sqrt_table[*num >> 16])) + ((e + 127) << 23);
    return(n);
}


inline float4 vect_fast_lenght(AD_Vect3D *a)
{ return (fast_sqrt(a->x*a->x+a->y*a->y+a->z*a->z)); }


float4 fast_rcpsqrt(float4 n);

void vect_table_normalize(AD_Vect3D *a, AD_Vect3D *out);
void vect_table_auto_normalize(AD_Vect3D *a);

#define FP_BITS(fp) (*(int32 *)&(fp))
#define fast_float_abs(fp) (FP_BITS(fp)&0x7FFFFFFF)
#define is_float_negative(fp) (FP_BITS(fp) & 0x80000000)
#define is_float_positive(fp) ((FP_BITS(fp) & 0x80000000) ^ 0x80000000)

float4 fast_sinf(float4 rad);
float4 fast_cosf(float4 rad);


//********************************************************************
//                        Noise del 3dsmax
//********************************************************************

#define B 0x100
#define BM 0xff
#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff
extern int32 p[B + B + 2];
extern float4 g[B + B + 2][3];
float4 noise3(float4 f1, float4 f2, float4 f3);

#endif