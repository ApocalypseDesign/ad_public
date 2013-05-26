#include <math.h>
#include "quadro2D.h"
#include "..\utility.h"
#include "painter2D.h"


extern HWND miawin;  // finestra principale; usata qui per le debug_error
painter2D quadro_painter;


quadro2D::quadro2D()
{
   rotaz_verso=QUADRO_ORARIO;
}


void quadro2D::set_parameters(char *file_img, int xfinale, int yfinale,
		                      int ang_iniziale, int step,
							  float rel_split_efx, int orario_antiorario)
{
  if (pic.load(file_img, RGBINTERLEAVED)==FILENOTFOUND)
	debug_error(miawin, "quadro2D set_parameteres: picture file not found!");

  // 0 --- 1
  // |     |
  // |     |
  // 3 --- 2
  xp[0]=-pic.dimx/2.0f;
  xp[1]=pic.dimx/2.0f;
  xp[2]=pic.dimx/2.0f;
  xp[3]=-pic.dimx/2.0f;
  yp[0]=pic.dimy/2.0f;
  yp[1]=pic.dimy/2.0f;
  yp[2]=-pic.dimy/2.0f;
  yp[3]=-pic.dimy/2.0f;

  ang0=ang_iniziale;
  step0=step;
  xf=xfinale;
  yf=yfinale;
  rel_split=rel_split_efx;
  rotaz_verso=orario_antiorario;
}


void quadro2D::init()
{
  if (image==(imagebuffer *)NULL)
     debug_error(miawin, "quadro2D: init() error: imagebuffer not assigned");
}


void quadro2D::draw_quadro(float p, unsigned char luminosita)
{
  // p va da 0 a 1
  float ang_cur, c, s;
  int i;

  if (rotaz_verso==QUADRO_ORARIO) ang_cur=(float)(((1.0f-p)*ang0)*M_PI/180.0f);
  else ang_cur=-(float)(((1.0f-p)*ang0)*M_PI/180.0f);

  c=cosf(ang_cur);
  s=sinf(ang_cur);
  for (i=0; i<4; i++)
  {
     xp_tr[i]=c*xp[i] + s*yp[i] + xf;
     yp_tr[i]=-s*xp[i] + c*yp[i] + yf;
  }
  quadro_painter.draw_aaline_add(xp_tr[0], yp_tr[0],
                             xp_tr[1], yp_tr[1],
							 luminosita, luminosita, luminosita, image);
  quadro_painter.draw_aaline_add(xp_tr[1], yp_tr[1],
                             xp_tr[2], yp_tr[2],
							 luminosita, luminosita, luminosita, image);
  quadro_painter.draw_aaline_add(xp_tr[2], yp_tr[2],
                             xp_tr[3], yp_tr[3],
							 luminosita, luminosita, luminosita, image);

  quadro_painter.draw_aaline_add(xp_tr[3], yp_tr[3],
                             xp_tr[0], yp_tr[0],
							 luminosita, luminosita, luminosita, image);
}


void quadro2D::draw_pic(float p)
{
   int xs, ys, offset, incrow;
   unsigned int *image32, *tex32, col;
   unsigned char lum;
   float rp;
   
   #define FLASH_DELTA 0.025f

   // rp va da 0 a 1
   rp=(p-rel_split)/(1.0f-rel_split);

   if ((rp < FLASH_DELTA) || (rp > 1.0f-FLASH_DELTA))
   {
	  // rp da 0 a 1 nel flash
      if (rp < FLASH_DELTA) rp=(rp-FLASH_DELTA)/FLASH_DELTA;
	  else rp=(rp-(1.0f-FLASH_DELTA))/FLASH_DELTA;
      lum=(unsigned char)(rp*255);
      col=lum | (lum<<8) | (lum << 16);
      offset=image->muly[yf-pic.dimy/2]+xf-pic.dimx/2;
      incrow=image->width-pic.dimx;
	  image32=(unsigned int *)(image->uint32ptr + offset);
      for (ys=pic.dimy; ys>0; ys--)
	  {
	     for(xs=pic.dimx; xs>0; xs--)
		 {
			*image32=col;
			image32++;
		 }
		 image32+=incrow;
	  }
   }
   else
   {
      offset=image->muly[yf-pic.dimy/2]+xf-pic.dimx/2;
      incrow=image->width-pic.dimx;
	  image32=(unsigned int *)(image->uint32ptr + offset);
	  tex32=pic.map32;
      for (ys=pic.dimy; ys>0; ys--)
	  {
	     for(xs=pic.dimx; xs>0; xs--)
		 {
			*image32=*tex32;
			image32++;
			tex32++;
		 }
		 image32+=incrow;
	  }
   }
}


void quadro2D::do_efx2D(double pos)
{
  float relpos, pos_per_step, rp;
  int i, j, quanti_quadri;
  unsigned char lum;

  // consideriamo la pos nel range [0;1]
  relpos=(float)((pos-fstartpos)/(fendpos-fstartpos));

  pos_per_step=1.0f/step0;
  if (relpos<=rel_split)
  {
	quanti_quadri=step0/2;
	if (quanti_quadri<=0) quanti_quadri=1;

    rp=relpos/rel_split;
	// rp va da 0 a 1

	i=(int)(ceil(rp/pos_per_step));
	if (i > quanti_quadri)
	{
	  lum=255/quanti_quadri;
	  for (j=0; j<quanti_quadri; j++)
		draw_quadro((j+i-quanti_quadri)*pos_per_step, lum*j);
	}
	else
	{
	  if (i==0) i=1;
	  lum=255/i;
	  for (j=0; j<i; j++)
		draw_quadro(j*pos_per_step, lum*j);
	}

    draw_quadro(rp, 255);
  }
  else draw_pic(relpos);
}
