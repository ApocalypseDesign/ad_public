// descrizione: effettua un fade in nero

#ifndef _FADE2D_H_
#define _FADE2D_H_

#include "..\efx2D.h"

class fade2D : public efx2D
{
public:
 material fmat;

 void init();
 void do_efx2D(double pos);
};

#endif