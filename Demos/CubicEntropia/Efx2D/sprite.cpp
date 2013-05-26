#include "sprite.h"
#include "..\Math\fastmath.h"


AD_Sprite::AD_Sprite()
{
  int w;

  num_keys=0;
  for (w=0; w<MAXKEY; w++)
  {
	keys[w].relpos=0;
    keys[w].x=keys[w].y=0;
    keys[w].scale_x=keys[w].scale_y=NOSCALING;
  }
  type=0 | RENDER_NOSCALING | RENDER_CLIP | RENDER_OVERWRITE;
}


void AD_Sprite::add_key(float relpos, float x, float y, float scale_x, float scale_y)
{
  if (num_keys>MAXKEY) return;

  if (relpos<0) relpos=0;
  if (relpos>1) relpos=1;

  keys[num_keys].relpos=relpos;
  keys[num_keys].x=x;
  keys[num_keys].y=y;

  if (scale_x<=0) scale_x=NOSCALING;
  if (scale_y<=0) scale_y=NOSCALING;

  keys[num_keys].scale_x=scale_x;
  keys[num_keys].scale_y=scale_y;
  num_keys++;
}


void AD_Sprite::set_render_type(int render_type)
{
  type=render_type;
}


// ########################################################
// ##############   VERSIONI SENZA SCALING   ##############
// ########################################################

void AD_Sprite::put(int x, int y, imagebuffer *imagebuf)
{
   int xs, ys;
   int voffset, incrow, tex_offset=0;
   unsigned int *image32=imagebuf->uint32ptr, *m32=map32;

   voffset=imagebuf->muly[y]+x;
   incrow=imagebuf->width-dimx;
   xs=dimx >> 1;
   ys=dimy;

   _asm
   {
	  mov edx, [incrow]
	  lea eax, [edx*4]

	  mov esi, [image32]
	  mov edx, [voffset]
	  shl edx, 2
	  add esi, edx

	  mov edi, [m32]
      mov ebx, [ys]

cicloX:
	  mov ecx, [xs]

inner:
      movq mm0, [edi]
	  add edi, 8
	  movq [esi], mm0
	  add esi, 8
	  dec ecx
	  jnz inner

	  add esi, eax
	  dec ebx
	  jnz cicloX
	  emms
   }

/*
   for (ys=0; ys<dimy; ys++)
   {
	 for(xs=0; xs<dimx; xs++)
	 {
	   imagebuf->uint32ptr[offset]=map32[tex_offset];
	   offset++;
	   tex_offset++;
	 }
	 offset+=incrow;
   }
*/
}


void AD_Sprite::put(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     put(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);
   put(xx, yy, imagebuf);
}


void AD_Sprite::put_clip(int x, int y, imagebuffer *imagebuf)
{
   int xs, ys, u, v;
   int x2, y2;
   int offset, incrow;
   int tex_offset, tex_incrow;


   u=v=0;
   x2=x+dimx;
   y2=y+dimy;

   // CIPPING 2D nelle x
   if (x>=imagebuf->width) return;
   if (x<0)
   {
	 u=(-x);
	 x=0;
   }
   if (x2>=imagebuf->width) x2=imagebuf->width;


   // CIPPING 2D nelle y
   if (y>=imagebuf->height) return;
   if (y<0)
   {
	 v=(-y);
	 y=0;
   }
   if (y2>=imagebuf->height) y2=imagebuf->height;


   // setup sezione video
   offset=imagebuf->muly[y]+x;
   incrow=imagebuf->width - (x2-x);

   // setup sezione sprite
   tex_offset=v*dimx+u;
   tex_incrow=dimx - (x2-x);

   for (ys=y; ys<y2; ys++)
   {
	 for(xs=x; xs<x2; xs++)
	 {
	   imagebuf->uint32ptr[offset]=map32[tex_offset];
	   offset++;
	   tex_offset++;
	 }
	 offset+=incrow;
	 tex_offset+=tex_incrow;
   }
}


void AD_Sprite::put_clip(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     put_clip(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);
   put_clip(xx, yy, imagebuf);
}



void AD_Sprite::put_blend50(int x, int y, imagebuffer *imagebuf)
{
   int xs, ys;
//   int pixel1, pixel2;
   int voffset, incrow, tex_offset;
   unsigned int *image32=imagebuf->uint32ptr, *m32=map32;

   voffset=imagebuf->muly[y]+x;
   incrow=imagebuf->width-dimx;
   tex_offset=0;
   xs=dimx >> 1;
   ys=dimy;

   _asm
   {
	  mov edx, [incrow]
	  lea eax, [edx*4]

	  mov esi, [image32]
	  mov edx, [voffset]
	  shl edx, 2
	  add esi, edx

	  mov edi, [m32]
      mov ebx, [ys]

	  mov ecx, 0x00fefefe
	  movd  mm7, ecx
	  punpckldq mm7, mm7

cicloX:
	  mov ecx, [xs]

inner:
      movq mm0, [esi]
	  movq mm2, mm7

	  movq mm1, [edi]
	  pand mm0, mm7

	  psrld mm0, 1
	  pand mm1, mm7

	  psrld mm1, 1
	  add edi, 8

	  paddd mm0, mm1
	  add esi, 8

	  movq [esi-8], mm0
	  dec ecx
	  jnz inner

	  add esi, eax

	  dec ebx
	  jnz cicloX
	  emms
   }

/*

   for (ys=0; ys<dimy; ys++)
   {
	 for(xs=0; xs<dimx; xs++)
	 {
	   pixel1=(imagebuf->uint32ptr[voffset] & 0x00fefefe) >>1;
       pixel2=(map32[tex_offset] & 0x00fefefe) >>1;
	   imagebuf->uint32ptr[voffset]=pixel1+pixel2;
	   voffset++;
	   tex_offset++;
	 }
	 voffset+=incrow;
   }
*/
}


void AD_Sprite::put_blend50(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     put_blend50(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);
   put_blend50(xx, yy, imagebuf);
}



void AD_Sprite::put_blend50_clip(int x, int y, imagebuffer *imagebuf)
{
   int xs, ys, u, v, pixel1, pixel2;
   int x2, y2;
   int offset, incrow;
   int tex_offset, tex_incrow;


   u=v=0;
   x2=x+dimx;
   y2=y+dimy;

   // CIPPING 2D nelle x
   if (x>=imagebuf->width) return;
   if (x<0)
   {
	 u=(-x);
	 x=0;
   }
   if (x2>=imagebuf->width) x2=imagebuf->width;


   // CIPPING 2D nelle y
   if (y>=imagebuf->height) return;
   if (y<0)
   {
	 v=(-y);
	 y=0;
   }
   if (y2>=imagebuf->height) y2=imagebuf->height;


   // setup sezione video
   offset=imagebuf->muly[y]+x;
   incrow=imagebuf->width - (x2-x);

   // setup sezione sprite
   tex_offset=v*dimx+u;
   tex_incrow=dimx - (x2-x);

   for (ys=y; ys<y2; ys++)
   {
	 for(xs=x; xs<x2; xs++)
	 {
	   pixel1=(map32[tex_offset] & 0xfefefe) >> 1;
	   pixel2=(imagebuf->uint32ptr[offset]  & 0xfefefe) >> 1;
	   imagebuf->uint32ptr[offset]=pixel1+pixel2;
	   offset++;
	   tex_offset++;
	 }
	 offset+=incrow;
	 tex_offset+=tex_incrow;
   }
}


void AD_Sprite::put_blend50_clip(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     put_blend50_clip(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);
   put_blend50_clip(xx, yy, imagebuf);
}



void AD_Sprite::put_add(int x, int y, imagebuffer *imagebuf)
{
   int xs, ys;
   int voffset, incrow, tex_offset;
   unsigned int *image32=imagebuf->uint32ptr, *m32=map32;

   voffset=imagebuf->muly[y]+x;
   incrow=imagebuf->width-dimx;
   tex_offset=0;
   xs=dimx >> 1;
   ys=dimy;


   _asm
   {
	  mov edx, [incrow]
	  lea eax, [edx*4]

	  mov esi, [image32]
	  mov edx, [voffset]
	  shl edx, 2
	  add esi, edx

	  mov edi, [m32]
      mov ebx, [ys]

cicloX:
	  mov ecx, [xs]

inner:
      movq mm0, [esi]
	  paddusb mm0, [edi]
	  add edi, 8
	  movq [esi], mm0
	  add esi, 8
	  dec ecx
      jnz inner

	  add esi, eax
	  dec ebx
	  jnz cicloX
	  emms
   }
}


void AD_Sprite::put_add(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     put_add(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);
   put_add(xx, yy, imagebuf);
}



void AD_Sprite::put_add_clip(int x, int y, imagebuffer *imagebuf)
{
   int xs, ys, u, v;
   int x2, y2;
   int offset, incrow;
   int tex_offset, tex_incrow;
   unsigned char r, g, b;


   u=v=0;
   x2=x+dimx;
   y2=y+dimy;

   // CIPPING 2D nelle x
   if (x>=imagebuf->width) return;
   if (x<0)
   {
	 u=(-x);
	 x=0;
   }
   if (x2>=imagebuf->width) x2=imagebuf->width;


   // CIPPING 2D nelle y
   if (y>=imagebuf->height) return;
   if (y<0)
   {
	 v=(-y);
	 y=0;
   }
   if (y2>=imagebuf->height) y2=imagebuf->height;


   // setup sezione video
   offset=imagebuf->muly[y]+x;
   incrow=imagebuf->width - (x2-x);

   // setup sezione sprite
   tex_offset=v*dimx+u;
   tex_incrow=dimx - (x2-x);

   for (ys=y; ys<y2; ys++)
   {
	 for(xs=x; xs<x2; xs++)
	 {
       b=imagebuf->uchar8ptr[offset*4];
       g=imagebuf->uchar8ptr[offset*4+1];
       r=imagebuf->uchar8ptr[offset*4+2];

	   b=min(b + map[tex_offset*4]     , 255);
	   g=min(g + map[tex_offset*4 +1]  , 255);
	   r=min(r + map[tex_offset*4 +2]  , 255);

       imagebuf->uchar8ptr[offset*4]=b;
       imagebuf->uchar8ptr[offset*4+1]=g;
       imagebuf->uchar8ptr[offset*4+2]=r;

	   offset++;
	   tex_offset++;
	 }
	 offset+=incrow;
	 tex_offset+=tex_incrow;
   }
}


void AD_Sprite::put_add_clip(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     put_blend50_clip(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);
   put_add_clip(xx, yy, imagebuf);
}









// ########################################################
// ###############   VERSIONI CON SCALING   ###############
// ########################################################

void AD_Sprite::put_scaled(int x1, int y1, int x2, int y2, imagebuffer *imagebuf)
{
   int xs, ys;
   unsigned int offset, incrow, tex_offset;
   unsigned int du, dv, v;

   du=(dimx << 16)/(x2-x1);
   dv=(dimy << 16)/(y2-y1);
   v=0;

   offset=imagebuf->muly[y1]+x1;
   incrow=imagebuf->width - (x2-x1);
   tex_offset=0;

   for (ys=y1; ys<y2; ys++)
   {
	 for(xs=x1; xs<x2; xs++)
	 {
	   imagebuf->uint32ptr[offset]=map32[(tex_offset >> 16)];
	   offset++;
	   tex_offset+=du;
	 }
	 offset+=incrow;
	 v+=dv;
	 tex_offset=((v >> 16)*dimx)<<16;
   }
}


void AD_Sprite::put_scaled(float pos, imagebuffer *imagebuf)
{
   int i;
   int lx, ly, xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   ly=myfist(((1.0f-t)*keys[i-1].scale_y + t*keys[i].scale_y)*dimy);
   lx=myfist(((1.0f-t)*keys[i-1].scale_x + t*keys[i].scale_x)*dimx);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   put_scaled(xx, yy, xx+lx, yy+ly, imagebuf);
}


// disegna la bitmap scalata effettuando il clipping 2D
void AD_Sprite::put_scaled_clip(int x1, int y1, int x2, int y2, imagebuffer *imagebuf)
{
   int xs, ys;
   unsigned int offset, incrow, tex_offset, tex_offset0;
   unsigned int du, dv, v, u;


   du=(dimx << 16)/(x2-x1);
   dv=(dimy << 16)/(y2-y1);
   u=v=0;

   // CIPPING 2D nelle x
   if (x1>=imagebuf->width) return;
   if (x1<0)
   {
	 u=(-x1)*du;
	 x1=0;
   }
   if (x2>=imagebuf->width) x2=imagebuf->width;


   // CIPPING 2D nelle y
   if (y1>=imagebuf->height) return;
   if (y1<0)
   {
	 v=(-y1)*dv;
	 y1=0;
   }
   if (y2>=imagebuf->height) y2=imagebuf->height;


   // setup sezione video
   offset=imagebuf->muly[y1]+x1;
   incrow=imagebuf->width - (x2-x1);

   // setup sezione sprite
   tex_offset0=(((v>>16)*dimx)<<16) + u;

   for (ys=y1; ys<y2; ys++)
   {
	 for(xs=x1; xs<x2; xs++)
	 {
	   imagebuf->uint32ptr[offset]=map32[(tex_offset >> 16)];
	   offset++;
	   tex_offset+=du;
	 }
	 offset+=incrow;
	 v+=dv;
	 tex_offset=(((v >> 16)*dimx)<<16) + u;
   }
}


void AD_Sprite::put_scaled_clip(float pos, imagebuffer *imagebuf)
{
   int i;
   int lx, ly, xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   ly=myfist(((1.0f-t)*keys[i-1].scale_y + t*keys[i].scale_y)*dimy);
   lx=myfist(((1.0f-t)*keys[i-1].scale_x + t*keys[i].scale_x)*dimx);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   put_scaled_clip(xx, yy, xx+lx, yy+ly, imagebuf);
}


void AD_Sprite::put_scaled_add_clip(int x1, int y1, int x2, int y2, imagebuffer *imagebuf)
{
   int xs, ys;
   unsigned int offset, incrow, tex_offset, tex_offset0;
   unsigned int du, dv, v, u;
   unsigned char r, g, b;


   du=(dimx << 16)/(x2-x1);
   dv=(dimy << 16)/(y2-y1);
   u=v=0;

   // CIPPING 2D nelle x
   if (x1>=imagebuf->width) return;
   if (x1<0)
   {
	 u=(-x1)*du;
	 x1=0;
   }
   if (x2>=imagebuf->width)	x2=imagebuf->width;



   // CIPPING 2D nelle y
   if (y1>=imagebuf->height) return;
   if (y1<0)
   {
	 v=(-y1)*dv;
	 y1=0;
   }
   if (y2>=imagebuf->height) y2=imagebuf->height;


   // setup sezione video
   offset=imagebuf->muly[y1]+x1;
   incrow=imagebuf->width - (x2-x1);

   // setup sezione sprite
   tex_offset0=(((v>>16)*dimx)<<16) + u;

   for (ys=y1; ys<y2; ys++)
   {
	 for(xs=x1; xs<x2; xs++)
	 {
       b=imagebuf->uchar8ptr[offset*4];
       g=imagebuf->uchar8ptr[offset*4+1];
       r=imagebuf->uchar8ptr[offset*4+2];

	   b=min(b + map[(tex_offset >> 16)*4]     , 255);
	   g=min(g + map[(tex_offset >> 16)*4 +1]  , 255);
	   r=min(r + map[(tex_offset >> 16)*4 +2]  , 255);

       imagebuf->uchar8ptr[offset*4]=b;
       imagebuf->uchar8ptr[offset*4+1]=g;
       imagebuf->uchar8ptr[offset*4+2]=r;

	   offset++;
	   tex_offset+=du;
	 }
	 offset+=incrow;
	 v+=dv;
	 tex_offset=(((v >> 16)*dimx)<<16) + u;
   }
}


void AD_Sprite::put_scaled_add_clip(float pos, imagebuffer *imagebuf)
{
   int i;
   int lx, ly, xx, yy;
   float t;

   if (num_keys<=0) return;
   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
   {
	 i++;
   }
   if (i==0) i=1;
   
   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   ly=myfist(((1.0f-t)*keys[i-1].scale_y + t*keys[i].scale_y)*dimy);
   lx=myfist(((1.0f-t)*keys[i-1].scale_x + t*keys[i].scale_x)*dimx);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   put_scaled_add_clip(xx, yy, xx+lx, yy+ly, imagebuf);
}


void AD_Sprite::init()
{
}


void AD_Sprite::do_efx2D(double pos)
{
  float current_pos;

  // consideriamo la pos nel range [0;1]
  current_pos=(float)((pos-fstartpos)/(fendpos-fstartpos));

  switch (type & RENDER_SCALING)
  {
    case RENDER_SCALING: 
		switch (type & RENDER_CLIP)
		{
		  case RENDER_CLIP:
			  if (type & RENDER_ADD)
				  put_scaled_add_clip(current_pos, image);
              else
				  if (type & RENDER_BLEND50) {}
//                  put_scaled_blend50_clip(current_pos, image);
              else
			  if (type & RENDER_OVERWRITE)
                  put_scaled_clip(current_pos, image);
			  break;

          case 0:
			  if (type & RENDER_ADD) {}
//				  put_scaled_add(current_pos, image);
              else
			  if (type & RENDER_BLEND50) {}
//				  put_scaled_blend50(current_pos, image);
              else
			  if (type & RENDER_OVERWRITE)
				  put_scaled(current_pos, image);
			  break;
		}
		break;


	case 0:
		switch (type & RENDER_CLIP)
		{
		  case RENDER_CLIP:
			  if (type & RENDER_ADD)
				  put_add_clip(current_pos, image);
              else
			  if (type & RENDER_BLEND50)
                  put_blend50_clip(current_pos, image);
              else
			  if (type & RENDER_OVERWRITE)
                  put_clip(current_pos, image);
			  break;

          case 0:
			  if (type & RENDER_ADD)
				  put_add(current_pos, image);
              else
			  if (type & RENDER_BLEND50)
				  put_blend50(current_pos, image);
              else
			  if (type & RENDER_OVERWRITE)
				  put(current_pos, image);
			  break;
		}
		break;
  }
}
