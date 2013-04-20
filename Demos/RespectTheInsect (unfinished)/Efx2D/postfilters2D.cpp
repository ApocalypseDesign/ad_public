#include <windows.h>
#include <math.h>
#include "postfilters2D.h"
#include "..\utility.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error

rainbow2D::rainbow2D()
{
}


void rainbow2D::init()
{
   if (image==(imagebuffer *)NULL)
      debug_error(miawin, "Rainbow2D: imagebuffer is NULL");
}


void rainbow2D::do_efx2D(double pos)
{
  int i;
  unsigned char *img;
  unsigned int *img32;

  if (image==(imagebuffer *)NULL)
      debug_error(miawin, "Rainbow2D: imagebuffer is NULL");

  img=image->uchar8ptr;
  img32=image->uint32ptr;

  for (i=0; i<image->channelsize; i++)
  {
    unsigned char r, g, b;
    float red, green, blue;
    float hue, saturation, value;

    b = *img++;
    g = *img++;
    r = *img++;
	img++,

    red   = r / 255.0f;
    green = g / 255.0f;
    blue  = b / 255.0f;

    rgb_to_hsv(red, green, blue, &hue, &saturation, &value);

//    hue = value * (parameters[1] + 1.0) + parameters[0];
    hue = (float)(value * (pos + 1.0) + pos);
    hue = hue - (int) hue;

    hsv_to_rgb(hue, saturation, value, &red, &green, &blue);
      
    r = (unsigned char)(red * 255);
    g = (unsigned char)(green * 255);
    b = (unsigned char)(blue * 255);

    *img32=(b | (g<<8) | (r<<16));
	img32++;
  }
}


//************************************************************

rubbing2D::rubbing2D()
{
   imageout=(imagebuffer *)NULL;
}


void rubbing2D::set_imageout(imagebuffer *o)
{
   if (o==(imagebuffer *)NULL)
      debug_error(miawin, "Rubbing2D: set_imageout is NULL");
   else imageout=o;
}


void rubbing2D::init()
{
   if (image==(imagebuffer *)NULL)
      debug_error(miawin, "Rubbing2D: imagebuffer is NULL");
   if (imageout==(imagebuffer *)NULL)
      debug_error(miawin, "Rubbing2D: imageout is NULL");
}


void rubbing2D::do_efx2D(double pos)
{
   #define XY(X,Y) ((X + image->muly[Y])<<2)

   if (image==(imagebuffer *)NULL)
      debug_error(miawin, "Rubbing2D: imagebuffer is NULL");
   if (imageout==(imagebuffer *)NULL)
      debug_error(miawin, "Rubbing2D: imageout is NULL");

  unsigned char *srcpix=image->uchar8ptr;
  unsigned char *dstpix=imageout->uchar8ptr;
  int x, y;
  int delta;

  delta = 5;

  for (y = 0; y < image->height; y++)
    for (x = 0; x < image->width; x++)
    {
	   int x2, y2;
	   int count, total, average, orig;
	   int color;
	
	   total = 0;
	   count = 0;

	   for (x2 = -delta; x2 <= delta; x2++)
	     for (y2 = -delta; y2 <= delta; y2++)
		 {
	        if (((x + x2) >= 0) &&
		       ((x + x2) < image->width) &&
		       ((y + y2) >= 0) &&
		       ((y + y2) < image->height))
			{
		       total += 
		         srcpix[XY(x + x2, y + y2)] + /* red */
		         srcpix[XY(x + x2, y + y2) + 1] + /* green */
		         srcpix[XY(x + x2, y + y2) + 2]; /* blue */
		         count++;
			}
		 }
		 average = total / count;
	     orig = srcpix[XY(x,y)] +
	            srcpix[XY(x,y)+1] +
	            srcpix[XY(x,y)+2];

	     if (average > orig)
		 {
		    color=0;
		 }
	     else
		 {
		    color=255;
		 }
	     dstpix[XY(x,y)]   = color;
	     dstpix[XY(x,y)+1] = color;
	     dstpix[XY(x,y)+2] = color;
	}
}

//************************************************************

mosaic2D::mosaic2D()
{
}


void mosaic2D::init()
{
   if (image==(imagebuffer *)NULL)
      debug_error(miawin, "Mosaic2D: imagebuffer is NULL");
}

typedef struct
{
  int left;
  int top;
  int right;
  int bottom;
} rect;


void average_rect(imagebuffer *surf, rect *rgn, int *c)
{
  int r = 0;
  int g = 0;
  int b = 0;
  int i;
  int x, y;
  int blocksize = (rgn->right - rgn->left) * (rgn->bottom - rgn->top);

  if (blocksize==0) blocksize=1;

  for (y = rgn->top; y < rgn->bottom; y++)
  {
    i = (surf->muly[y] + rgn->left)*4;
    for (x = rgn->left; x < rgn->right; x++)
	{
      b += surf->uchar8ptr[i++];
      g += surf->uchar8ptr[i++];
      r += surf->uchar8ptr[i++];
	  i++;
    }
  }
  r = r / blocksize;
  g = g / blocksize;
  b = b / blocksize;
  *c=(b | (g<<8) | (r<<16));
}


void mosaic2D::do_efx2D(double pos)
{
   if (image==(imagebuffer *)NULL)
      debug_error(miawin, "Mosaic2D: imagebuffer is NULL");

  int step;
  int x, y;
  int x2, y2;
  int i;
  rect rgn;
  int c;
  
  pos=(pos-fstartpos)/(fendpos-fstartpos);
  step = (int)((1-pos)*32+2);
  i = 0;

  for (y = 0; y < image->height; y += step)
  {
    for (x = 0; x < image->width; x += step)
	{
      rgn.left = x;
      rgn.top = y;
      rgn.right = x + step;
      rgn.bottom = y + step;
      if (rgn.right >= image->width) rgn.right = image->width - 1;
      if (rgn.bottom >= image->height) rgn.bottom = image->height - 1;
      average_rect(image, &rgn, &c);
      for (y2 = y; y2 < y + step; y2++)
	  {
	     if (y2 >= image->height) break;
	     i = image->muly[y2] + x;
	     for (x2 = 0; x2 < step; x2++)
		 {
	        if ((x+x2) >= image->width) break;
	        image->uint32ptr[i++] = c;
		 }
	  }
	}
  }
}

//************************************************************

sniper2D::sniper2D()
{
}


void sniper2D::init()
{
   if (image==(imagebuffer *)NULL)
      debug_error(miawin, "Sniper2D: imagebuffer is NULL");
}


void draw_circle(int x, int y, int radius, int c, imagebuffer *src)
{
  #define XY2(X,Y) ((X + src->muly[Y])<<2)

  int width = src->width;
  int i, j;
  int cx, cy;
  int dist;
  int rsquared;
  int idx;
  unsigned char r, g, b;
  unsigned char r2, g2, b2;
/*
  for (j = y; j < y + 2*radius; j++)
  {
    for (i = x; i < x + 2*radius; i++)
	{
	   idx = XY2(i, j);
	   src->uchar8ptr[idx] = 0;
	   src->uchar8ptr[idx+1] = 0;
	   src->uchar8ptr[idx+2] = 0;
	}
  }
*/

  cx = x + radius;
  cy = y + radius;
  rsquared = radius * radius;
  r=r2=(c >> 16) & 0xff;
  g=g2=(c >> 8) & 0xff;
  b=b2=c & 0xff;
  for (j = y; j < y + radius; j++)
  {
    for (i = x; i < x + radius; i++)
	{
      dist = (cx - i) * (cx - i) + (cy - j) * (cy - j);
      if (dist < rsquared)
	  {
/*
	     scale = (double)((double)sqrt(dist) / (double)sqrt(rsquared));
	     r2 = r * scale;
	     g2 = g * scale;
	     b2 = b * scale;
*/
	     // Quadrant II
	     idx = XY2(i, j);
	     src->uchar8ptr[idx] = b2;
	     src->uchar8ptr[idx+1] = g2;
	     src->uchar8ptr[idx+2] = r2;

	     // Quadrant I
	     idx = XY2(2 * cx - i - 1, j);
	     src->uchar8ptr[idx] = b2;
	     src->uchar8ptr[idx+1] = g2;
	     src->uchar8ptr[idx+2] = r2;

	     // Quadrant III
	     idx = XY2(i, 2 * cy - j - 1);
	     src->uchar8ptr[idx] = b2;
	     src->uchar8ptr[idx+1] = g2;
	     src->uchar8ptr[idx+2] = r2;

	     // Quadrant IV
	     idx = XY2(2 * cx - i - 1, 2 * cy - j - 1);
	     src->uchar8ptr[idx] = b2;
	     src->uchar8ptr[idx+1] = g2;
	     src->uchar8ptr[idx+2] = r2;
	  }
    }
  }
}


void sniper2D::do_efx2D(double pos)
{
   if (image==(imagebuffer *)NULL)
      debug_error(miawin, "Sniper2D: imagebuffer is NULL");

  rect rgn;
  int radius;
  int diameter;
  int x, y;
  int c;

  pos=(pos-fstartpos)/(fendpos-fstartpos);
  radius = (int)(3.0*sin(32*pos)+2 )+3;
  diameter = 2 * radius;

  for (y = 0; y < image->height - diameter; y += diameter - 1)
  {
    for (x = 0; x < image->width - diameter; x += diameter - 1)
	{
	   rgn.left = x;
	   rgn.top = y;
	   rgn.right = x + diameter;
	   rgn.bottom = y + diameter;
	   average_rect(image, &rgn, &c);
	   draw_circle(x, y, radius, c, image);
    }
  }
}