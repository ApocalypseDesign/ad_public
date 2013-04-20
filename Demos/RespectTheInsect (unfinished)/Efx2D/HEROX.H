#ifndef _HEROX_H_
#define _HEROX_H_

#define HEROX_PARTICLE_SIZE 64
#define HEROX_INTENSITY 180.0f
#define HEROX_SKIP 16
#define HEROX_RANDOMNESS 0.0f
#define HEROX_TRESHOLD_R 250
#define HEROX_TRESHOLD_G 250
#define HEROX_TRESHOLD_B 250

#include "..\efx2D.h"

class herox : public efx2D
{
	unsigned char *particle_map;
	//char *particle_displace;
	void paint_particle(int x,int y);

public:

	void init();
	void do_efx2D(double pos);
};

#endif