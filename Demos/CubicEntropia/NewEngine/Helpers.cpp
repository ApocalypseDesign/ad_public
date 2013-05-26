#include <windows.h>
#include "Helpers.h"

extern HWND miawin;

CHelper::CHelper()
{
   p_Width=0;
   p_Height=0;
   p_Lenght=0;
}

//--------------------------------------------------------------------

int32 CHelper::m_Init(void *arg1)
{
   if (p_Width <= 0) return(0);
   if (p_Height <= 0) return(0);
   if (p_Lenght <= 0) return(0);
   return(((CGeometricObject *)(this))->m_Init(arg1));
}

//--------------------------------------------------------------------

void CHelper::m_Update(float4 frame)
{
   ((CGeometricObject *)(this))->m_Update(frame);
}

//--------------------------------------------------------------------

void CHelper::m_Paint(void *arg1, void *arg2, void *arg3)
{
   return;
}

//--------------------------------------------------------------------