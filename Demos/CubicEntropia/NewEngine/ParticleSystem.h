#ifndef _PARTICLESYSTEM_H_
#define _PARTICLESYSTEM_H_

#include "RenderLib.h"
#include "WSMObject.h"

typedef struct
{
  AD_Vect3D pos, vel;
  float4 age;
  int32 color;
} Particle;

typedef struct
{
   AD_Vect3D pos;
   int32 color;
} ParticleVBVertex;

#define FVF_PARTICLE ( D3DFVF_XYZ | D3DFVF_DIFFUSE )

class CParticleSystem: public CGeometricObject
{
public:
  Particle *p_Particles;
  int32 p_MaxParticles;
  int32 p_ParticlesLive;
  float4 p_FadedParticles;
  float4 p_Life;
  float4 p_StartTime, p_EndTime;
  float4 p_EmitterWidth, p_EmitterHeight;
  float4 p_EmitterSpeed, p_EmitterVariation;
  float4 p_PrevFramepos;
  float4 p_Attenuation0, p_Attenuation1, p_Attenuation2, p_Attenuation3;
  float4 p_StepTime, p_MaxBirth;
  int32 p_NumWSM;
  char8 **p_WSMNames;
  // array dei nomi dei WSM (per linkare), e array di puntatori
  // agli effettivi WSMObject
  CWSMObject **p_LinkedWSM;

  CRollTrack *p_EmitterWidthTrack, *p_EmitterHeightTrack;
  CRollTrack *p_EmitterSpeedTrack, *p_EmitterVariationTrack;

  CGraphicSurface *p_Texture;


  CParticleSystem();
  int32 m_Init(void *arg1);
  void m_Update(float4 frame);
  void m_Paint(void *arg1, void *arg2, void *arg3);
  virtual void m_BirthParticle(int32 witch, float4 time);
};

#endif