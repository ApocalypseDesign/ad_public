#ifndef _RENDERLIB_H_
#define _RENDERLIB_H_

#include <io.h>
//#include "AtlBase.h"
//#include "Dshow.h"
//#include "QEdit.h"
#include "D3D8.h"
//#include "GraphicSurface.h"
#include "keyframe.h"
#include "jpeglib.h"
#include "png.h"

// enumeration of possible vertex format
#define FVF_ONLY_GEOMETRY             1
#define FVF_GEOMETRY_DIFFUSE          2
#define FVF_GEOMETRY_UV1              4
#define FVF_GEOMETRY_DIFFUSE_UV1      8
#define FVF_GEOMETRY_UV2             16
#define FVF_GEOMETRY_DIFFUSE_UV2     32
#define VS_GEOMETRY_UV1_DOT3         64
#define VS_GEOMETRY_UV2_DOT3        128


typedef struct
{
  int32 enumType;          // one of the above enumerated vertex format
  int32 vertexSize;        // size (in bytes) of the vertex
  int32 FVF;               // 0 for vertex shaders, else a valid FVF
  int32 SecondaryFVF;
  int32 SecondaryVertexSize;
  int32 SecondaryEnumType;
} INPUT_VERTEX_FORMAT;


typedef struct
{
  AD_Vect3D point;
  AD_Vect3D normal;
} FVFOnlyGeometry;

typedef struct
{
  AD_Vect3D point;
  float4 w;
  AD_VectUV uv1;
} FVFGeometryUV1Transformed;


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
  AD_Vect3D point;
  AD_Vect3D normal;
  AD_VectUV uv1;
  AD_Vect3D T;
} VSGeometryUV1DOT3;  // VSxxx stands for VertexShader


typedef struct
{
  AD_Vect3D point;
  AD_Vect3D normal;
  AD_VectUV uv1;
  AD_VectUV uv2;
  AD_Vect3D T;
} VSGeometryUV2DOT3;


//********************************************************************
//************************   CGraphicSurface   ***********************
//********************************************************************

#define GREYSCALE 1          // texture a 256 colori con palatte
#define RGBPLANAR 2          // texture RGB in modo planare (RRRR GGGG BBBB)
#define RGBINTERLEAVED 4     // texture RBG in modo interlacciato (RGBA RGBA)
#define TEXTURE_HAS_ALPHA 8  // la texture letta ha anche il canale alpha
#define FILENOTFOUND 0
#define ENDCONDITION_LOOP 1
#define ENDCONDITION_HOLD 2
#define ENDCONDITION_PINGPONG 4

class CGraphicSurface
{
private:
   float4 p_OptWidth, p_OptHeight;
   // restituiscono FILENOTFOUND (0) oppure 1 (tutto ok)
   int32 m_LoadJPG (char8 *namefile, int32 loadmethod);
   int32 m_LoadPNG (char8 *namefile, int32 loadmethod);
   int32 m_LoadVideo (char8 *namefile, int32 loadmethod);
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
   // supporto per le texture animate (AVI, MPEG ecc)
   float4 p_StartFrame;
   float4 p_PlayBackRate;
   int32 p_EndCondition;
   IDirect3DTexture8 **p_HWSurfaces;
   int32 p_NumTextures;
   int32 p_CurrentTextureIndex;
   int32 p_PlayDirection; // 1 = avanti,  -1 = indietro

   char8 *p_Palette;
   void *p_ScanLines;
   void *p_Pixels;

   CGraphicSurface();
   CGraphicSurface(int32 Width, int32 Height, int32 Bpp);
   CGraphicSurface(char8 *fileName, int32 *err);
   void m_Clear(int32 A, int32 R, int32 G, int32 B);
   void m_Clear(int32 ARGB);
   void m_Update(float4 frame);
   int32 m_Load (char8 *namefile, int32 loadmethod);
   void *m_Lock(void);
   void m_Unlock(void);
};


//********************************************************************
//****************************   CMaterial   *************************
//********************************************************************


#define OFF 0
#define MIXTYPE_NONE 0
#define ON 1

// used by p_MapsMixType and p_LightMixType
#define MIXTYPE_ADD           2
#define MIXTYPE_MODULATE      4
#define MIXTYPE_SUB           8
#define MIXTYPE_MODULATE2X    16
#define MIXTYPE_MODULATE4X    32
#define MIXTYPE_ADDSIGNED     64
#define MIXTYPE_ADDSIGNED2X   128
#define MIXTYPE_ADDSMOOTH     256
#define MIXTYPE_ALPHABLEND    512  // used only by p_TransparencyType
#define ALPHA_MAP             1024  // used only by p_TransparencyType
#define ALPHA_MAP1            2048  // used only by p_TransparencyType

// used only by p_LightMixType
#define MIXTYPE_TOONSHADE 4096

// used by p_BumpType
#define BUMP_EMBM  512
#define BUMP_DOT3  1024
#define AUTOMATIC_BUMPMAP 2048

// used by p_BackFaceCulling
#define FRONTFACE_CULLING 1
#define BACKFACE_CULLING 2

// used by p_Ma1Cchennel and  p_Map2Channel
#define SPECULARMAPPING 4
#define ENVMAPPING 8
#define TEXMAPPING1 16
#define TEXMAPPING2 32

// usati per il fogtype
#define LINEARFOG 1
#define EXPFOG    2
#define EXP2FOG   4

class CMaterial
{
private:
  void m_BuildVertexFormat(void); // fills p_InputVertexFormat

public:
  char8 p_Name[100], p_ChildrenName[100];
  int32 p_TransparencyType,
        p_MapsMixType,
        p_LightMixType,
        p_BumpType,
		p_Specular,
        p_FogType;
  int32 p_BackFaceCulling;

  float4 p_FogStart, p_FogEnd, p_FogDensity;
  int32 p_FogColor;

  INPUT_VERTEX_FORMAT p_InputVertexFormat;

  CGraphicSurface *p_Map1;
  CGraphicSurface *p_Map2;
  int32 p_NumPasses; // numero di passate per il solo materiale
                     // (non sono considerati NextMaterial nè
                     // DOT3 e TOON)

  int32 p_Map1Channel, p_Map2Channel;
  float4 p_U1Offset, p_V1Offset, p_U1Tile, p_V1Tile, p_W1Angle;
  CRollTrack *p_U1OffsetTrack, *p_V1OffsetTrack;
  CRollTrack *p_U1TileTrack, *p_V1TileTrack;
  CRollTrack *p_W1AngleTrack;
  AD_Matrix p_Map1Matrix;

  float4 p_U2Offset, p_V2Offset,  p_U2Tile, p_V2Tile, p_W2Angle;
  CRollTrack *p_U2OffsetTrack, *p_V2OffsetTrack;
  CRollTrack *p_U2TileTrack, *p_V2TileTrack;
  CRollTrack *p_W2AngleTrack;
  AD_Matrix p_Map2Matrix;
  
  CRollTrack *p_TransparencyTrack;
  float4 p_TransparencyAmount;

  CGraphicSurface *p_BumpMap;
  float4 p_BumpUScale, p_BumpVScale;

  CGraphicSurface *p_ToonGradient;
  float4 p_ToonScale;
  int32 p_ToonLightsMixType;
  int32 p_ToonMapsMixType;

  CGraphicSurface *p_AlphaMap;

  D3DMATERIAL8 p_D3DMaterial;
  CMaterial *p_NextMaterial;
  int32 p_WireFrame, p_WireFrameColor;

  CMaterial();
  int32 m_Init(void *arg1);
  void m_Update(float4 frame);
  int32 m_MapNeedUVTransform(int32 mapIndex);
};



//********************************************************************
//***************************   CRenderLib   *************************
//********************************************************************

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
    char8          strDesc[40];
    D3DModeInfo    SupportedModes[100];
    int32          NumSupportedModes;
    BOOL           CanDoWindowed;
    DWORD          Behavior;
    D3DMULTISAMPLE_TYPE MultiSampleType;  // FSAA
    D3DDEVTYPE   DeviceType;  // D3DDEVTYPE_HAL o D3DDEVTYPE_REF
    D3DCAPS8     Caps;        // Capabilities of this device
} D3DDeviceInfo;


// used by m_CreateDevice
#define HARDWARE_DEVICE 1
#define SOFTWARE_DEVICE 2

// usefull ranges for materials and textures
#define MAX_MATERIALS 100
#define MAX_TEXTURES 100
#define RGB_MAXVALUE 1.0f

// costanti di definizione del tipo di blur in uso
#define NOBLUR 0
#define MOTIONBLUR 1
#define RADIALBLUR 2
#define BLUR_ALPHA 4
#define BLUR_ADD   8

class CRenderLib
{
private:
  int32 p_BeginEnd_Scene;
  int32 p_IsDeviceCreated;
  int32 p_IsEnumerationDone;
  D3DDeviceInfo p_DeviceInfo;
  D3DDEVTYPE p_DeviceType;
  D3DModeInfo p_VideoMode;
  AD_Matrix p_TextureIDMatrix;

  HWND                        hWindow;
  LPDIRECT3D8                 p_D3D;
  D3DPRESENT_PARAMETERS       p_PresentParamteres;
  D3DFORMAT                   p_VideoFormat;
  D3DFORMAT                   p_DephtBufferFormat;
  LPDIRECT3DCUBETEXTURE8      p_NormalizationCubeMapTexture;
  
  char8 m_D3DFormat2Bpp (D3DFORMAT format);
  D3DTEXTUREOP m_MixType2ColorOp(int32 mixType);
  BOOL m_IsPow2 (CGraphicSurface *t);
  int32 m_CopyRGBTetxureToSurface (CGraphicSurface *t, int32 i, LPDIRECT3DTEXTURE8 s, int32 mipmap_levels);
  int32 m_CopyARGBTetxureToSurface (CGraphicSurface *t, int32 i, LPDIRECT3DTEXTURE8 s, int32 mipmap_levels);
  int32 m_CopyBumpTetxureToSurface (CGraphicSurface *t, int32 i, LPDIRECT3DTEXTURE8 s, int32 bumpType, int32 mipmap_levels);
  int32 m_BuildGreyMap(CGraphicSurface *src, CGraphicSurface *dest);
  BOOL m_FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType,
                                D3DFORMAT TargetFormat,
                                D3DFORMAT *pDepthStencilFormat);

  int32 m_SetMaterialFVF (CMaterial *m, int32 pass);
  int32 m_SetMaterialVS (CMaterial *m, int32 pass);
  void m_FixMaterialFVF(CMaterial *m);
  void m_D3DError(HRESULT hRet, char8 *msg);

  // stages per vertex buffer FVF compliant (accelerati in T&L)
  void m_SetMap0(CMaterial *m);
  void m_SetMap1(CMaterial *m);
  void m_SetMap12(CMaterial *m);

  // stages per il bump DOT3
  void m_DOT3PassLights(CMaterial *m, int32 firstLight, D3DLIGHT8 *light);
  void m_Map1DOT3FinalPass(CMaterial *m);
  void m_Map2DOT3FinalPass(CMaterial *m);

  // stages per l'environment map bump mapping
  void m_SetMap12EMBM(CMaterial *m);

  // stages per il TOON shading
  void m_SetToonPass1(void);
  void m_SetToonPassLights(CMaterial *m, int32 firstLight, D3DLIGHT8 *light);
  void m_SetToonFinalPass(CMaterial *m);

  void m_BuildNormalizationCubemap(int32 pixelDim, int32 mipmapLevels);
  void m_ReadVSCode (char8 *filename, void **buffer);
  void m_LoadAndCreateShaders(void);

  // *****************************************************************
  // ***************    supporto per il toon shading    **************
  // *****************************************************************
  DWORD *p_TOONCode_Pass1, *p_TOONCode_PassLights, *p_TOONCode_FinalPass;
  DWORD p_TOONPass1Handle, p_TOONPassLightsHandle, p_TOONFinalPassHandle;
  DWORD p_TOONDeclaration[10];
  CGraphicSurface *p_TOONGradient;

  // *****************************************************************
  // *****************    supporto per il bump DOT3    ***************
  // *****************************************************************
  DWORD *p_UV1DOT3Code, *p_UV1DOT3FinalPassCode;
  DWORD p_UV1DOT3Handle, p_UV1DOT3FinalPassHandle;
  DWORD p_UV1DOT3Declaration[10];
  DWORD *p_UV2DOT3Code, *p_UV2DOT3FinalPassCode;
  DWORD p_UV2DOT3Handle, p_UV2DOT3FinalPassHandle;
  DWORD p_UV2DOT3Declaration[10];


  // *****************************************************************
  // ***************    supporto per il motion blur    ***************
  // *****************************************************************
  IDirect3DVertexBuffer8 *p_VertexBufferBlur1, *p_VertexBufferBlur2;
  LPDIRECT3DTEXTURE8   p_FrontTextureBlur;
  IDirect3DSurface8    *p_OldFrontSurface, *p_OldDepthSurface;
  IDirect3DSurface8    *p_FrontSurfaceBlur;
  IDirect3DSurface8    *p_DepthSurfaceBlur;
  int32 p_FlipBufferBC;
  void m_SetUpMotionBlur(void);
  void m_UpdateScreenBlur(void);
  float4 p_BlurLevel;
  BOOL p_MotionBlurSupported;

  // *****************************************************************
  // ***************    supporto per il radial blur    ***************
  // *****************************************************************
  IDirect3DVertexBuffer8 *p_VertexBufferBlur3;
  LPDIRECT3DTEXTURE8   p_OldTextureBlur;
  LPDIRECT3DTEXTURE8   p_CopyTextureBlur;
  IDirect3DSurface8    *p_OldSurfaceBlur;
  IDirect3DSurface8    *p_CopySurfaceBlur;
  BOOL p_RadialBlurSupported;
  void m_SetUpRadialBlur(void);

  // setta in sequenza il motion e il radial blur
  void m_SetUpBlurs(void);


public:
  int32 p_MultiTexture;  // 0 = non supportato
                         // >=0 numero di massime texture blendabili
                         // in una singola passata
  BOOL p_EMBMSupported, p_DOT3Supported;
  int32 p_VSHardwareSupport;
  int32 p_MaxLightsAllowed;
  CGraphicSurface *p_FrontBuffer;
  int32 p_BlurType;
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
  int32 m_AddUpdate_BumpTexture (CGraphicSurface *t, int32 bumpType, int32 mipmap_levels);

  // fase di disegno
  int32 m_GetMaterialPasses(CMaterial *m);
  int32 m_SetMaterial (CMaterial *m, int32 pass);
  void m_BeginScene(void);
  void m_EndScene(void);
  void m_BeginTransparency(void);
  void m_EndTransparency(void);
  void m_SetBlur (int32 type, float4 level);
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

void DumpDebugPoint (FILE *f, char8 *msg, HRESULT hr);

#endif