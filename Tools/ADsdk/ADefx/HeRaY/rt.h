
#define NUM_SPHERES 3
#define NUM_LIGHTSOMNI 3
#define MAX_REFLECTIONS 5
#define MAX_DIST 999999999.0f
#define PI 3.14159
#define SKIP 8
//#define ANTIALIAS

class vector3D
{
public:
	float x,y,z;

	float normalize();
	friend vector3D operator+(vector3D first, vector3D second);
	friend vector3D operator-(vector3D first, vector3D second);
	friend float operator*(vector3D first, vector3D second); //dot
	friend vector3D operator^(vector3D first, vector3D second); //vect
	friend vector3D operator*(vector3D first, float second);
	friend vector3D operator/(vector3D first, float second);
};

class Ray
{
public:
	vector3D startpos,direction;
	float length;
};

class LightOmni
{
public:
	vector3D center,diffusecolor,ambientcolor,specularcolor;

	LightOmni()
	{
		ambientcolor.x=ambientcolor.y=ambientcolor.z=12.0f;
		diffusecolor.x=diffusecolor.y=diffusecolor.z=128.0f;
		specularcolor.x=specularcolor.y=specularcolor.z=128.0f;
	}
};

class Universe;

class Sphere
{
public:
	int ID;
	vector3D center,diffusecolor,ambientcolor,specularcolor;
	float radius,shiness;

	Sphere()
	{
		ambientcolor.x=ambientcolor.y=ambientcolor.z=12.0f;
		diffusecolor.x=diffusecolor.y=diffusecolor.z=128.0f;
		specularcolor.x=specularcolor.y=specularcolor.z=255.0f;
		shiness=4.0f;
	}
	bool intersect(Ray *inray,Universe *univ,float *dist);
	bool raytrace(Ray *inray,Universe *univ,int maxreflections,unsigned int *color,float *dist);
};

class Universe
{
public:
	int numSpheres;
	Sphere Spheres[NUM_SPHERES];
	int numLightsOmni;
	LightOmni LightsOmni[NUM_LIGHTSOMNI];
	//vector3D viewVector;

	Universe()
	{
		int i;

		numSpheres=NUM_SPHERES;
		numLightsOmni=NUM_LIGHTSOMNI;

		for (i=0; i<numSpheres; i++)
			Spheres[i].ID=i;
	}
	bool intersect(Ray *inray,int excludedobj);
	bool raytrace(Ray *inray,int excludedobj,int maxreflections,unsigned int *color);
};

