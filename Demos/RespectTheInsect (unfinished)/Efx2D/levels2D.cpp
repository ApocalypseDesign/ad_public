#include "levels2D.h"
#include "..\utility.h"
#include <math.h>

extern HWND miawin;  // finestra principale; usata qui
                     // per le debug_error

void levels2D::init()
{ 
	calc_luts(0,255,0,255,0,255);
	rmin=gmin=bmin=0;
	rmax=gmax=bmax=255;
} 

void levels2D::calc_luts(int r_min,int r_max,int g_min,int g_max,int b_min,int b_max)
{
	int i;

	if ((r_min!=old_r_min) || (r_max!=old_r_max))
	{
		for (i=0; i<r_min; i++) r_lut[i]=0;
		for (i=r_min; i<=r_max; i++) r_lut[i]=255*(i-r_min)/(r_max-r_min);
		for (i=r_max; i<256; i++) r_lut[i]=255;
		old_r_min=r_min;
		old_r_max=r_max;
	}
	
	if ((g_min!=old_g_min) || (g_max!=old_g_max))
	{
		for (i=0; i<g_min; i++) g_lut[i]=0;
		for (i=g_min; i<=g_max; i++) g_lut[i]=255*(i-g_min)/(g_max-g_min);
		for (i=g_max; i<256; i++) g_lut[i]=255;
		old_g_min=g_min;
		old_g_max=g_max;
	}
	
	if ((b_min!=old_b_min) || (b_max!=old_b_max))
	{
		for (i=0; i<b_min; i++) b_lut[i]=0;
		for (i=b_min; i<=b_max; i++) b_lut[i]=255*(i-b_min)/(b_max-b_min);
		for (i=b_max; i<256; i++) b_lut[i]=255;
		old_b_min=b_min;
		old_b_max=b_max;
	}
}

void levels2D::do_efx2D(double pos)
{
	int x;
	int r_min,r_max,g_min,g_max,b_min,b_max,r,g,b;
	float inv_vel=1.0f-velocity;

	if ((mode==LEVELS_AUTO) || (mode==LEVELS_AUTO_LUMINOSITY))
	{
		r_min=g_min=b_min=255;
		r_max=g_max=b_max=0;

		for (x=0; x<image->channelsize; x++)
		{
			r=(image->uint32ptr[x]>>16) & 0xFF;
			g=(image->uint32ptr[x]>>8) & 0xFF;
			b=image->uint32ptr[x] & 0xFF;
	
			image->uint32ptr[x]=(r_lut[r]<<16)+(g_lut[g]<<8)+b_lut[b];
			
		    if (r>r_max) r_max=r;
			if (g>g_max) g_max=g;
			if (b>b_max) b_max=b;

			if (r<r_min) r_min=r;
			if (g<g_min) g_min=g;
			if (b<b_min) b_min=b;
		}

		r_min=int(r_min*velocity+old_r_min*inv_vel);
		g_min=int(g_min*velocity+old_g_min*inv_vel);
		b_min=int(b_min*velocity+old_b_min*inv_vel);
		
		r_max=int(r_max*velocity+old_r_max*inv_vel);
		g_max=int(g_max*velocity+old_g_max*inv_vel);
		b_max=int(b_max*velocity+old_b_max*inv_vel);

		if (mode==LEVELS_AUTO_LUMINOSITY)
		{
			r_min=(r_min+(g_min<<1)+b_min)>>2;
			r_max=(r_max+(g_max<<1)+b_max)>>2;
			calc_luts(r_min,r_max,r_min,r_max,r_min,r_max);
		}
		else calc_luts(r_min,r_max,g_min,g_max,b_min,b_max);
	}

	if (mode==LEVELS_MANUAL)
	{
		calc_luts(rmin,rmax,gmin,gmax,bmin,bmax);

		for (x=0; x<image->channelsize; x++)
		{
			r=(image->uint32ptr[x]>>16) & 0xFF;
			g=(image->uint32ptr[x]>>8) & 0xFF;
			b=image->uint32ptr[x] & 0xFF;

			image->uint32ptr[x]=(r_lut[r]<<16)+(g_lut[g]<<8)+b_lut[b];
		}
	}
}