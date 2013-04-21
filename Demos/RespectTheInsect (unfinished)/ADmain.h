// codice principale della demo
// -------------------------------
// AD Demo System 99 - ADmain.h
// -------------------------------

#ifndef _ADMAIN_H_
#define _ADMAIN_H_

#include "dtimer.h"
//#include "fmod.h"
#include "Efx2D\herebbit2D.h"
#include "Efx2D\bump2D.h"
#include "Efx2D\flash2D.h"
#include "Efx2D\fadescope2D.h"
#include "Efx2D\hzblur2D.h"
#include "Efx2D\lowpass2D.h"
#include "Efx2D\hipass2D.h"
#include "Efx2D\hiboost2D.h"
#include "Efx2D\levels2D.h"
#include "Efx2D\zbuf2D.h"
#include "Efx2D\herox.h"
#include "Efx2D\weather2D.h"
#include "Efx2D\plasm2D.h"
#include "Efx2D\painter2D.h"
#include "Efx2D\spline.h"
#include "Efx2D\noise.h"
#include "Efx2D\blur2D.h"
#include "Efx2D\tomato2D.h"
#include "Efx2D\postfilters2D.h"
#include "Efx2D\TextScroller2D.h"
#include "Efx2D\quadro2D.h"
#include "Efx2D\sprite.h"
#include "Engines\scene3d.h"
#include "Engines\Raytracing\rayt_scene.h"
#include "Math\fastmath.h"

//-----------------------------------------------------------
// VARIABILI GLOBALI DELLA DEMO
dtimer demotimer;
dtimer demotimer_end;
//FMUSIC_MODULE *mod;
imagebuffer *global_ib;
AD_Sprite img1,img2,img3,img4;
BOOL NoSound;

char c64_1[31], c64_2[31], c64_3[31];
char c64_4[31], c64_5[31], c64_6[31];
char c64_7[31], c64_8[31], c64_9[31];
char c64_10[31], c64_11[31], c64_12[31];

//zbuf2D zbuf;
//hzblur2D hzblur;
//herebbit2D herebbit;
lowpass2D lowpass;
//hipass2D  hipass;
//hiboost2D hiboost;
//levels2D  levels;
//weather2D  weather; 
//bump2D     bump;
//herox	   herox1;
//fadescope2D fadescope;
//tomato2D	painter1;
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
//texture backg;
/*
AD_Sprite barra1, barra2;
AD_Sprite what, is, demoscene, p_inter;
flash2D flash[4], flash_logo[3];
quadro2D  quadri_end[12], logo_webbit, logo_sk, logo_ad;
*/
//AD_Scene3D  scena;
//TextScroller2D text1, text2, text3;

//#define SCENA_LOOP 6
//#define TEXT_LOOP 7
//-----------------------------------------------------------

double relpos(double start, double end, double interpos)
{ return(interpos*(end-start)+start); }


void ADmainEnter() //codice iniziale
{
  // per ragioni di preformance è meglio richiedere sempre 32bpp
  // se per caso non è disponibile sulla scheda video viene
  // automaticamaente trovato un altro modo video a bpp inferiore
  global_ib=Open_Screen(miawin, &driver_to_use);
  HANDLE hProc = GetCurrentProcess();
  SetPriorityClass(hProc, HIGH_PRIORITY_CLASS);
  int i;
/*
  if (NoSound == FALSE)
  {
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
  }
  */
  
  float global_scaling = max(driver_to_use.WantedMode.Width / 320, driver_to_use.WantedMode.Height / 240);

  img1.set_imagebuffer(global_ib);
  img1.load("data\\ok1.jpg",RGBINTERLEAVED);
  img1.set_render_type(RENDER_SCALING | RENDER_CLIP 
	                   | RENDER_OVERWRITE); 
  img1.add_key(0,0,0,		global_scaling, global_scaling);
  img1.add_key(1,0,-202,	global_scaling, global_scaling);
  img1.set_pos(0.0f,0.2f);
  img1.init();

  img2.set_imagebuffer(global_ib);
  img2.load("data\\ok2.jpg",RGBINTERLEAVED);
  img2.set_render_type(RENDER_SCALING | RENDER_CLIP 
	                   | RENDER_OVERWRITE); 
  img2.add_key(0,0,0,		global_scaling, global_scaling);
  img2.set_pos(0.2f,0.4f);
  img2.init();

  img3.set_imagebuffer(global_ib);
  img3.load("data\\ok3.jpg",RGBINTERLEAVED);
  img3.set_render_type(RENDER_SCALING | RENDER_CLIP 
	                   | RENDER_OVERWRITE); 
  img3.add_key(0,0,0,		1. * global_scaling, 1. * global_scaling);
  img3.add_key(0.5,80,0,	1. * global_scaling, 1. * global_scaling);
  img3.add_key(1,-260,-150,	4. * global_scaling, 4. * global_scaling);
  img3.set_pos(0.4f,0.6f);
  img3.init();

  img4.set_imagebuffer(global_ib);
  img4.load("data\\ok4.jpg",RGBINTERLEAVED);
  img4.set_render_type(RENDER_SCALING | RENDER_CLIP 
	                   | RENDER_OVERWRITE); 

  img4.add_key(0,0,0,			0.5614 * global_scaling, 0.625 * global_scaling);
  img4.add_key(0.3,-1,-1,		1. * global_scaling, 1. * global_scaling);
  img4.add_key(0.4,-150,0,		1. * global_scaling, 1. * global_scaling);
  img4.add_key(0.5,-50,-80,		1. *global_scaling, 1. * global_scaling);
  img4.add_key(0.6,-150,-100,	1. * global_scaling, 1. * global_scaling);
  img4.add_key(0.8,0,0,			0.5614 * global_scaling, 0.625 * global_scaling);
  img4.add_key(1,0,0,			0.5614 * global_scaling, 0.625 * global_scaling);
  img4.set_pos(0.6f,1.0f);
  img4.init();


  lowpass.set_imagebuffer(global_ib);
  lowpass.init();

  init_fast_math_library();
  _flushall();           // per essere sicuri di non desyncare
  global_ib->clear(0);
  demotimer.init(20);    // secondi di demo
  demotimer.start();

//  if (NoSound == FALSE) FMUSIC_PlaySong(mod);
}


void ADmainLoop() // loop principale
{
	double scena_pos;
	int i;

	global_ib->clear(0,0,0);



	if (img1.apply(curpos)) img1.do_efx2D(curpos);
	if (img2.apply(curpos)) img2.do_efx2D(curpos);
	if (img3.apply(curpos)) img3.do_efx2D(curpos);
	if (img4.apply(curpos)) img4.do_efx2D(curpos);
	//if (lowpass.apply(curpos)) lowpass.do_efx2D(curpos);
	

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
//     FMUSIC_FreeSong(mod);
//     FSOUND_Close();
  }

  /*
  str=fcvt(demotimer.get_stepsPerSecond(), 4, &dec, &sign);
  str[dec+1]=str[dec];
  str[dec]='.';
  str[dec+2]=0;
  strcat(str," fps");
  debug_error(miawin, str);
  */
}

#endif