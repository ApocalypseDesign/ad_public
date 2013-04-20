#ifndef _HZBLUR2D_H_
#define _HZBLUR2D_H_

// * SPANDIMERDA MODE ON
// modestamente in HereBit's style ;)
// * SPANDIMERDA MODE OFF

#include "..\efx2D.h"

#define MAX_HZBLUR_WINDOW 128
// 64 corrisponde a un valore max di window = 63

class hzblur2D : public efx2D
{
public:
	int val[MAX_HZBLUR_WINDOW*2+1][3];
	int window; // questo è il parametro da variare a runtime per
	            // settare l'intensità del blur (1 .. 63)

	hzblur2D() 
	{ window=2; }
	void init();
	void do_efx2D(double pos);
};

#endif