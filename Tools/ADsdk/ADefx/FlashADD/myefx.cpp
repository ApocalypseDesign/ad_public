#include "myefx.h"
#include <math.h>

void myefx::defineControllers(tChannel *defaultChannel)
{
	// immagini in input/output:
	addChannel(defaultChannel,"Out","canale in input/output");

	// proprietÓ dell'effetto:
	ctFloatConst *defaultIntensity[3];
	float *pippo[3];
	for (int i=0; i<3; i++)
	{
		pippo[i]=new float;
		*pippo[i]=0.5f;
		defaultIntensity[i]=new ctFloatLinear();
		defaultIntensity[i]->keys->keyAdd(0,pippo[i]);
	}
	
	addProperty(defaultIntensity[0],"Red-intensity","intensitÓ del flash (additivo) Rosso");
	addProperty(defaultIntensity[1],"Green-intensity","intensitÓ del flash (additivo) Verde");
	addProperty(defaultIntensity[2],"Blue-intensity","intensitÓ del flash (additivo) Blu");
}

void myefx::updateControllers() 
{ 
	// immagini in input/output:
	timageInOut=getChannelImage("Out");

	// controllers in input:
    inteR=(ctFloat *)getProperty("Red-intensity");
	inteG=(ctFloat *)getProperty("Green-intensity");
	inteB=(ctFloat *)getProperty("Blue-intensity");
} 

void myefx::init()
{
	//	init non necessario
	initialized=true;
}

void myefx::free()
{
	//  free non necessario
	initialized=false;
}

void myefx::paint(double pos)
{
  int intensityR=int(inteR->getValue(float(pos))*255.0);
  int intensityG=int(inteG->getValue(float(pos))*255.0);
  int intensityB=int(inteB->getValue(float(pos))*255.0);

  int inte_r=intensityR<<16, inte_g=intensityG<<8, inte_b=intensityB;

  unsigned int pippo;
  pippo=(unsigned int)timageInOut->uint32ptr;
  int max=timageInOut->size>>2;

 _asm 
  {
	//  push edi	// non necessario in vc6
	//  push ecx	// idem

	  mov ecx,max
	  mov edi,pippo
flashloop:
	  mov eax,[edi]

	  mov ebx,eax
	  and ebx,0x00FF0000
	  add ebx,inte_r
	  cmp ebx,0x00FF0000
	  jb nosatr
	  mov ebx,0x00FF0000
nosatr:
	  mov edx,ebx

	  mov ebx,eax
	  and ebx,0x0000FF00
	  add ebx,inte_g
	  cmp ebx,0x0000FF00
	  jb nosatg
	  mov ebx,0x0000FF00
nosatg:
	  add edx,ebx

	  mov ebx,eax
	  and ebx,0x000000FF
	  add ebx,inte_b
	  cmp ebx,0x000000FF
	  jb nosatb
	  mov ebx,0x000000FF
nosatb:
	  add edx,ebx

	  mov [edi],edx
	  dec ecx
	  add edi,4
	  test ecx,ecx
	  jnz flashloop

	  // pop ecx	// non necessario in vc
	  // pop edi	// idem
  }
}
