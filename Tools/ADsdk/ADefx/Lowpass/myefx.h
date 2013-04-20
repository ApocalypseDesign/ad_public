// * SPANDIMERDA MODE ON
// L'unico vero filtro LOWPASS in tempo reale mai realizzato
// modestamente in HereBit's style ;)
// naturalmente è leggermente approssimato, ma NON è un fake
// * SPANDIMERDA MODE OFF

#ifndef _MYEFX_H_
#define _MYEFX_H_

#define MYEFX_NAME "Lowpass"
#define MYEFX_DESC "Filtro passa-basso"

#define MAX_LOWPASS_WINDOW 32 
// 32 corrisponde a un valore max di window = 15
// è assurdo andare oltre perchè già a 15 è lentissimo

#include <string.h>
#include "efx2D.h"
#include "ADcontrol.h"
#include "image.h"

class myefx : public efx2D
{
protected:
	image *timageInOut;
	ctInt *windowSize;

public:
	int val[MAX_LOWPASS_WINDOW*2+1][3];

    myefx() { strcpy(className,MYEFX_NAME); }
	void *create() { return new myefx; }
	void defineControllers(tChannel *defaultChannel);
	void updateControllers();
	void init();
	void free();
	void paint(double pos);
};

#endif