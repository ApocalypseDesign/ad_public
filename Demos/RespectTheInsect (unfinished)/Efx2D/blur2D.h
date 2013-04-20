#ifndef _BLUR2D_H_
#define _BLUR2D_H_

#include <math.h>
#include "..\efx2D.h"

class blur2D : public efx2D
{
  private:
	void calc_tables(float xcenter, float ycenter, float zoom_);

  public:
	int *xtab;
	int *ytab;
	float xc, yc, zoom;

	blur2D();       // costruttore
	void set_parameters(float xcenter, float ycenter, float zoom_);
	void init();
	void do_efx2D(double pos);
};


class zblur2D : public efx2D
{
  private:

  public:
	imagebuffer *in_ib;
	int *xtab;
	int *ytab;
	float fact;

	zblur2D();       // costruttore
	void init();
	void set_imagebuffer_in(imagebuffer *ib);
	void do_efx2D(double pos);
};

#endif