//  Disegno raster dei triangoli

#ifndef _RASTERS_H_
#define _RASTERS_H_

#include <math.h>
#include "..\ADrender.h"

// restituita quando il materiale non e' valido; tale proc non
// disegna niente
void tria_raster_NULL(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

void tria_raster_flat_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_RGB_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_Ptexture_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texRGB_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_envRGB_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texenv_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texenv_RGB_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);


// ************************************************************
// RASTER OTTIMIZZATI (sono quelli per cui esistono le scanline
// fate in assembly; prima o poi vanno fatti tutti
// ************************************************************
void tria_raster_RGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// solo texture (con trasparenza anche)
void tria_raster_texture(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texture_tADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texture_tBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texture_tBLENDalpha(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// texture + luci
void tria_raster_texture_lADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_texture_lBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// solo envmap (con trasparenza anche)
void tria_raster_envmap(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_envmap_tADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_envmap_tBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_envmap_tBLENDalpha(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// envmap + luci
void tria_raster_envmap_lADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
void tria_raster_envmap_lBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// texture + envmap
void tria_raster_texenv_mBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

// texture + envmap + luci
void tria_raster_texenv_mBLEND50_lADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);

#endif