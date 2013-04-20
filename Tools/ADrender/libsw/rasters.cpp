#include <stdio.h>
#include <math.h>
#include "..\fastmath.h"
#include "rasters.h"
#include "trimacro.h"
#include "extrend.h"


// #################  DATI ESTRATTI DAL TRIANGOLO  ###################
AD_Vertex2D *myv[3];

// ####################################################################
// ####### PROCEDURE DI DISEGNO DEI TRIANGOLI (edge slope+sub) ########
// ####################################################################

#define LEFT_X_SLOPES(a,b) \
    faux=(a->xs - b->xs)*invyl; \
    left_dx=myfist16(faux); \
	left_x=myfist16(b->xs + prestep*faux); \

#define RIGHT_X_SLOPES(a,b) \
	faux=(a->xs - b->xs)*invyr; \
    right_dx=myfist16(faux); \
	right_x=myfist16(b->xs + prestep*faux); \

#define TEXTURE_SLOPES(a,b) \
	faux=(a->u - b->u)*invyl; \
    left_du=myfist(faux); \
	left_u=myfist(b->u + prestep*faux); \
	faux=(a->v - b->v)*invyl; \
	left_dv=myfist(faux); \
	left_v=myfist(b->v + prestep*faux); \

#define ENVMAP_SLOPES(a,b) \
	faux=(a->envu - b->envu)*invyl; \
    left_envdu=myfist(faux); \
	left_envu=myfist(b->envu + prestep*faux); \
	faux=(a->envv - b->envv)*invyl; \
	left_envdv=myfist(faux); \
	left_envv=myfist(b->envv + prestep*faux); \

#define RGB_SLOPES(a,b) \
	faux=(a->R - b->R)*invyl; \
    left_dR=myfist(faux); \
    left_R=myfist(b->R + prestep*faux); \
    faux=(a->G - b->G)*invyl; \
	left_dG=myfist(faux); \
	left_G=myfist(b->G + prestep*faux); \
	faux=(a->B - b->B)*invyl; \
	left_dB=myfist(faux); \
    left_B=myfist(b->B + prestep*faux); \


// ####################################################################
// ####### PROCEDURE DI DISEGNO DEI TRIANGOLI (rasterizzatori) ########
// ####################################################################

// restituita quando il materiale non e' valido; tale proc non
// disegna niente
void tria_raster_NULL(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  return;
}

void tria_raster_flat_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])

  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  if (paint_second_part>1) return;
  for (j=bottom_y-top_y; j>0; j--)
  {
    current_scanline_proc();
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}




void tria_raster_RGB_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_RGB

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
	current_scanline_proc();
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}



void tria_raster_Ptexture_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr, vv[3];

  paint_second_part=0;
  YSORTING_VERTEX
  vv[0]=*myv[0];   vv[1]=*myv[1];   vv[2]=*myv[2];
  if (vv[2].ys<0) return; \
  if ((FP_BITS(vv[0].xs)) & (FP_BITS(vv[1].xs)) & (FP_BITS(vv[2].xs)) & 0x80000000) return; \
  if ((vv[0].xs>scrX) && (vv[1].xs>scrX) && (vv[2].xs>scrX)) return; \

  vv[0].u=vv[0].u*vv[0].dist; vv[0].v=vv[0].u*vv[0].dist;
  vv[1].u=vv[1].u*vv[1].dist; vv[1].v=vv[1].v*vv[1].dist;
  vv[2].u=vv[2].u*vv[2].dist; vv[2].v=vv[2].v*vv[2].dist;

  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE

  y1=vv[0].ys;
  y2=vv[1].ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=&vv[2]; miovr=&vv[1]; }
  else { miovl=&vv[1]; miovr=&vv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
//  TEXTURE_SLOPES(miovl, &vv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
    current_scanline_proc();
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=vv[2].ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    //TEXTURE_SLOPES(&vv[2], &vv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}



void tria_raster_texRGB_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_RGB
  CONSTANT_SLOPE_TEXTURE

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
    current_scanline_proc();
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}




void tria_raster_envRGB_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_RGB
  CONSTANT_SLOPE_ENVMAP

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
    current_scanline_proc();
	offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    ENVMAP_SLOPES(myv[2], myv[1])
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}




void tria_raster_texenv_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE
  CONSTANT_SLOPE_ENVMAP


  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;


ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
    current_scanline_proc();
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
  }

  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    ENVMAP_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}




void tria_raster_texenv_RGB_slow(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE
  CONSTANT_SLOPE_ENVMAP
  CONSTANT_SLOPE_RGB

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;


ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
    current_scanline_proc();
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }

  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    ENVMAP_SLOPES(myv[2], myv[1])
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}



// ###########################################################
// ###   RASTERS SPECIALIZZATI (scanline asm inlineate)    ###
// ###########################################################

void tria_raster_RGB(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_RGB

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
//	current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_texture(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_texture_tADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_texture_tBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_texture_tBLENDalpha(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_texture_lADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE
  CONSTANT_SLOPE_RGB

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_texture_lBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE
  CONSTANT_SLOPE_RGB

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
    _asm
	{
       call current_scanline_proc
	}
//    current_scanline_proc();
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_envmap(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_ENVMAP

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    ENVMAP_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_envmap_tADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_ENVMAP

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    ENVMAP_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_envmap_tBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_ENVMAP

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    ENVMAP_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_envmap_tBLENDalpha(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_ENVMAP

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    ENVMAP_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_envmap_lADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_ENVMAP
  CONSTANT_SLOPE_RGB

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    ENVMAP_SLOPES(myv[2], myv[1])
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_envmap_lBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_ENVMAP
  CONSTANT_SLOPE_RGB

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;

ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }
  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    ENVMAP_SLOPES(myv[2], myv[1])
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_texenv_mBLEND50(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE
  CONSTANT_SLOPE_ENVMAP


  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;


ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
  }

  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    ENVMAP_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}


void tria_raster_texenv_mBLEND50_lADD(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3)
{
  float4 prestep, faux;
  float4 y1, y2, fcy, invyl, invyr;
  AD_Vertex2D *miovl, *miovr;

  paint_second_part=0;
  YSORTING_VERTEX
  CLIP_2D
  CALC_LONGEST_SCANLINE
  CONSTANT_SLOPE_Z
  CONSTANT_SLOPE_TEXTURE
  CONSTANT_SLOPE_ENVMAP
  CONSTANT_SLOPE_RGB

  y1=myv[0]->ys;
  y2=myv[1]->ys;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  if (scan<0) // vertice 2 a destra
  { miovl=myv[2]; miovr=myv[1]; }
  else { miovl=myv[1]; miovr=myv[2]; }
  invyl=1.0f/(miovl->ys - y1);
  invyr=1.0f/(miovr->ys - y1);
  LEFT_X_SLOPES(miovl, myv[0])
  RIGHT_X_SLOPES(miovr, myv[0])
  TEXTURE_SLOPES(miovl, myv[0])
  ENVMAP_SLOPES(miovl, myv[0])
  RGB_SLOPES(miovl, myv[0])
  left_dz=(miovl->dist - myv[0]->dist)*invyl;
  left_z=myv[0]->dist + prestep*left_dz;


ciclo:
;
  for (j=bottom_y-top_y; j>0; j--)
  {
   // current_scanline_proc();
    _asm
	{
       call current_scanline_proc
	}
    offset0=offset0+scrX;

    left_x+=left_dx;
    right_x+=right_dx;
    left_z+=left_dz;
	left_u+=left_du;
	left_v+=left_dv;
	left_envu+=left_envdu;
	left_envv+=left_envdv;
	left_R+=left_dR;
	left_G+=left_dG;
	left_B+=left_dB;
  }

  paint_second_part++;
  if (paint_second_part>1) return;
  
  y1=y2;
  if (y1>=scrY) return;
  prestep=-y1;
  if (y1>=0) 
  {
    fcy=fceil(y1);
    prestep=fcy + prestep;
    top_y=myfist(fcy);
  }
  else top_y=0;
  offset0=muly[top_y];

  y2=myv[2]->ys;
  if (y2 >= scrY) bottom_y=scrY;
  else bottom_y=myfist(fceil(y2));

  invyl=1.0f/(y2 - y1);
  faux=(myv[2]->xs - myv[1]->xs)*invyl;
  if (scan<0)  // vertice 2 a destra
  {
    right_dx=myfist16(faux);
	right_x=myfist16(myv[1]->xs+prestep*faux);
  }
  else        // vertice 2 a sinistra
  {
	left_dx=myfist16(faux);
	left_x=myfist16(myv[1]->xs + prestep*faux);
    TEXTURE_SLOPES(myv[2], myv[1])
    ENVMAP_SLOPES(myv[2], myv[1])
    RGB_SLOPES(myv[2], myv[1])
    left_dz=(myv[2]->dist - myv[1]->dist)*invyl;
	left_z=myv[1]->dist + prestep*left_dz;
  }
  goto ciclo;
}