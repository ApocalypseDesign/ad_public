#ifndef _HEREBBIT2D_H_
#define _HEREBBIT2D_H_

#include "..\efx2D.h"

class herebbit2D : public efx2D
{
public:
 material fmat;

 void init();
 void do_efx2D(double pos);
};

#endif