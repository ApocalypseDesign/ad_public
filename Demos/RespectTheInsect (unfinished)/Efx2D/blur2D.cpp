#include "blur2D.h"
#include "..\utility.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error

blur2D::blur2D()
{
  xtab=ytab=(int *)NULL;
  xc=yc=0;
  zoom=0;
}


void blur2D::calc_tables(float xcenter, float ycenter, float zoom_)
{
   int i;
   float val;

   if (xtab==(int *)NULL) return;
   if (ytab==(int *)NULL) return;

   for (i=0; i<image->width; i++)
   {
      val = ((xcenter - (float)i) * zoom_);
      xtab[i] = (int)val;
   }

   for (i=0; i<image->height; i++)
   {
      val = ((ycenter - (float)i) * zoom_);
      ytab[i] = (int)val;
   }
}


void blur2D::init()
{
   if (image==(imagebuffer *)NULL)
	   debug_error(miawin, "blur2D: init() error: imagebuffer not assigned");

   xtab=new int[image->width];
   ytab=new int[image->height];

   if ((xc==0) && (yc==0) && (zoom==0))
     set_parameters(image->width/2.0f, image->height/2.0f, 0.005f);
   else set_parameters(xc, yc, zoom);
}


void blur2D::set_parameters(float xcenter, float ycenter, float zoom_)
{
   if ((xc==xcenter) && (yc==ycenter) && (zoom==zoom_)) return;

   xc=xcenter;
   yc=ycenter;
   zoom=zoom_;
   if ((xtab!=(int *)NULL) && (ytab!=(int *)NULL))
      calc_tables(xc, yc, zoom);
}


#define RBLUR_LOOP \
       nx = x + xtab[x]; \
       ny = y + ytab[y]; \
       cofs = image->muly[y] + x; \
       nofs = image->muly[ny] + nx; \
       where[cofs]=RGBblend(where[cofs], where[nofs], 128); \



void blur2D::do_efx2D(double pos)
{
  int x, y, nx, ny, cx, cy;
  int cofs, nofs;
  unsigned int *where;

  where=image->uint32ptr;
  cx=(int)xc;
  cy=(int)yc;

  for (y=cy; y>=0; y--)
  {
    for (x=cx; x>=0; x--)
	{
      RBLUR_LOOP
	}
  }

  for (y=cy; y>=0; y--)
  {
    for (x=cx+1; x<image->width; x++)
	{
       RBLUR_LOOP
	}
  }

  for (y=cy+1; y<image->height; y++)
  {
    for (x=cx; x>=0; x--)
	{
       RBLUR_LOOP
	}
  }

  for (y=cy+1; y<image->height; y++)
  {
    for (x=cx+1; x<image->width; x++)
	{
       RBLUR_LOOP
	}
  }
}

//*********************************************************
zblur2D::zblur2D()
{
   fact=1.0f;
   in_ib=(imagebuffer *)NULL;
}


void zblur2D::init()
{
   fact=1.01f;
   if (in_ib==(imagebuffer *)NULL)
	   debug_error(miawin, "zblur: input buffer not setted");

   if ((in_ib->width != image->width) ||
       (in_ib->height != image->height))
	   debug_error(miawin, "zblur: input and output buffer dimensions mismatching");
}


void zblur2D::set_imagebuffer_in(imagebuffer *ib)
{
   in_ib=ib;
}


void zblur2D::do_efx2D(double pos)
{
  int o, x, y, offset, offset2, yp;
  unsigned int yarr[240];
  unsigned int xarr[320];
  unsigned char *dest, *org;
  unsigned int *dest32, *org32;
  double tmp=0;

// Clipping (x)
  o=(int)((image->width*0.5 *(fact-1.0)*(1.0/fact)));

// Precalcolo ciclo x
  for (x=0; x<image->width; x++)
  {
    xarr[x]=o;
    tmp+=1;
    if (tmp>=fact) { tmp-=fact; o++; }
  }
  tmp=0;

// Clipping (y)
  o=(int)((image->height*0.5  *(fact-1.0)*(1.0/fact)));

// Precalcolo ciclo y
  for (y=0;y<image->height;y++)
  {
    yarr[y]=(unsigned int)(o*image->width);
    tmp+=1;
    if (tmp>=fact) { tmp-=fact; o++; }
  }


  //offset=image->width*image->height;
  dest=image->uchar8ptr;
  org=in_ib->uchar8ptr;
  dest32=image->uint32ptr;
  org32=in_ib->uint32ptr;

  for (y=0; y<image->height; y++)
  {
    yp=yarr[y];
    for (x=0; x<image->width; x++)
	{
	  offset=image->muly[y]+x;
      offset2=xarr[x]+yp;
      dest32[offset]=RGBblend(dest32[offset],
		                      org32[offset2], 128);
    }
  }

//  fact+=0.02f;
}