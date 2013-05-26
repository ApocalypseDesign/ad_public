#include "RenderLib.h"
#include "Dot3Toon_VS.h"

extern FILE *fdebug;

void CRenderLib::m_SetToonPass1(void)
{
  HRESULT hr;

  hr=p_Device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
  hr=p_Device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
  hr=p_Device->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);

  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

  hr=p_Device->SetTexture(0, NULL);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=p_Device->SetVertexShader(p_TOONPass1Handle);
}

// -------------------------------------------------------------------

void CRenderLib::m_SetToonPassLights(CMaterial *m, int32 firstLight, D3DLIGHT8 *light)
{
  HRESULT hr;

  hr=p_Device->SetVertexShaderConstant(CV_LIGHT_POSITION, (void *)&light->Position.x, 1);
  hr=p_Device->SetVertexShaderConstant(CV_LIGHT_COLOR, (void *)&light->Diffuse.r, 1);
  hr=p_Device->SetVertexShaderConstant(CV_LIGHT_OBJECT_POSITION, (void *)&light->Direction.x, 1);

  // se è la prima luce non attivamo il blend cumulativo
  if (!firstLight)
  {
    hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	switch (m->p_ToonLightsMixType)
	{
	  case MIXTYPE_ADD:
	    hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	    hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	  case MIXTYPE_ADDSMOOTH:
	    hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	    hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	  default:
	    hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	    hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
	    //hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	    //hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		break;
	}
  }
  else
  {
    hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  }

  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  hr=p_Device->SetTexture(0, m->p_ToonGradient->p_HWSurfaces[0]);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

  hr=p_Device->SetTexture(1, NULL);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=p_Device->SetVertexShader(p_TOONPassLightsHandle);
}

// -------------------------------------------------------------------

void CRenderLib::m_SetToonFinalPass(CMaterial *m)
{
  AD_Matrix M1, M2;
  HRESULT hr;

  // risetto a wrap l'addressU e a linear i filtri
  hr=p_Device->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);

  if ((!m->p_Map1) || (m->p_Map1Channel!=TEXMAPPING1)) return;

  mat_transpose(&m->p_Map1Matrix, &M1);
  hr=p_Device->SetVertexShaderConstant(CV_MAP1UVMATRIX_0, (void *)&M1, 4);
  mat_transpose(&m->p_Map2Matrix, &M2);
  hr=p_Device->SetVertexShaderConstant(CV_MAP2UVMATRIX_0, (void *)&M2, 4);

  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  switch (m->p_ToonMapsMixType)
  {
	case MIXTYPE_ADD:
	  hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	  hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	  break;
	case MIXTYPE_ADDSMOOTH:
	  hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	  hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	  break;
	case MIXTYPE_MODULATE2X:
	  hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	  hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
	  break;
	default:
	  hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	  hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	  break;
  }

  hr=p_Device->SetTexture(0, m->p_Map1->p_HWSurfaces[m->p_Map1->p_CurrentTextureIndex]);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

  hr=p_Device->SetTexture(1, NULL);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=p_Device->SetTexture(2, NULL);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=p_Device->SetVertexShader(p_TOONFinalPassHandle);
}