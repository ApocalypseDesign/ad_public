#ifndef _HIPASS2D_H_
#define _HIPASS2D_H_

// * SPANDIMERDA MODE ON
// L'unico vero filtro HIPASS in tempo reale mai realizzato
// modestamente in HereBit's style ;)
// NON è un fake
// * SPANDIMERDA MODE OFF

#include "..\efx2D.h"

#define MAX_HIPASS_WINDOW 32 
// 32 corrisponde a un valore max di window = 15
// è assurdo andare oltre perchè già a 15 è lentissimo

class hipass2D : public efx2D
{
public:
	int val[MAX_HIPASS_WINDOW*2+1][3];
	int window; // questo è il parametro da variare a runtime per
	            // settare l'intensità (1 .. 15)

	hipass2D() 
	{ window=2; }
	void init();
	void do_efx2D(double pos);
};

#endif