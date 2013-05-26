#ifndef _MESH_H_
#define _MESH_H_

#include <math.h>
#include "BoundVolumes.h"

// La classe rappresenta una generica mesh triangolare
class CMesh
{
private:
   // inizializza un singolo triangolo
   void  m_InitTriangle(AD_Tria3D *t);
   // inizializza il triangolo con incluso il calcolo del bump space
   void  m_InitTriangleBump(AD_Tria3D *t);
   // inizializza tutti i triangoli
   void  m_InitTriangles(void);
   // calcola le normali ai vertici tenendo conto degli eventuali
   // smoothing groups
   void  m_BuildVertexNormals(void);
   // calcola le normali ai vertici e il bump space relativo tenendo
   // conto degli eventuali smoothing groups
   void  m_BuildVertexNormalsBump(void);
   // verifica se due vertici "driver" sono uguali
   int32 m_CompareDriverVertex (DriverVertex *v1, DriverVertex *v2);
   // setta a positive tutte le coordinate UV
   void m_InitTextureCoordinates(void);
   // ottimizza la mesh
   int32 m_Optimize(void);
   // costruisce dai dati letti nel .A3D le relative strutture
   // adatte a Direct3D 8.x
   int32 m_BuildDriverStructures(void);
   
   void m_CalcExtension(void);
   float4 p_MinX, p_MinY, p_MinZ;
   float4 p_MaxX, p_MaxY, p_MaxZ;

public:
   char8 p_Name[50];

   AD_Vect3D *p_ObjspaceVertex, *p_ObjspaceNormals;
   AD_VectUV *p_UV1vertex, *p_UV2vertex;
   BUMPSPACE *p_VertexBumpSpaces;
   int32 p_NumObjspaceVertex, p_NumObjspaceNormals;
   int32 p_NumUV1vertex, p_NumUV2vertex;
   int32 p_NumVertexBumpSpaces;

   DriverVertex *p_DriverVertex;
   int32 p_NumDriverVertex;

//------------------------------------------------------------------

   DriverTriangle *p_DriverTriangles;          // array di triangoli
   int32 p_NumDriverTriangles;

//------------------------------------------------------------------

   AD_Tria3D *p_Triangles;
   int32 p_NumTriangles;
   int32 *p_SmoothingGroups;  // array di smooting groups

//------------------------------------------------------------------

   CBoundVolume p_SPHEREBoundVolume;
   CBoundVolume p_AABBBoundVolume;
   CBoundVolume p_OBBBoundVolume;
   int32 p_SPHEREVolumeTreeDepth;
   int32 p_AABBVolumeTreeDepth;
   int32 p_OBBVolumeTreeDepth;

//------------------------------------------------------------------

   CMesh();
   int32 m_NormalizeObjspaceVertex(void);
   void m_GetExtension (float4 *x1, float4 *y1, float4 *z1,
	                    float4 *x2, float4 *y2, float4 *z2)
   {
      if ((!x1) || (!y1) || (!z1) ||
          (!x2) || (!y2) || (!z2))
		return;
	  *x1=p_MinX;
	  *y1=p_MinY;
	  *z1=p_MinZ;
	  *x2=p_MaxX;
	  *y2=p_MaxY;
	  *z2=p_MaxZ;
   }
   virtual int32 m_Init(void);
   virtual int32 m_Load (char8 *filenameA3D, char8 *meshname);
};

#endif