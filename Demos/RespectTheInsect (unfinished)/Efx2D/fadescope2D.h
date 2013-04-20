#ifndef _FADESCOPE2D_H_
#define _FADESCOPE2D_H_

#include "..\efx2D.h"

#define FADESCOPE2D_SUB 0
#define FADESCOPE2D_ADD 1

class fadescope2D : public efx2D
{
public:
	int intensity,
		delta_intensity,
		mode;

	fadescope2D() 
	{ intensity=250; delta_intensity=6; mode=0; }
	void init();
	void do_efx2D(double pos);
};

#endif