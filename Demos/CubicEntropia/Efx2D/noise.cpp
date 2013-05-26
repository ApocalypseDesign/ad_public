#include "noise.h"
#include "..\Math\fastmath.h"

#define MAXKNOTS 16
#define KNOTSMASK 15
#define SHINESS 128

float knots1[MAXKNOTS];
float knots2[MAXKNOTS][MAXKNOTS];
float knots3[MAXKNOTS][MAXKNOTS][MAXKNOTS];
int palette[256];
extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

inline float interpola (float a, float b, float t)
{
//	float ft = t * 3.1415927.0;
//	float f = (1 - cos(ft)) * .5;
//	return  (a*(1.0-f) + b*f);
	float t2=t*t;
	float t3=t2*t;
	float a_b=a-b;
	float res=2.0f*a_b*t3 -3.0f*a_b*t2 +a;
	return res;
}


// ----------------------------------------------------------
//                       Versione 1D
// ----------------------------------------------------------
noise1D::noise1D()
{
  return;
}


void noise1D::init()
{
  int i;

  for (i=0; i<MAXKNOTS; i++)
  {
    knots1[i]=noise1(i);
    knots1[i]=(knots1[i]*SHINESS);
  }
  return;
}


void noise1D::do_efx2D(double pos)
{
  return;
}


inline float interpolated_noise1(float x)
{
   int ix;
   float n1, n2, fr;

   ix=(int)x;
   fr=x-ix;
   n1=knots1[ix & KNOTSMASK];
   n2=knots1[(ix+1) & KNOTSMASK];

   // interpolazione dei tra i due valori
   return(interpola(n1, n2, fr));
}


float noise1D::Perlin(float x, float persistence, int octaves)
{
   float total, frequency, amplitude;
   int i;

   total=0;
   frequency=1;
   amplitude=1;
   for (i=0; i<octaves; i++)
   {
      total = total + interpolated_noise1(x*frequency)*amplitude;
	  frequency*=2;
	  amplitude*=persistence;
   }
   return total;
}



// ----------------------------------------------------------
//                       Versione 2D
// ----------------------------------------------------------
noise2D::noise2D()
{
  return;
}


void noise2D::init()
{
  int i, j;

  for (i=0; i<MAXKNOTS; i++)
  {
    for (j=0; j<MAXKNOTS; j++)
	{
	   knots2[i][j]=noise2(i, j);
	   knots2[i][j]=(knots2[i][j]*SHINESS);
	}
  }
  return;
}


inline float interpolated_noise2(float x, float y)
{
   int ix, iy, ix1, iy1;
   float n1, n2, n3, n4, frx, fry, i1, i2;

   ix=(int)(x);
   iy=(int)(y);

   frx=x-ix;
   fry=y-iy;

   ix1=(ix+1) & KNOTSMASK;
   iy1=(iy+1) & KNOTSMASK;
   ix=ix & KNOTSMASK;
   iy=iy & KNOTSMASK;

   n1=knots2[ix][iy];
   n2=knots2[ix1][iy];
   n3=knots2[ix][iy1];
   n4=knots2[ix1][iy1];

   i1=interpola(n1, n2, frx);
   i2=interpola(n3, n4, frx);
   return(interpola(i1, i2, fry));
}


float noise2D::Perlin(float x, float y, float persistence, int octaves)
{
   float total, freqx, freqy, amplitude;
   int i;

   total=0;
   freqx=x;
   freqy=y;
   amplitude=1;
   for (i=0; i<octaves; i++)
   {
      total = total + interpolated_noise2(freqx, freqy)*amplitude;
	  freqx*=2;
	  freqy*=2;
	  amplitude*=persistence;
   }
   return total;
}


void noise2D::do_efx2D(double pos)
{
  int x, y, ip, pixel;
  float p;
  float step, step_x, step_y, fx, fy;
/*
  _asm
  {
	 push eax
	 fstcw word ptr [cwrd]
	 mov [cwrd], ax
	 or ah, 0Ch
	 mov ax, [cwrd]
	 fldcw word ptr [cwrd]
	 pop eax
  }
*/

  step=(float)(pos*2.0);
  step_x=(float)(1.0/80.0);
  step_y=(float)(1.0/60.0);

  fy=0;
  for (y=0; y<image->height; y++)
  {
	fx=step;
    for (x=0; x<image->width; x++)
	{
	   p=Perlin(fx, fy, 0.45f, 6);
	   ip=myfist(p+SHINESS);
	   if (ip<0) ip=0;
	   if (ip>255) ip=255;

	   pixel=ip | (ip<<8) | (ip<<16);
	   image->uint32ptr[image->muly[y]+x]=pixel;
	   fx+=step_x;
	}
    fy+=step_y;
  }
  return;
}



// ----------------------------------------------------------
//                       Versione 3D
// ----------------------------------------------------------
noise3D::noise3D()
{
  return;
}


void noise3D::init()
{
  int i, j, k, i1, j1, k1, i_1, j_1, k_1;
  int r, g, b;
  float k3[MAXKNOTS][MAXKNOTS][MAXKNOTS];

  for (i=0; i<MAXKNOTS; i++)
  {
    for (j=0; j<MAXKNOTS; j++)
	{
      for (k=0; k<MAXKNOTS; k++)
	  {
	     knots3[i][j][k]=noise3(i, j, k);
	     knots3[i][j][k]=knots3[i][j][k]*SHINESS;
	  }
	}
  }


  for (i=0; i<MAXKNOTS; i++)
  {
    i1=(i+1) & KNOTSMASK;
    i_1=(i-1) & KNOTSMASK;
    for (j=0; j<MAXKNOTS; j++)
	{
	  j1=(j+1) & KNOTSMASK;
	  j_1=(j-1) & KNOTSMASK;
      for (k=0; k<MAXKNOTS; k++)
	  {
		 k1=(k+1) & KNOTSMASK;
		 k_1=(k-1) & KNOTSMASK;

	     k3[i][j][k]=(
			          knots3[i_1][j_1][k]/9-
			          knots3[i][j_1][k]/9+
			          knots3[i1][j_1][k]/9-
			          knots3[i_1][j][k]/9+
			          knots3[i][j][k]+
			          knots3[i1][j][k]/9-
			          knots3[i_1][j1][k]/9+
			          knots3[i][j1][k]/9-
			          knots3[i1][j1][k]/9
					 )/1;
	  }
	}
  }

  for (i=0; i<MAXKNOTS; i++)
    for (j=0; j<MAXKNOTS; j++)
      for (k=0; k<MAXKNOTS; k++)
	     knots3[i][j][k]=k3[i][j][k];

  for (i=0; i<256; i++)
  {
     r=(int)(128.0*sin(i/256.0))+128;
//     g=(int)(100*sin(6.28*i/256.0))+100;
//     b=(int)(30*sin(6.28*i/256.0))+30;
	 g=b=r;
	 palette[i]=b | (g<<8) | (r<<16);
  }
}


inline float interpolated_noise3(float x, float y, float z)
{
   int ix, iy, iz, ix1, iy1, iz1;
   float n1, n2, n3, n4, frx, fry, frz, i1, i2, i3, i4;

   ix=(int)(x);
   iy=(int)(y);
   iz=(int)(z);

   frx=x-ix;
   fry=y-iy;
   frz=z-iz;

   ix1=(ix+1) & KNOTSMASK;
   iy1=(iy+1) & KNOTSMASK;
   iz1=(iz+1) & KNOTSMASK;
   ix=ix & KNOTSMASK;
   iy=iy & KNOTSMASK;
   iz=iz & KNOTSMASK;

   n1=knots3[ix][iy][iz];
   n2=knots3[ix1][iy][iz];
   n3=knots3[ix][iy1][iz];
   n4=knots3[ix1][iy1][iz];
   i1=interpola(n1, n2, frx);
   i2=interpola(n3, n4, frx);
   i3=interpola(i1, i2, fry);

   n1=knots3[ix][iy][iz1];
   n2=knots3[ix1][iy][iz1];
   n3=knots3[ix][iy1][iz1];
   n4=knots3[ix1][iy1][iz1];
   i1=interpola(n1, n2, frx);
   i2=interpola(n3, n4, frx);
   i4=interpola(i1, i2, fry);

   return(interpola(i3, i4, frz));
}


float noise3D::Perlin(float x, float y, float z, float persistence, int octaves)
{
   float total, frequency, amplitude;
   int i;

   total=0;
   frequency=1;
   amplitude=1;
   for (i=0; i<octaves; i++)
   {
      total = total + interpolated_noise3(x*frequency, y*frequency, z*frequency)*amplitude;
	  frequency*=2;
	  amplitude*=persistence;
   }
   return total;
}


void noise3D::do_efx2D(double pos)
{
  int x, y, ip, pixel, ofs;
  // int ip2, s;
  float p;
  float step, step_x, step_y, fx, fy;

  step=(float)(pos*2.0);
  step_x=(float)(1.0/80.0);
  step_y=(float)(1.0/60.0);

  ofs=0;
  fy=0;
  for (y=0; y<image->height; y++)
  {
	fx=step;
    for (x=0; x<image->width; x++)
	{
	   p=Perlin(fx, fy, step, 0.45f, 6);
//	   s=(int)(128.0*sinf(6.28f*x/image->width))+128;
	   ip=myfist(p+SHINESS);
	   if (ip<0) ip=0;
	   if (ip>255) ip=255;
//       s=s+ip;
//	   if (s>255) s=255;
	   pixel=palette[ip];
//	   image->uint32ptr[ofs++]=pixel;
	   image->uint32ptr[ofs++]=ip | (ip<<8) | (ip<<16);
	   fx+=step_x;
	}
    fy+=step_y;
  }
}