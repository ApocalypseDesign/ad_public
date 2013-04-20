//  _____________________________________________________ _ _ _ 
// |_Plas_2d_Effect______________________________________|-|@|x|
// | ~~~~ ~~ ~~~~~~                                       ~ ~|~| 
// |                                                         |_|
// | Main code : Ska                                         |#|
// | Version   : 0.01                                        |#|
// | Date of creation : 15/12/1998                           |#|
// | Last modify      : 15/12/1998                           |~|
// |_________________________________________________________|_|

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "plasm2D.h"

#define Pi__ 3.141592
#define sqr(a) ((a)*(a))

/*
void plasm2D::init()
{ //----- Alloc memory for buffers ------------------(x)-;
  int y2,x2,y4,x4,i;
  double g;
  double max;
  double *tmptab;

  tmptab    =new double[512];
  pls1      =new int[512];
  pls2      =new int[512];
  kpls1     =new int[512];
  kpls2     =new int[512];

  y2=(image->height/2);
  y4=(image->height/4);
  x2=(image->width/2);
  x4=(image->width/4);
  
  // calcolo come si vuole (cos, tcb ecc)
  max=-1;
  for (i=0; i<512; i++)
  {
    tmptab[i]=((sin(Pi__*i/256)*(1-4*cos(Pi__*i/256))));
	if (fabs(tmptab[i])>max) max=fabs(tmptab[i]);
  }

  // resize negli intervalli stabiliti validi [0;255] per
  // pls1, [0;511] per kpls1
  g=256/max;
  for (i=0; i<512; i++)
  { 
	kpls1[i]=(int)(256+g*tmptab[i]);
    pls1[i]=(int)(128+tmptab[i]*g*0.5);
  }

  
  // calcolo come si vuole (cos, tcb ecc)
  max=-1;
  for (i=0; i<512; i++)
  {
	tmptab[i]=((sin(Pi__*i/256)*(1-2*sin(2*Pi__*i/256))));
	if (fabs(tmptab[i])>max) max=fabs(tmptab[i]);
  }
  // resize negli intervalli stabiliti validi [0;255] per
  // pls1, [0;511] per kpls1
  g=256/max;
  for (i=0; i<512; i++)
  { 
	kpls2[i]=(int)(256+g*tmptab[i]);
    pls2[i]=(int)(128+tmptab[i]*g*0.5);
  }

  k1=0;
  k2=0;
  delete []tmptab;
} 
*/


void plasm2D::init()
{
  #define NKEY 64

  int i, j;
  double g1, g2, g;
  double *tmptab, *tmptab_d;
  FILE *f;
  double h1, h2, h3, h4, t, t2, t3;

  tmptab    =new double[512/NKEY+1];
  tmptab_d  =new double[512/NKEY+1];
  pls1      =new int[512];
  pls2      =new int[512];
  kpls1     =new int[512];
  kpls2     =new int[512];

  f=fopen("plasma.txt", "wt");


  for (i=0; i<512/NKEY+1; i++) 
//    tmptab[i]=256+64*((sin(Pi__*i*NKEY/256)*(1-4*cos(Pi__*i*NKEY/256))));
	  tmptab[i]=(double)(rand()%512);
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
	   pls1[i*NKEY+j]=(int)(g/2.0);
	   fprintf(f, "i: %d,  pls1:%d,  kpls1: %d\n", j, pls1[i*NKEY+j], kpls1[i*NKEY+j]);
	 }
  }



  for (i=0; i<512/NKEY+1; i++) 
//	tmptab[i]=256+64*((sin(Pi__*i*NKEY/256)*(1-2*sin(2*Pi__*i*NKEY/256))));
	  tmptab[i]=(double)(rand()%512);

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
	   pls2[i*NKEY+j]=(int)(g/2.0);
	 }
  }



  k1=0;
  k2=0;
  fclose(f);
  delete []tmptab;
} 


void plasm2D::do_efx2D(double pos)
{ unsigned char *img;
  int y2,x2,y4,x4; 
  int s1, s2;
  int k3,k4,k5,k6;
  int d1, d2;
  int x, y;

 y2=(image->height/2);
 y4=(image->height/4);
 x2=(image->width/2);
 x4=(image->width/4);

 s1=kpls1[k2 & 511];
 s2=kpls1[k1 & 511];
 
 k1+=2; 
 k2-=1;  
 
 img=image->uchar8ptr;

 for (y=0; y<image->height; y++) 
 { k3 = (s1+y  -y2)>>1 ;
   k5 = (k1+y/2-y4)<<1;
  for (x=0; x<image->width; x++) 
  { k4 = (s2+x  -x2)>>1;
    k6 = (k2+x/2-x4)<<1;
	d1=(s1+kpls1[(( kpls1[(16+k2+k3-k6)&511] + kpls2[(   k4-k5)&511]))&511])>>2;
    d2=(s2+kpls2[(( kpls1[(   k1+k4-k5)&511] + kpls2[(32+k3+k6)&511]))&511])>>2;
    								  
    d2+=d1;   
     // d2&=255; 
	 //----- Blit with blur ----------(x)-;
/*
   	 (*img) = (7*(*img) + (pls1[(d2  ) & 511]))>>3 ;
	 img++;									 
     (*img) = (3*(*img) + (pls1[(d2+4) & 511]))>>2 ;     
     img++;
	 (*img) = (1*(*img) + (pls1[(d2+8) & 511]))>>1 ;	 
	 img+=2;
*/

   	 (*img) = (pls1[(d2  ) & 511]);
	 img++;									 
     (*img) = (pls1[(d2+8) & 511]);     
     img++;
	 (*img) = (pls1[(d2+16) & 511]);
	 img+=2;
  }        
 }
}
