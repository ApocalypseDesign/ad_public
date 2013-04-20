#ifndef _BLOB2D_H_
#define _BLOB2D_H_

#define nblob 16
#include "..\efx2D.h"

typedef struct Tblob
{
   int x, y;
   int bx, by;
   float va;
   int raggio;
   int pow;
   int *xabs;
   int *yabs;
} Tblob;


class blob2D : public efx2D
{
 private:

 unsigned char *tex;
 unsigned int  *texuint;
 unsigned char *deform_tab;
 int *distance_tab;
 Tblob blob[nblob];

 public:

 void init();
 void agg_cord(double pos);
 void do_efx2D(double pos);
};

#endif