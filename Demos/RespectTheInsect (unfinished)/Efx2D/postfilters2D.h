#ifndef _POSTFILTERS2D_H_
#define _POSTFILTERS2D_H_
// ********************************************************
// Realizzano dei filtri in post produzione su una immagine
// ********************************************************
#include "..\efx2D.h"

class rainbow2D : public efx2D
{
  private:

  public:
	rainbow2D();       // costruttore
	void init();
	void do_efx2D(double pos);
};


class rubbing2D : public efx2D
{
  private:

  public:
	imagebuffer *imageout;
	rubbing2D();       // costruttore
	void set_imageout(imagebuffer *o);
	void init();
	void do_efx2D(double pos);
};


class mosaic2D : public efx2D
{
  private:

  public:
	mosaic2D();       // costruttore
	void init();
	void do_efx2D(double pos);
};


class sniper2D : public efx2D
{
  private:

  public:
	sniper2D();       // costruttore
	void init();
	void do_efx2D(double pos);
};

#endif