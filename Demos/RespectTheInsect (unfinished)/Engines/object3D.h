
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "triangle.h"
#include "..\Math\matrix.h"
#include "ADrender.h"
#include "OSMobj.h"
#include "camera.h"
#include "lights.h"
#include "keyframe.h"


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
   AD_Matrix *skin_matrix;
   float weight;
}Skin_Bone;

typedef char Bone_Name[30];


class AD_Object3D
{
private:
   void get_vertex_normal (int16 quale, AD_Vect3D *vnorm);
   int  get_vertex_index_from_pointer (AD_Vertex3D *v);
   void precalc_radius();     // calcola il raggio della sfera circoscrittaj
   void init_normals(void);
   void init_tria(void);
   void init_vertex(void);
   void init_texture_coordinate(void);
   void split_tria_list (void);

public:
// *******************************************************************

   AD_Vertex3D *vertex3D;    // array di vertici originali
   AD_Vect3D *normals;
   AD_Vertex2D *vertex2D;    // array di vertici 2D
   int num_vertex3D, num_normals, num_vertex2D;
   AD_VectUV *vertexUV;
   int num_vertexUV;
   AD_Tria3D *tria;          // array di triangoli
   AD_Tria3D *tria_envRGB, *tria_RGB, *tria_envmap;
   int num_tria, num_tria_envRGB, num_tria_RGB, num_tria_envmap;
   int *triasmoothgroup;  // array di smooting groups

// *******************************************************************
   
   AD_OSMObject *OSMmods[MAX_OSM_MODIFIERS];
   int num_OSMmods;

// *******************************************************************

   // Strutture per effettuare Skin e Physic modifiers
   // array di puntatori a matrici
   AD_Matrix **bones_matrix, **bones_matrix_rot;
   Skin_Bone **skin_modifier;
   Bone_Name *bones_names;
   AD_Object3D *skinned_object;
   AD_Vect3D pos0, scale0;
   AD_Matrix rot0;

// *******************************************************************

   // keyframer
   postrack positiontrack;   // traccia delle posizioni
   rottrack rotationtrack;   // traccia delle rotazioni
   postrack scaletrack;
   AD_Vect3D pivot;

// *******************************************************************

   // strutture per i flare (per gli oggetti DUMMY)
   texture *flare;    // eventuale bitmap di flare
   float latoX, latoY;
   material matflare;
   float flare_scale_x, flare_scale_y;

// *******************************************************************

   char *father_name;     // usato per i .A3D
   AD_Object3D *father;   // settato dalla init
   int have_childrens;
   // usato SOLO negli .A3D; e' usato per risolvere il problema
   // di oggetti che hanno un materiale formato da solo una
   // envmap; infatti per questi oggetti non esiste il chunk
   // della lista delle facce texturizzate cosi' i triangoli
   // avranno sempre un materiale nullo associato
   material *matbase;
   
// *******************************************************************
   
   AD_Matrix currentmatrix;
   AD_Matrix currentmatrix_rot;
   AD_Matrix inverse_rotmatrix;
   AD_Matrix incremental_rotmatrix;
   AD_Matrix skin_matrix, physic_matrix;
   AD_Vect3D currentpos;     // posizione corrente dell'oggetto

   char *name;               // nome dell'oggetto
   float4 radius;            // raggio sfera circoscritta
   AD_Vect3D accum_scale;
   AD_Vect3D mid_point;
   int type;                 // vedere i #define a inizio file
   int num_omni, num_spot;

// *******************************************************************

   AD_Object3D();   // costruttore
   virtual int  init(void);
   virtual void paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight);
   virtual void paint_modifiers(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight);
   virtual void paint_bones(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight);
   virtual void build_objectmatrix (float4 framepos);  
};

#endif