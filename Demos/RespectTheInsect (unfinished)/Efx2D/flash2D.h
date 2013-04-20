#ifndef _FLASH2D_H_
#define _FLASH2D_H_

#include "..\efx2D.h"

class flash2D : public efx2D
{
public:
 material fmat;

 void init();
 void do_efx2D(double pos);
};

#endif