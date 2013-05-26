#include "flash2D.h"
#include "..\utility.h"
#include <math.h>

extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

void flash2D::init()
{ 
} 

void flash2D::do_efx2D(double pos)
{
  int
	   intensity=(unsigned char) 
                   (sin( (6.2832*get_interpos(pos)) + 4.7124 )*127 +127);

  int inte_r=intensity<<16,
	  inte_g=intensity<<8,
	  inte_b=intensity;

  int packed_intensity = (intensity << 16) | (intensity << 8) |
	                     intensity;

  unsigned int pippo;
  pippo=(unsigned int)image->uint32ptr;
  int max=image->channelsize;

  _asm   // ci voleva mmx ? e Turbo l'ha messa di dio, as usual
	     // perchè ? perchè Turbo roxa e voi SUXATE come pochiiiii
  {
    mov eax, [packed_intensity]
	movd mm0, eax
	punpckldq mm0, mm0
	mov ecx, [max]
	shr ecx, 1
	mov edi, [pippo]     // notare il nome molto significativo
	                     // usato da Here(sux)

ciclo:

    movq mm1, [edi]
	paddusb mm1, mm0
	movq [edi], mm1

	add edi, 8
	dec ecx
	jnz ciclo
	emms
  }
}