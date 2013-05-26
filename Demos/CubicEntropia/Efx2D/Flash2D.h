#ifndef _FLASH2D_H_
#define _FLASH2D_H_

#include "Efx2D.h"

class CFlash2D : public CEfx2D
{
private:
    void m_SetStages(void);
	LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer;

public:
 CFlash2D();
 int32 m_Init();
 void m_DoEfx2D(double pos);
};

#endif