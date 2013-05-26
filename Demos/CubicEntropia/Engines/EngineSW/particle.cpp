#include "..\..\Math\fastmath.h"
#include "..\particle.h"

extern float znear, rcpznear, screen_Xadd, screen_Yadd;
extern AD_Tria3D **list_to_paint_trasparent;  // lista dei triangoli trasparenti
extern HWND miawin;         // finestra principale; usata qui per le debug_error

#define VEL_SCALE (-0.020f*1200.0f/160)
#define VAR_SCALE (0.020f*1200.0f/160)

AD_ParticleSystem::AD_ParticleSystem()
{
  particles=(AD_Particle *)NULL;
  max_particles=live_particles=0;
  life=start_time=emitter_width=emitter_height=
  emitter_speed=emitter_variation=0;
  end_time=10000;
}


void AD_ParticleSystem::BirthParticle(int quale)
{
  // RAND_MAX e' 0x7fff
  AD_Vect3D vel, pos;

  vel.x=vel.z=0;
  vel.y=emitter_speed;
  if (emitter_variation != 0)
  {
	vel.x = -emitter_variation + float(rand())/float(RAND_MAX) * 2.0f*emitter_variation;
	vel.z = -emitter_variation + float(rand())/float(RAND_MAX) * 2.0f*emitter_variation;
	vel.y = emitter_speed - emitter_variation + float(rand())/float(RAND_MAX) * 2.0f*emitter_variation;
  }

  particles[quale].age=0;

  mat_mulvectaffine(&currentmatrix_rot, &vel, &particles[quale].vel);
  pos.x = -emitter_width*0.5f + float(rand())/float(RAND_MAX) * emitter_width;
  pos.z = -emitter_height*0.5f + float(rand())/float(RAND_MAX) * emitter_height;
  pos.y = 0.0f;
  mat_mulvect(&currentmatrix, &pos, &particles[quale].pos);
}


int AD_ParticleSystem::init(void)
{
  int i;

  emitter_speed*=VEL_SCALE;
  emitter_variation*=VAR_SCALE;
  live_particles=0;
  prev_framepos=start_time;
  step_time=max_birth=0;

  // un solo vertice3D, usato solo per il campo flag in modo che nella
  // scene3D.cpp (do_efx) quando fa il controllo se sono da clippare
  // nella znear lo passano sempre come dev'essere
  num_vertex3D=1;
  vertex3D=new AD_Vertex3D;
  vertex3D[0].flags=0;

  num_vertex2D=max_particles*4;
  vertex2D=new AD_Vertex2D[num_vertex2D];
  num_tria=max_particles*2;
  tria=new AD_Tria3D[num_tria];
  particles=new AD_Particle[max_particles];

  for (i=0; i<max_particles; i++)
  {
    tria[i*2+0].materiale=&matflare;
    tria[i*2+0].sp1=&vertex2D[i*4+0];
    tria[i*2+0].sp2=&vertex2D[i*4+2];
    tria[i*2+0].sp3=&vertex2D[i*4+3];
    tria[i*2+0].sp1->u=0.0f*TEXTURE_PRECISION;
    tria[i*2+0].sp1->v=0.0f*TEXTURE_PRECISION;
    tria[i*2+0].sp2->u=0.0f*TEXTURE_PRECISION;
    tria[i*2+0].sp2->v=0.99f*TEXTURE_PRECISION;
    tria[i*2+0].sp3->u=0.99f*TEXTURE_PRECISION;
    tria[i*2+0].sp3->v=0.99f*TEXTURE_PRECISION;
    tria[i*2+0].v1=tria[i*2+0].v2=tria[i*2+0].v3=&vertex3D[0];

	tria[i*2+1].materiale=&matflare;
    tria[i*2+1].sp1=&vertex2D[i*4+3];
    tria[i*2+1].sp2=&vertex2D[i*4+1];
    tria[i*2+1].sp3=&vertex2D[i*4+0];
    tria[i*2+1].sp1->u=0.99f*TEXTURE_PRECISION;
    tria[i*2+1].sp1->v=0.99f*TEXTURE_PRECISION;
    tria[i*2+1].sp2->u=0.99f*TEXTURE_PRECISION;
    tria[i*2+1].sp2->v=0.0f*TEXTURE_PRECISION;
    tria[i*2+1].sp3->u=0.0f*TEXTURE_PRECISION;
    tria[i*2+1].sp3->v=0.0f*TEXTURE_PRECISION;
    tria[i*2+1].v1=tria[i*2+1].v2=tria[i*2+1].v3=&vertex3D[0];

	particles[i].age=-1;
    vect_set(&particles[i].pos, 0, 0, 0);
    vect_set(&particles[i].vel, 0, 0, 0);
  }
  

  // setto il materiale
  matflare.texture_ptr=flare;
  matflare.flags=(0 | PAINT_TEXTURE | IS_TRASPARENT);
  matflare.trasparencytype=MIXTYPE_ADD;
  AddUpdate_Material(&matflare);

  if (flare!=(texture *)NULL)
  {
    latoX=(float)flare->dimx;
    latoY=(float)flare->dimy;
    for (i=0; i<num_vertex2D; i++)
	{
	   vertex2D[i].u*=latoX;
	   vertex2D[i].v*=latoY;
	}
  }


  // ############  INIZIALIZZAZIONI DELLE KEYFRAMER  ##########
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);
  return(0);
}


void AD_ParticleSystem::build_objectmatrix (float4 framepos)
{
   AD_Vect3D postmp, stmp;
   AD_Quaternion objrot;
   AD_Matrix posttrans, scaling, maux;

   accum_scale.x=accum_scale.y=accum_scale.z=1.0f;
   mat_identity(&currentmatrix_rot);

   // estrazione dei dati col keyframer: niente di piu' facile col c++ !!!
   if (rotationtrack.numkey>0)
   {
     rotationtrack.get_data(framepos, &objrot);
     quat_rotquat_to_matrix(&objrot, &currentmatrix_rot);
   }
   mat_copy(&currentmatrix_rot, &currentmatrix);

   if (scaletrack.numkey>0)
   {
     scaletrack.get_data(framepos, &stmp);
     mat_setmatrix_of_scaling(&scaling, stmp.x, stmp.y, stmp.z);
	 accum_scale.x*=stmp.x;
	 accum_scale.y*=stmp.y;
	 accum_scale.z*=stmp.z;
   }
   else mat_identity(&scaling);
   
   if (positiontrack.numkey>0) positiontrack.get_data(framepos, &currentpos);
   mat_setmatrix_of_pretraslation(&posttrans, &currentpos);

   mat_mul(&scaling, &currentmatrix_rot, &maux);
   mat_mul(&posttrans, &maux, &currentmatrix);
 
   if (father!=(AD_Object3D *)NULL)
   {
     mat_mulaffine(&father->currentmatrix_rot, &currentmatrix_rot, &currentmatrix_rot);
	 mat_mul(&father->currentmatrix, &currentmatrix, &currentmatrix);
     mat_mulvect(&father->currentmatrix, &currentpos, &postmp);
	 vect_copy(&postmp, &currentpos);
	 
	 accum_scale.x*=father->accum_scale.x;
	 accum_scale.y*=father->accum_scale.y;
	 accum_scale.z*=father->accum_scale.z;
   }

   mat_transpose(&currentmatrix_rot, &inverse_rotmatrix);
}


void AD_ParticleSystem::paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight)
{
  float dt, nlX, nlY, invz, mb;
  int i, mbi, j, tindex;
  AD_Vect3D cam_pos, forza_acc;
  
  build_objectmatrix(pos);
  if (pos<start_time) return;
  dt=pos-prev_framepos;
  if (pos>end_time)
  {
	  mbi=0;
	  goto birthday;
  }

  if (step_time+dt>=1)
  {
    mb=(step_time+dt)*max_particles/life;
	if (mb >= max_particles/life)
		 mb = max_particles/life;
	step_time=0;
	if (mb>=1)
	{
	  mbi=myfist(mb);
	  max_birth=0;
	}
	else
	{
	  max_birth+=mb;
	  mbi=myfist(max_birth);
	  if (mbi>=1) max_birth=0;
	}
  }
  else
  {
	step_time+=dt;
	mbi=0;
  }

birthday:
;
  for (i=0; i<max_particles; i++)
  {
	if (particles[i].age >= 0)
      particles[i].age+=dt;
	if (particles[i].age >= life)
      particles[i].age=-1;
  }


  for (i=0; ((i<max_particles) && (mbi>0)); i++)
  {
	if (particles[i].age < 0)
	{
	    BirthParticle(i);
		mbi--;
	}
  }


  tindex=0;
  for (i=0; i<max_particles; i++)
  {
	if (particles[i].age > 0)
	{
      // il particle procede, eventualmente deflesso
	  // da modificatori (tipo vento); i modificatori
	  // hanno effetto sulla velocita'

	  for (j=0; j<numWSM; j++)
	  {
		 WSMlinked[j]->Force(pos, &particles[i].pos, &particles[i].vel, &forza_acc);
         particles[i].vel.x+=forza_acc.x*dt;
         particles[i].vel.y+=forza_acc.y*dt;
         particles[i].vel.z+=forza_acc.z*dt;
	  }

      particles[i].pos.x+=particles[i].vel.x*dt;
      particles[i].pos.y+=particles[i].vel.y*dt;
      particles[i].pos.z+=particles[i].vel.z*dt;
      mat_mulvect(&telecamera->currentmatrix, &particles[i].pos, &cam_pos);
      if ((cam_pos.z > telecamera->znear) &&
	      (cam_pos.z < telecamera->zfar))
	  {
        // proietto in 2D su schermo
	    invz=1.0f/cam_pos.z;
	    nlX=(flare_scale_x*latoX)/(cam_pos.z*cam_pos.z + 1.0f);
        nlY=(flare_scale_y*latoY)/(cam_pos.z*cam_pos.z + 1.0f);
		if (nlX > 2.0f*latoX) nlX=2.0f*latoX;
		if (nlY > 2.0f*latoY) nlY=2.0f*latoY;
        cam_pos.x=telecamera->prospettivaX*(cam_pos.x*invz) + (telecamera->screenX);
        cam_pos.y=(telecamera->screenY) - telecamera->prospettivaY*(cam_pos.y*invz);
        cam_pos.x-=nlX*0.5f;
        cam_pos.y-=nlY*0.5f;

	    // mi calcolo i punti dei 2 triangoli
        vertex2D[tindex].xs=cam_pos.x;
        vertex2D[tindex].ys=cam_pos.y;
        vertex2D[tindex].dist=invz;

        vertex2D[tindex+1].xs=cam_pos.x + nlX;
        vertex2D[tindex+1].ys=cam_pos.y;
        vertex2D[tindex+1].dist=invz;

        vertex2D[tindex+2].xs=cam_pos.x;
        vertex2D[tindex+2].ys=cam_pos.y + nlY;
        vertex2D[tindex+2].dist=invz;

        vertex2D[tindex+3].xs=cam_pos.x + nlX;
        vertex2D[tindex+3].ys=cam_pos.y + nlY;
        vertex2D[tindex+3].dist=invz;

        tria[i*2].mid_z=cam_pos.z;
        tria[i*2+1].mid_z=cam_pos.z;
	    *list_to_paint_trasparent++=&tria[i*2];
	    *list_to_paint_trasparent++=&tria[i*2+1];
	  }
	}
	tindex+=4;
  }
  prev_framepos=pos;
}