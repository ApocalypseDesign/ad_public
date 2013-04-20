#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#include "hole2D.h"
#include "..\utility.h"

#define Pi__ 3.141592
#define sqr(a) ((a)*(a))

extern HWND miawin;  // finestra principale; usata qui per le debug_error

hole2D::hole2D()   // costruttore
{
  textdata=textcoord=(unsigned char *)NULL; 
  k1=k2=k3=k4=1;
  r_blur=g_blur=b_blur=0;
  rm=gm=bm=0;
  blittype=NOBLUR;
}


void hole2D::set_blur(int r, int g, int b)
{ 
  int i;

  rm=gm=bm=1;
  r_blur=r;
  g_blur=g;
  b_blur=b;
  if (r!=0)
  {	for (i=1;i<=(r_blur);i++) {rm*=2;}
    rm-=1;
  }
  else rm=0;
  if (g!=0)
  { for (i=1;i<=(g_blur);i++) {gm*=2;}
    gm-=1;
  }
  else gm=0;
  if (b!=0)
  { for (i=1;i<=(b_blur);i++) {bm*=2;}
    bm-=1;
  }
  else bm=0;
 
  if  ((rm==0)&&(gm==00)&&(bm==0)) blittype=NOBLUR;
  else blittype=BLUR;
}


void hole2D::init()
{ //----- Alloc memory for buffers ------------------(x)-;
  int i, j, x1, y1, offs;
  float dx, dy, dz, d;
  float x, y, z, ang;
  unsigned char u, v;
	
  textcoord =(unsigned char *) malloc (2*image->channelsize*sizeof(unsigned char));
  
  memset(textcoord,0,2*image->channelsize);
  
  x1 = image->width;
  y1 = image->height;
  k3=0;
  k4=0;
  offs=0;
     
  // precalc the (u,v) coordinates
    for (j=-(y1/2); j<(y1/2); j++) 
	{
     for (i=-(x1/2); i<(x1/2); i++)
     { 
	   dx = ((float)i ) / (x1);
       
	   dy = ((float)-j) / (y1);
	   
       dz = 1;
       // normalize 
       d =(float)( 20/sqrt( dx*dx + dy*dy + 1 ) );
       dx *= d;
       dy *= d;
       dz *= d;
       // interpolation 
       x = 0;
       y = 0;
       z = 0;
       // precision	(16)
       d = 16;
       // interpolate along ray
       while (d>0)
       {
        // continue until we hit a wall
        while (( (sqr(x-x_centre(z))+sqr(y-y_centre(z))) < radius(z)) && (z<1024))
        {
          x += dx;
          y += dy;
          z += dz;
        }
       // reduce precision and reverse direction
       x -= dx;  dx /= 2;
       y -= dy;  dy /= 2;
       z -= dz;  dz /= 2;
       d -= 1;
       }
      // calculate the texture coordinates
	  x -= x_centre(z);
      y -= y_centre(z);
      ang = (float)(atan2(y,x) * x1 / Pi__ );
      u = ((unsigned char)ang);
      v = ((unsigned char)z);

      // store texture coordinates
      textcoord[offs  ] = u;
      textcoord[offs+1] = v;
      offs += 2;
      }
    }
} 

void hole2D::set_uvcontrol(int _k1, int _k2)
{ 
  k1=_k1;
  k2=_k2;
}

void hole2D::set_texture(unsigned char *_back)
{ 
   textdata=_back;
} 


void hole2D::set_texture(char *nomefile)
{
//  if (tex.load_pcx(nomefile, RGBINTERLEAVED)==FILENOTFOUND)
    if (mytex.load_jpg(nomefile, RGBINTERLEAVED)==FILENOTFOUND)
	  debug_error(miawin, "warmhole error: texture not found");

  textdata=mytex.map;
}


float hole2D::x_centre(float f )
{
  return (float) (-24 * sin(  f * Pi__ / 512 ));
  //return (float) (16*sin(Pi__*f/512)*(1-4*cos(Pi__*f/512)));
  //return (float) f/4;
}

float hole2D::y_centre(float f )
{
  //return (float) (64 * sin(  f * Pi__ / 512 ));
  return (float) (-8*cos(Pi__*f/512)*(1-4*sin(Pi__*f/256)));
  //return (float) f/2;
}


float hole2D::radius(float f )
{
  return (float) (128+ 64*cos(f * Pi__ / 512));
}


void hole2D::plot(int du, int dv)
{   
	unsigned char *img=image->uchar8ptr;
//	unsigned int *img32=image->uint32ptr;
	unsigned int tex_offset;
	int vshift=mytex.xbits;
	int cont=image->channelsize;
	int _umask_=mytex.dimx-1;
	int _vmask_=mytex.dimy-1;
    unsigned char *textc=textcoord;
	unsigned int *text_data_32=(unsigned int *)textdata;
	int k=0, kk=0;
//    unsigned char u, v;

// usate per il blur mmx
  __int64 mul_blur=(__int64)(bm) | ((__int64)(gm)<<16) 
	                | ((__int64)(rm)<<32);
  int max_mul=max(max(b_blur, g_blur), r_blur);
  int bmul, rmul, gmul;
  __int64 mulsh;
  __int64 shift=(__int64)(max_mul);

  bmul=1 << (max_mul-b_blur);
  gmul=1 << (max_mul-g_blur);
  rmul=1 << (max_mul-r_blur);
  mulsh=(__int64)(bmul) | ((__int64)(gmul)<<16) |
	    ((__int64)(rmul)<<32);


  if (blittype==NOBLUR)
  {
    _asm
    {
	  mov esi, [textc]
	  mov edx, [img]
	  mov edi, [text_data_32]

   ciclo:
	  mov ax, [esi]   // leggo v-u
	  mov bx, [esi]

	  and eax, 0xff    // estraggo u
	  and ebx, 0xff00  // estraggo v

	  shr ebx, 8
	  add eax, [du]

	  and eax, [_umask_]
	  add ebx, [dv]

      and ebx, [_vmask_]
	  mov ecx, [vshift]

      shl ebx, cl
	  add eax, ebx     // eax = tex_offset

	  mov ebx, [edi+eax*4]  // leggo un pixel della texture
	  mov [edx], ebx

	  mov ecx, [cont]
	  add esi, 2
	  add edx, 4
	  dec ecx
	  mov [cont], ecx
	  jnz ciclo
	  emms
	}
  }
  else
  {
    _asm
    {
	  mov esi, [textc]
	  mov edx, [img]
	  pxor mm7, mm7

   ciclo2:
	  mov ax, [esi]   // leggo v-u
	  mov bx, [esi]

	  and eax, 0xff    // estraggo u
	  and ebx, 0xff00  // estraggo v

	  shr ebx, 8
	  add eax, [du]

	  and eax, [_umask_]
	  add ebx, [dv]

      and ebx, [_vmask_]
	  mov ecx, [vshift]

      shl ebx, cl
	  mov edi, [text_data_32]

	  movd mm0, [edx]  // leggo un pixel di sfondo
	  add eax, ebx     // eax = tex_offset

      punpcklbw mm0, mm7     // pixel di sfondo in word16
	  pmullw mm0, [mul_blur]

	  movd mm1, [edi+eax*4]  // leggo un pixel della texture
      punpcklbw mm1, mm7     // in word16

	  paddw mm0, mm1
      pmullw mm0, [mulsh]

	  psrlw mm0, [shift]
      packuswb mm0, mm7

	  movd [edx], mm0

	  mov ecx, [cont]
	  add esi, 2
	  add edx, 4
	  dec ecx
	  mov [cont], ecx
	  jnz ciclo2
	  emms
	}
  }


/*
	for (k=0; k<(image->channelsize); k++)
	{
     // load (u,v) and add displacement
     u = textcoord[kk] + du; u = u & _umask_;
     v = textcoord[kk+1] + dv; v = v & _vmask_;
	 kk+=2;
	 tex_offset=(v<<vshift)+u;

     // versione senza blur C
	 // (*img32) = (text_data_32[((v<<vshift)+u)]);
     // img32++;

     // versione C col blur
	  (*img) = (rm*(*img) + textdata[((v<<vshift)+u)*4   ])>>r_blur;
      img++;
      (*img) = (gm*(*img) + textdata[((v<<vshift)+u)*4 +1])>>g_blur;
      img++;	   
      (*img) = (bm*(*img) + textdata[((v<<vshift)+u)*4 +2])>>b_blur;
 	  img+=2;
    }
*/
}



void hole2D::do_efx2D(double pos)
{  
  k3+=k1;
  k4+=k2;
  plot(k3,k4);
}
