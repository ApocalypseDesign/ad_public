#include "RenderLib.h"

extern FILE *fdebug;

const int32 BLURDIMX=512;
const int32 BLURDIMY=512;
const float4 RADIALBLUR_SCALING = (float4)0.005;

void CRenderLib::m_SetUpMotionBlur(void)
{
  HRESULT hr;
  D3DSURFACE_DESC desc;
  void *l_VertexData;
  FVFGeometryUV1Transformed *l_Data;
  int32 i;
  #define uvEPS 1.0f

  hr=p_Device->CreateTexture(BLURDIMX, BLURDIMY, 1, D3DUSAGE_RENDERTARGET,
	                         p_VideoMode.Format, D3DPOOL_DEFAULT, &p_FrontTextureBlur);

  if (hr != D3D_OK)
  {
    p_MotionBlurSupported=FALSE;
    DumpDebugPoint(fdebug, "Motion blur not supported", 0);
	return;
  }

  p_MotionBlurSupported=TRUE;
  // creo un depth buffer che matchi come formato a quello
  // creato col device
  p_OldDepthSurface->GetDesc(&desc);
  hr=p_Device->CreateDepthStencilSurface(BLURDIMX, BLURDIMY, desc.Format,
                                         D3DMULTISAMPLE_NONE,
										 &p_DepthSurfaceBlur);
  m_D3DError(hr, "MotionBlur: depth buffer (AB) creation failed!");

  // prendiamo le superfici dalle texture
  p_FrontTextureBlur->GetSurfaceLevel(0, &p_FrontSurfaceBlur);


  // creo il vertex buffer dei due triangoloni che coprono tutto
  // lo schermo (originale)
  hr=p_Device->CreateVertexBuffer(6*sizeof(FVFGeometryUV1Transformed),
                                  D3DUSAGE_WRITEONLY,
							      D3DFVF_XYZRHW | D3DFVF_TEX1,
                                  D3DPOOL_MANAGED,
                                  &p_VertexBufferBlur1);
  hr=p_VertexBufferBlur1->Lock(0, 0, (BYTE **)&l_VertexData, 0);
  l_Data=(FVFGeometryUV1Transformed *)l_VertexData;
  vect_set(&l_Data[0].point, 0, 0, 0.5f);
  l_Data[0].uv1.u=l_Data[0].uv1.v=0;
  vect_set(&l_Data[1].point, (float4)p_VideoMode.Width-0, 0, 0.5f);
  l_Data[1].uv1.u=uvEPS;
  l_Data[1].uv1.v=0;
  vect_set(&l_Data[2].point, (float4)p_VideoMode.Width-0, (float4)p_VideoMode.Height-0, 0.5f);
  l_Data[2].uv1.u=l_Data[2].uv1.v=uvEPS;
  l_Data[3]=l_Data[2];
  l_Data[5]=l_Data[0];
  vect_set(&l_Data[4].point, 0, (float4)p_VideoMode.Height-0, 0.5f);
  l_Data[4].uv1.u=0;
  l_Data[4].uv1.v=uvEPS;
  for (i=0; i<6; i++) l_Data[i].w=1.0f/l_Data[i].point.z;
  hr=p_VertexBufferBlur1->Unlock();
  DumpDebugPoint(fdebug, "p_VertexBufferBlur2 creation done", 0);
  DumpDebugPoint(fdebug, "Motion blur VB creation done", 0);

  // cancellamento del backbuffer
  hr=p_Device->SetRenderTarget(p_FrontSurfaceBlur, p_DepthSurfaceBlur);
  m_D3DError(hr, "SetUpMotionBlur: clearing new front buffer failed!");
  hr=p_Device->BeginScene();
//  m_ClearScreen(0x007f7f7f, 1);
  m_ClearScreen(0, 1);
  hr=p_Device->EndScene();
  DumpDebugPoint(fdebug, "New front buffer cleared", 0);
}

//--------------------------------------------------------------------

void CRenderLib::m_SetUpRadialBlur(void)
{
  int32 i;
  HRESULT hr;
  float4 dx, dy;
  void *l_VertexData;
  FVFGeometryUV1Transformed *l_Data;
  #define uvEPS 1.0f

  hr=p_Device->CreateTexture(BLURDIMX, BLURDIMY, 1, D3DUSAGE_RENDERTARGET,
	                         p_VideoMode.Format, D3DPOOL_DEFAULT, &p_OldTextureBlur);

  if ((hr != D3D_OK) || (p_MotionBlurSupported==FALSE))
  {
    p_RadialBlurSupported=FALSE;
    DumpDebugPoint(fdebug, "Radial blur not supported", 0);
	return;
  }


  hr=p_Device->CreateTexture(BLURDIMX, BLURDIMY, 1, D3DUSAGE_RENDERTARGET,
	                         p_VideoMode.Format, D3DPOOL_DEFAULT, &p_CopyTextureBlur);

  if ((hr != D3D_OK) || (p_MotionBlurSupported==FALSE))
  {
    p_RadialBlurSupported=FALSE;
    DumpDebugPoint(fdebug, "Radial blur not supported", 0);
	return;
  }


  p_RadialBlurSupported=TRUE;
  // prendiamo la superfici dalla texture appena creata e facciamo
  // un clear
  hr=p_OldTextureBlur->GetSurfaceLevel(0, &p_OldSurfaceBlur);
  hr=p_CopyTextureBlur->GetSurfaceLevel(0, &p_CopySurfaceBlur);

  hr=p_Device->SetRenderTarget(p_OldSurfaceBlur, p_DepthSurfaceBlur);
  hr=p_Device->BeginScene();
  m_ClearScreen(0, 1);
  hr=p_Device->EndScene();

  hr=p_Device->SetRenderTarget(p_CopySurfaceBlur, p_DepthSurfaceBlur);
  hr=p_Device->BeginScene();
  m_ClearScreen(0, 1);
  hr=p_Device->EndScene();


  //float4 DX=(float4)p_VideoMode.Width;
  //float4 DY=(float4)p_VideoMode.Height;
  float4 DX=BLURDIMX;
  float4 DY=BLURDIMY;
  dx=(RADIALBLUR_SCALING*DX);
  dy=(RADIALBLUR_SCALING*DY);
 

  // creo il vertex buffer dei due triangoloni che coprono tutto
  // lo schermo e oltre
  DumpDebugPoint(fdebug, "Motion blur VB creation started", 0);
  hr=p_Device->CreateVertexBuffer(6*sizeof(FVFGeometryUV1Transformed),
                               D3DUSAGE_WRITEONLY,
							   D3DFVF_XYZRHW | D3DFVF_TEX1,
                               D3DPOOL_DEFAULT,
                               &p_VertexBufferBlur2);
  hr=p_VertexBufferBlur2->Lock(0, 0, (BYTE **)&l_VertexData, 0);
  l_Data=(FVFGeometryUV1Transformed *)l_VertexData;
  vect_set(&l_Data[0].point, -dx, -dy, 0.5f);
  l_Data[0].uv1.u=l_Data[0].uv1.v=0;
  vect_set(&l_Data[1].point, DX+dx, -dy, 0.5f);
  l_Data[1].uv1.u=uvEPS;
  l_Data[1].uv1.v=0;
  vect_set(&l_Data[2].point, DX+dx, DY+dy, 0.5f);
  l_Data[2].uv1.u=l_Data[2].uv1.v=uvEPS;
  l_Data[3]=l_Data[2];
  l_Data[5]=l_Data[0];
  vect_set(&l_Data[4].point, -dx, DY+dy, 0.5f);
  l_Data[4].uv1.u=0;
  l_Data[4].uv1.v=uvEPS;

  for (i=0; i<6; i++) l_Data[i].w=1.0f/l_Data[i].point.z;
  hr=p_VertexBufferBlur2->Unlock();
  DumpDebugPoint(fdebug, "p_VertexBufferBlur1 creation done", 0);


  dx=150;
  dy=150;
  // Vertex Buffer 3
  hr=p_Device->CreateVertexBuffer(6*sizeof(FVFGeometryUV1Transformed),
                               D3DUSAGE_WRITEONLY,
							   D3DFVF_XYZRHW | D3DFVF_TEX1,
                               D3DPOOL_DEFAULT,
                               &p_VertexBufferBlur3);
  hr=p_VertexBufferBlur3->Lock(0, 0, (BYTE **)&l_VertexData, 0);
  l_Data=(FVFGeometryUV1Transformed *)l_VertexData;
  vect_set(&l_Data[0].point, dx, dy, 0.5f);
  l_Data[0].uv1.u=l_Data[0].uv1.v=0;
  vect_set(&l_Data[1].point, (float4)BLURDIMX-dx, dy, 0.5f);
  l_Data[1].uv1.u=uvEPS;
  l_Data[1].uv1.v=0;
  vect_set(&l_Data[2].point, (float4)BLURDIMX-dx, (float4)BLURDIMY-dy, 0.5f);
  l_Data[2].uv1.u=l_Data[2].uv1.v=uvEPS;
  l_Data[3]=l_Data[2];
  l_Data[5]=l_Data[0];
  vect_set(&l_Data[4].point, dx, (float4)BLURDIMY-dy, 0.5f);
  l_Data[4].uv1.u=0;
  l_Data[4].uv1.v=uvEPS;
  for (i=0; i<6; i++) l_Data[i].w=1.0f/l_Data[i].point.z;
  hr=p_VertexBufferBlur3->Unlock();
}

//--------------------------------------------------------------------

void CRenderLib::m_SetBlur (int32 type, float4 level)
{
  if (level<=0) 
  {
	p_BlurLevel=0;
    p_BlurType=NOBLUR;
  }
  else
  {
    if (level>=0.95) p_BlurLevel=0.95f;
    else p_BlurLevel=level;
    p_BlurType=type;
  }

}

//--------------------------------------------------------------------

void CRenderLib::m_UpdateScreenBlur(void)
{
  HRESULT hr;
  int32 blendFactor;

  if (p_BlurType & MOTIONBLUR)
  {
	// scrivo sul frontbuffer originale  il rendering su texture
	// che è stato fatto; utilizzo un blenign di tipo alpha e fattore
	// pari al livello di blur
    hr=p_Device->SetRenderTarget(p_OldFrontSurface, p_OldDepthSurface);
    hr=p_Device->BeginScene();
    hr=p_Device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    hr=p_Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    hr=p_Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    hr=p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    hr=p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    // calcolo il fattore di blend
    blendFactor=(int32)((1.0f-p_BlurLevel)*255.0f);
    //blendFactor=(blendFactor<<24) | (0x00FFFFFF);
    blendFactor=(blendFactor<<24) & (0xFF000000);
    hr=p_Device->SetRenderState(D3DRS_TEXTUREFACTOR, blendFactor);
	if (p_BlurType & BLUR_ALPHA)
	{
      hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
	{
      hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	}

    hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    //hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    hr=p_Device->SetTexture(0, p_FrontTextureBlur);
    hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    // stage 1 inutilizzato
    hr=p_Device->SetTexture(1, NULL);
    hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    hr=p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    // vertex buffer e shader
    hr=p_Device->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_TEX1);
    hr=p_Device->SetStreamSource(0, p_VertexBufferBlur1,
	                           sizeof(FVFGeometryUV1Transformed));

    hr=p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    hr=p_Device->EndScene();

    hr=p_Device->Present(NULL, NULL, NULL, NULL);
    m_D3DError(hr, "CRenderLib::m_UpdateScreen failed!");
    hr=p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    p_FlipBufferBC++;
  }
  else
  if (p_BlurType & RADIALBLUR)
  {
	// 3 passi:
    hr=p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    hr=p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    hr=p_Device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
    hr=p_Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    hr=p_Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	if (p_BlurType & BLUR_ADD)
	{
      hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	}
	else
	{
      hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
      hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
    hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	hr=p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    blendFactor=(int32)((1.0f-p_BlurLevel)*255.0f);
	blendFactor=blendFactor & 255;
    blendFactor=(blendFactor<<24) | (0x00FFFFFF);

	// copysurface=AlphaBlend(Allargata(oldsurfaceblur), p_FrontSurfaceBlur)
	hr=p_Device->CopyRects(p_FrontSurfaceBlur, NULL, 0, p_CopySurfaceBlur, NULL);
	
	hr=p_Device->SetRenderTarget(p_CopySurfaceBlur, p_DepthSurfaceBlur);
    hr=p_Device->BeginScene();
      hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      p_Device->SetRenderState(D3DRS_TEXTUREFACTOR, blendFactor);
      hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      hr=p_Device->SetTexture(0, p_OldTextureBlur);
      hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      hr=p_Device->SetTexture(1, NULL);
      hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      // vertici scalati in positivo all'esterno
      hr=p_Device->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_TEX1);
      hr=p_Device->SetStreamSource(0, p_VertexBufferBlur2, sizeof(FVFGeometryUV1Transformed));
      hr=p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    hr=p_Device->EndScene();

	
	// OldFrontBuffer = copysurface
	hr=p_Device->SetRenderTarget(p_OldFrontSurface, p_OldDepthSurface);
    hr=p_Device->BeginScene();
      hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
      hr=p_Device->SetTexture(0, p_CopyTextureBlur);
	  //hr=p_Device->SetTexture(0, p_FrontTextureBlur);
      hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
      hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
      hr=p_Device->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_TEX1);
      hr=p_Device->SetStreamSource(0, p_VertexBufferBlur1, sizeof(FVFGeometryUV1Transformed));
      hr=p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
    hr=p_Device->EndScene();

    hr=p_Device->Present(NULL, NULL, NULL, NULL);
    m_D3DError(hr, "CRenderLib::m_UpdateScreen failed!");

    hr=p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    hr=p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	// swap tra Copy e Old
	LPDIRECT3DTEXTURE8 swap_texture;
    IDirect3DSurface8 *swap_surface; 

	swap_surface=p_OldSurfaceBlur;
	p_OldSurfaceBlur=p_CopySurfaceBlur;
	p_CopySurfaceBlur=swap_surface;

	swap_texture=p_OldTextureBlur;
	p_OldTextureBlur=p_CopyTextureBlur;
	p_CopyTextureBlur=swap_texture;
  }
}

//-------------------------------------------------------------------

void CRenderLib::m_SetUpBlurs(void)
{
  HRESULT hr;

  hr=p_Device->GetRenderTarget(&p_OldFrontSurface);
  m_D3DError(hr, "m_SetUpBlurs: getting front buffer failed!");
  hr=p_Device->GetDepthStencilSurface(&p_OldDepthSurface);
  m_D3DError(hr, "m_SetUpBlurs: getting depth buffer failed!");

  DumpDebugPoint(fdebug, "Begin motion blur creation", 0);
  m_SetUpMotionBlur();
  DumpDebugPoint(fdebug, "Motion blur creation done", 0);
  
  DumpDebugPoint(fdebug, "Begin radial blur creation", 0);
  m_SetUpRadialBlur();
  DumpDebugPoint(fdebug, "Radial blur creation done", 0);

  // cancelliamo le superfici originali
  hr=p_Device->SetRenderTarget(p_OldFrontSurface, p_OldDepthSurface);
  m_D3DError(hr, "SetUpRdialBlur: clearing old buffer failed!");
  hr=p_Device->BeginScene();
  m_ClearScreen(0, 1);
  hr=p_Device->EndScene();
  DumpDebugPoint(fdebug, "Old buffer cleared", 0);
}