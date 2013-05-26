#ifndef _RAYTMATERIAL_H_
#define _RAYTMATERIAL_H_

#include "ADrender.h"

class rayt_material
{
  public:
     char *name;
     int flags;
     float kdr, ksr;
     float glossiness;
	 float transparency;
     float refraction_index;
     AD_Vect3D diffuse_color, specular_color;
	 AD_Vect3D omni_prec_diffcol[10], omni_prec_speccol[10];
     texture *texture_ptr;

     rayt_material()
	 {
        vect_set(&diffuse_color, 0.1f, 0.5f, 0.8f);
        vect_set(&specular_color, 0.6f, 0.4f, 0.3f);
		kdr=1;
		ksr=0.5;
		glossiness=0.5;
        texture_ptr=(texture *)NULL;
        flags=0;
	 }
};

#endif