//  _____________________________________________________ _ _ _ 
// |_Qwav_2d_Effect______________________________________|-|@|x|
// | ~~~~ ~~ ~~~~~~                                       ~ ~|~| 
// |                                                         |_|
// | Main code : Ska                                         |#|
// | Version   : 0.01                                        |#|
// | Date of creation : 15/12/1998                           |#|
// | Last modify      : 20/10/1999                           |~|
// |_________________________________________________________|_|

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "..\ADrender.h"
#include "..\utility.h"
#include "qwave2D.h"


extern HWND miawin;  // finestra principale; usata qui per le debug_error

#define Pi__ 3.141592
#define sqr(a) ((a)*(a))

void qwave2D::init()
{ //----- Alloc memory for buffers ------------------(x)-;
  double g; 
  double max,max1,max2;
  int i;

  tmptab    =(double*) malloc(1024*sizeof(double));
  pls1      =(int *) malloc(1024*sizeof(int));
  pls2      =(int *) malloc(1024*sizeof(int));
  kpls1     =(int *) malloc(1024*sizeof(int));
  kpls2     =(int *) malloc(1024*sizeof(int));
  
  xback=(unsigned char *) malloc(4*image->channelsize*sizeof(unsigned char));
  
  mod_y = new int [2400]; 
  for (i=0; i<2400; i++)
  {
    mod_y[i] = i%image->height;
  }

   
  max=0;
  for (i=0; i<=1024; i++)
  {
	g=(sin(Pi__*i/256)*(1-4*cos(Pi__*i/256)));
    tmptab[i]=g;
    if (g<0) g=(0-g);
    if (max<g) max=g;
  }
  max1=(128/max);
  max2=(64/max); 
  for (i=0; i<=1024; i++) 
  {
	 kpls1[i]=(int)(128+max1*tmptab[i]);
     pls1[i]=(int)(64+max2*tmptab[i]);
  }

  max=0;
  
  for (i=0;i<=1024;i++)
  {
	g=(sin(Pi__*i/256)*(-1+2*cos(2*Pi__*i/256)));
    tmptab[i]=g;
    if (g<0) g=(0-g);
    if (max<g) max=g;
  }  
  max1=(128/max);
  max2=(16/max); 
  for (i=0;i<=1024;i++) 
  { 
	kpls2[i]=(int)(128+max1*tmptab[i]);
    pls2[i]=(int)(16+max2*tmptab[i]);
  } 

  k1=0; 
  k2=0;   
} 

void qwave2D::set_background(unsigned char *_back)
{ 
   back=_back;
} 	


void qwave2D::set_background(char *nomefile)
{
   if (mytex.load_jpg(nomefile, RGBINTERLEAVED)==FILENOTFOUND)
	  debug_error(miawin, "qwave2D error: texture not found");

  back=mytex.map;
}

void qwave2D::set_background(imagebuffer *img)
{
   back=img->uchar8ptr; 
}

void qwave2D::do_efx2D(double pos)
{ 
  unsigned char *img;
  int s1, s2;
  unsigned int d1, d2, *xback32=(unsigned int *)xback;
  unsigned int *back32=(unsigned int *)back;
  int t1, t2;
  int buf_ofs, y_ofs, aux;
  int x, y;

  buf_ofs=0;
  y_ofs=0;
  
  s1=kpls1[(k1)&1023];
  s2=kpls2[(k2)&1023]; 
  k1+=2;  
  k2-=1;  
  
  img=image->uchar8ptr;

  for (y=0; y<image->height ; y++) 
  {  t1=(s2+kpls1[(y+k2)&1023]);
	 t2=(s1+kpls2[(y+k1)&1023]);   

	 
    
    for (x=0; x<image->width ; x++) 
    {  
	  
       d1=(unsigned int)pls1[(t1+x)&1023];
       d1=d1+y_ofs+x;
	   if (d1 >= image->channelsize) { d1=0; }

       d2=(unsigned int)pls2[(t2+x)&1023];
       aux=mod_y[y+d2];
       d2=x+aux*(image->width);

	   xback32[buf_ofs]=back32[d1];
/*
       (*img) = (7*(*img)+(xback[4*d2  ]))>>3;
       img++;
       (*img) = (3*(*img)+(xback[4*d2+1]))>>2;
       img++;
	   (*img) = (1*(*img)+(xback[4*d2+2]))>>1;
	   img+=2;
*/	        
       (*img) = xback[4*d2  ];
       img++;
       (*img) = xback[4*d2  +1];
       img++;
       (*img) = xback[4*d2  +2];
       img+=2;
       buf_ofs++;
	} 
    y_ofs+=image->width;
  }
}
