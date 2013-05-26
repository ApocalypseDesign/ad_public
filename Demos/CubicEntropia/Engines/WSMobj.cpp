#include <math.h>
#include "WSMobj.h"
#include "..\Math\fastmath.h"

#define forceScaleFactor  float(1200*1200)/float(160*160)

// ***********************************************************
// ******************       VENTO      ***********************
// ***********************************************************
int AD_WindModifier::init_from_A3D(void)
{
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);

  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0) &&
	  (father==(AD_Object3D *)NULL) && (have_childrens<=0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }
  if (decay < 0.0f) decay = 0.0f;
  return(0);
}


void AD_WindModifier::build_objectmatrix (float4 framepos)
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
   mat_get_row(&currentmatrix, 1, &forza);
   vect_auto_normalize(&forza);
   vect_scale(&forza, strenght*0.00016f*forceScaleFactor, &forza);
}


void AD_WindModifier::paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight)
{
   build_objectmatrix(pos);
}


void AD_WindModifier::Force (float framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel)
{
  AD_Vect3D posrel, tf, p;
  float dist, freq, turb, stren;

  freq=frequency;
  turb=turbolence;
  stren=strenght;


  if (mapping==FORCE_PLANAR)
  {
    if (decay!=0.0f)
	{
	  vect_sub_inline(pos, &currentpos, &posrel);
	  dist=vect_dot(&forza, &posrel);
      dist = fabsf(dist);
      stren *= (float)exp(-decay*dist);
	}			
  } 
  else
  {
    vect_sub_inline(pos, &currentpos, &forza);
    dist = vect_length(&forza);
    if (dist!=0.0f) vect_scale_inline(&forza, 1.0f/dist, &forza);
    if (decay!=0.0f)
      stren *= (float)exp(-decay*dist);
    vect_scale_inline(&forza, stren*0.00016f*forceScaleFactor, &forza);
  }

  if (turb!=0.0f)
  {
    vect_sub_inline(pos, &currentpos, &posrel);	
    freq *= 0.01f;
    turb *= 0.0001f * forceScaleFactor;
    p.x  = freq * float(framepos);
    tf.x = scale*rand()*p.x;
    p.y  = freq * float(framepos);
    tf.y = scale*rand()*p.y;
    p.z  = freq * float(framepos);
    tf.z = scale*rand()*p.z;

    vect_scale_inline(&tf, turb, &tf);
	vect_add_inline(&forza, &tf, accel);
  }
  else
  {
    accel->x=forza.x;
    accel->y=forza.y;
    accel->z=forza.z;
  }
}



// ***********************************************************
// ******************      GRAVITA'    ***********************
// ***********************************************************
int AD_GravityModifier::init_from_A3D(void)
{
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);

  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0) &&
	  (father==(AD_Object3D *)NULL) && (have_childrens<=0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }
  if (decay <0.0f) decay = 0.0f;
  return(0);
}


void AD_GravityModifier::build_objectmatrix (float4 framepos)
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
   mat_get_row(&currentmatrix, 1, &forza);
   vect_auto_normalize(&forza);
   vect_scale_inline(&forza, strenght * 0.00016f * forceScaleFactor, &forza);
}


void AD_GravityModifier::paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight)
{
   build_objectmatrix(pos);
}


void AD_GravityModifier::Force (float framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel)
{
    AD_Vect3D posrel;
    float dist, stren;

    stren=strenght;

    if (mapping==FORCE_PLANAR)
	{
	  if (decay!=0.0f)
	  {
	    vect_sub_inline(pos, &currentpos, &posrel);
        dist = vect_dot(&forza, &posrel);
	    stren *= (float)exp(-decay*dist);
	  }
	  accel->x=forza.x;
	  accel->y=forza.y;
	  accel->z=forza.z;
	}
    else
	{
	  vect_sub_inline(&currentpos, pos, &forza);
      dist = vect_fast_lenght(&forza);
      if (dist!=0.0f) vect_scale_inline(&forza, 1.0f/dist, &forza);
      if (decay!=0.0f) 
        stren *= (float)exp(-decay*dist);
      vect_scale_inline(&forza, stren * 0.00016f * forceScaleFactor, accel);
	}
}



// ***********************************************************
// ******************       BOMBA      ***********************
// ***********************************************************
int AD_BombModifier::init_from_A3D(void)
{
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);

  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0) &&
	  (father==(AD_Object3D *)NULL) && (have_childrens<=0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }
  return(0);
}


void AD_BombModifier::build_objectmatrix (float4 framepos)
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


void AD_BombModifier::paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight)
{
   build_objectmatrix(pos);
}


void AD_BombModifier::Force (float framepos, AD_Vect3D *pos, AD_Vect3D *vel, AD_Vect3D *accel)
{
}