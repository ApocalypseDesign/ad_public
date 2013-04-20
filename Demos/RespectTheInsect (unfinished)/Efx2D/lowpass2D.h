#ifndef _LOWPASS2D_H_
#define _LOWPASS2D_H_

// * SPANDIMERDA MODE ON
// L'unico vero filtro LOWPASS in tempo reale mai realizzato
// modestamente in HereBit's style ;)
// naturalmente è leggermente approssimato, ma NON è un fake
// * SPANDIMERDA MODE OFF

#include "..\efx2D.h"

#define MAX_LOWPASS_WINDOW 32 
// 32 corrisponde a un valore max di window = 15
// è assurdo andare oltre perchè già a 15 è lentissimo

class lowpass2D : public efx2D
{
public:
	int val[MAX_LOWPASS_WINDOW*2+1][3];
	int window; // questo è il parametro da variare a runtime per
	            // settare l'intensità del blur (1 .. 15)

	lowpass2D() 
	{ window=2; }
	void init();
	void do_efx2D(double pos);
};

#endif