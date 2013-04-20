#ifndef _HIBOOST2D_H_
#define _HIBOOST2D_H_

// * SPANDIMERDA MODE ON
// L'unico vero filtro HIBOOST in tempo reale mai realizzato
// (per intenderci il "maschera di contrasto" di photoshop)
// modestamente in HereBit's style ;)
// NON è un fake
// * SPANDIMERDA MODE OFF

#include "..\efx2D.h"

#define MAX_HIBOOST_WINDOW 32 
// 32 corrisponde a un valore max di window = 15
// è assurdo andare oltre perchè già a 15 è lentissimo

class hiboost2D : public efx2D
{
public:
	int val[MAX_HIBOOST_WINDOW*2+1][3];
	int window; // questo è il parametro da variare a runtime per
	            // settare la dimensione della finestra (1 .. 15)
	int intensity; // questo è il parametro da variare a runtime per
			   	   // settare l'intensità dell'effetto (da 1 in su)
	hiboost2D() 
	{ window=2; intensity=4; }
	void init();
	void do_efx2D(double pos);
};

#endif