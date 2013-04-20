// FLASH2D EFFECT v2000.2
// FlashADD - by HereBit
// descrizione: effettua un flash additivo, versione non-MMX

#ifndef _MYEFX_H_
#define _MYEFX_H_

#define MYEFX_NAME "Media"
#define MYEFX_DESC "Media tra due canali, versione non-MMX"

#include <string.h>
#include "efx2D.h"
#include "ADcontrol.h"
#include "image.h"

class myefx : public efx2D
{
protected:
	image *timageOut,*timageIn1,*timageIn2;

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