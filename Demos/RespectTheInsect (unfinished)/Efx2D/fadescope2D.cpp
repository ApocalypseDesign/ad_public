#include "fadescope2D.h"
#include "..\utility.h"
#include <math.h>

extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

void fadescope2D::init()
{ 
} 

void fadescope2D::do_efx2D(double pos)
{
  int delta_count;
  unsigned int pippo,pippo2;
  int max=image->width/2;
  int packed_intensity,packed_delta_intensity;

  if (intensity<=0) return;
  if (delta_intensity<0) delta_intensity=0;
  if (delta_intensity==0) delta_count=image->height/2;
	  else delta_count=abs(intensity/delta_intensity);
  if (delta_count>image->height/2) delta_count=image->height/2;
  if (delta_count<=0) return;

  packed_intensity = (intensity << 16) | (intensity << 8) | intensity;
  packed_delta_intensity = (delta_intensity << 16) | (delta_intensity << 8) |
								delta_intensity;

  pippo=(unsigned int)image->uint32ptr;
  pippo2=pippo+image->channelsize*4-delta_count*image->width*4;
  
if (mode==FADESCOPE2D_SUB)
  _asm   // ndHere: codice parzialmente rippato dal flash MMX di Torbo 
	     // a sua volta rippato dal flash asm normale mio ;)
	     // N.B. RIPPARE RULEZZZ
  {
    mov eax, [packed_intensity]
	movd mm0, eax
	punpckldq mm0, mm0

	mov eax, [packed_delta_intensity]
	movd mm2, eax
	punpckldq mm2, mm2

	mov ebx, [delta_count]
	mov ecx, [max]
	mov edi, [pippo]     // ndTurbo: notare il nome molto significativo
	                     // usato da Here(sux)

						 // ndHere: MA RULLAAA
						 // il coefficiente Pippo è di un design allucinante
						 // infatti + sotto ho aggiunto pippo2 ;P

// ***PARTE SUPERIORE***
ciclo: //inizio ciclo per scurire una riga orizzontale

    movq mm1, [edi]
	psubusb mm1, mm0
	movq [edi], mm1

	add edi, 8
	dec ecx
	jnz ciclo //fine ciclo linea orizzontale

	psubusb mm0, mm2 //decremento il sottrattore (intensity -= delta_intensity)

	mov ecx, [max] //ripristino la dimensione della linea

	dec ebx 
	test ebx, ebx
	jnz ciclo //salto se devo scurire un'altra linea

// ***PARTE INFERIORE***
    mov edi, [pippo2] 
	mov ebx, [delta_count]
ciclo2: //inizio ciclo per scurire una riga orizzontale

    movq mm1, [edi]
	psubusb mm1, mm0
	movq [edi], mm1

	add edi, 8
	dec ecx
	jnz ciclo2 //fine ciclo linea orizzontale

	paddusb mm0, mm2 //incremento il sottrattore (intensity += delta_intensity)

	mov ecx, [max] //ripristino la dimensione della linea

	dec ebx 
	test ebx, ebx
	jnz ciclo2 //salto se devo scurire un'altra linea

	emms
  }

if (mode==FADESCOPE2D_ADD)
  _asm   // ndHere: codice parzialmente rippato dal flash MMX di Torbo 
	     // a sua volta rippato dal flash asm normale mio ;)
	     // N.B. RIPPARE RULEZZZ
  {
    mov eax, [packed_intensity]
	movd mm0, eax
	punpckldq mm0, mm0

	mov eax, [packed_delta_intensity]
	movd mm2, eax
	punpckldq mm2, mm2

	mov ebx, [delta_count]
	mov ecx, [max]
	mov edi, [pippo]     // ndTurbo: notare il nome molto significativo
	                     // usato da Here(sux)

						 // ndHere: MA RULLAAA
						 // il coefficiente Pippo è di un design allucinante
						 // infatti + sotto ho aggiunto pippo2 ;P

// ***PARTE SUPERIORE***
ciclo3: //inizio ciclo per schiarire una riga orizzontale

    movq mm1, [edi]
	paddusb mm1, mm0
	movq [edi], mm1

	add edi, 8
	dec ecx
	jnz ciclo3 //fine ciclo linea orizzontale

	psubusb mm0, mm2 //decremento l'addendo (intensity -= delta_intensity)

	mov ecx, [max] //ripristino la dimensione della linea

	dec ebx 
	test ebx, ebx
	jnz ciclo3 //salto se devo schiarire un'altra linea

// ***PARTE INFERIORE***
    mov edi, [pippo2] 
	mov ebx, [delta_count]
ciclo4: //inizio ciclo per schiarire una riga orizzontale

    movq mm1, [edi]
	paddusb mm1, mm0
	movq [edi], mm1

	add edi, 8
	dec ecx
	jnz ciclo4 //fine ciclo linea orizzontale

	paddusb mm0, mm2 //incremento l'addendo (intensity += delta_intensity)

	mov ecx, [max] //ripristino la dimensione della linea

	dec ebx 
	test ebx, ebx
	jnz ciclo4 //salto se devo schiarire un'altra linea

	emms
  }
}