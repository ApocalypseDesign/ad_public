#include "textscroller2D.h"
#include "C64charset.h"
#include "..\utility.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error

TextScroller2D::TextScroller2D()
{
  vertical_rows=-1;
  num_frasi=0;
  screen_y1=screen_y2=-1;
  interline=2;
  interchar=1;
  render_type=FONT_OVERWRITE;
  alignement=FONT_LEFT_ALIGNED;
}


void TextScroller2D::set_parameters(int y1, int y2,
									int interlinea, int intercarattere,
									int rendertype, int allineamento,
									float _stop)
{
   if (y1 < y2)
   {
      screen_y1=y1;
      screen_y2=y2;
   }
   else
   {
      screen_y1=y2;
      screen_y2=y1;
   }
   if (y1==y2) 
	   debug_error(miawin, "TextScroller2D:set_height_parameters, 0 virtual height!");

   interline=interlinea;
   interchar=intercarattere;
   stop=_stop;
   render_type=rendertype;
   if ((render_type<FONT_ADD) || (render_type>FONT_OVERWRITE))
	   render_type=FONT_OVERWRITE;
   alignement=allineamento;
   if ((alignement<FONT_LEFT_ALIGNED) || (alignement>FONT_RIGHT_ALIGNED))
	   alignement=FONT_LEFT_ALIGNED;
}


void TextScroller2D::add_phrase (char *s, int color)
{
   frasi[num_frasi]=new PHRASE;
   strcpy(frasi[num_frasi]->phrase, s);
   frasi[num_frasi]->color=color;
   num_frasi++;
}


void TextScroller2D::init()
{
   int i, l;

   if (image==(imagebuffer *)NULL)
	   debug_error(miawin, "TextScroller2D: init() error: imagebuffer not assigned");

  // significa che non è stata chiamata la set_height_parameters
  // wuindi uso tutto lo schermo
  if ((screen_y1<0) || (screen_y2<0))
  {
	 screen_y1=0;
     screen_y2=image->height-1;
  }

  // precalcola l'altezza virtuale dello schermo formato da tutte
  // le frasi stampate; il *8 e' del fontsize
  vertical_rows=num_frasi*(8+interline);

  // calcola la spaziatura per rispettare l'allinemanto
  switch (alignement)
  {
	 case FONT_LEFT_ALIGNED:
              for (i=0; i<num_frasi; i++)
			  frasi[i]->x_alignement=0;
			  break;

	 case FONT_CENTER_ALIGNED:
              for (i=0; i<num_frasi; i++)
			  {
	             // lunghezza in pixel della frase
				 l=strlen(frasi[i]->phrase)*(8+interchar);
	             frasi[i]->x_alignement=(image->width-l)/2;
			  }
              break;

	 case FONT_RIGHT_ALIGNED:
              for (i=0; i<num_frasi; i++)
			  {
	             // lunghezza in pixel della frase
				 l=strlen(frasi[i]->phrase)*(8+interchar);
	             frasi[i]->x_alignement=image->width-l;
			  }
			  break;
  }
}


void TextScroller2D::put_phrase_line_add (int y, int yy, PHRASE_PTR phrase)
{
   char *font, c;
   unsigned char *s;
   int col, p1,ofs;

   #define PIXEL_LOOP_ADD(maschera) \
   if (c & maschera) { \
			p1=image->uint32ptr[ofs]; \
			image->uint32ptr[ofs]=RGBAdd(p1, col); \
		 } \
         ofs++; \

   s=(unsigned char *)phrase->phrase;
   col=phrase->color;
   font=(char *)(C64font)+yy;
   ofs=image->muly[y]+phrase->x_alignement;

   while (*s != '\0')
   {
	  c=font[(*s) << 3];
      PIXEL_LOOP_ADD(128)
      PIXEL_LOOP_ADD(64)
      PIXEL_LOOP_ADD(32)
      PIXEL_LOOP_ADD(16)
      PIXEL_LOOP_ADD(8)
      PIXEL_LOOP_ADD(4)
      PIXEL_LOOP_ADD(2)
      PIXEL_LOOP_ADD(1)
      ofs+=interchar;
	  s++;
   }
}


void TextScroller2D::put_phrase_line_blend50 (int y, int yy, PHRASE_PTR phrase)
{
   char *font, c;
   unsigned char *s;
   int col, p1, ofs;

   #define PIXEL_LOOP_BLEND50(maschera) \
   if (c & maschera) { \
			p1=(image->uint32ptr[ofs] & 0x00fefefe) >> 1; \
			image->uint32ptr[ofs]=p1+col; \
		 } \
         ofs++; \

   s=(unsigned char *)phrase->phrase;
   col=(phrase->color & 0x00fefefe) >> 1;
   font=(char *)(C64font)+yy;
   ofs=image->muly[y]+phrase->x_alignement;

   while (*s != '\0')
   {
	  c=font[(*s) <<3];
      PIXEL_LOOP_BLEND50(128)
      PIXEL_LOOP_BLEND50(64)
      PIXEL_LOOP_BLEND50(32)
      PIXEL_LOOP_BLEND50(16)
      PIXEL_LOOP_BLEND50(8)
      PIXEL_LOOP_BLEND50(4)
      PIXEL_LOOP_BLEND50(2)
      PIXEL_LOOP_BLEND50(1)
      ofs+=interchar;
	  s++;
   }
}


void TextScroller2D::put_phrase_line_overwrite (int y, int yy, PHRASE_PTR phrase)
{
   char *font, c;
   unsigned char *s;
   int col, ofs;

   #define PIXEL_LOOP_OVERWRITE(maschera) \
	   if (c & maschera) image->uint32ptr[ofs]=col; \
       ofs++; \

   s=(unsigned char *)phrase->phrase;
   col=phrase->color;
   font=(char *)(C64font)+yy;

   ofs=image->muly[y]+phrase->x_alignement;
   while (*s != '\0')
   {
	  c=font[(*s) << 3];
      PIXEL_LOOP_OVERWRITE(128)
      PIXEL_LOOP_OVERWRITE(64)
      PIXEL_LOOP_OVERWRITE(32)
      PIXEL_LOOP_OVERWRITE(16)
      PIXEL_LOOP_OVERWRITE(8)
      PIXEL_LOOP_OVERWRITE(4)
      PIXEL_LOOP_OVERWRITE(2)
      PIXEL_LOOP_OVERWRITE(1)
      ofs+=interchar;
	  s++;
   }
}


void TextScroller2D::do_efx2D(double pos)
{
   int j, i_frase, i_line_char, i_frase_old;
   int v1, v2, sy;
   float relpos, rfrasi;

  if (num_frasi<=0) return;
  // relpos va da 0 a 1 per [fstartpos; fend_pos]
  relpos=(float)((pos-fstartpos)/(fendpos-fstartpos));

  rfrasi=relpos/stop;
  if (rfrasi>1) rfrasi=1;


  //v1=(int)(ceil(relpos*vertical_rows))-(screen_y2-screen_y1);
  v1=(int)(ceil(rfrasi*vertical_rows))-(screen_y2-screen_y1);
  v2=v1+(screen_y2-screen_y1);
  if ((v1<0) && (v2<0)) return;

  sy=screen_y1+1;
  if (v1<0)
  {
	 sy=sy-v1;
	 v1=0;
  }

  i_line_char=v1 % (8+interline);
  i_frase=i_frase_old=(v1/(8+interline)) % num_frasi;
  j=sy;

  switch (render_type)
  {
	 case FONT_ADD: while (j<screen_y2-1)
					{
	                   if (i_line_char<8) put_phrase_line_add(j, i_line_char, frasi[i_frase]);
	                   v1++;
	                   i_line_char=v1 % (8+interline);
	                   i_frase=(v1 / (8+interline)) % num_frasi;
	                   j++;
					}
		            break;

	 case FONT_BLEND50: while (j<screen_y2-1)
						{
	                       if (i_line_char<8) put_phrase_line_blend50(j, i_line_char, frasi[i_frase]);
	                       v1++;
	                       i_line_char=v1 % (8+interline);
	                       i_frase=(v1 / (8+interline)) % num_frasi;
	                       j++;
						}
		                break;

	 case FONT_OVERWRITE: while (j<screen_y2-1)
						  {
	                         if (i_line_char<8) put_phrase_line_overwrite(j, i_line_char, frasi[i_frase]);
	                         v1++;
	                         i_line_char=v1 % (8+interline);
	                         i_frase=(v1 / (8+interline)) % num_frasi;
	                         j++;
						  }
		                  break;
  }
}