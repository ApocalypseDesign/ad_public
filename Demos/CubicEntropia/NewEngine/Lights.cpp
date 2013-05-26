#include "Lights.h"
#include "FastMath.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error

// ####################################################################
// ######################  LUCI OMNIDIREZIONALI  ######################
// ####################################################################

CLight::CLight()
{
   p_FlareScaleX=1.0f;
   p_FlareScaleY=1.0f;
   vect_set(&p_CurrentColor, 1.0f, 1.0f, 1.0f);
   vect_set(&p_CurrentPositionInObjectSpace, 0, 0, 0);
   p_Attenuation0=1;
   p_Attenuation1=0;
   p_Attenuation2=0;
   p_MaxRange=5000.0f;
   // settaggio luce D3D
   p_D3DLight.Type=D3DLIGHT_POINT;
}

//--------------------------------------------------------------------

int32 CLight::m_Init(void *arg1)
{
  int32 kc;

  if (p_ColorTrack)
  {
     // questo swapping serve perche' essendo la colortrack una traccia di posizione (ovvero
     // tripletta di float, che pero' in questo caso sono intesi come RGB) la yz vengono
     // swappate dal keyframer reader, e quindi essendo questi colori non devono essere
     // swappati  ==> serve uno swap per tornare ai dati giusti
     for (kc=0; kc<p_ColorTrack->p_NumKeys; kc++)
     {
       vect_swap_3DS(&p_ColorTrack->p_Keys[kc].p);
       vect_swap_3DS(&p_ColorTrack->p_Keys[kc].incomtg);
       vect_swap_3DS(&p_ColorTrack->p_Keys[kc].outcomtg);
       vect_scale(&p_ColorTrack->p_Keys[kc].p, RGB_MAXVALUE, &p_ColorTrack->p_Keys[kc].p);
	   vect_scale(&p_ColorTrack->p_Keys[kc].incomtg, RGB_MAXVALUE, &p_ColorTrack->p_Keys[kc].incomtg);
       vect_scale(&p_ColorTrack->p_Keys[kc].outcomtg, RGB_MAXVALUE, &p_ColorTrack->p_Keys[kc].outcomtg);
     }
  }
  
  if (p_ColorTrack)
    if (!p_ColorTrack->m_Init(1)) return(0);

  // luce ambientale emessa nulla e speculare massima
  p_D3DLight.Ambient.b=0.0f;
  p_D3DLight.Ambient.g=0.0f;
  p_D3DLight.Ambient.r=0.0f;
  p_D3DLight.Ambient.a=1.0f;
  p_D3DLight.Diffuse.b=p_CurrentColor.z;
  p_D3DLight.Diffuse.g=p_CurrentColor.y;
  p_D3DLight.Diffuse.r=p_CurrentColor.x;
  p_D3DLight.Diffuse.a=1.0f;
  p_D3DLight.Specular.b=p_CurrentColor.z;
  p_D3DLight.Specular.g=p_CurrentColor.y;
  p_D3DLight.Specular.r=p_CurrentColor.x;
  p_D3DLight.Specular.a=1.0f;
  p_D3DLight.Attenuation0=p_Attenuation0;
  p_D3DLight.Attenuation1=p_Attenuation1;
  p_D3DLight.Attenuation2=p_Attenuation2;
  p_D3DLight.Range=p_MaxRange;
  if (!CGeometricObject::m_Init(arg1))
    return(0);
  p_D3DLight.Position.x=p_CurrentPosition.x;
  p_D3DLight.Position.y=p_CurrentPosition.y;
  p_D3DLight.Position.z=p_CurrentPosition.z;
  return(1);
}

//--------------------------------------------------------------------

void CLight::m_Update(float4 frame)
{
  if (p_ColorTrack)
	  p_ColorTrack->m_GetData(frame, &p_CurrentColor);

  if (p_CurrentColor.x < 0) p_CurrentColor.x=0;
  if (p_CurrentColor.x > RGB_MAXVALUE) p_CurrentColor.x=RGB_MAXVALUE;
  if (p_CurrentColor.y < 0) p_CurrentColor.y=0;
  if (p_CurrentColor.y > RGB_MAXVALUE) p_CurrentColor.y=RGB_MAXVALUE;
  if (p_CurrentColor.z < 0) p_CurrentColor.z=0;
  if (p_CurrentColor.z > RGB_MAXVALUE) p_CurrentColor.z=RGB_MAXVALUE;

  p_D3DLight.Diffuse.b=p_CurrentColor.z;
  p_D3DLight.Diffuse.g=p_CurrentColor.y;
  p_D3DLight.Diffuse.r=p_CurrentColor.x;
  p_D3DLight.Diffuse.a=1.0f;
  p_D3DLight.Specular.b=p_CurrentColor.z;
  p_D3DLight.Specular.g=p_CurrentColor.y;
  p_D3DLight.Specular.r=p_CurrentColor.x;
  p_D3DLight.Specular.a=1.0f;

  CGeometricObject::m_Update(frame);
  p_D3DLight.Position.x=p_CurrentPosition.x;
  p_D3DLight.Position.y=p_CurrentPosition.y;
  p_D3DLight.Position.z=p_CurrentPosition.z;
}

//--------------------------------------------------------------------

void CLight::m_Paint(void *arg1, void *arg2, void *arg3)
{
   CRenderLib *RL;
//   AD_Matrix l_Identity;

   // se la luce non è visibile non la disegno
   if (!p_Visible) return;
   if (p_BaseMaterial)
   {
      RL=(CRenderLib *)arg1;
	  if (!RL) return;

      //mat_identity(&l_Identity);
	  //RL->m_SetWorldMatrix(&l_Identity);
	  // RL->m_SetMaterial(p_BaseMaterial, 0);
	  // TO DO: disegnare il point sprite
   }
}

//--------------------------------------------------------------------

// ####################################################################
// ###########################  LUCI SPOT  ############################
// ####################################################################

CSpotLight::CSpotLight()
{
  p_CurrentHotspot=(float4)(Pi__/3.0);
  p_CurrentFalloff=(float4)(Pi__/2.0);
  p_CosCurrentFalloff=(float4)cos(p_CurrentFalloff);
  // settaggio luce D3D
  p_D3DLight.Type=D3DLIGHT_SPOT;
}

//--------------------------------------------------------------------

int32 CSpotLight::m_Init(void *arg1)
{
  if (p_TargetTrack)
	if (!p_TargetTrack->m_Init(1)) return(0);

  if (p_FalloffTrack)
	if (!p_FalloffTrack->m_Init(1)) return(0);

  if (p_HotspotTrack)
	if (!p_HotspotTrack->m_Init(1)) return(0);

	return(CLight::m_Init(arg1));
}

//--------------------------------------------------------------------

void CSpotLight::m_Update (float4 frame)
{
  AD_Vect3D l_Dir;

  if (p_TargetTrack)
	  p_TargetTrack->m_GetData(frame, &p_CurrentTargetPosition);

  CLight::m_Update(frame);
  vect_sub(&p_CurrentTargetPosition, &p_CurrentPosition, &l_Dir);

  p_D3DLight.Direction.x=l_Dir.x;
  p_D3DLight.Direction.y=l_Dir.y;
  p_D3DLight.Direction.z=l_Dir.z;
}

//--------------------------------------------------------------------

void CSpotLight::m_Paint(void *arg1, void *arg2, void *arg3)
{
   CRenderLib *RL;
//   AD_Matrix l_Identity;

   // se la luce non è visibile non la disegno
   if (!p_Visible) return;
   if (p_BaseMaterial)
   {
      RL=(CRenderLib *)arg1;
	  if (!RL) return;

      //mat_identity(&l_Identity);
	  //RL->m_SetWorldMatrix(&l_Identity);
	  //RL->m_SetMaterial(p_BaseMaterial, 0);
	  // TO DO: disegnare il point sprite
   }
}