#ifndef _GEOMETRICOBJECT_H_
#define _GEOMETRICOBJECT_H_

#include <math.h>
#include "Keyframe.h"
#include "Mesh.h"
#include "OSM.h"

#define MAX_OSM_MODIFIERS 10

typedef struct
{
   CMesh *Mesh;
   float4 DistMin, DistMax;
   int32 VBStart;  // inizio dei vertici dentro il VertexBuffer
   int32 VBLong;   // quanti vertici occupa (multiplo di 3)
   int32 IBStart;  // inizio dei vertici dentro l'IndexBuffer
   int32 IBLong;
} LOD, *LOD_PTR;

#define MAX_LODS 5


class CGeometricObject
{
private:
   int32 m_CopyVertex(int32 baseIndex, void *dest, CMesh *mesh);
   int32 m_CopyTEX1Vertex(int32 baseIndex, void *dest, CMesh *mesh);
   int32 m_CopyTEX2Vertex(int32 baseIndex, void *dest, CMesh *mesh);
   int32 m_CopyTEX1DOT3Vertex(int32 baseIndex, void *dest, CMesh *mesh);
   int32 m_CopyTEX2DOT3Vertex(int32 baseIndex, void *dest, CMesh *mesh);
   void m_SortLods (void);
   void m_DoOSMs(int32 witchLod);

public:
//--------------------------------------------------------------------

   int32 p_Type;
   char8 p_Name[50];
   char8 p_FatherName[50];
   CGeometricObject *p_Father;
   int32 p_HasChildrens;

//--------------------------------------------------------------------

   CMaterial *p_BaseMaterial;

//--------------------------------------------------------------------

   LOD p_Lods[MAX_LODS];
   int32 p_NumLods;
   CBoundVolume *p_SPHEREBoundVolume;
   CBoundVolume *p_AABBBoundVolume;
   CBoundVolume *p_OBBBoundVolume;

//--------------------------------------------------------------------

   CObjectSpaceModifier *p_OSMs[MAX_OSM_MODIFIERS];
   int32 p_NumOSMs;

//--------------------------------------------------------------------

   IDirect3DVertexBuffer8 *p_VertexBuffer;
   int32 p_StaticVertex;

//--------------------------------------------------------------------
   
   CHideTrack *p_HideTrack;
   CPosTrack *p_PosTrack;
   CRotTrack *p_RotTrack;
   CPosTrack *p_ScaleTrack;

   int32 p_Visible;
   AD_Vect3D p_Pivot;
   AD_Vect3D p_CurrentPosition;
   AD_Vect3D p_CurrentPositionInCameraSpace;  // per fare dei sort
   AD_Quaternion p_CurrentRotationQuaternion;
   AD_Matrix p_CurrentRotationMatrix;
   AD_Vect3D p_CurrentScale, p_TotalScale;
   AD_Matrix p_WorldMatrix;
   AD_Matrix p_InverseWorldMatrix;

//--------------------------------------------------------------------

   CGeometricObject();
   int32 m_Clone(CGeometricObject *source);
   virtual int32 m_Init(void *arg1);
   void m_BuildWorldMatrix(void);
   virtual void m_Update(float4 frame);
   int32 m_IntersectRay (Ray *ray);
   virtual void m_Paint(void *arg1, void *arg2, void *arg3);
};

#endif