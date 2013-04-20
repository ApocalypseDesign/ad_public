//  _____________________________________________________ _ _ _ 
// |_Water_2d_Effect_____________________________________|-|@|x|
// | ~~~~~ ~~ ~~~~~~                                      ~ ~|~| 
// |                                                         |_|
// | Main code : Ska                                         |#|
// | Version   : 0.01                                        |#|
// | Date of creation : 15/12/1998                           |#|
// | Last modify      : 20/10/1999                           |~|
// |_________________________________________________________|_|

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "water2D.h"
#include "..\ADrender.h"
#include "..\utility.h"

#define Pi__ 3.141592
#define sqr(a) ((a)*(a))

extern HWND miawin;  // finestra principale; usata qui per le debug_error


water2D::water2D()   // costruttore
{
 w_new=w_old=(short int *)NULL;
 back=(unsigned char *)NULL;
 r_blur=g_blur=b_blur=0;
 rm=gm=bm=0;
 blittype=NOBLUR;
}


void water2D::set_blur(int r, int g, int b)
{ 
  int i;

  rm=gm=bm=1;
  r_blur=r;
  g_blur=g;
  b_blur=b;
  if (r!=0)
  {	for (i=1;i<=(r_blur);i++) {rm*=2;}
    rm-=1;
  }
  else rm=0;
  if (g!=0)
  { for (i=1;i<=(g_blur);i++) {gm*=2;}
    gm-=1;
  }
  else gm=0;
  if (b!=0)
  { for (i=1;i<=(b_blur);i++) {bm*=2;}
    bm-=1;
  }
  else bm=0;
 
  if  ((rm==0)&&(gm==00)&&(bm==0)) blittype=NOBLUR;
  else blittype=BLUR;
}


void water2D::init()
{ //----- Alloc memory for buffers ------------------(x)-;
  w_old =(short int *) malloc(image->channelsize*sizeof(short int));
  w_new =(short int *) malloc(image->channelsize*sizeof(short int));
  
  for (int i=0; i<image->channelsize; i++)
  {
    w_old[i]=w_new[i]=0;
  }
} 


void water2D::set_background(unsigned char *_back)
{
  back=_back;
} 	


void water2D::set_background(char *nomefile)
{
   if (mytex.load_jpg(nomefile, RGBINTERLEAVED)==FILENOTFOUND)
	  debug_error(miawin, "water error: texture not found");

  AddUpdate_Texture(&mytex);
  back=mytex.map;
}


void water2D::goccia(int x, int y)
{ 
  int w= x + image->muly[y];

   w_old[w]=254;
   w_old[w+1]=193;
   w_old[w-1]=193;
   w_old[w+image->width]=183;
   w_old[w-image->width]=183;
   w_old[w+image->width+1]=172;
   w_old[w-image->width-1]=172;
   w_old[w+image->width-1]=172;
   w_old[w-image->width+1]=172;
}


void water2D::dowater(short int *w_old, short int *w_new)
{


// ---------   settaggi per la versione ASM
  int x11=((image->width*2) *2);
  int image_width = (image->width *2);
  int c = image->channelsize-(image->width*4 *2);

  _asm
  {
	mov esi, [w_old]
	add esi, [x11]      // base esi = w_old + x11

	mov edi, [w_new]
	add edi, [x11]      // base edi = w_new + x11

	mov eax, [image_width]   // usato per fare scostamenti
	mov ebx, [x11]           // usato per fare scostamenti
	mov ecx, [c]
	shr ecx, 2

ciclo:

	movq    mm0, [esi+eax+2]   // mm0 +=w_old[boffs+(image->width+1)]
    pxor    mm1, mm1

	movq    mm2, [esi+eax-2]
	paddw   mm1, mm0
    movq    mm3, [esi+ebx] 
	paddw   mm1, mm2
    movq    mm4, [esi]
	paddw   mm1, mm3
    movq    mm5, [esi+ebx-2]
	paddw   mm1, mm4
    movq    mm6, [esi+2]
	paddw   mm1, mm5
    movq    mm7, [esi+ebx+2]
	paddw   mm1, mm6
    movq    mm0, [esi-2]
	paddw   mm1, mm7
	movq    mm5, [edi+eax]
	paddw   mm1, mm0

    psrlw  mm1, 2
	add    esi, 8
	psrlw  mm5, 7           // densita'
	add    edi, 8
	paddw    mm5, [edi+eax-8]
    psubusw  mm1, mm5

    movq  [edi+eax-8], mm1
	dec ecx
	jnz ciclo
    emms
  }
/*

  // -----    settaggi per la versione C
  int k;
  int x11=(image->width*2);
  int image_width = image->width;
  int c = image->channelsize-(image->width*4);
  int boffs = x11;
  short int k1;

   for(k=c; k>0; k--)
   { 
	  k1= (w_old[boffs+(image->width+1)    ]+
           w_old[boffs+(image->width-1)    ]+
           w_old[boffs+x11                 ]+
           w_old[boffs                     ]+
           w_old[boffs+x11-1               ]+
           w_old[boffs+1                   ]+
           w_old[boffs+x11+1               ]+
           w_old[boffs-1                   ])>>2 ;
       
      k1-= (w_new[boffs+image->width]   +
	       (w_new[boffs+image->width]>>7 )) ; // densita'
      
	  w_new[boffs+image->width]=max(k1,0);
	  boffs++;
    }
*/
}


void water2D::plot(short int *w_new, unsigned char *back)
{ 
  unsigned char *img=image->uchar8ptr; 
  unsigned int *img32=image->uint32ptr;
  unsigned char *bb=back;
  unsigned int *back32=(unsigned int *)back;
  int x22=(image->width);
  int cont = (image->channelsize-x22);
  int k,k2;
  short int k1;
  __int64 mul_blur=(__int64)(bm) | ((__int64)(gm)<<16) 
	                | ((__int64)(rm)<<32);

  int max_mul=max(max(b_blur, g_blur), r_blur);
  int bmul, rmul, gmul;
  __int64 mulsh;
  __int64 shift=(__int64)(max_mul);

  bmul=1 << (max_mul-b_blur);
  gmul=1 << (max_mul-g_blur);
  rmul=1 << (max_mul-r_blur);
  mulsh=(__int64)(bmul) | ((__int64)(gmul)<<16) |
	    ((__int64)(rmul)<<32);

/*
  if (blittype==NOBLUR)
  {
    // ------------   versione ASM  senza blur
	  _asm
	  {
		mov edi, [w_new]
        mov ecx, [cont]
		mov edx, [x22]      // edx = image->width
        shl edx, 1          // edx = image->width espressa in 
		                    // short int
		xor ebx, ebx        // ebx = k
		mov esi, [img]

    ciclo:
		xor eax, eax
        mov ax, [edi]
		add ax, [edi+2]
		add ax, [edi+edx]
		shr eax, 4       // eax = k1

        movd mm0, eax
		punpcklbw mm0, mm0
		punpcklwd mm0, mm0  // mm0 = |?|?|?|?|k1|k1|k1|k1|
		
		add eax, ebx     // eax=k2 := k+k1
		shl eax, 2       // eax = k2 << 2

        add eax, [bb]    // eax = back + k2<<2
        movd mm1, [eax]  // mm1 = pixel di sfondo
		paddusb mm1, mm0

        movd [esi], mm1
		
		add esi, 4   // mi sposto nell'image buffer un pixel avanti
        add edi, 2   // mi sposto in w_new di un short int avanti

		inc ebx
		dec ecx
		jnz ciclo
		
        emms
	  }
  }
  else
  {
   // ------------   versione ASM con blur
	  _asm
	  {
		mov edi, [w_new]
        mov ecx, [cont]
		mov edx, [x22]      // edx = image->width
        shl edx, 1          // edx = image->width espressa in 
		                    // short int
		xor ebx, ebx        // ebx = k
		mov esi, [img]
        pxor mm7, mm7

    ciclo2:
		xor eax, eax
        mov ax, [edi]
		add ax, [edi+2]
		add ax, [edi+edx]
		sar eax, 4       // eax = k1

        movd mm0, eax
		add eax, ebx     // eax=k2 := k+k1

		punpcklbw mm0, mm0
		shl eax, 2       // eax = k2 << 2

		punpcklwd mm0, mm0  // mm0 = |?|?|?|?|k1|k1|k1|k1|
        add eax, [bb]       // eax = back + k2<<2

        movd mm1, [eax]  // mm1 = pixel di sfondo (back)
		paddusb mm1, mm0 // mm1 saturato a 255

        movd mm6, [esi]
        punpcklbw mm1, mm7   // mm1 = pixel back in word16

		pmullw mm1, [mul_blur]
        punpcklbw mm6, mm7   // mm6 = *img in word16

		movq  mm5, [mulsh]
		paddw mm6, mm1

        pmullw mm6, mm5
        add edi, 2   // mi sposto in w_new di un short int avanti

		psrlw mm6, [shift]
        packuswb mm6, mm7

        movd [esi], mm6
		add esi, 4   // mi sposto nell'image buffer un pixel avanti

		inc ebx
		dec ecx
		jnz ciclo2
        emms
	  }
  }
*/

//  if (blittype==NOBLUR)
//  {
 //  ---------   versioni C
  for(k=0;k<(image->channelsize-x22);k++)  
  { 
	 k1 = ((w_new[k             ]+                    
            w_new[k+1           ]+
            w_new[k+image->width] )>>4); // distorsione
         
   k2=min((k+k1),image->channelsize);
   //k1>>=1; // illuminazione

    // versione senza blur
	*img32=back32[k2];
	img32++;
  }
/*
  }
  else
  {
    for(k=0;k<(image->channelsize-x22);k++)  
	{ 
	 k1 = ((w_new[k             ]+                    
            w_new[k+1           ]+
            w_new[k+image->width] )>>4); // distorsione
         
     k2=min((k+k1),image->channelsize);
     k1>>=1; // illuminazione
     // versione col blur
	 (*img) = ((*img)+31*(min(back[(k2<<2)  ]+k1,255)) )>>5;
     img++;
     (*img) = ((*img)+15*(min(back[(k2<<2)+1]+k1,255)) )>>4;
     img++;
	 (*img) = ((*img)+7*(min(back[(k2<<2)+2]+k1,255)) )>>3;
	 img+=2;
	}
  }
*/
}


void water2D::do_efx2D(double pos)
{  
  short int *tmp, i;
  float xs, ys;
  int xi, yi;

//    xs=(cosf(4*pos)+1)/2;
//    ys=(sinf(5*pos)+1)/2;
    xs=16+4*pos;
	ys=-0.7*xs*pos;

	xs*=(image->width-2);
	ys*=(image->height-2);
	xi=abs((int)xs);
	yi=abs((int)ys);
	xi=(xi % (image->width-2)) +1;
	yi=(yi % (image->height-2)) +1;
	goccia(xi, yi);

	ys=0.8*xs*pos;
	ys*=(image->height-2);
	yi=abs((int)ys);
	yi=1+(yi % (image->height-2));
	goccia(xi, yi);
   
  dowater(w_old, w_new);
  plot(w_new, back);  
  tmp=w_new; w_new=w_old; w_old=tmp;
}
