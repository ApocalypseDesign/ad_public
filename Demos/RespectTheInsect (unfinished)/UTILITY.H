// Funzioni di base condivise

#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <windows.h>
#include <stdio.h>

#define Pi__ 3.14159265358979323846
#define M_PI  3.14159265358979323846
#define sqr(a) ((a)*(a))

void debug_error(HWND hWnd, char *messaggio);
void memsetd(void *dest, unsigned int val, int dwords);
void memcpyd(void *dest, void *sorg, int dwords);
void rgb_to_hsv(float r, float g, float b, float *h, float *s, float *v);
void hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b);
float my_atof(char *s);
char esiste_file (char *nome);
void write_string0 (FILE *f, char *st);

inline unsigned int RGBAdd(unsigned int a, unsigned int b)
{ unsigned int overflow;
  a &= 0xfefeff; 
  b=(b & 0xfefeff)+a;
  overflow=b & 0x1010100;
  overflow=overflow-(overflow>>8);
  return (b | overflow);
}

inline int RGBSub(int a, int b)
{ int overflow;
  b&=0xfefeff;
  a=(a|0x1010100)-b;
  overflow=a&0x1010100;
  overflow-=overflow>>8;
  return a&overflow;
}

inline unsigned int RGBblend(unsigned int c1, unsigned int c2, unsigned char alp)
{
   unsigned int r1, g1, b1;
   unsigned int r2, g2, b2;
   unsigned int r, g, b;

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


#endif