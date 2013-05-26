#include <math.h>
#include "WSMObject.h"
#include "FastMath.h"

static float4 forceScaleFactor = (float4)(((1200*1200)/(40*40)));

// ***********************************************************
// ******************       VENTO      ***********************
// ***********************************************************
CWindModifier::CWindModifier()
{
  p_Strength=0;
  p_Decay=0;
  p_Turbolence=0;
  p_Frequency=0;
  p_Scale=0;
  p_Mapping=FORCE_PLANAR;
  vect_set(&p_Force, 0, 0, 0);
  p_StrengthTrack=(CRollTrack *)NULL;
  p_DecayTrack=(CRollTrack *)NULL;
  p_TurbolenceTrack=(CRollTrack *)NULL;
  p_FrequencyTrack=(CRollTrack *)NULL;
  p_ScaleTrack=(CRollTrack *)NULL;
}

//--------------------------------------------------------------------

int32 CWindModifier::m_Init(void *arg1)
{
  if (p_StrengthTrack) p_StrengthTrack->m_Init(1);
  if (p_DecayTrack) p_DecayTrack->m_Init(1);
  if (p_TurbolenceTrack) p_TurbolenceTrack->m_Init(1);
  if (p_FrequencyTrack) p_FrequencyTrack->m_Init(1);
  if (p_ScaleTrack) p_ScaleTrack->m_Init(1);

  if (p_Decay<0) p_Decay=0;
  return(CGeometricObject::m_Init(arg1));
}

//--------------------------------------------------------------------

void CWindModifier::m_Update(float4 frame)
{
  CGeometricObject::m_Update(frame);

  if (p_StrengthTrack)
     p_StrengthTrack->m_GetData(frame, &p_Strength);

  if (p_DecayTrack)
     p_DecayTrack->m_GetData(frame, &p_Decay);
  if (p_Decay<0) p_Decay=0;

  if (p_TurbolenceTrack)
     p_TurbolenceTrack->m_GetData(frame, &p_Turbolence);
  if (p_Turbolence<0) p_Turbolence=0;
  
  if (p_FrequencyTrack)
     p_FrequencyTrack->m_GetData(frame, &p_Frequency);
  if (p_Frequency<0) p_Frequency=0;

  if (p_ScaleTrack)
     p_ScaleTrack->m_GetData(frame, &p_Scale);
  if (p_Scale<0) p_Scale=0;

  mat_get_column(&p_WorldMatrix, 1, &p_Force);
  vect_auto_normalize(&p_Force);

  p_ScaledStrength=p_Strength*0.0001f*forceScaleFactor*1.5f;
  vect_scale(&p_Force, p_ScaledStrength, &p_ScaledForce);
}

//--------------------------------------------------------------------

void CWindModifier::m_Force (float4 framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel)
{
  AD_Vect3D posrel, tf, p, force, p2;
  float4 dist, freq, turb;

  freq=p_Frequency;
  turb=p_Turbolence;

  if (p_Mapping==FORCE_PLANAR)
  {
    if (p_Decay!=0.0f)
	{
	  vect_sub(pos, &p_CurrentPosition, &posrel);
	  dist=fabsf(vect_dot(&p_Force, &posrel));
      vect_scale(&p_ScaledForce, (float4)exp(-p_Decay*dist), accel);
	}
	else vect_copy(&p_ScaledForce, accel);
  } 
  else
  {
    vect_sub(pos, &p_CurrentPosition, &force);
    dist = vect_length(&force);
    if (dist != 0) vect_auto_scale(&force, 1.0f/dist);
    if (p_Decay != 0)
      vect_scale(&force, p_ScaledStrength*(float4)exp(-p_Decay*dist), accel);
	else
      vect_scale(&force, p_ScaledStrength, accel);
  }

  if (turb != 0)
  {
    vect_sub(pos, &p_CurrentPosition, &p2);
    freq *= 0.01f;

	vect_copy(&p2, &p);
    p.x  = freq * framepos;
    tf.x = noise3(p.x*p_Scale, p.y*p_Scale, p.z*p_Scale);

	vect_copy(&p2, &p);
    p.y  = freq * framepos;
    tf.y = noise3(p.x*p_Scale, p.y*p_Scale, p.z*p_Scale);

	vect_copy(&p2, &p);    
	p.z  = freq * framepos;
    tf.z = noise3(p.x*p_Scale, p.y*p_Scale, p.z*p_Scale);

    turb *= 0.0001f*forceScaleFactor;
    vect_auto_scale(&tf, turb);
	vect_add(accel, &tf, accel);
  }
}



// ***********************************************************
// ******************      GRAVITA'    ***********************
// ***********************************************************
CGravityModifier::CGravityModifier()
{
  p_StrengthTrack=(CRollTrack *)NULL;
  p_DecayTrack=(CRollTrack *)NULL;
}

//--------------------------------------------------------------------

int32 CGravityModifier::m_Init(void *arg1)
{
  if (p_StrengthTrack) p_StrengthTrack->m_Init(1);
  if (p_DecayTrack) p_DecayTrack->m_Init(1);
  if (p_Decay<0) p_Decay=0;
  return(CGeometricObject::m_Init(arg1));
}

//--------------------------------------------------------------------

void CGravityModifier::m_Update(float4 frame)
{
  CGeometricObject::m_Update(frame);

  if (p_StrengthTrack)
     p_StrengthTrack->m_GetData(frame, &p_Strength);

  if (p_DecayTrack)
     p_DecayTrack->m_GetData(frame, &p_Decay);
  if (p_Decay<0) p_Decay=0;

  mat_get_column(&p_WorldMatrix, 1, &p_Force);
  vect_auto_normalize(&p_Force);

  p_ScaledStrength=p_Strength*0.0001f*forceScaleFactor*1.5f;
  vect_scale(&p_Force, p_ScaledStrength, &p_ScaledForce);
}

//--------------------------------------------------------------------

void CGravityModifier::m_Force (float4 framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel)
{
    AD_Vect3D posrel, force;
    float4 dist;

    if (p_Mapping==FORCE_PLANAR)
	{
	  if (p_Decay!=0)
	  {
	    vect_sub(pos, &p_CurrentPosition, &posrel);
        dist = fabsf(vect_dot(&p_Force, &posrel));
		vect_scale(&p_ScaledForce, (float4)exp(-p_Decay*dist), accel);
	  }
	  else
	  vect_copy(&p_ScaledForce, accel);
	}
    else
	{
	  vect_sub(&p_CurrentPosition, pos, &force);
      dist = vect_length(&force);
      if (dist != 0) vect_auto_scale(&force, 1.0f/dist);
      if (p_Decay != 0) 
	  {
        vect_scale(&force, p_ScaledStrength*(float4)exp(-p_Decay*dist), accel);
	  }
	  else
	    vect_scale(&force, p_ScaledStrength, accel);
	}
}



// ***********************************************************
// ******************       BOMBA      ***********************
// ***********************************************************
int32 CBombModifier::m_Init(void *arg1)
{
  return(CGeometricObject::m_Init(arg1));
}

//--------------------------------------------------------------------

void CBombModifier::m_Update(float4 frame)
{
  CGeometricObject::m_Update(frame);
}

//--------------------------------------------------------------------

void CBombModifier::m_Force (float4 framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel)
{
}