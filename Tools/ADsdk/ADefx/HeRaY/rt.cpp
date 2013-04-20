#include <math.h>
#include "rt.h"

inline float noz(float n)
{
	if (n==0.0f) return 0.0000001f;
	return n;
}

float vector3D::normalize()
{
	float module;
	
	module=x*x+y*y+z*z;
	if (module<=0.0f) return module;

	module=noz(float(sqrt(module)));
	x/=module;
	y/=module;
	z/=module;

	return module;
}

vector3D operator+(vector3D first, vector3D second)
{
	first.x+=second.x;
	first.y+=second.y;
	first.z+=second.z;

	return first;
}

vector3D operator-(vector3D first, vector3D second)
{
	first.x-=second.x;
	first.y-=second.y;
	first.z-=second.z;

	return first;
}

float operator*(vector3D first, vector3D second)
{
	float ftemp;

	ftemp=first.x*second.x+
		  first.y*second.y+
		  first.z*second.z;

	return ftemp;
}

vector3D operator^(vector3D first, vector3D second)
{
	first.x=first.x*second.x+first.x*second.y+first.x*second.z;
	first.y=first.y*second.x+first.y*second.y+first.y*second.z;
	first.z=first.z*second.x+first.z*second.y+first.z*second.z;

	return first;
}

vector3D operator*(vector3D first, float second)
{
	first.x*=second;
	first.y*=second;
	first.z*=second;

	return first;
}

vector3D operator/(vector3D first, float second)
{
	second=noz(second);

    first.x/=second;
	first.y/=second;
	first.z/=second;

	return first;
}

bool Sphere::intersect(Ray *inray,Universe *univ,float *dist)
{
	vector3D vtemp;
	float b,c; //  x^2 + by + c = 0
	float t1,t2,delta; // intersezioni 

	//b=inray.direction*(inray.startpos-center);

	vtemp=inray->startpos-center;

	b=inray->direction*vtemp;

	//c=(inray.startpos-center)^2 - radius^2

	c=vtemp*vtemp-radius*radius;

	delta=b*b-c;

	if (delta<=0.0f) return false;

	delta=float(sqrt(delta));

	t1=-b+delta;
    t2=-b-delta;

	if ((t1<=0.0f) || (t2<=0.0f)) return false;
	if (t2<t1) t1=t2;

	*dist=t1;
	return true;
}

bool Sphere::raytrace(Ray *inray,Universe *univ,int maxreflections,unsigned int *color,float *dist)
{
	vector3D vtemp,intersection,normal;
	Ray rayreflect,raytolight;
	unsigned int reflectcolor;
	float b,c; //  x^2 + by + c = 0
	float t1,t2,delta; // intersezioni 
	float angle; // angolo tra la normale e la luce
	float cr,cg,cb;
	unsigned int ir,ig,ib;
	int i;


	//b=inray.direction*(inray.startpos-center);

	vtemp=inray->startpos-center;

	b=inray->direction*vtemp;

	//c=(inray.startpos-center)^2 - radius^2

	c=vtemp*vtemp-radius*radius;

	delta=b*b-c;

	if (delta<=0.0f) return false;

	delta=float(sqrt(delta));

	t1=-b+delta;
    t2=-b-delta;

	if ((t1<=0.0f) || (t2<=0.0f)) return false;
	if (t2<t1) t1=t2;

	// ora t1 contiene la distanza del punto di intersezione + vicino;
	
	intersection=inray->startpos+inray->direction*t1; 

	normal=(intersection-center)/radius;

	cr=ambientcolor.x;
	cg=ambientcolor.y;
	cb=ambientcolor.z;

	rayreflect.startpos=intersection;
	rayreflect.direction=inray->direction-normal*((normal*inray->direction)*2.0f);
	rayreflect.direction.normalize();
	rayreflect.length=0;

	for(i=0; i<univ->numLightsOmni; i++)
	{
		// *** controllo se illuminato ***
		raytolight.startpos=intersection;
		raytolight.direction=univ->LightsOmni[i].center-intersection;
		raytolight.direction.normalize();
        //if (raytolight.direction.normalize()<radius) continue;
		if (!univ->intersect(&raytolight,ID)) //riga incriminata
		{
			// *** shading ***
			angle=normal*raytolight.direction;
	
			if (angle>0.0f)
			{
				cr+=float(univ->LightsOmni[i].diffusecolor.x*angle*diffusecolor.x/255.0);
				cg+=float(univ->LightsOmni[i].diffusecolor.y*angle*diffusecolor.y/255.0);
				cb+=float(univ->LightsOmni[i].diffusecolor.z*angle*diffusecolor.z/255.0);
			}

			angle=raytolight.direction*rayreflect.direction;
			if (angle>0.0f)
			{
				angle=float(pow(angle,shiness));
				cr+=float(univ->LightsOmni[i].specularcolor.x*angle*specularcolor.x/255.0);
				cg+=float(univ->LightsOmni[i].specularcolor.y*angle*specularcolor.y/255.0);
				cb+=float(univ->LightsOmni[i].specularcolor.z*angle*specularcolor.z/255.0);
			}

		}
	}
	if (cr>255.0f) cr=255.0f;
	if (cg>255.0f) cg=255.0f;
	if (cb>255.0f) cb=255.0f;

	*color=unsigned int(cb)+(unsigned int(cg)<<8)+(unsigned int(cr)<<16);

	if (maxreflections>0)
	{
		if (!univ->raytrace(&rayreflect,ID,maxreflections-1,&reflectcolor))
			reflectcolor=0;
		reflectcolor=(reflectcolor & 0xFEFEFE)>>1;
		ir=(*color & 0xFF0000)+(reflectcolor & 0xFF0000);
		ig=(*color & 0xFF00)+(reflectcolor & 0xFF00);
		ib=(*color & 0xFF)+(reflectcolor & 0xFF);
		if (ir>0xFF0000) ir=0xFF0000;
		if (ig>0xFF00) ig=0xFF00;
		if (ib>0xFF) ib=0xFF;

		*color=ir+ig+ib;
	}

	*dist=t1;
	
	return true;
}

bool Universe::intersect(Ray *inray,int excludedobj)
{
	int i;
	float dist;//,mindist;

	//mindist=dist=MAX_DIST;
	for (i=0; i<numSpheres; i++)
	{
		if (i==excludedobj) continue;
		if (Spheres[i].intersect(inray,this,&dist)) 
		{
			return true;
			//if (dist<mindist) *color=tmpcolor;
		}
		//if (dist<mindist) mindist=dist;
	}

	return false;
	//return (mindist<MAX_DIST);
}

bool Universe::raytrace(Ray *inray,int excludedobj,int maxreflections,unsigned int *color)
{
	int i;
	unsigned int tmpcolor;
	float dist,mindist;

	mindist=dist=MAX_DIST;
	for (i=0; i<numSpheres; i++)
	{
		if (i==excludedobj) continue;
		if (Spheres[i].raytrace(inray,this,maxreflections,&tmpcolor,&dist)) 
		{
			if (dist<mindist) *color=tmpcolor;
		}
		if (dist<mindist) mindist=dist;
	}

	return (mindist<MAX_DIST);
}