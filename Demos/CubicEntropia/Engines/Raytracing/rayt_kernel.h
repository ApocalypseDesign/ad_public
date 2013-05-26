#ifndef _RAYTKERNEL_H_
#define _RAYTKERNEL_H_

#include "rayt_object.h"

inline void ReflectedRay (AD_Vect3D *dir, AD_Vect3D *normal, AD_Vect3D *reflected)
{
   float k;

   k=-2.0f*vect_dot(normal, dir);
   vect_scale(normal, k, reflected);
   vect_add(reflected, dir, reflected);
}

#endif