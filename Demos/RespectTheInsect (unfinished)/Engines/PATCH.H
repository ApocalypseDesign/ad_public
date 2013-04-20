// Classe che gestisce un oggetto fatto da pezze di Bezier
// importato dal 3DSMax 3.0; non riadatto ilc odice di
// bezier.cpp in quanto il max contempla pezze bicubiche,
// quindi posso utilizzare alcune ottimizzazioni aggiuntive
// by [AD]Turbo
#ifndef _PATCH_H_
#define _PATCH_H_

#include "object3D.h"

#define QUADRATIC_PATCH 0
#define TRIANGULAR_PATCH 1

typedef struct
{
  int type;
  int vert[4];      // indici ai vertici di un PatchObject
  int vect[8];      // indici ai vettori di un PatchObject
  int interior[4];  // indici agli interiors di un PatchObject
  int UVvert[4];    // indici ai vertici texture
} AD_Patch;



// Spiegazione del significato che i campi di AD_Object3D
// avranno quando e' un PatchObject
//
// points: punti fisici 3d creati durante la tassellizzazione
// points_tr: punti fisici 3d della trimesh generata trasfomati
// normals: le normali ai vertici che formeranno la mesh triangolare
// vertex3D: punti 3D dei triangoli generati
// vertex2D: punti 2D dei triangoli generati (e proiettati)
// vertexUV: punti texture dei triangoli generati (e proiettati)
// num_vertex: numero di vertici generati dalla tassellizzazione
// num_tria: numero di triangoli generati dalla tassellizzazione
// positiontrack: traccia delle posizioni di tutto l'oggetto
// rotationtrack: traccia delle rotazioni di tutto l'oggetto
// scaletrack: traccia degli scaling di tutto l'oggetto
// pivot:  punto pivot
// matbase: materiale base, tutti i triangoli generati avranno
//          questo materiale
//
//
// NB: le tracce di posizione di vertici e vettori sovrascriveranno
// direttamente su points e vectors, visto che non serve avere
// gli originali per le entita' costituenti

class AD_PatchObject: public AD_Object3D
{
public:
   AD_Vect3D *verteces, *verteces_tr; // vertici geometrici
   AD_Vect3D *vectors, *vectors_tr;   // vettori geometrici
   AD_Vect3D *verteces_trn, *vectors_trn;
   AD_VectUV *UVverteces;
   postrack *vert_pos, *vect_pos;     // tracce di posizione
                                      // di vertici e vettori

   AD_Patch *patches;
   int num_patches;
   int num_UVverteces;          // numero di vertici texture
   int num_points, num_vectors; // numero di vertici geometrici
                                // numero di vettori geometrici

   // indicano il numero di punti sulla curva da
   // calcolare (compresi i 2 estremi) nei due sensi
   // tali parametri devono essere almeno 2 (o superiore)
   float u_evaluations, v_evaluations;

   AD_Vect3D currentpos_incamera;
   BOOL swap_normals;

   AD_PatchObject();  // costruttore
   void Evaluate_Patch(AD_Patch *p, float pu, float pv, AD_Vect3D *r);
   void Evaluate_uDerivate(AD_Patch *p, float pu, float pv, AD_Vect3D *r);
   void Evaluate_vDerivate(AD_Patch *p, float pu, float pv, AD_Vect3D *r);
   int Is_Patch_Culled (AD_Patch *p);

   int Tassellate_Texture(void);
   int Tassellate_Normals(void);
   int Tassellate_NormalsTexture(void);
   int Tassellate(void);

   // metodi virtuali derivati da AD_Object3D
   int init(void);
   void paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight);
   void build_objectmatrix (float4 framepos);  
};


#endif