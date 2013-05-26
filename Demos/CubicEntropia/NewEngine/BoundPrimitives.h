#ifndef _BOUNDPRIMITIVES_H_
#define _BOUNDPRIMITIVES_H_

#include "Triangle.h"

typedef struct
{
  AD_Vect3D base, direction;
  float4 distance;
} Ray;

#define WORLDSPACE_RAY 0
#define CAMERASPACE_RAY 1

#define OBB_PRIMITIVE 0
#define AABB_PRIMITIVE 1
#define SPHERE_PRIMITIVE 2
#define UNKNOW_PRIMITIVE 3

class CBoundPrimitive
{
public:
  float4 p_EnclosedVolume;
  AD_Vect3D p_Center;
  //AD_Vect3D p_WorldCenter;
  int32 p_Type;
  AD_Vect3D *p_PaintVertex;
  int32 p_NumPaintVertex;

  CBoundPrimitive();
  virtual int32 m_IntersectRay (Ray *ray)
  { return(0); }
  virtual int32 m_IntersectPrimitive (CBoundPrimitive *primitive)
  { return(0); }
  virtual float4 m_GetEnclosedVolume(void)
  { return(0); }
  virtual void m_Update(AD_Matrix *obj2world, CBoundPrimitive *out)
  { return; }
  virtual int32 m_Bound(AD_Vect3D *vertex, AD_Tria3D *triangles,
	                    int32 *indexes, int32 start, int32 end)
  { return(0); }
  virtual void m_GetLongestAxis (int32 index, AD_Vect3D *out)
  { return; }
};

//--------------------------------------------------------------------

class CSpherePrimitive: public CBoundPrimitive
{
private:
  int32 m_IntersectSphere (CSpherePrimitive *sphere);

public:
  float4 p_Radius;
  // float4 p_WorldRadius;

  CSpherePrimitive();
  int32 m_IntersectRay (Ray *ray);
  int32 m_IntersectPrimitive (CBoundPrimitive *primitive);
  float4 m_GetEnclosedVolume(void);
  void m_Update(AD_Matrix *obj2world, CBoundPrimitive *out);
  int32 m_Bound(AD_Vect3D *vertex, AD_Tria3D *triangles,
                int32 *indexes, int32 start, int32 end);
  void m_GetLongestAxis (int32 index, AD_Vect3D *out);
};

//--------------------------------------------------------------------

class CAABBPrimitive: public CBoundPrimitive
{
public:
  float4 p_Width, p_Height, p_Length;
  //float4 p_WorldWidth, p_WorldHeight, p_WorldLength;

  CAABBPrimitive();
  int32 m_IntersectRay (Ray *ray);
  int32 m_IntersectPrimitive (CBoundPrimitive *primitive);
  float4 m_GetEnclosedVolume(void);
  void m_Update(AD_Matrix *obj2world, CBoundPrimitive *out);
  int32 m_Bound(AD_Vect3D *vertex, AD_Tria3D *triangles,
                int32 *indexes, int32 start, int32 end);
  void m_GetLongestAxis (int32 index, AD_Vect3D *out);
};

//--------------------------------------------------------------------

class COBBPrimitive: public CBoundPrimitive
{
public:
  float4 p_Width, p_Height, p_Length;
  //float4 p_WorldWidth, p_WorldHeight, p_WorldLength;
  AD_Vect3D p_XAxis, p_YAxis, p_ZAxis;
  //AD_Vect3D p_WorldXAxis, p_WorldYAxis, p_WorldZAxis;


  COBBPrimitive();
  int32 m_IntersectRay (Ray *ray);
  int32 m_IntersectPrimitive (CBoundPrimitive *primitive);
  float4 m_GetEnclosedVolume(void);
  void m_Update(AD_Matrix *obj2world, CBoundPrimitive *out);
  int32 m_Bound(AD_Vect3D *vertex, AD_Tria3D *triangles,
                int32 *indexes, int32 start, int32 end);
  void m_GetLongestAxis (int32 index, AD_Vect3D *out);
};

#endif