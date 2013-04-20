// FlashSUB - by HereBit
// descrizione: effettua un flash sottrattivo, versione non-MMX

#ifndef _MYEFX_H_
#define _MYEFX_H_

#define MYEFX_NAME "FlashSUB"
#define MYEFX_DESC "flash sottrattivo, versione non-MMX"

#include <string.h>
#include "efx2D.h"
#include "ADcontrol.h"
#include "image.h"

class myefx : public efx2D
{
protected:
	image *timageInOut;
	ctFloat *inteR,*inteG,*inteB;

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