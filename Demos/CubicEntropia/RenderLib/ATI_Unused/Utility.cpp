#include "RenderLib.h"

char8 CRenderLib::D3DFormat2Bpp (D3DFORMAT format)
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

BOOL CRenderLib::IsPow2 (CGraphicSurface *t)
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

int32 CRenderLib::CopyRGBTetxureToSurface (CGraphicSurface *t, LPDIRECT3DTEXTURE8 s)
{
	int32 pixel_ofs;
    D3DLOCKED_RECT  d3dlr;
	D3DSURFACE_DESC d3dsd;
	INT Pitch;
    BYTE* pBytes;
	DWORD *src;
	HRESULT hr;

    // descrizione della superficie texture
    s->GetLevelDesc(0, &d3dsd);
    // Lock the texture
    if (FAILED(s->LockRect(0, &d3dlr, 0, 0))) return(0);

	Pitch=d3dlr.Pitch;
    pBytes = (BYTE*)d3dlr.pBits;
    pixel_ofs=0;
	src=(DWORD *)t->p_Pixels;

    if (d3dsd.Format==D3DFMT_X8R8G8B8)
    for (DWORD y=0; y<d3dsd.Height; y++)
    {
        DWORD* pDstData32 = (DWORD*)pBytes;
        for (DWORD x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = src[pixel_ofs];
            pixel_ofs++;
            pDstData32[x] = dwPixel;
        }
        pBytes += Pitch;
    }
	else
    if (d3dsd.Format==D3DFMT_R5G6B5)
    for (DWORD y=0; y<d3dsd.Height; y++)
    {
        WORD*  pDstData16 = (WORD*)pBytes;
        for (DWORD x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = src[pixel_ofs];
            pixel_ofs++;

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
    return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::CopyARGBTetxureToSurface (CGraphicSurface *t, LPDIRECT3DTEXTURE8 s)
{
	int32 pixel_ofs;
    D3DLOCKED_RECT  d3dlr;
	D3DSURFACE_DESC d3dsd;
	INT Pitch;
    BYTE* pBytes;
	DWORD *src;
	HRESULT hr;

    // descrizione della superficie texture
    s->GetLevelDesc(0, &d3dsd);
    // Lock the texture
    if (FAILED(s->LockRect(0, &d3dlr, 0, 0))) return(0);

	Pitch=d3dlr.Pitch;
    pBytes = (BYTE*)d3dlr.pBits;
    pixel_ofs=0;
	src=(DWORD *)t->p_Pixels;

    if (d3dsd.Format==D3DFMT_A8R8G8B8)
    for (DWORD y=0; y<d3dsd.Height; y++)
    {
        DWORD* pDstData32 = (DWORD*)pBytes;
        for (DWORD x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = src[pixel_ofs];
            pixel_ofs++;
            pDstData32[x] = dwPixel;
        }
        pBytes += Pitch;
    }
	else
    if (d3dsd.Format==D3DFMT_R5G6B5)
    for (DWORD y=0; y<d3dsd.Height; y++)
    {
        WORD*  pDstData16 = (WORD*)pBytes;
        for (DWORD x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = src[pixel_ofs];
            pixel_ofs++;

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
    return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::CopyBumpTetxureToSurface (CGraphicSurface *t, LPDIRECT3DTEXTURE8 s, int32 bumpType)
{
  D3DLOCKED_RECT  d3dlr;
  D3DSURFACE_DESC d3dsd;
  HRESULT hr;

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
     BYTE *pDstTopRow = (BYTE *)d3dlr.pBits;
     BYTE *pDstCurRow = pDstTopRow;
     BYTE *pDstBotRow = pDstTopRow + (dwDstPitch * (d3dsd.Height - 1));

     for (DWORD y=0; y<d3dsd.Height; y++)
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

       for (DWORD x=0; x<d3dsd.Width; x++)
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
         pSrcB0++;
         pSrcB1++;
         pSrcB2++;
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
     s->UnlockRect(0);
  }
  return(1);
}

//--------------------------------------------------------------------

int32 CRenderLib::BuildGreyMap(CGraphicSurface *src, CGraphicSurface *dest)
{
   int32 i;
   uint32 RGB, R, G, B, *srcData;
   uchar8 *destData, cLUM;
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
	  fR=(float4)R;
	  fG=(float4)G;
	  fB=(float4)B;
	  // algoritmo "luma"
	  fLUM=(float4)(fR*0.299+fG*0.587+fB*0.114);

	  // algoritmo "luminesance"
	  //fLUM=(float4)(fR*0.212671+fG*0.715160+fB*0.072169);
	  cLUM=(uchar8)fLUM;
	  destData[i]=cLUM;
   }
   return(1);
}