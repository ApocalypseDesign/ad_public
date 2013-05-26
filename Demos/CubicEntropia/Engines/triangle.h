#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "types.h"
#include "..\Math\vect3D.h"
#include "..\Math\matrix.h"
#include "camera.h"


typedef struct
{
  AD_Vect3D midpoint;            // punto medio                12
  material *materiale;           // puntatore al materiale      4
  AD_Vect3D normal;              // normale al triangolo       12
  float4 mid_z;                  // serve per fare il zsorting  4
  AD_Vertex3D *v1, *v2, *v3;     // puntatori vertici          12
  AD_Vect3D *n1, *n2, *n3;       // puntatori alle normali     12
  AD_Vertex2D *sp1, *sp2, *sp3;  // puntattori ai vertici2D    12
  float4 radius;                 // raggio circoscritto         4
                                 // -----------------------------
                                 // totale                     72
} AD_Tria3D, *AD_Tria3DPtr;


int tria_init(AD_Tria3D *t);
int tria_isculled3D(AD_Tria3D *t, AD_Vect3D *camera_pos);
int tria_isculled2D(AD_Vertex2D *v1, AD_Vertex2D *v2, AD_Vertex2D *v3);
int triaobj_isclipped_bounding(AD_Vect3D *v, float4 radius, AD_Camera *telecamera);
inline int triaobj_isclipped_bounding_inline(AD_Vect3D *v, float4 raggio, AD_Camera *telecamera)
{
  // controllo piani z del frustrum; notare che un oggetto e' clippato
  // appena risulta fuori da uno dei 4 piani del frustrum;
  if (v->z + raggio<=telecamera->znear) return(1);  // controllo piano znear
  if (v->z - raggio>=telecamera->zfar) return(1);  // controllo piano zfar

  raggio=-raggio;
  // nei prodotto scalari posso evitare di considerare certe
  // componenti, visto che per i piani left e right la
  // componente y delle normali e' zero, cosi' come le
  // componenti x delle normali dei piano up e down
  if ((v->x*telecamera->frustrum_left_normal.x+
	   v->z*telecamera->frustrum_left_normal.z) <=raggio) return(1);
  if ((v->x*telecamera->frustrum_right_normal.x+
	   v->z*telecamera->frustrum_right_normal.z) <=raggio) return(1);
  if ((v->y*telecamera->frustrum_up_normal.y+
	   v->z*telecamera->frustrum_up_normal.z) <=raggio) return(1);
  if ((v->y*telecamera->frustrum_down_normal.y+
	   v->z*telecamera->frustrum_down_normal.z) <=raggio) return(1);
  return(0);
}


void tria_paint_general(AD_Tria3D *t);
/*
void tria_paint_general_trasparent(AD_Tria3D *t);
void tria_paint_flat(AD_Tria3D *t);
void tria_paint_RGB(AD_Tria3D *t);
void tria_paint_texture(AD_Tria3D *t);
void tria_paint_envmap(AD_Tria3D *t);
void tria_paint_texenv(AD_Tria3D *t);
void tria_paint_texRGB(AD_Tria3D *t);
void tria_paint_envRGB(AD_Tria3D *t);
void tria_precalc_normal(AD_Tria3D *t);
void tria_precalc_radius(AD_Tria3D *t);
void tria_precalc_puntomedio(AD_Tria3D *t);
*/
#endif