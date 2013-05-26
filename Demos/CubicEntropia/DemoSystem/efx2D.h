#ifndef _EFX2D_H_
#define _EFX2D_H_

#include "RenderLib.h"
#include "Utility.h"

class CEfx2D
{
public:
   double fstartpos, fendpos;
   CGraphicSurface *p_WorkBuffer;
   CRenderLib *p_RenderLib;

   CEfx2D();
   void m_SetWorkBuffer(CGraphicSurface *imgbuf);
   void m_SetRenderLib(CRenderLib *lib);
   void m_SetPos(double startpos, double endpos);
   double m_GetRelPos(double pos);

   int32 m_Init();
   virtual void m_DoEfx2D(double pos) {} // da overloadare
   inline int32 CEfx2D::m_Apply(double pos)
   {
     if ((pos>=fstartpos) && (pos<=fendpos)) return 1;
        else return 0;
   }
};

#endif