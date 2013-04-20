
#include "fade2D.h"
#include <math.h>

void fade2D::init()
{ 
  /*int i,col;
  
  for (i=0; i<512; i++)
   {
     col=i;
     if (col<0) col=0;
     if (col>255) col=255;
     flightmap[i]=(unsigned char)(col);
   }*/

   fmat.flags=PAINT_LIGHT;
   fmat.trasparency=OFF;
//   fmat.trasparency=MIXTYPE_ADD;
   fmat.lightmixtype=LIGHTMIXTYPE_BLEND3DS;
   AddUpdate_Material(&fmat);
} 

void fade2D::do_efx2D(double pos)
{
  // v1----v2
  // |      |
  // |      |
  // v3----v4
  DrawTriangleProc drawp;
  float intensity, cpos;
  AD_Vertex2D v1, v2, v3, v4;

  cpos=(float)(3.141592*(pos-fstartpos)/(fendpos-fstartpos));
  intensity=(float)(-255.0f*sinf(cpos)+255.0f);
  if (intensity<0) intensity=0;
  if (intensity>255) intensity=255;

  Begin_Scene();
  Begin_ExclusiveTransparency();

  v1.xs=v1.ys=v1.z=0.00001f; v1.dist=1.0f;
  v2.xs=(float)image->width; v2.ys=0; v2.z=0.00001f; v2.dist=1.0f;
  v3.xs=0; v3.ys=(float)image->height; v3.z=0.00001f; v3.dist=1.0f;
  v4.xs=(float)image->width-1.0f; v4.ys=(float)image->height; v4.z=0.00001f; v4.dist=1.0f;

  if (intensity<0) intensity=0;
  if (intensity>255) intensity=255;

  v1.R=v2.R=v3.R=v4.R=intensity;
  v1.G=v2.G=v3.G=v4.G=intensity;
  v1.B=v2.B=v3.B=v4.B=intensity;

  drawp=Set_Material(&fmat);
  drawp(&v1, &v2, &v4);
  drawp(&v4, &v3, &v1);
  End_Scene();
  End_ExclusiveTransparency();
}
