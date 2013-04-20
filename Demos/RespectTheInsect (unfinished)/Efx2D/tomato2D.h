#ifndef _TOMATO2D_H_
#define _TOMATO2D_H_

#include <math.h>
#include "..\efx2D.h"

typedef struct
{
  int left;
  int top;
  int right;
  int bottom;
} rect;


class tomato2D : public efx2D
{
  private:
    imagebuffer *blurred_img;
    imagebuffer *current_img;
	int blocksize, err_limit;

    int  do_rect(imagebuffer *src, imagebuffer *ref, rect *rgn, int *c);
    void draw_circle(int x, int y, int radius, int c, imagebuffer *where);
    void draw_rectangle(int x, int y, int radius, int c, imagebuffer *where);

	void blur_image(imagebuffer *src, imagebuffer *dest);
	void paint_layer(imagebuffer *src, imagebuffer *ref, imagebuffer *dest, int r);

  public:

	tomato2D();       // costruttore
	void init();
	void do_efx2D(double pos);
};

#endif