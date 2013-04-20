#include "myefx.h"
#include <math.h>

void myefx::defineControllers(tChannel *defaultChannel)
{
	// immagini in input/output:
	addChannel(defaultChannel,"Out","canale in output");
	addChannel(defaultChannel,"In1","canale in input 1");
	addChannel(defaultChannel,"In2","canale in input 2");

	// proprietà dell'effetto:
}

void myefx::updateControllers() 
{ 
	// immagini in input/output:
	timageOut=getChannelImage("Out");
	timageIn1=getChannelImage("In1");
	timageIn2=getChannelImage("In2");

	// controllers in input:
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

	int i;

	for (i=0; i<(timageOut->size>>2); i++)
	timageOut->uint32ptr[i]=((timageIn1->uint32ptr[i] & 0xFEFEFE)>>1) + 
		                    ((timageIn2->uint32ptr[i] & 0xFEFEFE)>>1);
}
