// classe di base per gli effetti 2D v3.0
// by HereBit
// ultima modifica: 2 marzo 99

#include "efx2D.h"

void efx2D::set_imagebuffer(imagebuffer *imgbuf)
{
  image=imgbuf;
}

void efx2D::set_pos(double startpos, double endpos)
{
  fstartpos=startpos;
  if (fstartpos<0.0) fstartpos=0.0;
  fendpos=endpos;
  if (fendpos>1.0) fendpos=1.0;
}
   
void efx2D::set_channels(int rchan, int gchan, int bchan)
{
  frchan=rchan;
  fgchan=gchan;
  fbchan=bchan;
}

