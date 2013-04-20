// ADefx
// descrizione: progetto di base per creare nuovi efx

#ifndef _MYEFX_H_
#define _MYEFX_H_

#define MYEFX_NAME "Sprite"
#define MYEFX_DESC "sprite Efx"

#include <string.h>
#include "efx2D.h"
#include "ADcontrol.h"
#include "image.h"

class myefx : public efx2D
{
protected:


public:
    myefx() { strcpy(className,MYEFX_NAME); }
	void *create() { return new myefx; }
	void defineControllers(tChannel *defaultChannel);
	void updateControllers();
	void init();
	void free();
	void paint(double pos);
};

#endif