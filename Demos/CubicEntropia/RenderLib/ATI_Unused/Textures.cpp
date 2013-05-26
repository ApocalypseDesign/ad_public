#include "RenderLib.h"

int32 CRenderLib::m_AddUpdate_Texture (CGraphicSurface *t, int32 mipmap_levels)
{
  HRESULT hr;
  char8 msg[200]="";

  if (t==NULL) return(0);
  if (IsPow2(t)==FALSE)
  {
     strcpy(msg, t->p_Name);
	 strcat(msg, " has no power of 2 dimensions");
     m_Error(msg);
  }
  if (t->p_Type==GREYSCALE)
  {
     strcpy(msg, t->p_Name);
	 strcat(msg, " is not a true color bitmap (only truecolor bitmap can be used as Map1 and Map2!)");
     m_Error(msg);
  }

  if (!t->p_HWSurface)
  {
	// la texture va aggiunta; cerco prima dello stesso formato
	// che ha il video
    hr=p_Device->CreateTexture(t->p_Width, t->p_Height, mipmap_levels,
		                       0, p_VideoMode.Format, D3DPOOL_MANAGED,
							   &t->p_HWSurface);
	// cerco in sequenza 24, 32, 16, 15
	if (hr != D3D_OK)
	{
      hr=p_Device->CreateTexture(t->p_Width, t->p_Height, mipmap_levels,
		                         0, D3DFMT_R8G8B8, D3DPOOL_MANAGED,
								 &t->p_HWSurface);
	  if (hr != D3D_OK)
	  {
        hr=p_Device->CreateTexture(t->p_Width, t->p_Height, mipmap_levels,
			                       0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
								   &t->p_HWSurface);
	    if (hr != D3D_OK)
		{
          hr=p_Device->CreateTexture(t->p_Width, t->p_Height, mipmap_levels,
			                         0, D3DFMT_R5G6B5, D3DPOOL_MANAGED,
								     &t->p_HWSurface);
	      if (hr != D3D_OK)
		  {
            hr=p_Device->CreateTexture(t->p_Width, t->p_Height, mipmap_levels,
				                       0, D3DFMT_X1R5G5B5, D3DPOOL_MANAGED,
								       &t->p_HWSurface);
			
			if (hr != D3D_OK)
			  m_D3DError(hr, "Not good texture formats found");
		  }
		}
	  }
	}
  }
  return(CopyRGBTetxureToSurface(t, t->p_HWSurface));
}

//--------------------------------------------------------------------

int32 CRenderLib::m_AddUpdate_AlphaTexture (CGraphicSurface *t, int32 mipmap_levels)
{
  HRESULT hr;
  char8 msg[200]="";

  if (t==NULL) return(0);
  if (IsPow2(t)==FALSE)
  {
     strcpy(msg, t->p_Name);
	 strcat(msg, " has no power of 2 dimensions");
     m_Error(msg);
  }

  if (!t->p_HWSurface)
  {
    hr=D3DERR_NOTFOUND ;
	// la texture va aggiunta; cerco prima dello stesso formato
	// che ha il video (o meglio stessa profondita')
	if ((p_VideoMode.Format==D3DFMT_R8G8B8) ||
        (p_VideoMode.Format==D3DFMT_X8R8G8B8))
	{
       hr=p_Device->CreateTexture(t->p_Width, t->p_Height, mipmap_levels,
		                          0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
						          &t->p_HWSurface);
	}
	// cerco a 16 bit
	if (hr != D3D_OK)
	{
       hr=p_Device->CreateTexture(t->p_Width, t->p_Height, mipmap_levels,
		                          0, D3DFMT_A1R5G5B5, D3DPOOL_MANAGED,
								  &t->p_HWSurface);
	   if (hr != D3D_OK) return(0);
		  m_D3DError(hr, "Not good alpha texture formats found");
	}
  }
  return(CopyARGBTetxureToSurface(t, t->p_HWSurface));
}

//--------------------------------------------------------------------

int32 CRenderLib::m_AddUpdate_BumpTexture (CGraphicSurface *t, int32 bumpType)
{
  HRESULT hr;
  char8 msg[200]="";

  if (t==NULL) return(0);
  if (IsPow2(t)==FALSE)
  {
     strcpy(msg, t->p_Name);
	 strcat(msg, " has no power of 2 dimensions");
     m_Error(msg);
  }

  if (!t->p_HWSurface)
  {
     if (bumpType & BUMP_EMBM)
	 {
       // creo la texture a 16bit per la mappa dei delta richiesta
	   // dall'EMBM (provo il formato V8U8)
       hr=p_Device->CreateTexture(t->p_Width, t->p_Height, 1, 
                                  0, D3DFMT_V8U8, D3DPOOL_MANAGED,
                                  &t->p_HWSurface);
       if (hr != D3D_OK)
	     m_D3DError(hr, "U8V8 bump format not found");
	 }
	 else
     // creo la texture a 32bit per la normal map richiesta dal DOT3
	 // (provo i formati A8R8G8B8 e X8R8G8B8)
     if (bumpType & BUMP_DOT3)
	 {
       hr=p_Device->CreateTexture(t->p_Width, t->p_Height, 1, 
                                  0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
                                  &t->p_HWSurface);
       if (hr != D3D_OK)
       hr=p_Device->CreateTexture(t->p_Width, t->p_Height, 1, 
                                  0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
                                  &t->p_HWSurface);
       if (hr != D3D_OK)
	     m_D3DError(hr, "D3DFMT_X8R8G8B8 nor D3DFMT_A8R8G8B8 format(s) not found, DOT3 needs one of them!!");
	 }
  }
  return(CopyBumpTetxureToSurface(t, t->p_HWSurface, bumpType));
}

//--------------------------------------------------------------------

void CRenderLib::m_BuildNormalizationCubemap(int32 pixelDim, int32 mipmapLevels)
{
  HRESULT hr;
  uint32 i, y, x;

  hr = p_Device->CreateCubeTexture(pixelDim, mipmapLevels, 0,
	                               D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
								   &p_NormalizationCubeMapTexture);
  if (hr != D3D_OK)
    m_D3DError(hr, "Normalization CubeMap can't be created!");


  for (i=0; i<6; i++)
  {
     D3DLOCKED_RECT Locked;
     AD_Vect3D Normal;
     float4 w, h;
     D3DSURFACE_DESC ddsdDesc;
		
     p_NormalizationCubeMapTexture->GetLevelDesc(0, &ddsdDesc);
     p_NormalizationCubeMapTexture->LockRect((D3DCUBEMAP_FACES)i, 0, &Locked, NULL, 0);

     for (y=0; y<ddsdDesc.Height; y++)
	 {
        h = (float4)y / ((float4)(ddsdDesc.Height - 1));
        h *= 2.0f;
        h -= 1.0f;

        for (x=0; x<ddsdDesc.Width; x++)
		{
           w = (float4)x / ((float4)(ddsdDesc.Width - 1));
           w *= 2.0f;
           w -= 1.0f;
           DWORD* pBits = (DWORD*)((BYTE*)Locked.pBits + (y * Locked.Pitch));
           pBits += x;
           switch((D3DCUBEMAP_FACES)i)
		   {
              case D3DCUBEMAP_FACE_POSITIVE_X:
                        vect_set(&Normal, 1.0f, -h, -w);
						break;
              case D3DCUBEMAP_FACE_NEGATIVE_X:
						vect_set(&Normal, -1.0f, -h, w);
						break;
              case D3DCUBEMAP_FACE_POSITIVE_Y:
                        vect_set(&Normal, w, 1.0f, h);
						break;
              case D3DCUBEMAP_FACE_NEGATIVE_Y:
                        vect_set(&Normal, w, -1.0f, -h);
						break;
              case D3DCUBEMAP_FACE_POSITIVE_Z:
                        vect_set(&Normal, w, -h, 1.0f);
						break;
              case D3DCUBEMAP_FACE_NEGATIVE_Z:
                        vect_set(&Normal, -w, -h, -1.0f);
						break;
		   }
           vect_auto_normalize(&Normal);
           // Scale to be a color from 0 to 255 (127 is 0)
		   Normal.x=(Normal.x+1)*127.0f;
		   Normal.y=(Normal.y+1)*127.0f;
		   Normal.z=(Normal.z+1)*127.0f;
           *pBits = (DWORD)(((DWORD)Normal.x << 16) | ((DWORD)Normal.y << 8) | ((DWORD)Normal.z << 0));
		}
	 }
     p_NormalizationCubeMapTexture->UnlockRect((D3DCUBEMAP_FACES)i, 0);
  }
}

//--------------------------------------------------------------------