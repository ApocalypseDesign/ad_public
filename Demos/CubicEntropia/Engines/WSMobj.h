#ifndef _WSMOBJ_H_
#define _WSMOBJ_H_

#include "object3D.h"

class AD_WSMObject: public AD_Object3D
{
public:
    AD_WSMObject() { }
	virtual void Force (float framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel) { }
};

typedef AD_WSMObject *AD_WSMObject_ptr;

#define FORCE_PLANAR     0
#define FORCE_SPHERICAL  1

// ***********************************************************
// ******************       VENTO      ***********************
// ***********************************************************
class AD_WindModifier: public AD_WSMObject
{
public:
  float strenght, decay, turbolence;
  float frequency, scale;
  int mapping;
  AD_Vect3D forza;  // estratto dalla currentmatrix

  AD_WindModifier() { }
  // metodi virtuali derivati da AD_Object3D
  int init_from_A3D(void);
  void paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight);
  void build_objectmatrix (float4 framepos);

  // Metodo usato dai particle system
  void Force (float framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel);
};




// ***********************************************************
// ******************      GRAVITA'    ***********************
// ***********************************************************
class AD_GravityModifier: public AD_WSMObject
{
public:
  float strenght, decay;
  int mapping;
  AD_Vect3D forza;  // estratto dalla currentmatrix

  AD_GravityModifier() { }

  // metodi virtuali derivati da AD_Object3D
  int init_from_A3D(void);
  void paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight);
  void build_objectmatrix (float4 framepos);

  // Metodo usato dai particle system
  void Force (float framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel);
};




// ***********************************************************
// ******************       BOMBA      ***********************
// ***********************************************************
class AD_BombModifier: public AD_WSMObject
{
public:
  float strenght, chaos, range;
  float frequency, scale;
  int mapping, start_time, duration;
  int unlimited_range;
  

  // metodi virtuali derivati da AD_Object3D
  int init_from_A3D(void);
  void paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight);
  void build_objectmatrix (float4 framepos);

  // Metodo usato dai particle system
  void Force (float framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel);
};

#endif
