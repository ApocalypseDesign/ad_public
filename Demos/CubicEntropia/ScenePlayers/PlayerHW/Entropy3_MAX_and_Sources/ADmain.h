// codice principale della demo
// -------------------------------
// AD Demo System 99 - ADmain.h
// -------------------------------

#ifndef _ADMAIN_H_
#define _ADMAIN_H_

//#include "fmod.h"
#include "bass.h"
#include "dtimer.h"
#include "Scene3d.h"
#include "TexFont.h"
#include "Fade2D.h"
#include "Flash2D.h"
#include "Sprite2D.h"
#include "FastMath.h"


FILE *fdeb;

CTimer demotimer;

//float4 l_BlurLevel=0;
//int32 l_BlurType=NOBLUR;
BOOL NoSound;
//FMUSIC_MODULE *mod;
HMUSIC mod=NULL;
CGraphicSurface *global_ib;

CFont *font1=(CFont *)NULL;
CFont *font2=(CFont *)NULL;
CFont *font4=(CFont *)NULL;
CFont *font5=(CFont *)NULL;



#define SCENA0_START  (0)
#define SCENA0_END    (0.001)

#define SCENA1_START  (0.001)
#define SCENA1_END    (0.1)

#define SCENA2_START  (0.1)
#define SCENA2_END    (0.26)

#define SCENA3_START  (0.26)
#define SCENA3_END    (0.30)

#define SCENA4_START  (0.30)
#define SCENA4_END    (0.48)

#define SCENA5_START  (0.48)
#define SCENA5_END    (0.64)

#define SCENA6_START  (0.64)
#define SCENA6_END    (1.00)

//#define SCENA7_START  (0.85)
//#define SCENA7_END    (1.0)

//#define SCENA8_START  (0.81466)
//#define SCENA8_END    (1.00000)

#define NUM_FLASH 10
#define NUM_FADE  5


// SCENA1: 
CScene3D scena1;

// SCENA2: 
CScene3D scena2;

// SCENA3: 
CScene3D scena3;

// SCENA4: 
CScene3D scena4;

// SCENA5: 
CScene3D scena5;

// SCENA6: 
CScene3D scena6;

// SCENA7: 
CScene3D scena7;

// SCENA8: 
CScene3D scena8;


CFade2D Fade2D[NUM_FADE];
CFlash2D Flash2D[NUM_FLASH];
// effetti syncati con comandi del modulo
CFlash2D FlashSFX;
int32 FlashSFXFlag=0;
CFade2D FadeSFX;
int32 FadeSFXFlag=0;

CSprite2D loadPic, scroller;

// la funzione restituisce la posizione assoluta dati i
// range assoluti dell'intervallo (es. SCENA1_START e
// SCENA1_END) e la posizione relativa (interpos, che
// e' sempre tra [0;1]) all'interno di quell'intervallo
double relpos(double start, double end, double interpos)
{ return(interpos*(end-start)+start); }


void CALLBACK MySyncProc(HSYNC handle, DWORD channel,
                         DWORD data, DWORD user)
{ 

   // comando E81 per il flash
   if (data==1)
   {
	 FlashSFX.m_SetPos(curpos-0.002, curpos+0.002);
     FlashSFXFlag=1;
   }
   else
   // comando E82 per il fade
   if (data==2)
   {
	 FadeSFX.m_SetPos(curpos-0.005, curpos+0.005);
     FadeSFXFlag=1;
   }
}



void ADmainEnter() //codice iniziale
{
  int32 i;
    // per ragioni di preformance è meglio richiedere sempre 32bpp
    // se per caso non è disponibile sulla scheda video viene
    // automaticamaente trovato un altro modo video a bpp inferiore

  init_fast_math_library();
  RenderLib.m_OpenScreen(miawin, ScrMode, RunFullscreen);
  //RenderLib.m_SetBlur(MOTIONBLUR | BLUR_ADD, 0.5);
  RenderLib.m_SetBlur(0, 0);

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
  if (!mod) debug_error(miawin, "FMOD error: voglio lop sticchio!");
*/

  fdeb=fopen("SceneLog.txt", "wt");

  // ---------------------------  Bass 1.6  ----------------------------
  if (NoSound==FALSE)
  {
    if (!BASS_Init(-1, 44100, 0, miawin)) 
      debug_error(miawin, "Can't initialize digital sound system");

    mod=BASS_MusicLoad(FALSE, "Data\\Modulo.mo3",
                       0, 0, BASS_MUSIC_LOOP);
    if (!mod)
	  debug_error(miawin, "Can't load sound file (modulo.mo3)");
  }

  // aggiungo il font
  i=AddFontResource("Data\\REN_STIM.TTF");

  // pic di loading
  loadPic.m_SetWorkBuffer(global_ib);
  loadPic.m_SetRenderLib(&RenderLib);
  loadPic.m_SetPos(SCENA1_START, SCENA1_END);
  loadPic.m_SetParameters("Data\\Loading.png", BLEND_ADD, 0, 0);
  loadPic.m_AddKey(0, 0, -64, 1, 1);



  loadPic.m_Init();
  RenderLib.m_BeginScene();
  RenderLib.m_ClearScreen(0, 1.0f);
  loadPic.m_DoEfx2D(0);
  RenderLib.m_EndScene();
  RenderLib.m_UpdateScreen();

  // ------------------------ Scena 1 -----------------------------
  i=_chdir("Data\\Scena1");
  scena1.m_SetWorkBuffer(global_ib);
  scena1.m_SetRenderLib(&RenderLib);
  scena1.m_SetPos(SCENA1_START, SCENA1_END);
  scena1.m_Load("scena1");
  //scena1.m_Load("scena1_cameralinkata");
  scena1.m_Init();
  DumpDebugPoint (fdeb, "Scene1 Loaded and Initialized", 0);
  Fade2D[0].m_SetPos(SCENA0_START, SCENA1_START+0.01);

  font1=new CFont("Ren & Stimpy", 60, 0);
  font1->m_SetWorkBuffer(global_ib);
  font1->m_SetRenderLib(&RenderLib);
  font1->m_SetPos(SCENA1_START, SCENA1_END);
  font1->m_AddPhrase(0.1,  0.4, 0.1f, 0.08f,  "Sometimes they return...",       -0.93f, -0.95, 0xffffffff, 0.1f, 0.1f);
  font1->m_AddPhrase(0.3,  0.6, 0.1f, 0.08f,  "in an old style fashion...",     -0.93f, 0.7,   0xffffffff, 0.1f, 0.1f);
  font1->m_AddPhrase(0.5,  0.8, 0.1f, 0.08f,  "same design again..",            -0.93f, -0.95, 0xffffffff, 0.1f, 0.1f);
  font1->m_AddPhrase(0.7,  1.0, 0.1f, 0.08f,  "..but much flavours",            -0.93f, 0.7, 0xffffffff, 0.1f, 0.1f);
  font1->m_Init();


  // ------------------------ Scena 2 -----------------------------
  i=_chdir("..\\..\\Data\\Scena2");
  scena2.m_SetWorkBuffer(global_ib);
  scena2.m_SetRenderLib(&RenderLib);
  scena2.m_SetPos(SCENA2_START, SCENA2_END);
  scena2.m_Load("scena2");
  scena2.m_Init();
  DumpDebugPoint (fdeb, "Scene2 Loaded and Initialized", 0);
  Fade2D[1].m_SetPos(relpos(SCENA2_START, SCENA2_END, -0.01),
	                 relpos(SCENA2_START, SCENA2_END, 0.01));
  font2=new CFont("Ren & Stimpy", 75, 0);
  font2->m_SetWorkBuffer(global_ib);
  font2->m_SetRenderLib(&RenderLib);
  font2->m_SetPos(SCENA2_START, SCENA2_END);
  font2->m_AddPhrase(0.4,  0.6, 0.1f, 0.08f,  "when entropy meets",     -0.95f, -0.90, 0xff000000, 0.12f, 0.12f);
  font2->m_AddPhrase(0.5,  0.7, 0.1f, 0.08f,  "a bicubic function",     -0.95f, -0.43, 0xff000000, 0.12f, 0.12f);
  font2->m_AddPhrase(0.6,  0.8, 0.1f, 0.08f,  "caos will be bounded..", -0.95f,  0.23, 0xff000000, 0.12f, 0.12f);
  font2->m_AddPhrase(0.7,  0.9, 0.1f, 0.08f,  "..and a demo born.",     -0.95f,  0.75, 0xff000000, 0.12f, 0.12f);
  font2->m_AddPhrase(0.4,  0.6, 0.1f, 0.08f,  "when entropy meets",     -0.96f, -0.90, 0xff771133, 0.105f, 0.105f);
  font2->m_AddPhrase(0.5,  0.7, 0.1f, 0.08f,  "a bicubic function",     -0.96f, -0.43, 0xff882222, 0.105f, 0.105f);
  font2->m_AddPhrase(0.6,  0.8, 0.1f, 0.08f,  "caos will be bounded..", -0.96f,  0.23, 0xff993311, 0.105f, 0.105f);
  font2->m_AddPhrase(0.7,  0.9, 0.1f, 0.08f,  "..and a demo born.",     -0.96f,  0.75, 0xffaa4400, 0.105f, 0.105f);
  font2->m_Init();


  // ------------------------ Scena 3 -----------------------------
  _chdir("..\\..\\Data\\Scena3");
  scena3.m_SetWorkBuffer(global_ib);
  scena3.m_SetRenderLib(&RenderLib);
  scena3.m_SetPos(SCENA3_START, SCENA3_END);
  scena3.m_Load("scena3");
  scena3.m_Init();
  DumpDebugPoint (fdeb, "Scene3 Loaded and Initialized", 0);
  Fade2D[2].m_SetPos(relpos(SCENA3_START, SCENA3_END, -0.3),
	                 relpos(SCENA3_START, SCENA3_END, 0.2));

  // ------------------------ Scena 4 -----------------------------
  _chdir("..\\..\\Data\\Scena4");
  scena4.m_SetWorkBuffer(global_ib);
  scena4.m_SetRenderLib(&RenderLib);
  scena4.m_SetPos(SCENA4_START, SCENA4_END);
  scena4.m_Load("scena4");
  scena4.m_Init();
  DumpDebugPoint (fdeb, "Scene4 Loaded and Initialized", 0);
  Fade2D[3].m_SetPos(relpos(SCENA4_START, SCENA4_END, -0.01),
	                 relpos(SCENA4_START, SCENA4_END, 0.01));
  font4=new CFont("Ren & Stimpy", 75, 0);
  font4->m_SetWorkBuffer(global_ib);
  font4->m_SetRenderLib(&RenderLib);
  font4->m_SetPos(SCENA4_START, SCENA4_END);
  font4->m_AddPhrase(0.5,  0.7, 0.1f, 0.08f,  "every good fish",     -0.95f, -0.90, 0xff000000, 0.12f, 0.12f);
  font4->m_AddPhrase(0.6,  0.8, 0.1f, 0.08f,  "needs a good demo",   -0.95f, -0.43, 0xff000000, 0.12f, 0.12f);
  font4->m_AddPhrase(0.7,  0.9, 0.1f, 0.08f,  "with a lame fog",     -0.95f,  0.23, 0xff000000, 0.12f, 0.12f);
  font4->m_AddPhrase(0.8,  1.0, 0.1f, 0.08f,  "to swim into...",     -0.95f,  0.75, 0xff000000, 0.12f, 0.12f);
  font4->m_Init();

  // ------------------------ Scena 5 -----------------------------
  _chdir("..\\..\\Data\\Scena5");
  scena5.m_SetWorkBuffer(global_ib);
  scena5.m_SetRenderLib(&RenderLib);
  scena5.m_SetPos(SCENA5_START, SCENA5_END);
  scena5.m_Load("scena5");
  scena5.m_Init();
  DumpDebugPoint (fdeb, "Scene5 Loaded and Initialized", 0);
  font5=new CFont("Ren & Stimpy", 80, 0);
  DumpDebugPoint (fdeb, "Font allocated", 0);
  font5->m_SetWorkBuffer(global_ib);
  DumpDebugPoint (fdeb, "Font m_SetWorkBuffer done", 0);
  font5->m_SetRenderLib(&RenderLib);
  DumpDebugPoint (fdeb, "Font m_SetWorkBuffer done", 0);
  font5->m_SetPos(SCENA5_START, SCENA5_END);
  DumpDebugPoint (fdeb, "Font m_SetWorkBuffer done", 0);
  float4 h=1.0f/54.0f;
  float4 y=-1.0f/8.0f;
  font5->m_AddPhrase(0*h,  18*h, 0.1f, 0.08f,  "Aardbei",      0.00f,  -5.500*y, 0x55aa4433, 0.1f, 0.1f);
  font5->m_AddPhrase(1*h,  19*h, 0.1f, 0.08f,  "Blasphemy",   -0.21f,  -3.875*y, 0x55bb5544, 0.1f, 0.1f);
  font5->m_AddPhrase(2*h,  20*h, 0.1f, 0.08f,  "Bomb",         0.19f,  -2.250*y, 0x55bb6655, 0.1f, 0.1f);
  font5->m_AddPhrase(3*h,  21*h, 0.1f, 0.08f,  "Byterapers",  -0.30f,  -0.625*y, 0x55bb7766, 0.1f, 0.1f);
  font5->m_AddPhrase(4*h,  22*h, 0.1f, 0.08f,  "Calodox",     -0.03f,   1.000*y, 0x55cc8877, 0.1f, 0.1f);
  font5->m_AddPhrase(5*h,  23*h, 0.1f, 0.08f,  "Cocoon",      -0.03f,   2.625*y, 0x55cc9988, 0.1f, 0.1f);
  font5->m_AddPhrase(6*h,  24*h, 0.1f, 0.08f,  "Coma",         0.30f,   4.250*y, 0x55ddaa88, 0.1f, 0.1f);
  font5->m_AddPhrase(7*h,  25*h, 0.1f, 0.08f,  "Complex",      0.02f,   5.875*y, 0x55eebb99, 0.1f, 0.1f);
  font5->m_AddPhrase(8*h,  26*h, 0.1f, 0.08f,  "Exceed",       0.11f,   7.500*y, 0x55ffcc99, 0.1f, 0.1f);
  font5->m_AddPhrase(9*h,  27*h, 0.1f, 0.08f,  "Faurbraush",  -0.90f,  -5.500*y, 0x55ffcc99, 0.1f, 0.1f);
  font5->m_AddPhrase(10*h, 28*h, 0.1f, 0.08f,  "Fudge",       -0.90f,  -3.875*y, 0x55eebb99, 0.1f, 0.1f);
  font5->m_AddPhrase(11*h, 29*h, 0.1f, 0.08f,  "Haujobb",     -0.90f,  -2.250*y, 0x55ddaa88, 0.1f, 0.1f);
  font5->m_AddPhrase(12*h, 30*h, 0.1f, 0.08f,  "Kolor",       -0.90f,  -0.625*y, 0x55cc9988, 0.1f, 0.1f);
  font5->m_AddPhrase(13*h, 31*h, 0.1f, 0.08f,  "Mandula",     -0.90f,   1.000*y, 0x55cc8877, 0.1f, 0.1f);
  font5->m_AddPhrase(14*h, 32*h, 0.1f, 0.08f,  "Matrix",      -0.90f,   2.625*y, 0x55bb7766, 0.1f, 0.1f);
  font5->m_AddPhrase(15*h, 33*h, 0.1f, 0.08f,  "Moppi",       -0.90f,   4.250*y, 0x55bb6655, 0.1f, 0.1f);
  font5->m_AddPhrase(16*h, 34*h, 0.1f, 0.08f,  "Noice",       -0.90f,   5.875*y, 0x55bb5544, 0.1f, 0.1f);
  font5->m_AddPhrase(17*h, 35*h, 0.1f, 0.08f,  "Nomad",       -0.90f,   7.500*y, 0x55aa4433, 0.1f, 0.1f);
  font5->m_AddPhrase(18*h, 36*h, 0.1f, 0.08f,  "Orange",            0.0f,  -5.500*y, 0x55aa4433, 0.1f, 0.1f);
  font5->m_AddPhrase(19*h, 37*h, 0.1f, 0.08f,  "Popsy Team",        0.0f,  -3.875*y, 0x55bb5544, 0.1f, 0.1f);
  font5->m_AddPhrase(20*h, 38*h, 0.1f, 0.08f,  "Pulse",             0.0f,  -2.250*y, 0x55bb6655, 0.1f, 0.1f);
  font5->m_AddPhrase(21*h, 39*h, 0.1f, 0.08f,  "Purple",            0.0f,  -0.625*y, 0x55bb7766, 0.1f, 0.1f);
  font5->m_AddPhrase(22*h, 40*h, 0.1f, 0.08f,  "Rage",              0.0f,   1.000*y, 0x55cc8877, 0.1f, 0.1f);
  font5->m_AddPhrase(23*h, 41*h, 0.1f, 0.08f,  "Ram Jam",           0.0f,   2.625*y, 0x55cc9988, 0.1f, 0.1f);
  font5->m_AddPhrase(24*h, 42*h, 0.1f, 0.08f,  "Replay",            0.0f,   4.250*y, 0x55ddaa88, 0.1f, 0.1f);
  font5->m_AddPhrase(25*h, 43*h, 0.1f, 0.08f,  "Sanity",            0.0f,   5.875*y, 0x55eebb99, 0.1f, 0.1f);
  font5->m_AddPhrase(26*h, 44*h, 0.1f, 0.08f,  "Satori",            0.0f,   7.500*y, 0x55ffcc99, 0.1f, 0.1f); 
  font5->m_AddPhrase(27*h, 45*h, 0.1f, 0.08f,  "Spaceballs",        -0.90f,   -5.500*y, 0x55ffcc99, 0.1f, 0.1f);
  font5->m_AddPhrase(28*h, 46*h, 0.1f, 0.08f,  "SpinningKids",         -0.90f,   -3.875*y, 0x55eebb99, 0.1f, 0.1f);
  font5->m_AddPhrase(29*h, 47*h, 0.1f, 0.08f,  "Sunflower",         -0.90f,   -2.250*y, 0x55ddaa88, 0.1f, 0.1f);
  font5->m_AddPhrase(30*h, 48*h, 0.1f, 0.08f,  "Taat",              -0.90f,   -0.625*y, 0x55cc9988, 0.1f, 0.1f);
  font5->m_AddPhrase(31*h, 49*h, 0.1f, 0.08f,  "Tbl",               -0.90f,    1.000*y, 0x55cc8877, 0.1f, 0.1f);
  font5->m_AddPhrase(32*h, 50*h, 0.1f, 0.08f,  "Threestate",        -0.90f,    2.625*y, 0x55bb7766, 0.1f, 0.1f);
  font5->m_AddPhrase(33*h, 51*h, 0.1f, 0.08f,  "Unique",            -0.90f,    4.250*y, 0x55bb6655, 0.1f, 0.1f);
  font5->m_AddPhrase(34*h, 52*h, 0.1f, 0.08f,  "Valhalla",          -0.90f,    5.875*y, 0x55bb5544, 0.1f, 0.1f);
  font5->m_AddPhrase(35*h, 53*h, 0.1f, 0.08f,  "Yodel",             -0.90f,    7.500*y, 0x55aa4433, 0.1f, 0.1f);
  DumpDebugPoint (fdeb, "Font phrases added", 0);
  font5->m_LoadAnimation("dummy.a3d", "Dummy02");
  DumpDebugPoint (fdeb, "Font m_LoadAnimation done", 0);
  font5->m_Init();
  DumpDebugPoint (fdeb, "Font m_Init done", 0);
  Fade2D[4].m_SetPos(relpos(SCENA5_START, SCENA5_END, -0.01),
	                 relpos(SCENA5_START, SCENA5_END, 0.01));

  // ------------------------ Scena 6 -----------------------------
  //_chdir("..\\..\\Data\\Scena6");
  _chdir("..\\..\\Data\\Scena7");
  scena6.m_SetWorkBuffer(global_ib);
  scena6.m_SetRenderLib(&RenderLib);
  scena6.m_SetPos(SCENA6_START, SCENA6_END);
  //scena6.m_Load("scena6");
  scena6.m_Load("scena7");
  scena6.m_Init();
  DumpDebugPoint (fdeb, "Scene6 Loaded and Initialized", 0);
  Flash2D[0].m_SetPos(relpos(SCENA6_START, SCENA6_END, -0.02),
	                  relpos(SCENA6_START, SCENA6_END, 0.02));

  Flash2D[1].m_SetPos(relpos(SCENA6_START, SCENA6_END, 0.49),
	                  relpos(SCENA6_START, SCENA6_END, 0.52));

  scroller.m_SetWorkBuffer(global_ib);
  scroller.m_SetRenderLib(&RenderLib);
  scroller.m_SetPos(SCENA6_START, SCENA6_END);
  scroller.m_SetParameters("scroller.png", BLEND_ADD, 0, 0);
  scroller.m_AddKey(0, 0, 512, 1, 1);
  scroller.m_AddKey(1, 0, -512, 1, 1);
  scroller.m_Init();

  Fade2D[6].m_SetPos(relpos(SCENA6_START, SCENA6_END, 0.95),
	                 relpos(SCENA6_START, SCENA6_END, 1.05));

  // ------------------------ Scena 7 -----------------------------
  /*_chdir("..\\..\\Data\\Scena7");
  scena7.m_SetWorkBuffer(global_ib);
  scena7.m_SetRenderLib(&RenderLib);
  scena7.m_SetPos(SCENA7_START, SCENA7_END);
  scena7.m_Load("scena7");
  scena7.m_Init();
  DumpDebugPoint (fdeb, "Scene7 Loaded and Initialized", 0);
  Fade2D[6].m_SetPos(relpos(SCENA7_START, SCENA7_END, -0.07),
	                 relpos(SCENA7_START, SCENA7_END, 0.07));

  // ------------------------ Scena 8 -----------------------------
/*  _chdir("..\\Data\\Scena8");
  scena8.m_SetWorkBuffer(global_ib);
  scena8.m_SetRenderLib(&RenderLib);
  scena8.m_SetPos(SCENA8_START, SCENA8_END);
  scena8.m_Load("scena8");
  scena8.m_Init();
  DumpDebugPoint (fdeb, "Scene7 Loaded and Initialized", 0);*/

  // ------------------------  Flash  -----------------------------
  for (i=0; i<NUM_FLASH; i++)
  {
    Flash2D[i].m_SetWorkBuffer(global_ib);
    Flash2D[i].m_SetRenderLib(&RenderLib);
    Flash2D[i].m_Init();  
  }
  FlashSFX.m_SetWorkBuffer(global_ib);
  FlashSFX.m_SetRenderLib(&RenderLib);
  FlashSFX.m_Init();  
  DumpDebugPoint (fdeb, "Flashes Loaded and Initialized", 0);

  // -------------------------  Fade  -----------------------------
  for (i=0; i<NUM_FADE; i++)
  {
    Fade2D[i].m_SetWorkBuffer(global_ib);
    Fade2D[i].m_SetRenderLib(&RenderLib);
    Fade2D[i].m_Init();
  }
  FadeSFX.m_SetWorkBuffer(global_ib);
  FadeSFX.m_SetRenderLib(&RenderLib);
  FadeSFX.m_Init();  
  DumpDebugPoint (fdeb, "Fades Loaded and Initialized", 0);


  // ritorno nella directory dell'eseguibile e rimuovo il font
  _chdir("..\\..\\");
  i=RemoveFontResource("Data\\REN_STIM.TTF");

  _flushall();

   // lunghezza della demo in secondi
  // 30 + 45 + 15 + 80 + 50 + 40 + 35
  demotimer.m_Init(300);
  demotimer.m_Start();
  //demotimer.m_SetPos(SCENA3_END*0.7);
  DumpDebugPoint (fdeb, "Timer has started..", 0);

  // comando E81 per i flash  
  if ((NoSound == FALSE) && (mod))
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
//	FMUSIC_PlaySong(mod);
}


void ADmainLoop() // loop principale
{
  int32 i;

  RenderLib.m_BeginScene();
  //DumpDebugPoint (fdeb, "Begin scene done", 0);

  //RenderLib.m_ClearScreen(0x007f7f7f, 1.0f);
  //RenderLib.m_ClearScreen(0, 1.0f);


// ------------------------------  scena1  --------------------------
	  if (scena1.m_Apply(curpos))
	  {
          RenderLib.m_ClearScreen(0, 1.0f);
          RenderLib.m_SetBlur(MOTIONBLUR | BLUR_ALPHA, 0.67);
          //RenderLib.m_SetBlur(0, 0);
          scena1.m_DoEfx2D(curpos);
	      if (font1)
		  {
	        if (font1->m_Apply(curpos))
              font1->m_DoEfx2D(curpos);
		  }
	  }
// ------------------------------  scena2  --------------------------
      if (scena2.m_Apply(curpos))
	  {
          RenderLib.m_ClearScreen(0, 1.0f);
          RenderLib.m_SetBlur(RADIALBLUR | BLUR_ADD, 0.09);
          scena2.m_DoEfx2D(curpos);
	      if (font2)
		  {
	        if (font2->m_Apply(curpos))
              font2->m_DoEfx2D(curpos);
		  }
	  }
// ------------------------------  scena3  --------------------------
	  if (scena3.m_Apply(curpos))
	  {
          RenderLib.m_ClearScreen(0, 1.0f);
          RenderLib.m_SetBlur(MOTIONBLUR | BLUR_ALPHA, 0.8);
          scena3.m_DoEfx2D(curpos);
	  }
// ------------------------------  scena4  --------------------------
	  if (scena4.m_Apply(curpos))
	  {
          RenderLib.m_ClearScreen(0xff, 20, 80, 120, 1.0f);
          RenderLib.m_SetBlur(MOTIONBLUR | BLUR_ALPHA, 0.5);
          scena4.m_DoEfx2D(curpos);
	      if (font4)
		  {
	        if (font4->m_Apply(curpos))
              font4->m_DoEfx2D(curpos);
		  }
	  }
// ------------------------------  scena5  --------------------------
	  if (scena5.m_Apply(curpos))
	  {
          RenderLib.m_ClearScreen(0, 1.0f);
          RenderLib.m_SetBlur(RADIALBLUR | BLUR_ADD, 0.095);
          scena5.m_DoEfx2D(curpos);
	  }
	  if (font5)
	  {
	    if (font5->m_Apply(curpos))
          font5->m_DoEfx2D(curpos);
	  }
// ------------------------------  scena6  --------------------------
	  if (scena6.m_Apply(curpos))
	  {
          RenderLib.m_ClearScreen(0, 1.0f);
          RenderLib.m_SetBlur(MOTIONBLUR | BLUR_ALPHA, 0.6);
          scena6.m_DoEfx2D(curpos);
		  if (scroller.m_Apply(curpos))
              scroller.m_DoEfx2D(curpos);
	  }
// ------------------------------  scena7  --------------------------
/*	  if (scena7.m_Apply(curpos))
	  {
          RenderLib.m_ClearScreen(0, 1.0f);
          RenderLib.m_SetBlur(MOTIONBLUR | BLUR_ALPHA, 0.75);
          scena7.m_DoEfx2D(curpos);
	  }
// ------------------------------  scena8  --------------------------
	  /*if (scena8.m_Apply(curpos))
	  {
          RenderLib.m_SetBlur(BLUR_ADD | BLUR_ADD, 0.5);
          scena8.m_DoEfx2D(curpos);
	  }*/
//-------------------------------  flash  ----------------------------
      for (i=0; i<NUM_FLASH; i++)
	  {
        if (Flash2D[i].m_Apply(curpos))
            Flash2D[i].m_DoEfx2D(curpos);
	  }
	  if ((FlashSFX.m_Apply(curpos)) && FlashSFXFlag)
		FlashSFX.m_DoEfx2D(curpos);

//-------------------------------  fade  ----------------------------
      for (i=0; i<NUM_FADE; i++)
	  {
        if (Fade2D[i].m_Apply(curpos))
            Fade2D[i].m_DoEfx2D(curpos);
	  }
	  if ((FadeSFX.m_Apply(curpos)) && FadeSFXFlag)
		FadeSFX.m_DoEfx2D(curpos);

	  if (curpos>0.985)
	  {
		  BASS_SetVolume((1 - (curpos-0.985f)/(0.015f))*100.0f);
	  }

//--------------------------------------------------------------------

  RenderLib.m_EndScene();
  //DumpDebugPoint (fdeb, "End scene done", 0);

  demotimer.m_Step();
  curpos=demotimer.m_GetPos();

  char8 s[100];
  sprintf(s, "curpos: %f\n", curpos);
  //DumpDebugPoint (fdeb, s, 0);

  RenderLib.m_UpdateScreen();
  //DumpDebugPoint (fdeb, "m_UpdateScreen done", 0);

}


void ADmainExit() // codice finale
{
  demotimer.m_Stop();
  fclose(fdeb);

  if (font1) delete font1;
  if (font2) delete font2;
  if (font4) delete font5;
  if (font5) delete font5;

  RenderLib.m_Close_Screen();
  if (NoSound == FALSE)
  {
     //FMUSIC_FreeSong(mod);
     //FSOUND_Close();
     if (mod) BASS_MusicFree(mod);
     BASS_Stop();
	 BASS_Free();
  }
}

#endif