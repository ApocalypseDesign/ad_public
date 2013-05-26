// codice principale della demo
// -------------------------------
// AD Demo System 99 - ADmain.h
// -------------------------------

#ifndef _ADMAIN_H_
#define _ADMAIN_H_



#include "dtimer.h"
//#include "fmod.h"
#include "bass.h"
//#include "Efx2D\bump2D.h"
#include "Efx2D\hzblur2D.h"
#include "Efx2D\lowpass2D.h"
#include "Efx2D\herebbit2D.h"
#include "Efx2D\fadescope2D.h"
#include "Efx2D\fade2D.h"
#include "Efx2D\levels2D.h"
#include "Efx2D\flash2D.h"
//#include "Efx2D\plasm2D.h"
//#include "Efx2D\painter2D.h"
//#include "Efx2D\spline.h"
//#include "Efx2D\noise.h"
//#include "Efx2D\blur2D.h"
//#include "Efx2D\tomato2D.h"
//#include "Efx2D\postfilters2D.h"
#include "Efx2D\TextScroller2D.h"
#include "Efx2D\quadro2D.h"
#include "Efx2D\sprite.h"
#include "Engines\scene3d.h"
//#include "Engines\Raytracing\rayt_scene.h"
#include "Math\fastmath.h"

//-----------------------------------------------------------
// VARIABILI GLOBALI DELLA DEMO
dtimer demotimer;
dtimer demotimer_end;
//FMUSIC_MODULE *mod=NULL;
HMUSIC mod;
imagebuffer *global_ib;
imagebuffer *ib_aux;
BOOL NoSound;
herebbit2D herebbit;
fadescope2D fadescope;
fade2D endfade;
levels2D levels1;
//bump2D     bump;
hzblur2D   hzblur;
lowpass2D	   blur;

//flash2D    flash;
//noise2D    pnoise2D;
//plasm2D    plasma;
//painter2D  painter;
//noise3D    pnoise3D;
//spline2D   spline;
//blur2D     blur;
//rainbow2D  rainbow;
//rubbing2D  rubbing;
//mosaic2D   mosaic;
//tomato2D   tomato;
//sniper2D   sniper;
//AD_RaytScene rtscene;
texture backg;
//AD_Sprite barra1, barra2;
//AD_Sprite what, is, demoscene, p_inter;
flash2D flash;
int flashflag=0;
quadro2D  required;
AD_Scene3D  scena1, scena2;
TextScroller2D text1, text2, text3;

#define SCENA_LOOP 6
#define TEXT_LOOP 7
//-----------------------------------------------------------

double relpos(double start, double end, double interpos)
{ return(interpos*(end-start)+start); }


void CALLBACK MySyncProc(HSYNC handle, DWORD channel,
                       DWORD data, DWORD user)
{
   if (data==1)
   {
	 flash.set_pos(curpos-0.005, curpos+0.005);
     flashflag=1;
   }
}


void ADmainEnter() //codice iniziale
{
  // per ragioni di preformance è meglio richiedere sempre 32bpp
  // se per caso non è disponibile sulla scheda video viene
  // automaticamaente trovato un altro modo video a bpp inferiore
  global_ib=Open_Screen(miawin, &driver_to_use);
  HANDLE hProc = GetCurrentProcess();
  SetPriorityClass(hProc, HIGH_PRIORITY_CLASS);
  int i;

  if (NoSound == FALSE)
  {
/*
      // inizializzazione FMOD library
      FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
      FSOUND_SetDriver(0);
      FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
      if (!FSOUND_Init(22050, 32, 0))
	  {
         if (!FSOUND_Init(11025, 32, 0))
	        debug_error(miawin, "FMOD init error");
	  }
      //  mod = FMUSIC_LoadSong(SoundFile);
      mod = FMUSIC_LoadSong("Data\\modulo.xm");
      if (!mod) debug_error(miawin, "FMOD error: file del modulo non presente");
*/
    //Initialize digital sound - default device, 44100hz, stereo, 16 bits, no syncs used */
	if (!BASS_Init(-1, 44100, 0, miawin)) 
		debug_error(miawin, "Can't initialize digital sound system");

//	mod=BASS_MusicLoad(FALSE, "Data\\modulo.mo3",
//	                   0, 0, BASS_MUSIC_LOOP);
	mod=BASS_MusicLoad(FALSE, "Data\\ccj-webi.mo3",
	                   0, 0,0/* BASS_MUSIC_LOOP*/);
	if (!mod)
		debug_error(miawin, "Can't load sound file (modulo.mo3)");
  }


/*
  flash.set_imagebuffer(global_ib);
  flash.set_pos(0.7, 1);  // si flasha verso la fine
  flash.init();
*/

/*
  pnoise2D.set_imagebuffer(global_ib);
  pnoise2D.set_pos(0, 1);
  pnoise2D.init();

  pnoise3D.set_imagebuffer(global_ib);
  pnoise3D.set_pos(0, 1);
  pnoise3D.init();
*/
/*
  plasma.set_imagebuffer(global_ib);
  plasma.set_blur(0, 0, 0);
  plasma.set_pos(0, 1);
  plasma.init();
*/
/*
  spline.add_key(110, 90);
  spline.add_key(210, 120);
  spline.add_key(180, 90);
  spline.add_key(130, 190);
  spline.init();
  painter.init();
*/
/*
  blur.set_imagebuffer(global_ib);
//blur.set_imagebuffer_in(global_ib);
  blur.set_pos(0, 1);
  blur.init();
*/
/*
  rainbow.set_imagebuffer(global_ib);
  rainbow.set_pos(0, 1);
  rainbow.init();
*/

  flash.set_imagebuffer(global_ib);

  scena1.set_imagebuffer(global_ib);
  scena1.load_scene("Data\\tastiera");
  scena1.set_pos(0.042, 0.4);
  scena1.init();

  scena2.set_imagebuffer(global_ib);
  scena2.load_scene("Data\\cerv");
  scena2.set_pos(0.4, 0.5);
  scena2.init();

  fadescope.set_imagebuffer(global_ib);
  fadescope.set_pos(0.728, 1.0);
  fadescope.delta_intensity=4;
  fadescope.mode=FADESCOPE2D_ADD;
  fadescope.init();

  hzblur.set_imagebuffer(global_ib);
  hzblur.set_pos(0.042, 0.4);
  hzblur.init();

  endfade.set_imagebuffer(global_ib);
  endfade.set_pos(0.9, 1.0);
  endfade.init();


  levels1.set_imagebuffer(global_ib);
  levels1.set_pos(0.0, 0.1);
  levels1.init();

  blur.set_imagebuffer(global_ib);
  blur.set_pos(0,0.042 );
  blur.init();

  herebbit.set_imagebuffer(global_ib);
  herebbit.set_pos(0.0,1.0);
  herebbit.init();

  backg.load("Data\\tipa.jpg", RGBINTERLEAVED);

  text1.set_imagebuffer(global_ib);
  text1.set_pos(0.31, 0.4);
  text1.set_parameters(1, 200,
                       1, 1, FONT_OVERWRITE, FONT_CENTER_ALIGNED, 0.9);
  text1.add_TXTfile("Data\\testo.txt", 0xFFFFFFFF);
  text1.add_phrase("", 0x00FFFFFF);
  text1.init();

  required.set_imagebuffer(global_ib);
  required.set_pos(0.42, 0.49);
//  required.set_pos(0, 0.3);
  required.set_parameters("Data\\req.jpg", -160, 205, 60,
                           16, 0.001f, QUADRO_ANTIORARIO);
  required.init();
/*
  // i 3 loghi che appiono (webbit, sk, AD) + relativi flash
  logo_webbit.set_imagebuffer(global_ib);
  logo_webbit.set_pos(0, 0.33);
  logo_webbit.set_parameters("Data\\logo_webbit.png", 160, 100, 45,
	                         8, 0.08f, QUADRO_ORARIO);
  logo_webbit.init();

  logo_sk.set_imagebuffer(global_ib);
  logo_sk.set_pos(0.35, 0.66);
  logo_sk.set_parameters("Data\\logo_sk.png", 160, 100, 45,
	                         8, 0.05f, QUADRO_ORARIO);
  logo_sk.init();

  logo_ad.set_imagebuffer(global_ib);
  logo_ad.set_pos(0.70, 0.98);
  logo_ad.set_parameters("Data\\logo_ad.png", 160, 100, 45,
	                         8, 0.05f, QUADRO_ORARIO);
  logo_ad.init();

  for (i=0; i<3; i++) flash_logo[i].set_imagebuffer(global_ib);
  flash_logo[0].set_pos(0.32, 0.34);
  flash_logo[1].set_pos(0.65, 0.67);
  flash_logo[2].set_pos(0.97, 0.99);
  for (i=0; i<3; i++) flash_logo[i].init();


  for (i=0; i<12; i++) quadri_end[i].set_imagebuffer(global_ib);
  quadri_end[0].set_parameters("Data\\musta1.png", 40, 40, 45,
	                           8, 0.05f, QUADRO_ANTIORARIO);

  quadri_end[1].set_parameters("Data\\musta2.png", 120, 40, 45,
	                           8, 0.12f, QUADRO_ANTIORARIO);
  

  what.set_imagebuffer(global_ib);
  what.load("Data\\what.png", RGBINTERLEAVED);
  what.set_pos(0.70, 1);
  what.init();
  is.set_imagebuffer(global_ib);
  is.load("Data\\is.png", RGBINTERLEAVED);
  is.set_pos(0.75, 1);
  is.init();
  demoscene.set_imagebuffer(global_ib);
  demoscene.load("Data\\demoscene.png", RGBINTERLEAVED);
  demoscene.set_pos(0.80, 1);
  demoscene.init();
  p_inter.set_imagebuffer(global_ib);
  p_inter.load("Data\\p_inter.png", RGBINTERLEAVED);
  p_inter.set_pos(0.85, 1);
  p_inter.init();

  for (i=0; i<4; i++) flash[i].set_imagebuffer(global_ib);
  flash[0].set_pos(0.7, 0.71);
  flash[1].set_pos(0.75, 0.76);
  flash[2].set_pos(0.80, 0.81);
  flash[3].set_pos(0.85, 0.86);
  for (i=0; i<4; i++) flash[i].init();


//  rtscene.set_imagebuffer(global_ib);
//  rtscene.set_pos(0, 1);
//  rtscene.load_scene("c:\\3dsmax3_1\\meshes\\rayt");
//  rtscene.load_scene("rayt");
//  rtscene.init();
*/

  init_fast_math_library();
  _flushall();           // per essere sicuri di non desyncare
  global_ib->clear(0);
  demotimer.init(153);    // 90 secondi di demo
  demotimer.start();

//  if (NoSound == FALSE) FMUSIC_PlaySong(mod);
  	if (NoSound == FALSE)
	{
		BASS_Start();
        if (!BASS_MusicPlay(mod))
		debug_error(miawin, "Con't play module");

	     BASS_ChannelSetSync(mod,
	                     BASS_SYNC_MUSICFX/* | BASS_SYNC_ONETIME*/,
                        1,
                        &MySyncProc,
                        0);
	}
// comando E81 per i flash
}


void ADmainLoop() // loop principale
{
	hzblur.window=int(sin(curpos*1000.0)*10.0)+10;

	if ((curpos>0.042)) global_ib->clear(0);
	else memcpyd(global_ib->voidptr, backg.map32, 320*240);

	if ((text1.apply(curpos)) && (curpos<0.5))
		text1.do_efx2D(curpos);

	if ((curpos>0.4) && (curpos<0.5)) 
	{
		text1.do_efx2D(sin(curpos*100.0)*0.5+0.5);
		hzblur.window=3;
		hzblur.do_efx2D(curpos);
	}

		if (required.apply(curpos))
		required.do_efx2D(curpos);


	if ((scena1.apply(curpos)) && (curpos<0.5))
		scena1.do_efx2D(curpos);
	if (scena2.apply(curpos))
		scena2.do_efx2D(curpos);

	if ((curpos>0.6) && (curpos<0.728)) 
	{
		scena1.set_pos(0,1);
		scena1.do_efx2D(sin(curpos*228.0)*0.5+0.5);
	}

	
	if ((curpos>0.728)) 
	{
		hzblur.window=5;
        hzblur.do_efx2D(curpos);
		herebbit.do_efx2D(curpos);
		scena2.set_pos(0,1);
		scena2.do_efx2D(sin(curpos*228.0)*0.5+0.5);
	}

	if ((curpos>0.5) && (curpos<0.6)) 
	{
		text1.set_pos(0,1);
		text1.do_efx2D((curpos-0.5)*10.0);
	}


	if (curpos<0.02) 
	{
		blur.window=int((0.02-curpos)*200.0);
		blur.do_efx2D(curpos);
	}

	if ((curpos>0.535) && (curpos<0.68)) herebbit.do_efx2D(curpos);

	levels1.mode=LEVELS_MANUAL;
	levels1.rmax=levels1.gmax=levels1.bmax=int(levels1.get_interpos(curpos)*255.0*3.0)+1;
	if (levels1.rmax>255) levels1.rmax=levels1.gmax=levels1.bmax=255;

	if (levels1.apply(curpos)) levels1.do_efx2D(curpos);

	if (hzblur.apply(curpos)) hzblur.do_efx2D(curpos);
/*
	if (curpos>0.9) 
	{
		levels1.mode=LEVELS_MANUAL;
		levels1.rmin=levels1.gmin=levels1.bmin=255-int(levels1.get_interpos(curpos)*255.0);
		if (levels1.rmin<0) levels1.rmin=levels1.gmin=levels1.bmin=0;

		levels1.do_efx2D(curpos);
	}*/

	if ((flash.apply(curpos)) && flashflag)
		flash.do_efx2D(curpos);

	fadescope.intensity=int(sin(curpos*700.0)*50.0+127.0);
	if (fadescope.apply(curpos)) fadescope.do_efx2D(curpos);

	if (endfade.apply(curpos)) endfade.do_efx2D(curpos);

	demotimer.step();
	curpos=demotimer.get_pos();
    Update_Screen();
}


void ADmainExit() // codice finale
{
  char *str=new char[100];
  int dec, sign;

  demotimer.stop();
  demotimer_end.stop();

  Close_Screen();
  if (NoSound == FALSE)
  {
     //FMUSIC_FreeSong(mod);
     //FSOUND_Close();
     BASS_MusicFree(mod);
	 BASS_Stop();
	 BASS_Free();
  }

  str=fcvt(demotimer.get_stepsPerSecond(), 4, &dec, &sign);
  str[dec+1]=str[dec];
  str[dec]='.';
  str[dec+2]=0;
  strcat(str," fps");
  debug_error(miawin, str);
}

#endif