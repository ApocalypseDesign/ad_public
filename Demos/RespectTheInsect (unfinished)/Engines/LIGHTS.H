#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "types.h"
#include "..\Math\vect3D.h"
#include "keyframe.h"
#include "ADrender.h"
#include "camera.h"
#include "triangle.h"

class AD_Light
{ public:
    char *name;
    AD_Vect3D currentcolor;    // R = x,  G = y,  B = z
    postrack colortrack;
};


class AD_Omnilight : public AD_Light
{
public:
  AD_Vect3D currentpos;
  AD_Vect3D currentpos_inobject;
  AD_Vect3D currentpos_incamera;
  postrack positiontrack;
  float near_radius, far_radius;
  float inv_near_radius, inv_far_radius;

  // strutture per i flare
  texture *flare;    // eventuale bitmap di flare
  AD_Vertex3D  v3D[4];
  AD_Tria3D    tria[2];
  AD_Vertex2D  v2D[4];  // 2 triangoli = 4 vertici 2D
  float latoX, latoY;
  material matflare;
  float flare_scale_x, flare_scale_y;


  AD_Omnilight()
  {
	flare=(texture *)NULL;
	latoX=latoY=0;
    flare_scale_x=flare_scale_y=160.0f;
    near_radius=10000;
	far_radius=50000;
  }

  void init(void);
  void update (float4 pos, AD_Camera *cam);
};


class AD_Spotlight : public AD_Light
{
public:
  AD_Vect3D currentpos, currentpos_inobject;
  AD_Vect3D currenttarget, currenttarget_inobject;
  AD_Vect3D view_normalized_vector;
  AD_Vect3D currentpos_incamera;

  float4 currenthotspot;
  float4 currentfalloff, cos_currentfalloff;
  float near_radius, far_radius;
  float inv_near_radius, inv_far_radius;

  postrack positiontrack;
  postrack targettrack;

  void init();
  void update (float4 pos, AD_Camera *cam);
};

#endif