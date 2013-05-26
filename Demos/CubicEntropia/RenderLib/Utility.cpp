#include <math.h>
#include "RenderLib.h"
#include "D3DX8Tex.h"

extern FILE *fdebug;

#define MIPMAP_FILTER D3DX_FILTER_BOX

char8 CRenderLib::m_D3DFormat2Bpp (D3DFORMAT format)
{
   switch (format)
   {
	  case D3DFMT_R5G6B5:
      case D3DFMT_X1R5G5B5: return(16);
		                    break;

      case D3DFMT_R8G8B8: return(24);
		                  break;

      case D3DFMT_A8R8G8B8:
      case D3DFMT_X8R8G8B8: return(32);
		                    break;
   }
   return(0);
}

//--------------------------------------------------------------------

BOOL CRenderLib::m_IsPow2 (CGraphicSurface *t)
{
  int32 i, count, mask;

  count=0;
  for (i=0; i<32; i++)
  {
	 mask=1<<i;
	 if (t->p_Width & mask) count++;
	 if (t->p_Height & mask) count++;
  }
  if (count!=2) return(FALSE);
  else return(TRUE);
}

//--------------------------------------------------------------------

D3DTEXTUREOP CRenderLib::m_MixType2ColorOp(int32 mixType)
{
  switch (mixType)
  {
     case MIXTYPE_ADD:
       return(D3DTOP_ADD);
	   break;
     case MIXTYPE_SUB:
       return(D3DTOP_SUBTRACT);
	   break;
     case MIXTYPE_MODULATE:
       return(D3DTOP_MODULATE);
	   break;
     case MIXTYPE_MODULATE2X:
       return(D3DTOP_MODULATE2X);
	   break;
     case MIXTYPE_MODULATE4X:
       return(D3DTOP_MODULATE4X);
	   break;
     case MIXTYPE_ADDSIGNED:
       return(D3DTOP_ADDSIGNED);
	   break;
     case MIXTYPE_ADDSIGNED2X:
       return(D3DTOP_ADDSIGNED2X);
	   break;
     case MIXTYPE_ADDSMOOTH:
       return(D3DTOP_ADDSMOOTH);
	   break;
     default:
       return(D3DTOP_MODULATE);
	   break;
  }
}

//--------------------------------------------------------------------

int32 CRenderLib::m_CopyRGBTetxureToSurface (CGraphicSurface *t, int32 i, LPDIRECT3DTEXTURE8 s, int32 mipmap_levels)
{
	int32 pixel_ofs;
	uint32 *map32;
    D3DLOCKED_RECT  d3dlr;
	D3DSURFACE_DESC d3dsd;
	INT Pitch;
    BYTE* pBytes;
	DWORD x, y;
	uint32 *src;
	HRESULT hr;

    s->GetLevelDesc(0, &d3dsd);
    if (FAILED(s->LockRect(0, &d3dlr, 0, 0))) return(0);

	Pitch=d3dlr.Pitch;
    pBytes = (BYTE *)d3dlr.pBits;
	map32=(uint32 *)t->p_Pixels;
	src=(uint32 *)&map32[i*t->p_ChannelSize];
    pixel_ofs=0;

    if (d3dsd.Format==D3DFMT_X8R8G8B8)
      for (y=0; y<d3dsd.Height; y++)
	  {
         DWORD *pDstData32 = (DWORD *)pBytes;
         for (x=0; x<d3dsd.Width; x++)
		 {
			uint32 p1;

			p1=src[pixel_ofs++];
			pDstData32[x] = p1;
		 }
         pBytes += Pitch;
	  }
	else
    if (d3dsd.Format==D3DFMT_R5G6B5)
      for (y=0; y<d3dsd.Height; y++)
	  {
         WORD *pDstData16 = (WORD *)pBytes;
         for (x=0; x<d3dsd.Width; x++)
         {
            DWORD dwPixel = src[pixel_ofs++];
            DWORD r = (dwPixel >> 19) & (DWORD)(31);
            DWORD g = (dwPixel >> 10) & (DWORD)(63);
            DWORD b = (dwPixel >> 3) & (DWORD)(31);
            r = r << 11;
			g = g << 5;
            pDstData16[x] = (WORD)(r+g+b);
         }
         pBytes += Pitch;
	  }
	hr=s->UnlockRect(0);
    if (mipmap_levels>1) hr=D3DXFilterTexture(s, NULL, 0, MIPMAP_FILTER);
    return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_CopyARGBTetxureToSurface (CGraphicSurface *t, int32 i, LPDIRECT3DTEXTURE8 s, int32 mipmap_levels)
{
	int32 pixel_ofs;
	uint32 *map32;
    D3DLOCKED_RECT  d3dlr;
	D3DSURFACE_DESC d3dsd;
	INT Pitch;
    BYTE* pBytes;
	DWORD *src, x, y;
	HRESULT hr;

    s->GetLevelDesc(0, &d3dsd);
    if (FAILED(s->LockRect(0, &d3dlr, 0, 0))) return(0);

	Pitch=d3dlr.Pitch;
    pBytes = (BYTE*)d3dlr.pBits;
	map32=(uint32 *)t->p_Pixels;
	src=(DWORD *)&map32[i*t->p_ChannelSize];
    pixel_ofs=0;

    if (d3dsd.Format==D3DFMT_A8R8G8B8)
    for (y=0; y<d3dsd.Height; y++)
    {
        DWORD *pDstData32 = (DWORD *)pBytes;
        for (x=0; x<d3dsd.Width; x++)
            pDstData32[x] = src[pixel_ofs++];
        pBytes += Pitch;
    }
	else
    if (d3dsd.Format==D3DFMT_A1R5G5B5)
    for (y=0; y<d3dsd.Height; y++)
    {
        WORD *pDstData16 = (WORD *)pBytes;
        for (x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = src[pixel_ofs++];
            DWORD a = (dwPixel >> 31) & (DWORD)(1);
            DWORD r = (dwPixel >> 19) & (DWORD)(31);
            DWORD g = (dwPixel >> 11) & (DWORD)(31);
            DWORD b = (dwPixel >> 3) & (DWORD)(31);
			a = a << 15;
            r = r << 10;
			g = g << 5;
            pDstData16[x] = (WORD)(a+r+g+b);
        }
        pBytes += Pitch;
    }
	hr=s->UnlockRect(0);
    if (mipmap_levels>1) hr=D3DXFilterTexture(s, NULL, 0, MIPMAP_FILTER);
    return(1);
}

//--------------------------------------------------------------------

BYTE PackFloatInByte(float4 in)
{ return (BYTE) ((in+1.0f) / 2.0f * 255.0f); }

int32 CRenderLib::m_CopyBumpTetxureToSurface (CGraphicSurface *t, int32 i, LPDIRECT3DTEXTURE8 s, int32 bumpType, int32 mipmap_levels)
{
  D3DLOCKED_RECT  d3dlr;
  D3DSURFACE_DESC d3dsd;
  HRESULT hr;
  DWORD x, y;

  // descrizione della superficie texture
  s->GetLevelDesc(0, &d3dsd);

  if (bumpType & BUMP_EMBM)
  {
     if (d3dsd.Format != D3DFMT_V8U8) return(0);
     if (FAILED(s->LockRect(0, &d3dlr, 0, 0))) return(0);

     DWORD dwSrcPitch = (DWORD)t->p_Width;
     BYTE *pSrcTopRow = (BYTE *)t->p_Pixels;
     BYTE *pSrcCurRow = pSrcTopRow;
     BYTE *pSrcBotRow = pSrcTopRow + (dwSrcPitch * (t->p_Height - 1));

	 DWORD dwDstPitch = (DWORD)d3dlr.Pitch;
     BYTE *pDstCurRow = (BYTE *)d3dlr.pBits;

     for (y=0; y<d3dsd.Height; y++)
	 {
       BYTE *pSrcB0; // addr of current pixel
       BYTE *pSrcB1; // addr of pixel below current pixel, wrapping to top if necessary
       BYTE *pSrcB2; // addr of pixel above current pixel, wrapping to bottom if necessary
       BYTE *pDstT;  // addr of dest pixel;

       pSrcB0 = pSrcCurRow;
       if (y == d3dsd.Height - 1) pSrcB1 = pSrcTopRow;
       else pSrcB1 = pSrcCurRow + dwSrcPitch;
       if (y == 0) pSrcB2 = pSrcBotRow;
       else pSrcB2 = pSrcCurRow - dwSrcPitch;
       pDstT = pDstCurRow;

       for (x=0; x<d3dsd.Width; x++)
	   {
         BYTE v00; // Current pixel
         BYTE v01; // Pixel to the right of current pixel, wrapping to left edge if necessary
         BYTE vM1; // Pixel to the left of current pixel, wrapping to right edge if necessary
         BYTE v10; // Pixel one line below.
         BYTE v1M; // Pixel one line above.

         v00 = *(pSrcB0+0);
         if (x == d3dsd.Width - 1) v01 = *(pSrcCurRow);
         else v01 = *(pSrcB0+4);
         if (x == 0) vM1 = *(pSrcCurRow + (4 * (d3dsd.Width - 1)));
         else vM1 = *(pSrcB0-4);
         v10 = *(pSrcB1+0);
         v1M = *(pSrcB2+0);

         LONG iDu = LONG(vM1)-LONG(v01); // the delta-u bump value
         LONG iDv = LONG(v1M)-LONG(v10); // the delta-v bump value
         *pDstT++ = (BYTE)iDu;
         *pDstT++ = (BYTE)iDv;
         // Move one pixel to the right (src is 8-bpp)
         pSrcB0++; pSrcB1++; pSrcB2++;
	   }
       // Move to the next line
       pSrcCurRow += dwSrcPitch;
       pDstCurRow += dwDstPitch;
	 }
     hr=s->UnlockRect(0);
  }
  else
  if (bumpType & BUMP_DOT3)
  {
     if ((d3dsd.Format != D3DFMT_A8R8G8B8) &&
		 (d3dsd.Format != D3DFMT_X8R8G8B8))
		 return(0);
     if (FAILED(s->LockRect(0, &d3dlr, 0, 0))) return(0);

     DWORD dwSrcPitch = (DWORD)t->p_Width;
     BYTE *pSrcTopRow = (BYTE *)t->p_Pixels;
     BYTE *pSrcCurRow = pSrcTopRow;
     BYTE *pSrcBotRow = pSrcTopRow + (dwSrcPitch * (t->p_Height - 1));

	 DWORD dwDstPitch = (DWORD)d3dlr.Pitch;
     BYTE *pDstCurRow = (BYTE *)d3dlr.pBits;

	 float4 dX, dY;
	 float4 nX, nY, nZ, oolen;

     // builds normal map from height map
	 for (y=0; y<d3dsd.Height; y++)
	 {
       BYTE *pSrcB0; // addr of current pixel
       BYTE *pSrcB1; // addr of pixel below current pixel, wrapping to top if necessary
       BYTE *pSrcB2; // addr of pixel above current pixel, wrapping to bottom if necessary
       BYTE *pDstT;  // addr of dest pixel;

       pSrcB0 = pSrcCurRow;
       if (y == d3dsd.Height - 1) pSrcB1 = pSrcTopRow;
       else pSrcB1 = pSrcCurRow + dwSrcPitch;
       if (y == 0) pSrcB2 = pSrcBotRow;
       else pSrcB2 = pSrcCurRow - dwSrcPitch;
       pDstT = pDstCurRow;

       for (x=0; x<d3dsd.Width; x++)
	   {
		 int32 x1=x+1, x_1=x-1;

         if (x == d3dsd.Width - 1) x1 = 0;
         if (x == 0) x_1 = d3dsd.Width - 1;

		 // Y Sobel filter
		 dY =  ((float4)pSrcB1[x_1]) / 255.0f * -1.0f;
		 dY += ((float4)pSrcB1[x])   / 255.0f * -2.0f;
		 dY += ((float4)pSrcB1[x1])  / 255.0f * -1.0f;
		 dY += ((float4)pSrcB2[x_1]) / 255.0f * +1.0f;
		 dY += ((float4)pSrcB2[x])   / 255.0f * +2.0f;
		 dY += ((float4)pSrcB2[x1])  / 255.0f * +1.0f;
          // X Sobel filter
		 dX =  ((float4)pSrcB2[x_1]) / 255.0f * -1.0f;
		 dX += ((float4)pSrcB0[x_1]) / 255.0f * -2.0f;
		 dX += ((float4)pSrcB1[x_1]) / 255.0f * -1.0f;
		 dX += ((float4)pSrcB2[x1])  / 255.0f * +1.0f;
		 dX += ((float4)pSrcB0[x1])  / 255.0f * +2.0f;
		 dX += ((float4)pSrcB1[x1])  / 255.0f * +1.0f;
            
         // Cross Product of components of gradient reduces to
         nX = -dX;
         nY = -dY;
         nZ = 1;
            
         // Normalize
         oolen = 1.0f/((float4)sqrt(nX*nX + nY*nY + nZ*nZ));
         nX *= oolen;
         nY *= oolen;
         nZ *= oolen;
		 *pDstT++=(BYTE) PackFloatInByte(nX);
		 *pDstT++=(BYTE) PackFloatInByte(nY);
		 *pDstT++=(BYTE) PackFloatInByte(nZ);
		 pDstT++;
	   }
       // Move to the next line
       pSrcCurRow += dwSrcPitch;
       pDstCurRow += dwDstPitch;
	 }
     hr=s->UnlockRect(0);
  }
  if (mipmap_levels>1) hr=D3DXFilterTexture(s, NULL, 0, MIPMAP_FILTER);
  return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::m_BuildGreyMap(CGraphicSurface *src, CGraphicSurface *dest)
{
   int32 i;
   uint32 RGB, R, G, B, *srcData;
   uchar8 *destData;
   float4 fR, fG, fB, fLUM;

   if ((src->p_Width != dest->p_Width) ||
	   (src->p_Height != dest->p_Height))
	   return(0);

   srcData=(uint32 *)src->p_Pixels;
   destData=(uchar8 *)dest->p_Pixels;
   for (i=0; i<src->p_ChannelSize; i++)
   {
      RGB=srcData[i];
	  R=(RGB >> 16) & 255;
	  G=(RGB >> 8) & 255;
	  B=RGB & 255;
	  fR=(float4)R; fG=(float4)G; fB=(float4)B;
	  // algoritmo "luma"
	  fLUM=(float4)(fR*0.299+fG*0.587+fB*0.114);
	  // algoritmo "luminesance"
	  //fLUM=(float4)(fR*0.212671+fG*0.715160+fB*0.072169);
	  destData[i]=(uchar8)fLUM;
   }
   return(1);
}