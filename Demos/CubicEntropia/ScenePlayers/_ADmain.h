// codice principale della demo
// -------------------------------
// AD Demo System 99 - ADmain.h
// -------------------------------

#ifndef _ADMAIN_H_
#define _ADMAIN_H_

#include "fmod.h"
#include "dtimer.h"
#include "Scene3d.h"
#include "TexFont.h"
#include "Fade2D.h"
#include "Flash2D.h"
#include "FastMath.h"


FILE *fdeb;

CTimer demotimer;
CScene3D scena;
CScene3D scena2;
float4 l_BlurLevel=0;
int32 l_BlurType=NOBLUR;
int32 DoScena1=0, DoScena2=0;
BOOL NoSound;
FMUSIC_MODULE *mod;
CGraphicSurface *global_ib;

CFont *font=(CFont *)NULL;
CFade2D Fade2D;
CFlash2D Flash2D;

// la funzione restituisce la posizione assoluta dati i
// range assoluti dell'intervallo (es. SCENA1_START e
// SCENA1_END) e la posizione relativa (interpos, che
// e' sempre tra [0;1]) all'interno di quell'intervallo
double relpos(double start, double end, double interpos)
{ return(interpos*(end-start)+start); }


void ADmainEnter() //codice iniziale
{
    // per ragioni di preformance è meglio richiedere sempre 32bpp
    // se per caso non è disponibile sulla scheda video viene
    // automaticamaente trovato un altro modo video a bpp inferiore

  init_fast_math_library();
  RenderLib.m_OpenScreen(miawin, ScrMode, RunFullscreen);
  RenderLib.m_SetBlur(l_BlurType | BLUR_ADD, l_BlurLevel);

  HANDLE hProc = GetCurrentProcess();
  SetPriorityClass(hProc, NORMAL_PRIORITY_CLASS);

/*
  // inizializzazione FMOD library
  FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
  FSOUND_SetDriver(0);  // driver di default
  FSOUND_SetMixer(FSOUND_MIXER_AUTODETECT);
  if (!FSOUND_Init(44100, 32, 0))
  {
    if (!FSOUND_Init(22050, 32, 0))
	  debug_error(miawin, "FMOD init error");
  }
  mod = FMUSIC_LoadSong(SoundFile);
  if (!mod) debug_error(miawin, "FMOD error: file del modulo non presente");
*/

  fdeb=fopen("SceneLog.txt", "wt");

  if (strcmp(FileToPlay, "")!=0)
  {
    scena.m_SetWorkBuffer(global_ib);
    scena.m_SetRenderLib(&RenderLib);
    scena.m_Load(FileToPlay);
    scena.m_SetPos(0, 1);
    scena.m_Init();
	DoScena1=1;
  }
  font=new CFont("Ren & Stimpy", 80, 0);
  DumpDebugPoint (fdeb, "Font allocated", 0);
  font->m_SetWorkBuffer(global_ib);
  DumpDebugPoint (fdeb, "Font m_SetWorkBuffer done", 0);
  font->m_SetRenderLib(&RenderLib);
  DumpDebugPoint (fdeb, "Font m_SetWorkBuffer done", 0);
  font->m_SetPos(0, 1);
  DumpDebugPoint (fdeb, "Font m_SetWorkBuffer done", 0);

  float4 h=1.0f/54.0f;
  float4 y=-1.0f/8.0f;
/*
  font->m_AddPhrase(0*h,  1*h,  0.5f, 0.03f,  "3state",      0.30f,  -8*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(1*h,  2*h,  0.5f, 0.03f,  "Ardbei",      0.30f,  -7*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(2*h,  3*h,  0.5f, 0.03f,  "blasphemy",   0.30f,  -6*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(3*h,  4*h,  0.5f, 0.03f,  "bomb",        0.30f,  -5*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(4*h,  5*h,  0.5f, 0.03f,  "byterapers",  0.30f,  -4*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(5*h,  6*h,  0.5f, 0.03f,  "calodox",     0.30f,  -3*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(6*h,  7*h,  0.5f, 0.03f,  "cocoon",      0.30f,  -2*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(7*h,  8*h,  0.5f, 0.03f,  "coma",        0.30f,  -1*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(8*h,  9*h,  0.5f, 0.03f,  "complex",     0.30f,   0*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(9*h,  10*h, 0.5f, 0.03f,  "exceed",      0.30f,   1*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(10*h, 11*h,  0.5f, 0.03f,  "farbrausch", 0.30f,   2*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(11*h, 12*h, 0.5f, 0.03f,  "fudge",       0.30f,   3*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(12*h, 13*h, 0.5f, 0.03f,  "haujobb",     0.30f,   4*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(13*h, 14*h, 0.5f, 0.03f,  "kolor",       0.30f,   5*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(14*h, 15*h, 0.5f, 0.03f,  "mandula",     0.30f,   6*y, 0xffffffff, 0.1f, 0.1f);
  font->m_AddPhrase(15*h, 16*h, 0.5f, 0.03f,  "matrix",      0.30f,   7*y, 0xffffffff, 0.1f, 0.1f);
*/
  font->m_AddPhrase(0*h,  18*h, 0.1f, 0.08f,  "Aardbei",      0.00f,  -5.500*y, 0x55aa4433, 0.1f, 0.1f);
  font->m_AddPhrase(1*h,  19*h, 0.1f, 0.08f,  "Blasphemy",   -0.21f,  -3.875*y, 0x55bb5544, 0.1f, 0.1f);
  font->m_AddPhrase(2*h,  20*h, 0.1f, 0.08f,  "Bomb",         0.19f,  -2.250*y, 0x55bb6655, 0.1f, 0.1f);
  font->m_AddPhrase(3*h,  21*h, 0.1f, 0.08f,  "Byterapers",  -0.30f,  -0.625*y, 0x55bb7766, 0.1f, 0.1f);
  font->m_AddPhrase(4*h,  22*h, 0.1f, 0.08f,  "Calodox",     -0.03f,   1.000*y, 0x55cc8877, 0.1f, 0.1f);
  font->m_AddPhrase(5*h,  23*h, 0.1f, 0.08f,  "Cocoon",      -0.03f,   2.625*y, 0x55cc9988, 0.1f, 0.1f);
  font->m_AddPhrase(6*h,  24*h, 0.1f, 0.08f,  "Coma",         0.30f,   4.250*y, 0x55ddaa88, 0.1f, 0.1f);
  font->m_AddPhrase(7*h,  25*h, 0.1f, 0.08f,  "Complex",      0.02f,   5.875*y, 0x55eebb99, 0.1f, 0.1f);
  font->m_AddPhrase(8*h,  26*h, 0.1f, 0.08f,  "Exceed",       0.11f,   7.500*y, 0x55ffcc99, 0.1f, 0.1f);

  font->m_AddPhrase(9*h,  27*h, 0.1f, 0.08f,  "Faurbraush",  -0.90f,  -5.500*y, 0x55ffcc99, 0.1f, 0.1f);
  font->m_AddPhrase(10*h, 28*h, 0.1f, 0.08f,  "Fudge",       -0.90f,  -3.875*y, 0x55eebb99, 0.1f, 0.1f);
  font->m_AddPhrase(11*h, 29*h, 0.1f, 0.08f,  "Haujobb",     -0.90f,  -2.250*y, 0x55ddaa88, 0.1f, 0.1f);
  font->m_AddPhrase(12*h, 30*h, 0.1f, 0.08f,  "Kolor",       -0.90f,  -0.625*y, 0x55cc9988, 0.1f, 0.1f);
  font->m_AddPhrase(13*h, 31*h, 0.1f, 0.08f,  "Mandula",     -0.90f,   1.000*y, 0x55cc8877, 0.1f, 0.1f);
  font->m_AddPhrase(14*h, 32*h, 0.1f, 0.08f,  "Matrix",      -0.90f,   2.625*y, 0x55bb7766, 0.1f, 0.1f);
  font->m_AddPhrase(15*h, 33*h, 0.1f, 0.08f,  "Moppi",       -0.90f,   4.250*y, 0x55bb6655, 0.1f, 0.1f);
  font->m_AddPhrase(16*h, 34*h, 0.1f, 0.08f,  "Noice",       -0.90f,   5.875*y, 0x55bb5544, 0.1f, 0.1f);
  font->m_AddPhrase(17*h, 35*h, 0.1f, 0.08f,  "Nomad",       -0.90f,   7.500*y, 0x55aa4433, 0.1f, 0.1f);


  font->m_AddPhrase(18*h, 36*h, 0.1f, 0.08f,  "Orange",            0.0f,  -5.500*y, 0x55aa4433, 0.1f, 0.1f);
  font->m_AddPhrase(19*h, 37*h, 0.1f, 0.08f,  "Popsy Team",        0.0f,  -3.875*y, 0x55bb5544, 0.1f, 0.1f);
  font->m_AddPhrase(20*h, 38*h, 0.1f, 0.08f,  "Pulse",             0.0f,  -2.250*y, 0x55bb6655, 0.1f, 0.1f);
  font->m_AddPhrase(21*h, 39*h, 0.1f, 0.08f,  "Purple",            0.0f,  -0.625*y, 0x55bb7766, 0.1f, 0.1f);
  font->m_AddPhrase(22*h, 40*h, 0.1f, 0.08f,  "Rage",              0.0f,   1.000*y, 0x55cc8877, 0.1f, 0.1f);
  font->m_AddPhrase(23*h, 41*h, 0.1f, 0.08f,  "Ram Jam",           0.0f,   2.625*y, 0x55cc9988, 0.1f, 0.1f);
  font->m_AddPhrase(24*h, 42*h, 0.1f, 0.08f,  "Replay",            0.0f,   4.250*y, 0x55ddaa88, 0.1f, 0.1f);
  font->m_AddPhrase(25*h, 43*h, 0.1f, 0.08f,  "Sanity",            0.0f,   5.875*y, 0x55eebb99, 0.1f, 0.1f);
  font->m_AddPhrase(26*h, 44*h, 0.1f, 0.08f,  "Satori",            0.0f,   7.500*y, 0x55ffcc99, 0.1f, 0.1f);
  
  font->m_AddPhrase(27*h, 45*h, 0.1f, 0.08f,  "Spaceballs",        -0.90f,   -5.500*y, 0x55ffcc99, 0.1f, 0.1f);
  font->m_AddPhrase(28*h, 46*h, 0.1f, 0.08f,  "SpinningKids",         -0.90f,   -3.875*y, 0x55eebb99, 0.1f, 0.1f);
  font->m_AddPhrase(29*h, 47*h, 0.1f, 0.08f,  "Sunflower",         -0.90f,   -2.250*y, 0x55ddaa88, 0.1f, 0.1f);
  font->m_AddPhrase(30*h, 48*h, 0.1f, 0.08f,  "Taat",              -0.90f,   -0.625*y, 0x55cc9988, 0.1f, 0.1f);
  font->m_AddPhrase(31*h, 49*h, 0.1f, 0.08f,  "Tbl",               -0.90f,    1.000*y, 0x55cc8877, 0.1f, 0.1f);
  font->m_AddPhrase(32*h, 50*h, 0.1f, 0.08f,  "Threestate",        -0.90f,    2.625*y, 0x55bb7766, 0.1f, 0.1f);
  font->m_AddPhrase(33*h, 51*h, 0.1f, 0.08f,  "Unique",            -0.90f,    4.250*y, 0x55bb6655, 0.1f, 0.1f);
  font->m_AddPhrase(34*h, 52*h, 0.1f, 0.08f,  "Valhalla",          -0.90f,    5.875*y, 0x55bb5544, 0.1f, 0.1f);
  font->m_AddPhrase(35*h, 53*h, 0.1f, 0.08f,  "Yodel",             -0.90f,    7.500*y, 0x55aa4433, 0.1f, 0.1f);

  //font->m_AddPhrase(0.3f, 0.4f, 0.05f, 0.015f,  "              ", -0.4f,  -0.4f, 0xffffffff, 0.1f, 0.1f);
  //font->m_AddPhrase(0.4f, 0.5f, 0.05f, 0.015f,  "              ",  0.2f,   0.2f, 0xffffffff, 0.1f, 0.1f);
  DumpDebugPoint (fdeb, "Font phrases added", 0);
  //font->m_LoadAnimation("d:\\REDSHIFT\\demo_sw\\data\\Scena1\\scena1D.a3d", "Dummy01");
  font->m_LoadAnimation("d:\\REDSHIFT\\demo_sw\\data\\Scena3\\dummy.a3d", "Dummy02");
  DumpDebugPoint (fdeb, "Font m_LoadAnimation done", 0);
  font->m_Init();
  DumpDebugPoint (fdeb, "Font m_Init done", 0);

  
  Fade2D.m_SetWorkBuffer(global_ib);
  Fade2D.m_SetRenderLib(&RenderLib);
  Fade2D.m_SetPos(0, 1);
  Fade2D.m_Init();

  Flash2D.m_SetWorkBuffer(global_ib);
  Flash2D.m_SetRenderLib(&RenderLib);
  Flash2D.m_SetPos(0, 1);
  Flash2D.m_Init();

  if (strcmp(FileToPlay2, "")!=0)
  {
    scena2.m_SetWorkBuffer(global_ib);
    scena2.m_SetRenderLib(&RenderLib);
    scena2.m_Load(FileToPlay2);
    scena2.m_SetPos(0, 1);
    scena2.m_Init();
	DoScena2=1;
  }

  _flushall();

   // lunghezza della demo in secondi
  if (scena.p_NumFrames > scena2.p_NumFrames)
    demotimer.m_Init(scena.p_NumFrames/(float4)fpsdiv);
  else demotimer.m_Init(scena2.p_NumFrames/(float4)fpsdiv);

  DumpDebugPoint (fdeb, "Timer settings..", 0);
  demotimer.m_Start();
  DumpDebugPoint (fdeb, "Timer ahs started..", 0);
//	FMUSIC_PlaySong(mod);
}


void ADmainLoop() // loop principale
{
    RenderLib.m_BeginScene();
    //DumpDebugPoint (fdeb, "Begin scene done", 0);

      //RenderLib.m_ClearScreen(0x007f7f7f, 1.0f);
      RenderLib.m_ClearScreen(0, 1.0f);
	  
	  if ((DoScena1) && (scena.m_Apply(curpos)))
          scena.m_DoEfx2D(curpos);
	  if ((DoScena2) && (scena.m_Apply(curpos)))
          scena2.m_DoEfx2D(curpos);
      //DumpDebugPoint (fdeb, "Scene drawing done", 0);

	  if (font)
	  {
	    //if (font->m_Apply(curpos))
          //font->m_DoEfx2D(curpos);
	  }

	  //if (Fade2D.m_Apply(curpos))
         //  Fade2D.m_DoEfx2D(curpos);

	  //if (Flash2D.m_Apply(curpos))
        //   Flash2D.m_DoEfx2D(curpos);

    RenderLib.m_EndScene();
    //DumpDebugPoint (fdeb, "End scene done", 0);

	demotimer.m_Step();
	curpos=demotimer.m_GetPos();
    RenderLib.m_UpdateScreen();
    //DumpDebugPoint (fdeb, "m_UpdateScreen done", 0);
}


void ADmainExit() // codice finale
{
  char8 str[150];

  demotimer.m_Stop();
  fclose(fdeb);

  if (font) delete font;

  RenderLib.m_Close_Screen();
/*  
  // sto musica e free delle FMOD
  FMUSIC_FreeSong(mod);
  FSOUND_Close();
*/

  sprintf(str, "fps: %f,  blurlevel: %f", demotimer.m_GetStepsPerSecond(), l_BlurLevel);
  debug_error(miawin, str);
}

#endif