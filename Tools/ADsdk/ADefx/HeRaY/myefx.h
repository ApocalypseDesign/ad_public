// FLASH2D EFFECT v2000.2
// HeRaY - by HereBit
// descrizione: effettua un flash additivo, versione non-MMX

#ifndef _MYEFX_H_
#define _MYEFX_H_

#define MYEFX_NAME "HeRaY"
#define MYEFX_DESC "Realtime Raytracing Efx by HereBit"

#include <string.h>
#include "efx2D.h"
#include "ADcontrol.h"
#include "image.h"
#include "rt.h"

class myefx : public efx2D
{
protected:
	image *tOut;
	Universe univ;
	Ray myRay;

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