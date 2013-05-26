#include "tomato2D.h"
#include "..\utility.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error

void tomato2D::blur_image(imagebuffer *src, imagebuffer *dest)
{
   int x, y;
   unsigned int R, G, B, ofs;

   if ((src->width != dest->width) || (src->width != dest->width))
	   debug_error(miawin, "tomato2D::blur_image err: difference in dimensions");

   for (y=1; y<src->height-1; y++)
   {
	 ofs=src->muly[y];
     for (x=1; x<src->width-1; x++)
	 {
        R=G=B=0;

		B+=src->uchar8ptr[(ofs + x)*4];
		B+=src->uchar8ptr[(ofs + x-1)*4];
		B+=src->uchar8ptr[(ofs + x+1)*4];
		B+=src->uchar8ptr[(ofs - src->width)*4];
		B+=src->uchar8ptr[(ofs + src->width)*4];

		G+=src->uchar8ptr[(ofs + x)*4 + 1];
		G+=src->uchar8ptr[(ofs + x-1)*4 + 1];
		G+=src->uchar8ptr[(ofs + x+1)*4 + 1];
		G+=src->uchar8ptr[(ofs - src->width)*4 + 1];
		G+=src->uchar8ptr[(ofs + src->width)*4 + 1];

		R+=src->uchar8ptr[(ofs + x)*4 + 2];
		R+=src->uchar8ptr[(ofs + x-1)*4 + 2];
		R+=src->uchar8ptr[(ofs + x+1)*4 + 2];
		R+=src->uchar8ptr[(ofs - src->width)*4 + 2];
		R+=src->uchar8ptr[(ofs + src->width)*4 + 2];

		B/=5; G/=5; R/=5;
		dest->uchar8ptr[(ofs + x)*4]=(unsigned char)B;
		dest->uchar8ptr[(ofs + x)*4+1]=(unsigned char)G;
		dest->uchar8ptr[(ofs + x)*4+2]=(unsigned char)R;
	 }
   }
}


tomato2D::tomato2D()
{
  blurred_img=current_img=(imagebuffer *)NULL;
}


void tomato2D::init()
{
   if (image==(imagebuffer *)NULL)
	   debug_error(miawin, "tomato2D: init() error: imagebuffer not assigned");

  blurred_img=new imagebuffer(image->width, image->height);
  current_img=new imagebuffer(image->width, image->height);
}


void tomato2D::draw_circle(int x, int y, int radius, int c, imagebuffer *where)
{
  #define XY2(X,Y) ((X + where->muly[Y])<<2)

  int width = where->width;
  int i, j;
  int cx, cy;
  int dist;
  int rsquared;
  int idx;
  unsigned char r, g, b;
  unsigned char r2, g2, b2;

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
	     idx = XY2(i, j);
	     where->uchar8ptr[idx] = b2;
	     where->uchar8ptr[idx+1] = g2;
	     where->uchar8ptr[idx+2] = r2;

	     idx = XY2(2 * cx - i - 1, j);
	     where->uchar8ptr[idx] = b2;
	     where->uchar8ptr[idx+1] = g2;
	     where->uchar8ptr[idx+2] = r2;

	     idx = XY2(i, 2 * cy - j - 1);
	     where->uchar8ptr[idx] = b2;
	     where->uchar8ptr[idx+1] = g2;
	     where->uchar8ptr[idx+2] = r2;

	     idx = XY2(2 * cx - i - 1, 2 * cy - j - 1);
	     where->uchar8ptr[idx] = b2;
	     where->uchar8ptr[idx+1] = g2;
	     where->uchar8ptr[idx+2] = r2;
	  }
    }
  }
}


void tomato2D::draw_rectangle(int x, int y, int radius, int c, imagebuffer *where)
{
      for (int y2=y; y2<y+radius; y2++)
	  {
	     int i = where->muly[y2] + x;
	     for (int x2=0; x2<radius; x2++)
		 {
	        where->uint32ptr[i++] = c;
		 }
	  }
}


int tomato2D::do_rect(imagebuffer *src, imagebuffer *ref, rect *rgn, int *c)
{
  int b, g, r;
  int i;
  int x, y, max_error=-1, err, tot_err, err_x, err_y;

  tot_err=0;
  for (y = rgn->top; y < rgn->bottom; y++)
  {
    i = (src->muly[y] + rgn->left)*4;
    for (x = rgn->right- rgn->left; x>0 ; x--)
	{
	  b=src->uchar8ptr[i] - ref->uchar8ptr[i];
	  g=src->uchar8ptr[i+1] - ref->uchar8ptr[i+1];
	  r=src->uchar8ptr[i+2] - ref->uchar8ptr[i+2];
	  i+=4;
	  err=b*b + g*g + r*r;
	  tot_err+=err;
	  if (err > max_error)
	  {
		 err_x=x;
		 err_y=y;
		 max_error=err;
	  }
    }
  }

  if (tot_err > err_limit)
  {
	 rgn->bottom=err_y;
     rgn->left=err_x+rgn->left;
     *c=src->uint32ptr[ref->muly[err_y]+ rgn->left];
	 return(1);
  }
  else return(0);
}


void tomato2D::paint_layer(imagebuffer *src, imagebuffer *ref, imagebuffer *dest, int r)
{
  rect rgn;
  int diameter;
  int x, y, col;
  int B, G, R;
  int i;
  int xx, yy, max_error, err, tot_err, err_x, err_y;

  if (r==0) r=1;
  diameter=2*r;
  blocksize = diameter*diameter;
  err_limit = blocksize*4096;

  for (y=0; y<src->height-diameter; y+=diameter-1)
  {
    for (x=0; x<src->width-diameter; x+=diameter-1)
	{
	   rgn.left = x;
	   rgn.top = y;
	   rgn.right = x + diameter;
	   rgn.bottom = y + diameter;
       tot_err=0;
       max_error=-1;
       for (yy = rgn.top; yy < rgn.bottom; yy++)
	   {
           i = (src->muly[yy] + rgn.left)*4;
           for (xx = rgn.right- rgn.left; xx>0 ; xx--)
		   {
	          B=src->uchar8ptr[i] - ref->uchar8ptr[i];
	          G=src->uchar8ptr[i+1] - ref->uchar8ptr[i+1];
	          R=src->uchar8ptr[i+2] - ref->uchar8ptr[i+2];
	          i+=4;
	          err=B*B + G*G + R*R;
	          tot_err+=err;
	          if (err > max_error)
			  {
		         err_x=xx;
		         err_y=yy;
		         max_error=err;
			  }
		   }
	   }
	   
       if (tot_err > err_limit)
	   {
	      rgn.bottom=err_y;
          rgn.left=err_x + rgn.left;
          col = src->uint32ptr[ref->muly[err_y] + rgn.left];
		  if ((rgn.left>r) && (rgn.left+r<dest->width-1) &&
			  (rgn.bottom>r) && (rgn.bottom+r<dest->height-1))
	         for (yy=rgn.bottom-r; yy<rgn.bottom+r; yy++)
			 {
	            i = dest->muly[yy] + rgn.left-r;
	            for (xx=diameter; xx>0; xx--) dest->uint32ptr[i++] = col;
			 }
	   }
	}
  }
}


void tomato2D::do_efx2D(double pos)
{
   //blur_image(image, blurred_img);
   current_img->clear(0);
   blurred_img->clear(0);
   //memcpy(blurred_img->voidptr, image->voidptr, blurred_img->channelsize*4);

   paint_layer(image, current_img, blurred_img, 4);
   memcpyd(current_img->voidptr, blurred_img->voidptr, blurred_img->channelsize);

   paint_layer(image, current_img, blurred_img, 4);
   memcpyd(current_img->voidptr, blurred_img->voidptr, blurred_img->channelsize);

   paint_layer(image, current_img, blurred_img, 2);
//   memcpyd(current_img->voidptr, blurred_img->voidptr, blurred_img->channelsize);
//   memcpyd(image->voidptr, blurred_img->voidptr, blurred_img->channelsize);
//   blur_image(blurred_img, image);
   memcpyd(image->voidptr, blurred_img->voidptr, blurred_img->channelsize);
}