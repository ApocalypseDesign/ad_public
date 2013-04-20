#include "myefx.h"
#include <math.h>

  #define Pi__ 3.141592
  #define sqr(a) ((a)*(a))


void myefx::defineControllers(tChannel *defaultChannel)
{
	// immagini in input/output:
	addChannel(defaultChannel,"Out","canale in output");

	// proprietà:
	ctIntConst *const1=new ctIntLinear;
	ctIntConst *const2=new ctIntLinear;        
	int *v1=new int;
	int *v2=new int;
	*v1=1;
	*v2=1;
	const1->keys->keyAdd(0,v1);
	const2->keys->keyAdd(0,v2);
    
	addProperty(const1, "Vel1", "Velocità di look nella tabella1");
	addProperty(const2, "Vel2", "Velocità di look nella tabella2");
}


void myefx::updateControllers() 
{ 
	// immagini in input/output:
	timageInOut=getChannelImage("Out");

	// proprietà:
	Controller_k1=(ctInt *)getProperty("Vel1");
	Controller_k2=(ctInt *)getProperty("Vel2");
} 


void myefx::init()
{
  double tmptab[512];
  pls1      =new int[512];
  pls2      =new int[512];
  pls3      =new int[512];
  kpls1     =new int[512];
  kpls2     =new int[512];
  kpls3     =new int[512];

  y2=(timageInOut->height/2);
  y4=(timageInOut->height/4);
  x2=(timageInOut->width/2);
  x4=(timageInOut->width/4);
    
  //----- Set plasm r,g,b buffers -------------------(x)-;
  max=0;
  for (i=0;i<512;i++)
   {g=(sin(Pi__*i/256)*(1-4*cos(Pi__*i/256)));
    tmptab[i]=g;
    if (g<0) g=(0-g);
    if (max<g) max=g;
    }  
  max1=(256/max);
  max2=(128/max); 
    for (i=0;i<512;i++) 
   { kpls1[i]=(int)(256+max1*tmptab[i]);
     pls1[i]=(int)(128+max2*tmptab[i]);
   }  

  max=0;
  for (i=0;i<512;i++)
   {g=(sin(Pi__*i/256)*(1-2*sin(2*Pi__*i/256)));
    tmptab[i]=g;
    if (g<0) g=(0-g);
    if (max<g) max=g;
   }  
  max1=(256/max);
  max2=(128/max); 
  for (i=0;i<512;i++) 
   { kpls2[i]=(int)(256+max1*tmptab[i]);
     pls2[i]=(int)(128+max2*tmptab[i]);
   } 

  max=0;
  for (i=0;i<512;i++)
   {g=(cos(2*Pi__*i/256)*(-1+4*sin(Pi__*i/256)));
    if (g<0) g=(0-g);
    tmptab[i]=g;
    if (max<g) max=g;
   }  
  max1=(256/max);
  max2=(128/max); 
  for (i=0;i<512;i++) 
  { 
	 kpls3[i]=(int)(256+max1*tmptab[i]);
     pls3[i]=(int)(128+max2*tmptab[i]);
  } 
  k1=0; 
  k2=0;   

  initialized=true;
}

void myefx::free()
{
  delete [] pls1;     
  delete [] pls2;     
  delete [] pls3;     
  delete [] kpls1;     
  delete [] kpls2;  
  delete [] kpls3;

  initialized=false;
}

void myefx::paint(double pos)
{
  unsigned char *img=timageInOut->uchar8ptr;
  
 s1=kpls1[(k2)&511];
 s2=kpls1[(k1)&511];
 
 k1+=int(Controller_k1->getValue(float(pos)));
 k2+=int(Controller_k2->getValue(float(pos)));
 

 for (y=0; y<timageInOut->height; y++) 
 { 
   k3 = (s1+y  -y2)>>1 ;
   k5 = (k1+y/2-y4)<<1;
   for (x=0; x<timageInOut->width; x++) 
   { 
	 k4 = (s2+x  -x2)>>1;
     k6 = (k2+x/2-x4)<<1;
	 d1=(s1+kpls1[(( kpls1[(16+k2+k3-k6)&511] + kpls2[(   k4-k5)&511]))&511])>>2;
     d2=(s2+kpls2[(( kpls1[(   k1+k4-k5)&511] + kpls2[(32+k3+k6)&511]))&511])>>2;
    								  
     d2+=d1;   
   	 (*img) = pls1[d2 & 255];
	 img++;									 
     (*img) = pls1[(d2+8) & 255];     
     img++;
	 (*img) = pls1[(d2+16) & 255];
	 img+=2;
   }
 }
}
