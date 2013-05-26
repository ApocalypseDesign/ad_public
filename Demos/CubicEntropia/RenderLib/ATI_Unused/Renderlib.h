#ifndef _RENDERLIB_H_
#define _RENDERLIB_H_

#include <D3D8.h>
#include "keyframe.h"
#include "jpeglib.h"
#include "png.h"

#define GREYSCALE 0            // texture a 256 colori con palatte
#define RGBPLANAR 1            // texture RGB in modo planare (RRRR GGGG BBBB)
#define RGBINTERLEAVED 2       // texture RBG in modo interlacciato (RGBA RGBA)
#define TEXTURE_HAS_ALPHA 10   // la texture letta ha anche il canale alpha
#define FILENOTFOUND 0

class CGraphicSurface
{
private:
   float4 p_OptWidth, p_OptHeight;

   // restituiscono FILENOTFOUND (0) oppure 1 (tutto ok)
   int32 m_LoadJPG (char8 *namefile, int32 loadmethod);
   int32 m_LoadPNG (char8 *namefile, int32 loadmethod);
   int32 m_Int2Bits (int32 val);

public:
   int32 p_Width;
   int32 p_Height;
   int32 p_ChannelSize, p_ChannelSize2x;
   int32 p_MipMapLevels;
   int32 p_XShift, p_YShift;
   int32 *p_MulY;
   char8 p_Name[100];
   int32 p_Type;
   char8 *p_Palette;
   void *p_ScanLines;
   void *p_Pixels;
   //int32 p_HWRenderPos;
   IDirect3DTexture8 *p_HWSurface;

   CGraphicSurface();
   CGraphicSurface(int32 Width, int32 Height, int32 Bpp);
   void m_Clear(int32 A, int32 R, int32 G, int32 B);
   void m_Clear(int32 ARGB);
   
   int32 m_Load (char8 *namefile, int32 loadmethod);
   void *m_Lock(void);
   void m_Unlock(void);
};

//--------------------------------------------------------------------

#define ON 1
#define OFF 0

// utilizzate maps-mixtype
#define MAPSMIXTYPE_TEXSUBENV     256
#define MAPSMIXTYPE_ENVSUBTEX     257

// utilizzata sia per maps/light/trasparency-mixtype
#define MIXTYPE_ADD               258
#define MIXTYPE_BLEND50           259
#define MIXTYPE_ALPHABLEND        260
#define MIXTYPE_SUB               261
#define MIXTYPE_NONE              262
#define MIXTYPE_BLEND3DS          263
#define ALPHA_MAP                 264

// utilizzata per p_BumpType
#define BUMP_EMBM  512
#define BUMP_DOT3  1024
#define AUTOMATIC_BUMPMAP 2048

// utlizzate per il culling
#define FRONTFACE_CULLING 1
#define BACKFACE_CULLING 2

//#define PAINT_FLAT 1
#define PAINT_LIGHT 2
//#define PAINT_TEXTURE1 4
//#define PAINT_TEXTURE2 8
//#define IS_TRASPARENT 16
//#define PAINT_BUMPMAP 32

// per le MAP1CHANNEL e MAP2CHANNEL
#define ENVMAPPING 8
#define TEXMAPPING1 16
#define TEXMAPPING2 32

typedef void (__cdecl *SetParameters)(void);

class CMaterial
{
private:
  void m_BuildFVF(void);

public:
  char8 p_Name[100], p_FatherName[100];
  int32 p_TransparencyAmount,
        p_TransparencyType,
        p_MapsMixType,
        p_AlphaMixFactor,
        p_LightMixType,
        p_BumpType,
		p_Specular;
  int32 p_BackFaceCulling;
  int32 p_Flags;
  int32 p_FVF;
  uint32 p_FVFSize;
  CGraphicSurface *p_Map1;
  CGraphicSurface *p_Map2;
  int32 p_NumPasses;

  // i primi 3 bit indicano l'indice che ricevono dentro il formato
  // del vertex buffer, i casi possibili sono
  //
  // + solo la Mappa1:
  //    + se non c'è la bumpmap
  //        - se è envmap allora non servono indici
  //        - se è texture allora prende indice 0
  //    + se  c'è la bumpmap si segnala errore
  //
  //
  // + Mappa1 e Mappa2:
  //    + se non esiste la bumpmap
  //       - se sono tutte e due texture allora Mappa1 prende indice0
  //         e Mappa 2 prende indice 1
  //       - se una delle due è envmap allora l'altra riceve indice 0
  //    + se esiste la bumpmap
  //       - se sono tutte e 2 texture allora Mappa1 prende indice0
  //         e Mappa2 prende indice1, la bump prende indice0

  int32 p_Map1Channel, p_Map2Channel;
  float4 p_U1Offset, p_V1Offset, p_U1Tile, p_V1Tile, p_W1Angle;
  CRolltrack *p_U1OffsetTrack, *p_V1OffsetTrack;
  CRolltrack *p_U1TileTrack, *p_V1TileTrack;
  CRolltrack *p_W1AngleTrack;
  AD_Matrix p_Map1Matrix;

  float4 p_U2Offset, p_V2Offset,  p_U2Tile, p_V2Tile, p_W2Angle;
  CRolltrack *p_U2OffsetTrack, *p_V2OffsetTrack;
  CRolltrack *p_U2TileTrack, *p_V2TileTrack;
  CRolltrack *p_W2AngleTrack;
  AD_Matrix p_Map2Matrix;
  
  CRolltrack *p_TransparencyTrack;

  CGraphicSurface *p_BumpMap;
  float4 p_BumpUScale, p_BumpVScale;

  CGraphicSurface *p_AlphaMap;
  D3DMATERIAL8 p_D3DMaterial;
  CMaterial *p_NextMaterial;

  CMaterial();
  int32 m_Init(void *arg1);
  void m_Update(float4 frame);
  int32 m_MapNeedUVTransform(int32 mapIndex);
  int32 m_GetNumPasses(void);
  uint32 m_FVF2Size(uint32 FVF);
};

//--------------------------------------------------------------------

typedef struct
{
    int32      Width;      // Screen width in this mode
    int32      Height;     // Screen height in this mode
    char8      Bpp;        // bits per pixel; calcolato in base a Format
    D3DFORMAT  Format;     // Pixel format in this mode
    DWORD      Behavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
} D3DModeInfo;


typedef struct
{
  AD_Vect3D point;
  AD_Vect3D normal;
} FVFOnlyGeometry;


typedef struct
{
  AD_Vect3D point;
  AD_Vect3D normal;
  uint32 ARGB;
} FVFGeometryDiffuse;


typedef struct
{
  AD_Vect3D point;
  AD_Vect3D normal;
  AD_VectUV uv1;
} FVFGeometryUV1;


typedef struct
{
  AD_Vect3D point;
  AD_Vect3D normal;
  uint32 ARGB;
  AD_VectUV uv1;
} FVFGeometryDiffuseUV1;


typedef struct
{
  AD_Vect3D point;
  AD_Vect3D normal;
  AD_VectUV uv1;
  AD_VectUV uv2;
} FVFGeometryUV2;


typedef struct
{
  AD_Vect3D point;
  AD_Vect3D normal;
  uint32 ARGB;
  AD_VectUV uv1;
  AD_VectUV uv2;
} FVFGeometryDiffuseUV2;


typedef struct
{
    char8          strDesc[40];
    D3DModeInfo    SupportedModes[100];
    int32          NumSupportedModes;
    BOOL           CanDoWindowed;
    DWORD          Behavior;
    D3DMULTISAMPLE_TYPE MultiSampleType;  // livelli di FSAA
    D3DDEVTYPE   DeviceType;  // D3DDEVTYPE_HAL o D3DDEVTYPE_REF
    D3DCAPS8     Caps;        // Capabilities of this device
} D3DDeviceInfo;


// per la m_CreateDevice
#define HARDWARE_DEVICE 1
#define SOFTWARE_DEVICE 2

#define MAX_MATERIALS 100
#define MAX_TEXTURES 100
#define RGB_MAXVALUE 1.0f

class CRenderLib
{
private:
  int32 p_BeginEnd_Scene;
  int32 p_IsDeviceCreated;
  int32 p_IsEnumerationDone;
  D3DDeviceInfo p_DeviceInfo;
  D3DDEVTYPE p_DeviceType;
  D3DModeInfo p_VideoMode;

  HWND                        hWindow;
  LPDIRECT3D8                 p_D3D;
  D3DPRESENT_PARAMETERS       p_PresentParamteres;
  D3DFORMAT                   p_VideoFormat;
  D3DFORMAT                   p_DephtBufferFormat;
  LPDIRECT3DCUBETEXTURE8      p_NormalizationCubeMapTexture;
  
  char8 D3DFormat2Bpp (D3DFORMAT format);
  BOOL IsPow2 (CGraphicSurface *t);
  int32 CopyRGBTetxureToSurface (CGraphicSurface *t, LPDIRECT3DTEXTURE8 s);
  int32 CopyARGBTetxureToSurface (CGraphicSurface *t, LPDIRECT3DTEXTURE8 s);
  int32 CopyBumpTetxureToSurface (CGraphicSurface *t, LPDIRECT3DTEXTURE8 s, int32 bumpType);
  int32 BuildGreyMap(CGraphicSurface *src, CGraphicSurface *dest);
  BOOL FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType,
                              D3DFORMAT TargetFormat,
                              D3DFORMAT *pDepthStencilFormat);
  void m_SetMaterialProcedures (CMaterial *m);
  void m_SetMaterial (CMaterial *m);
  void m_D3DError(HRESULT hRet, char8 *msg);

  void m_SetMap0(CMaterial *m);
  void m_SetMap1(CMaterial *m);
  void m_SetMap1DOT3(CMaterial *m);
  void m_SetMap12(CMaterial *m);
  void m_SetMap12EMBM(CMaterial *m);
  void m_SetMap12DOT3(CMaterial *m);
  void m_BuildNormalizationCubemap(int32 pixelDim, int32 mipmapLevels);

public:
  int32 p_MultiTexture;  // 0 = non supportato
                         // >=0 numero di massime texture blendabili
                         // in una singola passata
  BOOL p_EMBMSupported, p_DOT3Supported;
  int32 p_VSHardwareSupport;
  BOOL p_TextureSubtract;
  int32 p_MaxLightsAllowed;
  CGraphicSurface *p_FrontBuffer;

  LPDIRECT3DDEVICE8 p_Device;

  CMaterial *p_Materials[MAX_MATERIALS];
  int32 p_NumMaterials;

  CRenderLib();
  
  // Creazione, enumerazione, apertura e chiusura dello schermo
  int32 m_CreateDevice(int32 type);
  int32 m_EnumerateScreenModes (void);
  D3DModeInfo *m_GetScreenMode(int32 index);
  D3DModeInfo *m_GetScreenMode(int32 width, int32 height, int32 bpp);
  CGraphicSurface *m_OpenScreen(HWND hWnd, D3DModeInfo *screen_mode,
	                          BOOL FullScreen);
  void m_Close_Screen(void);
  int32 m_Reset(void);

  // materiali e texture
  int32 m_AddUpdate_Material (CMaterial *m);
  int32 m_AddUpdate_Texture (CGraphicSurface *t, int32 mipmap_levels);
  int32 m_AddUpdate_AlphaTexture (CGraphicSurface *t, int32 mipmap_levels);
  int32 m_AddUpdate_BumpTexture (CGraphicSurface *t, int32 bumpType);

  // fase di disegno
  void m_SetMaterial (CMaterial *m, int32 pass);
  void m_BeginScene(void);
  void m_EndScene(void);
  void m_BeginExclusiveTransparency(void);
  void m_EndExclusiveTransparency(void);
  void m_UpdateScreen(void);

  int32 m_SetViewport (int32 x0, int32 y0, int32 width, int32 height, float4 zmin, float4 zmax);
  int32 m_SetViewport (int32 x0, int32 y0, int32 width, int32 height);
  int32 m_ClearScreen(int32 ARGB, float4 zclear);
  int32 m_ClearScreen(int32 A, int32 R, int32 G, int32 B, float4 zclear);

  int32 m_SetWorldMatrix (AD_Matrix *matrix);
  int32 m_SetViewMatrix (AD_Matrix *matrix);
  int32 m_SetProjMatrix (AD_Matrix *matrix);
  int32 m_SetProjMatrix (float4 fov, float4 aspect_ratio, float4 znear, float4 zfar);
  void  m_Error(char8 *msg);
};


//------------------------------------------
// metodi specifici per la versione software
//------------------------------------------
// floatbuffer *SW_Get_WBuffer();

#endif