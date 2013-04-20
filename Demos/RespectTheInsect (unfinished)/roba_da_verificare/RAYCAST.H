#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "efx2D.h"
#include "ADrender.h"
#include "matrix.h"
#include "vect3d.h"

typedef struct
{
  AD_Vect3D origin;
  AD_Vect3D direction;
  AD_Vect3D intersection;
  float t;
  float u, v;
} AD_Ray;


typedef struct
{
  int u, v;    // u e v in fixed point 16:16
} RayUV;


#define X_INTERPOLATION_STEP 8
#define Y_INTERPOLATION_STEP 8
#define X_SHIFT 3     //   2^X_SHIFT = X_INTERPOLATION_STEP
#define Y_SHIFT 3     //   2^Y_SHIFT = Y_INTERPOLATION_STEP


class AD_Raycast : public efx2D
{
public:

 texture mytex;
 RayUV *ray_shooted;
 int nray_x, nray_y;  // dice quanti punti griglia ho
                      // nelle 2 direzioni

 // definizione della quadratica
 AD_Matrix A;
 AD_Vect3D b;
 float c;


 // costruttori
 AD_Raycast ();
 AD_Raycast (float a11, float a22, float a33,
	         float a12, float a13, float a23,
             float b1,  float b2,  float b3,
			 float cc);

 void assign_quadric (float a11, float a22, float a33,
	                  float a12, float a13, float a23,
					  float b1,  float b2,  float b3,
					  float cc);

 void assign_quadric (AD_Matrix *m, AD_Vect3D *v, float cc);

 void trace_one_ray (AD_Ray *r, float k);
 void init();
 void do_efx2D(double pos); // richiama le varie paint(pos) degli oggetti
};

#endif