// Funzioni di base condivise

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <windows.h>
#include <stdio.h>

#ifdef  _MSC_VER
#include "cpu_detect.h"
#endif

#include "types.h"
#define sqr(a) ((a)*(a))

// -------------------------------------------------------------------
void debug_error (HWND hWnd, char8 *messaggio);
// -------------------------------------------------------------------
void memsetd (void *dest, uint32 val, uint32 dwords);
// -------------------------------------------------------------------
void memcpyd (void *dest, void *sorg, int32 dwords);
// -------------------------------------------------------------------
void rgb_to_hsv (float4 r, float4 g, float4 b, float4 *h, float4 *s, float4 *v);
// -------------------------------------------------------------------
void hsv_to_rgb (float4 h, float4 s, float4 v, float4 *r, float4 *g, float4 *b);
// -------------------------------------------------------------------
float4 my_atof (char8 *s);
// -------------------------------------------------------------------
char8 esiste_file (char8 *nome);
// -------------------------------------------------------------------
void write_string0 (FILE *f, char8 *st);
// -------------------------------------------------------------------
void read_string0 (FILE *f, char8 *st);
// -------------------------------------------------------------------
float4 map_intervals (float4 Interval1Start, float4 Interval1End, 
                      float4 Interval2Start, float4 Interval2End,
					  float4 Interval1Position);
// -------------------------------------------------------------------
inline uint32 RGBAdd(uint32 a, uint32 b)
{ 
  uint32 overflow;

  a &= 0xfefeff; 
  b=(b & 0xfefeff)+a;
  overflow=b & 0x1010100;
  overflow=overflow-(overflow>>8);
  return (b | overflow);
}
// -------------------------------------------------------------------
inline uint32 RGBSub(uint32 a, uint32 b)
{ 
  uint32 overflow;

  b&=0xfefeff;
  a=(a|0x1010100)-b;
  overflow=a&0x1010100;
  overflow-=overflow>>8;
  return a&overflow;
}
// -------------------------------------------------------------------
inline uint32 RGBblend(uint32 c1, uint32 c2, uchar8 alp)
{
   uint32 r1, g1, b1;
   uint32 r2, g2, b2;
   uint32 r, g, b;

   r1=(c1 >> 16) & 0xff;
   g1=(c1 >> 8) & 0xff;
   b1=c1 & 0xff;

   r2=(c2 >> 16) & 0xff;
   g2=(c2 >> 8) & 0xff;
   b2=c2 & 0xff;

   r=((alp*r1)+(255-alp)*r2) >>8;
   g=((alp*g1)+(255-alp)*g2) >>8;
   b=((alp*b1)+(255-alp)*b2) >>8;

   return (b | (g<<8) | (r<<16));
}
// -------------------------------------------------------------------
// funzioni di supporto per le CPU
int32 cpu_isK6III (void);
// -------------------------------------------------------------------
int32 cpu_isAthlon (void);
// -------------------------------------------------------------------
int32 cpu_has3DNow (void);
// -------------------------------------------------------------------
int32 cpu_hasMMX (void);
// -------------------------------------------------------------------
int32 cpu_hasSSE (void);
// -------------------------------------------------------------------
inline float4 relPos(float4 start, float4 end, float4 interpos)
{ return((interpos-start)/(end-start)); }

#endif