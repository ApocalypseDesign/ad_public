// descrizione: disegnatore di primitive 2D

#ifndef _PAINTER2D_H_
#define _PAINTER2D_H_

#include "ADrender.h"
#include "spline.h"

class painter2D
{
public:
   painter2D();

   // primitive di disegno

   // linea con subpixel
   void draw_line(float x1, float y1, float x2, float y2,
	              unsigned char r, unsigned char g, unsigned char b,
			      imagebuffer *ib);

   // linea anti-aliased con subpixel
   void draw_aaline(float x1, float y1, float x2, float y2,
	                unsigned char r, unsigned char g, unsigned char b,
			        imagebuffer *ib);

   void draw_aaline_add(float x1, float y1, float x2, float y2,
	                unsigned char r, unsigned char g, unsigned char b,
			        imagebuffer *ib);

   void draw_spline2D (spline2D *s2D, int subd, 
	   	               unsigned char r, unsigned char g, unsigned char b,
	                   imagebuffer *ib);

   void draw_aaspline2D (spline2D *s2D, int subd,
	                     unsigned char r, unsigned char g, unsigned char b,	   
	                     imagebuffer *ib);
   void init();
};

#endif