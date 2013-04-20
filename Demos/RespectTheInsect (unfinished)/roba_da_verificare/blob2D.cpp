#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "..\ADrender.h"
#include "blob2D.h"

#define sqr(a) ((a)*(a))

void blob2D::init()
{
    texture imgloader;
    int x, y, deltax, deltay;
    int i;

    imgloader.load_jpg("oldscool.jpg", RGBINTERLEAVED);
    texuint=imgloader.map32;

    deform_tab = new unsigned char[image->width*image->height];

    for (i=0; i<nblob; i++)
    {
     blob[i].x = (image->width>>1);
     blob[i].y = (image->height>>1);
	 blob[i].pow = (int) (image->width*0.5)+rand()%(int)(image->width*1.2);
     blob[i].va = (float) ( ( (rand()%450)/20) );
     blob[i].raggio = 10+rand()%(image->width>>3);
	 blob[i].xabs = new int [image->width];
	 blob[i].yabs = new int [image->width];
    }

    // PRECALCOLO DELLE DISTANZE
    distance_tab = (int *) malloc (image->width*image->height*sizeof(int)) ;
    for (y=0; y<image->height; y++)
    {
      for (x=0; x<image->width; x++)
      {
         deltax = abs(x);
         deltay = abs(y);
         distance_tab[x+image->muly[y]] = ((int) ( (image->width*12.5) / (sqrt(sqr(deltax)+sqr(deltay))+0.05)/sqrt(nblob/2)));
      }
    }
}


void blob2D::agg_cord(double pos)
{
   int i;

   for (i=0; i<nblob; i++)
   {
       blob[i].bx = blob[i].x+(int) ((blob[i].raggio*(pos/3)*blob[i].raggio)*cos(pos*blob[i].va));
       blob[i].by = blob[i].y+(int) ((blob[i].raggio*2)*sin(pos*blob[i].va));
	   blob[i].bx=blob[i].bx % 256;
	   blob[i].by=blob[i].by % 256;
       if (blob[i].bx > image->width) blob[i].bx = image->width;
       if (blob[i].bx < 0) blob[i].bx = 0;
       if (blob[i].by > image->height) blob[i].by = image->height;
       if (blob[i].by < 0) blob[i].by = 0;

     }
}


void blob2D::do_efx2D(double pos)
{
   int x, y, i, j, offset;
   int deltay[nblob];
   unsigned int dist;
   unsigned char tx=0, *texch;
   int ty=0;

   agg_cord(pos);
   offset=0;
   texch=(unsigned char *)texuint;

   for (i=0;i<nblob;i++) 
   {
	  for (j=0;j<image->width;j++)
	  blob[i].xabs[j] = abs(j-blob[i].bx);
   }

   for (y=0; y<image->height; y++)
   {
     for (i=0; i<nblob; i++) 
        deltay[i] = image->muly[abs(y-blob[i].by)];

     for (x=0; x<image->width; x++)
     {
       dist = 0;
       for (i=0; i<nblob; i++)
         dist += distance_tab[blob[i].xabs[x]+deltay[i]];
      
	   dist &=0x0000FFFF;
	   if (dist<240)
//	  image->uint32ptr[offset] = texuint[ty+dist];
	   {
	     image->uchar8ptr[offset*4] =   (3*image->uchar8ptr[offset*4  ] + texch[(ty+dist)*4  ])>>2; 
		 image->uchar8ptr[offset*4+1] = (1*image->uchar8ptr[offset*4+1] + texch[(ty+dist)*4+1])>>1;
	     image->uchar8ptr[offset*4+2] = (0*image->uchar8ptr[offset*4+2] + texch[(ty+dist)*4+2]);
	   }
	   else image->uint32ptr[offset] = 0;
       offset++;
	 }
     ty+=256;
	 ty = ty%(65536);
   }
   ty=0;
}