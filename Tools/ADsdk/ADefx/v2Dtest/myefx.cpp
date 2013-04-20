#include "myefx.h"
#include <math.h>

void myefx::defineControllers(tChannel *defaultChannel)
{
	// immagini in input/output:
	addChannel(defaultChannel,"Out","canale in input/output");

	// proprietà dell'effetto:
	ctVector2DLinear *defaultv=new ctVector2DLinear();
	Vector2D *pippo=new Vector2D;
	pippo->x=0.5f;
	pippo->y=0.5f;
	defaultv->keys->keyAdd(0,pippo);
	pippo=new Vector2D;
	pippo->x=0.9f;
	pippo->y=0.9f;
	defaultv->keys->keyAdd(500,pippo);
	pippo=new Vector2D;
	pippo->x=0.9f;
	pippo->y=0.1f;
	defaultv->keys->keyAdd(999,pippo);
	
	addProperty(defaultv,"Position","posizione");
}

void myefx::updateControllers() 
{ 
	// immagini in input/output:
	timageInOut=getChannelImage("Out");

	// controllers in input:
    miovect=(ctVector2D *)getProperty("Position");
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
  int px,py;
  Vector2D vpos=Vector2D(miovect->getValue(float(pos)));

  if (vpos.x<0.0f) vpos.x=0.0f;
  if (vpos.y<0.0f) vpos.y=0.0f;
  if (vpos.x>1.0f) vpos.x=1.0f;
  if (vpos.y>1.0f) vpos.y=1.0f;

  px=(int) (vpos.x*(float)(timageInOut->width-1));
  py=(int) (vpos.y*(float)(timageInOut->height-1));

  timageInOut->uint32ptr[px+timageInOut->muly[py]]=0xffffff;
}
