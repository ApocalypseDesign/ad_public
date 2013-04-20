//  _____________________________________________________ _ _ _ 
// |_Water_2d_Effect_____________________________________|-|@|x|
// | ~~~~~ ~~ ~~~~~~                                      ~ ~|~| 
// |                                                         |_|
// | Main code : Ska                                         |#|
// | Version   : 0.01                                        |#|
// | Date of creation : 15/12/1998                           |#|
// | Last modify      : 20/10/1999                           |~|
// |_________________________________________________________|_|


#ifndef _WATER2D_H_
#define _WATER2D_H_

#include "..\efx2D.h"

#define NOBLUR 0
#define BLUR   1

class water2D : public efx2D
{
private:
 short int *w_new, *w_old;     // used 
 int r_blur, g_blur, b_blur;   // value range (0-8)
 int rm, gm, bm;               // r,g,b mul
 int blittype;

 void goccia(int x ,int y);
 void plot(short int *w_new, unsigned char *back);
 void dowater(short *w_old, short int *w_new); 


 public:

 unsigned char *back;          // background 
 texture mytex;
 water2D();   // costruttore
 void init();
 void set_blur(int r, int g,int b);
 void set_background(unsigned char *_back);
 void set_background(char *nomefile);
 void do_efx2D(double pos);
};

#endif