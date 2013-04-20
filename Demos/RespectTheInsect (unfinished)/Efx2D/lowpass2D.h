#ifndef _LOWPASS2D_H_
#define _LOWPASS2D_H_

// * SPANDIMERDA MODE ON
// L'unico vero filtro LOWPASS in tempo reale mai realizzato
// modestamente in HereBit's style ;)
// naturalmente � leggermente approssimato, ma NON � un fake
// * SPANDIMERDA MODE OFF

#include "..\efx2D.h"

#define MAX_LOWPASS_WINDOW 32 
// 32 corrisponde a un valore max di window = 15
// � assurdo andare oltre perch� gi� a 15 � lentissimo

class lowpass2D : public efx2D
{
public:
	int val[MAX_LOWPASS_WINDOW*2+1][3];
	int window; // questo � il parametro da variare a runtime per
	            // settare l'intensit� del blur (1 .. 15)

	lowpass2D() 
	{ window=2; }
	void init();
	void do_efx2D(double pos);
};

#endif