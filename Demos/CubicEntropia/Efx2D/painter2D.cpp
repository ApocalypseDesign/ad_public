#include <math.h>
#include "..\Math\fastmath.h"
#include "painter2D.h"

extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

painter2D::painter2D()
{
}


void painter2D::init()
{
}


void painter2D::draw_line(float x1, float y1, float x2, float y2,
	                      unsigned char r, unsigned char g, unsigned char b,
					      imagebuffer *ib)
{
   float prestep, fcy, faux, xd, yd;
   int top_y, bottom_y, top_x, bottom_x, j;
   int x, x_inc, y, y_inc, offset0, ofs, w;
   unsigned int *img32, col32;

   // rigetti banali
   if ((x1<0) && (x2<0)) return;
   if ((y1<0) && (y2<0)) return;
   if ((x1>=(float)(ib->width)) &&
	   (x2>=(float)(ib->width))) return;
   if ((y1>=(float)(ib->height)) &&
	   (y2>=(float)(ib->height))) return;

   col32=(unsigned int)(b | (g << 8) | (r << 16));
   xd=x2-x1;
   yd=y2-y1;
   img32=ib->uint32ptr;

   if (fabsf(yd)>fabsf(xd))
   {
	  if (y1>y2) { faux=y1; y1=y2; y2=faux; faux=x1; x1=x2; x2=faux; }
      // subpixel + clipping
      prestep=-y1;
      if (y1>=0) 
	  {
        fcy=fceil(y1);
        prestep=fcy + prestep;
        top_y=myfist(fcy);
	  } else top_y=0;
      if (y2 >= (float)(ib->height)) bottom_y=ib->height;
      else bottom_y=myfist(fceil(y2));

      faux=(x2 - x1)/(y2-y1);
      x_inc=myfist16(faux);
      x=myfist16(x1 + prestep*faux);
      offset0=ib->muly[top_y];
      for (j=abs(bottom_y-top_y); j>0; j--)
	  {
		 w=ceil16(x);
		 if ((w>=0) && (w<ib->width))
		 {
	       ofs=offset0 + w;
           img32[ofs]=col32;
         }
		 offset0=offset0+ib->width;
         x+=x_inc;
	  }
   }
   else
   {
	  if (x1>x2) { faux=y1; y1=y2; y2=faux; faux=x1; x1=x2; x2=faux; }
      // subpixel + clipping
      prestep=-x1;
      if (x1>=0) 
	  {
        fcy=fceil(x1);
        prestep=fcy + prestep;
        bottom_x=myfist(fcy);
	  } else bottom_x=0;
      if (x2 >= (float)(ib->width)) top_x=ib->width;
      else top_x=myfist(fceil(x2));

      faux=(y2-y1)/(x2-x1);
/*
	  if ((y1<0) && (y2>=0))
	  {
		 bottom_x=bottom_x + myfist(fceil(-y1/faux));
		 y1=0;
		 prestep=0;
	  }
	  else
	  if ((y1>=0) && (y2<0))
	  {
		 top_x=top_x - myfist(fceil(-y2/faux));
	  }
*/
      y_inc=myfist16(faux);
      y=myfist16(y1 + prestep*faux);
      for (j=bottom_x; j<top_x; j++)
	  {
		 w=ceil16(y);
		 if ((w>=0) && (w<ib->height))
		 {
	       ofs=ib->muly[w]+j;
           img32[ofs]=col32;
		 }
         y+=y_inc;
	  }
   }
}


void painter2D::draw_aaline(float x1, float y1, float x2, float y2,
	                        unsigned char r, unsigned char g, unsigned char b,
					        imagebuffer *ib)
{

   float prestep, fcy, faux, xd, yd;
   int top_y, bottom_y, top_x, bottom_x, j, cx, fraz, ifraz;
   int x, x_inc, y, y_inc, offset0, ofs, c1, c2, c3, w;
   unsigned char *img;

   // rigetti banali
   if ((x1<0) && (x2<0)) return;
   if ((y1<0) && (y2<0)) return;
   if ((x1>(float)(ib->width)) &&
	   (x2>(float)(ib->width))) return;
   if ((y1>(float)(ib->height)) &&
	   (y2>(float)(ib->height))) return;

   xd=x2-x1;
   yd=y2-y1;
   img=ib->uchar8ptr;

   if (fabsf(yd)>fabsf(xd))
   {
	  if (y1>y2) { faux=y1; y1=y2; y2=faux; faux=x1; x1=x2; x2=faux; }
      // subpixel + clipping
      prestep=-y1;
      if (y1>=0) 
	  {
        fcy=fceil(y1);
        prestep=fcy + prestep;
        top_y=myfist(fcy);
	  } else top_y=0;
      if (y2 >= (float)(ib->height)) bottom_y=ib->height;
      else bottom_y=myfist(fceil(y2));

      faux=(x2 - x1)/(y2-y1);
      x_inc=myfist16(faux);
      x=myfist16(x1 + prestep*faux);
      offset0=ib->muly[top_y];
      for (j=abs(bottom_y-top_y); j>0; j--)
	  {
		 cx=ceil16(x);
		 if ((cx>=0) && (cx<ib->width-1))
		 {
	        ofs=offset0 + cx;
		    fraz=x & 0x0000FFFF;
		    ifraz=0x10000-fraz;
		    c1=(fraz*b)>>16;
		    c2=(fraz*g)>>16;
		    c3=(fraz*r)>>16;
            img[ofs*4]=b-(unsigned char)c1;
		    img[ofs*4+1]=g-(unsigned char)c2;
		    img[ofs*4+2]=r-(unsigned char)c3;
		    c1=(ifraz*b)>>16;
		    c2=(ifraz*g)>>16;
		    c3=(ifraz*r)>>16;
		    img[ofs*4+4]=b-(unsigned char)c1;
		    img[ofs*4+5]=g-(unsigned char)c2;
		    img[ofs*4+6]=r-(unsigned char)c3;
		 }
         offset0=offset0+ib->width;
         x+=x_inc;
	  }
   }
   else
   {
	  if (x1>x2) { faux=y1; y1=y2; y2=faux; faux=x1; x1=x2; x2=faux; }
      // subpixel + clipping
      prestep=-x1;
      if (x1>=0) 
	  {
        fcy=fceil(x1);
        prestep=fcy + prestep;
        top_x=myfist(fcy);
	  } else top_x=0;
      if (x2 >= (float)(ib->width)) bottom_x=ib->width;
      else bottom_x=myfist(fceil(x2));

      faux=(y2-y1)/(x2-x1);
      y_inc=myfist16(faux);
      y=myfist16(y1 + prestep*faux);
      for (j=top_x; j<bottom_x; j++)
	  {
		 w=ceil16(y);
		 if ((w>=0) && (w<ib->height-1))
		 {
	        ofs=(ib->muly[w]+j)*4;
		    fraz=y & 0x0000FFFF;
		    ifraz=0x10000-fraz;
		    c1=(fraz*b)>>16;
		    c2=(fraz*g)>>16;
		    c3=(fraz*r)>>16;
            img[ofs]=b-(unsigned char)c1;
		    img[ofs+1]=g-(unsigned char)c2;
		    img[ofs+2]=r-(unsigned char)c3;
		    c1=(ifraz*b)>>16;
		    c2=(ifraz*g)>>16;
		    c3=(ifraz*r)>>16;
		    img[ofs+ib->width*4]=b-(unsigned char)c1;
		    img[ofs+ib->width*4+1]=g-(unsigned char)c2;
		    img[ofs+ib->width*4+2]=r-(unsigned char)c3;
		 }
         y+=y_inc;
	  }
   }
}


void painter2D::draw_aaline_add(float x1, float y1, float x2, float y2,
	                        unsigned char r, unsigned char g, unsigned char b,
					        imagebuffer *ib)
{

   float prestep, fcy, faux, xd, yd;
   int top_y, bottom_y, top_x, bottom_x, j, cx, fraz, ifraz;
   int x, x_inc, y, y_inc, offset0, ofs, c1, c2, c3, w;
   unsigned char *img;
   unsigned int *img32, c;

   // rigetti banali
   if ((x1<0) && (x2<0)) return;
   if ((y1<0) && (y2<0)) return;
   if ((x1>(float)(ib->width)) &&
	   (x2>(float)(ib->width))) return;
   if ((y1>(float)(ib->height)) &&
	   (y2>(float)(ib->height))) return;

   xd=x2-x1;
   yd=y2-y1;
   img=ib->uchar8ptr;
   img32=ib->uint32ptr;

   if (fabsf(yd)>fabsf(xd))
   {
	  if (y1>y2) { faux=y1; y1=y2; y2=faux; faux=x1; x1=x2; x2=faux; }
      // subpixel + clipping
      prestep=-y1;
      if (y1>=0) 
	  {
        fcy=fceil(y1);
        prestep=fcy + prestep;
        top_y=myfist(fcy);
	  } else top_y=0;
      if (y2 >= (float)(ib->height)) bottom_y=ib->height;
      else bottom_y=myfist(fceil(y2));

      faux=(x2 - x1)/(y2-y1);
      x_inc=myfist16(faux);
      x=myfist16(x1 + prestep*faux);
      offset0=ib->muly[top_y];
      for (j=abs(bottom_y-top_y); j>0; j--)
	  {
		 cx=ceil16(x);
		 if ((cx>=0) && (cx<ib->width-1))
		 {
	        ofs=offset0 + cx;
		    fraz=x & 0x0000FFFF;
		    ifraz=0x10000-fraz;
		    c1=(fraz*b)>>16;
		    c2=(fraz*g)>>16;
		    c3=(fraz*r)>>16;

            c=(b-(unsigned char)c1) + ((g-(unsigned char)c2) <<8) +
		      ((r-(unsigned char)c3)<<16);

		    c1=img32[ofs];
            img32[ofs]=RGBAdd(c, c1);

		    c1=(ifraz*b)>>16;
		    c2=(ifraz*g)>>16;
		    c3=(ifraz*r)>>16;
            c=(b-(unsigned char)c1) + ((g-(unsigned char)c2) <<8) +
		       ((r-(unsigned char)c3)<<16);

		    c1=img32[ofs+1];
            img32[ofs+1]=RGBAdd(c, c1);
		 }
         offset0=offset0+ib->width;
         x+=x_inc;
	  }
   }
   else
   {

	  if (x1>x2) { faux=y1; y1=y2; y2=faux; faux=x1; x1=x2; x2=faux; }
      // subpixel + clipping
      prestep=-x1;
      if (x1>=0) 
	  {
        fcy=fceil(x1);
        prestep=fcy + prestep;
        top_x=myfist(fcy);
	  } else top_x=0;
      if (x2 >= (float)(ib->width)) bottom_x=ib->width;
      else bottom_x=myfist(fceil(x2));

      faux=(y2-y1)/(x2-x1);
      y_inc=myfist16(faux);
      y=myfist16(y1 + prestep*faux);
      for (j=top_x; j<bottom_x; j++)
	  {
		 w=ceil16(y);
		 if ((w>=0) && (w<ib->height-1))
		 {
	        ofs=(ib->muly[w]+j);
		    fraz=y & 0x0000FFFF;
		    ifraz=0x10000-fraz;
		    c1=(fraz*b)>>16;
		    c2=(fraz*g)>>16;
		    c3=(fraz*r)>>16;
            c=(b-(unsigned char)c1) + ((g-(unsigned char)c2) <<8) +
		      ((r-(unsigned char)c3)<<16);

		    c1=img32[ofs];
            img32[ofs]=RGBAdd(c, c1);

		    c1=(ifraz*b)>>16;
		    c2=(ifraz*g)>>16;
		    c3=(ifraz*r)>>16;
            c=(b-(unsigned char)c1) + ((g-(unsigned char)c2) <<8) +
		      ((r-(unsigned char)c3)<<16);

		    c1=img32[ofs+ib->width];
            img32[ofs+ib->width]=RGBAdd(c, c1);
		 }
         y+=y_inc;
	  }
   }
}


void painter2D::draw_spline2D (spline2D *s2D, int subd,
	                           unsigned char r, unsigned char g, unsigned char b,
							   imagebuffer *ib)
{
   float step, t;
   key2D p1, p2;
   int i;

   if (subd<2) subd=2;
   step=1.0f/subd;
   t=step;
   s2D->evaluate(0, &p1);
   for (i=0; i<subd; i++)
   {
     s2D->evaluate(t, &p2);
	 draw_line(p1.x, p1.y, p2.x, p2.y, r, g, b, ib);
	 t+=step;
	 p1=p2;
   }
}


void painter2D::draw_aaspline2D (spline2D *s2D, int subd,
	                             unsigned char r, unsigned char g, unsigned char b,
								 imagebuffer *ib)
{
   float step, t;
   key2D p1, p2;
   int i;

   if (subd<2) subd=2;
   step=1.0f/(subd-1);
   t=step;
   s2D->evaluate(0, &p1);
   for (i=0; i<subd; i++)
   {
     s2D->evaluate(t, &p2);
     draw_aaline(p1.x, p1.y, p2.x, p2.y, r, g, b, ib);
	 t+=step;
	 p1=p2;
   }
}