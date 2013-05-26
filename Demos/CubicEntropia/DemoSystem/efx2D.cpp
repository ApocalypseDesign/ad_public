#include "Efx2D.h"

extern HWND miawin;

CEfx2D::CEfx2D()
{
  //fstartpos=0.0;
  //fendpos=1.0;
  fstartpos=-1.0;
  fendpos=-1.0;
  p_WorkBuffer=(CGraphicSurface *)NULL;
  p_RenderLib=(CRenderLib *)NULL;
}

//--------------------------------------------------------------------

int32 CEfx2D::m_Init()
{
  if (!p_RenderLib)
    debug_error(miawin, "CEfx2D::m_Init() error: render lib is NULL!!!");
  //if (!p_WorkBuffer)
    //debug_error(miawin, "CEfx2D::m_Init() error: work buffer is NULL!!!");
  return(1);
}

//--------------------------------------------------------------------

double CEfx2D::m_GetRelPos(double pos)
{
  if (pos<fstartpos) return(0);
  else
  if (pos>fendpos) return(1.0);
  else return ((pos-fstartpos)/(fendpos-fstartpos));
}

//--------------------------------------------------------------------

void CEfx2D::m_SetWorkBuffer(CGraphicSurface *imgbuf)
{
  p_WorkBuffer=imgbuf;
}

//--------------------------------------------------------------------

void CEfx2D::m_SetRenderLib(CRenderLib *lib)
{
  p_RenderLib=lib;
}

//--------------------------------------------------------------------

void CEfx2D::m_SetPos(double startpos, double endpos)
{
  fstartpos=startpos;
  if (fstartpos<0.0) fstartpos=0.0;
  fendpos=endpos;
  if (fendpos>1.0) fendpos=1.0;
}

//--------------------------------------------------------------------
