#include "sprite.h"
#include "fastmath.h"
#include "shared.h"
#include "ADbase.h"

extern HWND miawin;			// finestra principale

AD_Sprite::AD_Sprite()
{
//  1----2
//  |    |
//  |    |
//  3----4

  int w;

  num_keys=num_tkeys=0;
  for (w=0; w<MAXKEY; w++)
  {
	keys[w].relpos=tkeys[w].relpos=0;
    keys[w].x=keys[w].y=0;
    keys[w].scale_x=keys[w].scale_y=NOSCALING;
	tkeys[w].tlevel=255;
  }
  use_tmask=0;  // non usare maschera di trasparenza
  type=0 | RENDER_NOSCALING | RENDER_CLIP | RENDER_OVERWRITE;
}


void AD_Sprite::load_transparency_map(char *nomefileJPEG, BOOL needinvert)
{
   texture *t;
   int i;
   unsigned char r, g, b;
   unsigned int sum;

   if (map==(unsigned char *)NULL) debug_error(miawin, "load_transparency_map must be called after load_jpg");
   t=new texture;
   if (t->load_jpg(nomefileJPEG, RGBINTERLEAVED)==FILENOTFOUND)
	   debug_error(miawin, "load_transparency_map: file not found");
   
   if ((dimx!=t->dimx) || (dimy!=t->dimy))
	   debug_error(miawin, "load_transparency_map: transparency map has different resolution of sprite");

   if (!needinvert)
   // copio le informazioni di trasparenza nella texture
   for (i=0; i<channelsize; i++)
   {
	  b=t->map[i*4];
	  g=t->map[i*4+1];
	  r=t->map[i*4+2];
	  sum=r+b+g;
	  if (sum<50)
		  map32[i]&=0x00FFFFFF;
	  else 
		  map32[i]|=0xFF000000;
   }
   else
   for (i=0; i<channelsize; i++)
   {
	  b=t->map[i*4];
	  g=t->map[i*4+1];
	  r=t->map[i*4+2];
	  sum=r+b+g;
	  if (sum>=240*3)
		  map32[i]&=0x00FFFFFF;
	  else
		  map32[i]|=0xFF000000;
   }

   use_tmask=1;
   delete t;
}


void AD_Sprite::add_key(float relpos, float x, float y, float scale_x, float scale_y)
{
  if (num_keys>MAXKEY) return;

  if (relpos<0) relpos=0;
  if (relpos>1) relpos=1;

  keys[num_keys].relpos=relpos;
  if (image==NULL) debug_error(miawin, "Sprite: image=NULL, add_key function must be called after set_imagebuffer");
  keys[num_keys].x=x*image->width;
  keys[num_keys].y=y*image->height;

//  if (scale_x<=0) scale_x=NOSCALING;
//  if (scale_y<=0) scale_y=NOSCALING;

  keys[num_keys].scale_x=scale_x*image->width;
  keys[num_keys].scale_y=scale_y*image->height;
  num_keys++;
}


void AD_Sprite::add_tkey(float relpos, float tlevel)
{
  if (num_tkeys>MAXKEY) return;

  if (relpos<0) relpos=0;
  if (relpos>1) relpos=1;
  if (tlevel<0) tlevel=0;
  if (tlevel>255) tlevel=255;

  tkeys[num_tkeys].relpos=relpos;
  if (image==NULL) debug_error(miawin, "Sprite: image=NULL, add_tkey function must be called after set_imagebuffer");
  tkeys[num_tkeys].tlevel=tlevel;
  num_tkeys++;
}


void AD_Sprite::set_render_type(int render_type)
{
  type=render_type;
  if (type & RENDER_ADD)
  {
     mat.flags|=IS_TRASPARENT;
	 mat.trasparency=MIXTYPE_ADD;
  }
  else
  if (type & RENDER_BLEND50)
  {
     mat.flags|=IS_TRASPARENT;
	 mat.trasparency=MIXTYPE_BLEND50;
  }
  else
  if (type & RENDER_OVERWRITE)
  {
     mat.flags&=(!IS_TRASPARENT);
	 mat.trasparency=OFF;
  }
}


// ########################################################
// ##############   VERSIONI SENZA SCALING   ##############
// ########################################################

void AD_Sprite::put(int x, int y, imagebuffer *imagebuf)
{
  DrawTriangleProc drawproc;

  Begin_Scene();
  drawproc=Set_Material(&mat);
  vert[0].xs=(float)x;         vert[0].ys=(float)y;
  vert[1].xs=(float)(x+dimx);  vert[1].ys=(float)y;
  vert[2].xs=(float)x;         vert[2].ys=(float)(y+dimy);
  vert[3].xs=(float)(x+dimx);  vert[3].ys=(float)(y+dimy);
  drawproc(&vert[0], &vert[1], &vert[3]);
  drawproc(&vert[3], &vert[2], &vert[0]);
  End_Scene();
}


void AD_Sprite::put(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
     else
	 {
       i=0;
       while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
       if (i==0) i=1;
       t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
       mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
	 }
     put(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
	 i++;

   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
   else
   {
     i=0;
     while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
     if (i==0) i=1;
     t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
     mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
   }

   put(xx, yy, imagebuf);
}


void AD_Sprite::put_blend50(int x, int y, imagebuffer *imagebuf)
{
  DrawTriangleProc drawproc;

  Begin_Scene();
  Begin_ExclusiveTransparency();
  drawproc=Set_Material(&mat);
  vert[0].xs=(float)x;         vert[0].ys=(float)y;
  vert[1].xs=(float)(x+dimx);  vert[1].ys=(float)y;
  vert[2].xs=(float)x;         vert[2].ys=(float)(y+dimy);
  vert[3].xs=(float)(x+dimx);  vert[3].ys=(float)(y+dimy);
  drawproc(&vert[0], &vert[1], &vert[3]);
  drawproc(&vert[3], &vert[2], &vert[0]);
  End_Scene();
  End_ExclusiveTransparency();
}


void AD_Sprite::put_blend50(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
     else
	 {
       i=0;
       while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
       if (i==0) i=1;
       t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
       mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
	 }
     put_blend50(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
	 i++;

   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
   else
   {
     i=0;
     while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
     if (i==0) i=1;
     t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
     mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
   }
   put_blend50(xx, yy, imagebuf);
}


void AD_Sprite::put_add(int x, int y, imagebuffer *imagebuf)
{
  DrawTriangleProc drawproc;

  Begin_Scene();
  Begin_ExclusiveTransparency();
  drawproc=Set_Material(&mat);
  vert[0].xs=(float)x;         vert[0].ys=(float)y;
  vert[1].xs=(float)(x+dimx);  vert[1].ys=(float)y;
  vert[2].xs=(float)x;         vert[2].ys=(float)(y+dimy);
  vert[3].xs=(float)(x+dimx);  vert[3].ys=(float)(y+dimy);
  drawproc(&vert[0], &vert[1], &vert[3]);
  drawproc(&vert[3], &vert[2], &vert[0]);
  End_Scene();
  End_ExclusiveTransparency();
}


void AD_Sprite::put_add(float pos, imagebuffer *imagebuf)
{
   int i;
   int xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
     else
	 {
       i=0;
       while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
       if (i==0) i=1;
       t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
       mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
	 }
     put_add(myfist(keys[0].x), myfist(keys[0].y), imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
	 i++;

   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
   else
   {
     i=0;
     while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
     if (i==0) i=1;
     t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
     mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
   }
   put_add(xx, yy, imagebuf);
}



// ########################################################
// ###############   VERSIONI CON SCALING   ###############
// ########################################################

void AD_Sprite::put_scaled(int x1, int y1, int x2, int y2, imagebuffer *imagebuf)
{
  DrawTriangleProc drawproc;

  Begin_Scene();
  drawproc=Set_Material(&mat);
  vert[0].xs=(float)x1;        vert[0].ys=(float)y1;
  vert[1].xs=(float)x2;        vert[1].ys=(float)y1;
  vert[2].xs=(float)x1;        vert[2].ys=(float)y2;
  vert[3].xs=(float)x2;        vert[3].ys=(float)y2;
  drawproc(&vert[0], &vert[1], &vert[3]);
  drawproc(&vert[3], &vert[2], &vert[0]);
  End_Scene();
}


void AD_Sprite::put_scaled(float pos, imagebuffer *imagebuf)
{
   int i;
   int lx, ly, xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
      if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
      else
	  {
        i=0;
        while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
        if (i==0) i=1;
        t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
        mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
	  }
	  put_scaled(myfist(keys[0].x), myfist(keys[0].y),
                 myfist(keys[0].scale_x), myfist(keys[0].scale_y),
				 imagebuf);
	  return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
	 i++;

   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   ly=myfist(((1.0f-t)*keys[i-1].scale_y + t*keys[i].scale_y));
   lx=myfist(((1.0f-t)*keys[i-1].scale_x + t*keys[i].scale_x));
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
   else
   {
     i=0;
     while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
     if (i==0) i=1;
     t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
     mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
   }
   put_scaled(xx, yy, lx, ly, imagebuf);
}


void AD_Sprite::put_scaled_add(int x1, int y1, int x2, int y2, imagebuffer *imagebuf)
{
  DrawTriangleProc drawproc;

  Begin_Scene();
  Begin_ExclusiveTransparency();
  drawproc=Set_Material(&mat);
  vert[0].xs=(float)x1;        vert[0].ys=(float)y1;
  vert[1].xs=(float)x2;        vert[1].ys=(float)y1;
  vert[2].xs=(float)x1;        vert[2].ys=(float)y2;
  vert[3].xs=(float)x2;        vert[3].ys=(float)y2;
  drawproc(&vert[0], &vert[1], &vert[3]);
  drawproc(&vert[3], &vert[2], &vert[0]);
  End_Scene();
  End_ExclusiveTransparency();
}


void AD_Sprite::put_scaled_add(float pos, imagebuffer *imagebuf)
{
   int i;
   int lx, ly, xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
	 if (num_tkeys<1) goto putta;
     if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
     else
	 {
       i=0;
       while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
       if (i==0) i=1;
       t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
       mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
	 }
putta:
;
	 put_scaled_add(myfist(keys[0].x), myfist(keys[0].y),
	                myfist(keys[0].scale_x), myfist(keys[0].scale_y),
                    imagebuf);
     return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
	 i++;

   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   lx=myfist(((1.0f-t)*keys[i-1].scale_x + t*keys[i].scale_x));
   ly=myfist(((1.0f-t)*keys[i-1].scale_y + t*keys[i].scale_y));
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   if (num_tkeys<1) goto putta2;
   if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
   else
   {
     i=0;
     while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
     if (i==0) i=1;
     t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
     mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
   }
putta2:
;
   put_scaled_add(xx, yy, lx, ly, imagebuf);
}


void AD_Sprite::put_scaled_blend50(int x1, int y1, int x2, int y2, imagebuffer *imagebuf)
{
  DrawTriangleProc drawproc;

  Begin_Scene();
  Begin_ExclusiveTransparency();
  drawproc=Set_Material(&mat);
  vert[0].xs=(float)x1;        vert[0].ys=(float)y1;
  vert[1].xs=(float)x2;        vert[1].ys=(float)y1;
  vert[2].xs=(float)x1;        vert[2].ys=(float)y2;
  vert[3].xs=(float)x2;        vert[3].ys=(float)y2;
  drawproc(&vert[0], &vert[1], &vert[3]);
  drawproc(&vert[3], &vert[2], &vert[0]);
  End_Scene();
  End_ExclusiveTransparency();
}


void AD_Sprite::put_scaled_blend50(float pos, imagebuffer *imagebuf)
{
   int i;
   int lx, ly, xx, yy;
   float t;

   if (num_keys<0) return;
   if (num_keys==1)
   {
     if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
     else
	 {
       i=0;
       while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
       if (i==0) i=1;
       t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
       mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
	 }
     put_scaled_blend50(myfist(keys[0].x), myfist(keys[0].y),
		                myfist(keys[0].scale_x), myfist(keys[0].scale_y),
				        imagebuf);
	 return;
   }

   i=0;
   while ((i<num_keys) && (keys[i].relpos<pos))
	 i++;

   if (i==0) i=1;

   // interpolazioni lineari tra le key
   t=(pos - keys[i-1].relpos)/(keys[i].relpos - keys[i-1].relpos);
   ly=myfist(((1.0f-t)*keys[i-1].scale_y + t*keys[i].scale_y));
   lx=myfist(((1.0f-t)*keys[i-1].scale_x + t*keys[i].scale_x));
   xx=myfist((1.0f-t)*keys[i-1].x + t*keys[i].x);
   yy=myfist((1.0f-t)*keys[i-1].y + t*keys[i].y);

   if (num_tkeys==1) mat.trasparency=myfist(tkeys[0].tlevel);
   else
   {
     i=0;
     while ((i<num_tkeys) && (tkeys[i].relpos<pos)) i++;
     if (i==0) i=1;
     t=(pos - tkeys[i-1].relpos)/(tkeys[i].relpos - tkeys[i-1].relpos);
     mat.trasparency=myfist((1.0f-t)*tkeys[i-1].tlevel + t*tkeys[i].tlevel);
   }
   put_scaled_blend50(xx, yy, lx, ly, imagebuf);
}



//************************************************************
//************************************************************
void AD_Sprite::init()
{
   vert[0].u=0;
   vert[0].v=0;
   vert[0].dist=1.0f;
   vert[0].z=0.00000001f;

   vert[1].u=0.999f;
   vert[1].v=0;
   vert[1].dist=1.0f;
   vert[1].z=0.00000001f;

   vert[2].u=0;
   vert[2].v=0.999f;
   vert[2].dist=1.0f;
   vert[2].z=0.00000001f;

   vert[3].u=0.999f;
   vert[3].v=0.999f;
   vert[3].dist=1.0f;
   vert[3].z=0.00000001f;

   if (map!=(unsigned char *)NULL)
   {
	  mat.texture_pos=0;
	  mat.texture_ptr=(texture *)this;
	  if (use_tmask==0)
	  {
	     AddUpdate_Texture(mat.texture_ptr);
	     AddUpdate_Material(&mat);
	  }
	  else
	  {
		int type2=0;
		type2|=RENDER_ADD;  // cosi' faccio in modo che chiami
		                    // la set_ExclusiveTransparency
		if (type & RENDER_SCALING)
			type2|=RENDER_SCALING;
        // non devo settare i flag del materiale perche' le
		// chiamate di AddUpdate_Alphaxxxx settano le
		// uniche chiamate giuste
		AddUpdate_AlphaTexture(mat.texture_ptr);
	    AddUpdate_AlphaMaterial(&mat);
	  }
   }
}


void AD_Sprite::do_efx2D(double pos)
{
  float current_pos;

  // consideriamo la pos nel range [0;1]
  current_pos=(float)((pos-fstartpos)/(fendpos-fstartpos));

  switch (type & RENDER_SCALING)
  {
    case RENDER_SCALING: 
		  if (type & RENDER_ADD)
				  put_scaled_add(current_pos, image);
          else
		  if (type & RENDER_BLEND50)
                  put_scaled_blend50(current_pos, image);
          else
		  if (type & RENDER_OVERWRITE)
                  put_scaled(current_pos, image);
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
}
