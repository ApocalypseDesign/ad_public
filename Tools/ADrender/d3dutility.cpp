#include "D3Dutility.h"
#include "ADbase.h"

BOOL is_texture_pow2 (texture *tex)
{
  int i, count, mask;

  count=0;
  for (i=0; i<32; i++)
  {
	 mask=1<<i;
	 if (tex->dimx & mask) count++;
	 if (tex->dimy & mask) count++;
  }
  if (count!=2) return(FALSE);
  else return(TRUE);
}

int CopyRGBTetxureToSurface (texture *t, LPDIRECT3DTEXTURE9 s)
{
	int pixel_ofs=0;

    // descrizione della superficie texture
	D3DSURFACE_DESC d3dsd;
    s->GetLevelDesc(0, &d3dsd);

    // Lock the texture
    D3DLOCKED_RECT  d3dlr;
    if (FAILED(s->LockRect(0, &d3dlr, 0, 0))) return(-1);

	INT Pitch = d3dlr.Pitch;
    BYTE* pBytes = (BYTE*)d3dlr.pBits;

    if(d3dsd.Format==D3DFMT_X8R8G8B8)
    for (DWORD y=0; y<d3dsd.Height; y++)
    {
        DWORD* pDstData32 = (DWORD*)pBytes;
        for(DWORD x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = t->map32[pixel_ofs];
            pixel_ofs++;
            pDstData32[x] = dwPixel;
        }
        pBytes += Pitch;
    }
	else
    if(d3dsd.Format==D3DFMT_R5G6B5)
    for (DWORD y=0; y<d3dsd.Height; y++)
    {
        WORD*  pDstData16 = (WORD*)pBytes;
        for(DWORD x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = t->map32[pixel_ofs];
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
	s->UnlockRect(0);
    return 0;
}


int CopyARGBTetxureToSurface (texture *t, LPDIRECT3DTEXTURE9 s)
{
	int pixel_ofs=0;

    // descrizione della superficie texture
	D3DSURFACE_DESC d3dsd;
    s->GetLevelDesc(0, &d3dsd);

    // Lock the texture
    D3DLOCKED_RECT  d3dlr;
    if (FAILED(s->LockRect(0, &d3dlr, 0, 0))) return(-1);

	INT Pitch = d3dlr.Pitch;
    BYTE* pBytes = (BYTE*)d3dlr.pBits;

    if(d3dsd.Format==D3DFMT_A8R8G8B8)
    for (DWORD y=0; y<d3dsd.Height; y++)
    {
        DWORD* pDstData32 = (DWORD*)pBytes;
        for(DWORD x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = t->map32[pixel_ofs];
            pixel_ofs++;
            pDstData32[x] = dwPixel;
        }
        pBytes += Pitch;
    }
	else
    if(d3dsd.Format==D3DFMT_A1R5G5B5)
    for (DWORD y=0; y<d3dsd.Height; y++)
    {
        WORD*  pDstData16 = (WORD*)pBytes;
        for(DWORD x=0; x<d3dsd.Width; x++)
        {
            DWORD dwPixel = t->map32[pixel_ofs];
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
	s->UnlockRect(0);
    return 0;
}