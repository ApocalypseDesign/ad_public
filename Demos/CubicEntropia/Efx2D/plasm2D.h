
#ifndef _PLASM2D_H_
#define _PLASM2D_H_

#include <math.h>

#include "..\efx2D.h"

class plasm2D : public efx2D
{
  private:
	int *kpls1, *kpls2;
	int *pls1, *pls2;
    int k1, k2;
	unsigned int rblur, gblur, bblur;
	unsigned int rblur_shift, gblur_shift, bblur_shift;
	double oldpos, frazpos;

    void draw_blur(double pos);
    void draw_noblur(double pos);

  public:

	plasm2D();
	void init_color_table(void);
	void init_look_table(void);
	void set_blur (unsigned int rb, unsigned int gb, unsigned int bb);
	void init();
	void do_efx2D(double pos);
};

#endif