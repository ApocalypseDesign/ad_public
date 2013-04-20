#include <windows.h>
#define WIN32_LEAN_AND_MEAN	
#define D3D_OVERLOADS // deve stare prima di #include <d3d.h>
#include "..\ADrender.h"
#include "..\D3Dutility.h"
#include "..\ADbase.h"
#include "rasters.h"
#include "setmat.h"
#include "extrend.h"

#define MAXTEXTURES 100
extern unsigned int current_transparency;

float TEXTURE_PRECISION = 1.0f;
float ENVMAP_PRECISION  = 1.0f/2.0f;
float RGB_PRECISION     = 1.0f;
float RGB_MAXVALUE      = 1.0f*255.0f;

char enumeration_done=0;    // =0 se non e' stata fatta l'enumerazione
LPDIRECT3D8                 g_pD3D=NULL;
LPDIRECT3DDEVICE8           g_pd3dDevice=NULL;
D3DPRESENT_PARAMETERS       g_d3dpp;
D3DFORMAT                   video_format;
LPDIRECT3DTEXTURE8        g_pD3DTexture[MAXTEXTURES];
int num_texture;

HWND hWindow;
imagebuffer image;
material *matold;
float cur_znear=1.0f, cur_zfar=1000.0f;
D3DMATRIX matProj;

// Features supoortate (o non) dalla scheda 3D
BOOL singlepass_multitexture_supported=FALSE;
BOOL Wbuffer_supported=FALSE;
BOOL EMBM_supported=FALSE;
BOOL DOT3_supported=FALSE;
BOOL subtract_supported=FALSE;


// serve per sincronizzarsi, ovvero che ogni Begin_Scene viene
// prima di ogni End_Scene e che per fare una End_Scene si deve
// aver fatto la Begin_Scene
BOOLEAN begin_end_scene=TRUE;
FILE *fdebug;


int Enumerate_DrawDriver (D3DDEVICEINFO **tab_dev)
{
//  fdebug=fopen("libHWlog.txt", "wt");
//  if (fdebug==NULL) debug_error_hwlib(hWindow, "crezione file di log non riuscita");
//  fprintf(fdebug, "ADrender Hardware Lib log\n");
//  fflush(fdebug);
  return (EnumerateDriver(tab_dev));
}


HRESULT SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,
                                     FLOAT fNearPlane, FLOAT fFarPlane )
{
    if( fabs(fFarPlane-fNearPlane) < 0.01f )
        return E_INVALIDARG;
    if( fabs(sin(fFOV/2)) < 0.01f )
        return E_INVALIDARG;

    FLOAT w = fAspect * ( cosf(fFOV/2)/sinf(fFOV/2) );
    FLOAT h =   1.0f  * ( cosf(fFOV/2)/sinf(fFOV/2) );
    FLOAT Q = fFarPlane / ( fFarPlane - fNearPlane );

    ZeroMemory( &mat, sizeof(D3DMATRIX) );
    mat._11 = w;
    mat._22 = h;
    mat._33 = Q;
//    mat._34 = 1.0f;
    mat._43 = -Q*fNearPlane;
    mat._44=1.0f;
    return S_OK;
}


imagebuffer *Open_Screen (HWND hWnd, D3DDEVICEINFO *devinf)
{
  HRESULT                     hr;

  // controlli iniziali
  if (devinf==NULL) return(NULL);
  hWindow=hWnd;

  image.width=devinf->WantedMode.Width;
  image.height=devinf->WantedMode.Height;
  image.channelsize=image.width*image.height;
  image.muly=new int[image.height];
  for (int j=0; j<image.height; j++) image.muly[j]=j*image.width;
  image.uchar8ptr=(unsigned char *)NULL;
  image.uint32ptr=(unsigned int *)NULL;
  image.voidptr=(void *)NULL;
//  fprintf(fdebug, "Image buffer creted...OK\n");
//  fflush(fdebug);

  ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
  if ((!devinf->bFullscreen) && (devinf->bCanDoWindowed)) g_d3dpp.Windowed = TRUE;
  else g_d3dpp.Windowed = FALSE;
  g_d3dpp.BackBufferCount        = 1;
  g_d3dpp.MultiSampleType        = devinf->MultiSampleType;
  g_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  g_d3dpp.EnableAutoDepthStencil = TRUE;
  g_d3dpp.AutoDepthStencilFormat = devinf->WantedMode.DepthStencilFormat;
  g_d3dpp.hDeviceWindow          = hWnd;
  if (!devinf->bFullscreen)
  {
    g_d3dpp.BackBufferWidth  = devinf->WantedMode.Width;
    g_d3dpp.BackBufferHeight = devinf->WantedMode.Height;
    g_d3dpp.BackBufferFormat = devinf->WantedMode.Format;
  }
  else
  {
    g_d3dpp.BackBufferWidth  = devinf->WantedMode.Width;
    g_d3dpp.BackBufferHeight = devinf->WantedMode.Height;
    g_d3dpp.BackBufferFormat = devinf->WantedMode.Format;
  }

  // Create the device
  hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
	                        devinf->DeviceType,
                            hWnd,
							devinf->WantedMode.dwBehavior,
							&g_d3dpp,
                            &g_pd3dDevice);
  check_dx_lib(hWindow, hr, "g_pD3D->CreateDevice");
  video_format=devinf->WantedMode.Format;

  // Check per il supporto del multitexture (tex+env)
  if ((devinf->d3dCaps.MaxTextureBlendStages>1)   &&
      (devinf->d3dCaps.MaxSimultaneousTextures>1))
	     singlepass_multitexture_supported=TRUE;
  else singlepass_multitexture_supported=FALSE;
  
  if (singlepass_multitexture_supported==FALSE)
  {
     Close_Screen();
     debug_error_lib(hWindow, "Your 3D Graphics Board do not support multitexture");
  }

  // Check per il supporto del'environmental bump mapping (G400, Radeon, Gf3)
  if ((devinf->d3dCaps.TextureOpCaps & (D3DTEXOPCAPS_BUMPENVMAP | D3DTEXOPCAPS_BUMPENVMAPLUMINANCE)) &&
      (devinf->d3dCaps.MaxTextureBlendStages>= 3)) EMBM_supported=TRUE;
  else EMBM_supported=FALSE;
        
  // Check per il supporto del DOT3 bump mapping (Gf1/2/3, Radeon)  
  if (devinf->d3dCaps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3)
	   DOT3_supported=TRUE;
  else DOT3_supported=FALSE;

  // Check per il supporto del mix type di tipo SUB (sottrattivo)
  if (devinf->d3dCaps.TextureOpCaps & D3DTEXOPCAPS_SUBTRACT)
	   subtract_supported=TRUE;
  else subtract_supported=FALSE;


  // RenderState
  g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
  g_pd3dDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  //g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);

  g_pd3dDevice->SetTextureStageState(1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
  g_pd3dDevice->SetTextureStageState(1, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
  g_pd3dDevice->SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  g_pd3dDevice->SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
  g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
  g_pd3dDevice->SetTexture(1, NULL);

  SetProjectionMatrix(matProj, 3.1415f, 1.0f, cur_znear, cur_zfar);
  hr=g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
  check_dx_lib(hWnd, hr, "Device->SetTransform");
//  fprintf(fdebug, "Projection matrix setted...OK\n");

/*
  float fStart = 8.06f,
        fEnd   = 1550.18f;
  hr=g_pDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, 0x00ADBEEF);
  hr=g_pDevice->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, D3DFOG_LINEAR);
  hr=g_pDevice->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE,  D3DFOG_NONE);
  hr=g_pDevice->SetRenderState(D3DRENDERSTATE_FOGSTART, *(DWORD *)(&fStart));
  hr=g_pDevice->SetRenderState(D3DRENDERSTATE_FOGEND,   *(DWORD *)(&fEnd));
*/

  num_texture=0;
  HW_numvert=0;
  matold=(material *)NULL;
  begin_end_scene=TRUE;
  HW_texrgb_arrays[0]=new TEX1RGBVERTEX[8192*3];
  HW_texrgb_arrays[1]=new TEX1RGBVERTEX[8192*3];
  HW_texrgb=HW_texrgb_arrays[0];
  HW_texenv_arrays[0]=new TEX2RGBVERTEX[8192*3];
  HW_texenv_arrays[1]=new TEX2RGBVERTEX[8192*3];
  HW_texenv=HW_texenv_arrays[0];
  Clear_DepthBuffer(0);

//  fprintf(fdebug, "Lib closed...OK\n");
//  fflush(fdebug);
  return(&image);
}


void Begin_Scene()
{
  if (begin_end_scene==TRUE)
  {
    matold=(material *)NULL;
	begin_end_scene=FALSE;
    HRESULT hr = g_pd3dDevice->BeginScene();
    check_dx_lib(hWindow, hr, "BeginScene");
    HW_numvert=0;

    hr=g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    check_dx_lib(hWindow, hr, "Begin_Scene: alphablend");
    hr=g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    check_dx_lib(hWindow, hr, "Begin_Scene: zwriteenable");
  }
}


int AddUpdate_Material (material *m)
{
  char errmsg[70]="";
  unsigned char p;
  int xy, i;

  if (m==NULL) debug_error_lib(hWindow, "AddUpdate_Material: material NULL(not allocated)");
  if (m->name!=(char *)NULL) strcpy(errmsg, m->name);
  strcpy(errmsg, ": ");

  // le mappe bump devono essere delle 8bit a scale di grigio
  if ((m->bump_ptr!=(texture *)NULL) && (m->bump_ptr->type!=GREYSCALE))
  {
     strcat(errmsg, "Bump map MUST be an 8-bit grayscale texture");
     debug_error_lib(hWindow, errmsg);
  }

  // l'environmental bump mapping necessita di un'envmap
  if ((m->bump_ptr!=(texture *)NULL) && (m->bumptype==BUMP_EMBM))
  {
	 if (m->envmap_ptr==(texture *)NULL)
	 {
       strcat(errmsg, "Environmental Bump needs an envmap texture");
       debug_error_lib(hWindow, errmsg);
	 }
  }

  // le mappe alpha devono essere delle 8bit a scale di grigio
  if ((m->alphamap_ptr!=(texture *)NULL) && (m->alphamap_ptr->type!=GREYSCALE))
  {
     strcat(errmsg, "Alpha map MUST be an 8-bit grayscale texture");
     debug_error_lib(hWindow, errmsg);
  }

  // la mappa alpha e la texture devono avere stesse dimensioni
  // TEX + MAPPA ALPHA
  if ((m->alphamap_ptr!=(texture *)NULL) && (m->texture_ptr!=(texture *)NULL))
  {
     if ((m->alphamap_ptr->dimx!=m->texture_ptr->dimx) ||
         (m->alphamap_ptr->dimy!=m->texture_ptr->dimy))
	 {
        strcat(errmsg, "Texture and alpha map MUST have same dimensions(width and height)");
        debug_error_lib(hWindow, errmsg);
	 }
	 else
	 {
		 // mette nel canale alpha della texture il valore del
		 // pixel corrispondente della mappa alpha
		 xy=m->alphamap_ptr->dimx*m->alphamap_ptr->dimy;
		 for (i=0; i<xy; i++)
		 {
            p=m->alphamap_ptr->map[i];
            m->texture_ptr->map32[i] = (m->texture_ptr->map32[i] & 0x00FFFFFF) | (((int)p) << 24);
		 }
	 }
     AddUpdate_AlphaTexture(m->texture_ptr);
  }
  else
  // ENV + MAPPA ALPHA
  if ((m->alphamap_ptr!=(texture *)NULL) && (m->texture_ptr==(texture *)NULL) &&
	  (m->envmap_ptr!=(texture *)NULL))
  {
     if ((m->alphamap_ptr->dimx!=m->envmap_ptr->dimx) ||
         (m->alphamap_ptr->dimy!=m->envmap_ptr->dimy))
	 {
        strcat(errmsg, "Envmap and alpha map MUST have same dimensions(width and height)");
        debug_error_lib(hWindow, errmsg);
	 }
	 else
	 {
		 // mette nel canale alpha dell'envmap il valore del
		 // pixel corrispondente della mappa alpha
		 xy=m->alphamap_ptr->dimx*m->alphamap_ptr->dimy;
		 for (i=0; i<xy; i++)
		 {
            p=m->alphamap_ptr->map[i];
            m->envmap_ptr->map32[i] = (m->envmap_ptr->map32[i] & 0x00FFFFFF) | (((int)p) << 24);
		 }
	 }
     AddUpdate_AlphaTexture(m->envmap_ptr);
  }


  if ((m->alphamap_ptr==(texture *)NULL) && (m->texture_ptr!=(texture *)NULL))
	  AddUpdate_Texture(m->texture_ptr);

  if ((m->alphamap_ptr==(texture *)NULL) && (m->envmap_ptr!=(texture *)NULL))
	  AddUpdate_Texture(m->envmap_ptr);
  else
  if ((m->alphamap_ptr!=(texture *)NULL) && (m->texture_ptr!=(texture *)NULL) &&
	  (m->envmap_ptr!=(texture *)NULL))
	  AddUpdate_Texture(m->envmap_ptr);

  set_material_procedures(m);
  return(0);
}


int AddUpdate_Texture (texture *t)
{
  HRESULT hr;
  char errmsg[90]="";

  if (t==NULL) debug_error_lib(hWindow, "AddUpdate_Texture: texture passed is NULL");
  if (is_texture_pow2(t)==FALSE)
  {
     strcpy(errmsg, t->name);
	 strcat(errmsg, " has no power of 2 dimensions");
     debug_error_lib(hWindow, errmsg);
  }

  if (t->HW_ADRender_Pos < 0)
  {
	// la texture va aggiunta; cerco prima dellos tesso formato
	// che ha il video
    hr=g_pd3dDevice->CreateTexture(t->dimx, t->dimy, 1, 0,
		                           video_format,
		                           D3DPOOL_MANAGED,
								   &g_pD3DTexture[num_texture]);
	// cerco in sequenza 24, 32, 16, 15
	if (hr != D3D_OK)
	{
      hr=g_pd3dDevice->CreateTexture(t->dimx, t->dimy, 1, 0,
	                                 D3DFMT_R8G8B8,
		                             D3DPOOL_MANAGED,
								     &g_pD3DTexture[num_texture]);
	  if (hr != D3D_OK)
	  {
        hr=g_pd3dDevice->CreateTexture(t->dimx, t->dimy, 1, 0,
	  	                               D3DFMT_X8R8G8B8,
		                               D3DPOOL_MANAGED,
								       &g_pD3DTexture[num_texture]);
	    if (hr != D3D_OK)
		{
          hr=g_pd3dDevice->CreateTexture(t->dimx, t->dimy, 1, 0,
	  	                                 D3DFMT_R5G6B5,
		                                 D3DPOOL_MANAGED,
								         &g_pD3DTexture[num_texture]);
	      if (hr != D3D_OK)
		  {
            hr=g_pd3dDevice->CreateTexture(t->dimx, t->dimy, 1, 0,
	  	                                   D3DFMT_X1R5G5B5,
		                                   D3DPOOL_MANAGED,
								           &g_pD3DTexture[num_texture]);
			
			if (hr != D3D_OK) debug_error_lib(hWindow, "Not good texture formats found");
		  }
		}
	  }
	}
    t->HW_ADRender_Pos=num_texture;
    num_texture++;
  }
  
  if (CopyRGBTetxureToSurface(t, g_pD3DTexture[t->HW_ADRender_Pos])==-1)
	 debug_error_lib(hWindow, "Texture Copy Sram->Vram failed");
//  fprintf(fdebug, "Texture %s AddedUpdated...OK\n", t->name);
//  fflush(fdebug);
  return(0);
}


int AddUpdate_AlphaTexture (texture *t)
{
  HRESULT hr;
  char errmsg[90]="";

  if (t==NULL) debug_error_lib(hWindow, "AddUpdate_AlphaTexture: texture passed is NULL");
  if (is_texture_pow2(t)==FALSE)
  {
     strcpy(errmsg, t->name);
	 strcat(errmsg, " has no power of 2 dimensions");
     debug_error_lib(hWindow, errmsg);
  }

  if (t->HW_ADRender_Pos < 0)
  {
    hr=D3DERR_NOTFOUND ;
	// la texture va aggiunta; cerco prima dellos tesso formato
	// che ha il video (o meglio stessa profondita')
	if ((video_format==D3DFMT_R8G8B8) ||
        (video_format==D3DFMT_X8R8G8B8))
	{
       hr=g_pd3dDevice->CreateTexture(t->dimx, t->dimy, 1, 0,
		                              D3DFMT_A8R8G8B8,
		                              D3DPOOL_MANAGED,
								      &g_pD3DTexture[num_texture]);
	}
	
	// cerco a 16 bit
	if (hr != D3D_OK)
	{
       hr=g_pd3dDevice->CreateTexture(t->dimx, t->dimy, 1, 0,
		                              D3DFMT_A1R5G5B5,
		                              D3DPOOL_MANAGED,
								      &g_pD3DTexture[num_texture]);
	   if (hr != D3D_OK) debug_error_lib(hWindow, "Not good alpha texture formats found");
	}
    t->HW_ADRender_Pos=num_texture;
    num_texture++;
  }
  if (CopyARGBTetxureToSurface(t, g_pD3DTexture[t->HW_ADRender_Pos])==-1)
	 debug_error_lib(hWindow, "AlphaTexture Copy Sram->Vram failed");
  return(0);
}


DrawTriangleProc Set_Material (material *m)
{
  if (m==(material *)NULL) return(tria_raster_NULL);
  if (m==matold)
  {
    return(matold->myraster);
  }
  if (matold != NULL)
  {
    matold->myscanline();
//    fprintf(fdebug, "Transarency %d...OK\n", matold->trasparency);
//    fflush(fdebug);
  }
  matold=m;
  if (m->trasparency <=0) current_transparency=0;
  current_transparency=(((unsigned int)m->trasparency) << 24) & 0xFF000000;
//  fprintf(fdebug, "Transarency %d...OK\n", matold->trasparency);
//  fflush(fdebug);
  return (matold->myraster);
}


void Clear_DepthBuffer(float zclear)
{
  HRESULT hr;

  hr = g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
  check_dx_lib(hWindow, hr, "Device::Clear Zbuffer");
}


void Set_ZRanges(float znear, float zfar)
{
  if ((znear!=cur_znear) || (zfar!=cur_zfar))
  {
	cur_znear=znear;
	cur_zfar=zfar;
    SetProjectionMatrix(matProj, 3.1415f, 1.0f, cur_znear, cur_zfar);
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
  }
}


void End_Scene(void)
{
  if (begin_end_scene==FALSE)
  {
	begin_end_scene=TRUE;
    if (matold != (material *)NULL)
	{
	  matold->myscanline();
	}
	HRESULT hr = g_pd3dDevice->EndScene();
    check_dx_lib(hWindow, hr, "g_pd3dDevice->EndScene()");
  }
}


void Update_Screen(void)
{
  HRESULT hr;

  hr=g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
  check_dx_lib(hWindow, hr, "Update Screen");
  Clear_DepthBuffer(0);
}


void Close_Screen()
{
  if (g_pd3dDevice)
  {
     g_pd3dDevice->Release();
     g_pD3D->Release();
     g_pd3dDevice = NULL;
     g_pD3D       = NULL;
     delete [] image.muly;
  }
//	fflush(fdebug);
//	fclose(fdebug);
}




void Begin_ExclusiveTransparency(void)
{
  HRESULT hr;

  if (matold)	
    matold->myscanline();

  hr=g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  check_dx_lib(hWindow, hr, "Begin_ExclusiveTransparency: alphablend");
  hr=g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
  check_dx_lib(hWindow, hr, "Begin_ExclusiveTransparency: zwriteenable");
}


void End_ExclusiveTransparency(void)
{
  HRESULT hr;

  hr=g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  check_dx_lib(hWindow, hr, "End_ExclusiveTransparency: alphablend");
  hr=g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  check_dx_lib(hWindow, hr, "End_ExclusiveTransparency: zwriteenable");
}


floatbuffer *SW_Get_WBuffer(void)
{
   return((floatbuffer *)NULL);
}