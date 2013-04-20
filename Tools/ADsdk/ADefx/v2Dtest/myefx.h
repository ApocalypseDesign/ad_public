// test EFFECT v2000
// - by HereBit
// descrizione: test di Vector2D

#ifndef _MYEFX_H_
#define _MYEFX_H_

#define MYEFX_NAME "v2Dtest"
#define MYEFX_DESC "test Vector2D"

#include <string.h>
#include "efx2D.h"
#include "ADcontrol.h"
#include "image.h"

class myefx : public efx2D
{
protected:
	image *timageInOut;
	ctVector2D *miovect;

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