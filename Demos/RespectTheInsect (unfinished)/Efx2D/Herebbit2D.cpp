#include "herebbit2D.h"
#include "..\utility.h"
#include <math.h>

extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

void herebbit2D::init()
{ 
} 

void herebbit2D::do_efx2D(double pos)
{
	int x,y,intens;

//	if (rand()>RAND_MAX/2)
	{

	for(y=0; y<image->height; y++)
		for(x=0; x<image->width; x++)
		{
			intens=((image->uint32ptr[x+image->muly[y]] & 0xFF)+
				   (((image->uint32ptr[x+image->muly[y]]>>8) & 0xFF)<<1)+
				   ((image->uint32ptr[x+image->muly[y]]>>16) & 0xFF))>>2;
			if (y & 1) intens>>=1;
			
			image->uint32ptr[x+image->muly[y]]=(255-intens)+(intens<<8)+((intens)<<16);
		}

	}
}