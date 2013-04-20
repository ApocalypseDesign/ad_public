// classe di base per i buffer v1.1

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "image.h"
#include "ADbasic.h"

image::image(void) 
{ size=width=height=0; }

image::image (int sizex, int sizey)
{
  int j;

  width=sizex;
  height=sizey;
  size=(width*height)*4;
  voidptr=(void *) malloc(size);
  uchar8ptr=(unsigned char *) voidptr;
  uint32ptr=(unsigned int *) voidptr;
  
  muly=(int *)malloc(height*sizeof(int));
  for (j=0; j<height; j++) muly[j]=j*width;
}

image::image (int sizex, int sizey, void *buffer)
{
  int j;

  width=sizex;
  height=sizey;
  size=(width*height)*4;
  voidptr=buffer;
  uchar8ptr=(unsigned char *) voidptr;
  uint32ptr=(unsigned int *) voidptr;
  
  muly=(int *)malloc(height*sizeof(int));
  for (j=0; j<height; j++) muly[j]=j*width;
}

void image::clear()
{
  memsetd(voidptr,0,size>>2); 
}

void image::clear(unsigned char value)
{
  memsetd(voidptr,int(value)+(int(value)<<8)+(int(value)<<16),size>>2); 
}

void image::clear(unsigned char r, unsigned char g, unsigned char b)
{
  memsetd(voidptr,int(b)+(int(g)<<8)+(int(r)<<16),size>>2); 
}

