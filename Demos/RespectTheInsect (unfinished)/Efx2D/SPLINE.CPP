#include "spline.h"
#include "..\Math\fastmath.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error 

// **********************************************************
// ***********      SPLINE MONODIMENSIONALI     *************
// **********************************************************

spline1D::spline1D()
{
   der=der_tr=keys=keys_tr=(float *)NULL;
   nkeys=0;
}


void spline1D::add_key(float k)
{
   if (keys==(float *)NULL)
   {
	  keys=new float[MAXKEYS];
	  keys_tr=new float[MAXKEYS];
	  der=new float[MAXKEYS];
	  der_tr=new float[MAXKEYS];
   }
   
   if (nkeys<MAXKEYS)
   {
     keys[nkeys]=k;
     nkeys++;
   }
}


void spline1D::init()
{
  int i;
  
  if (nkeys<=1) return;
  if (nkeys==2)
  {
	 der[0]=der[1]=0;  // spline naturali
	 return;
  }

  der[0]=(keys[1]-keys[0])/2.0f;
  der[nkeys-1]=(keys[nkeys-2]-keys[nkeys-1])/2.0f;
  for (i=1; i<nkeys-1; i++)
  {
	 der[i]=(keys[i+1]-keys[i-1])/2.0f;
  }
}


void spline1D::evaluate(float pos, float *result)
{
  int i;
  float t, t2, t3;
  float f1, f2, h1, h2, h3, h4;

  if ((nkeys<1) || (pos<0)) return;
  if (nkeys==1)
  {
	*result=keys[0];
	return;
  }
  if (pos>1)
  {
	*result=keys[nkeys-1];
	return;
  }

  // ci sono almeno 2 key e 0<=pos<=1
  i=myfist(fceil(pos*(nkeys-1)));
  if (i==0) i=1;

  f1=(float)(i)/(float)nkeys;
  f2=(float)(i+1)/(float)nkeys;
  t=(pos-f1)/(f2-f1);
  t2=t*t;
  t3=t2*t;

  // caloclo delle funzioni base
  h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
  h2=-2.0f*t3 + 3.0f*t2;
  h3=t3 - 2.0f*t2 + t;
  h4=t3 - t2;

  *result=(h1*keys[i-1] + h2*keys[i] +
           h3*der[i-1] + h4*der[i]);
}


// **********************************************************
// *************      SPLINE BIDIMENSIONALI     *************
// **********************************************************

spline2D::spline2D()
{
   der=der_tr=keys=keys_tr=(key2D *)NULL;
   nkeys=0;
}


void spline2D::add_key(float x, float y)
{
   if (keys==(key2D *)NULL)
   {
	  keys=new key2D[MAXKEYS];
	  keys_tr=new key2D[MAXKEYS];
	  der=new key2D[MAXKEYS];
	  der_tr=new key2D[MAXKEYS];
   }
   
   if (nkeys<MAXKEYS)
   {
     keys[nkeys].x=x;
     keys[nkeys].y=y;
     nkeys++;
   }
}


void spline2D::init()
{
  int i;

  for (i=0; i<nkeys; i++) keys_tr[i]=keys[i];
  if (nkeys<=1) return;
  if (nkeys==2)
  {
	 der[0].x=der[1].x=0;
	 der[0].y=der[1].y=0;
	 der_tr[0].x=der_tr[1].x=0;
	 der_tr[0].y=der_tr[1].y=0;
	 return;
  }

  der[0].x=(keys[1].x-keys[0].x)/2.0f;
  der[0].y=(keys[1].y-keys[0].y)/2.0f;
  der[nkeys-1].x=(keys[nkeys-2].x-keys[nkeys-1].x)/2.0f;
  der[nkeys-1].y=(keys[nkeys-2].y-keys[nkeys-1].y)/2.0f;
  for (i=1; i<nkeys-1; i++)
  {
	 der[i].x=(keys[i+1].x-keys[i-1].x)/2.0f;
	 der[i].y=(keys[i+1].y-keys[i-1].y)/2.0f;
  }
  for (i=0; i<nkeys; i++) der_tr[i]=der[i];
}


void spline2D::rotate_key (float x, float y, float deg)
{
   float a11, a12, a21, a22, u, v, ang;
   int i;

   ang=(float)(deg*3.1415/180.0);
   a11=a22=cosf(ang);
   a12=sinf(ang);
   a21=-a12;

   for (i=0; i<nkeys; i++)
   {
	  u=keys[i].x-x;
	  v=keys[i].y-y;
      keys_tr[i].x=a11*u+a12*v  + x;
      keys_tr[i].y=a21*u+a22*v  + y;

	  u=der[i].x;
	  v=der[i].y;
      der_tr[i].x=a11*u+a12*v;
      der_tr[i].y=a21*u+a22*v;
   }
}


void spline2D::evaluate(float pos, key2D *result)
{
  int i;
  float t, t2, t3;
  float f1, f2, h1, h2, h3, h4;

  if ((nkeys<1) || (pos<0)) return;
  if (nkeys==1)
  {
	*result=keys_tr[0];
	return;
  }
  if (pos>1)
  {
	*result=keys_tr[nkeys-1];
	return;
  }

  // ci sono almeno 2 key e 0<=pos<=1
  i=myfist(fceil(pos*(nkeys-1)));
  if (i==0)
  {
	 f1=0;
	 f2=(float)(i+1)/(float)(nkeys-1);
     i=1;
  }
  else
  {
    f1=(float)(i-1)/(float)(nkeys-1);
    f2=(float)(i)/(float)(nkeys-1);
  }

  t=(pos-f1)/(f2-f1);
  t2=t*t;
  t3=t2*t;

  // caloclo delle funzioni base
  h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
  h2=-2.0f*t3 + 3.0f*t2;
  h3=t3 - 2.0f*t2 + t;
  h4=t3 - t2;

  result->x=(h1*keys_tr[i-1].x + h2*keys_tr[i].x +
             h3*der_tr[i-1].x + h4*der_tr[i].x);
  result->y=(h1*keys_tr[i-1].y + h2*keys_tr[i].y +
             h3*der_tr[i-1].y + h4*der_tr[i].y);
}


// **********************************************************
// ************      SPLINE TRIDIMENSIONALI     *************
// **********************************************************

spline3D::spline3D()
{
   der=der_tr=keys=keys_tr=(key3D *)NULL;
   nkeys=0;
}


void spline3D::add_key(float x, float y, float z)
{
   if (keys==(key3D *)NULL)
   {
	  keys=new key3D[MAXKEYS];
	  keys_tr=new key3D[MAXKEYS];
	  der=new key3D[MAXKEYS];
	  der_tr=new key3D[MAXKEYS];
   }
   
   if (nkeys<MAXKEYS)
   {
     keys[nkeys].x=x;
     keys[nkeys].y=y;
     keys[nkeys].z=z;
     nkeys++;
   }
}


void spline3D::init()
{
  int i;
  
  if (nkeys<=1) return;
  if (nkeys==2)
  {
	 der[0].x=der[1].x=0;  // spline naturali
	 der[0].y=der[1].y=0;  // spline naturali
	 der[0].z=der[1].z=0;  // spline naturali
	 return;
  }

  der[0].x=(keys[1].x-keys[0].x)/2.0f;
  der[0].y=(keys[1].y-keys[0].y)/2.0f;
  der[0].z=(keys[1].z-keys[0].z)/2.0f;
  der[nkeys-1].x=(keys[nkeys-2].x-keys[nkeys-1].x)/2.0f;
  der[nkeys-1].y=(keys[nkeys-2].y-keys[nkeys-1].y)/2.0f;
  der[nkeys-1].z=(keys[nkeys-2].z-keys[nkeys-1].z)/2.0f;
  for (i=1; i<nkeys-1; i++)
  {
	 der[i].x=(keys[i+1].x-keys[i-1].x)/2.0f;
	 der[i].y=(keys[i+1].y-keys[i-1].y)/2.0f;
	 der[i].z=(keys[i+1].z-keys[i-1].z)/2.0f;
  }
}


void spline3D::evaluate(float pos, key3D *result)
{
  int i;
  float t, t2, t3;
  float f1, f2, h1, h2, h3, h4;

  if ((nkeys<1) || (pos<0)) return;
  if (nkeys==1)
  {
	*result=keys[0];
	return;
  }
  if (pos>1)
  {
	*result=keys[nkeys-1];
	return;
  }

  // ci sono almeno 2 key e 0<=pos<=1
  i=myfist(fceil(pos*(nkeys-1)));
  if (i==0) i=1;

  f1=(float)(i)/(float)nkeys;
  f2=(float)(i+1)/(float)nkeys;
  t=(pos-f1)/(f2-f1);
  t2=t*t;
  t3=t2*t;

  // caloclo delle funzioni base
  h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
  h2=-2.0f*t3 + 3.0f*t2;
  h3=t3 - 2.0f*t2 + t;
  h4=t3 - t2;

  result->x=(h1*keys[i-1].x + h2*keys[i].x +
             h3*der[i-1].x + h4*der[i].x);
  result->y=(h1*keys[i-1].y + h2*keys[i].y +
             h3*der[i-1].y + h4*der[i].y);
  result->z=(h1*keys[i-1].z + h2*keys[i].z +
             h3*der[i-1].z + h4*der[i].z);
}