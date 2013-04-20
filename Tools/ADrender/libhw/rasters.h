//  Disegno raster dei triangoli

#ifndef _RASTERS_H_
#define _RASTERS_H_

#include <math.h>
#include "..\ADrender.h"


// restituita quando il materiale non e' valido; tale proc non
// disegna niente
void tria_raster_NULL(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_RGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// solo texture
void tria_raster_texture(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texRGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// solo envmap
void tria_raster_envmap(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_envRGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// texture + envmap
void tria_raster_texenv(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texenv_RGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

#endif