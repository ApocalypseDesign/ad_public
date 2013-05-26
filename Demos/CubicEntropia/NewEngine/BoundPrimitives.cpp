#include <math.h>
#include "BoundPrimitives.h"
#include "FastMath.h"

CBoundPrimitive::CBoundPrimitive()
{
  p_EnclosedVolume=0;
  p_Type=UNKNOW_PRIMITIVE;
  vect_set(&p_Center, 0, 0, 0);
  p_PaintVertex=(AD_Vect3D *)NULL;
  p_NumPaintVertex=0;
}

//--------------------------------------------------------------------

//####################################################################
//#######################        SPHERE       ########################
//####################################################################

CSpherePrimitive::CSpherePrimitive()
{
  p_Radius=0;
  p_Type=SPHERE_PRIMITIVE;
  vect_set(&p_Center, 0, 0, 0);
}

//--------------------------------------------------------------------

void CSpherePrimitive::m_Update(AD_Matrix *obj2world, CBoundPrimitive *out)
{
  AD_Vect3D r, R;
  CSpherePrimitive *l_out=(CSpherePrimitive *)out;

  // sentro in world space
  mat_mulvect(obj2world, &p_Center, &l_out->p_Center);

  // calcolo empirico del nuovo raggio
  vect_set(&r, p_Radius, p_Radius, p_Radius);
  mat_mulvectaffine(obj2world, &r, &R);
  R.x=(float4)fabs(R.x);
  R.y=(float4)fabs(R.y);
  R.z=(float4)fabs(R.z);
  l_out->p_Radius=R.x;
  if (R.y > l_out->p_Radius) l_out->p_Radius=R.y;
  if (R.z > l_out->p_Radius) l_out->p_Radius=R.z;
}

//--------------------------------------------------------------------

int32 CSpherePrimitive::m_IntersectRay (Ray *ray)
{
  AD_Vect3D diff;
  //float4 b, s, c, t0, t1, k;
  float4 d, sqr_radius;

  vect_sub(&p_Center, &ray->base, &diff);
  d=vect_dot(&diff, &ray->direction);
  float4 ll=vect_dot(&diff, &diff);

  sqr_radius=p_Radius*p_Radius;
  if ((d<0) && (ll>sqr_radius)) return(0);
  float4 mm=ll-d*d;
  if (mm>sqr_radius) return(0);

  float4 q=(float4)sqrt(sqr_radius-mm);
  if (ll>sqr_radius) ray->distance=d-q;
  else ray->distance=d+q;
  return(1);
/*
  // calcoliamo i valori dei coefficienti dell'equazione
  vect_sub(&ray->base, &p_WorldCenter, &diff);

  sqr_radius=p_WorldRadius*p_WorldRadius;
  b = vect_dot(&ray->direction, &diff);
  c = vect_dot(&diff, &diff) - sqr_radius;
  d = (b * b) - c;
  if (d < 0) return(0);

  // le soluzioni esistono
  s=(float4)sqrt(d);
  t0 = (-b + s);
  t1 = (-b - s);

  if ((t0 > 1.0) || (t1 > 1.0))
  {
    if (t1 > 1.0) ray->distance = t1;
    if ((t0 < t1) && (t0 > 1.0)) ray->distance = t0;
    return 1;
  }
  return(0);*/
}

//--------------------------------------------------------------------

int32 CSpherePrimitive::m_IntersectSphere (CSpherePrimitive *sphere)
{
   AD_Vect3D s;
   float4 dist2, r;

   vect_sub(&sphere->p_Center, &p_Center, &s);
   dist2=vect_dot(&s, &s);
   r=sphere->p_Radius+p_Radius;
   if (dist2 > r*r) return(0);
   else return(1);
}

//--------------------------------------------------------------------

int32 CSpherePrimitive::m_IntersectPrimitive (CBoundPrimitive *primitive)
{
  if (primitive->p_Type==SPHERE_PRIMITIVE)
	 return(m_IntersectSphere((CSpherePrimitive *)primitive));
  return(0);
}

//--------------------------------------------------------------------

float4 CSpherePrimitive::m_GetEnclosedVolume(void)
{
  const float4 K=4.0f/3.0f*Pi__;

  p_EnclosedVolume=K*p_Radius*p_Radius;
  return(p_EnclosedVolume);
}

//--------------------------------------------------------------------

void CSpherePrimitive::m_GetLongestAxis (int32 index, AD_Vect3D *out)
{
   if (!out) return;
   switch (index & 3)
   {
      case 0: vect_set(out, 1, 0, 0);
              break;
      case 1: vect_set(out, 0, 1, 0);
              break;
      case 2: vect_set(out, 0, 0, 1);
              break;
   }
}

//--------------------------------------------------------------------

int32 CSpherePrimitive::m_Bound(AD_Vect3D *vertex, AD_Tria3D *triangles,
                                int32 *indexes, int32 start, int32 end)
{
   AD_Vect3D v;
   int32 k;
   int32 i1, i2, i3;
   float4 maxDist, dist;

   // calcolo del centroide
   vect_set(&p_Center, 0, 0, 0);
   for (k=start; k<=end; k++)
     vect_auto_add(&p_Center, &triangles[indexes[k]].midpoint);
   vect_auto_scale(&p_Center, 1.0f/(float4)(end-start+1));

   maxDist=-1E10;
   for (k=start; k<=end; k++)
   {
      i1=triangles[indexes[k]].i1;
      i2=triangles[indexes[k]].i2;
      i3=triangles[indexes[k]].i3;

	  vect_sub(&vertex[i1], &p_Center, &v);
	  dist=vect_dot(&v, &v);
	  if (dist>maxDist) maxDist=dist;

	  vect_sub(&vertex[i2], &p_Center, &v);
	  dist=vect_dot(&v, &v);
	  if (dist>maxDist) maxDist=dist;

	  vect_sub(&vertex[i3], &p_Center, &v);
	  dist=vect_dot(&v, &v);
	  if (dist>maxDist) maxDist=dist;
   }
   p_Radius=(float4)sqrt(maxDist);

   p_PaintVertex=new AD_Vect3D[6];
   p_NumPaintVertex=6;

   // costruisco le linee per disegnare la primitiva (gli assi)
   for (k=0; k<6; k++) vect_copy(&p_Center, &p_PaintVertex[k]);
   // linea nelle x
   p_PaintVertex[0].x-=p_Radius;
   p_PaintVertex[1].x+=p_Radius;
   // linea nelle y
   p_PaintVertex[2].y-=p_Radius;
   p_PaintVertex[3].y+=p_Radius;
   // linea nelle z
   p_PaintVertex[4].z-=p_Radius;
   p_PaintVertex[5].z+=p_Radius;
   m_GetEnclosedVolume();
   return(1);
}


//####################################################################
//################     AXES ALIGNED BOUNDING BOX    ##################
//####################################################################
CAABBPrimitive::CAABBPrimitive()
{
  p_Width=p_Height=p_Length=0;
}


int32 CAABBPrimitive::m_IntersectRay (Ray *ray)
{
   float4 tnear, tfar;
   float4 t1, t2, tswap, k;
   AD_Vect3D base0, dir0;

   #define SWAP_T \
   { \
     tswap=t1; \
     t1=t2; \
     t2=tswap; \
   } \

   vect_copy(&ray->direction, &dir0);
   vect_sub(&ray->base, &p_Center, &base0);
   tnear=-1E10;
   tfar=1E10;

   // piani X
   if (dir0.x == 0) goto testY;
   k=1.0f/dir0.x;
   t1=(-p_Width-base0.x)*k;
   t2=(p_Width-base0.x)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   //f=BOX_RIGHT;
   }
   //else f=BOX_LEFT;
   if (t1 > tnear)
   {
	  tnear=t1;
      //face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

testY:
;
   // piani Y
   if (dir0.y==0) goto testZ;
   k=1.0f/dir0.y;
   t1=(-p_Height-base0.y)*k;
   t2=(p_Height-base0.y)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   //f=BOX_UP;
   }
   //else f=BOX_DOWN;
   if (t1 > tnear)
   {
	  tnear=t1;
      //face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

testZ:
;
   // piani Z
   k=1.0f/dir0.z;
   t1=(-p_Length-base0.z)*k;
   t2=(p_Length-base0.z)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   //f=BOX_BACK;
   } //else f=BOX_FRONT;
   if (t1 > tnear)
   {
	  tnear=t1;
      //face_hitted=f;
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

//--------------------------------------------------------------------

int32 CAABBPrimitive::m_IntersectPrimitive (CBoundPrimitive *primitive)
{
   return(0);
}

//--------------------------------------------------------------------

float4 CAABBPrimitive::m_GetEnclosedVolume(void)
{
  p_EnclosedVolume=p_Width*p_Height*p_Length;
  return(p_EnclosedVolume);
}

//--------------------------------------------------------------------

void CAABBPrimitive::m_Update(AD_Matrix *obj2world, CBoundPrimitive *out)
{
  AD_Vect3D r, R;
  CAABBPrimitive *l_out=(CAABBPrimitive *)out;

  // centro in world space
  mat_mulvect(obj2world, &p_Center, &l_out->p_Center);

  // calcolo delle nuove dimensioni
  vect_set(&r, p_Width, p_Height, p_Length);
  mat_mulvectaffine(obj2world, &r, &R);
  l_out->p_Width=(float4)fabs(R.x);
  l_out->p_Height=(float4)fabs(R.y);
  l_out->p_Length=(float4)fabs(R.z);
}

//--------------------------------------------------------------------

void CAABBPrimitive::m_GetLongestAxis (int32 index, AD_Vect3D *out)
{
   float4 v[3], vv;
   AD_Vect3D w[3], ww;
   int32 i, j;

   if (!out) return;
   vect_set(&w[0], 1, 0, 0);
   vect_set(&w[1], 0, 1, 0);
   vect_set(&w[2], 0, 0, 1);
   v[0]=p_Width;
   v[1]=p_Height;
   v[2]=p_Length;
   for (i=0; i<2; i++)
   {
     for (j=i+1; j<3; j++)
	 {
		if (v[j]>v[i])
		{
		   vv=v[j];
		   v[j]=v[i];
		   v[i]=vv;
		   vect_copy(&w[j], &ww);
		   vect_copy(&w[i], &w[j]);
		   vect_copy(&ww, &w[i]);
		}
	 }
   }

   index=index & 3;
   vect_copy(&w[index], out);
}

//--------------------------------------------------------------------

int32 CAABBPrimitive::m_Bound(AD_Vect3D *vertex, AD_Tria3D *triangles,
                              int32 *indexes, int32 start, int32 end)
{
   AD_Vect3D vmin, vmax, v[8], w;
   AD_Vect3D bmin, bmax;
   int32 k;
   int32 i1, i2, i3;

   #define MAKE_LINE(a,b,c,d) \
     vect_copy(&v[a], &p_PaintVertex[c]); \
     vect_copy(&v[b], &p_PaintVertex[d]); \

   vect_set(&vmin, 1E10, 1E10, 1E10);
   vect_set(&vmax, -1E10, -1E10, -1E10);
   vect_set(&bmin, 1E10, 1E10, 1E10);
   vect_set(&bmax, -1E10, -1E10, -1E10);
   for (k=start; k<=end; k++)
   {
      i1=triangles[indexes[k]].i1;
      vect_copy(&vertex[i1], &w);
	  if (w.x < vmin.x) vmin.x=w.x;
	  if (w.y < vmin.y) vmin.y=w.y;
	  if (w.z < vmin.z) vmin.z=w.z;
	  if (w.x > vmax.x) vmax.x=w.x;
	  if (w.y > vmax.y) vmax.y=w.y;
	  if (w.z > vmax.z) vmax.z=w.z;

      i2=triangles[indexes[k]].i2;
      vect_copy(&vertex[i2], &w);
	  if (w.x < vmin.x) vmin.x=w.x;
	  if (w.y < vmin.y) vmin.y=w.y;
	  if (w.z < vmin.z) vmin.z=w.z;
	  if (w.x > vmax.x) vmax.x=w.x;
	  if (w.y > vmax.y) vmax.y=w.y;
	  if (w.z > vmax.z) vmax.z=w.z;

      i3=triangles[indexes[k]].i3;
      vect_copy(&vertex[i3], &w);
	  if (w.x < vmin.x) vmin.x=w.x;
	  if (w.y < vmin.y) vmin.y=w.y;
	  if (w.z < vmin.z) vmin.z=w.z;
	  if (w.x > vmax.x) vmax.x=w.x;
	  if (w.y > vmax.y) vmax.y=w.y;
	  if (w.z > vmax.z) vmax.z=w.z;

	  vect_copy(&triangles[indexes[k]].midpoint, &w);
	  if (w.x < bmin.x) bmin.x=w.x;
	  if (w.y < bmin.y) bmin.y=w.y;
	  if (w.z < bmin.z) bmin.z=w.z;
	  if (w.x > bmax.x) bmax.x=w.x;
	  if (w.y > bmax.y) bmax.y=w.y;
	  if (w.z > bmax.z) bmax.z=w.z;
   }

   // calcolo del centroide
   vect_add(&vmin, &vmax, &p_Center);
   vect_auto_scale(&p_Center, 0.5f);
   // calcolo delle dimensioni del box
   p_Width=(vmax.x-vmin.x)*0.5f;
   p_Height=(vmax.y-vmin.y)*0.5f;
   p_Length=(vmax.z-vmin.z)*0.5f;
   // se il centroide non risiede dentro la box formata dai punti
   // medi dei triangoli devo ricalcolarlo perkè significa che non
   // potrei trovare piani di split buoni
   if (!(
	     (p_Center.x>bmin.x) && (p_Center.x<bmax.x) &&
	     (p_Center.y>bmin.y) && (p_Center.y<bmax.y) &&
	     (p_Center.z>bmin.z) && (p_Center.z<bmax.z)
	    ))
   {
      vect_add(&bmin, &bmax, &p_Center);
      vect_auto_scale(&p_Center, 0.5f);
      vect_set(&vmin, 1E10, 1E10, 1E10);
      vect_set(&vmax, -1E10, -1E10, -1E10);
      for (k=start; k<=end; k++)
	  {
        i1=triangles[indexes[k]].i1;
        vect_sub(&vertex[i1], &p_Center, &w);
	    if (w.x < vmin.x) vmin.x=w.x;
	    if (w.y < vmin.y) vmin.y=w.y;
	    if (w.z < vmin.z) vmin.z=w.z;
	    if (w.x > vmax.x) vmax.x=w.x;
	    if (w.y > vmax.y) vmax.y=w.y;
	    if (w.z > vmax.z) vmax.z=w.z;
        i2=triangles[indexes[k]].i2;
        vect_sub(&vertex[i2], &p_Center, &w);
	    if (w.x < vmin.x) vmin.x=w.x;
	    if (w.y < vmin.y) vmin.y=w.y;
	    if (w.z < vmin.z) vmin.z=w.z;
	    if (w.x > vmax.x) vmax.x=w.x;
	    if (w.y > vmax.y) vmax.y=w.y;
	    if (w.z > vmax.z) vmax.z=w.z;
        i3=triangles[indexes[k]].i3;
        vect_sub(&vertex[i3], &p_Center, &w);
	    if (w.x < vmin.x) vmin.x=w.x;
	    if (w.y < vmin.y) vmin.y=w.y;
	    if (w.z < vmin.z) vmin.z=w.z;
	    if (w.x > vmax.x) vmax.x=w.x;
	    if (w.y > vmax.y) vmax.y=w.y;
	    if (w.z > vmax.z) vmax.z=w.z;
	  }
      p_Width=max((float4)fabs(vmin.x), (float4)fabs(vmax.x));
      p_Height=max((float4)fabs(vmin.y), (float4)fabs(vmax.y));
      p_Length=max((float4)fabs(vmin.z), (float4)fabs(vmax.z));
   }

   // costruisco le linee per disegnare la primitiva (gli assi)
   p_PaintVertex=new AD_Vect3D[24];
   p_NumPaintVertex=24;

   for (k=0; k<8; k++) vect_copy(&p_Center, &v[k]);
   // i due "piatti" ad altezza +/- p_Height
   for (k=0; k<4; k++) v[k].y+=p_Height;
   for (k=4; k<8; k++) v[k].y-=p_Height;
   for (k=0; k<2; k++) v[k].z-=p_Length;
   for (k=2; k<4; k++) v[k].z+=p_Length;
   v[0].x-=p_Width; v[3].x-=p_Width;
   v[1].x+=p_Width; v[2].x+=p_Width;
   for (k=4; k<6; k++) v[k].z-=p_Length;
   for (k=6; k<8; k++) v[k].z+=p_Length;
   v[4].x-=p_Width; v[7].x-=p_Width;
   v[5].x+=p_Width; v[6].x+=p_Width;

   MAKE_LINE(0,1,0,1)
   MAKE_LINE(1,2,2,3)
   MAKE_LINE(2,3,4,5)
   MAKE_LINE(3,0,6,7)
   MAKE_LINE(4,5,8,9)
   MAKE_LINE(5,6,10,11)
   MAKE_LINE(6,7,12,13)
   MAKE_LINE(7,4,14,15)
   MAKE_LINE(0,4,16,17)
   MAKE_LINE(1,5,18,19)
   MAKE_LINE(2,6,20,21)
   MAKE_LINE(3,7,22,23)
   m_GetEnclosedVolume();
   return(1);
}



//####################################################################
//##############     OBJECT ALIGNED BOUNDING BOX    ##################
//####################################################################
COBBPrimitive::COBBPrimitive()
{
  p_Width=p_Height=p_Length=0;
  vect_set(&p_XAxis, 1, 0, 0);
  vect_set(&p_YAxis, 0, 1, 0);
  vect_set(&p_ZAxis, 0, 0, 1);
}

//--------------------------------------------------------------------

int32 COBBPrimitive::m_IntersectRay (Ray *ray)
{
   float4 tnear, tfar;
   float4 t1, t2, tswap, k;
   AD_Vect3D base0, dir0, aux;
   AD_Matrix l_invrot;

   #define SWAP_T \
   { \
     tswap=t1; \
     t1=t2; \
     t2=tswap; \
   } \

   mat_insert_row(&l_invrot, 0, &p_XAxis);
   mat_insert_row(&l_invrot, 1, &p_YAxis);
   mat_insert_row(&l_invrot, 2, &p_ZAxis);

   mat_mulvectaffine(&l_invrot, &ray->direction, &dir0);
   vect_sub(&ray->base, &p_Center, &aux);
   mat_mulvectaffine(&l_invrot, &aux, &base0);
   tnear=-1E10;
   tfar=1E10;

   // piani X
   if (dir0.x == 0) goto testY;
   k=1.0f/dir0.x;
   t1=(-p_Width-base0.x)*k;
   t2=(p_Width-base0.x)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   //f=BOX_RIGHT;
   }
   //else f=BOX_LEFT;
   if (t1 > tnear)
   {
	  tnear=t1;
      //face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

testY:
;
   // piani Y
   if (dir0.y==0) goto testZ;
   k=1.0f/dir0.y;
   t1=(-p_Height-base0.y)*k;
   t2=(p_Height-base0.y)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   //f=BOX_UP;
   }
   //else f=BOX_DOWN;
   if (t1 > tnear)
   {
	  tnear=t1;
      //face_hitted=f;
   }
   if (t2 < tfar) tfar=t2;
   if (tnear > tfar) return(0);
   if (tfar < 0) return(0);

testZ:
;
   // piani Z
   k=1.0f/dir0.z;
   t1=(-p_Length-base0.z)*k;
   t2=(p_Length-base0.z)*k;
   if (t1 > t2)
   {
	   SWAP_T
	   //f=BOX_BACK;
   } //else f=BOX_FRONT;
   if (t1 > tnear)
   {
	  tnear=t1;
      //face_hitted=f;
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

//--------------------------------------------------------------------

int32 COBBPrimitive::m_IntersectPrimitive (CBoundPrimitive *primitive)
{
  return(0);
}

//--------------------------------------------------------------------

float4 COBBPrimitive::m_GetEnclosedVolume(void)
{
  p_EnclosedVolume=p_Width*p_Height*p_Length;
  return(p_EnclosedVolume);
}

//--------------------------------------------------------------------


void COBBPrimitive::m_Update(AD_Matrix *obj2world, CBoundPrimitive *out)
{
  COBBPrimitive *l_out=(COBBPrimitive *)out;

  // centro in world space
  mat_mulvect(obj2world, &p_Center, &l_out->p_Center);

  // calcolo delle nuove dimensioni
  mat_mulvectaffine(obj2world, &p_XAxis, &l_out->p_XAxis);
  mat_mulvectaffine(obj2world, &p_YAxis, &l_out->p_YAxis);
  mat_mulvectaffine(obj2world, &p_ZAxis, &l_out->p_ZAxis);
  l_out->p_Width=p_Width*vect_length(&l_out->p_XAxis);
  l_out->p_Height=p_Height*vect_length(&l_out->p_YAxis);
  l_out->p_Length=p_Length*vect_length(&l_out->p_ZAxis);
  vect_auto_normalize(&l_out->p_XAxis);
  vect_auto_normalize(&l_out->p_YAxis);
  vect_auto_normalize(&l_out->p_ZAxis);
}

//--------------------------------------------------------------------

void COBBPrimitive::m_GetLongestAxis (int32 index, AD_Vect3D *out)
{
   float4 v[3], vv;
   AD_Vect3D w[3], ww;
   int32 i, j;

   if (!out) return;
   vect_copy(&p_XAxis, &w[0]);
   vect_copy(&p_YAxis, &w[1]);
   vect_copy(&p_ZAxis, &w[2]);
   v[0]=p_Width;
   v[1]=p_Height;
   v[2]=p_Length;
   for (i=0; i<2; i++)
   {
     for (j=i+1; j<3; j++)
	 {
		if (v[j]>v[i])
		{
		   vv=v[j];
		   v[j]=v[i];
		   v[i]=vv;
		   vect_copy(&w[j], &ww);
		   vect_copy(&w[i], &w[j]);
		   vect_copy(&ww, &w[i]);
		}
	 }
   }
   index=index & 3;
   vect_copy(&w[index], out);
}

//--------------------------------------------------------------------

int32 COBBPrimitive::m_Bound(AD_Vect3D *vertex, AD_Tria3D *triangles,
                             int32 *indexes, int32 start, int32 end)
{
   // matrice di covarianza assieme ai suoi autovettori e autovaloiri
   AD_Matrix p_CovarianceMatrix;
   AD_Matrix p_CovEigenVectors;
   AD_Vect3D p_CovEigenValues;
   int32 i, j, k, i1, i2, i3;
   AD_Vect3D centroid, v1, v2, v3, v[8];
   AD_Vect3D vmin, vmax, bmin, bmax, w;
   float4 prjX, prjY, prjZ;
   float4 bmin_prjX, bmin_prjY, bmin_prjZ;
   float4 bmax_prjX, bmax_prjY, bmax_prjZ;
   float4 *p, *q, *r;
   float4 sum, s;

   #define MAKE_LINE2(a,b,c,d) \
     vect_copy(&v[a], &p_PaintVertex[c]); \
     vect_copy(&v[b], &p_PaintVertex[d]); \

   // calcolo del centroide (centro di massa)
   s=1.0f/((float4)(end-start+1));
   vect_set(&bmin, 1E10, 1E10, 1E10);
   vect_set(&bmax, -1E10, -1E10, -1E10);
   for (k=start; k<=end; k++)
   {
	 vect_copy(&triangles[indexes[k]].midpoint, &w);
	 if (w.x < bmin.x) bmin.x=w.x;
	 if (w.y < bmin.y) bmin.y=w.y;
	 if (w.z < bmin.z) bmin.z=w.z;
	 if (w.x > bmax.x) bmax.x=w.x;
	 if (w.y > bmax.y) bmax.y=w.y;
	 if (w.z > bmax.z) bmax.z=w.z;
   }
   vect_add(&bmin, &bmax, &centroid);
   vect_auto_scale(&centroid, 0.5f);

   // calcolo della matrice di covarianza Cjk (è una 3x3)
   p=&v1.x;
   q=&v2.x;
   r=&v3.x;
   for (j=0; j<3; j++)
     for (k=0; k<3; k++)
	 {
       sum=0;
       for (i=start; i<=end; i++)
	   {
         i1=triangles[indexes[i]].i1;
         i2=triangles[indexes[i]].i2;
         i3=triangles[indexes[i]].i3;
         vect_sub(&vertex[i1], &centroid, &v1);
         vect_sub(&vertex[i2], &centroid, &v2);
         vect_sub(&vertex[i3], &centroid, &v3);
		 sum=sum+p[j]*p[k]+q[j]*q[k]+r[j]*r[k];
	   }
	   p_CovarianceMatrix.a[j][k]=sum*s;
	 }
   
   // calcolo degli autovalori e autovettori
   mat_eigen(&p_CovarianceMatrix, &p_CovEigenValues, &p_CovEigenVectors, 0);
   // normalizzazione di sicurezza
   mat_get_column(&p_CovEigenVectors, 0, &v1);
   vect_normalize(&v1, &p_XAxis);
   mat_get_column(&p_CovEigenVectors, 1, &v2);
   vect_normalize(&v2, &p_YAxis);
   mat_get_column(&p_CovEigenVectors, 2, &v3);
   vect_normalize(&v3, &p_ZAxis);

   // calcolo il centro del box come estensioni minima e massima
   // delle proiezioni dei vertici
   vect_set(&vmin, 1E10, 1E10, 1E10);
   vect_set(&vmax, -1E10, -1E10, -1E10);
   for (k=start; k<=end; k++)
   {
     i1=triangles[indexes[k]].i1;
	 vect_sub(&vertex[i1], &centroid, &v1);
     prjX=vect_dot(&v1, &p_XAxis);
     prjY=vect_dot(&v1, &p_YAxis);
     prjZ=vect_dot(&v1, &p_ZAxis);
	 if (prjX < vmin.x) vmin.x=prjX;
	 if (prjY < vmin.y) vmin.y=prjY;
	 if (prjZ < vmin.z) vmin.z=prjZ;
	 if (prjX > vmax.x) vmax.x=prjX;
	 if (prjY > vmax.y) vmax.y=prjY;
	 if (prjZ > vmax.z) vmax.z=prjZ;
     i2=triangles[indexes[k]].i2;
	 vect_sub(&vertex[i2], &centroid, &v1);
     prjX=vect_dot(&v1, &p_XAxis);
     prjY=vect_dot(&v1, &p_YAxis);
     prjZ=vect_dot(&v1, &p_ZAxis);
	 if (prjX < vmin.x) vmin.x=prjX;
	 if (prjY < vmin.y) vmin.y=prjY;
	 if (prjZ < vmin.z) vmin.z=prjZ;
	 if (prjX > vmax.x) vmax.x=prjX;
	 if (prjY > vmax.y) vmax.y=prjY;
	 if (prjZ > vmax.z) vmax.z=prjZ;
     i3=triangles[indexes[k]].i3;
	 vect_sub(&vertex[i3], &centroid, &v1);
     prjX=vect_dot(&v1, &p_XAxis);
     prjY=vect_dot(&v1, &p_YAxis);
     prjZ=vect_dot(&v1, &p_ZAxis);
	 if (prjX < vmin.x) vmin.x=prjX;
	 if (prjY < vmin.y) vmin.y=prjY;
	 if (prjZ < vmin.z) vmin.z=prjZ;
	 if (prjX > vmax.x) vmax.x=prjX;
	 if (prjY > vmax.y) vmax.y=prjY;
	 if (prjZ > vmax.z) vmax.z=prjZ;
   }

   p_Width=(vmax.x-vmin.x)*0.5f;
   p_Height=(vmax.y-vmin.y)*0.5f;
   p_Length=(vmax.z-vmin.z)*0.5f;
   vect_scale(&p_XAxis, (vmax.x+vmin.x)*0.5f, &v1);
   vect_scale(&p_YAxis, (vmax.y+vmin.y)*0.5f, &v2);
   vect_scale(&p_ZAxis, (vmax.z+vmin.z)*0.5f, &v3);
   vect_add(&v1, &v2, &p_Center);
   vect_auto_add(&p_Center, &v3);
   vect_auto_add(&p_Center, &centroid);

   prjX=vect_dot(&p_Center, &p_XAxis);
   prjY=vect_dot(&p_Center, &p_YAxis);
   prjZ=vect_dot(&p_Center, &p_ZAxis);
   bmin_prjX=vect_dot(&bmin, &p_XAxis);
   bmin_prjY=vect_dot(&bmin, &p_YAxis);
   bmin_prjZ=vect_dot(&bmin, &p_ZAxis);
   bmax_prjX=vect_dot(&bmax, &p_XAxis);
   bmax_prjY=vect_dot(&bmax, &p_YAxis);
   bmax_prjZ=vect_dot(&bmax, &p_ZAxis);

   // se il centroide non risiede dentro la box formata dai punti
   // medi dei triangoli devo ricalcolarlo perkè significa che non
   // potrei trovare piani di split buoni
   if (!(
	     (prjX>bmin_prjX) && (prjX<bmax_prjX) &&
	     (prjY>bmin_prjY) && (prjY<bmax_prjY) &&
	     (prjZ>bmin_prjZ) && (prjZ<bmax_prjZ)
	    ))
   {
      vect_scale(&p_XAxis, (bmax_prjX+bmin_prjX)*0.5f, &v1);
      vect_scale(&p_YAxis, (bmax_prjY+bmin_prjY)*0.5f, &v2);
      vect_scale(&p_ZAxis, (bmax_prjZ+bmin_prjZ)*0.5f, &v3);
	  vect_add(&v1, &v2, &p_Center);
	  vect_auto_add(&p_Center, &v3);
	  vect_copy(&p_Center, &centroid);
      vect_set(&vmin, 1E10, 1E10, 1E10);
      vect_set(&vmax, -1E10, -1E10, -1E10);
      for (k=start; k<=end; k++)
	  {
        i1=triangles[indexes[k]].i1;
	    vect_sub(&vertex[i1], &centroid, &v1);
        prjX=vect_dot(&v1, &p_XAxis);
        prjY=vect_dot(&v1, &p_YAxis);
        prjZ=vect_dot(&v1, &p_ZAxis);
	    if (prjX < vmin.x) vmin.x=prjX;
	    if (prjY < vmin.y) vmin.y=prjY;
	    if (prjZ < vmin.z) vmin.z=prjZ;
	    if (prjX > vmax.x) vmax.x=prjX;
	    if (prjY > vmax.y) vmax.y=prjY;
	    if (prjZ > vmax.z) vmax.z=prjZ;
        i2=triangles[indexes[k]].i2;
	    vect_sub(&vertex[i2], &centroid, &v1);
        prjX=vect_dot(&v1, &p_XAxis);
        prjY=vect_dot(&v1, &p_YAxis);
        prjZ=vect_dot(&v1, &p_ZAxis);
	    if (prjX < vmin.x) vmin.x=prjX;
	    if (prjY < vmin.y) vmin.y=prjY;
	    if (prjZ < vmin.z) vmin.z=prjZ;
	    if (prjX > vmax.x) vmax.x=prjX;
	    if (prjY > vmax.y) vmax.y=prjY;
	    if (prjZ > vmax.z) vmax.z=prjZ;
        i3=triangles[indexes[k]].i3;
	    vect_sub(&vertex[i3], &centroid, &v1);
        prjX=vect_dot(&v1, &p_XAxis);
        prjY=vect_dot(&v1, &p_YAxis);
        prjZ=vect_dot(&v1, &p_ZAxis);
	    if (prjX < vmin.x) vmin.x=prjX;
	    if (prjY < vmin.y) vmin.y=prjY;
	    if (prjZ < vmin.z) vmin.z=prjZ;
	    if (prjX > vmax.x) vmax.x=prjX;
	    if (prjY > vmax.y) vmax.y=prjY;
	    if (prjZ > vmax.z) vmax.z=prjZ;
	  }
      p_Width=(vmax.x-vmin.x)*0.5f;
      p_Height=(vmax.y-vmin.y)*0.5f;
      p_Length=(vmax.z-vmin.z)*0.5f;
   }

   // costruisco le linee per disegnare la primitiva (gli assi)
   p_PaintVertex=new AD_Vect3D[24];
   p_NumPaintVertex=24;
   vect_scale (&p_XAxis, p_Width, &v1);
   vect_scale (&p_YAxis, p_Height, &v2);
   vect_scale (&p_ZAxis, p_Length, &v3);

   for (k=0; k<8; k++) vect_copy(&p_Center, &v[k]);
   // i due "piatti" ad altezza +/- p_Height
   for (k=0; k<4; k++) vect_auto_add(&v[k], &v2);
   for (k=4; k<8; k++) vect_auto_sub(&v[k], &v2);

   for (k=0; k<2; k++) vect_auto_sub(&v[k], &v3);
   for (k=2; k<4; k++) vect_auto_add(&v[k], &v3);
   vect_auto_sub(&v[0], &v1);  vect_auto_sub(&v[3], &v1);
   vect_auto_add(&v[1], &v1);  vect_auto_add(&v[2], &v1);

   for (k=4; k<6; k++) vect_auto_sub(&v[k], &v3);
   for (k=6; k<8; k++) vect_auto_add(&v[k], &v3);
   vect_auto_sub(&v[4], &v1);  vect_auto_sub(&v[7], &v1);
   vect_auto_add(&v[5], &v1);  vect_auto_add(&v[6], &v1);

   MAKE_LINE2(0,1,0,1)
   MAKE_LINE2(1,2,2,3)
   MAKE_LINE2(2,3,4,5)
   MAKE_LINE2(3,0,6,7)
   MAKE_LINE2(4,5,8,9)
   MAKE_LINE2(5,6,10,11)
   MAKE_LINE2(6,7,12,13)
   MAKE_LINE2(7,4,14,15)
   MAKE_LINE2(0,4,16,17)
   MAKE_LINE2(1,5,18,19)
   MAKE_LINE2(2,6,20,21)
   MAKE_LINE2(3,7,22,23)
   m_GetEnclosedVolume();
   return(1);
}