#ifndef _WEATHER2D_H_
#define _WEATHER2D_H_

#define PI 3.14156f

#define WEATHER_PARTICLE_SIZE 32
#define WEATHER_PARTICLE_NUM  2200
#define WEATHER_ZONE_NUM	   8
#define WEATHER_INTENSITY 10.0f
#define WEATHER_CORIOLIS_FORCE_INTENSITY 0.3f;

#include "..\efx2D.h"

typedef struct
{
	float pos_x,pos_y,vel_x,vel_y;
	float force,mass,attenuation,iner;
	int life;
} cloud;

typedef struct
{	
	float pos_x,pos_y,force;
} zone; //zone di alta/bassa pressione (force positivo o negativo)

class weather2D : public efx2D
{
	unsigned char *particle_map;
	cloud cl[WEATHER_PARTICLE_NUM];
	zone zo[WEATHER_ZONE_NUM];
	int steps;
	//char *particle_displace;
	void randomize_particle(int num);
	void paint_particle(int x,int y);
	void do_simulation();

public:

	void init();
	void do_efx2D(double pos);
};

#endif