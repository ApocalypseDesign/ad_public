//  _____________________________________________________ _ _ _ 
// |_Plas_2d_Effect______________________________________|-|@|x|
// | ~~~~ ~~ ~~~~~~                                       ~ ~|~| 
// |                                                         |_|
// | Main code : Ska                                         |#|
// | Version   : 0.01                                        |#|
// | Date of creation : 15/12/1998                           |#|
// | Last modify      : 15/12/1998                           |~|
// |_________________________________________________________|_|


#ifndef _PLASM2D_H_
#define _PLASM2D_H_

#include <math.h>
#include "..\efx2D.h"

class plasm2D : public efx2D
{
  private:
	int *kpls1, *kpls2;
	int *pls1, *pls2;
    int k, k1, k2;

  public:

	void init();
	void do_efx2D(double pos);
};

#endif