//  _____________________________________________________ _ _ _ 
// |_Hole_2d_Effect______________________________________|-|@|x|
// | ~~~~ ~~ ~~~~~~                                       ~ ~|~| 
// |                                                         |_|
// | Main code : Ska                                         |#|
// | Version   : 0.01                                        |#|
// | Date of creation : 15/12/1998                           |#|
// | Last modify      : 20/10/1999                           |~|
// |_________________________________________________________|_|


#ifndef _HOLE2D_H_
#define _HOLE2D_H_

#include "..\efx2D.h"

#define NOBLUR 0
#define BLUR   1

class hole2D : public efx2D
{
private:
 texture mytex;
 unsigned char *textdata, *textcoord; 
 int k1,k2,k3,k4;            // uv variance  - uv control
 
 int r_blur, g_blur, b_blur; // value range (0-8)
 int rm, gm, bm;             // r,g,b mul
 int blittype;

 float hole2D::x_centre(float f );
 float hole2D::y_centre(float f );
 float hole2D::radius(float f );
 void  hole2D::plot(int du, int dv );

 public:

 hole2D();   // costruttore
 void init();
 void set_texture(unsigned char *_textdata);
 void set_texture(char *nomefile);
 void set_uvcontrol(int _k1, int _k2);
 void set_blur(int r, int g,int b);
 void do_efx2D(double pos);

};

#endif