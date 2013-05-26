#ifndef _BUMP2D_H_
#define _BUMP2D_H_

#include <math.h>
#include "ADrender.h"
#include "..\efx2D.h"

class bump2D : public efx2D
{
  private:
 
  public:
    texture height_map;
    texture light_map;

	bump2D();       // costruttore
	void set_height_map(char *file);
    void set_height_map(texture *t);
	void set_height_map(imagebuffer *ib);
	void set_light_map(char *file);
	void init();
	void plot(int lightX, int lightY);
	void do_efx2D(double pos);
};

#endif