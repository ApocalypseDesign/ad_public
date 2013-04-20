// RotoZoomer - by HereBit
// descrizione:  versione non-MMX

#ifndef _MYEFX_H_
#define _MYEFX_H_

#define MYEFX_NAME "RotoZoomer"
#define MYEFX_DESC "RotoZoomer, versione non-MMX"

#include <string.h>
#include "efx2D.h"
#include "ADcontrol.h"
#include "image.h"

class myefx : public efx2D
{
protected:
	image *timageInOut,*ttexture;
	ctFloat *trot,*tzoomx,*tzoomy;
	ctVector2D *tpos;
	unsigned int xmask,ymask;

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