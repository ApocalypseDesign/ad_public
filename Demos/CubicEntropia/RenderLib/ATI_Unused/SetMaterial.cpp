#include <d3d8.h>
#include "types.h"
#include "SetMaterial.h"


LPDIRECT3DDEVICE8     ExtDevice;
LPDIRECT3DTEXTURE8    *ExtTextures;
int32                 TextureIndex1, TextureIndex2;
int32                 TransparencyAmount;


//--------------------------------------------------------------------

void setNULL(void) { return; }

void setRGB(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, NULL);
}


void setRGB_tADD(void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, NULL);
}


void setRGB_lBLEND3DS(void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, NULL);
}
// ***********************************************************
// ************************  TEXTURE  ************************
// ***********************************************************


void setTex (void)
{
  HRESULT hr;

  hr=ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
//  hr=ExtDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
//  check_dx_hwlib(hWindow, hr, "Device->SetTransform");
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");

//	if (HW_texrgb==HW_texrgb_arrays[0])
//     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
//  ExtDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}


void setTex_tBLENDalpha (void)
{
  HRESULT hr;

//	fprintf(fdebug, "setTex_tBLENDalpha called; current transparency level=%d\n", ((unsigned int)(current_transparency))>>24);
//	fflush(fdebug);
  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);

//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");

//	if (HW_texrgb==HW_texrgb_arrays[0])
//     HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
}


void setTex_tMAPalpha (void)
{
  HRESULT hr;

  hr = ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  hr = ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr = ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  hr = ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1,  D3DTA_TEXTURE);
  hr = ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  hr = ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_tADD (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lBLEND3DS (void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lADD (void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lSUB (void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);

}


void setTex_lBLEND3DS_tBLENDalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lBLEND3DS_tADD (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lBLEND3DS_tMAPalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lADD_tADD (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lADD_tBLENDalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lADD_tMAPalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lSUB_tADD (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lSUB_tBLENDalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


void setTex_lSUB_tMAPalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
}


// ***********************************************************
// *************************  ENVMAP  ************************
// ***********************************************************

void setEnv (void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_tBLENDalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_tMAPalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_tADD (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lBLEND3DS (void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lADD (void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lSUB (void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lBLEND3DS_tBLENDalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lBLEND3DS_tADD (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lBLEND3DS_tMAPalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lADD_tBLENDalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lADD_tADD (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lADD_tMAPalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lSUB_tBLENDalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lSUB_tADD (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}


void setEnv_lSUB_tMAPalpha (void)
{
  HRESULT hr;

  ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
}

// ***********************************************************
// *******************  ENVMAP + TEXTURE  ********************
// ***********************************************************
void setTexEnv_mBLEND3DS(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
/*
  ExtDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  ExtDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  hr = ExtDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		              TEX2VERTEX_FORMAT,
                  (void *)HW_texenv,
							    HW_numvert/3,
							    0);
  //  seconda passata
  ExtDevice->SetRenderState(D3DRS_ZWRITEENABLE , FALSE);
  ExtDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
	ExtDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);

  hr = ExtDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		              TEX2VERTEX_FORMAT,
                  (void *)HW_texenv,
							    HW_numvert/3,
							    0);

  ExtDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  ExtDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
*/
}


void setTexEnv_mADD(void)
{
  HRESULT hr;



  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mTEXSUBENV(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mENVSUBTEX(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex1]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}

// ***********************************************************
// ************  ENVMAP + TEXTURE + TRASPARENZA  *************
// ***********************************************************

void setTexEnv_mBLEND3DS_tBLENDalpha(void)
{
  HRESULT hr;

  hr = ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,  D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}



void setTexEnv_mBLEND3DS_tADD(void)
{
  HRESULT hr;

  hr = ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,  D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mBLEND3DS_tMAPalpha(void)
{
  HRESULT hr;

  hr = ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
//  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mADD_tBLENDalpha(void)
{
  HRESULT hr;

  hr = ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,  D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);
  
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}



void setTexEnv_mADD_tADD(void)
{
  HRESULT hr;

  hr = ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,  D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mADD_tMAPalpha(void)
{
  HRESULT hr;

  hr = ExtDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
//  ExtDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}

// ******************************************
// ***********   TEX+ENV+LUCI  ************
// ******************************************

void setTexEnv_mADD_lADD(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
/*
  ExtDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  ExtDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  hr = ExtDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		              TEX2VERTEX_FORMAT,
                  (void *)HW_texenv,
							    HW_numvert/3,
							    0);
  //  seconda passata
	ExtDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  ExtDevice->SetRenderState(D3DRS_ZWRITEENABLE , FALSE);
  ExtDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
  ExtDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

  ExtDevice->SetTexture(0, ExtTextures[TextureIndex2]);
  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);

  hr = ExtDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		              TEX2VERTEX_FORMAT,
                  (void *)HW_texenv,
							    HW_numvert/3,
							    0);

  ExtDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  ExtDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0); */
}


void setTexEnv_mADD_lSUB(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mADD_lBLEND3DS(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mBLEND3DS_lADD(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mBLEND3DS_lSUB(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}


void setTexEnv_mBLEND3DS_lBLEND3DS(void)
{
  HRESULT hr;

  ExtDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  ExtDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

  ExtDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  ExtDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
  
  ExtDevice->SetTexture(0, ExtTextures[TextureIndex1]);
  ExtDevice->SetTexture(1, ExtTextures[TextureIndex2]);

  ExtDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}