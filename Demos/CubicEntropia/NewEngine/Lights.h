#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "GeometricObject.h"

class CLight: public CGeometricObject
{
public:
   CPosTrack *p_ColorTrack;
   AD_Vect3D p_CurrentColor;
   AD_Vect3D p_CurrentPositionInObjectSpace;

   float4 p_FlareScaleX, p_FlareScaleY;
   float4 p_Attenuation0, p_Attenuation1, p_Attenuation2;
   float4 p_MaxRange;
   D3DLIGHT8 p_D3DLight;

   CLight();
   int32 m_Init(void *arg1);
   void m_Update(float4 frame);
   void m_Paint(void *arg1, void *arg2, void *arg3);
};


class CSpotLight : public CLight
{
public:
  AD_Vect3D p_CurrentTargetPosition;
  AD_Vect3D p_ViewNormalizedVector;

  float4 p_CurrentHotspot;
  float4 p_CurrentFalloff, p_CosCurrentFalloff;

  CPosTrack *p_TargetTrack;
  CRollTrack *p_HotspotTrack;
  CRollTrack *p_FalloffTrack;

  CSpotLight();
  int32 m_Init(void *arg1);
  void m_Update(float4 frame);
  void m_Paint(void *arg1, void *arg2, void *arg3);
};

#endif