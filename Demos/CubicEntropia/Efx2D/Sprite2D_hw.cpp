#include "Sprite2D.h"
#include <FastMath.h>

extern HWND miawin;
extern FILE *fdeb;

struct SPRITE2DVERTEX
{ 
   AD_Vect3D p;
   float4 w;
 //  int32 color;
   AD_VectUV uv1;
};
//#define FVF_SPRITE2DVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_SPRITE2DVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)


CSprite2D::CSprite2D()
{
  p_VertexBuffer=NULL;
  p_Texture=(CGraphicSurface *)NULL;
  p_BlendType=BLEND_ADD;
  p_Envelop1=0;
  p_Envelop2=0;

  p_NumKeys=0;
  for (int32 w=0; w<MAXKEY; w++)
  {
	p_Keys[w].relpos=0;
    p_Keys[w].x=0;
	p_Keys[w].y=0;
    p_Keys[w].scale_x=1;
	p_Keys[w].scale_y=1;
  }

}


//--------------------------------------------------------------------

void CSprite2D::m_AddKey(float4 relpos, float4 x, float4 y, float4 scale_x, float4 scale_y)
{
  if (p_NumKeys>MAXKEY) return;

  if (relpos<0) relpos=0;
  if (relpos>1) relpos=1;

  p_Keys[p_NumKeys].relpos=relpos;
  p_Keys[p_NumKeys].x=x;
  p_Keys[p_NumKeys].y=y;

//  if (scale_x<=0) scale_x=NOSCALING;
//  if (scale_y<=0) scale_y=NOSCALING;

  p_Keys[p_NumKeys].scale_x=scale_x;
  p_Keys[p_NumKeys].scale_y=scale_y;
  p_NumKeys++;
}


//--------------------------------------------------------------------

int32 CSprite2D::m_SetParameters (char8 *filename, int32 blendType,
	                              float4 envelop1, float4 envelop2)
{
   int32 *data, i;

   if (!filename) debug_error(miawin, "CSprite2D::m_SetParameters, filename is NULL!");
   
   p_Envelop1=envelop1;
   p_Envelop2=envelop2;
   if ((blendType==BLEND_ADD) || (blendType==BLEND_MODULATE))
     p_BlendType=blendType;

   p_Texture=new CGraphicSurface;
   if (p_Texture->m_Load(filename, RGBINTERLEAVED)==FILENOTFOUND)
     debug_error(miawin, "CSprite2D::m_SetParameters, texture filename doesn't exist!");


   p_RenderLib->m_AddUpdate_Texture(p_Texture, 1);

   return(1);
}


 //--------------------------------------------------------------------
int32 CSprite2D::m_Init(void)
{ 
  int32 res;
  HRESULT hr;
  
  res=CEfx2D::m_Init();
  if (FAILED(hr = p_RenderLib->p_Device->CreateVertexBuffer(6*sizeof(SPRITE2DVERTEX),
                                                            D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                            D3DPOOL_DEFAULT, &p_VertexBuffer)))
         return(0);
  DumpDebugPoint (fdeb, "CSprite2D Init cerate vertex buffer done", 0);



  if (p_Texture)
  {
    //p_RenderLib->m_AddUpdate_Texture(p_Texture, 1);
  }

  return(1);
} 

//--------------------------------------------------------------------

void CSprite2D::m_SetStages(void)
{
  HRESULT hr;

  hr=p_RenderLib->p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  hr=p_RenderLib->p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  if (p_BlendType==BLEND_ADD)
  {
    hr=p_RenderLib->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //hr=p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    hr=p_RenderLib->p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
    hr=p_RenderLib->p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  }
  else
  if (p_BlendType==BLEND_MODULATE)
  {
    hr=p_RenderLib->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    hr=p_RenderLib->p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    hr=p_RenderLib->p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
  }
  else
  {
    hr=p_RenderLib->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  }

  hr=p_RenderLib->p_Device->SetVertexShader(FVF_SPRITE2DVERTEX);
  hr=p_RenderLib->p_Device->SetStreamSource(0, p_VertexBuffer, sizeof(SPRITE2DVERTEX));
  if (p_Texture)
  {
    hr=p_RenderLib->p_Device->SetTexture(0, p_Texture->p_HWSurfaces[0]);
    hr=p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    hr=p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    hr=p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  }
  else
  {
    //hr=p_RenderLib->p_Device->SetTexture(0, NULL);
    //hr=p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    //hr=p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  }
  hr=p_RenderLib->p_Device->SetTexture(1, NULL);
  hr=p_RenderLib->p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=p_RenderLib->p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

//--------------------------------------------------------------------

void CSprite2D::m_DoEfx2D(double pos)
{
  SPRITE2DVERTEX *pVertices;
  float4 cpos;
  int32 intensity, color;
  HRESULT hr;
  D3DVIEWPORT8 vp;
  int32 i, xx, yy;
  float4 t;

  #define ZZ 0.5f
  #define WW 2.0f


  // consideriamo la pos nel range [0;1]
  cpos=(float4)((pos-fstartpos)/(fendpos-fstartpos));


  // calcolo l'interpolazione tra le key
  if (p_NumKeys<0)
  {
	 xx=0;
	 yy=0;
  }
  else
  if (p_NumKeys==1)
  {
    xx=p_Keys[0].x;
	yy=p_Keys[0].y;
  }
  else
  {
    i=0;
    while ((i<p_NumKeys) && (p_Keys[i].relpos<cpos)) i++;
    if (i==0) i=1;
    // interpolazioni lineari tra le key
    t=(cpos - p_Keys[i-1].relpos)/(p_Keys[i].relpos - p_Keys[i-1].relpos);
    xx=((1.0f-t)*p_Keys[i-1].x + t*p_Keys[i].x);
    yy=((1.0f-t)*p_Keys[i-1].y + t*p_Keys[i].y);
  }


  p_RenderLib->p_Device->GetViewport(&vp);

  hr=p_VertexBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);
  #define U0 0
  #define U1 1
  #define V0 0
  #define V1 1

  // 0 --- 1
  //       |
  //       2
  pVertices[0].p.x=xx;
  pVertices[0].p.y=yy;
//  pVertices[0].color=color;
  pVertices[0].uv1.u=U0;
  pVertices[0].uv1.v=V0;
  pVertices[0].p.z=ZZ;
  pVertices[0].w=WW;

  pVertices[1].p.x=xx+p_Texture->p_Width;//(float4)vp.Width;
  pVertices[1].p.y=yy;
  pVertices[1].p.z=ZZ;
  pVertices[1].w=WW;
  //pVertices[1].color=color;
  pVertices[1].uv1.u=U1;
  pVertices[1].uv1.v=V0;

  pVertices[2].p.x=xx+p_Texture->p_Width;//(float4)vp.Width;
  pVertices[2].p.y=yy+p_Texture->p_Height;//(float4)vp.Height;
  pVertices[2].p.z=ZZ;
  pVertices[2].w=WW;
  //pVertices[2].color=color;
  pVertices[2].uv1.u=U1;
  pVertices[2].uv1.v=V1;

  // 5
  // |
  // 4 --- 3
  pVertices[3].p.x=xx+p_Texture->p_Width;//(float4)vp.Width;
  pVertices[3].p.y=yy+p_Texture->p_Height;//(float4)vp.Height;
  pVertices[3].p.z=ZZ;
  pVertices[3].w=WW;
  //pVertices[2].color=color;
  pVertices[3].uv1.u=U1;
  pVertices[3].uv1.v=V1;

  pVertices[4].p.x=xx;
  pVertices[4].p.y=yy+p_Texture->p_Height;//(float4)vp.Height;
  pVertices[4].p.z=ZZ;
  pVertices[4].w=WW;
  //pVertices[4].color=color;
  pVertices[4].uv1.u=U0;
  pVertices[4].uv1.v=V1;

  pVertices[5].p.x=xx;
  pVertices[5].p.y=yy;
  pVertices[5].p.z=ZZ;
  pVertices[5].w=WW;
  //pVertices[5].color=color;
  pVertices[5].uv1.u=U0;
  pVertices[5].uv1.v=V0;
  hr=p_VertexBuffer->Unlock();

  m_SetStages();

  hr=p_RenderLib->p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
  p_RenderLib->p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}

