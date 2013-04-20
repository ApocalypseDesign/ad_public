#ifndef _ZBUF2D_H_
#define _ZBUF2D_H_

#include "..\efx2D.h"

class zbuf2D : public efx2D
{
public:
	floatbuffer *wbuffer;

 void init();
 void do_efx2D(double pos);
};

#endif