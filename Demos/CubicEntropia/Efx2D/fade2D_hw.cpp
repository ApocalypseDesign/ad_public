#include "Fade2D.h"
#include <FastMath.h>

extern HWND miawin;
extern FILE *fdeb;

struct FADE2DVERTEX
{ 
   AD_Vect3D p;
   float4 w;
   int32 color;
};
#define FVF_FADE2DVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)


CFade2D::CFade2D()
{
  p_VertexBuffer=NULL;
}

//--------------------------------------------------------------------

int32 CFade2D::m_Init(void)
{ 
  int32 res;
  HRESULT hr;
  
  res=CEfx2D::m_Init();
  if (FAILED(hr = p_RenderLib->p_Device->CreateVertexBuffer(6*sizeof(FADE2DVERTEX),
                                                            D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                            D3DPOOL_DEFAULT, &p_VertexBuffer)))
         return(0);
  DumpDebugPoint (fdeb, "CFade2D Init cerate vertex buffer done", 0);
  return(1);
} 

//--------------------------------------------------------------------

void CFade2D::m_SetStages(void)
{
  p_RenderLib->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  p_RenderLib->p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  p_RenderLib->p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  p_RenderLib->p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
  p_RenderLib->p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);

  p_RenderLib->p_Device->SetVertexShader(FVF_FADE2DVERTEX);
  p_RenderLib->p_Device->SetStreamSource(0, p_VertexBuffer, sizeof(FADE2DVERTEX));
  p_RenderLib->p_Device->SetTexture(0, NULL);
  p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  p_RenderLib->p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  p_RenderLib->p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

//--------------------------------------------------------------------

void CFade2D::m_DoEfx2D(double pos)
{
  FADE2DVERTEX *pVertices;
  float4 cpos;
  int32 intensity, color;
  HRESULT hr;
  D3DVIEWPORT8 vp;

  cpos=(float4)(3.141592*(pos-fstartpos)/(fendpos-fstartpos));
  intensity=(int32)(-255.0f*fast_sinf(cpos)+255.0f);
  if (intensity<0) intensity=0;
  if (intensity>255) intensity=255;
  color=0xFF000000 | (intensity) | (intensity<<8) | (intensity<<16);

  p_RenderLib->p_Device->GetViewport(&vp);

  m_SetStages();

  hr=p_VertexBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);
  // 0 --- 1
  //       |
  //       2
  pVertices[0].p.x=0;
  pVertices[0].p.y=0;
  pVertices[0].p.z=0.5f;
  pVertices[0].w=2.0f;
  pVertices[0].color=color;
  pVertices[1].p.x=(float4)vp.Width;
  pVertices[1].p.y=0;
  pVertices[1].p.z=0.5f;
  pVertices[1].w=2.0f;
  pVertices[1].color=color;
  pVertices[2].p.x=(float4)vp.Width;
  pVertices[2].p.y=(float4)vp.Height;
  pVertices[2].p.z=0.5f;
  pVertices[2].w=2.0f;
  pVertices[2].color=color;

  // 5
  // |
  // 4 --- 3
  pVertices[3].p.x=(float4)vp.Width;
  pVertices[3].p.y=(float4)vp.Height;
  pVertices[3].p.z=0.5f;
  pVertices[3].w=2.0f;
  pVertices[3].color=color;
  pVertices[4].p.x=0;
  pVertices[4].p.y=(float4)vp.Height;
  pVertices[4].p.z=0.5f;
  pVertices[4].w=2.0f;
  pVertices[4].color=color;
  pVertices[5].p.x=0;
  pVertices[5].p.y=0;
  pVertices[5].p.z=0.5f;
  pVertices[5].w=2.0f;
  pVertices[5].color=color;

  hr=p_VertexBuffer->Unlock();
  hr=p_RenderLib->p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

  p_RenderLib->p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

