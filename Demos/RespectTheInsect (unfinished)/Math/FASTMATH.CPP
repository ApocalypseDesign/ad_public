// Fast Math Library, riadattata da [AD]Turbo per il VC6

#include <math.h>
#include "fastmath.h"
#include "vect3d.h"

unsigned int fast_sqrt_table[256];
unsigned int fast_rcpsqrt_table[256];
float Scale16=65536.0;  // usata nella myfist16
float Scale5=32.0;  // usata nella myfist5
float half=0.5;         // usata nella fceil
short OldFPUCW, FPUCW;

// ###########################################################
// ################  FAST MATH PROCEDURES  ###################
// ###########################################################

void build_sqrt_tables(void)
{
    unsigned short i;
    float f;
    unsigned int *fi = (unsigned*)&f;  
                                       
    for(i = 0; i <= 0x7f; i++) 
	{

        *fi = 0;

        // first part of table:
        *fi = (i << 16) | (127 << 23);
        f = (float)sqrt(f);
        fast_sqrt_table[i] = (*fi & 0x7fffff);

        *fi = (i << 16) | (127 << 23);
        f = 1.0f/(float)sqrt(f);
        fast_rcpsqrt_table[i] = (*fi & 0x7fffff);


        // second part of table (odd exponent)
        *fi = 0;
        *fi = (i << 16) | (128 << 23);
        f = (float)sqrt(f);
        fast_sqrt_table[i+0x80] = (*fi & 0x7fffff);

        *fi = (i << 16) | (128 << 23);
        f = 1.0f/(float)sqrt(f);
        fast_rcpsqrt_table[i+0x80] = (*fi & 0x7fffff);
    }
}


void init_fast_math_library(void)
{
  build_sqrt_tables();
  _asm
  {
    fstcw   [OldFPUCW]
    mov     ax,OldFPUCW
    and     eax, 0xcff
    mov     [FPUCW], ax
    fldcw   [FPUCW]
  }
}



void my2fist16 (float4 *fvalues, int *ivalues)
{
// da asmare
  ivalues[0]=(int)(fvalues[0]*Scale16);
  ivalues[1]=(int)(fvalues[1]*Scale16);
}


int imul16(int x, int y)  // (x * y) >> 16
{
  int result;
  _asm
  {
	push eax
	push edx
    mov eax, x
	imul y
	shrd eax, edx, 16
	mov result, eax
	pop edx
	pop eax
  }
  return(result);
}


int imul14(int x, int y)  // (x * y) >> 14
{
  int result;
  _asm
  {
	push eax
	push edx
    mov eax, x
	imul y
	shrd eax, edx, 14
	mov result, eax
	pop edx
	pop eax
  }
  return(result);
}


int idiv16(int x, int y)  // (x << 16) / y
{
  int result;
  _asm
  {
	push eax
	push edx
    mov eax, x
	mov edx, eax
	sar edx, 16
    shl eax, 16
    idiv  y
	mov result, eax
    pop edx
	pop eax
  }
  return(result);
}


inline float fastsqrt(float n)
{
   unsigned int a, b;
   int d;

   a=*(unsigned int *)&n;
   d=a;
   a=a & 0x7FFFFF;
   d=d-0x3F800000;
   b=d;
   d=(d & 0xFF000000) >> 1;  // SAR
   b=b & 0x800000;
   d=d + 0x3F800000;
   a=a | b;
   a=a >> 16;  // SHR
   a=fast_sqrt_table[a];
   a=a + d;
   n=*(float *)&a;

   return n;
}


// fast 1/sqrt
float fast_rcpsqrt(float n)
{
    unsigned int *num = (unsigned *)&n;
    int e;  

//    if (n == 0) return (0); 
    e = (*num >> 23) - 127; 
                            
    *num &= 0x7fffff;  
    if (e & 0x01) *num |= 0x800000;

    e = e >> 1;
    if(*num) e = -(e + 1);
    else     e = -e;

    *num = ((fast_rcpsqrt_table[*num >> 16])) + ((e + 127) << 23);
    return(n);
}


void vect_fast_normalize(AD_Vect3D *a)
{
  float4 aux;

  aux=1.0f/fastsqrt(vect_dot(a, a));
  a->x=a->x*aux;
  a->y=a->y*aux;
  a->z=a->z*aux;
}