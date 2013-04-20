#include "myefx.h"
#include <math.h>
#include "adbasic.h"

void myefx::defineControllers(tChannel *defaultChannel)
{
	// immagini in input/output:
	addChannel(defaultChannel,"Out","canale in output");
	addImage("In","immagine in input");
	// proprietà
	ctVector2DConst *default_pos=new ctVector2DLinear();
	Vector2D *default_pos_key=new Vector2D;
	default_pos_key->x=0.0f;
	default_pos_key->y=0.0f;
	default_pos->keys->keyAdd(0,default_pos_key);

	addProperty(default_pos,"Pos","Posizione x (0.0 .. 1.0)");

	ctFloatConst *default_rot=new ctFloatLinear();
	float *default_rot_key=new float;
	*default_rot_key=0.0f;
	default_rot->keys->keyAdd(0,default_rot_key);

	addProperty(default_rot,"Rot","Rotazione (0.0 .. 2.0*pi)");

	ctFloatConst *default_zoom=new ctFloatLinear();
	float *default_zoom_key=new float;
	*default_zoom_key=1.0f;
	default_zoom->keys->keyAdd(0,default_zoom_key);

	addProperty(default_zoom,"ZoomX","Zoom x (1.0 = no zoom)");

	default_zoom=new ctFloatLinear();
	default_zoom_key=new float;
	*default_zoom_key=1.0f;
	default_zoom->keys->keyAdd(0,default_zoom_key);

	addProperty(default_zoom,"ZoomY","Zoom y (1.0 = no zoom)");
}

void myefx::updateControllers() 
{ 
	// canali in input:
	timageInOut=getChannelImage("Out");

	// immagini in input:
	ttexture=getImage("In");

	// controllers in input:
	tpos=(ctVector2D *)getProperty("Pos");
	trot=(ctFloat *)getProperty("Rot");
	tzoomx=(ctFloat *)getProperty("ZoomX");
	tzoomy=(ctFloat *)getProperty("ZoomY");

	xmask=ymask=1;
	while(xmask<=(unsigned int)(ttexture->width)) xmask<<=1;
	while(ymask<=(unsigned int)(ttexture->height)) ymask<<=1;
	xmask=(xmask>>1)-1;
	ymask=(ymask>>1)-1;
    //char *pippo=new char[100];
	//itoa(xmask,pippo,10);
	//debug_warning(0,pippo);
} 

void myefx::init()
{
	//  init non necessario
	initialized=true;
}

void myefx::free()
{
	//  free non necessario
	initialized=false;
}

void myefx::paint(double pos)
{
	unsigned int ix,iy,maxx,maxy;
	unsigned int u,v,ustart,vstart,du,dv;
	Vector2D mypos=tpos->getValue((float)pos);

	maxx=(unsigned int)timageInOut->width;
	maxy=(unsigned int)timageInOut->height;

	//du=(unsigned int) (65536.0f * (float) cos(trot->getValue((float)pos))); 
	//dv=(unsigned int) (65536.0f * (float) sin(trot->getValue((float)pos))); 

	du=(unsigned int) (65536.0f / (float) tzoomx->getValue((float)pos) * (float) cos(trot->getValue((float)pos))); 
	dv=(unsigned int) (65536.0f / (float) tzoomy->getValue((float)pos) * (float) sin(trot->getValue((float)pos))); 

	u=ustart=(unsigned int)(mypos.x * float(timageInOut->width * 65536));
	v=vstart=(unsigned int)(mypos.y * float(timageInOut->height * 65536));;


	for (iy=0; iy<maxy; iy++)
	{
		for (ix=0; ix<maxx; ix++)
		{
			timageInOut->uint32ptr[ix+timageInOut->muly[iy]]=
			((timageInOut->uint32ptr[ix+timageInOut->muly[iy]]& 0xFEFEFE) >> 1)
			+
			(((ttexture->uint32ptr[((u>>16) & xmask)+ttexture->muly[((v>>16) & ymask)]])& 0xFEFEFE) >>1);

			u+=du;
			v+=dv;
		}
		u=ustart-dv*iy;
		v=vstart+du*iy;
	}


}
