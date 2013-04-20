#include <windows.h>
#include "..\ADrender.h"
#include "..\ADbase.h"
#include "..\D3Dutility.h"
#include "rasters.h"
#include "extrend.h"

#define MAXDRIVER 5
#define MAXSCREENMODES 50

float TEXTURE_PRECISION = 65536.0f;
float ENVMAP_PRECISION  = 65536.0f/2.0f;
float RGB_PRECISION     = 65536.0f;
float RGB_MAXVALUE      = 254.0f*65536.0f;

// usate per supporto alle DirectX/DirectDraw
LPDIRECT3D9                 g_pD3D=NULL;
LPDIRECT3DDEVICE9           g_pd3dDevice=NULL;
D3DPRESENT_PARAMETERS       g_d3dpp;
D3DFORMAT                   video_format;

HWND hWindow;
FILE *fdebug;

// buffer sw in formato ARGB interlacciato
imagebuffer *image=(imagebuffer *)NULL;
floatbuffer *zbuffer=(floatbuffer *)NULL;
material *matold;

// serve per sincronizzarsi, ovvero che ogni Begin_Scene viene
// prima di ogni End_Scene e che per fare una End_Scene si deve
// aver fatto la Begin_Scene
BOOLEAN begin_end_scene;


int Enumerate_DrawDriver (D3DDEVICEINFO **tab_dev)
{
//  fdebug=fopen("libSWlog.txt", "wt");
//  if (fdebug==NULL) return(-1);
//  fprintf(fdebug, "ADrender Software Lib log\n");
//  fflush(fdebug);
//  fprintf(fdebug, "Driver Enumeration...OK\n");
//  fflush(fdebug);
  return(EnumerateDriver(tab_dev));
}


imagebuffer *Open_Screen (HWND hWnd, D3DDEVICEINFO *devinf)
{
  HRESULT                     hr;

  // controlli iniziali
  if (devinf==NULL) return(NULL);

  hWindow=hWnd;
/*
  hr = DirectDrawCreateEx(devinf->pDriverGUID, (VOID**)&g_pDD, IID_IDirectDraw7, NULL);
  check_dx_swlib(hWnd, hr, "DirectDrawCreateEx");
  fprintf(fdebug, "DirectDraw object created...OK\n");
  fflush(fdebug);

  // Exclusive/normal mode 
  // (deve essere normal se è in finestra, altrimenti fullscreen e exclusive)
  if (devinf->bFullscreen) hr = g_pDD->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    else hr = g_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
  check_dx_swlib(hWnd, hr, "SetCooperativeLevel");
  fprintf(fdebug, "SetcoopertaiveLevel...OK\n");
  fflush(fdebug);


  // SetDisplayMode
  ddblitter=TRUE;
  if (devinf->bFullscreen)
  {
	  hr = g_pDD->SetDisplayMode(devinf->ddsdMode.dwWidth,
		                         devinf->ddsdMode.dwHeight,
							     devinf->ddsdMode.ddpfPixelFormat.dwRGBBitCount, 0, 0);
	  if ((hr!=DD_OK) && (devinf->ddsdMode.ddpfPixelFormat.dwRGBBitCount==32))
      {
		hr = g_pDD->SetDisplayMode(devinf->ddsdMode.dwWidth,
		                           devinf->ddsdMode.dwHeight, 24, 0, 0);
		ddblitter=FALSE;
	  }
      if ((hr!=DD_OK) && (devinf->ddsdMode.ddpfPixelFormat.dwRGBBitCount==32))
        hr = g_pDD->SetDisplayMode(devinf->ddsdMode.dwWidth,
		                           devinf->ddsdMode.dwHeight, 16, 0, 0);
	  if ((hr!=DD_OK) && (devinf->ddsdMode.ddpfPixelFormat.dwRGBBitCount==32))
        hr = g_pDD->SetDisplayMode(devinf->ddsdMode.dwWidth,
		                           devinf->ddsdMode.dwHeight, 8, 0, 0);
	  check_dx_swlib(hWnd, hr, "SetDisplayMode FAILED");
	}
    fprintf(fdebug, "SetDisplayMode...OK\n");
    fflush(fdebug);

    // Creazione della surface primaria
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    hr = g_pDD->CreateSurface(&ddsd, &g_pDDSPrimary, NULL);
    check_dx_swlib(hWnd, hr, "CreateSurface (primary)");
    fprintf(fdebug, "Primary Surface created...OK\n");
    fflush(fdebug);

	// controllo se è 32bpp ARGB
	hr = g_pDDSPrimary->GetSurfaceDesc(&ddsd);
    check_dx_swlib(hWnd, hr, "primary surface -> GetSurfaceDesc");

	if ((ddsd.ddpfPixelFormat.dwFlags==DDPF_RGB) &&
		(ddsd.ddpfPixelFormat.dwRGBBitCount==32) &&
		(ddsd.ddpfPixelFormat.dwRBitMask==0x00FF0000) &&
		(ddsd.ddpfPixelFormat.dwGBitMask==0x0000FF00) &&
		(ddsd.ddpfPixelFormat.dwBBitMask==0x000000FF))
		ddblitter=TRUE;
	else ddblitter=FALSE;
*/   
    // Creazione del buffer
    image=new imagebuffer(devinf->WantedMode.Width,
		                  devinf->WantedMode.Height);
    image->clear(0);
	zbuffer=new floatbuffer(devinf->WantedMode.Width,
		                    devinf->WantedMode.Height);
	zbuffer->clear(0);
	matold=(material *)NULL;
    begin_end_scene=TRUE;  // posso fare la Begin_Scene
/*
	// Creo la superficie del buffer
	ZeroMemory(&(ddsd), sizeof(ddsd));
	ZeroMemory(&(ddsd.ddpfPixelFormat), sizeof(ddsd.ddpfPixelFormat));
	ZeroMemory(&(ddsd.ddsCaps), sizeof(ddsd.ddsCaps));

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PITCH | 
		           DDSD_LPSURFACE | DDSD_PIXELFORMAT;
    ddsd.dwWidth = devinf->ddsdMode.dwWidth;
	ddsd.dwHeight = devinf->ddsdMode.dwHeight;
	ddsd.lPitch = devinf->ddsdMode.dwWidth*4;
	ddsd.lpSurface=(LPVOID)(image->voidptr);

	// chiedo espressamente il modo ARGB
	ddsd.ddpfPixelFormat.dwSize=sizeof(ddsd.ddpfPixelFormat);
	ddsd.ddpfPixelFormat.dwFlags=DDPF_RGB;
	ddsd.ddpfPixelFormat.dwRGBBitCount=32;
	ddsd.ddpfPixelFormat.dwRBitMask=0x00FF0000;
	ddsd.ddpfPixelFormat.dwGBitMask=0x0000FF00;
	ddsd.ddpfPixelFormat.dwBBitMask=0x000000FF;
	//ddsd.ddpfPixelFormat.dwRGBAlphaBitMask=0x000000;

    hr = g_pDD->CreateSurface(&ddsd, &g_pDDSBuffer, NULL);
    check_dx_swlib(hWnd, hr, "CreateSurface (buffer)");
    fprintf(fdebug, "BackBuffer Surface created...OK\n");
    fflush(fdebug);
  
	FrameBufferInit(image);
    fprintf(fdebug, "Frame Buffer init...OK\n");
    fflush(fdebug);
*/
  ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
  if ((!devinf->bFullscreen) && (devinf->bCanDoWindowed)) g_d3dpp.Windowed = TRUE;
  else g_d3dpp.Windowed = FALSE;
  g_d3dpp.BackBufferCount        = 1;
  g_d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
  g_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  g_d3dpp.EnableAutoDepthStencil = FALSE;
  g_d3dpp.AutoDepthStencilFormat = devinf->WantedMode.DepthStencilFormat;
  g_d3dpp.hDeviceWindow          = hWnd;
  g_d3dpp.Flags                  = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
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
	                        D3DDEVTYPE_REF,
                            hWnd,
							D3DCREATE_SOFTWARE_VERTEXPROCESSING,
							&g_d3dpp,
                            &g_pd3dDevice);
  check_dx_lib(hWindow, hr, "g_pD3D->CreateDevice");
  video_format=devinf->WantedMode.Format;

  init_render(image, zbuffer);
//  fprintf(fdebug, "init render...OK\n");
//  fflush(fdebug);
  return(image);
}


void Begin_Scene()
{
  if (begin_end_scene==TRUE)
  {
    matold=(material *)NULL;
    trick+=trick_step;
	begin_end_scene=FALSE;
  }
}


int AddUpdate_Material (material *m)
{
  int tbl;

  if (m->texture_ptr) AddUpdate_Texture(m->texture_ptr);
  if (m->envmap_ptr) AddUpdate_Texture(m->envmap_ptr);

  if (m->flags & IS_TRASPARENT)
  {
	if (m->trasparency_table==NULL)
      m->trasparency_table=new unsigned char[256];
    for (tbl=0; tbl<256; tbl++)
	  m->trasparency_table[tbl]=(char)(1.0*tbl*m->trasparency/255.0);
  }

  if ((m->flags & PAINT_TEXTURE) && (m->flags & PAINT_ENVMAP) &&
	  (m->mapsmixtype==MIXTYPE_ALPHABLEND))
  {
	if (m->alphablend_table==NULL)
      m->alphablend_table=new unsigned char[256];
    for (tbl=0; tbl<256; tbl++)
	  m->alphablend_table[tbl]=(char)(1.0*tbl*m->alphamixfactor/255.0);
  }
  
  set_material_procedures(m);
  return(0);
}


int AddUpdate_Texture (texture *t)
{
  char errmsg[90];

  if (t==NULL) debug_error_lib(hWindow, "AddUpdate_Texture: texture passed is NULL");
  if (is_texture_pow2(t)==FALSE)
  {
     strcpy(errmsg, t->name);
	 strcat(errmsg, " has no power of 2 dimensions");
     debug_error_lib(hWindow, errmsg);
  }
  return(0);
}


int AddUpdate_AlphaTexture (texture *t)
{
  char errmsg[90];

  if (t==NULL) debug_error_lib(hWindow, "AddUpdate_AlphaTexture: texture passed is NULL");
  if (is_texture_pow2(t)==FALSE)
  {
     strcpy(errmsg, t->name);
	 strcat(errmsg, " has no power of 2 dimensions");
     debug_error_lib(hWindow, errmsg);
  }
  return(0);
}
int AddUpdate_BumpTexture (texture *t)  { return(0); }


DrawTriangleProc Set_Material (material *m)
{
  if (m==(material *)NULL) return(tria_raster_NULL);
  if (m==matold) return(matold->myraster);

  current_scanline_proc=m->myscanline;
  matold=m;

  if (m->texture_ptr!=(texture *)NULL)
  {
	texmap8=matold->texture_ptr->map;
    texmap32=matold->texture_ptr->map32;
    _umask_=matold->texture_ptr->dimx-1;
    vmask=matold->texture_ptr->dimy-1;
    vshift=matold->texture_ptr->xbits;
	ttable=matold->trasparency_table;
	atable=matold->alphablend_table;
  }
  
  if (m->envmap_ptr!=(texture *)NULL)
  {
	envmap8=matold->envmap_ptr->map;
    envmap32=matold->envmap_ptr->map32;
    envvshift=matold->envmap_ptr->xbits;
	ttable=matold->trasparency_table;
	atable=matold->alphablend_table;
  }
  return (matold->myraster);
}


void Clear_DepthBuffer(float zclear)
{
  zbuffer->clear(1.0f/zclear);
  trick=0;
}


void Set_ZRanges(float znear, float zfar)
{
  trick_step=1.0f/znear;
}


void End_Scene(void)
{
  if (begin_end_scene==FALSE) begin_end_scene=TRUE;
}


void blitter_32_to_16(unsigned int *src, unsigned int *dst, unsigned int s)
{
   _asm
   {
	  mov ecx, s
      mov esi, src

      mov edi, dst
	  shr ecx, 2

	  pxor mm7, mm7
	  sub edi, 8

ciclo:
	  movq mm0, [esi]
	  pxor mm7, mm7

	  movq mm2, [esi]
      movq mm1, [esi+8]

	  punpcklbw mm0, mm1
	  punpckhbw mm2, mm1

	  movq mm1, mm0
	  punpcklbw mm0, mm2

	  punpckhbw mm1, mm2
	  movq mm4, mm0

	  punpcklbw mm1, mm1     // MM1 = R3R3R2R1 in 16 bit
	  punpcklbw mm0, mm7     // MM0 = B3B3B2B1 in 16 bit

	  punpckhbw mm4, mm7     // MM4 = G3G3G2G1 in 16 bit
	  psrlw mm0, 3

	  psrlw mm1, 3
	  add esi, 16

	  psrlw mm4, 2
      add edi, 8

	  psllw mm1, 11
	  psllw mm4, 5

      por mm0, mm4
      por mm0, mm1

	  movq [edi], mm0
	  dec ecx

      jnz ciclo
      emms
   }
}


void Update_Screen(void)
{
	IDirect3DSurface9  *g_pBackBuffer;
	D3DLOCKED_RECT     Locked_Rect;
	unsigned int *srcbuf, *dstbuf;
	HRESULT hr;

	srcbuf=image->uint32ptr;
	hr=g_pd3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, D3DBACKBUFFER_TYPE_MONO, &g_pBackBuffer);
    //hr=g_pd3dDevice->GetFrontBuffer(&g_pBackBuffer);
	hr=g_pBackBuffer->LockRect(&Locked_Rect, NULL, NULL);
	dstbuf=(unsigned int *)Locked_Rect.pBits;
	
    if (video_format==D3DFMT_X8R8G8B8)
	{
       memcpyd_lib(dstbuf, srcbuf, image->width*image->height);
	}
	else
    if (video_format==D3DFMT_R5G6B5)
	{
       blitter_32_to_16(srcbuf, dstbuf, image->width*image->height);
	}

	hr=g_pBackBuffer->UnlockRect();
	hr=g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}


void Close_Screen()
{
//	fclose(fdebug);

    if (g_pd3dDevice)
	{
       g_pd3dDevice->Release();
       g_pD3D->Release();
       g_pd3dDevice = NULL;
       g_pD3D       = NULL;
	}

	if (image)
	{
	  delete image;
	  image=(imagebuffer *)NULL;
	}
    
	if (zbuffer)
	{
	  delete zbuffer;
	  zbuffer=(floatbuffer *)NULL;
	}
}


void Begin_ExclusiveTransparency(void) { return; }
void End_ExclusiveTransparency(void)   { return; }

//------------------------------------------
// metodi specifici per la versione software
//------------------------------------------
floatbuffer *SW_Get_WBuffer(void) { return(zbuffer); }
