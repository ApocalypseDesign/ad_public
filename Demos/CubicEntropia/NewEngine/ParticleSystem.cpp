#include "ParticleSystem.h"
#include "FastMath.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error

//const float4 VEL_SCALE = (float4)(-0.020f*1200.0f/160);
const float4 VEL_SCALE = -0.4f;
const float4 VAR_SCALE = 0.4f;

//--------------------------------------------------------------------

CParticleSystem::CParticleSystem()
{
  p_Particles=(Particle *)NULL;
  p_FadedParticles=0;
  p_MaxParticles=0;
  p_ParticlesLive=0;
  p_Life=0;
  p_StartTime=0;
  p_EmitterWidth=0;
  p_EmitterHeight=0;
  p_EmitterSpeed=0;
  p_EmitterVariation=0;
  p_EndTime=10000;
  p_EmitterWidthTrack=(CRollTrack *)NULL;
  p_EmitterHeightTrack=(CRollTrack *)NULL;
  p_EmitterSpeedTrack=(CRollTrack *)NULL;
  p_EmitterVariationTrack=(CRollTrack *)NULL;
  p_Texture=(CGraphicSurface *)NULL;
  p_Attenuation0=0.4f;
  p_Attenuation1=1.0f;
  p_Attenuation2=0.0f;
  p_Attenuation3=1.0f;
}

//--------------------------------------------------------------------

void CParticleSystem::m_BirthParticle(int32 witch, float4 time)
{
  AD_Vect3D vel, pos;
  int32 r, g, b;

  vel.x=vel.z=0;
  vel.y=p_EmitterSpeed;
  if (p_EmitterVariation != 0)
  {
	vel.x = -p_EmitterVariation + float(rand())/float(RAND_MAX) * 2.0f*p_EmitterVariation;
	vel.z = -p_EmitterVariation + float(rand())/float(RAND_MAX) * 2.0f*p_EmitterVariation;
	vel.y = p_EmitterSpeed - p_EmitterVariation + float(rand())/float(RAND_MAX) * 2.0f*p_EmitterVariation;
  }

  //p_Particles[witch].age=0.1f*((float4)rand())/RAND_MAX*p_Life;
  p_Particles[witch].age=0;

  mat_mulvectaffine(&p_CurrentRotationMatrix, &vel, &p_Particles[witch].vel);
  pos.x = -p_EmitterWidth*0.5f + float(rand())/float(RAND_MAX) * p_EmitterWidth;
  pos.z = -p_EmitterHeight*0.5f + float(rand())/float(RAND_MAX) * p_EmitterHeight;
  pos.y = 0;
  mat_mulvect(&p_WorldMatrix, &pos, &p_Particles[witch].pos);
  /*r=rand()%24+48*fast_cosf(4.0f*time*Pi__/180.0f)+48; 
  g=rand()%24+48*fast_cosf(3.0f*time*Pi__/180.0f)+48;
  b=rand()%24+48*fast_sinf(2.0f*time*Pi__/180.0f)+48;*/
  r=rand()%32+80; 
  g=rand()%16+20;
  b=rand()%8+10;
  p_Particles[witch].color=b | (g<<8) | (r << 16);
}

//--------------------------------------------------------------------

int32 CParticleSystem::m_Init(void *arg1)
{
  int32 i, j;
  CRenderLib *RL;
  HRESULT hr;
  char8 msg[150];

  strcpy(msg, p_Name);
  strcat(msg, " is a particle system without a texture! please control .ADP file!");
  if (!p_Texture) debug_error(miawin, msg);
  
  j=CGeometricObject::m_Init(arg1);
  if (!j) return(0);

  p_EmitterSpeed*=VEL_SCALE;
  p_EmitterVariation*=VAR_SCALE;
  p_ParticlesLive=0;
  p_PrevFramepos=p_StartTime;
  p_StepTime=0;
  p_MaxBirth=0;

  if (p_EmitterWidthTrack) p_EmitterWidthTrack->m_Init(1);
  if (p_EmitterHeightTrack) p_EmitterHeightTrack->m_Init(1);
  if (p_EmitterSpeedTrack) p_EmitterSpeedTrack->m_Init(1);
  if (p_EmitterVariationTrack) p_EmitterVariationTrack->m_Init(1);

  RL=(CRenderLib *)arg1;
  if (!RL) return(0);
  if (p_Texture->p_Type == GREYSCALE)
	  debug_error(miawin, "Particle system has a grayscale texture! Only 24bit are supported for particles!");
  RL->m_AddUpdate_Texture(p_Texture, 1);

  p_Particles=new Particle[p_MaxParticles];
  for (i=0; i<p_MaxParticles; i++)
  {
     p_Particles[i].age=-1;
     vect_set(&p_Particles[i].pos, 0, 0, 0);
     vect_set(&p_Particles[i].vel, 0, 0, 0);
  }

  // creazione ed inizializzazione del vertexbuffer
  hr=RL->p_Device->CreateVertexBuffer(p_MaxParticles*sizeof(ParticleVBVertex),
                                      D3DUSAGE_WRITEONLY |
									  D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS, // | D3DUSAGE_SOFTWAREPROCESSING,
									  FVF_PARTICLE,
                                      D3DPOOL_DEFAULT,
									  //D3DPOOL_MANAGED,
									  &p_VertexBuffer);

  srand(1);
  return(1);
}

//--------------------------------------------------------------------

void CParticleSystem::m_Update (float4 framepos)
{
  float4 dt, mb;
  int32 i, mbi, j;
  AD_Vect3D acceleration;


  CGeometricObject::m_Update(framepos);

  if (p_EmitterWidthTrack)
        p_EmitterWidthTrack->m_GetData(framepos, &p_EmitterWidth);
  if (p_EmitterWidth<0) p_EmitterWidth=0;

  if (p_EmitterHeightTrack)
        p_EmitterHeightTrack->m_GetData(framepos, &p_EmitterHeight);
  if (p_EmitterHeight<0) p_EmitterHeight=0;

  if (p_EmitterSpeedTrack)
        p_EmitterSpeedTrack->m_GetData(framepos, &p_EmitterSpeed);

  if (p_EmitterVariationTrack)
        p_EmitterVariationTrack->m_GetData(framepos, &p_EmitterVariation);
  if (p_EmitterVariation<0) p_EmitterVariation=0;

  // re-inizializzazione del particleSystem
  if (framepos<p_PrevFramepos)
  {
     p_PrevFramepos=framepos;
	 for (i=0; i<p_MaxParticles; i++)
	 {
       p_Particles[i].age=-1;
       vect_set(&p_Particles[i].pos, 0, 0, 0);
       vect_set(&p_Particles[i].vel, 0, 0, 0);
	 }
     srand(1);
	 return;
  }

  if (framepos<p_StartTime) return;
  dt=framepos-p_PrevFramepos;
  if (framepos>p_EndTime)
  {
	  mbi=0;
	  goto birthday;
  }

  //if (p_StepTime+dt>=1)
  {
    //mb=(p_StepTime+dt)*p_MaxParticles/p_Life;
	mb=dt*p_MaxParticles/p_Life;
	if (mb >= p_MaxParticles/p_Life)
		 mb = p_MaxParticles/p_Life;
	//p_StepTime=0;
	if (mb>=1)
	{
	  mbi=(int32)(mb);
	  p_MaxBirth=0;
	}
	else
	{
	  p_MaxBirth+=mb;
	  mbi=(int32)(p_MaxBirth);
	  if (mbi>=1) p_MaxBirth=0;
	}
  }
/*
  else
  {
	mbi=0;
    mb=(p_StepTime+dt)*p_MaxParticles/p_Life;
	if (mb >= p_MaxParticles/p_Life)
		 mb = p_MaxParticles/p_Life;
	if (mb>=1)
	{
	  mbi=myfist(mb);
	  p_MaxBirth=0;
      p_StepTime=0;
	}
	else
	{
	  p_MaxBirth+=mb;
	  mbi=myfist(p_MaxBirth);
	  if (mbi>=1) p_MaxBirth=0;
	}
  }*/

birthday:
;
  for (i=0; i<p_MaxParticles; i++)
  {
	// questi 2 if ORIGINARIAMENTE erano invertiti e senza l'if
	if (p_Particles[i].age >= p_Life)
      p_Particles[i].age=-1;
	else
	if (p_Particles[i].age >= 0)
      p_Particles[i].age+=dt;
  }


  for (i=0; ((i<p_MaxParticles) && (mbi>0)); i++)
  {
	if (p_Particles[i].age < 0)
	{
	    m_BirthParticle(i, framepos);
		mbi--;
	}
  }

  for (i=0; i<p_MaxParticles; i++)
  {
	if (p_Particles[i].age > 0)
	{
      // il particle procede, eventualmente deflesso
	  // da modificatori (tipo vento); i modificatori
	  // hanno effetto sulla velocita'
	  for (j=0; j<p_NumWSM; j++)
	  {
		 p_LinkedWSM[j]->m_Force(framepos, &p_Particles[i].pos, &p_Particles[i].vel, &acceleration);
         p_Particles[i].vel.x+=acceleration.x*dt;
         p_Particles[i].vel.y+=acceleration.y*dt;
         p_Particles[i].vel.z+=acceleration.z*dt;
	  }
      p_Particles[i].pos.x+=p_Particles[i].vel.x*dt;
      p_Particles[i].pos.y+=p_Particles[i].vel.y*dt;
      p_Particles[i].pos.z+=p_Particles[i].vel.z*dt;
	}
  }
  p_PrevFramepos=framepos;
}

//--------------------------------------------------------------------

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
void CParticleSystem::m_Paint(void *arg1, void *arg2, void *arg3)
{
  CRenderLib *RL;
  HRESULT hr;
  ParticleVBVertex *l_Particles;
  AD_Matrix l_identity;
  int32 numLive, i, alpha;
  DWORD zw, ab;

  RL=(CRenderLib *)arg1;
  if (!RL) return;

  hr=RL->p_Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &ab);
  hr=RL->p_Device->GetRenderState(D3DRS_ZWRITEENABLE, &zw);

  // matrice identita', i particle vivono gia' nello spazio mondo!!!
  mat_identity(&l_identity);
  RL->m_SetWorldMatrix(&l_identity);

  hr=RL->p_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
  hr=RL->p_Device->SetRenderState(D3DRS_POINTSCALEENABLE,  TRUE);
  hr=RL->p_Device->SetRenderState(D3DRS_POINTSIZE,     F2DW(p_Attenuation0));
  hr=RL->p_Device->SetRenderState(D3DRS_POINTSIZE_MIN, F2DW(0.0f));
  //RL->p_Device->SetRenderState(D3DRS_POINTSIZE_MAX, F2DW(50.0f));
  hr=RL->p_Device->SetRenderState(D3DRS_POINTSCALE_A,  F2DW(p_Attenuation1));
  hr=RL->p_Device->SetRenderState(D3DRS_POINTSCALE_B,  F2DW(p_Attenuation2));
  hr=RL->p_Device->SetRenderState(D3DRS_POINTSCALE_C,  F2DW(p_Attenuation3));
  hr=RL->p_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  hr=RL->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  hr=RL->p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr=RL->p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  // SERVE SENNO' SONO CAZZI
  hr=RL->p_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
  hr=RL->p_Device->SetRenderState(D3DRS_FOGENABLE, FALSE);

  hr=RL->p_Device->SetTexture(0, p_Texture->p_HWSurfaces[0]);
  hr=RL->p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=RL->p_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  hr=RL->p_Device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  hr=RL->p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  hr=RL->p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  hr=RL->p_Device->SetTexture(1, NULL);
  hr=RL->p_Device->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
  hr=RL->p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
  hr=RL->p_Device->SetTexture(2, NULL);
  hr=RL->p_Device->SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
  hr=RL->p_Device->SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

  // Set up the vertex buffer to be rendered
  RL->p_Device->SetStreamSource(0, p_VertexBuffer, sizeof(ParticleVBVertex));
  RL->p_Device->SetVertexShader(FVF_PARTICLE);

begin:
  //hr=p_VertexBuffer->Lock(0, 0, (BYTE **)&l_Particles, D3DLOCK_NOOVERWRITE);
  hr=p_VertexBuffer->Lock(0, 0, (BYTE **)&l_Particles, 0);
  if (hr!=0) goto begin;

  numLive=0;

  if (p_FadedParticles<=0)
    for (i=0; i<p_MaxParticles; i++)
	{
	  // lock e paint con drawprimitve
	  if (p_Particles[i].age > 0)
	  {
        vect_copy(&p_Particles[i].pos, &l_Particles[numLive].pos);
        l_Particles[numLive].color=0xffffffff;
        numLive++;
	  }
	}
  else
  {
	float4 h=p_FadedParticles*p_Life;
	float4 invLife=255.0f/(p_Life-h);
	float4 fStart=0.08f*p_Life;

    for (i=0; i<p_MaxParticles; i++)
	{
	  // lock e paint con drawprimitve
	  if (p_Particles[i].age > 0)
	  {
        vect_copy(&p_Particles[i].pos, &l_Particles[numLive].pos);
		if (p_Particles[i].age<=fStart)
		{
		   alpha=(int32)(255.0f*p_Particles[i].age/fStart);
		}
		else
		if (p_Particles[i].age<=h) alpha=255;
		else
		{
          alpha=((int32)((p_Particles[i].age-h)*invLife)) & 0xFF;
	      alpha=255-alpha;
		}
        //l_Particles[numLive].color=0x00ffffff | (alpha <<24);
        //l_Particles[numLive].color=(bb) | (gg<<8) | (rr<<16) | (alpha <<24);
        l_Particles[numLive].color=p_Particles[numLive].color | (alpha <<24);
        numLive++;
	  }
	}
  }
again:
  hr=p_VertexBuffer->Unlock();
  if (hr!=0) goto again;

  if (numLive>0)
	 hr=RL->p_Device->DrawPrimitive(D3DPT_POINTLIST, 0, numLive);
  hr=RL->p_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
  hr=RL->p_Device->SetRenderState(D3DRS_POINTSCALEENABLE,  FALSE);
  hr=RL->p_Device->SetRenderState(D3DRS_ZWRITEENABLE, zw);
  hr=RL->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, ab);
}