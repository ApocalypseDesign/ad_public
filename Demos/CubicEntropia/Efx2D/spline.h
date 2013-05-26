#ifndef _SPLINE_H_
#define _SPLINE_H_

#include <math.h>

// Le spline generate sono di terzo grado e vettore dei nodi
// uniforme

#define MAXKEYS 50

typedef struct
{
  float x, y;
} key2D;

typedef struct
{
  float x, y, z;
} key3D;


class spline1D
{
  private:

  public:
	float *keys, *keys_tr;
	float *der, *der_tr;
	int nkeys;

	spline1D();
	void add_key(float k);
	void init();
	void evaluate(float pos, float *result);
};


class spline2D
{
  private:

  public:
	key2D *keys, *keys_tr;
	key2D *der, *der_tr;
	int nkeys;

	spline2D();
	void add_key(float x, float y);
	void init();
	void evaluate(float pos, key2D *result);
	void rotate_key (float x, float y, float deg);
};


class spline3D
{
  private:

  public:
	key3D *keys, *keys_tr;
	key3D *der, *der_tr;
	int nkeys;

	spline3D();
	void add_key(float x, float y, float z);
	void init();
	void evaluate(float pos, key3D *result);
};

#endif