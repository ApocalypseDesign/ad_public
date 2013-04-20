// classe di base per gli effetti 2D v3.0
// by HereBit
// ultima modifica: 2 marzo 99

#ifndef _EFX2D_H_
#define _EFX2D_H_

#include "ADrender.h"

class efx2D
{
private:
   
protected:
   
public:

   double fstartpos, fendpos;
   int frchan, fgchan, fbchan;
   imagebuffer *image;

   efx2D(void) //Constructor
   { 
    fstartpos=0.0;
    fendpos=1.0;
    frchan=fgchan=fbchan=1;
	image=(imagebuffer *)NULL;
   }

   void set_imagebuffer(imagebuffer *imgbuf);
   void set_pos(double startpos, double endpos);
   void set_channels(int rchan, int gchan, int bchan);
  
   virtual void init() { } // da overloadare

   virtual void do_efx2D(double pos) { } // da overloadare

   inline int efx2D::apply(double pos)
   {
     if ((pos>=fstartpos) && (pos<=fendpos)) return 1;
        else return 0;
   }

   inline double efx2D::get_interpos(double pos)
   { 
     double mypos=pos;
     double divis; 

     if (mypos<0.0) mypos=0.0;
        else if (mypos>1.0) mypos=1.0;
  
     divis=fendpos-fstartpos;
     if (divis==0.0) divis=0.0000001;
     return (mypos-fstartpos)/divis;
   }

   
   //virtual ~efx2D(void) { } //Destructor
};

#endif