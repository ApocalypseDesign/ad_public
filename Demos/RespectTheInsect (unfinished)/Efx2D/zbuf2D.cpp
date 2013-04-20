#include "zbuf2D.h"
#include "..\utility.h"
#include "ADrender.h"
#include <math.h>


extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

void zbuf2D::init()
{ 
	wbuffer=SW_Get_WBuffer();
} 

void zbuf2D::do_efx2D(double pos)
{
	int x,y;

	for (y=1; y<image->height-1; y++)
		for(x=1; x<image->width-1; x++)
			if (
				(fabs(wbuffer->where[x+wbuffer->muly[y]]-wbuffer->where[x+1+wbuffer->muly[y]])>0.0001f)
				|| (fabs(wbuffer->where[x+wbuffer->muly[y]]-wbuffer->where[x+wbuffer->muly[y+1]])>0.0001f)
			    )
			{
				image->uint32ptr[x-1+image->muly[y]]=(image->uint32ptr[x-1+image->muly[y]] &0xFEFEFE)>>1;
				image->uint32ptr[x+image->muly[y-1]]=(image->uint32ptr[x+image->muly[y-1]] &0xFEFEFE)>>1;
				image->uint32ptr[x+image->muly[y]]=0x000000;
				image->uint32ptr[x+1+image->muly[y]]=(image->uint32ptr[x+1+image->muly[y]] &0xFEFEFE)>>1;
				image->uint32ptr[x+image->muly[y+1]]=(image->uint32ptr[x+image->muly[y+1]] &0xFEFEFE)>>1;
			}
			//else image->uint32ptr[x+image->muly[y]]=(image->uint32ptr[x+image->muly[y]] & 0xc0c0c0);
 
}