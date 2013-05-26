#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "matrix.h"
#include "RenderLib.h"


typedef struct
{
  int32 i1, i2, i3;
  int32 UV1_i1, UV1_i2, UV1_i3;
  int32 UV2_i1, UV2_i2, UV2_i3;

  BUMPSPACE bumpspace;
  CMaterial *materiale;           // puntatore al materiale      4
  AD_Vect3D normal;              // normale al triangolo       12
  AD_Vect3D *n1, *n2, *n3;       // puntatori alle normali     12
  AD_Vect3D midpoint;
  BUMPSPACE *bump1, *bump2, *bump3;
} AD_Tria3D, *AD_Tria3DPtr;


typedef struct
{
  int32 driverV1, driverV2, driverV3;
} DriverTriangle, *DriverTrianglePtr;

#endif