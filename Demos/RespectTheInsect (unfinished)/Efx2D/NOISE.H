#ifndef _NOISE_H_
#define _NOISE_H_

#include <math.h>
#include "ADrender.h"
#include "..\efx2D.h"

class noise1D : public efx2D
{
  private:

  public:

	noise1D();
	float Perlin(float x, float persistence, int octaves);
	void init();
	void do_efx2D(double pos);
};


class noise2D : public efx2D
{
  private:
 
  public:

	noise2D();
	float Perlin(float x, float y, float persistence, int octaves);
	void init();
	void do_efx2D(double pos);
};


class noise3D : public efx2D
{
  private:

  public:

	noise3D();
	float Perlin(float x, float y, float z, float persistence, int octaves);
	void init();
	void do_efx2D(double pos);
};

// ************  NUMERI RANDOM NEL RANGE -1;1  ***************

inline float noise1(int x)
{
   x = (x<<13) ^ x;
   return (float)(1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

inline float noise2(int x, int y)
{
	int n;
	float res;

    n = x + y * 57;
    n = (n<<13) ^ n;
    res=(float)((1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0));
	return res;
}

inline float noise3(int x, int y, int z)
{
	int n;
    n = x + y * 57 + z * 1137;
    n = (n<<13) ^ n;
    return (float)(1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

// ************  NUMERI RANDOM NEL RANGE 0;1  ***************
inline float noise1_positive(int x)
{
   float r;

   x = (x<<13) ^ x;
   r=(float)(1.0 - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
   r=(r+1.0f)/2.0f;
   return(r);
}

inline float noise2_positive(int x, int y)
{
   int n;
   float r;

   n = x + y * 57;
   n = (n<<13) ^ n;
   r=(float)((1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0));
   r=(r+1.0f)/2.0f;
   return(r);
}

inline float noise3_positive(int x, int y, int z)
{
   int n;
   float r;
   
   n = x + y * 57 + z * 1137;
   n = (n<<13) ^ n;
   r=(float)(1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
   r=(r+1.0f)/2.0f;
   return(r);
}

#endif