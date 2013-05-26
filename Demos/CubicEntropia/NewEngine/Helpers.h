#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "GeometricObject.h"

// include il caso di Point, Dummy e BoxGizmo
class CHelper: public CGeometricObject
{
public:
   float4 p_Width, p_Height, p_Lenght;

   CHelper();
   int32 m_Init(void *arg1);
   void m_Update(float4 frame);
   void m_Paint(void *arg1, void *arg2, void *arg3);
};

#endif