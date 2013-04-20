#include <d3d9.h>
#include <math.h>
#include "setmat.h"
#include "..\ADrender.h"
#include "..\ADbase.h"
#include "extrend.h"


#define MAXTEXTURES 100

extern  LPDIRECT3DDEVICE9       g_pd3dDevice;
extern  LPDIRECT3DTEXTURE9      g_pD3DTexture[MAXTEXTURES];
extern  material                *matold;
extern  HWND                    hWindow;
extern  FILE                    *fdebug;
extern  int                     current_transparency;
int                             current_FVFflags;
void                            *current_vertex_buffer;

// procedure di settaggio D3D per ogni materiale
void setNULL(void)
{
   return;
}

void setRGB(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, NULL);
    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		                               HW_numvert/3,
									   (void *)HW_texrgb,
									   sizeof(TEX1RGBVERTEX));
    HW_numvert=0;
  }
}


void setRGB_tADD(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, NULL);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
									   HW_numvert/3,
                                       (void *)HW_texrgb,
                                       sizeof(TEX1RGBVERTEX));
    HW_numvert=0;
  }
}


void setRGB_lBLEND3DS(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, NULL);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
    HW_numvert=0;
  }
}
// ***********************************************************
// ************************  TEXTURE  ************************
// ***********************************************************


void setTex (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    hr=g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    hr=g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
//    hr=g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
//    check_dx_hwlib(hWindow, hr, "Device->SetTransform");
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
//  g_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
  }
}


void setTex_tBLENDalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
//	fprintf(fdebug, "setTex_tBLENDalpha called; current transparency level=%d\n", ((unsigned int)(current_transparency))>>24);
//	fflush(fdebug);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_tMAPalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    hr = g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    hr = g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    hr = g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    hr = g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    hr = g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1,  D3DTA_TEXTURE);
    hr = g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    hr = g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
    HW_numvert=0;
  }
}


void setTex_tADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texrgb,
		                               sizeof(TEX1RGBVERTEX));

	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lBLEND3DS (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lSUB (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lBLEND3DS_tBLENDalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lBLEND3DS_tADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							          HW_numvert/3,		                          
                                      (void *)HW_texrgb,
							          sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lBLEND3DS_tMAPalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							          HW_numvert/3,		                          
                                      (void *)HW_texrgb,
							          sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lADD_tADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lADD_tBLENDalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lADD_tMAPalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lSUB_tADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lSUB_tBLENDalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setTex_lSUB_tMAPalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
	//check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


// ***********************************************************
// *************************  ENVMAP  ************************
// ***********************************************************

void setEnv (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
//       HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_tBLENDalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
       //HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_tMAPalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
    HW_numvert=0;
  }
}


void setEnv_tADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lBLEND3DS (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lSUB (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
//	else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lBLEND3DS_tBLENDalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lBLEND3DS_tADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lBLEND3DS_tMAPalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lADD_tBLENDalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lADD_tADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lADD_tMAPalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lSUB_tBLENDalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lSUB_tADD (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}


void setEnv_lSUB_tMAPalpha (void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX1RGBVERTEX_FORMAT);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,		                          
                                       (void *)HW_texrgb,
							           sizeof(TEX1RGBVERTEX));
//	check_dx_hwlib(hWindow, hr, "DrawPrimitiveUP: ");
    HW_numvert=0;
//	if (HW_texrgb==HW_texrgb_arrays[0])
  //     HW_texrgb=HW_texrgb_arrays[1];
	//else HW_texrgb=HW_texrgb_arrays[0];
  }
}

// ***********************************************************
// *******************  ENVMAP + TEXTURE  ********************
// ***********************************************************
void setTexEnv_mBLEND3DS(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
/*
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		                          TEX2VERTEX_FORMAT,
                                  (void *)HW_texenv,
							      HW_numvert/3,
							      0);
    //  seconda passata
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE , FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);

    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		                          TEX2VERTEX_FORMAT,
                                  (void *)HW_texenv,
							      HW_numvert/3,
							      0);

    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
*/
    HW_numvert=0;
  }
}


void setTexEnv_mADD(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mTEXSUBENV(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mENVSUBTEX(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
    
    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);
	g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}

// ***********************************************************
// ************  ENVMAP + TEXTURE + TRASPARENZA  *************
// ***********************************************************

void setTexEnv_mBLEND3DS_tBLENDalpha(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    hr = g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    hr = g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,  D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}



void setTexEnv_mBLEND3DS_tADD(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    hr = g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    hr = g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,  D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mBLEND3DS_tMAPalpha(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    hr = g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    hr = g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
//    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mADD_tBLENDalpha(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    hr = g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    hr = g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,  D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}



void setTexEnv_mADD_tADD(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    hr = g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    hr = g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,  D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mADD_tMAPalpha(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    hr = g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    hr = g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
//    g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,  D3DTOP_DISABLE);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}

// ******************************************
// ***********   TEX+ENV+LUCI    ************
// ******************************************

void setTexEnv_mADD_lADD(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
/*
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		                          TEX2VERTEX_FORMAT,
                                  (void *)HW_texenv,
							      HW_numvert/3,
							      0);
    //  seconda passata
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE , FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

    g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);

    hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		                          TEX2VERTEX_FORMAT,
                                  (void *)HW_texenv,
							      HW_numvert/3,
							      0);

    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE , TRUE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	*/
    HW_numvert=0;
  }
}


void setTexEnv_mADD_lSUB(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mADD_lBLEND3DS(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mBLEND3DS_lADD(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mBLEND3DS_lSUB(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}


void setTexEnv_mBLEND3DS_lBLEND3DS(void)
{
  HRESULT hr;

  if (HW_numvert>0)
  {
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

    g_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
    
	g_pd3dDevice->SetTexture(0, g_pD3DTexture[matold->texture_ptr->HW_ADRender_Pos]);
    g_pd3dDevice->SetTexture(1, g_pD3DTexture[matold->envmap_ptr->HW_ADRender_Pos]);

    g_pd3dDevice->SetFVF(TEX2RGBVERTEX_FORMAT);
	hr = g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
							           HW_numvert/3,
                                       (void *)HW_texenv,
							           sizeof(TEX2RGBVERTEX));
    g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    HW_numvert=0;
  }
}