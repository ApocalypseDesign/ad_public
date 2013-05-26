#include "RenderLib.h"
#include "Dot3Toon_VS.h"

extern FILE *fdebug;

void CRenderLib::m_DOT3PassLights(CMaterial *m, int32 firstLight, D3DLIGHT8 *light)
{
  HRESULT hr;
  AD_Matrix ID, M1, M2;
 
  mat_identity(&ID);

  hr=p_Device->SetVertexShaderConstant(CV_LIGHT_POSITION, (void *)&light->Position.x, 1);
  hr=p_Device->SetVertexShaderConstant(CV_LIGHT_COLOR, (void *)&light->Diffuse.r, 1);
  hr=p_Device->SetVertexShaderConstant(CV_LIGHT_OBJECT_POSITION, (void *)&light->Direction.x, 1);

  mat_transpose(&m->p_Map1Matrix, &M1);
  hr=p_Device->SetVertexShaderConstant(CV_MAP1UVMATRIX_0, (void *)&M1, 4);
  mat_transpose(&m->p_Map2Matrix, &M2);
  hr=p_Device->SetVertexShaderConstant(CV_MAP2UVMATRIX_0, (void *)&M2, 4);

  // se è la prima luce non attivamo il blend cumulativo
  if (!firstLight)
  {
    hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  }
  else
    hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


  hr=p_Device->SetTexture(0, m->p_BumpMap->p_HWSurfaces[m->p_BumpMap->p_CurrentTextureIndex]);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

  // coordinate della cubemap
  hr=p_Device->SetTexture(1, p_NormalizationCubeMapTexture);
  hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSU,	D3DTADDRESS_CLAMP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSV,	D3DTADDRESS_CLAMP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSW,	D3DTADDRESS_CLAMP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);

  hr=p_Device->SetTexture(2, NULL);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_MODULATE);
  hr=p_Device->SetVertexShader(p_UV1DOT3Handle);
}

//--------------------------------------------------------------------

void CRenderLib::m_Map1DOT3FinalPass(CMaterial *m)
{
  HRESULT hr;
  AD_Matrix M1, M2;

  mat_transpose(&m->p_Map1Matrix, &M1);
  hr=p_Device->SetVertexShaderConstant(CV_MAP1UVMATRIX_0, (void *)&M1, 4);
  mat_transpose(&m->p_Map2Matrix, &M2);
  hr=p_Device->SetVertexShaderConstant(CV_MAP2UVMATRIX_0, (void *)&M2, 4);

  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  switch (m->p_LightMixType)
  {
    case MIXTYPE_MODULATE:
          hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
          hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	      break;
    case MIXTYPE_ADD:
          hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
          hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
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
  hr=p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
  hr=p_Device->SetTexture(2, NULL);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=p_Device->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  hr=p_Device->SetVertexShader(p_UV1DOT3FinalPassHandle);
}

//--------------------------------------------------------------------

void CRenderLib::m_Map2DOT3FinalPass(CMaterial *m)
{
  HRESULT hr;

  hr=p_Device->SetVertexShader(p_UV2DOT3FinalPassHandle);
}

//--------------------------------------------------------------------

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
void CRenderLib::m_SetMap12EMBM(CMaterial *m)
{
  HRESULT hr;
  AD_Matrix ID;

  mat_identity(&ID);

  hr=p_Device->SetTexture(0, m->p_Map1->p_HWSurfaces[m->p_Map1->p_CurrentTextureIndex]);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  hr=p_Device->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX *)&m->p_Map1Matrix);

  // bump map sullo stage 1
  hr=p_Device->SetTextureStageState(1, D3DTSS_BUMPENVMAT00, F2DW(m->p_BumpUScale));
  hr=p_Device->SetTextureStageState(1, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
  hr=p_Device->SetTextureStageState(1, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
  hr=p_Device->SetTextureStageState(1, D3DTSS_BUMPENVMAT11, F2DW(m->p_BumpVScale));
  hr=p_Device->SetTexture(1, m->p_BumpMap->p_HWSurfaces[m->p_BumpMap->p_CurrentTextureIndex]);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  hr=p_Device->SetTransform(D3DTS_TEXTURE1, (D3DMATRIX *)&m->p_Map1Matrix);

  hr=p_Device->SetTexture(2, m->p_Map2->p_HWSurfaces[m->p_Map2->p_CurrentTextureIndex]);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);
  hr=p_Device->SetTextureStageState(2, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(2, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  hr=p_Device->SetTransform(D3DTS_TEXTURE2, (D3DMATRIX *)&m->p_Map2Matrix);

  // -------------------  mixing delle due mappe  --------------------
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, m_MixType2ColorOp(m->p_MapsMixType));

  switch (m->p_Map1Channel)
  {
	 case ENVMAPPING:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACENORMAL | 0);
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACENORMAL | 0);
	    break;
	 case SPECULARMAPPING:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | 0);
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | 0);
	    break;
	 case TEXMAPPING1:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
		break;

	 case TEXMAPPING2:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
		break;
  }

  switch (m->p_Map2Channel)
  {
	 case ENVMAPPING:
        hr=p_Device->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACENORMAL | 0);
	    break;

	 case SPECULARMAPPING:
        hr=p_Device->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | 0);
	    break;

	 case TEXMAPPING1:
        hr=p_Device->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 0);
		break;

	 case TEXMAPPING2:
        hr=p_Device->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 1);
		break;
  }
}