#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "object3D.h"
#include "WSMobj.h"

#define MAXWSM 20

typedef struct
{
  AD_Vect3D pos, vel;
  float age;
} AD_Particle;


class AD_ParticleSystem: public AD_Object3D
{
public:
  AD_Particle *particles;
  int max_particles;
  int live_particles;
  float life;
  float start_time, end_time;
  float emitter_width, emitter_height;
  float emitter_speed, emitter_variation;
  float prev_framepos;
  float step_time, max_birth;
  int numWSM;
  char **WSMNames;
  // array dei nomi dei WSM (per linkare), e array di puntatori
  // agli effettivi WSMObject
  AD_WSMObject_ptr *WSMlinked;


  AD_ParticleSystem();
  // metodi virtuali derivati da AD_Object3D
  int init(void);
  void paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight);
  void build_objectmatrix (float4 framepos);

  virtual void BirthParticle(int quale);
};

#define Spray_Partcle_System AD_ParticleSystem

#endif