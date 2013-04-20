//  _____________________________________________________ _ _ _ 
// |_Qwav_2d_Effect______________________________________|-|@|x|
// | ~~~~ ~~ ~~~~~~                                       ~ ~|~| 
// |                                                         |_|
// | Main code : Ska                                         |#|
// | Version   : 0.01                                        |#|
// | Date of creation : 15/12/1998                           |#|
// | Last modify      : 20/10/1999                           |~|
// |_________________________________________________________|_|


#ifndef _QWAVE2D_H_
#define _QWAVE2D_H_

#include "..\efx2D.h"
#include "..\ADrender.h"

class qwave2D : public efx2D
{
private:

 int *kpls1, *kpls2;
 int *pls1,  *pls2;
 int *mod_y;

 double *tmptab;

 int k1,k2;
 unsigned char *back;
 unsigned char *xback;
 

 public:

 texture mytex;

 void init();
 void set_background(unsigned char *_back);
 void set_background(char *nomefile);
 void set_background(imagebuffer *img);
 void do_efx2D(double pos);
};

#endif