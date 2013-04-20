#include <stdlib.h>
#include <math.h>
#include "bump2D.h"
#include "..\utility.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error

bump2D::bump2D()    // costruttore
{
}


void bump2D::set_height_map(char *file)
{
  if (height_map.load(file, RGBINTERLEAVED)==FILENOTFOUND)
    debug_error(miawin, "bump2D: heightmap file not found!");
}


void bump2D::set_height_map(imagebuffer *ib)
{
   height_map.dimx=ib->width;
   height_map.dimy=ib->height;
   height_map.map=ib->uchar8ptr;
   height_map.map32=ib->uint32ptr;
//   height_map.xbits=height_map.dim_to_bits(height_map.dimx);
//   height_map.ybits=height_map.dim_to_bits(height_map.dimy);
   height_map.channelsize=ib->width*ib->height;
   height_map.channelsize2x=height_map.channelsize*2;
   height_map.img=*ib;
}


void bump2D::set_height_map(texture *t)
{
   height_map=*t;
}


void bump2D::set_light_map(char *file)
{
  if (light_map.load(file, RGBINTERLEAVED)==FILENOTFOUND)
    debug_error(miawin, "bump2D: lightmap file not found!");
}


void bump2D::init()
{
  if ((image->width!=height_map.dimx) ||
      (image->height!=height_map.dimy))
  { debug_error(miawin, "bump error: dimensioni diverse di heightmap e imagebuffer"); }
}

/*
void bump2D::plot(int lightX, int lightY)
{
  int x, y;
  unsigned char *hmap=height_map.map;
  unsigned char *lmap=light_map.map;
  int *muly=image->muly;
  int o1, o2, ov, ol;
  int oy, oy_1, oy1;
  int nx_b, ny_b;
  unsigned int p1, p2;

  for (y=1; y<image->height-1; y++) 
  { 
	oy_1=muly[y-1] << 2;
	oy=muly[y] << 2;
	oy1=muly[y+1] << 2;
	ov=muly[y]+1;
    for (x=1; x<image->width-1; x++) 
	{  
      o1=oy+4;
	  o2=oy-4;
	  // calcolo delle normali
	  nx_b=((hmap[o1+1] - hmap[o2+1])+
		    (hmap[o1+2] - hmap[o2+2])) << 1;
      ny_b=((hmap[oy1+1] - hmap[oy_1+1])+
           (hmap[oy1+2] - hmap[oy_1+2])) << 1;

//	  nx_b=(hmap[o1+1] - hmap[o2+1]) << 1;
//    ny_b=(hmap[oy1+2] - hmap[oy_1+2]) << 1;
	  oy+=4;
	  oy_1+=4;
	  oy1+=4;

//      nx_b=nx_b - (x - lightX) + (light_map.dimx / 2);
//      ny_b=ny_b - (y - lightY) + (light_map.dimy / 2);

      nx_b=nx_b + x;
      ny_b=ny_b + y;

	  if (nx_b < 0) nx_b=0;
	  else if (nx_b >= light_map.dimx) {nx_b=light_map.dimx-1;}
	  if (ny_b < 0) ny_b=0;
	  else if (ny_b >= light_map.dimy) {ny_b=light_map.dimy-1;}

//	  ol=(ny_b << light_map.xbits) + nx_b;
	  ol=muly[ny_b] + nx_b;

//	  image->uchar8ptr[ov*4]=lmap[ol*4];
//	  ol=(ny_g << light_map.xbits) + nx_g;
//	  image->uchar8ptr[ov*4+1]=lmap[ol*4+1];
//	  ol=(ny_r << light_map.xbits) + nx_r;
//	  image->uchar8ptr[ov*4+2]=lmap[ol*4+2];

	  p1=(height_map.map32[ov] & 0x00fefefe) >> 1;
	  p2=(light_map.map32[ol] & 0x00fefefe) >> 1;
	  image->uint32ptr[ov]=p1+p2;
	  ov++;
	}
  }
}
*/

// versione per la intro del webbit 2001
void bump2D::plot(int lightX, int lightY)
{
  int x, y;
  unsigned int *hmap32=height_map.map32;
  unsigned char *hmap=height_map.map;;
  unsigned int *lmap32=light_map.map32, *dest=image->uint32ptr;
  int *muly=image->muly;
  int ov, ol;
  int oy, oy_1, oy1;
  int nx_b, ny_b;
  unsigned int p1, p2;

  
  for (y=36; y<image->height-69; y++) 
  { 
	oy_1=muly[y-1] << 2;
	oy=muly[y] << 2;
	oy1=muly[y+1] << 2;
	ov=muly[y]+1;
    for (x=1; x<image->width-1; x++) 
	{  
	  // calcolo delle normali
	  nx_b=((hmap[oy+5] - hmap[oy-3])+
		    (hmap[oy+6] - hmap[oy-2])) << 1;
      ny_b=((hmap[oy1+1] - hmap[oy_1+1])+
           (hmap[oy1+2] - hmap[oy_1+2])) << 1;

	  oy+=4;
	  oy_1+=4;
	  oy1+=4;

      nx_b=nx_b + x;
      ny_b=ny_b + y;
	  if (nx_b < 0) nx_b=0;
	  else if (nx_b >= light_map.dimx) {nx_b=light_map.dimx-1;}
	  if (ny_b < 0) ny_b=0;
	  else if (ny_b >= light_map.dimy) {ny_b=light_map.dimy-1;}

	  ol=muly[ny_b] + nx_b;
	  p1=hmap32[ov];
	  p2=lmap32[ol];
	  dest[ov++]=RGBAdd(p1, p2);
	}
  }
}


void bump2D::do_efx2D(double pos)
{
/*
   plot((int)(110*cos(20*pos)+140.0f), 
	    (int)(80*sin(15*pos)+110.0f));
*/
   plot(0, 0);
}