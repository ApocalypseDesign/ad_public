// classe di base per i buffer v1.1

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "ADrender.h"
#include "ADbase.h"

imagebuffer::imagebuffer (int sizex, int sizey)
{
  int j;

  width=sizex;
  real_height=height=sizey;
  real_channelsize=channelsize=width*height;
  real_where=uchar8ptr=new unsigned char[channelsize*4];
  uint32ptr=(unsigned int *)real_where;
  uchar8ptr=(unsigned char *)real_where;
  voidptr=(void *)real_where;
  
  muly=new int[height];
  for (j=0; j<height; j++) muly[j]=j*width;
}


void imagebuffer::set_cinemascope(int top, int bottom)
// per ora non va
{
  uchar8ptr=(unsigned char *)(((int)real_where)+top*width*4);
  height=real_height-top-bottom;
  channelsize=width*height;
}


void imagebuffer::clear (unsigned char value)
{
  memsetd_lib(voidptr, int(value)+(int(value)<<8)+(int(value)<<16), channelsize);
}


void imagebuffer::clear (unsigned char r, unsigned char g, unsigned char b)
{
  memsetd_lib(voidptr, int(b)+(int(g)<<8)+(int(r)<<16), channelsize);
}


floatbuffer::floatbuffer (int sizex, int sizey)
{
  int j_2;

  width=sizex;
  height=sizey;
  size=width*height;
  where=(float *)malloc(size*sizeof(float));
  
  muly=(int *)malloc(height*sizeof(int));
  for (j_2=0; j_2<height; j_2++) muly[j_2]=j_2*width;
}


void floatbuffer::clear (float value)
{
//  int j_1;
  int *g=(int *)(&value);

//  for (j_1=0; j_1<size; j_1++) where[j_1]=value;
  memsetd_lib((void *)where, *g, size); 
}