// FADE2D EFFECT v2.2
// by Turbo
// ultima modifica: 9-12-199
// descrizione: effettua un fade in nero

#include "fade2D.h"
#include "..\utility.h"
#include <math.h>

extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

void fade2D::init()
{ 
} 

void fade2D::do_efx2D(double pos)
{

  unsigned int pippo;
  pippo=(unsigned int)image->uint32ptr;
  int max=image->channelsize;
  float relpos;
  unsigned int intens;

  relpos=(float)((pos-fstartpos)/(fendpos-fstartpos));
  intens=(unsigned int)(relpos*255);

  _asm   // ci voleva mmx ? e Turbo l'ha messa di dio, as usual
	     // perchè ? perchè Turbo roxa e voi SUXATE come pochiiiii
  {
	mov ecx, [max]
	shr ecx, 1
	mov edi, [pippo]     // notare il nome molto significativo
	                     // usato da Here(sux)
    movd mm7, [intens]
	punpcklbw mm7, mm7
	punpcklwd mm7, mm7
	punpckldq mm7, mm7

ciclo:

    movq mm1, [edi]
	psubusb mm1, mm7
	movq [edi], mm1

	add edi, 8
	dec ecx
	jnz ciclo
	emms
  }
}