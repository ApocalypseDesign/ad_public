#include "herox.h"
#include "..\utility.h"
#include <math.h>

extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

void herox::init()
{ 
	int x,y;
	float value;

	particle_map=(unsigned char *)malloc(HEROX_PARTICLE_SIZE*HEROX_PARTICLE_SIZE);

	for (y=0; y<HEROX_PARTICLE_SIZE; y++)
		for (x=0; x<HEROX_PARTICLE_SIZE;  x++)
		{
			value=(1.0-sqrt(1+sqr(x-HEROX_PARTICLE_SIZE/2)+sqr(y-HEROX_PARTICLE_SIZE/2))/(HEROX_PARTICLE_SIZE/2))*HEROX_INTENSITY;
			if (value<0.0f) value=0.0f;
			if (value>255.0f) value=255.0f;
			particle_map[x+y*HEROX_PARTICLE_SIZE]=
				unsigned char (value);
		}

	/*
	for (y=0; y<HEROX_PARTICLE_SIZE; y++)
	{
		particle_map[0+y*HEROX_PARTICLE_SIZE]=0xF;
		particle_map[HEROX_PARTICLE_SIZE-1+y*HEROX_PARTICLE_SIZE]=0xF;
		particle_map[y]=0xF;
		particle_map[y+(HEROX_PARTICLE_SIZE-1)*HEROX_PARTICLE_SIZE]=0xF;
	}
	*/
				
} 

void herox::paint_particle(int x,int y)
{	
	int ix,iy,startx,starty;
	int dispx,dispy;
	int color;
	int r,g,b;

	startx=x-HEROX_PARTICLE_SIZE/2;
	starty=y-HEROX_PARTICLE_SIZE/2;

	if ((startx>=0) && (x<image->width-HEROX_PARTICLE_SIZE/2)
		&& (starty>=0) && (y<image->height-HEROX_PARTICLE_SIZE/2))
	{
		for (iy=0; iy<HEROX_PARTICLE_SIZE; iy++)
			for (ix=0; ix<HEROX_PARTICLE_SIZE; ix++)
			{	
				dispx=0;
				dispy=0;
				
				if ((ix>0) && (ix<HEROX_PARTICLE_SIZE-1))
				dispx=int(particle_map[ix+1+iy*HEROX_PARTICLE_SIZE])-int(particle_map[ix-1+iy*HEROX_PARTICLE_SIZE]);
		
				if ((iy>0) && (iy<HEROX_PARTICLE_SIZE-1))
				dispy=int(particle_map[ix+(iy+1)*HEROX_PARTICLE_SIZE])-int(particle_map[ix+(iy-1)*HEROX_PARTICLE_SIZE]);

				dispx>>=4;
				dispy>>=4;

				//dispx=dispy=0;

				color=//image->uint32ptr[startx+ix-dispx+image->muly[starty+iy-dispy]];
					((image->uint32ptr[startx+ix-dispx+image->muly[starty+iy-dispy]]& 0xfefefe)>>1)
					+ ((image->uint32ptr[startx+ix+image->muly[starty+iy]] & 0xfefefe)>>1);
				b=color & 0xFF;
				g=(color & 0xFF00)>>8;
				r=(color & 0xFF0000)>>16;
				/*
				r-=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				g-=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				b-=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				if (r<0) r=0;
				if (g<0) g=0;
				if (b<0) b=0;
				*/
				r+=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				//g+=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				//b+=particle_map[ix+iy*HEROX_PARTICLE_SIZE];
				if (r>255) r=255;
				//if (g>255) g=255;
				//if (b>255) b=255;
				
				color=b+(g<<8)+(r<<16);
				image->uint32ptr[startx+ix+image->muly[starty+iy]]=color;
				//	((color & 0xfefefe)>>1)
				//	+ ((image->uint32ptr[startx+ix+image->muly[starty+iy]] & 0xfefefe)>>1);
			}
	
	//	image->uint32ptr[x+image->muly[y]]=0; //per testare sta rullata
	}
}

void herox::do_efx2D(double pos)
{
	int x,y,x0,y0;
	unsigned int treshold_b=HEROX_TRESHOLD_B,
		         treshold_g=HEROX_TRESHOLD_G<<8,
				 treshold_r=HEROX_TRESHOLD_R<<16;

	x0=HEROX_SKIP/2+int(sin(pos*500.0)*(HEROX_SKIP/2));
	y0=HEROX_SKIP/2+int(sin(pos*800.0+107.12)*(HEROX_SKIP/2));

	for (y=y0; y<image->height; y+=HEROX_SKIP)
		for (x=x0; x<image->width;  x+=HEROX_SKIP)
		{
			if ((image->uint32ptr[x+image->muly[y]] & 0xFF)>treshold_b
				&& ((image->uint32ptr[x+image->muly[y]] & 0xFF00)>treshold_g)
				&& ((image->uint32ptr[x+image->muly[y]] & 0xFF0000)>treshold_r))
				paint_particle(x,y);
		}


//	paint_particle(160,120);

 /* int
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
	mov edi, [pippo]     // ndTurbo: notare il nome molto significativo
	                     // usato da Here(sux)

						 // ndHere: MA RULLAAA
						 // il coefficiente Pippo è di un design allucinante

ciclo:

    movq mm1, [edi]
	paddusb mm1, mm0
	movq [edi], mm1

	add edi, 8
	dec ecx
	jnz ciclo
	emms
  }

  */
}