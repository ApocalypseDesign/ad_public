
#ifndef _RAYTOBJECT_H_
#define _RAYTOBJECT_H_

#include "..\..\Math\matrix.h"
#include "ADrender.h"
#include "..\camera.h"
#include "..\lights.h"
#include "..\keyframe.h"
#include "rayt_material.h"


// non toccare, i numeri sono legati a quelli di riga 75
// in UTILS3DS.H
#define TRIMESH 0
#define DUMMY 4
#define BONE 9
#define PATCHMESH 5
#define PARTICLESYSTEM 6
#define WINDMODIFIER 7
#define GRAVITYMODIFIER 8
#define MAX_OSM_MODIFIERS 10


typedef struct
{
  AD_Vect3D base, direction;
  float distance;
} AD_Ray;


class AD_RaytObject
{
public:
  
   int ID;                   // identificativo numerico univoco
                             // che identifica l'oggetto
   int ID_shadow_cached;

   rayt_material *mymat;
   // keyframer
   postrack positiontrack;   // traccia delle posizioni
   rottrack rotationtrack;   // traccia delle rotazioni
   postrack scaletrack;
   AD_Vect3D pivot;


   // strutture per i flare (per gli oggetti DUMMY)
   texture *flare;    // eventuale bitmap di flare
   float latoX, latoY;
   material matflare;
   float flare_scale_x, flare_scale_y;

   char *father_name;
   AD_RaytObject *father;
   int have_childrens;

   AD_Matrix currentmatrix;
   AD_Matrix currentmatrix_rot;
   AD_Matrix inverse_rotmatrix;
   AD_Vect3D currentpos;     // posizione corrente dell'oggetto

   char *name;               // nome dell'oggetto
   AD_Vect3D accum_scale;
   AD_Vect3D first_hit_pos;
   int type;            // puo' essere una TRIMESH
                        // o un DUMMY o una PATCHMESH
                       
   AD_RaytObject();   // costruttore
   void build_objectmatrix (float4 framepos);

   virtual int init(void)
   { return(0); }
   virtual int intersect(AD_Ray *ray)
   { return(0); }
   virtual int intersect_first(AD_Ray *ray)
   { return(0); }
   virtual void get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal)
   { return;}
   virtual void update (float framepos, AD_Vect3D *first_hit) {}
};


class AD_RaytSphere : public AD_RaytObject
{
public:
   float radius, sqr_radius, inv_radius;
   float first_hit_c;

   AD_RaytSphere();
   int init(void);
   int intersect(AD_Ray *ray);
   int intersect_first(AD_Ray *ray);
   void get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal);
   void update (float framepos, AD_Vect3D *first_hit);
};

// indice delle facce (piani)
#define BOX_UP    0
#define BOX_DOWN  1
#define BOX_LEFT  2
#define BOX_RIGHT 3
#define BOX_FRONT 4
#define BOX_BACK  5

class AD_RaytBox : public AD_RaytObject
{
public:
   AD_Vect3D normals[6], normals_tr[6];
   AD_Vect3D minb, maxb;
   int face_hitted;
   float lenght, width, height;
   
   AD_RaytBox();
   int init(void);
   int intersect(AD_Ray *ray);
   int intersect_first(AD_Ray *ray);
   void get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal);
   void update (float framepos, AD_Vect3D *first_hit);
};


class AD_RaytPlane : public AD_RaytObject
{
public:
   float lenght, width;
   AD_Vect3D normal, normal_tr;
   AD_Vect3D minb, maxb;

   AD_RaytPlane();
   int init(void);
   int intersect(AD_Ray *ray);
   int intersect_first(AD_Ray *ray);
   void get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal);
   void update (float framepos, AD_Vect3D *first_hit);
};


class AD_RaytCone : public AD_RaytObject
{
public:
   AD_RaytCone();
   int init(void);
   int intersect(AD_Ray *ray);
   int intersect_first(AD_Ray *ray);
   void get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal);
   void update (float framepos, AD_Vect3D *first_hit);
};

typedef struct
{
  int ID_obj;
  AD_RaytObject *obj;
  float u, v;
  rayt_material *mat;
} AD_ShootedRay;


#endif