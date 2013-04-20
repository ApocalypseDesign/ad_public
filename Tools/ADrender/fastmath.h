// Fast Math Library, riadattata da [AD]Turbo per il VC6
// Versione ridoatta per la ADRender !

#ifndef _FASTMATH_H_
#define _FASTMATH_H_

#include "types.h"

// sono in fastmath.cpp
extern float Scale16_lib, Scale5_lib, half_lib;

inline int ceil16(int value)
{ return ((value+0xffff)>>16); }


inline float fceil(float value)
{
	float result;
	_asm
	{
		fld     value
        fadd    dword ptr half_lib
        frndint
        fstp    result
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
   fmul Scale16_lib
   fistp dword ptr result
 }
 return(result);
}

#define FP_BITS(fp) (*(int *)&(fp))
#define fast_float_abs(fp) (FP_BITS(fp)&0x7FFFFFFF)
#define is_float_negative(fp) (FP_BITS(fp) & 0x80000000)
#define is_float_positive(fp) ((FP_BITS(fp) & 0x80000000) ^ 0x80000000)

#endif