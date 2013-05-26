#ifndef _SPRITE2D_H_
#define _SPRITE2D_H_

#include "Efx2D.h"

#define BLEND_ADD       1
#define BLEND_MODULATE  2
#define BLEND_NONE      4

#define MAXKEY 10

typedef struct
{
  float4 relpos;
  float4 x, y, scale_x, scale_y;
} SpriteKey;

class CSprite2D : public CEfx2D
{
private:
    void m_SetStages(void);
	CGraphicSurface *p_Texture;
	LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer;
	int32 p_BlendType;
	float4 p_Envelop1, p_Envelop2;

    SpriteKey p_Keys[MAXKEY];
	int32 p_NumKeys;

public:
 CSprite2D();
 int32 m_SetParameters (char8 *filename, int32 blendType,
	                    float4 envelop1, float4 envelop2);
 void m_AddKey(float4 relpos, float4 x, float4 y, float4 scale_x, float4 scale_y);
   
 int32 m_Init();
 void m_DoEfx2D(double pos);
};

#endif