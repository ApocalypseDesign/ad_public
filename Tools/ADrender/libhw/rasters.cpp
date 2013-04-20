#include <stdio.h>
#include <math.h>
#include "..\fastmath.h"
#include "rasters.h"
#include "extrend.h"

unsigned int current_transparency;
extern FILE *fdebug;

#define MAKE_RGB_VERTEX(sorg, dest) \
	dest.x=sorg->xs; \
	dest.y=sorg->ys; \
	dest.z=sorg->z; \
	dest.w=sorg->dist; \
	dest.ARGB=current_transparency | (myfist(sorg->R) << 16) | (myfist(sorg->G) << 8) | myfist(sorg->B); \

// *******************************************************************
#define MAKE_TEX_VERTEX(sorg, dest) \
	dest.x=sorg->xs; \
	dest.y=sorg->ys; \
	dest.z=sorg->z; \
	dest.w=sorg->dist; \
    dest.ARGB=current_transparency; \
	dest.u=sorg->u; \
	dest.v=sorg->v; \

#define MAKE_TEXRGB_VERTEX(sorg, dest) \
	dest.x=sorg->xs; \
	dest.y=sorg->ys; \
	dest.z=sorg->z; \
	dest.w=sorg->dist; \
	dest.ARGB=current_transparency | (myfist(sorg->R) << 16) | (myfist(sorg->G) << 8) | myfist(sorg->B); \
	dest.u=sorg->u; \
	dest.v=sorg->v; \
// *******************************************************************
#define MAKE_ENV_VERTEX(sorg, dest) \
	dest.x=sorg->xs; \
	dest.y=sorg->ys; \
	dest.z=sorg->z; \
	dest.w=sorg->dist; \
    dest.ARGB=current_transparency; \
	dest.u=sorg->envu; \
	dest.v=sorg->envv; \

#define MAKE_ENVRGB_VERTEX(sorg, dest) \
	dest.x=sorg->xs; \
	dest.y=sorg->ys; \
	dest.z=sorg->z; \
	dest.w=sorg->dist; \
	dest.ARGB=current_transparency | (myfist(sorg->R) << 16) | (myfist(sorg->G) << 8) | myfist(sorg->B); \
	dest.u=sorg->envu; \
	dest.v=sorg->envv; \
// *******************************************************************
#define MAKE_TEXENV_VERTEX(sorg, dest) \
	dest.x=sorg->xs; \
	dest.y=sorg->ys; \
	dest.z=sorg->z; \
	dest.w=sorg->dist; \
	dest.ARGB=current_transparency; \
	dest.u=sorg->u; \
	dest.v=sorg->v; \
	dest.envu=sorg->envu; \
	dest.envv=sorg->envv; \

#define MAKE_TEXENVRGB_VERTEX(sorg, dest) \
	dest.x=sorg->xs; \
	dest.y=sorg->ys; \
	dest.z=sorg->z; \
	dest.w=sorg->dist; \
	dest.u=sorg->u; \
	dest.v=sorg->v; \
	dest.envu=sorg->envu; \
	dest.envv=sorg->envv; \
	dest.ARGB=current_transparency | (myfist(sorg->R) << 16) | (myfist(sorg->G) << 8) | myfist(sorg->B); \
// *******************************************************************

// ####################################################################
// ####### PROCEDURE DI DISEGNO DEI TRIANGOLI (rasterizzatori) ########
// ####################################################################

// restituita quando il materiale non e' valido; tale proc non
// disegna niente
void tria_raster_NULL(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  return;
}


void tria_raster_RGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  	MAKE_RGB_VERTEX(v1, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_RGB_VERTEX(v2, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_RGB_VERTEX(v3, HW_texrgb[HW_numvert])
    HW_numvert++;
}



// ***********************************************************
// ************************  TEXTURE  ************************
// ***********************************************************

void tria_raster_texture(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
	MAKE_TEX_VERTEX(v1, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_TEX_VERTEX(v2, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_TEX_VERTEX(v3, HW_texrgb[HW_numvert])
    HW_numvert++;
}


void tria_raster_texRGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
	MAKE_TEXRGB_VERTEX(v1, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_TEXRGB_VERTEX(v2, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_TEXRGB_VERTEX(v3, HW_texrgb[HW_numvert])
    HW_numvert++;
}



// ***********************************************************
// *************************  ENVMAP  ************************
// ***********************************************************

void tria_raster_envmap(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
	MAKE_ENV_VERTEX(v1, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_ENV_VERTEX(v2, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_ENV_VERTEX(v3, HW_texrgb[HW_numvert])
    HW_numvert++;
}


void tria_raster_envRGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
	MAKE_ENVRGB_VERTEX(v1, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_ENVRGB_VERTEX(v2, HW_texrgb[HW_numvert])
    HW_numvert++;
	MAKE_ENVRGB_VERTEX(v3, HW_texrgb[HW_numvert])
    HW_numvert++;
}


// ***********************************************************
// *******************  ENVMAP + TEXTURE  ********************
// ***********************************************************

void tria_raster_texenv(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  MAKE_TEXENV_VERTEX(v1, HW_texenv[HW_numvert])
  HW_numvert++;
  MAKE_TEXENV_VERTEX(v2, HW_texenv[HW_numvert])
  HW_numvert++;
  MAKE_TEXENV_VERTEX(v3, HW_texenv[HW_numvert])
  HW_numvert++;
}



void tria_raster_texenv_RGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  MAKE_TEXENVRGB_VERTEX(v1, HW_texenv[HW_numvert])
  HW_numvert++;
  MAKE_TEXENVRGB_VERTEX(v2, HW_texenv[HW_numvert])
  HW_numvert++;
  MAKE_TEXENVRGB_VERTEX(v3, HW_texenv[HW_numvert])
  HW_numvert++;
}