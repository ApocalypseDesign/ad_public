#include "myefx.h"
#include <math.h>
#include <time.h>


void myefx::defineControllers(tChannel *defaultChannel)
{
	// immagini in input/output:
	addChannel(defaultChannel,"Out","canale in input/output");
}

void myefx::updateControllers() 
{ 
	// immagini in input/output:
	tOut=getChannelImage("Out");
} 

void myefx::init()
{
    int i;

	srand( (unsigned)time(NULL)+clock() );


	//tOut->clear(0,0,0);
	for (i=0; i<univ.numSpheres; i++)
	{
		univ.Spheres[i].diffusecolor.x=float(rand()*255.0/RAND_MAX);
		univ.Spheres[i].diffusecolor.y=float(rand()*255.0/RAND_MAX);
		univ.Spheres[i].diffusecolor.z=float(rand()*255.0/RAND_MAX);
		univ.Spheres[i].specularcolor.x=float(rand()*255.0/RAND_MAX);
		univ.Spheres[i].specularcolor.y=float(rand()*255.0/RAND_MAX);
		univ.Spheres[i].specularcolor.z=float(rand()*255.0/RAND_MAX);
		univ.Spheres[i].shiness=1.0f+float(rand()*8.0/RAND_MAX);
	}

	initialized=true;
}

void myefx::free()
{
	//  free non necessario
	initialized=false;
}

void myefx::paint(double pos)
{
	int x,y,i;
	float ang;
	unsigned int color;
	bool hit=false;
#ifdef ANTIALIAS
	unsigned int c1,c2,c3,c4;
#endif

//	tOut->clear(0,0,128);
/*	for (y=0; y<tOut->height; y++)
	for (x=0; x<tOut->width; x++)
	tOut->uint32ptr[x+tOut->muly[y]]=x*y+x*x+y*y;
*/
	for (i=0; i<univ.numLightsOmni; i++)
	{
		ang=float(i*1.0f*PI/float(univ.numLightsOmni));
		univ.LightsOmni[i].center.x=-1000+float(sin(pos*29.0f+ang))*2350.0f;;
		univ.LightsOmni[i].center.y=-480+float(sin(32+pos*44.0f+ang))*1350.0f;;
		univ.LightsOmni[i].center.z=float(fabs(sin(pos*22.0+ang)*1255.0));;
		univ.LightsOmni[i].diffusecolor.x=float(fabs(sin(pos*12.0+ang)*255.0));
		univ.LightsOmni[i].diffusecolor.y=float(fabs(sin(322.0+pos*10.0+ang)*255.0));
		univ.LightsOmni[i].diffusecolor.z=float(fabs(sin(212.0+pos*17.0+ang)*255.0));

		univ.LightsOmni[i].specularcolor.x=120.0f+float(fabs(sin(pos*12.0+ang)*135.0));
		univ.LightsOmni[i].specularcolor.y=120.0f+float(fabs(sin(322.0+pos*10.0+ang)*135.0));
		univ.LightsOmni[i].specularcolor.z=120.0f+float(fabs(sin(212.0+pos*17.0+ang)*135.0));
	}

	for (i=0; i<univ.numSpheres; i++)
	{
		ang=float(i*2.0f*PI/float(univ.numSpheres));
		univ.Spheres[i].center.x=float(cos(pos*23.0+ang)*500.0);
		univ.Spheres[i].center.y=float(sin(pos*19.0+ang)*200.0);
		univ.Spheres[i].center.z=1300.0f+float(sin(26.0+pos*10.0+ang)*550.0);;
		univ.Spheres[i].radius=200.0f+float(sin(i*100.0+pos*10.0+ang)*100.0);
	}
	
	myRay.startpos.x=0;
	myRay.startpos.y=0;
	myRay.startpos.z=-256;

	for (y=0; y<tOut->height; y++)
	for (x=0; x<tOut->width; x++)
	{
		myRay.direction.x=float(x-tOut->width/2);
		myRay.direction.y=float(y-tOut->height/2);
		myRay.direction.z=float(tOut->width*0.8f);
		myRay.direction.normalize();

#ifdef ANTIALIAS
		color=c1=c2=c3=c4=128;
		hit=false;
		
		hit|=univ.raytrace(&myRay,-1,MAX_REFLECTIONS,&c1);
		
		myRay.direction.x=float(x+0.5f-tOut->width/2);
		myRay.direction.y=float(y-tOut->height/2);
		myRay.direction.z=float(tOut->width*0.8f);
		myRay.direction.normalize();
		
		hit|=univ.raytrace(&myRay,-1,MAX_REFLECTIONS,&c2);

		myRay.direction.x=float(x-tOut->width/2);
		myRay.direction.y=float(y+0.5f-tOut->height/2);
		myRay.direction.z=float(tOut->width*0.8f);
		myRay.direction.normalize();
		
		hit|=univ.raytrace(&myRay,-1,MAX_REFLECTIONS,&c3);

		myRay.direction.x=float(x+0.5f-tOut->width/2);
		myRay.direction.y=float(y+0.5f-tOut->height/2);
		myRay.direction.z=float(tOut->width*0.8f);
		myRay.direction.normalize();

		hit|=univ.raytrace(&myRay,-1,MAX_REFLECTIONS,&c4);

		if (hit)
		{
		c1=((c1 & 0xFEFEFE)>>1) + ((c2 & 0xFEFEFE)>>1);
		c2=((c3 & 0xFEFEFE)>>1) + ((c4 & 0xFEFEFE)>>1);
		color=((c1 & 0xFEFEFE)>>1) + ((c2 & 0xFEFEFE)>>1);
		
			tOut->uint32ptr[x+tOut->muly[y]]=color;
		}
		
#else
		if (univ.raytrace(&myRay,-1,MAX_REFLECTIONS,&color))
		{
			tOut->uint32ptr[x+tOut->muly[y]]=color;
			if (hit==false) { x-=1+SKIP; if (x<0) x=0; }
			hit=true;
		}
		else
		{
			//tOut->uint32ptr[x+tOut->muly[y]]=0xAD; //test * lo skip
			x+=SKIP;
			hit=false;
		}
#endif

		/*
	    univ.Spheres[0].center.x=float(cos(y*0.08+pos*20.0)*50.0);
		univ.Spheres[0].center.y=float(cos(x*0.08+pos*20.0)*50.0);
		univ.Spheres[0].center.z=1800.0f;
		univ.Spheres[0].radius=300.0f;
		*/
		
	}
		

}
