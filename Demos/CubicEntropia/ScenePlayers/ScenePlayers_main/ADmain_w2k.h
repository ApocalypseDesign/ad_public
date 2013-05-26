// codice principale della demo
// -------------------------------
// AD Demo System 99 - ADmain.h
// -------------------------------

#ifndef _ADMAIN_H_
#define _ADMAIN_H_

#include "dtimer.h"
#include "fmod.h"
#include "Efx2D\bump2D.h"
#include "Efx2D\flash2D.h"
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
FMUSIC_MODULE *mod;
imagebuffer *global_ib;
imagebuffer *ib_aux;
BOOL NoSound;

char c64_1[31], c64_2[31], c64_3[31];
char c64_4[31], c64_5[31], c64_6[31];
char c64_7[31], c64_8[31], c64_9[31];
char c64_10[31], c64_11[31], c64_12[31];


bump2D     bump;
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
AD_Sprite barra1, barra2;
AD_Sprite what, is, demoscene, p_inter;
flash2D flash[4], flash_logo[3];
quadro2D  quadri_end[12], logo_webbit, logo_sk, logo_ad;
AD_Scene3D  scena;
TextScroller2D text1, text2, text3;

#define SCENA_LOOP 6
#define TEXT_LOOP 7
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

  ib_aux=new imagebuffer(global_ib->width, global_ib->height);
  ib_aux->clear(0);

/*
  rubbing.set_imagebuffer(ib_aux);
  rubbing.set_imageout(global_ib);
  rubbing.set_pos(0, 1);
  rubbing.init();
*/
//  mosaic.set_imagebuffer(global_ib);
//  mosaic.set_pos(0, 0.08);
//  mosaic.init();
/*
  sniper.set_imagebuffer(global_ib);
  sniper.set_pos(0, 1);
  sniper.init();
*/
//  plasma.set_imagebuffer(global_ib);
  //plasma.set_blur(0, 0, 0);
//  plasma.set_pos(0, 1);
//  plasma.init();

//  backg.load_jpg("Data\\webbit.jpg", RGBINTERLEAVED);
/*
  tomato.set_imagebuffer(global_ib);
  tomato.set_pos(0, 1);
  tomato.init();
*/


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
  barra1.set_imagebuffer(global_ib);
  barra1.load("Data\\barra1.jpg", RGBINTERLEAVED);
  barra1.set_pos(0, 1);
  barra1.init();

  barra2.set_imagebuffer(global_ib);
  barra2.load("Data\\barra2.jpg", RGBINTERLEAVED);
  barra2.set_pos(0, 1);
  barra2.init();

  bump.set_imagebuffer(ib_aux);
  bump.set_pos(0, 1);
//  bump.set_height_map("Data\\bumpa.jpg");
  bump.set_light_map("Data\\light1.jpg");
  bump.set_height_map(global_ib);
  bump.init();

  scena.set_imagebuffer(global_ib);
  scena.load_scene("Data\\webbit");
  scena.set_pos(0, 1);
  scena.init();

//  text.set_imagebuffer(ib_aux);
  text1.set_imagebuffer(ib_aux);
  text1.set_pos(0, 0.25);
  text1.set_parameters(barra1.dimy, ib_aux->height-barra2.dimy,
                       0, 0, FONT_OVERWRITE, FONT_RIGHT_ALIGNED, 0.05);

  for (i=0; i<30; i++) c64_1[i]=(char)(i);
  c64_1[30]='\0';
  for (i=0; i<30; i++) c64_2[i]=(char)(i+30);
  c64_2[30]='\0';
  for (i=0; i<30; i++) c64_3[i]=(char)(i+60);
  c64_3[30]='\0';
  for (i=0; i<30; i++) c64_4[i]=(char)(i+90);
  c64_4[30]='\0';
  for (i=0; i<30; i++) c64_5[i]=(char)(i+120);
  c64_5[30]='\0';
  for (i=0; i<30; i++) c64_6[i]=(char)(i+150);
  c64_6[30]='\0';
  for (i=0; i<30; i++) c64_7[i]=(char)(i+180);
  c64_7[30]='\0';
  for (i=0; i<30; i++) c64_8[i]=(char)(i+210);
  c64_8[30]='\0';
  for (i=0; i<16; i++) c64_9[i]=(char)(i+240);
  c64_9[16]='\0';

  text1.add_phrase("jxxxxxxxxxxxxxxxxxxxe",  0x00FFFFFF);
  text1.add_phrase("j APOCALYPSE DESIGN e",  0x00DDEEFF);
  text1.add_phrase("j  INVITE YOU TO :  e",  0x00CCEEFF);
  text1.add_phrase("j ----------------- e",  0x00BBCCEE); 
  text1.add_phrase("j * WEBB.IT  2K+1 * e",  0x00AACCEE);
  text1.add_phrase("jPADOVA, 25-30.07.01e",  0x0099AAEE);
  text1.add_phrase("jHTTP:// WWW.WEBB.ITe",  0x008899DD);
  text1.add_phrase("j ----------------- e",  0x007788DD);
  text1.add_phrase("jDEMOSCENE  COMUNITYe",  0x006677CC);
  text1.add_phrase("jIS WAITING  FOR YOUe",  0x005566CC);
  text1.add_phrase("j AT CAMPUS SECTION e",  0x004455BB);
  text1.add_phrase("jYELD INSIDE  WEBBITe",  0x003344BB);
  text1.add_phrase("j       JOIN US !!! e",  0x002233AA);
  text1.add_phrase("jyyyyyyyyyyyyyyyyyyye",  0x001122AA);
  text1.add_phrase("", 0x00FFFFFF);
  text1.add_phrase("", 0x00FFFFFF);
/*  text1.add_phrase(c64_1, 0x00FFFFFF);
  text1.add_phrase(c64_2, 0x00FFFFFF);
  text1.add_phrase(c64_3, 0x00FFFFFF);
  text1.add_phrase(c64_4, 0x00FFFFFF);
  text1.add_phrase(c64_5, 0x00FFFFFF);
  text1.add_phrase(c64_6, 0x00FFFFFF);
  text1.add_phrase(c64_7, 0x00FFFFFF);
  text1.add_phrase(c64_8, 0x00FFFFFF);
  text1.add_phrase(c64_9, 0x00FFFFFF);
  text1.add_phrase("", 0x00FFFFFF);
*/
  text1.init();


  text2.set_imagebuffer(ib_aux);
  text2.set_pos(0.33, 0.58);
  text2.set_parameters(barra1.dimy, ib_aux->height-barra2.dimy,
                       0, 0, FONT_OVERWRITE, FONT_LEFT_ALIGNED, 0.05);
  text2.add_phrase("jxxxxxxxxxxxxxxxxxxxe",  0x00FFFFFF);
  text2.add_phrase("j  DEMOSCENE EVENT  e",  0x00FFDDEE);
  text2.add_phrase("j WILL BE ORGANIZED e",  0x00FFCCEE);
  text2.add_phrase("j BY SPINING KIDS!! e",  0x00EEBBCC); 
  text2.add_phrase("j ----------------- e",  0x00EEAACC);
  text2.add_phrase("jJOIN THE FAST COMPOe",  0x00EE99AA);
  text2.add_phrase("j AND STAY TUNED !! e",  0x00DD8899);
  text2.add_phrase("j ----------------- e",  0x00DD7788);
  text2.add_phrase("j COORDINATES ARE : e",  0x00CC6677);
  text2.add_phrase("j 25/07/01 AT 11:00 e",  0x00CC5566);
  text2.add_phrase("jPRENOTE YOUR TICKETe",  0x00BB4455);
  text2.add_phrase("j NOW, JUST VISIT : e",  0x00BB3344);
  text2.add_phrase("jHTTP:// WWW.WEBB.ITe",  0x00AA2233);
  text2.add_phrase("jyyyyyyyyyyyyyyyyyyye",  0x00AA1122);
  text2.add_phrase("", 0x00FFFFFF);
  text2.add_phrase("", 0x00FFFFFF);
  text2.init();

  text3.set_imagebuffer(ib_aux);
  text3.set_pos(0.66, 0.91);
  text3.set_parameters(barra1.dimy, ib_aux->height-barra2.dimy,
                       0, 0, FONT_OVERWRITE, FONT_RIGHT_ALIGNED, 0.05);
  text3.add_phrase("jxxxxxxxxxxxxxxxxxxxe",  0x00FFFFFF);
  text3.add_phrase("j APOCALYPSE DESIGN e",  0x00EEFFDD);
  text3.add_phrase("j TEAM, GREETS JUST e",  0x00EEFFCC);
  text3.add_phrase("j ITSELF, OUR MUM & e",  0x00CCEEBB); 
  text3.add_phrase("j  OUR BITCHES !!!  e",  0x00CCEEAA);
  text3.add_phrase("j ----------------- e",  0x00AAEE99);
  text3.add_phrase("j :DESIGN:  SKA     e",  0x0099DD88);
  text3.add_phrase("j :CODE: TURBO,HERE e",  0x0088DD77);
  text3.add_phrase("j :MUSIC: CICILLEJU e",  0x0077CC66);
  text3.add_phrase("j :END PIC: MUSTAFA e",  0x0066CC55);
  text3.add_phrase("j ----------------- e",  0x0055BB44);
  text3.add_phrase("j FUCK  ALL ITALIAN e",  0x0044BB33);
  text3.add_phrase("j   SCENERS !!!!!!  e",  0x0033AA22);
  text3.add_phrase("jyyyyyyyyyyyyyyyyyyye",  0x0022AA11);
  text3.add_phrase("", 0x00FFFFFF);
  text3.add_phrase("", 0x00FFFFFF);
  text3.init();


  // i 3 loghi che appiono (webbit, sk, AD) + relativi flash
  logo_webbit.set_imagebuffer(ib_aux);
  logo_webbit.set_pos(0, 0.20);
  logo_webbit.set_parameters("Data\\logo_webbit.png", 78, 103, 45,
	                         8, 0.08f, QUADRO_ORARIO);
  logo_webbit.init();

  logo_sk.set_imagebuffer(ib_aux);
  logo_sk.set_pos(0.33, 0.53);
  logo_sk.set_parameters("Data\\logo_sk.png", 243, 103, 45,
	                         8, 0.05f, QUADRO_ORARIO);
  logo_sk.init();

  logo_ad.set_imagebuffer(ib_aux);
  logo_ad.set_pos(0.66, 0.86);
  logo_ad.set_parameters("Data\\logo_ad.png", 80, 103, 45,
	                         8, 0.05f, QUADRO_ORARIO);
  logo_ad.init();

  for (i=0; i<3; i++) flash_logo[i].set_imagebuffer(ib_aux);
  flash_logo[0].set_pos(0.249, 0.252);
  flash_logo[1].set_pos(0.579, 0.582);
  flash_logo[2].set_pos(0.909, 0.912);
  for (i=0; i<3; i++) flash_logo[i].init();


  for (i=0; i<12; i++) quadri_end[i].set_imagebuffer(global_ib);
  quadri_end[0].set_parameters("Data\\musta1.png", 40, 40, 45,
	                           8, 0.05f, QUADRO_ANTIORARIO);

  quadri_end[1].set_parameters("Data\\musta2.png", 120, 40, 45,
	                           8, 0.12f, QUADRO_ANTIORARIO);
  
  quadri_end[2].set_parameters("Data\\musta3.png", 200, 40, 45,
	                           8, 0.06f, QUADRO_ANTIORARIO);

  quadri_end[3].set_parameters("Data\\musta4.png", 280, 40, 45,
	                           8, 0.10f, QUADRO_ANTIORARIO);
  
  quadri_end[4].set_parameters("Data\\musta5.png", 40, 120, 45,
	                           8, 0.15f, QUADRO_ORARIO);

  quadri_end[5].set_parameters("Data\\musta6.png", 120, 120, 45,
	                           8, 0.04f, QUADRO_ANTIORARIO);
  
  quadri_end[6].set_parameters("Data\\musta7.png", 200, 120, 45,
	                           8, 0.09f, QUADRO_ORARIO);

  quadri_end[7].set_parameters("Data\\musta8.png", 280, 120, 45,
	                           8, 0.13f, QUADRO_ORARIO);
  
  quadri_end[8].set_parameters("Data\\musta9.png", 40, 200, 45,
	                           8, 0.11f, QUADRO_ORARIO);

  quadri_end[9].set_parameters("Data\\musta10.png", 120, 200, 45,
	                           8, 0.07f, QUADRO_ORARIO);
  
  quadri_end[10].set_parameters("Data\\musta11.png", 200, 200, 45,
	                           8, 0.14f, QUADRO_ANTIORARIO);

  quadri_end[11].set_parameters("Data\\musta12.png", 280, 200, 45,
	                           8, 0.05f, QUADRO_ORARIO);

  quadri_end[5].set_pos(0.05, 1);
  quadri_end[11].set_pos(0.10, 1);
  quadri_end[2].set_pos(0.15, 1);
  quadri_end[9].set_pos(0.20, 1);
  quadri_end[0].set_pos(0.25, 1);
  quadri_end[6].set_pos(0.30, 1);
  quadri_end[3].set_pos(0.35, 1);
  quadri_end[8].set_pos(0.40, 1);
  quadri_end[1].set_pos(0.45, 1);
  quadri_end[7].set_pos(0.50, 1);
  quadri_end[10].set_pos(0.55, 1);
  quadri_end[4].set_pos(0.60, 1);
  for (i=0; i<12; i++) quadri_end[i].init();  

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

  init_fast_math_library();
  _flushall();           // per essere sicuri di non desyncare
  global_ib->clear(0);
  demotimer.init(90);    // 90 secondi di demo
  demotimer.start();

  if (NoSound == FALSE) FMUSIC_PlaySong(mod);
}


void ADmainLoop() // loop principale
{
  double scena_pos;
  int i;

//  global_ib->clear(0);

    memsetd(global_ib->uint32ptr+barra1.dimy*global_ib->width,
		    0, 
			(global_ib->height-barra1.dimy-barra2.dimy)*global_ib->width);

	//  ib_aux->clear(0);
//    memcpy(global_ib->voidptr, backg.map, global_ib->channelsize*4);
//  ib_aux->clear(0);

/*
    if (flash.apply(curpos))
		flash.do_efx2D(curpos);
    if (pnoise2D.apply(curpos))
		pnoise2D.do_efx2D(curpos);
*/
/*
    if (pnoise3D.apply(curpos))
		pnoise3D.do_efx2D(curpos);
*/
/*
    if (plasma.apply(curpos))
		plasma.do_efx2D(curpos);

    if (bump.apply(curpos))
		bump.do_efx2D(curpos);
*/
/*
	spline.rotate_key(160, 130, curpos*1440.0);
	painter.draw_spline2D(&spline, 20,
		                 155, 100, 255,
						 global_ib);
*/
//    if (rainbow.apply(curpos))
//		rainbow.do_efx2D(curpos);
//    if (plasma.apply(curpos))
//		plasma.do_efx2D(0.4);
/*
	if (mosaic.apply(curpos))
		mosaic.do_efx2D(curpos);
*/
//    if (rtscene.apply(curpos))
//		rtscene.do_efx2D(curpos);
//	if (tomato.apply(curpos))
//		tomato.do_efx2D(curpos);

//    if (scena.apply(curpos))
	{
	   scena_pos=curpos*SCENA_LOOP;
	   scena_pos=scena_pos-(float)((int)(scena_pos));
	   scena.do_efx2D(scena_pos);
	}
    if (bump.apply(curpos))
		bump.do_efx2D(curpos);

    // barra1.put(0, 0, global_ib);

    if (text1.apply(curpos)) text1.do_efx2D(curpos);
    if (text2.apply(curpos)) text2.do_efx2D(curpos);
    if (text3.apply(curpos)) text3.do_efx2D(curpos);

	if (logo_webbit.apply(curpos)) logo_webbit.do_efx2D(curpos);
	if (logo_sk.apply(curpos)) logo_sk.do_efx2D(curpos);
	if (logo_ad.apply(curpos)) logo_ad.do_efx2D(curpos);

	for (i=0; i<3; i++)
	  if (flash_logo[i].apply(curpos))  flash_logo[i].do_efx2D(curpos);


	memcpyd(global_ib->uint32ptr, barra1.map32, barra1.channelsize);

    memcpyd(
		     global_ib->uint32ptr+barra1.dimy*global_ib->width,
		     ib_aux->uint32ptr+barra1.dimy*ib_aux->width,
			 (global_ib->height-barra1.dimy-barra2.dimy)*global_ib->width
		   );

    // barra2.put(0, global_ib->height-barra2.dimy, global_ib);
	memcpyd(global_ib->uint32ptr+(global_ib->channelsize-barra2.channelsize),
		    barra2.map32, barra2.channelsize);

	demotimer.step();
	curpos=demotimer.get_pos();
    Update_Screen();
}


void AD_PreExit()
{
  double cpos=0.0;
  double scena_pos;
  int i;
  float master_vol;

  master_vol=(float)(FMUSIC_GetMasterVolume(mod));

  demotimer_end.init(16);    // 16 secondi di fine demo
  demotimer_end.start();  
  while (cpos<1)
  {
    FMUSIC_SetMasterVolume(mod, (int)((1.0f-cpos)*master_vol));
	// il timer normale procede con la sua velocità eventualmente
	// loopando
	demotimer.step();
	curpos=demotimer.get_pos();
	if (curpos>=1.0)
	{
	   curpos=0;
       demotimer.init(90);
       demotimer.start();
    }

    memsetd(global_ib->uint32ptr+barra1.dimy*global_ib->width,
		    0, 
			(global_ib->height-barra1.dimy-barra2.dimy)*global_ib->width);
    scena_pos=curpos*SCENA_LOOP;
    scena_pos=scena_pos-(float)((int)(scena_pos));
	scena.do_efx2D(scena_pos);
    if (bump.apply(curpos))
		bump.do_efx2D(curpos);

    if (text1.apply(curpos)) text1.do_efx2D(curpos);
    if (text2.apply(curpos)) text2.do_efx2D(curpos);
    if (text3.apply(curpos)) text3.do_efx2D(curpos);
    //text_pos=curpos*TEXT_LOOP;
    //text.do_efx2D(text_pos);

	if (logo_webbit.apply(curpos)) logo_webbit.do_efx2D(curpos);
	if (logo_sk.apply(curpos)) logo_sk.do_efx2D(curpos);
	if (logo_ad.apply(curpos)) logo_ad.do_efx2D(curpos);

	memcpyd(global_ib->uint32ptr, barra1.map32, barra1.channelsize);
    memcpyd(
		     global_ib->uint32ptr+barra1.dimy*global_ib->width,
		     ib_aux->uint32ptr+barra1.dimy*ib_aux->width,
			 (global_ib->height-barra1.dimy-barra2.dimy)*global_ib->width
		   );
	memcpyd(global_ib->uint32ptr+(global_ib->channelsize-barra2.channelsize),
		    barra2.map32, barra2.channelsize);

    if (quadri_end[5].apply(cpos)) quadri_end[5].do_efx2D(cpos);
    if (quadri_end[11].apply(cpos)) quadri_end[11].do_efx2D(cpos);
    if (quadri_end[2].apply(cpos)) quadri_end[2].do_efx2D(cpos);
    if (quadri_end[9].apply(cpos)) quadri_end[9].do_efx2D(cpos);
    if (quadri_end[0].apply(cpos)) quadri_end[0].do_efx2D(cpos);
    if (quadri_end[6].apply(cpos)) quadri_end[6].do_efx2D(cpos);
    if (quadri_end[3].apply(cpos)) quadri_end[3].do_efx2D(cpos);
    if (quadri_end[8].apply(cpos)) quadri_end[8].do_efx2D(cpos);
    if (quadri_end[1].apply(cpos)) quadri_end[1].do_efx2D(cpos);
    if (quadri_end[7].apply(cpos)) quadri_end[7].do_efx2D(cpos);
    if (quadri_end[10].apply(cpos)) quadri_end[10].do_efx2D(cpos);
    if (quadri_end[4].apply(cpos)) quadri_end[4].do_efx2D(cpos);

	if (what.apply(cpos)) what.put_add(20, 27, global_ib);
	if (is.apply(cpos)) is.put_add(150, 55, global_ib);
	if (demoscene.apply(cpos)) demoscene.put_add(10, 90, global_ib);
	if (p_inter.apply(cpos)) p_inter.put_add(235, 150, global_ib);

	for (i=0; i<4; i++)
	  if (flash[i].apply(cpos))	flash[i].do_efx2D(cpos);
		  ;
	demotimer_end.step();
	cpos=demotimer_end.get_pos();
    Update_Screen();
  }

  PostMessage(miawin, WM_CLOSE, 0, 0);
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
     FMUSIC_FreeSong(mod);
     FSOUND_Close();
  }

  str=fcvt(demotimer.get_stepsPerSecond(), 4, &dec, &sign);
  str[dec+1]=str[dec];
  str[dec]='.';
  str[dec+2]=0;
  strcat(str," fps");
  debug_error(miawin, str);
}

#endif