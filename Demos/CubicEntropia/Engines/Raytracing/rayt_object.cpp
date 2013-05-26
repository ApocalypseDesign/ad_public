// Metodi della classe AD_Object3D
// By Here & [Turbo]

#include <windows.h>
#include <math.h>
#include "..\..\utility.h"
#include "..\..\Math\matrix.h"
#include "..\..\Math\fastmath.h"
#include "rayt_object.h"


extern HWND miawin;         // finestra principale; usata qui per le debug_error
extern FILE *fdeb;

// ---------------------------------------------------------
AD_RaytObject::AD_RaytObject()   // costruttore
{

   flare=(texture *)NULL;
   latoX=latoY=0;
   flare_scale_x=flare_scale_y=160.0f;

   father_name=name=(char *)NULL;
   father=(AD_RaytObject *)NULL;
   have_childrens=0;
}


void AD_RaytObject::build_objectmatrix (float4 framepos)
// costruisce la matrice di trasformazione, che servira' poi per trasformare
// i vertici dell'oggetto;
{
   AD_Vect3D postmp, stmp;
   AD_Quaternion objrot;
   AD_Matrix posttrans, scaling, maux;

   accum_scale.x=accum_scale.y=accum_scale.z=1.0f;

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
 
   if (father!=(AD_RaytObject *)NULL)
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


// ===================================================================
// **************************     SFERA     **************************
// ===================================================================
AD_RaytSphere::AD_RaytSphere()
{
}


int AD_RaytSphere::init(void)
{
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);
  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0) &&
	  (father==(AD_RaytObject *)NULL) && (have_childrens<=0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }
  sqr_radius=radius*radius;
  inv_radius=1.0f/radius;
  return (1);
}


void AD_RaytSphere::update(float framepos, AD_Vect3D *first_hit)
{
  build_objectmatrix(framepos);
  vect_sub(first_hit, &currentpos, &first_hit_pos);
  first_hit_c=vect_dot(&first_hit_pos, &first_hit_pos) - sqr_radius;
}


int AD_RaytSphere::intersect(AD_Ray *ray)
{
 AD_Vect3D diff;
 float t0, t1, b, c, d, s;

 /*** Calcoliamo i valori dei coefficienti dell'equazione ***/
 vect_sub(&ray->base, &currentpos, &diff);

 b = vect_dot(&ray->direction, &diff);
 c = vect_dot(&diff, &diff) - sqr_radius;
 d = (b * b) - c;

 if (d < 0) return(0);

 /*** Le soluzioni esistono ***/
 s=mysqrt(d);
 t0 = (-b + s);
 t1 = (-b - s);

 if ((t0 > 1.0) || (t1 > 1.0))
 {
   if (t1 > 1.0) ray->distance = t1;
   if ((t0 < t1) && (t0 > 1.0)) ray->distance = t0;
   return 1;
 }
 return(0);
}


int AD_RaytSphere::intersect_first(AD_Ray *ray)
{
 float t0, t1, b, d, s;

 b = vect_dot(&ray->direction, &first_hit_pos);
 d = (b * b) - first_hit_c;
 if (d < 0) return(0);

 /*** Le soluzioni esistono ***/
 s=mysqrt(d);
 t0 = (-b + s);
 t1 = (-b - s);

 if ((t0 > 1.0) || (t1 > 1.0))
 {
   if (t1 > 1.0) ray->distance = t1;
   if ((t0 < t1) && (t0 > 1.0)) ray->distance = t0;
   return 1;
 }
 return(0);
}


void AD_RaytSphere::get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal)
{
   vect_sub(hitpoint, &currentpos, normal);
   vect_scale(normal, inv_radius, normal);
}


// ===================================================================
// ***************************     BOX     ***************************
// ===================================================================
AD_RaytBox::AD_RaytBox()
{
}

int AD_RaytBox::init(void)
{
  int i;

  if (positiontrack.init()==-1) return(-1);
  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0) &&
	  (father==(AD_RaytObject *)NULL) && (have_childrens<=0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);

  vect_set(&normals[BOX_UP], 0, height/2.0f, 0);
  vect_set(&normals[BOX_DOWN], 0, -height/2.0f, 0);
  vect_set(&normals[BOX_LEFT], -width/2.0f, 0, 0);
  vect_set(&normals[BOX_RIGHT], width/2.0f, 0, 0);
  vect_set(&normals[BOX_FRONT], 0, 0, -lenght/2.0f);
  vect_set(&normals[BOX_BACK], 0, 0, lenght/2.0f);

  for (i=0; i<6; i++) vect_auto_normalize(&normals[i]);

  minb.x=-width/2.0f;
  minb.y=-height/2.0f;
  minb.z=-lenght/2.0f;
  maxb.x=width/2.0f;
  maxb.y=height/2.0f;
  maxb.z=lenght/2.0f;
  return(1);
}


void AD_RaytBox::update(float framepos, AD_Vect3D *first_hit)
{
  int i;
  AD_Vect3D p;

  build_objectmatrix(framepos);
  vect_sub(first_hit, &currentpos, &p);
  mat_mulvectaffine(&inverse_rotmatrix, &p, &first_hit_pos);

  for (i=0; i<6; i++)
  {
	 mat_mulvectaffine(&currentmatrix_rot, &normals[i], &normals_tr[i]);
  }
}


int AD_RaytBox::intersect(AD_Ray *ray)
{
   float tnear, tfar;
   float t1, t2, tswap, k;
   AD_Vect3D base0, dir0, aux;
   int f;

#define SWAP_T \
{ \
   tswap=t1; \
   t1=t2; \
   t2=tswap; \
} \

   mat_mulvectaffine(&inverse_rotmatrix, &ray->direction, &dir0);
   // controllo se i piani sono paralleli alla direzione
   if ((dir0.x==0) || (dir0.y==0) || (dir0.z==0)) return(0);

   vect_sub(&ray->base, &currentpos, &aux);
   mat_mulvectaffine(&inverse_rotmatrix, &aux, &base0);
   tnear=-1E10;
   tfar=1E10;

   // piani X
   k=1.0f/dir0.x;
   t1=(minb.x-base0.x)*k;
   t2=(maxb.x-base0.x)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   f=BOX_RIGHT;
   }
   else f=BOX_LEFT;
   if (t1 > tnear)
   {
	  tnear=t1;
      face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

   // piani Y
   k=1.0f/dir0.y;
   t1=(minb.y-base0.y)*k;
   t2=(maxb.y-base0.y)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   f=BOX_UP;
   }
   else f=BOX_DOWN;
   if (t1 > tnear)
   {
	  tnear=t1;
      face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

   // piani Z
   k=1.0f/dir0.z;
   t1=(minb.z-base0.z)*k;
   t2=(maxb.z-base0.z)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   f=BOX_BACK;
   } else f=BOX_FRONT;
   if (t1 > tnear)
   {
	  tnear=t1;
      face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

   if (tnear>=1)
   {
     ray->distance=tnear;
     return(1);
   }
   else return(0);
}


int AD_RaytBox::intersect_first(AD_Ray *ray)
{
   float tnear, tfar;
   float t1, t2, tswap, k;
   AD_Vect3D dir0;
   int f;

#define SWAP_T \
{ \
   tswap=t1; \
   t1=t2; \
   t2=tswap; \
} \

   mat_mulvectaffine(&inverse_rotmatrix, &ray->direction, &dir0);
   // controllo se i piani sono paralleli alla direzione
   if ((dir0.x==0) || (dir0.y==0) || (dir0.z==0)) return(0);


   tnear=-1E10;
   tfar=1E10;

   // piani X
   k=1.0f/dir0.x;
   t1=(minb.x-first_hit_pos.x)*k;
   t2=(maxb.x-first_hit_pos.x)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   f=BOX_RIGHT;
   }
   else f=BOX_LEFT;
   if (t1 > tnear)
   {
	  tnear=t1;
      face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

   // piani Y
   k=1.0f/dir0.y;
   t1=(minb.y-first_hit_pos.y)*k;
   t2=(maxb.y-first_hit_pos.y)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   f=BOX_UP;
   }
   else f=BOX_DOWN;
   if (t1 > tnear)
   {
	  tnear=t1;
      face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

   // piani Z
   k=1.0f/dir0.z;
   t1=(minb.z-first_hit_pos.z)*k;
   t2=(maxb.z-first_hit_pos.z)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   f=BOX_BACK;
   } else f=BOX_FRONT;
   if (t1 > tnear)
   {
	  tnear=t1;
      face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

   if (tnear>=1)
   {
     ray->distance=tnear;
     return(1);
   }
   else return(0);
}


void AD_RaytBox::get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal)
{
   vect_copy(&normals_tr[face_hitted], normal);
}


// ===================================================================
// **************************     PIANO     **************************
// ===================================================================
AD_RaytPlane::AD_RaytPlane()
{
}

int AD_RaytPlane::init(void)
{
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);
  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0) &&
	  (father==(AD_RaytObject *)NULL) && (have_childrens<=0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }

  // I piani sono originariamente sul piano XZ (y=0) nel sistema
  // di coordinate dell'engine (destroso); la lenght si riferisce
  // all'asse z, la width si riferisce all'asse x
  vect_set(&normal, 0, 1, 0);

  minb.x=-width/2.0f;
  minb.z=-lenght/2.0f;
  maxb.x=width/2.0f;
  maxb.z=lenght/2.0f;
  return(1);
}


void AD_RaytPlane::update(float framepos, AD_Vect3D *first_hit)
{
  AD_Vect3D p;

  build_objectmatrix(framepos);
  vect_sub(first_hit, &currentpos, &p);
  mat_mulvectaffine(&inverse_rotmatrix, &p, &first_hit_pos);
  mat_mulvectaffine(&currentmatrix_rot, &normal, &normal_tr);
}


int AD_RaytPlane::intersect(AD_Ray *ray)
{
   AD_Vect3D base0, dir0, aux;
   float t;


   mat_mulvectaffine(&inverse_rotmatrix, &ray->direction, &dir0);
   // controllo se i piani sono paralleli alla direzione
   if ((dir0.x==0) || (dir0.z==0)) return(0);

   vect_sub(&ray->base, &currentpos, &aux);
   mat_mulvectaffine(&inverse_rotmatrix, &aux, &base0);
   t=-base0.y/dir0.y;
   if (t>=1)
   {
	 aux.x=base0.x+t*dir0.x;
	 if ((aux.x>maxb.x) || (aux.x<minb.x)) return(0);
	 aux.z=base0.z+t*dir0.z;
	 if ((aux.z>maxb.z) || (aux.z<minb.z)) return(0);
     ray->distance=t;
     return(1);
   }
   else return(0);   
}


int AD_RaytPlane::intersect_first(AD_Ray *ray)
{
   AD_Vect3D dir0, aux;
   float t;


   mat_mulvectaffine(&inverse_rotmatrix, &ray->direction, &dir0);
   // controllo se i piani sono paralleli alla direzione
   if ((dir0.x==0) || (dir0.z==0)) return(0);

   t=-first_hit_pos.y/dir0.y;
   if (t>=1)
   {
	 aux.x=first_hit_pos.x+t*dir0.x;
	 if ((aux.x>maxb.x) || (aux.x<minb.x)) return(0);
	 aux.z=first_hit_pos.z+t*dir0.z;
	 if ((aux.z>maxb.z) || (aux.z<minb.z)) return(0);
     ray->distance=t;
     return(1);
   }
   else return(0);   
}


void AD_RaytPlane::get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal)
{
   vect_copy(&this->normal, normal);
}


// ===================================================================
// ***************************     CONO     **************************
// ===================================================================
AD_RaytCone::AD_RaytCone()
{
}

int AD_RaytCone::init(void)
{
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);
  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0) &&
	  (father==(AD_RaytObject *)NULL) && (have_childrens<=0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }
  return(1);
}

void AD_RaytCone::update(float framepos, AD_Vect3D *first_hit)
{
  build_objectmatrix(framepos);
  vect_sub(first_hit, &currentpos, &first_hit_pos);
}

int AD_RaytCone::intersect(AD_Ray *ray)
{
   return(0);
}


int AD_RaytCone::intersect_first(AD_Ray *ray)
{
   return(0);
}


void AD_RaytCone::get_normal(AD_Vect3D *hitpoint, AD_Vect3D *normal)
{
   return;
}