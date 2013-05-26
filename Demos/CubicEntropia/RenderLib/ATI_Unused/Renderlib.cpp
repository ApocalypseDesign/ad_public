#include <math.h>
#include "RenderLib.h"

extern LPDIRECT3DDEVICE8     ExtDevice;
extern LPDIRECT3DTEXTURE8    *ExtTextures;
extern int32                 TextureIndex1, TextureIndex2;
extern int32                 TransparencyAmount;

CRenderLib::CRenderLib()
{
  int32 i;

  p_BeginEnd_Scene=0;
  p_IsDeviceCreated=0;
  p_IsEnumerationDone=0;

  p_D3D=(LPDIRECT3D8)NULL;
  p_Device=(LPDIRECT3DDEVICE8)NULL;
  p_FrontBuffer=(CGraphicSurface *)NULL;

  p_NormalizationCubeMapTexture=(LPDIRECT3DCUBETEXTURE8)NULL;

  p_NumMaterials=0;
  for (i=0; i<MAX_MATERIALS; i++)
    p_Materials[i]=(CMaterial *)NULL;

  p_MultiTexture=FALSE;
  p_EMBMSupported=FALSE;
  p_DOT3Supported=FALSE;
  p_VSHardwareSupport=0;
  p_TextureSubtract=FALSE;
  p_MaxLightsAllowed=0;
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetWorldMatrix (AD_Matrix *matrix)
{
  HRESULT hr;
  AD_Matrix l_m;

  mat_transpose(matrix, &l_m);

  hr=p_Device->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&l_m);
  m_D3DError(hr, "CRenderLib::m_SetWorldMatrix failed!");
  return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetViewMatrix (AD_Matrix *matrix)
{
  HRESULT hr;
  AD_Matrix l_m;

  mat_transpose(matrix, &l_m);

  hr=p_Device->SetTransform(D3DTS_VIEW, (D3DMATRIX *)&l_m);
  m_D3DError(hr, "CRenderLib::m_SetViewMatrix failed!");
  return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetProjMatrix (AD_Matrix *matrix)
{
  HRESULT hr;

  hr=p_Device->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)matrix);
  m_D3DError(hr, "CRenderLib::m_SetProjMatrix(1) failed!");
  return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetProjMatrix (float4 fov, float4 aspect_ratio,
								   float4 znear, float4 zfar)
{
  HRESULT hr;
  D3DMATRIX matrix;
  float4 h, w, Q;
 
  w = (float4)(1/tan(fov*0.5));   // 1/tan(x) == cot(x)
  h = (float4)(1/tan(fov*0.5));   // 1/tan(x) == cot(x)
  h=h*aspect_ratio;  // aspect_ratio=Width/Height
  Q = zfar/(zfar - znear);
 
  ZeroMemory(&matrix, sizeof(matrix));
  matrix._11 = w;
  matrix._22 = h;
  matrix._33 = Q;
  matrix._34 = 1;
  matrix._43 = -Q*znear;

  hr=p_Device->SetTransform(D3DTS_PROJECTION, &matrix);
  m_D3DError(hr, "CRenderLib::m_SetProjMatrix(2) failed!");
  return(1);
}

//--------------------------------------------------------------------

CGraphicSurface *CRenderLib::m_OpenScreen(HWND hWnd,
							 			  D3DModeInfo *screen_mode,
										  BOOL FullScreen)
{
  HRESULT hr;
  BOOL MultiTexture;

  hWindow=hWnd;
  p_FrontBuffer=new CGraphicSurface(screen_mode->Width,
                                    screen_mode->Height,
                                    //screen_mode->Bpp
									32);

  ZeroMemory(&p_PresentParamteres, sizeof(p_PresentParamteres));
  if ((!FullScreen) && (p_DeviceInfo.CanDoWindowed))
	p_PresentParamteres.Windowed=TRUE;
  else p_PresentParamteres.Windowed=FALSE;
  p_PresentParamteres.BackBufferCount        = 1;
  p_PresentParamteres.MultiSampleType        = p_DeviceInfo.MultiSampleType;
  p_PresentParamteres.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  p_PresentParamteres.EnableAutoDepthStencil = TRUE;
  p_PresentParamteres.AutoDepthStencilFormat = screen_mode->DepthStencilFormat;
  p_PresentParamteres.hDeviceWindow          = hWnd;
  p_PresentParamteres.BackBufferWidth  = screen_mode->Width;
  p_PresentParamteres.BackBufferHeight = screen_mode->Height;
  p_PresentParamteres.BackBufferFormat = screen_mode->Format;

  // Create the device
  hr = p_D3D->CreateDevice(D3DADAPTER_DEFAULT, 
	                       p_DeviceType,
                           hWnd,
						   screen_mode->Behavior,
						   &p_PresentParamteres,
                           &p_Device);
  if (hr!=D3D_OK)
  {
    hr = p_D3D->CreateDevice(D3DADAPTER_DEFAULT, 
	                         p_DeviceType,
                             hWnd,
						     D3DCREATE_SOFTWARE_VERTEXPROCESSING,
						     &p_PresentParamteres,
                             &p_Device);
	p_VSHardwareSupport=0;
    m_D3DError(hr, "CRenderLib::m_OpenScreen, p_D3D->CreateDevice failed!");
  }


  // Check per il supporto del multitexture (tex+env)
  if ((p_DeviceInfo.Caps.MaxTextureBlendStages>1)   &&
      (p_DeviceInfo.Caps.MaxSimultaneousTextures>1))
	     MultiTexture=p_DeviceInfo.Caps.MaxSimultaneousTextures;
  else MultiTexture=0;
  
  if (MultiTexture==0)
  {
     m_Close_Screen();
     m_D3DError(hr, "CRenderLib::m_OpenScreen, Your card doesn't support multitexture!");
  }

  // Check per il supporto del'environmental bump mapping (G400, Radeon, GF3, GF4)
  if ((p_DeviceInfo.Caps.TextureOpCaps & (D3DTEXOPCAPS_BUMPENVMAP | D3DTEXOPCAPS_BUMPENVMAPLUMINANCE)) &&
      (p_DeviceInfo.Caps.MaxTextureBlendStages>= 3))
	   p_EMBMSupported=TRUE;
  else p_EMBMSupported=FALSE;
        
  // Check per il supporto del DOT3 bump mapping (GF1/2/3/4, Radeon)  
  if ((p_DeviceInfo.Caps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3) &&
	  (p_DeviceInfo.Caps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP))
	   p_DOT3Supported=TRUE;
  else p_DOT3Supported=FALSE;

  // Check per il supporto del mix type di tipo SUB (sottrattivo)
  if (p_DeviceInfo.Caps.TextureOpCaps & D3DTEXOPCAPS_SUBTRACT)
	   p_TextureSubtract=TRUE;
  else p_TextureSubtract=FALSE;

  p_MaxLightsAllowed=p_DeviceInfo.Caps.MaxActiveLights;

  p_Device->SetRenderState(D3DRS_AMBIENT, 0);

  // RenderState
  p_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
  p_Device->SetRenderState(D3DRS_CLIPPING, TRUE);
  p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  p_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
  p_Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
  p_Device->SetRenderState(D3DRS_DITHERENABLE, TRUE);
  p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  p_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  
  // Stage 0
  p_Device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  p_Device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  p_Device->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  p_Device->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

  // Stage 1
  p_Device->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  p_Device->SetTextureStageState(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  p_Device->SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  p_Device->SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
  p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  p_Device->SetTexture(1, NULL);

  // settaggio delle matrici
  AD_Matrix Identity;
  mat_identity(&Identity);
  m_SetWorldMatrix (&Identity);
  m_SetViewMatrix (&Identity);
  m_SetProjMatrix(1.5708f, 1.0f, 1.0f, 2000.0f);

  m_ClearScreen(0, 0.0f);
  p_VideoMode=*screen_mode;
  return(p_FrontBuffer);
}

//--------------------------------------------------------------------

void CRenderLib::m_Close_Screen(void)
{
  if (p_Device)
  {
     p_Device->Release();
     p_D3D->Release();
     p_Device = NULL;
     p_D3D = NULL;
  }
  if (p_FrontBuffer) delete p_FrontBuffer;
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetViewport (int32 x0, int32 y0, int32 width, int32 height, float4 zmin, float4 zmax)
{
  D3DVIEWPORT8 vp;
  HRESULT hr;

  vp.X=x0;
  vp.Y=y0;
  vp.Width=width;
  vp.Height=height;
  vp.MinZ=zmin;
  vp.MaxZ=zmax;
  hr=p_Device->SetViewport(&vp);
  m_D3DError(hr, "CRenderLib::m_SetViewport(1) failed!!");
  return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetViewport (int32 x0, int32 y0, int32 width, int32 height)
{
  D3DVIEWPORT8 vp;
  HRESULT hr;

  vp.X=x0;
  vp.Y=y0;
  vp.Width=width;
  vp.Height=height;
  vp.MinZ=0.0f;
  vp.MaxZ=1.0f;
  hr=p_Device->SetViewport(&vp);
  m_D3DError(hr, "CRenderLib::m_SetViewport(2) failed!!");
  return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_ClearScreen(int32 ARGB, float4 zclear)
{
  HRESULT hr;

  hr = p_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
	                   ARGB, zclear, 0);
  m_D3DError(hr, "CRenderLib::m_ClearScreen(1) failed!!");
  return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_ClearScreen(int32 A, int32 R, int32 G, int32 B, float4 zclear)
{
  HRESULT hr;
  int32 ARGB;

  A&=255;
  R&=255;
  G&=255;
  B&=255;
  ARGB=(A<<24) | (R<<16) | (G<<8) | B;
  hr = p_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
	                   ARGB, zclear, 0);
  m_D3DError(hr, "CRenderLib::m_ClearScreen(1) failed!!");
  return(1);
}

//--------------------------------------------------------------------

void CRenderLib::m_BeginScene(void)
{
//  if (begin_end_scene==TRUE)
  {
    HRESULT hr = p_Device->BeginScene();
    m_D3DError(hr, "CRenderLib::m_BeginScene, p_Device->BeginScene() failed!");

    hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_D3DError(hr, "CRenderLib::m_BeginScene, D3DRS_ALPHABLENDENABLE failed!");
    hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_D3DError(hr, "CRenderLib::m_BeginScene, D3DRS_ZWRITEENABLE failed!");
  }
}

//--------------------------------------------------------------------

void CRenderLib::m_EndScene(void)
{
//  if (begin_end_scene==FALSE)
  {
	HRESULT hr=p_Device->EndScene();
    m_D3DError(hr, "CRenderLib::m_EndScene, p_Device->EndScene() failed!");
  }
}

//--------------------------------------------------------------------

void CRenderLib::m_UpdateScreen(void)
{
  HRESULT hr;

  hr=p_Device->Present(NULL, NULL, NULL, NULL);
  m_D3DError(hr, "CRenderLib::m_UpdateScreen failed!");
}

//--------------------------------------------------------------------

void CRenderLib::m_BeginExclusiveTransparency(void)
{
  HRESULT hr;

  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  m_D3DError(hr, "CRenderLib::m_BeginExclusiveTransparency, D3DRS_ALPHABLENDENABLE failed!");
  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  m_D3DError(hr, "CRenderLib::m_BeginExclusiveTransparency, D3DRS_ZWRITEENABLE failed!");
}

//--------------------------------------------------------------------

void CRenderLib::m_EndExclusiveTransparency(void)
{
  HRESULT hr;

  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  m_D3DError(hr, "CRenderLib::m_EndExclusiveTransparency, D3DRS_ALPHABLENDENABLE failed!");
  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  m_D3DError(hr, "CRenderLib::m_EndExclusiveTransparency, D3DRS_ZWRITEENABLE failed!");
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMaterial (CMaterial *m)
{
   HRESULT hr;

   ExtDevice=p_Device;
   //ExtTextures=p_Textures;

   // due mappe
   if (m->p_Map1 && m->p_Map2)
   {
	  if (m->p_BumpMap)
	  {
        // verifichiamo il tipo di bump e se è supportato OK!
        // altrimenti niente bump
        if ((m->p_BumpType & BUMP_EMBM) && (p_EMBMSupported))
			m_SetMap12EMBM(m);
		else
	    if ((m->p_BumpType & BUMP_DOT3) && (p_DOT3Supported))
			 m_SetMap12DOT3(m);
	  }
      else m_SetMap12(m);
   }
   else
   if (m->p_Map1)
   {
	  if (m->p_BumpMap)
	  {
	    if ((m->p_BumpType & BUMP_DOT3) && (p_DOT3Supported))
			 m_SetMap1DOT3(m);
	    else m_SetMap1(m);
	  }
	  else m_SetMap1(m);
   }
   else m_SetMap0(m);

   // parti indipendenti dal numero di mappe
   TransparencyAmount=m->p_TransparencyAmount;
   if ((m->p_TransparencyType != OFF) &&
       (m->p_TransparencyType != ALPHA_MAP))
   {
      m->p_D3DMaterial.Diffuse.a=(float4)(m->p_TransparencyAmount/255.0);
      p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	  if (m->p_TransparencyType == MIXTYPE_ALPHABLEND)
        p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	  else
	  if (m->p_TransparencyType == MIXTYPE_ADD)
        p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
      p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
      p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
   }
   else
   {
      if (m->p_TransparencyType == ALPHA_MAP)
	  {
        p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	  }
   }

   hr=p_Device->SetMaterial(&m->p_D3DMaterial);
   m_D3DError(hr, "CRenderLib::m_SetMaterial(1), p_Device->SetMaterial failed!");

   // culling mode
   if (!m->p_BackFaceCulling) p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
   else
   if (m->p_BackFaceCulling==BACKFACE_CULLING)
	    p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
   else p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

   // lighting enabled/disabled
   if (m->p_LightMixType==MIXTYPE_NONE) p_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
   else
   {
	  p_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
      // specular component
      if (m->p_Specular==ON) p_Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
      else p_Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	  // --------------------   mixing di luce   ---------------------
	  if (m->p_LightMixType==MIXTYPE_ADD)
        hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	  else
	  if ((m->p_LightMixType==MIXTYPE_BLEND50) || (m->p_LightMixType==MIXTYPE_BLEND3DS))
        hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      else
	  if (m->p_LightMixType==MIXTYPE_SUB)
        hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
   }

   // vertex shader
   hr=p_Device->SetVertexShader(m->p_FVF);
   m_D3DError(hr, "CRenderLib::m_SetMaterial(1), p_Device->SetVertexShader failed!");
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMaterial (CMaterial *m, int32 pass)
{
   if ((pass<0) || (pass>=m->p_NumPasses)) return;
   if (!m) return;

   if (pass==0) m_SetMaterial(m);
   else
   {
      if (m->p_NextMaterial) m_SetMaterial(m->p_NextMaterial);
   }
}