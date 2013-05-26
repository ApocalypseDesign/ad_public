// Fast Math Library, riadattata da [AD]Turbo per il VC6

#include <math.h>
#include "FastMath.h"
#include "Matrix.h"

uint32 fast_sqrt_table[256];
uint32 fast_rcpsqrt_table[256];
float4 Scale16=65536.0;  // usata nella myfist16
float4 Scale5=32.0;  // usata nella myfist5
float4 half=0.5;         // usata nella fceil
float4 Pi__ = (float4)3.141592654;
float4 Pi2__ = (float4)6.283185307;
float4 InvPi__ = (float4)0.318309886;

short OldFPUCW, FPUCW;

//********************************************************************
//                        Noise del 3dsmax
//********************************************************************
#define ADOT(a,b) (a[0] * b[0] + a[1] * b[1] + a[2] * b[2])

#define setup(u,b0,b1,r0,r1) \
	t = u + N; \
	b0 = ((int32)t) & BM; \
	b1 = (b0+1) & BM; \
	r0 = t - (float4)((int32)t); \
	r1 = r0 - 1.0f; \

#define at(rx,ry,rz) (rx * q[0] + ry * q[1] + rz * q[2])
#define s_curve(t) (t * t * (3.0f - 2.0f * t))
#define lerp(t, a, b) (a + t*(b - a))
static int32 p[B + B + 2];
static float4 g[B + B + 2][3];

// ###########################################################
// ################  FAST MATH PROCEDURES  ###################
// ###########################################################

void build_sqrt_tables(void)
{
    unsigned short i;
    float4 f;
    uint32 *fi = (unsigned*)&f;  
                                       
    for (i = 0; i <= 0x7f; i++) 
	{

        *fi = 0;

        // first part of table:
        *fi = (i << 16) | (127 << 23);
        f = (float4)sqrt(f);
        fast_sqrt_table[i] = (*fi & 0x7fffff);

        *fi = (i << 16) | (127 << 23);
        f = 1.0f/(float4)sqrt(f);
        fast_rcpsqrt_table[i] = (*fi & 0x7fffff);


        // second part of table (odd exponent)
        *fi = 0;
        *fi = (i << 16) | (128 << 23);
        f = (float4)sqrt(f);
        fast_sqrt_table[i+0x80] = (*fi & 0x7fffff);

        *fi = (i << 16) | (128 << 23);
        f = 1.0f/(float4)sqrt(f);
        fast_rcpsqrt_table[i+0x80] = (*fi & 0x7fffff);
    }
}


void init_fast_math_library(void)
{
  int32 K6III, Athlon, Has3DNow;
  int32 i, j, k;

#ifdef  _MSC_VER
  K6III=cpu_isK6III();
  Athlon=cpu_isAthlon();
  Has3DNow=cpu_has3DNow();
  // mettiamo le procedure 3DNow + veloci su tutte le cpu
  if (Has3DNow)
  {
     vect_normalize=*vect_normalize_3DNow;
     vect_auto_normalize=*vect_auto_normalize_3DNow;
     vect_length=*vect_length_3DNow;
	 mat_mulvect=mat_mulvect_3DNow;
	 mat_mul=mat_mul_3DNow;
	 // ADD, SUB, CROSS e SCALE sono + veloci per 3DNow solo sui K6III
	 if (K6III)
	 {
		 vect_add=*vect_add_3DNow;
		 vect_auto_add=*vect_auto_add_3DNow;
         vect_sub=*vect_sub_3DNow;
         vect_auto_sub=*vect_auto_sub_3DNow;
         vect_cross=*vect_cross_3DNow;
         vect_scale=*vect_scale_3DNow;
         vect_auto_scale=*vect_auto_scale_3DNow;
	 }
	 else
	 {
		 vect_add=*vect_add_C;
		 vect_auto_add=*vect_auto_add_C;
         vect_sub=*vect_sub_C;
         vect_auto_sub=*vect_auto_sub_C;
         vect_cross=*vect_cross_C;
         vect_scale=*vect_scale_C;
         vect_auto_scale=*vect_auto_scale_C;
	 }
  }
  else
  {
     vect_normalize=*vect_normalize_asm;
     vect_auto_normalize=*vect_auto_normalize_asm;
     vect_length=*vect_length_C;  // FARE IN ASM!
     vect_cross=*vect_cross_C;
     vect_scale=*vect_scale_C;
     vect_auto_scale=*vect_auto_scale_C;
	 mat_mulvect=mat_mulvect_C;
	 mat_mul=mat_mul_C;
     vect_add=*vect_add_C;
	 vect_auto_add=*vect_auto_add_C;
     vect_sub=*vect_sub_C;
     vect_auto_sub=*vect_auto_sub_C;
  }
#else
     vect_normalize=*vect_normalize_C;
     vect_auto_normalize=*vect_auto_normalize_C;
     vect_length=*vect_length_C;  // FARE IN ASM!
     vect_cross=*vect_cross_C;
     vect_scale=*vect_scale_C;
     vect_auto_scale=*vect_auto_scale_C;
	 mat_mulvect=mat_mulvect_C;
	 mat_mul=mat_mul_C;
     vect_add=*vect_add_C;
	 vect_auto_add=*vect_auto_add_C;
     vect_sub=*vect_sub_C;
     vect_auto_sub=*vect_auto_sub_C;
#endif
  
  // procedure che sono comunque più veloci in C su tutte le CPU
  // (o perchè manca un'implementazione più veloce (in asm?))
  vect_auto_neg=vect_auto_neg_C;
  vect_neg=vect_neg_C;
  vect_length2=*vect_length2_C;
  vect_dot=*vect_dot_C;


  build_sqrt_tables();
  /*
  _asm
  {
    fstcw   [OldFPUCW]
    mov     ax,OldFPUCW
    and     eax, 0xcff
    mov     [FPUCW], ax
    fldcw   [FPUCW]
  }
  */
  // -----------------------------------------------------------------
  //                      noise3 del 3dsmax
  // -----------------------------------------------------------------
   float4 v[3], s;
   // Create an array of random vectors uniformly on the unit sphere
   srand(1);
   for (i=0; i<B; i++) {
		do {				// Choose uniformly in a cube
			for (j=0; j<3 ; j++)
				v[j] = (float4)((rand() % (B + B)) - B) / B;
			s = ADOT(v,v);
		} while (s > 1.0);		// If not in sphere try again
		s = (float4)(sqrt(s));
		for (j=0; j<3 ; j++)	// Else normalize
			g[i][j] = v[j] / s;
	}

	// Create a pseudorandom permutation of [1..B]
	for (i=0; i<B; i++)	p[i] = i;
	for (i=B; i>0; i-=2) {
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	/* Extend g and p arrays to allow for faster indexing */
	for (i=0; i<B + 2; i++) {
		p[B + i] = p[i];
		for (j=0; j<3 ; j++) g[B + i][j] = g[i][j];
	}
}



void my2fist16 (float4 *fvalues, int32 *ivalues)
{
// da asmare
  ivalues[0]=(int32)(fvalues[0]*Scale16);
  ivalues[1]=(int32)(fvalues[1]*Scale16);
}


int32 imul16(int32 x, int32 y)  // (x * y) >> 16
{
  int32 result;
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


int32 imul14(int32 x, int32 y)  // (x * y) >> 14
{
  int32 result;
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


int32 idiv16(int32 x, int32 y)  // (x << 16) / y
{
  int32 result;
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


inline float4 fastsqrt(float4 n)
{
   uint32 a, b;
   int32 d;

   a=*(uint32 *)&n;
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
   n=*(float4 *)&a;

   return n;
}


// fast 1/sqrt
float4 fast_rcpsqrt(float4 n)
{
    uint32 *num = (unsigned *)&n;
    int32 e;  

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


void vect_table_auto_normalize(AD_Vect3D *a)
{
  float4 aux;

  aux=1.0f/fastsqrt(vect_dot(a, a));
  a->x=a->x*aux;
  a->y=a->y*aux;
  a->z=a->z*aux;
}


void vect_table_normalize(AD_Vect3D *a, AD_Vect3D *out)
{
  float4 aux;

  aux=1.0f/fastsqrt(vect_dot(a, a));
  out->x=a->x*aux;
  out->y=a->y*aux;
  out->z=a->z*aux;
}


float4 fast_sinf(float4 rad)
{
   float4 result;

   _asm
   {
	  fld dword ptr [rad]
      fsin
      fstp dword ptr [result]
   }
   return(result);
}


float4 fast_cosf(float4 rad)
{
   float4 result;
  _asm
   {
	  fld dword ptr [rad]
      fcos
      fstp dword ptr [result]
   }
   return(result);
}

//********************************************************************
//                        Noise del 3dsmax
//********************************************************************
float4 noise3(float4 f1, float4 f2, float4 f3)
{
	int32 bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	float4 rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	int32 i, j;

	setup(f1, bx0,bx1, rx0,rx1);
	setup(f2, by0,by1, ry0,ry1);
	setup(f3, bz0,bz1, rz0,rz1);

	i = p[bx0];
	j = p[bx1];

	b00 = p[i + by0];
	b10 = p[j + by0];
	b01 = p[i + by1];
	b11 = p[j + by1];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

	q = g[b00 + bz0] ; u = at(rx0, ry0, rz0);
	q = g[b10 + bz0] ; v = at(rx1, ry0, rz0);
	a = lerp(t, u, v);

	q = g[b01 + bz0] ; u = at(rx0, ry1, rz0);
	q = g[b11 + bz0] ; v = at(rx1, ry1, rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);		/* interpolate in y at lo x */

	q = g[b00 + bz1] ; u = at(rx0, ry0, rz1);
	q = g[b10 + bz1] ; v = at(rx1, ry0, rz1);
	a = lerp(t, u, v);

	q = g[b01 + bz1] ; u = at(rx0, ry1, rz1);
	q = g[b11 + bz1] ; v = at(rx1, ry1, rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);		/* interpolate in y at hi x */

	return (1.5f * lerp(sz, c, d));	/* interpolate in z */
}