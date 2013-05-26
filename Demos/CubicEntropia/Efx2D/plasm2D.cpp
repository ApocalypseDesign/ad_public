#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "plasm2D.h"
#include "..\utility.h"
#include "noise.h"

extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

plasm2D::plasm2D()
{
   kpls1=kpls2=(int *)NULL;
   pls1=pls2=(int *)NULL;
   k1=k2=0;
   rblur=gblur=bblur=0;
   rblur_shift=gblur_shift=bblur_shift=0;
}


void plasm2D::set_blur (unsigned int rb, unsigned int gb, unsigned int bb)
{
  if (rb>0)
  {
     rblur=(1 << rb)-1;
	 rblur_shift=rb;
  }

  if (gb>0)
  {
     gblur=(1 << gb)-1;
	 gblur_shift=gb;
  }

  if (bb>0)
  {
     bblur=(1 << bb)-1;
	 bblur_shift=bb;
  }
}


void plasm2D::init_color_table(void)
{
  #define NKEY 64
  double tmptab[512/NKEY+1], tmptab_d[512/NKEY+1],
	     g, g1, g2;
//  int Rtab[512/NKEY+1], Gtab[512/NKEY+1], Btab[512/NKEY+1];
  double h1, h2, h3, h4, t, t2, t3;
  int i, j, ig;

  pls1=new int[512];
  pls2=new int[512];

  for (i=0; i<512/NKEY+1; i++)
	  tmptab[i]=(double)((noise1_positive(i)*255+
	                      noise1_positive(i*3)*255+
						  noise1_positive(i*5)*255)/3.0);
  tmptab[512/NKEY]=tmptab[0];

  for (i=0; i<512/NKEY+1; i++)
  {
    if (i==0) tmptab_d[i]=0.5*(tmptab[i+2]-tmptab[i]);
	else
    if (i==512/NKEY) tmptab_d[i]=0.5*(tmptab[i]-tmptab[i-2]);
	else 
	tmptab_d[i]=0.5*(tmptab[i+1]-tmptab[i-1]);
  }
  tmptab_d[512/NKEY]=tmptab_d[0];

  for (i=0; i<512/NKEY; i++)
  {
     g1=tmptab[i];
     g2=tmptab[i+1];
	 for (j=0; j<NKEY; j++)
	 {
	   t=(double)(1.0*j/(NKEY));
       t2=t*t;
       t3=t2*t;
       h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
       h2=-2.0f*t3 + 3.0f*t2;
       h3=t3 - 2.0f*t2 + t;
       h4=t3 - t2;
	   g=(h1*g1 + h2*g2 +
          h3*tmptab_d[i] + h4*tmptab_d[i+1]);
	   if (g<0) g=0;
	   if (g>255) g=255;
	   ig=(int)(g);
	   
	   pls1[i*NKEY+j]=ig;
	 }
  }
}


void plasm2D::init_look_table(void)
{
  #define NKEY 64
  double tmptab[512/NKEY+1], tmptab_d[512/NKEY+1],
	     g, g1, g2;
  double h1, h2, h3, h4, t, t2, t3;
  int i, j;

  kpls1=new int[512];
  kpls2=new int[512];

  for (i=0; i<512/NKEY+1; i++)
	  tmptab[i]=(double)((noise1_positive(i)*512+
	                      noise1_positive(i*3)*512+
						  noise1_positive(i*7)*512)/3.0);
  tmptab[512/NKEY]=tmptab[0];

  for (i=0; i<512/NKEY+1; i++)
  {
    if (i==0) tmptab_d[i]=0.5*(tmptab[i+2]-tmptab[i]);
	else
    if (i==512/NKEY) tmptab_d[i]=0.5*(tmptab[i]-tmptab[i-2]);
	else 
	tmptab_d[i]=0.5*(tmptab[i+1]-tmptab[i-1]);
  }
  tmptab_d[512/NKEY]=tmptab_d[0];

  for (i=0; i<512/NKEY; i++)
  {
     g1=tmptab[i];
     g2=tmptab[i+1];
	 for (j=0; j<NKEY; j++)
	 {
	   t=(double)(1.0*j/(NKEY));
       t2=t*t;
       t3=t2*t;
       h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
       h2=-2.0f*t3 + 3.0f*t2;
       h3=t3 - 2.0f*t2 + t;
       h4=t3 - t2;
	   g=(h1*g1 + h2*g2 +
          h3*tmptab_d[i] + h4*tmptab_d[i+1]);
	   
	   kpls1[i*NKEY+j]=(int)(g);
	 }
  }

  for (i=0; i<512/NKEY+1; i++)
	  tmptab[i]=(double)((noise1_positive(i)*512+
	                      noise1_positive(i*9)*512+
						  noise1_positive(i*7)*512)/3.0);
  tmptab[512/NKEY]=tmptab[0];

  for (i=0; i<512/NKEY+1; i++)
  {
    if (i==0) tmptab_d[i]=0.5*(tmptab[i+2]-tmptab[i]);
	else
    if (i==512/NKEY) tmptab_d[i]=0.5*(tmptab[i]-tmptab[i-2]);
	else 
	tmptab_d[i]=0.5*(tmptab[i+1]-tmptab[i-1]);
  }
  tmptab_d[512/NKEY]=tmptab_d[0];

  for (i=0; i<512/NKEY; i++)
  {
     g1=tmptab[i];
     g2=tmptab[i+1];
	 for (j=0; j<NKEY; j++)
	 {
	   t=(double)(1.0*j/(NKEY));
       t2=t*t;
       t3=t2*t;
       h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
       h2=-2.0f*t3 + 3.0f*t2;
       h3=t3 - 2.0f*t2 + t;
       h4=t3 - t2;
	   g=(h1*g1 + h2*g2 +
          h3*tmptab_d[i] + h4*tmptab_d[i+1]);
	   
	   kpls2[i*NKEY+j]=(int)(g);
	 }
  }
}


void plasm2D::init()
{
  init_look_table();
  init_color_table();
  k1=0;
  k2=0;
  frazpos=0;
  oldpos=0;
} 


void plasm2D::draw_blur(double pos)
{
  unsigned char *img;
  int y2, x2, y4, x4; 
  int s1, s2;
  int k3, k4, k5, k6;
  int d1, d2;
  int x, y;

  y2=(image->height/2);
  y4=(image->height/4);
  x2=(image->width/2);
  x4=(image->width/4);
  s1=kpls1[k2 & 511];
  s2=kpls1[k1 & 511];
  img=image->uchar8ptr;

  for (y=0; y<image->height; y++) 
  { 
     k3 = (s1+y  -y2)>>1;
     k5 = (k1+y/2-y4)<<1;
     for (x=0; x<image->width; x++) 
	 {
	    k4 = (s2+x  -x2)>>1;
        k6 = (k2+x/2-x4)<<1;
	    d1=(s1+kpls1[(( kpls1[(16+k2+k3-k6)&511] + kpls2[(   k4-k5)&511]))&511])>>2;
        d2=(s2+kpls2[(( kpls1[(   k1+k4-k5+d1)&511] + kpls2[(32+k3+k6)&511]))&511])>>2;
        // d2=(s2+kpls2[(( kpls1[(   k1+k4-k5)&511] + kpls2[(32+k3+k6)&511]))&511])>>2;
    								  
        d2+=d1;
   	    (*img) = (rblur*(*img) + (pls1[(d2*4  ) & 511]))>>rblur_shift;
	    img++;									 
        (*img) = (gblur*(*img) + (pls1[(d2*4+8) & 511]))>>gblur_shift;
        img++;
	    (*img) = (bblur*(*img) + (pls1[(d2+16) & 511]))>>bblur_shift;
	    img+=2;
	 }     
  }
}


void plasm2D::draw_noblur(double pos)
{
  unsigned char *img;
  int y2, x2, y4, x4; 
  int s1, s2;
  int k3, k4, k5, k6;
  int d1, d2;
  int x, y;

  y2=(image->height/2);
  y4=(image->height/4);
  x2=(image->width/2);
  x4=(image->width/4);
  s1=kpls1[k2 & 511];
  s2=kpls1[k1 & 511];
  img=image->uchar8ptr;

  for (y=0; y<image->height; y++) 
  { 
     k3 = (s1+y  -y2)>>1 ;
     k5 = (k1+y/2-y4)<<1;
     for (x=0; x<image->width; x++) 
	 {
	    k4 = (s2+x  -x2)>>1;
        k6 = (k2+x/2-x4)<<1;
	    d1=(s1+kpls1[(( kpls1[(16+k2+k3-k6)&511] + kpls2[(   k4-k5)&511]))&511])>>2;
        d2=(s2+kpls2[(( kpls1[(   k1+k4-k5+d1)&511] + kpls2[(32+k3+k6)&511]))&511])>>2;
        // d2=(s2+kpls2[(( kpls1[(   k1+k4-k5)&511] + kpls2[(32+k3+k6)&511]))&511])>>2;
    								  
        d2+=d1;
   	    (*img) = (pls1[(d2  ) & 511]);
	    img++;									 
        (*img) = (pls1[(d2+8) & 511]);     
        img++;
	    (*img) = (pls1[(d2+16) & 511]);
	    img+=2;
	 }     
  }
}


void plasm2D::do_efx2D(double pos)
{ 
  double relpos;
  #define PVEL 1900.0

  // Sync perfetto made by Turbo
  relpos=(pos-fstartpos)/(fendpos-fstartpos);
  if ((pos-oldpos) > 1.0/PVEL)
  {
    k1=k1+(int)(2.0*(pos-oldpos)*PVEL);
    k2=k2-(int)((pos-oldpos)*PVEL);
	oldpos=relpos;
	frazpos=0;
  }
  else
  {
	 frazpos=frazpos+(pos-oldpos);
	 if (frazpos > 1.0/PVEL)
	 {
        k1=k1+(int)(2.0*frazpos*PVEL);
        k2=k2-(int)(frazpos*PVEL);
	    oldpos=relpos;
	    frazpos=0;
	 }
  }


  if ((rblur+gblur+bblur) > 0) draw_blur(pos);
  else draw_noblur(pos);
}