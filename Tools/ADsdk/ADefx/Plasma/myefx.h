// Plasma - by Ska & Turbo & HereBit
// descrizione: plasma, versione non-MMX

#ifndef _MYEFX_H_
#define _MYEFX_H_

#define MYEFX_NAME "Plasma"
#define MYEFX_DESC "Plasma, versione non-MMX"

#include <string.h>
#include "efx2D.h"
#include "ADcontrol.h"
#include "image.h"

class myefx : public efx2D
{
protected:
	image *timageInOut;
	ctInt *Controller_k1;
	ctInt *Controller_k2;

private:
	unsigned char *plasmpal;
    int *rgb;
	int *kpls1, *kpls2, *kpls3;
	int *pls1, *pls2, *pls3;
	double *tmptab;
	int i, x, y;
	int y2, x2, y4, x4;
	int k, k1, k2, k3, k4, k5, k6;
	int s1, s2, s3;
	int d1, d2, d3;
	double g;
	double max, max1, max2;
	double min, min1, min2;

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