#include "RenderLib.h"
#include "Dot3Toon_VS.h"

extern FILE *fdebug;

void CRenderLib::m_ReadVSCode (char8 *filename, void **buffer)
{
  FILE *fVSO;
  char8 msg[100];
  DWORD *l_buffer;

  strcpy(msg, "");
  if (filename) strcat(msg, filename);
  fVSO=fopen(filename, "rb");
  if (!fVSO)
  {
	strcat(msg, " not found!");
	m_Error(msg);
  }

  l_buffer=(DWORD *)(new char8[filelength(fileno(fVSO))]);
  fread(l_buffer, filelength(fileno(fVSO)), sizeof(char8), fVSO);
  fclose(fVSO);
  *buffer=l_buffer;
}


// create vertex shaders for dot3 bump mapping and toon shading
void CRenderLib::m_LoadAndCreateShaders(void)
{
  DWORD l_usage;
  HRESULT hr;
  float4 c[4];
  int32 err;
  
  #include "uv1dot3_point.h"

  #include "uv1dot3_finalpass.h"

  #include "ToonShade_PassLights.h"

  #include "ToonShade_Pass1.h"

  #include "ToonShade_FinalPass.h"

  if (p_VSHardwareSupport) l_usage=0;
  else l_usage=D3DUSAGE_SOFTWAREPROCESSING;

  // *****************************************************************
  // ********************      DOT3 SHADERS      *********************
  // *****************************************************************
  p_UV1DOT3Declaration[0]=D3DVSD_STREAM(0);
  p_UV1DOT3Declaration[1]=D3DVSD_REG(0, D3DVSDT_FLOAT3); // Position
  p_UV1DOT3Declaration[2]=D3DVSD_REG(1, D3DVSDT_FLOAT3); // Normal
  p_UV1DOT3Declaration[3]=D3DVSD_REG(2, D3DVSDT_FLOAT2); // UV1
  p_UV1DOT3Declaration[4]=D3DVSD_REG(3, D3DVSDT_FLOAT3); // T
  p_UV1DOT3Declaration[5]=D3DVSD_END();

  /*m_ReadVSCode("uv1dot3_point.vso", (void **)&p_UV1DOT3Code);
  hr=p_Device->CreateVertexShader(p_UV1DOT3Declaration,
	                              (DWORD *)p_UV1DOT3Code,
                                  &p_UV1DOT3Handle, l_usage);*/
  hr=p_Device->CreateVertexShader(p_UV1DOT3Declaration,
	                              (DWORD *)dwUv1dot3_pointVertexShader,
                                  &p_UV1DOT3Handle, l_usage);
  m_D3DError(hr, "CRenderLib::m_OpenScreen, UV1Dot3 vertex shader creation failed!");


  /*m_ReadVSCode("uv1dot3_finalpass.vso", (void **)&p_UV1DOT3FinalPassCode);
  hr=p_Device->CreateVertexShader(p_UV1DOT3Declaration,
	                              (DWORD *)p_UV1DOT3FinalPassCode,
                                  &p_UV1DOT3FinalPassHandle, l_usage);*/
  hr=p_Device->CreateVertexShader(p_UV1DOT3Declaration,
	                              (DWORD *)dwUv1dot3_finalpassVertexShader,
                                  &p_UV1DOT3FinalPassHandle, l_usage);
  m_D3DError(hr, "CRenderLib::m_OpenScreen, UV1Dot3 Final Pass vertex shader creation failed!");


  // *****************************************************************
  // ********************      TOON SHADERS      *********************
  // *****************************************************************
  p_TOONDeclaration[0]=D3DVSD_STREAM(0);
  p_TOONDeclaration[1]=D3DVSD_REG(0, D3DVSDT_FLOAT3); // Position
  p_TOONDeclaration[2]=D3DVSD_REG(1, D3DVSDT_FLOAT3); // Normal
  p_TOONDeclaration[3]=D3DVSD_REG(2, D3DVSDT_FLOAT2); // UV1
  p_TOONDeclaration[4]=D3DVSD_END();
  /*m_ReadVSCode("ToonShade_Pass1.vso", (void **)&p_TOONCode_Pass1);
  hr=p_Device->CreateVertexShader(p_TOONDeclaration,
	                              (DWORD *)p_TOONCode_Pass1,
                                  &p_TOONPass1Handle, l_usage);*/
  hr=p_Device->CreateVertexShader(p_TOONDeclaration,
	                              (DWORD *)dwToonShade_Pass1VertexShader,
                                  &p_TOONPass1Handle, l_usage);
  m_D3DError(hr, "CRenderLib::m_OpenScreen, ToonShade_Pass1 vertex shader creation failed!");

  /*m_ReadVSCode("ToonShade_PassLights.vso", (void **)&p_TOONCode_PassLights);
  hr=p_Device->CreateVertexShader(p_TOONDeclaration,
	                              (DWORD *)p_TOONCode_PassLights,
                                  &p_TOONPassLightsHandle, l_usage);*/
  hr=p_Device->CreateVertexShader(p_TOONDeclaration,
	                              (DWORD *)dwToonShade_PassLightsVertexShader,
                                  &p_TOONPassLightsHandle, l_usage);
  m_D3DError(hr, "CRenderLib::m_OpenScreen, ToonShade_PassLights vertex shader creation failed!");

  /*m_ReadVSCode("ToonShade_FinalPass.vso", (void **)&p_TOONCode_FinalPass);
  hr=p_Device->CreateVertexShader(p_TOONDeclaration,
	                              (DWORD *)p_TOONCode_FinalPass,
                                  &p_TOONFinalPassHandle, l_usage);*/
  hr=p_Device->CreateVertexShader(p_TOONDeclaration,
	                              (DWORD *)dwToonShade_FinalPassVertexShader,
                                  &p_TOONFinalPassHandle, l_usage);
  m_D3DError(hr, "CRenderLib::m_OpenScreen, ToonShade_FinalPass vertex shader creation failed!");

  // carico e aggiungo la texture del gradiente per il TOON shader


  p_TOONGradient=new CGraphicSurface("ToonGradient.png", &err);
  if (err==FILENOTFOUND)
    m_Error("CRenderLib, ToonGradient.png not found!");

  if (p_TOONGradient->p_Type & GREYSCALE)
    m_Error("CRenderLib, ToonGradient.png must be a 24bit (16M colors) texture!");

  m_AddUpdate_Texture(p_TOONGradient, 1);
  DumpDebugPoint(fdebug, "Toon gradient added", 0);
  
  // *****************************************************************
  // ************   costanti fisse per tutti gli shaders   ***********
  // *****************************************************************
  c[0]=c[1]=c[2]=c[3]=0;
  hr=p_Device->SetVertexShaderConstant(CV_ZERO, c, 1);
  c[0]=c[1]=c[2]=c[3]=1;
  hr=p_Device->SetVertexShaderConstant(CV_ONE, c, 1);
  c[0]=c[1]=c[2]=c[3]=0.5f;
  hr=p_Device->SetVertexShaderConstant(CV_HALF, c, 1);
  c[0]=c[1]=c[2]=1.5f;
  c[3]=1.0f;
  hr=p_Device->SetVertexShaderConstant(CV_SILOUETTE_SCALING, c, 1);
}