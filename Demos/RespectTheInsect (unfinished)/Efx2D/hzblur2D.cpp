#include "hzblur2D.h"
#include "..\utility.h"
#include <math.h>

// HORIZONTAL BLUR FILTER by HERE

// ndHere: per iniziare premetto: (TURBO && SKA) SUXXX ;)
//		   ora posso iniziare a codare con design...

extern HWND miawin;  // ndTurbo: finestra principale; usata qui
                     // per le debug_error

					 // ndHere: Turbo, lo vedi che suxi?
					 // non serviva mettere sta variabile suxante
				     // alla debug_error basta passargli 0 che va! :P

void hzblur2D::init()
{ 
	// niente init, io rullezzo troppo per fare cose suxanti come gli init
} 

/*

FUNZIONAMENTO:

  simile a lowpass2D.cpp

*/

void hzblur2D::do_efx2D(double pos)
{
	int x,y,i,j,r_sum,g_sum,b_sum;
	int r_med,g_med,b_med;
	int total_window=(window<<1)+1;
	int square_total_window=total_window*total_window;

	if (window<=0) return;
	if (total_window>MAX_HZBLUR_WINDOW) return;

	//*********************************************************************

	for(y=0; y<image->height; y++) // loop principale delle y
	{
		// preparao tutto in modo da ripetere i pixel al bordo sinistro
		// *** ZONA 4 ***

		r_sum=((image->uint32ptr[image->muly[y]])>>16) & 0xFF;
		g_sum=((image->uint32ptr[image->muly[y]])>>8) & 0xFF;
		b_sum=(image->uint32ptr[image->muly[y]]) & 0xFF;

		val[0][0]=(r_sum<<16)/total_window;
		val[0][1]=(g_sum<<16)/total_window;
		val[0][2]=(b_sum<<16)/total_window;

		for(j=1; j<=window; j++) memcpy(val[j],val[0],12);

		for (x=1; x<=window; x++)
		{
			r_sum=((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF;
			g_sum=((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF;
			b_sum=(image->uint32ptr[x+image->muly[y]]) & 0xFF;

			val[window+x][0]=(r_sum<<16)/total_window;
			val[window+x][1]=(g_sum<<16)/total_window;
			val[window+x][2]=(b_sum<<16)/total_window;
		}

		r_med=g_med=b_med=0;

		for (x=0; x<total_window; x++)
		{
			r_med+=val[x][0];
			g_med+=val[x][1];
			b_med+=val[x][2];
		}
		
		i=0;
		
		// ok, ora i valori sono messi in modo che al bordo i valori si ripetono

		for(x=0; x<image->width-window-1; x++) // loop principale delle x
		{
			// *** ZONA 5 *** (la + importante come ottimizzazione)

			r_med-=val[i][0];
			g_med-=val[i][1];
			b_med-=val[i][2];

			//r_sum=((image->uint32ptr[x+window+image->muly[y]])>>16) & 0xFF;
		//	g_sum=((image->uint32ptr[x+window+image->muly[y]])>>8) & 0xFF;
		//	b_sum=(image->uint32ptr[x+window+image->muly[y]]) & 0xFF;


			val[i][0]=(image->uint32ptr[x+window+image->muly[y]] & 0xFF0000)/total_window;
			val[i][1]=((image->uint32ptr[x+window+image->muly[y]]<<8) & 0xFF0000)/total_window;
			val[i][2]=((image->uint32ptr[x+window+image->muly[y]]<<16) & 0xFF0000)/total_window;


			r_med+=val[i][0];
			g_med+=val[i][1];
			b_med+=val[i][2];
/*
			r_sum=((((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF)<<1) - (r_med>>16);
			g_sum=((((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF)<<1) - (g_med>>16);
			b_sum=(((image->uint32ptr[x+image->muly[y]]) & 0xFF)<<1) - (b_med>>16);

			r_sum=(r_med>>16) -(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF) ;
			g_sum=(g_med>>16) - (((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF);
			b_sum=(b_med>>16) -((image->uint32ptr[x+image->muly[y]]) & 0xFF);

			r_sum=abs(r_sum);
			g_sum=abs(g_sum);
			b_sum=abs(b_sum);
*/

			image->uint32ptr[x+image->muly[y]]=

				(r_med & 0xFF0000)+((g_med>>8) & 0xFF00)+(b_med>>16); //LOWPASS

				//((((r_med<<16)+(g_med<<8)+b_med)&0xFEFEFE)>>1)+ //LOWPASS/BLUR50%
				//((image->uint32ptr[x+image->muly[y]]&0xFEFEFE)>>1);
				
			//	(r_sum<<16)+(g_sum<<8)+b_sum;

			i++;
			if (i>=total_window) i-=total_window;

		} // fine loop principale delle x

		// ora ripeto i pixel anche al bordo destro
		for(x=image->width-window-1; x<image->width; x++) // loop finale delle x
		{
			// *** ZONA 6 ***

			r_med-=val[i][0];
			g_med-=val[i][1];
			b_med-=val[i][2];

			r_sum=((image->uint32ptr[image->width-1+image->muly[y]])>>16) & 0xFF;
			g_sum=((image->uint32ptr[image->width-1+image->muly[y]])>>8) & 0xFF;
			b_sum=(image->uint32ptr[image->width-1+image->muly[y]]) & 0xFF;

			val[i][0]=(r_sum<<16)/total_window;
			val[i][1]=(g_sum<<16)/total_window;
			val[i][2]=(b_sum<<16)/total_window;

			r_med+=val[i][0];
			g_med+=val[i][1];
			b_med+=val[i][2];

		    image->uint32ptr[x+image->muly[y]]=(r_med & 0xFF0000)+((g_med>>8) & 0xFF00)+(b_med>>16); //LOWPASS

			i++;
			if (i>=total_window) i-=total_window;

		} // fine loop finale delle x

	} // fine loop principale delle y


}