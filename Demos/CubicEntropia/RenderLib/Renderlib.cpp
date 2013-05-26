#include <math.h>
#include "RenderLib.h"
#include "Dot3Toon_VS.h"

FILE *fdebug=NULL;

CRenderLib::CRenderLib()
{
  int32 i;

  p_BeginEnd_Scene=0;
  p_IsDeviceCreated=0;
  p_IsEnumerationDone=0;

  p_D3D=(LPDIRECT3D8)NULL;
  p_Device=(LPDIRECT3DDEVICE8)NULL;
  p_FrontBuffer=(CGraphicSurface *)NULL;

  p_OldFrontSurface=(IDirect3DSurface8 *)NULL;
  p_OldDepthSurface=(IDirect3DSurface8 *)NULL;
  p_FrontSurfaceBlur=(IDirect3DSurface8 *)NULL;
  p_DepthSurfaceBlur=(IDirect3DSurface8 *)NULL;
  p_MotionBlurSupported=FALSE;
  p_FlipBufferBC=0;
  p_BlurLevel=0;

  p_BlurType=NOBLUR;
  p_OldTextureBlur=(LPDIRECT3DTEXTURE8)NULL;
  p_OldSurfaceBlur=(IDirect3DSurface8 *)NULL;
  p_RadialBlurSupported=FALSE;

  p_NormalizationCubeMapTexture=(LPDIRECT3DCUBETEXTURE8)NULL;

  p_NumMaterials=0;
  for (i=0; i<MAX_MATERIALS; i++)
    p_Materials[i]=(CMaterial *)NULL;

  p_MultiTexture=FALSE;
  p_EMBMSupported=FALSE;
  p_DOT3Supported=FALSE;
  p_VSHardwareSupport=0;
  p_MaxLightsAllowed=0;

  p_UV1DOT3Code=(DWORD *)NULL;
  p_UV2DOT3Code=(DWORD *)NULL;
  p_TOONCode_Pass1=(DWORD *)NULL;
  p_TOONCode_PassLights=(DWORD *)NULL;
  p_TOONCode_FinalPass=(DWORD *)NULL;
  p_UV1DOT3Handle=0;
  p_UV2DOT3Handle=0;
  p_TOONPass1Handle=0;
  p_TOONPassLightsHandle=0;
  p_TOONFinalPassHandle=0;
  for (i=0; i< 8; i++) p_UV1DOT3Declaration[i]=0;
  for (i=0; i< 8; i++) p_UV2DOT3Declaration[i]=0;

  mat_identity(&p_TextureIDMatrix);
  p_TextureIDMatrix.a[1][1]=-1;  // le v vanno swappate (colpa del MAX)
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

  fdebug=fopen("RenderLog.txt", "wt");

  CoInitialize(NULL);
  DumpDebugPoint(fdebug, "CoInitialize(NULL)", 0);

  hWindow=hWnd;
  p_FrontBuffer=new CGraphicSurface(screen_mode->Width,
                                    screen_mode->Height,
                                    32);

  ZeroMemory(&p_PresentParamteres, sizeof(p_PresentParamteres));
  if ((!FullScreen) && (p_DeviceInfo.CanDoWindowed))
	p_PresentParamteres.Windowed=TRUE;
  else p_PresentParamteres.Windowed=FALSE;
  p_PresentParamteres.BackBufferCount        = 1;
  p_PresentParamteres.MultiSampleType        = p_DeviceInfo.MultiSampleType;
  p_PresentParamteres.SwapEffect             = D3DSWAPEFFECT_FLIP;//D3DSWAPEFFECT_DISCARD;
  p_PresentParamteres.EnableAutoDepthStencil = TRUE;
  p_PresentParamteres.AutoDepthStencilFormat = screen_mode->DepthStencilFormat;
  //p_PresentParamteres.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
  //p_PresentParamteres.FullScreen_PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
  //p_PresentParamteres.FullScreen_PresentationInterval=D3DPRESENT_INTERVAL_ONE;

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
  DumpDebugPoint(fdebug, "CreateDevice", 0);


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
  {
	   p_DOT3Supported=TRUE;
       m_BuildNormalizationCubemap(256, 1);
  } else p_DOT3Supported=FALSE;


  p_MaxLightsAllowed=p_DeviceInfo.Caps.MaxActiveLights;
  // i device che non hanno T&L hanno questo caps pari a 0
  if (p_MaxLightsAllowed <= 0)
     p_MaxLightsAllowed=8; // setto 8 luci come per le schede
                           // che hanno T&L

  // luce ambientale nulla
  p_Device->SetRenderState(D3DRS_AMBIENT, 0);

  // RenderState
  hr=p_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
  hr=p_Device->SetRenderState(D3DRS_CLIPPING, TRUE);
  hr=p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  hr=p_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
  hr=p_Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
  hr=p_Device->SetRenderState(D3DRS_DITHERENABLE, TRUE);
  hr=p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  hr=p_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

  DumpDebugPoint(fdebug, "Render states done", 0);

  // Stage 0
  hr=p_Device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  hr=p_Device->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX *)&p_TextureIDMatrix);
  DumpDebugPoint(fdebug, "Texture stage 0 done", 0);

  // Stage 1
  hr=p_Device->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(1, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
  hr=p_Device->SetTexture(1, NULL);
  hr=p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  hr=p_Device->SetTransform(D3DTS_TEXTURE1, (D3DMATRIX *)&p_TextureIDMatrix);
  DumpDebugPoint(fdebug, "Texture stage 1 done", 0);

  // Stage 2
  hr=p_Device->SetTextureStageState(2, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(2, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(2, D3DTSS_MIPFILTER, D3DTEXF_LINEAR);
  hr=p_Device->SetTextureStageState(2, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(2, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=p_Device->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
  hr=p_Device->SetTexture(2, NULL);
  hr=p_Device->SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  hr=p_Device->SetTransform(D3DTS_TEXTURE2, (D3DMATRIX *)&p_TextureIDMatrix);
  DumpDebugPoint(fdebug, "Texture stage 2 done", 0);

  // creazione e loading degli shaders (vertex shaders)
  DumpDebugPoint(fdebug, "Begin shader creation", 0);
  m_LoadAndCreateShaders();
  DumpDebugPoint(fdebug, "End shader creation", 0);

  p_VideoMode=*screen_mode;
  DumpDebugPoint(fdebug, "Begin blur(s) creation", 0);
  m_SetUpBlurs();
  DumpDebugPoint(fdebug, "Blur(s) creation done", 0);

  // settaggio delle matrici
  AD_Matrix Identity;
  mat_identity(&Identity);
  m_SetWorldMatrix (&Identity);
  m_SetViewMatrix (&Identity);
  m_SetProjMatrix(1.5708f, 1.0f, 1.0f, 2000.0f);
  DumpDebugPoint(fdebug, "Base matrix cration done", 0);
  return(p_FrontBuffer);
}

//--------------------------------------------------------------------

void CRenderLib::m_Close_Screen(void)
{
  if (p_Device)
  {
	 if (p_OldFrontSurface) p_OldFrontSurface->Release();
	 if (p_OldDepthSurface) p_OldDepthSurface->Release();

     if (p_FrontSurfaceBlur) p_FrontSurfaceBlur->Release();
     if (p_DepthSurfaceBlur) p_DepthSurfaceBlur->Release();
     if (p_OldSurfaceBlur) p_OldSurfaceBlur->Release();
     if (p_CopySurfaceBlur) p_CopySurfaceBlur->Release();

	 if (p_Device) p_Device->Release();
     if (p_D3D) p_D3D->Release();
     p_Device = NULL;
     p_D3D = NULL;
	 if (p_UV1DOT3Code) delete [] p_UV1DOT3Code;
	 if (p_UV2DOT3Code) delete [] p_UV2DOT3Code;
     if (p_TOONCode_Pass1) delete [] p_TOONCode_Pass1;
     if (p_TOONCode_PassLights) delete [] p_TOONCode_PassLights;

	 if (fdebug) fclose(fdebug);
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

  hr = p_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER /*| D3DCLEAR_STENCIL*/,
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
  hr = p_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER/* | D3DCLEAR_STENCIL*/,
	                   ARGB, zclear, 0);
  m_D3DError(hr, "CRenderLib::m_ClearScreen(1) failed!!");
  return(1);
}

//--------------------------------------------------------------------

void CRenderLib::m_BeginScene(void)
{
  HRESULT hr;

  if ((p_BlurType!=NOBLUR) && (p_BlurLevel>0))
  {
    if ((p_BlurType & MOTIONBLUR) && (p_MotionBlurSupported))
	{
       hr=p_Device->SetRenderTarget(p_FrontSurfaceBlur, p_DepthSurfaceBlur);
	}
    else
    if ((p_BlurType & RADIALBLUR) && (p_RadialBlurSupported))
	{
       hr=p_Device->SetRenderTarget(p_FrontSurfaceBlur, p_DepthSurfaceBlur);
	}
	else
     hr=p_Device->SetRenderTarget(p_OldFrontSurface, p_OldDepthSurface);
  }
  else hr=p_Device->SetRenderTarget(p_OldFrontSurface, p_OldDepthSurface);

  m_D3DError(hr, "CRenderLib::m_BeginScene setting target failed!!");

  hr = p_Device->BeginScene();
  m_D3DError(hr, "CRenderLib::m_BeginScene, p_Device->BeginScene() failed!");
  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  m_D3DError(hr, "CRenderLib::m_BeginScene, D3DRS_ALPHABLENDENABLE failed!");
  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  m_D3DError(hr, "CRenderLib::m_BeginScene, D3DRS_ZWRITEENABLE failed!");
}

//--------------------------------------------------------------------

void CRenderLib::m_EndScene(void)
{
  HRESULT hr=p_Device->EndScene();
  m_D3DError(hr, "CRenderLib::m_EndScene, p_Device->EndScene() failed!");
}

//--------------------------------------------------------------------

void CRenderLib::m_UpdateScreen(void)
{
  HRESULT hr;

  if ((p_BlurLevel>0) && (p_BlurType!=NOBLUR))
  {
    if ((p_BlurType & MOTIONBLUR) && (p_MotionBlurSupported))
        m_UpdateScreenBlur();
    else
    if ((p_BlurType & RADIALBLUR) && (p_RadialBlurSupported))
        m_UpdateScreenBlur();
	else
	{
      hr=p_Device->Present(NULL, NULL, NULL, NULL);
      m_D3DError(hr, "CRenderLib::m_UpdateScreen failed!");
	}
  }
  else
  {
     hr=p_Device->Present(NULL, NULL, NULL, NULL);
     m_D3DError(hr, "CRenderLib::m_UpdateScreen failed!");
  }
}

//--------------------------------------------------------------------

void CRenderLib::m_BeginTransparency(void)
{
  HRESULT hr;

  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  m_D3DError(hr, "CRenderLib::m_BeginExclusiveTransparency, D3DRS_ALPHABLENDENABLE failed!");
  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  m_D3DError(hr, "CRenderLib::m_BeginExclusiveTransparency, D3DRS_ZWRITEENABLE failed!");
}

//--------------------------------------------------------------------

void CRenderLib::m_EndTransparency(void)
{
  HRESULT hr;

  hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  m_D3DError(hr, "CRenderLib::m_EndExclusiveTransparency, D3DRS_ALPHABLENDENABLE failed!");
  hr=p_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  m_D3DError(hr, "CRenderLib::m_EndExclusiveTransparency, D3DRS_ZWRITEENABLE failed!");
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetMaterialFVF (CMaterial *m, int32 pass)
{
   HRESULT hr;
   DWORD factor;

   //hr=p_Device->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING, FALSE);

   // we have 2 maps
   if (m->p_Map1 && m->p_Map2)
   {
	  if (m->p_BumpMap)
	  {
        // check if the specified bump type is supported
        if ((m->p_BumpType & BUMP_EMBM) && (p_EMBMSupported))
			m_SetMap12EMBM(m);
		else m_SetMap12(m);
	  } // two maps with no bumpmap applied
      else m_SetMap12(m);
   }
   else if (m->p_Map1) m_SetMap1(m); // just one map
        else m_SetMap0(m);  // no maps


   // ***************************************************************
   // -------------  maps indipendent sections  ---------------------
   // ***************************************************************
   if ((m->p_TransparencyType != OFF) &&
       (m->p_TransparencyType != ALPHA_MAP) &&
	   (m->p_TransparencyType != ALPHA_MAP1))
   {
      // provo entrambi i metodi
      m->p_D3DMaterial.Diffuse.a=m->p_TransparencyAmount;
	  factor=(factor & 0x00FFFFFF) | (((uint32)(255.0f*m->p_TransparencyAmount))<<24);
      p_Device->SetRenderState(D3DRS_TEXTUREFACTOR, factor);
      hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	  if (m->p_TransparencyType == MIXTYPE_ALPHABLEND)
        hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	  else
	  if (m->p_TransparencyType == MIXTYPE_ADD)
        hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

      if (m->p_LightMixType!=MIXTYPE_NONE)
        hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
      else
		hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
      hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
   }
   else
   if ((m->p_TransparencyType == ALPHA_MAP) ||
	   (m->p_TransparencyType == ALPHA_MAP1))
   {
     hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
     hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
     hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
     hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
   }
   else hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

   hr=p_Device->SetMaterial(&m->p_D3DMaterial);
   m_D3DError(hr, "CRenderLib::m_SetMaterial(1), p_Device->SetMaterial failed!");

   // ----------------------  filling mode  ------------------------   
   if (m->p_WireFrame)
	   hr=p_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
   else hr=p_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

   // ----------------------  culling mode  ------------------------
   if (!m->p_BackFaceCulling)
	   p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
   else if (m->p_BackFaceCulling==BACKFACE_CULLING)
	      p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        else p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


   // ------------------  fog enabled/disabled  ----------------------
   if (m->p_FogType!=OFF)
   {
     hr=p_Device->SetRenderState(D3DRS_FOGENABLE, TRUE);
     hr=p_Device->SetRenderState(D3DRS_FOGCOLOR, m->p_FogColor);
     if (m->p_FogType==LINEARFOG)
	 {
       hr=p_Device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
       hr=p_Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
       hr=p_Device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&m->p_FogStart));
       hr=p_Device->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&m->p_FogEnd));
	 }
     else
	 if (m->p_FogType==EXPFOG)
	 {
       hr=p_Device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
       hr=p_Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);
	   hr=p_Device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&m->p_FogDensity));
	 }
     else
	 if (m->p_FogType==EXP2FOG)
	 {
       hr=p_Device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
       hr=p_Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP2);
	   hr=p_Device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&m->p_FogDensity));
	 }
   }
   else
   {
     hr=p_Device->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
     hr=p_Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
     hr=p_Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
   }

   // ----------------  lighting enabled/disabled  -------------------
   if (m->p_LightMixType==MIXTYPE_NONE)
   {
	  p_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
      hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
   }
   else
   {
	  p_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
      // specular component
      if (m->p_Specular==ON) p_Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
      else p_Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	  // --------------------   light mixing    ---------------------
      hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, m_MixType2ColorOp(m->p_LightMixType));
   }
   return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetMaterialVS (CMaterial *m, int32 pass)
{
  HRESULT hr;
  D3DLIGHT8 light;
  BOOL enable;
  int32 i, j, k;
  AD_Matrix matWorld, matView, matProj;
  AD_Matrix matPVW, mat, scale;


  //hr=p_Device->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING, TRUE);

  k=m_GetMaterialPasses(m);
  // *****************************************************************
  //             sezioni indipendendi dal vertex shader
  // *****************************************************************
  if ((m->p_TransparencyType != OFF) &&
      (m->p_TransparencyType != ALPHA_MAP) &&
	  (m->p_TransparencyType != ALPHA_MAP1))
  {
    hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	if (m->p_TransparencyType == MIXTYPE_ALPHABLEND)
      hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	else
	if (m->p_TransparencyType == MIXTYPE_ADD)
      hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  }
  else
  if ((m->p_TransparencyType == ALPHA_MAP) ||
	  (m->p_TransparencyType == ALPHA_MAP1))
  {
    hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    hr=p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    hr=p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    hr=p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  }
  else hr=p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

   // ----------------------  filling mode  ------------------------   
   if (m->p_WireFrame)
	   hr=p_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
   else hr=p_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

  // *****************************************************************
  // **************************   BUMP Dot3   ************************
  // *****************************************************************
  // per il DOT3 dobbiamo settare la luce giusta per la passata corrente
  if ((m->p_BumpType & BUMP_DOT3) && (p_DOT3Supported))
  {
    if (!m->p_BackFaceCulling) p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    else if (m->p_BackFaceCulling==BACKFACE_CULLING)
	       p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
         else p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    // ultima passata del DOT3
	if (k-1==pass)
	{
        if ((!m->p_Map1) || (m->p_Map1Channel!=TEXMAPPING1)) return(0);
		m_Map1DOT3FinalPass(m);
		return(1);
	}
	else
	{
      // estraiamo la luce per la passata corrente (che va da 0 a
      // numero_di_luci_attive-1)
      j=-1;
      for (i=0; i<p_MaxLightsAllowed; i++)
	  {
        enable=FALSE;
        p_Device->GetLightEnable(i, &enable);
	    if (enable) j++;
        if (j==pass) goto dopo;
	  }
      dopo:
      ;
      if (j<0) return(0); // se nn ci sono luci non bisogna
	                      // fare le passate di dot3
      hr=p_Device->GetLight(j, &light);
	  // per la prima luce/passata settiamo le costanti del vertex
	  // shader
      if (pass==0)
	  {
        hr=p_Device->GetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&matProj);
        hr=p_Device->GetTransform(D3DTS_VIEW, (D3DMATRIX *)&matView);
        hr=p_Device->GetTransform(D3DTS_WORLD, (D3DMATRIX *)&matWorld);
		mat_mul(&matView, &matProj, &matPVW);
		mat_mul(&matWorld, &matPVW, &matPVW);
		mat_transpose(&matPVW, &mat);
        hr=p_Device->SetVertexShaderConstant(CV_WORLDVIEWPROJ_0, &mat.a[0][0], 4);
		mat_transpose(&matWorld, &mat);
        hr=p_Device->SetVertexShaderConstant(CV_WORLD_0, &mat.a[0][0], 4);
        m_DOT3PassLights(m, 1, &light);
		return(1);
	  }
	  else
	  {
		m_DOT3PassLights(m, 0, &light);
		return(1);
	  }
	}
  }
  else
  // *****************************************************************
  // ************************   TOON shading   ***********************
  // *****************************************************************
  if (m->p_LightMixType==MIXTYPE_TOONSHADE)
  {
     // la prima passata vuole il modello scalato di un epsilon in piu'
	 if (pass==0) mat_setmatrix_of_scaling(&scale, m->p_ToonScale, m->p_ToonScale, m->p_ToonScale);
	 else mat_setmatrix_of_scaling(&scale, 1, 1, 1);

     hr=p_Device->GetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&matProj);
     hr=p_Device->GetTransform(D3DTS_VIEW, (D3DMATRIX *)&matView);
     hr=p_Device->GetTransform(D3DTS_WORLD, (D3DMATRIX *)&matWorld);
	 mat_mul(&matView, &matProj, &matPVW);
	 mat_mul(&matWorld, &matPVW, &matPVW);
	 mat_mul(&scale, &matPVW, &matPVW);
	 mat_transpose(&matPVW, &mat);
     hr=p_Device->SetVertexShaderConstant(CV_WORLDVIEWPROJ_0, &mat.a[0][0], 4);

     // prima passata: rendering con scaling di offset e colore nero
     // per la silouette
     if (pass==0)
	 {
		// disegno le facce non visibili
        p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
        m_SetToonPass1();
		return(1);
	 }
	 else // passata finale con la texture
	 if (k-1==pass)
	 {
        m_SetToonFinalPass(m);
        if ((!m->p_Map1) || (m->p_Map1Channel!=TEXMAPPING1)) return(0);
		else return(1);
	 }
	 // passata delle luci
	 else
	 {
        // estraiamo la luce per la passata corrente (che va da 1 a
		// numero_di_luci_attive)
        j=-1;
        for (i=0; i<p_MaxLightsAllowed; i++)
		{
          enable=FALSE;
          p_Device->GetLightEnable(i, &enable);
	      if (enable) j++;
          if (j==pass-1) goto dopo2;
		}
        dopo2:
        ;
        if (j<0) return(0); // se non ci sono luci non bisogna fare
		                    // passate di disegno
        hr=p_Device->GetLight(j, &light);
        if (m->p_BackFaceCulling==BACKFACE_CULLING)
	       p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        else p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		if (pass==1) m_SetToonPassLights(m, 1, &light);
		else m_SetToonPassLights(m, 0, &light);
		return(1);
	 }
  }
}

//--------------------------------------------------------------------

int32 CRenderLib::m_GetMaterialPasses(CMaterial *m)
{
  int32 i, j;
  BOOL enable;

  if (!m) return(0);

  if (m->p_BumpType & BUMP_DOT3)
  {
     j=0;
     for (i=0; i<p_MaxLightsAllowed; i++)
	 {
       enable=FALSE;
       p_Device->GetLightEnable(i, &enable);
	   if (enable) j++;
	 }
	 return(j*m->p_NumPasses+1);
  }
  else
  if (m->p_LightMixType==MIXTYPE_TOONSHADE)
  {
     j=0;
     for (i=0; i<p_MaxLightsAllowed; i++)
	 {
       enable=FALSE;
       p_Device->GetLightEnable(i, &enable);
	   if (enable) j++;
	 }
	 return(1+j*m->p_NumPasses+1);
  }
  // caso di shader FVF compliant
  else
  {
    if (m->p_NextMaterial)
	  return(m->p_NumPasses + m->p_NextMaterial->p_NumPasses);
	else
	  return(m->p_NumPasses);
  }
}

//--------------------------------------------------------------------

int32 CRenderLib::m_SetMaterial (CMaterial *m, int32 pass)
{
  HRESULT hr;
  CMaterial *material;
  int32 res;

  if ((!m) || (pass<0)) return(0);

  // vertex shader: non si stratifica
  if (m->p_InputVertexFormat.FVF==0)
    return(m_SetMaterialVS(m, pass));
  // pipeline fissa, no vertex shaders
  else
  {
    // dobbiamo risalire dalla passata globale a quella locale del
    // materiale giusto
    if (m->p_NextMaterial)
	{
       if (pass >= m->p_NumPasses)
	   {
         // TO DO: abilitare l'alpha blending
		 pass-=m->p_NumPasses;
		 material=m->p_NextMaterial;
         res=m_SetMaterialFVF(material, pass);
         hr=p_Device->SetVertexShader(material->p_InputVertexFormat.FVF);
         m_D3DError(hr, "CRenderLib::m_SetMaterial(1), p_Device->SetVertexShader(FVF) failed!");
		 return(res);
	   }
	}
	else
	{
      // in questo caso non c'è il NextMaterial, quindi la passata
      // globale è anche locale
      res=m_SetMaterialFVF(m, pass);
      hr=p_Device->SetVertexShader(m->p_InputVertexFormat.FVF);
      m_D3DError(hr, "CRenderLib::m_SetMaterial(1), p_Device->SetVertexShader(FVF) failed!");
	  return(res);
	}
  }
}

//--------------------------------------------------------------------

void DumpDebugPoint (FILE *f, char8 *msg, HRESULT hr)
{
  if ((!f) || (!msg)) return;

  fprintf(f, "%s, hr=%d\n", msg, hr);
  fflush(f);
}