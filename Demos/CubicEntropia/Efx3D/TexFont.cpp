#include <stdio.h>
#include "TexFont.h"
#include "UtilsA3D.h"

extern HWND miawin;
extern FILE *fdeb;

#define MAX_NUM_VERTICES 50*6

struct FONT2DVERTEX
{ 
   AD_Vect3D p;
   float4 w;
   int32 color;
   float4 tu, tv;
};

#define FVF_FONT2DVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
inline FONT2DVERTEX InitFont2DVertex(float4 xx, float4 yy, float4 zz, float4 ww,
									 int32 color,
                                     float4 tu, float4 tv)
{
    FONT2DVERTEX v;
	v.p.x = xx;
	v.p.y = yy;
	v.p.z = zz;
	v.w = ww;
	v.color = color;
	v.tu = tu; v.tv = tv;
    return v;
}


//--------------------------------------------------------------------

CFont::CFont(char8* fontName, int32 height, int32 flags)
{
	strcpy(p_FontName, fontName);
    p_FontHeight         = height;
    p_FontFlags          = flags;
    p_Texture            = (CGraphicSurface *)NULL;
    p_VertexBuffer       = NULL;
    p_NumPhrases=0;
}

//--------------------------------------------------------------------

int32 CFont::m_InitMaterial(void)
{
    // Establish the font and texture size
    p_TextScale  = 1.0f; // Draw fonts into texture without scaling

    // Large fonts need larger textures
    if (p_FontHeight > 40) p_TexWidth = p_TexHeight = 1024;
    else if (p_FontHeight > 20) p_TexWidth = p_TexHeight = 512;
    else p_TexWidth = p_TexHeight = 256;

    // If requested texture is too big, use a smaller texture and smaller font,
    // and scale up when rendering.
    D3DCAPS8 d3dCaps;
    p_RenderLib->p_Device->GetDeviceCaps(&d3dCaps);

    if (p_TexWidth > d3dCaps.MaxTextureWidth)
    {
        p_TextScale = (float4)d3dCaps.MaxTextureWidth / (float4)p_TexWidth;
        p_TexWidth = p_TexHeight = d3dCaps.MaxTextureWidth;
    }

	// setup del materiale da usare
	p_Texture=new CGraphicSurface(p_TexWidth, p_TexHeight, 32);

    // Prepare to create a bitmap
    int32      *pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory(&bmi.bmiHeader,  sizeof(BITMAPINFOHEADER));
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       =  (int32)p_TexWidth;
    bmi.bmiHeader.biHeight      = -(int32)p_TexHeight;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount    = 32;

    // Create a DC and a bitmap for the font
    HDC hDC = CreateCompatibleDC(NULL);
    HBITMAP hbmBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS,
                                         (void **)&pBitmapBits, NULL, 0);
    SetMapMode(hDC, MM_TEXT);

    INT nHeight    = -MulDiv(p_FontHeight, 
        (INT)(GetDeviceCaps(hDC, LOGPIXELSY)*p_TextScale), 72);
    int32 dwBold   = (p_FontFlags & FONT_BOLD)   ? FW_BOLD : FW_NORMAL;
    int32 dwItalic = (p_FontFlags & FONT_ITALIC) ? TRUE    : FALSE;
    HFONT hFont    = CreateFont(nHeight, 0, 0, 0, dwBold, dwItalic,
                          FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                          CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                          VARIABLE_PITCH, p_FontName);
    if (NULL==hFont) return(0);

    SelectObject(hDC, hbmBitmap);
    SelectObject(hDC, hFont);
    // Set text properties
    SetTextColor(hDC, RGB(255,255,255));
    SetBkColor(hDC, 0x00000000);
    SetTextAlign(hDC, TA_TOP);
    // Loop through all printable character and output them to the bitmap..
    // Meanwhile, keep track of the corresponding tex coords for each character.
    int32 x = 0;
    int32 y = 0;
    char8 str[2];
    SIZE size;

    for (char8 c=32; c<127; c++)
    {
        str[0] = c;
        GetTextExtentPoint32(hDC, str, 1, &size);

        if ((int32)(x+size.cx+1) > p_TexWidth)
        {
            x  = 0;
            y += size.cy+1;
        }
        ExtTextOut(hDC, x+0, y+0, ETO_OPAQUE, NULL, str, 1, NULL);
        p_TexCoords[c-32][0] = ((float4)(x+0))/p_TexWidth;
        p_TexCoords[c-32][1] = ((float4)(y+0))/p_TexHeight;
        p_TexCoords[c-32][2] = ((float4)(x+0+size.cx))/p_TexWidth;
        p_TexCoords[c-32][3] = ((float4)(y+0+size.cy))/p_TexHeight;
        x += size.cx+1;
    }

    int32 *pDst32=(int32 *)p_Texture->p_Pixels;
    uchar8 bAlpha;

    for (y=0; y<p_TexHeight; y++)
    {
      for (x=0; x<p_TexWidth; x++)
      {
         bAlpha = (uchar8)((pBitmapBits[p_TexWidth*y + x] & 0xff));
         if (bAlpha > 0) *pDst32++ = (bAlpha << 24) | 0x00ffffff;
         else *pDst32++ = 0x00000000;
      }
    }

    DeleteObject(hbmBitmap);
    DeleteDC(hDC);
    DeleteObject(hFont);
	p_RenderLib->m_AddUpdate_AlphaTexture(p_Texture, 1);
	return(1);
}

//--------------------------------------------------------------------

int32 CFont::m_LoadAnimation (char8 *A3DFileName, char8 *objectName)
{
   CFileA3D fileA3D;
   int32 res;

   res=fileA3D.m_Load_PosTrack(SCALE_TRACK_ID, objectName, A3DFileName, &p_ScaleTrack);
   DumpDebugPoint (fdeb, "CFont m_LoadAnimation: LoadPosTrack Done", 0);

   res=fileA3D.m_Load_RotTrack(ROTATION_TRACK_ID, objectName, A3DFileName, &p_RotationTrack);
   DumpDebugPoint (fdeb, "CFont m_LoadAnimation: LoadRotTrack Done", 0);
   return(1);
}

//--------------------------------------------------------------------

int32 CFont::m_Init(void)
{
    HRESULT hr;
	int32 res, i;
	float4 dt, k, a;

	res=CEfx2D::m_Init();
    DumpDebugPoint (fdeb, "CFDont Init CEfx2D done", 0);

	// inizializzazione delle tracce di animazione
    p_ScaleTrack.m_Init(1);

	if (p_RotationTrack.p_NumKeys>0)
	{
      // uso l'asse z per tutte le key
	  for (i=0; i<p_RotationTrack.p_NumKeys; i++)
	  {
	    p_RotationTrack.p_Keys[i].rotquat.x=0;
	    p_RotationTrack.p_Keys[i].rotquat.y=0;
	    p_RotationTrack.p_Keys[i].rotquat.z=1;
	  }
	}
	p_RotationTrack.m_Init(1);
    DumpDebugPoint (fdeb, "CFont Keyframer done", 0);

    if (!m_InitMaterial()) return(0);
    DumpDebugPoint (fdeb, "CFont InitMaterial done", 0);

    if (FAILED(hr = p_RenderLib->p_Device->CreateVertexBuffer(MAX_NUM_VERTICES*sizeof(FONT2DVERTEX),
                                                     D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
                                                     D3DPOOL_DEFAULT, &p_VertexBuffer)))
         return(0);
    DumpDebugPoint (fdeb, "CFont Init cerate vertex buffer done", 0);

	for (i=0; i<p_NumPhrases; i++)
	{
       dt=(p_Phrases[i].relend-p_Phrases[i].relstart);
	   k=dt*p_Phrases[i].relfixed;
	   a=(dt-k)*0.5f;
       p_Phrases[i].phraseFixedStart=p_Phrases[i].relstart+a;
       p_Phrases[i].phraseFixedEnd=p_Phrases[i].relend-a;
	   // calcolo il delay massimo che puo' assumere una lettera; nel
	   // caso di stringa con un solo carattere non ha senso parlare
	   // di delay per ogni lettera
	   if (strlen(p_Phrases[i].text)<=1)
	   {
         p_Phrases[i].reldelay=0;
	   }
	   else
	   {
	     if (p_Phrases[i].reldelay> 1.0f/(strlen(p_Phrases[i].text)-1))
               p_Phrases[i].reldelay=1.0f/(strlen(p_Phrases[i].text));

         p_Phrases[i].phraseDelayPerChar=p_Phrases[i].reldelay*a;
         p_Phrases[i].phraseTimePerChar=
			 a-(strlen(p_Phrases[i].text)-1)*p_Phrases[i].phraseDelayPerChar;
	   }
	}
    DumpDebugPoint (fdeb, "CFont Init phrases done", 0);
    return(1);
}

//--------------------------------------------------------------------

int32 CFont::m_AddPhrase(float4 relstart, float4 relend,
		              float4 relfixed, float4 reldelay,
                      char8 *text,
		              float4 x, float4 y, uint32 color,
		              float4 scaleX, float4 scaleY)

{
  if (p_NumPhrases>=MAX_PHRASES) return(0);
  p_Phrases[p_NumPhrases].relstart=relstart;
  p_Phrases[p_NumPhrases].relend=relend;
  p_Phrases[p_NumPhrases].relfixed=relfixed;
  p_Phrases[p_NumPhrases].reldelay=reldelay;
  p_Phrases[p_NumPhrases].scaleX=scaleX;
  p_Phrases[p_NumPhrases].scaleY=scaleY;
  p_Phrases[p_NumPhrases].x=x;
  p_Phrases[p_NumPhrases].y=y;
  p_Phrases[p_NumPhrases].color=color;
  strcpy(p_Phrases[p_NumPhrases].text, text);
  p_NumPhrases++;
  return(1);
}

//--------------------------------------------------------------------
// Desc: Get the dimensions of a text string
//--------------------------------------------------------------------
int32 CFont::m_GetTextExtent(char8 *text, SIZE* size)
{
   float4 rowWidth, rowHeight, width, height;
   float4 tx1, tx2;
   char8 c;

   if (!text || !size) return(0);

   rowWidth  = 0;
   rowHeight = (p_TexCoords[0][3]-p_TexCoords[0][1])*p_TexHeight;
   width     = 0;
   height    = rowHeight;

   while (*text)
   {
        c = *text++;
        if (c == '\n')
        {
            rowWidth = 0.0f;
            height  += rowHeight;
        }
        if (c < ' ') continue;
        tx1 = p_TexCoords[c-32][0];
        tx2 = p_TexCoords[c-32][2];
        rowWidth += (tx2-tx1)*p_TexWidth;
        if (rowWidth > width) width = rowWidth;
    }
    size->cx = (int32)width;
    size->cy = (int32)height;
    return(1);
}

//--------------------------------------------------------------------

void CFont::m_SetStates(void)
{
  p_RenderLib->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  p_RenderLib->p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  p_RenderLib->p_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  p_RenderLib->p_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  p_RenderLib->p_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  p_RenderLib->p_Device->SetVertexShader(FVF_FONT2DVERTEX);
  p_RenderLib->p_Device->SetStreamSource(0, p_VertexBuffer, sizeof(FONT2DVERTEX));
  p_RenderLib->p_Device->SetTexture(0, p_Texture->p_HWSurfaces[0]);
  p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
  p_RenderLib->p_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  p_RenderLib->p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  p_RenderLib->p_Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
}

//-----------------------------------------------------------------------------

int32 CFont::m_DrawAnimatedText(float4 x, float4 y,
                                float4 xScale, float4 yScale,
						        float4 end_xScale, float4 end_yScale,
							    uint32 color,
                                char8 *text, int32 start_char, int32 end_char,
								AD_Matrix *rot)
{
    int32 l;
	char8 c;
	float4 tx1, ty1, tx2, ty2, h, w, sx, sy, sz, rhw, fStartX;
	float4 wrot, hrot;
    D3DVIEWPORT8 vp;
    FONT2DVERTEX *pVertices;
    int32 dwNumTriangles = 0L;

    #define ROT_2DPOINT(x, y) \
	  wrot=rot->a[0][0]*x+rot->a[0][1]*y; \
	  hrot=rot->a[1][0]*x+rot->a[1][1]*y; \

	if ((start_char<0) || (end_char<0)) return(0);
	if (end_char<start_char) return(0);
	l=strlen(text);
	if ((start_char>=l) || (end_char >= l)) return(0);

    p_RenderLib->p_Device->GetViewport(&vp);
    sx  = (x+1.0f)*vp.Width/2;
    sy  = (y+1.0f)*vp.Height/2;
    sz  = 0.9f;
    rhw = 1.0f;
    fStartX = sx;

    float4 fLineHeight = (p_TexCoords[0][3] - p_TexCoords[0][1] )*p_TexHeight;
           fLineHeight=1.0f/fLineHeight;

    // Fill vertex buffer
    p_VertexBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);

	l=0;
    while ((*text) && (l<=end_char))
    {
        c = *text++;
        tx1 = p_TexCoords[c-32][0];
        ty1 = p_TexCoords[c-32][1];
        tx2 = p_TexCoords[c-32][2];
        ty2 = p_TexCoords[c-32][3];
        w = (tx2-tx1)*p_TexWidth;
        h = (ty2-ty1)*p_TexHeight;

		if (l<start_char)
		{
           w *= (end_xScale*vp.Width)*fLineHeight;
           h *= (end_yScale*vp.Height)*fLineHeight;
           w=w*0.5f;
		   goto next_letter;
		}
		else
		{
           w *= (xScale*vp.Width)*fLineHeight;
           h *= (yScale*vp.Height)*fLineHeight;
           w=w*0.5f;
		}

        if (c == '\n')
        {
            sx  = fStartX;
            sy += yScale*vp.Height;
        }
        if (c < ' ') goto next_letter;

        if (c != ' ')
        {
           h=h*0.5f;

		   ROT_2DPOINT(-w, h)
           *pVertices++ = InitFont2DVertex(sx+w+wrot-0.5f, sy+h+hrot-0.5f, sz, rhw, color, tx1, ty2);
		   ROT_2DPOINT(-w, -h)
           *pVertices++ = InitFont2DVertex(sx+w+wrot-0.5f, sy+h+hrot-0.5f, sz, rhw, color, tx1, ty1);
		   ROT_2DPOINT(w, h)
           *pVertices++ = InitFont2DVertex(sx+w+wrot-0.5f, sy+h+hrot-0.5f, sz, rhw, color, tx2, ty2);

		   ROT_2DPOINT(w, -h)
           *pVertices++ = InitFont2DVertex(sx+w+wrot-0.5f, sy+h+hrot-0.5f, sz, rhw, color, tx2, ty1);
		   ROT_2DPOINT(w, h)
           *pVertices++ = InitFont2DVertex(sx+w+wrot-0.5f, sy+h+hrot-0.5f, sz, rhw, color, tx2, ty2);
		   ROT_2DPOINT(-w, -h)
           *pVertices++ = InitFont2DVertex(sx+w+wrot-0.5f, sy+h+hrot-0.5f, sz, rhw, color, tx1, ty1);

           dwNumTriangles += 2;
           if (dwNumTriangles*3 > (MAX_NUM_VERTICES-6))
           {
              // Unlock, render, and relock the vertex buffer
              p_VertexBuffer->Unlock();
              p_RenderLib->p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);
              p_VertexBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);
              dwNumTriangles = 0;
           }
        }
next_letter:
;
        sx=sx+2*w;
		l++;
    }

    // Unlock and render the vertex buffer
    p_VertexBuffer->Unlock();
    if (dwNumTriangles > 0)
        p_RenderLib->p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);
    return(1);
}




//-----------------------------------------------------------------------------
// Name: DrawText()
// Desc: Draws 2D text
//-----------------------------------------------------------------------------
int32 CFont::m_DrawText(float4 sx, float4 sy, uint32 color,
                        char8 *text, int32 start_char, int32 end_char,
						float4 xScale, float4 yScale)
{
    int32 l;
	char8 c;
	float4 tx1, ty1, tx2, ty2, h, w, sz, rhw, fStartX;
    D3DVIEWPORT8 vp;
    FONT2DVERTEX *pVertices = NULL;
    int32 dwNumTriangles = 0;

	if ((start_char<0) || (end_char<0)) return(0);
	if (end_char<start_char) return(0);
	l=strlen(text);
	if (start_char>=l) return(0);
	if (end_char >= l) return(0);

    m_SetStates();

    p_RenderLib->p_Device->GetViewport(&vp);
    sx  = (sx+1.0f)*vp.Width/2;
    sy  = (sy+1.0f)*vp.Height/2;
    sz  = 0.9f;
    rhw = 1.0f;
    fStartX = sx;

    float4 fLineHeight = (p_TexCoords[0][3] - p_TexCoords[0][1] )*p_TexHeight;

    // Fill vertex buffer
    p_VertexBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);

	l=0;
    while ((*text) && (l<=end_char))
    {
        c = *text++;
        tx1 = p_TexCoords[c-32][0];
        ty1 = p_TexCoords[c-32][1];
        tx2 = p_TexCoords[c-32][2];
        ty2 = p_TexCoords[c-32][3];
        w = (tx2-tx1)*p_TexWidth;
        h = (ty2-ty1)*p_TexHeight;
        w *= (xScale*vp.Width)/fLineHeight;
        h *= (yScale*vp.Height)/fLineHeight;
		
		if (l<start_char) goto next_letter;
        if (c < ' ') goto next_letter;
		else
        if (c =='\n')
        {
            sx = fStartX;
            sy += (p_TexCoords[0][3]-p_TexCoords[0][1])*p_TexHeight;
        }
		else
        if (c != ' ')
        {
            *pVertices++ = InitFont2DVertex(sx+0-0.5f, sy+h-0.5f, sz, rhw, color, tx1, ty2);
            *pVertices++ = InitFont2DVertex(sx+0-0.5f, sy+0-0.5f, sz, rhw, color, tx1, ty1);
            *pVertices++ = InitFont2DVertex(sx+w-0.5f, sy+h-0.5f, sz, rhw, color, tx2, ty2);
            *pVertices++ = InitFont2DVertex(sx+w-0.5f, sy+0-0.5f, sz, rhw, color, tx2, ty1);
            *pVertices++ = InitFont2DVertex(sx+w-0.5f, sy+h-0.5f, sz, rhw, color, tx2, ty2);
            *pVertices++ = InitFont2DVertex(sx+0-0.5f, sy+0-0.5f, sz, rhw, color, tx1, ty1);
            dwNumTriangles += 2;

            if (dwNumTriangles*3 > (MAX_NUM_VERTICES-6))
            {
                // Unlock, render, and relock the vertex buffer
                p_VertexBuffer->Unlock();
                p_RenderLib->p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);
                pVertices = NULL;
                p_VertexBuffer->Lock(0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD);
                dwNumTriangles = 0;
            }
        }
next_letter:
;
        sx += w;
		l++;
    }

    // Unlock and render the vertex buffer
    p_VertexBuffer->Unlock();
    if (dwNumTriangles > 0)
        p_RenderLib->p_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);
    p_RenderLib->p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    return(1);
}

//--------------------------------------------------------------------

int32 CFont::m_DrawPhrase (FONTPHRASE *phrase, float4 relpos)
{
   int32 i, l;
   float4 start, end;
   float4 animStartScale, animEndScale;  // range del keyframer di scaling
   float4 animStartRot, animEndRot;  // range del keyframer di rotazione
   uint32 alpha, r, color;
   float4 animScaleX, animScaleY, animPos;
   float4 end_animScaleX, end_animScaleY;
   AD_Vect3D scale3D;
   AD_Matrix rot, end_rot;

   mat_identity(&rot);
   mat_identity(&end_rot);

   l=strlen(phrase->text);
   end_animScaleX=phrase->scaleX;
   end_animScaleY=phrase->scaleY;
   if (p_ScaleTrack.p_NumKeys>0)
   {
      p_ScaleTrack.m_GetTimeRange(&animStartScale, &animEndScale);
	  p_ScaleTrack.m_GetData(animEndScale, &scale3D);
	  end_animScaleX*=scale3D.x;
	  end_animScaleY*=scale3D.y;
   }
   animScaleX=phrase->scaleX;
   animScaleY=phrase->scaleY;

   if (p_RotationTrack.p_NumKeys>0)
   {
      p_RotationTrack.m_GetTimeRange(&animStartRot, &animEndRot);
	  p_RotationTrack.m_GetData(animEndRot, &end_rot);
   }

   // in questo intervallo la frase è tutta visibile completa
   // di transizioni (colore, alpha, animazioni) terminate
   if ((relpos>=phrase->phraseFixedStart) &&
       (relpos<phrase->phraseFixedEnd))
      m_DrawAnimatedText(phrase->x, phrase->y,
                         end_animScaleX, end_animScaleX,
                         end_animScaleX, end_animScaleY,
			             phrase->color, phrase->text, 0, l-1, &end_rot);
   else
   if (relpos<phrase->phraseFixedStart)
   {
      for (i=0; i<l; i++)
	  {
         animScaleX=phrase->scaleX;
         animScaleY=phrase->scaleY;

		 // calcolo l'inizio e fine della lettera i-esima
         start=phrase->relstart+i*phrase->phraseDelayPerChar;
		 if (relpos<start) continue;
         end=start+phrase->phraseTimePerChar;
		 // scrivo la lettera a colore e trasformazioni piene
		 if (relpos>=end)
		 {
            m_DrawAnimatedText(phrase->x, phrase->y,
                               end_animScaleX, end_animScaleY,
                               end_animScaleX, end_animScaleY,
			                   phrase->color, phrase->text, i, i, &end_rot);
		 }
		 else
		 {
            r=(uint32)(255.0f*relPos(start, end, relpos));
			alpha=(phrase->color >> 24);
			alpha=((alpha*r)>>8) & 255;
			color=(phrase->color & 0x00FFFFFF) | (alpha << 24);
			// estrazione dell'animazione
			if (p_ScaleTrack.p_NumKeys>0)
			{
			  animPos=map_intervals(start, end, animStartScale, animEndScale, relpos);
		      p_ScaleTrack.m_GetData(animPos, &scale3D);
		      animScaleX*=scale3D.x;
			  animScaleY*=scale3D.y;
			}
            if (p_RotationTrack.p_NumKeys>0)
			{
			   animPos=map_intervals(start, end, animStartRot, animEndRot, relpos);
	           p_RotationTrack.m_GetData(animPos, &rot);
			}
            m_DrawAnimatedText(phrase->x, phrase->y,
                               animScaleX, animScaleY,
                               end_animScaleX, end_animScaleY,
				               color, phrase->text, i, i, &rot);
		 }
	  }
   }
   else
   // fase di scomparsa della frase
   if (relpos>phrase->phraseFixedEnd)
   {
      for (i=0; i<l; i++)
	  {
         animScaleX=phrase->scaleX;
         animScaleY=phrase->scaleY;

		 // calcolo l'inizio e fine della lettera i-esima
         start=i*phrase->phraseDelayPerChar+phrase->phraseFixedEnd;
		 if (relpos<start)
            m_DrawAnimatedText(phrase->x, phrase->y,
                               end_animScaleX, end_animScaleY,
                               end_animScaleX, end_animScaleY,
			                   phrase->color, phrase->text, i, i, &end_rot);
		 else
		 {
            end=start+phrase->phraseTimePerChar;
		    // scrivo la lettera a colore pieno
		    if (relpos<end)
			{
			  alpha=(phrase->color >> 24) & 255;
              r=(uint32)(255.0f*relPos(start, end, relpos));
			  alpha=alpha-(((alpha*r)>>8) & 255);
			  color=(phrase->color & 0x00FFFFFF) | (alpha << 24);
			  // estrazione dell'animazione
			  if (p_ScaleTrack.p_NumKeys>0)
			  {
			    animPos=map_intervals(start, end, animStartScale, animEndScale, relpos);
				animPos=animEndScale-animPos;
		        p_ScaleTrack.m_GetData(animPos, &scale3D);
		        animScaleX*=scale3D.x;
				animScaleY*=scale3D.y;
			  }
              if (p_RotationTrack.p_NumKeys>0)
			  {
			    animPos=map_intervals(start, end, animStartRot, animEndRot, relpos);
				animPos=animEndRot-animPos;
	            p_RotationTrack.m_GetData(animPos, &rot);
			  }

              m_DrawAnimatedText(phrase->x, phrase->y,
                                 animScaleX, animScaleY,
                                 end_animScaleX, end_animScaleY,
				                 color, phrase->text, i, i, &rot);
			}
		 }
	  }
   }
   else
   {
      for (i=0; i<l; i++)
	  {
         m_DrawAnimatedText(phrase->x, phrase->y,
                            end_animScaleX, end_animScaleY,
                            end_animScaleX, end_animScaleY,
			                phrase->color, phrase->text, i, i, &end_rot);
	  }
   }
   return(1);
}

//--------------------------------------------------------------------

void CFont::m_DoEfx2D(double pos)
{
   float4 relpos;
   int32 i;

   m_SetStates();
   //DumpDebugPoint (fdeb, "CFDont Doefx setStages done", 0);

   relpos=(float4)m_GetRelPos(pos);
   for (i=0; i<p_NumPhrases; i++)
   {
      if ((relpos>=p_Phrases[i].relstart) &&
          (relpos<p_Phrases[i].relend))
	   m_DrawPhrase (&p_Phrases[i], relpos);
   }
   p_RenderLib->p_Device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
   //DumpDebugPoint (fdeb, "CFDont Doefx exititing done", 0);
}