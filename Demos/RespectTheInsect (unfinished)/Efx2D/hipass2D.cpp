#include "hipass2D.h"
#include "..\utility.h"
#include <math.h>

// HIPASS FILTER by HERE

// ndHere: per iniziare premetto: (TURBO && SKA) SUXXX ;)
//		   ora posso iniziare a codare con design...

extern HWND miawin;  // ndTurbo: finestra principale; usata qui
                     // per le debug_error

					 // ndHere: Turbo, lo vedi che suxi?
					 // non serviva mettere sta variabile suxante
				     // alla debug_error basta passargli 0 che va! :P

void hipass2D::init()
{ 
	// niente init, io rullezzo troppo per fare cose suxanti come gli init
} 

/*

FUNZIONAMENTO:

 vedere lowpass.cpp

*/

void hipass2D::do_efx2D(double pos)
{
	int x,y,i,j,r_sum,g_sum,b_sum;
	int r_med,g_med,b_med;
	int total_window=(window<<1)+1;
	int square_total_window=total_window*total_window;

	if (window<=0) return;
	if (total_window>MAX_HIPASS_WINDOW) return;

	r_med=g_med=b_med=0;
	i=0;

	//*********************************************************************

	for(y=0; y<window; y++) // loop iniziale delle y
	{
		// preparo tutto in modo da ripetere i pixel al bordo sinistro
		// *** ZONA 1 ***

	r_sum=g_sum=b_sum=0;

		for (j=y-window; j<=0; j++)
		{
			r_sum+=((image->uint32ptr[0])>>16) & 0xFF;
			g_sum+=((image->uint32ptr[0])>>8) & 0xFF;
			b_sum+=(image->uint32ptr[0]) & 0xFF;
		}
		for(j=1; j<=y+window; j++)
		{
			r_sum+=((image->uint32ptr[image->muly[j]])>>16) & 0xFF;
			g_sum+=((image->uint32ptr[image->muly[j]])>>8) & 0xFF;
			b_sum+=(image->uint32ptr[image->muly[j]]) & 0xFF;
		}

		val[0][0]=(r_sum<<16)/square_total_window;
		val[0][1]=(g_sum<<16)/square_total_window;
		val[0][2]=(b_sum<<16)/square_total_window;

		for(j=1; j<=window; j++) memcpy(val[j],val[0],12);

		for (x=1; x<=window; x++)
		{
			r_sum=g_sum=b_sum=0;

			
			for (j=y-window; j<=0; j++)
			{
				r_sum+=((image->uint32ptr[x])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x]) & 0xFF;
			}
			for(j=1; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[x+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+image->muly[j]]) & 0xFF;
			}
			val[window+x][0]=(r_sum<<16)/square_total_window;
			val[window+x][1]=(g_sum<<16)/square_total_window;
			val[window+x][2]=(b_sum<<16)/square_total_window;
		}

		r_med=g_med=b_med=0;

		for (x=0; x<total_window; x++)
		{
			r_med+=0;val[x][0];
			g_med+=0;val[x][1];
			b_med+=0;val[x][2];
		}
		
		i=0;
		

		// ok, ora i valori sono messi in modo che al bordo i valori si ripetono

		for(x=0; x<image->width-window-1; x++) // loop principale delle x
		{
			// *** ZONA 2 ***

			r_med-=val[i][0];
			g_med-=val[i][1];
			b_med-=val[i][2];

			r_sum=g_sum=b_sum=0;

			for (j=y-window; j<0; j++)
			{
				r_sum+=((image->uint32ptr[x+window])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+window])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+window]) & 0xFF;
			}
			for(j=0; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[x+window+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+window+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+window+image->muly[j]]) & 0xFF;
			}

			val[i][0]=(r_sum<<16)/square_total_window;
			val[i][1]=(g_sum<<16)/square_total_window;
			val[i][2]=(b_sum<<16)/square_total_window;

			r_med+=val[i][0];
			g_med+=val[i][1];
			b_med+=val[i][2];

			r_sum=128+(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF) - (r_med>>16);
			g_sum=128+(((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF) - (g_med>>16);
			b_sum=128+((image->uint32ptr[x+image->muly[y]]) & 0xFF) - (b_med>>16);

			if (r_sum<0) r_sum=0; else 	if (r_sum>255) r_sum=255;
			if (g_sum<0) g_sum=0; else 	if (g_sum>255) g_sum=255;
			if (b_sum<0) b_sum=0; else 	if (b_sum>255) b_sum=255;	

			image->uint32ptr[x+image->muly[y]]=(r_sum<<16)+(g_sum<<8)+b_sum; // HIPASS
			
			i++;
			if (i>=total_window) i-=total_window;

		} // fine loop principale delle x

		// ora ripeto i pixel anche al bordo destro
		for(x=image->width-window-1; x<image->width; x++) // loop finale delle x
		{
			// *** ZONA 3 ***
			r_med-=val[i][0];
			g_med-=val[i][1];
			b_med-=val[i][2];

			r_sum=g_sum=b_sum=0;

			for (j=y-window; j<=0; j++)
			{
				r_sum+=((image->uint32ptr[image->width-1])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[image->width-1])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[image->width-1]) & 0xFF;
			}
			for(j=1; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[image->width-1+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[image->width-1+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[image->width-1+image->muly[j]]) & 0xFF;
			}

			val[i][0]=(r_sum<<16)/square_total_window;
			val[i][1]=(g_sum<<16)/square_total_window;
			val[i][2]=(b_sum<<16)/square_total_window;

			r_med+=val[i][0];
			g_med+=val[i][1];
			b_med+=val[i][2];

			r_sum=128+(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF) - (r_med>>16);
			g_sum=128+(((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF) - (g_med>>16);
			b_sum=128+((image->uint32ptr[x+image->muly[y]]) & 0xFF) - (b_med>>16);

			if (r_sum<0) r_sum=0; else 	if (r_sum>255) r_sum=255;
			if (g_sum<0) g_sum=0; else 	if (g_sum>255) g_sum=255;
			if (b_sum<0) b_sum=0; else 	if (b_sum>255) b_sum=255;	

			image->uint32ptr[x+image->muly[y]]=(r_sum<<16)+(g_sum<<8)+b_sum; // HIPASS

			i++;
			if (i>=total_window) i-=total_window;

		} // fine loop finale delle x

	} // fine loop iniziale delle y


	//*********************************************************************

	for(y=window; y<image->height-window-1; y++) // loop principale delle y
	{
		// preparao tutto in modo da ripetere i pixel al bordo sinistro
		// *** ZONA 4 ***



		r_sum=g_sum=b_sum=0;

		for(j=y-window; j<=y+window; j++)
		{
			r_sum+=((image->uint32ptr[image->muly[j]])>>16) & 0xFF;
			g_sum+=((image->uint32ptr[image->muly[j]])>>8) & 0xFF;
			b_sum+=(image->uint32ptr[image->muly[j]]) & 0xFF;
		}

		val[0][0]=(r_sum<<16)/square_total_window;
		val[0][1]=(g_sum<<16)/square_total_window;
		val[0][2]=(b_sum<<16)/square_total_window;

		for(j=1; j<=window; j++) memcpy(val[j],val[0],12);

		for (x=1; x<=window; x++)
		{
			r_sum=g_sum=b_sum=0;

			for(j=y-window; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[x+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+image->muly[j]]) & 0xFF;
			}

			val[window+x][0]=(r_sum<<16)/square_total_window;
			val[window+x][1]=(g_sum<<16)/square_total_window;
			val[window+x][2]=(b_sum<<16)/square_total_window;
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

			r_sum=g_sum=b_sum=0;

			for(j=y-window; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[x+window+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+window+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+window+image->muly[j]]) & 0xFF;
			}

			val[i][0]=(r_sum<<16)/square_total_window;
			val[i][1]=(g_sum<<16)/square_total_window;
			val[i][2]=(b_sum<<16)/square_total_window;


			r_med+=val[i][0];
			g_med+=val[i][1];
			b_med+=val[i][2];

		/*	r_sum=((((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF)<<1) - (r_med>>16);
			g_sum=((((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF)<<1) - (g_med>>16);
			b_sum=(((image->uint32ptr[x+image->muly[y]]) & 0xFF)<<1) - (b_med>>16);
*/
			r_sum=128+(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF) - (r_med>>16);
			g_sum=128+(((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF) - (g_med>>16);
			b_sum=128+((image->uint32ptr[x+image->muly[y]]) & 0xFF) - (b_med>>16);
/*
			r_sum= (r_med>>16)-(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF);
			g_sum= (g_med>>16)-(((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF);
			b_sum= (b_med>>16)-((image->uint32ptr[x+image->muly[y]]) & 0xFF);

//			r_sum=r_med -(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF) ;
//			g_sum=g_med - (((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF);
//			b_sum=b_med -((image->uint32ptr[x+image->muly[y]]) & 0xFF);

			r_sum=abs(r_sum<<1);
			g_sum=abs(g_sum<<1);
			b_sum=abs(b_sum<<1);
*/
			if (r_sum<0) r_sum=0; else 	if (r_sum>255) r_sum=255;
			if (g_sum<0) g_sum=0; else 	if (g_sum>255) g_sum=255;
			if (b_sum<0) b_sum=0; else 	if (b_sum>255) b_sum=255;	

			image->uint32ptr[x+image->muly[y]]=

				//(r_med & 0xFF0000)+((g_med>>8) & 0xFF00)+(b_med>>16); //LOWPASS

				//((((r_med<<16)+(g_med<<8)+b_med)&0xFEFEFE)>>1)+ //LOWPASS/BLUR50%
				//((image->uint32ptr[x+image->muly[y]]&0xFEFEFE)>>1);
				
				(r_sum<<16)+(g_sum<<8)+b_sum;

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

			r_sum=g_sum=b_sum=0;

			for(j=y-window; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[image->width-1+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[image->width-1+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[image->width-1+image->muly[j]]) & 0xFF;
			}

			val[i][0]=(r_sum<<16)/square_total_window;
			val[i][1]=(g_sum<<16)/square_total_window;
			val[i][2]=(b_sum<<16)/square_total_window;

			r_med+=val[i][0];
			g_med+=val[i][1];
			b_med+=val[i][2];

		 	r_sum=128+(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF) - (r_med>>16);
			g_sum=128+(((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF) - (g_med>>16);
			b_sum=128+((image->uint32ptr[x+image->muly[y]]) & 0xFF) - (b_med>>16);

			if (r_sum<0) r_sum=0; else 	if (r_sum>255) r_sum=255;
			if (g_sum<0) g_sum=0; else 	if (g_sum>255) g_sum=255;
			if (b_sum<0) b_sum=0; else 	if (b_sum>255) b_sum=255;	

			image->uint32ptr[x+image->muly[y]]=(r_sum<<16)+(g_sum<<8)+b_sum; // HIPASS

			i++;
			if (i>=total_window) i-=total_window;

		} // fine loop finale delle x

	} // fine loop principale delle y


	//***********************************************************************

	for(y=image->height-window-1; y<image->height; y++) // loop finale delle y
	{
		// preparao tutto in modo da ripetere i pixel al bordo sinistro
		// *** ZONA 7 ***

		r_sum=g_sum=b_sum=0;
		
		for(j=y-window; j<image->height; j++)
		{
			r_sum+=((image->uint32ptr[image->muly[j]])>>16) & 0xFF;
			g_sum+=((image->uint32ptr[image->muly[j]])>>8) & 0xFF;
			b_sum+=(image->uint32ptr[image->muly[j]]) & 0xFF;
		}
		for(j=image->height; j<=y+window; j++)
		{
			r_sum+=((image->uint32ptr[image->muly[image->height-1]])>>16) & 0xFF;
			g_sum+=((image->uint32ptr[image->muly[image->height-1]])>>8) & 0xFF;
			b_sum+=(image->uint32ptr[image->muly[image->height-1]]) & 0xFF;
		}

		val[0][0]=(r_sum<<16)/square_total_window;
		val[0][1]=(g_sum<<16)/square_total_window;
		val[0][2]=(b_sum<<16)/square_total_window;

		for(j=1; j<=window; j++) memcpy(val[j],val[0],12);

		for (x=1; x<=window; x++)
		{
			r_sum=g_sum=b_sum=0;
			
			for(j=y-window; j<image->height; j++)
			{
				r_sum+=((image->uint32ptr[x+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+image->muly[j]]) & 0xFF;
			}
			for(j=image->height; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[x+image->muly[image->height-1]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+image->muly[image->height-1]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+image->muly[image->height-1]]) & 0xFF;
			}

			val[window+x][0]=(r_sum<<16)/square_total_window;
			val[window+x][1]=(g_sum<<16)/square_total_window;
			val[window+x][2]=(b_sum<<16)/square_total_window;
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
			// *** ZONA 8 ***

			r_med-=val[i][0];
			g_med-=val[i][1];
			b_med-=val[i][2];

			r_sum=g_sum=b_sum=0;

			for(j=y-window; j<image->height; j++)
			{
				r_sum+=((image->uint32ptr[x+window+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+window+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+window+image->muly[j]]) & 0xFF;
			}
			for(j=image->height; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[x+window+image->muly[image->height-1]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[x+window+image->muly[image->height-1]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[x+window+image->muly[image->height-1]]) & 0xFF;
			}

			val[i][0]=(r_sum<<16)/square_total_window;
			val[i][1]=(g_sum<<16)/square_total_window;
			val[i][2]=(b_sum<<16)/square_total_window;

			r_med+=val[i][0];
			g_med+=val[i][1];
			b_med+=val[i][2];

			r_sum=128+(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF) - (r_med>>16);
			g_sum=128+(((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF) - (g_med>>16);
			b_sum=128+((image->uint32ptr[x+image->muly[y]]) & 0xFF) - (b_med>>16);

			if (r_sum<0) r_sum=0; else 	if (r_sum>255) r_sum=255;
			if (g_sum<0) g_sum=0; else 	if (g_sum>255) g_sum=255;
			if (b_sum<0) b_sum=0; else 	if (b_sum>255) b_sum=255;	

			image->uint32ptr[x+image->muly[y]]=(r_sum<<16)+(g_sum<<8)+b_sum; // HIPASS

			i++;
			if (i>=total_window) i-=total_window;

		} // fine loop principale delle x

		// ora ripeto i pixel anche al bordo destro
		for(x=image->width-window-1; x<image->width; x++) // loop finale delle x
		{
			// *** ZONA 9 ***

			r_med-=val[i][0];
			g_med-=val[i][1];
			b_med-=val[i][2];

			r_sum=g_sum=b_sum=0;

			for(j=y-window; j<image->height; j++)
			{
				r_sum+=((image->uint32ptr[image->width-1+image->muly[j]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[image->width-1+image->muly[j]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[image->width-1+image->muly[j]]) & 0xFF;
			}
			for(j=image->height; j<=y+window; j++)
			{
				r_sum+=((image->uint32ptr[image->width-1+image->muly[image->height-1]])>>16) & 0xFF;
				g_sum+=((image->uint32ptr[image->width-1+image->muly[image->height-1]])>>8) & 0xFF;
				b_sum+=(image->uint32ptr[image->width-1+image->muly[image->height-1]]) & 0xFF;
			}

			val[i][0]=(r_sum<<16)/square_total_window;
			val[i][1]=(g_sum<<16)/square_total_window;
			val[i][2]=(b_sum<<16)/square_total_window;

			r_med+=val[i][0];
			g_med+=val[i][1];
			b_med+=val[i][2];

			r_sum=128+(((image->uint32ptr[x+image->muly[y]])>>16) & 0xFF) - (r_med>>16);
			g_sum=128+(((image->uint32ptr[x+image->muly[y]])>>8) & 0xFF) - (g_med>>16);
			b_sum=128+((image->uint32ptr[x+image->muly[y]]) & 0xFF) - (b_med>>16);

			if (r_sum<0) r_sum=0; else 	if (r_sum>255) r_sum=255;
			if (g_sum<0) g_sum=0; else 	if (g_sum>255) g_sum=255;
			if (b_sum<0) b_sum=0; else 	if (b_sum>255) b_sum=255;	

			image->uint32ptr[x+image->muly[y]]=(r_sum<<16)+(g_sum<<8)+b_sum; // HIPASS

			i++;
			if (i>=total_window) i-=total_window;

		} // fine loop finale delle x

	} // fine loop finale delle y

}