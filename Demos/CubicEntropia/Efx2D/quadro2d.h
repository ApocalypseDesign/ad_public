#ifndef _QUADRO2D_H_
#define _QUADRO2D_H_

#include <math.h>
#include "..\efx2D.h"
#include "ADrender.h"

#define QUADRO_ORARIO 0
#define QUADRO_ANTIORARIO 1

class quadro2D : public efx2D
{
  private:
	void draw_quadro(float p, unsigned char luminosita);
	void draw_pic(float p);

  public:
    float xp[4], yp[4];
    float xp_tr[4], yp_tr[4];
	int ang0, step0, xf, yf, rotaz_verso;
	float rel_split;
	texture pic;

	quadro2D();
	void set_parameters(char *file_img, int xfinale, int yfinale,
		                int ang_iniziale, int step,
						float rel_split_efx, int orario_antiorario);
	void init();
	void do_efx2D(double pos);
};

#endif