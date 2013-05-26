#ifndef _FONT_H_
#define _FONT_H_

#include <D3D8.h>
#include "Efx2D.h"

// Font creation flags
#define FONT_BOLD        0x0001
#define FONT_ITALIC      0x0002
#define FONT_ZENABLE     0x0004

// Font rendering flags
#define FONT_CENTERED    0x0001
#define FONT_TWOSIDED    0x0002
#define FONT_FILTERED    0x0004

#define MAX_PHRASES 40

typedef struct
{
   float4 relstart, relend;
   float4 relfixed, reldelay;
   float4 x, y;
   float4 scaleX, scaleY;
   uint32 color;
   char8 text[60];
   // valori precalcolati a init time
   float4 phraseFixedStart;
   float4 phraseFixedEnd;
   float4 phraseTimePerChar;
   float4 phraseDelayPerChar;
} FONTPHRASE, *FONTPHRASE_PTR;


class CFont: public CEfx2D
{
    // Font properties
    char8   p_FontName[80];
    int32   p_FontHeight;
    int32   p_FontFlags;

	// frasi e numero di frasi
	FONTPHRASE p_Phrases[MAX_PHRASES];
	int32   p_NumPhrases;

    LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer;        // VertexBuffer for rendering text
    CGraphicSurface *p_Texture;
	// Texture dimensions
    int32   p_TexWidth, p_TexHeight;
    float4  p_TextScale;
    float4  p_TexCoords[128-32][4];

    CPosTrack p_ScaleTrack;
    CRotTrack p_RotationTrack;

    int32 m_GetTextExtent(char8* text, SIZE *size);
	int32 m_InitMaterial(void);
	// setta i texture stage e i render stage per il disegno
	void m_SetStates(void);	
	// queste funzioni sono inaccessibili pubblicamente, perchè
	// va usata la AddPhrase e il DoEfx se si vogliono frasi
	// animate
    int32 m_DrawAnimatedText(float4 x, float4 y, 
                             float4 xScale, float4 yScale,
							 float4 end_xScale, float4 end_yScale,
						     uint32 color, 
                             char8 *text, int32 start_char, int32 end_char,
							 AD_Matrix *rot);
	int32 m_DrawPhrase (FONTPHRASE *phrase, float4 relpos);


public:
    CFont(char8 *fontName, int32 height, int32 flags=0L);
	int32 m_LoadAnimation (char8 *A3DFileName, char8 *objectName);
	int32 m_Init(void);
	int32 m_AddPhrase(float4 relstart, float4 relend,
		              float4 relfixed, float4 reldelay,
                      char8 *text,
		              float4 x, float4 y, uint32 color,
		              float4 scaleX=1, float4 scaleY=1);

    int32 m_DrawText(float4 x, float4 y, uint32 color, 
                     char8 *text, int32 start_char, int32 end_char,
					 float4 xScale=1, float4 yScale=1);

    void m_DoEfx2D(double pos);
};


#endif