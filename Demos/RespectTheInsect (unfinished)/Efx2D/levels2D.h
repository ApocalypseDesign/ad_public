#ifndef _LEVELS2D_H_
#define _LEVELS2D_H_

#include "..\efx2D.h"

#define LEVELS_AUTO 0
#define LEVELS_AUTO_LUMINOSITY 1
#define LEVELS_MANUAL 2

class levels2D : public efx2D
{
private:
	int old_r_min,old_r_max,old_g_min,old_g_max,old_b_min,old_b_max;
public:
	int mode;
	int rmin,rmax,gmin,gmax,bmin,bmax;
	float velocity; // da 0.0f a 1.0f
	unsigned char r_lut[256],g_lut[256],b_lut[256];

	levels2D ()
	{
		velocity=1.0f;
	}

	void calc_luts(int r_min,int r_max,int g_min,int g_max,int b_min,int b_max);

	void init();
	void do_efx2D(double pos);
};

#endif