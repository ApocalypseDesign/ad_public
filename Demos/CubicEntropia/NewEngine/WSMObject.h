#ifndef _WSMOBJECT_H_
#define _WSMOBJECT_H_

#include "GeometricObject.h"

class CWSMObject: public CGeometricObject
{
public:
    CWSMObject() { }
	virtual void m_Force (float4 framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel) {}
};

typedef CWSMObject *CWSMObjectPtr;

#define FORCE_PLANAR     0
#define FORCE_SPHERICAL  1

// ***********************************************************
// ******************       VENTO      ***********************
// ***********************************************************
class CWindModifier: public CWSMObject
{
public:
  float4 p_Strength, p_Decay, p_Turbolence, p_ScaledStrength;
  float4 p_Frequency, p_Scale;
  int32 p_Mapping;
  AD_Vect3D p_Force, p_ScaledForce;  // estratto dalla currentmatrix

  CRollTrack *p_StrengthTrack, *p_DecayTrack, *p_TurbolenceTrack;
  CRollTrack *p_FrequencyTrack, *p_ScaleTrack;

  CWindModifier();
  int32 m_Init(void *arg1);
  void m_BuildWorldMatrix(void);
  void m_Update(float4 frame);
  void m_Force (float4 framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel);
};




// ***********************************************************
// ******************      GRAVITA'    ***********************
// ***********************************************************
class CGravityModifier: public CWSMObject
{
public:
  float4 p_Strength, p_Decay, p_ScaledStrength;
  int32 p_Mapping;
  AD_Vect3D p_Force, p_ScaledForce;  // estratto dalla currentmatrix

  CRollTrack *p_StrengthTrack, *p_DecayTrack;

  CGravityModifier();
  int32 m_Init(void *arg1);
  void m_BuildWorldMatrix(void);
  void m_Update(float4 frame);
  void m_Force (float4 framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel);
};




// ***********************************************************
// ******************       BOMBA      ***********************
// ***********************************************************
class CBombModifier: public CWSMObject
{
public:
  float4 p_Strenght, p_Chaos, p_Range;
  float4 p_Frequency, p_Scale;
  int32 p_Mapping, p_StartTime, p_Duration;
  int32 p_UnlimitedRange;

  CBombModifier();
  int32 m_Init(void *arg1);
  void m_BuildWorldMatrix(void);
  void m_Update(float4 frame);
  void m_Force (float4 framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel);
};

#endif