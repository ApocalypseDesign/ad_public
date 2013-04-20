#include <math.h>
#include "OSMobj.h"
#include "..\utility.h"

// ***********************************************************
// ********************       TWIST      *********************
// ***********************************************************
void AD_TwistModifier::init(void)
{
   int i;

   if (center_track!=(postrack *)NULL)
       center_track->init();

   if (angle_track!=(rolltrack *)NULL)
	   angle_track->init();

   if (bias_track!=(rolltrack *)NULL)
	   bias_track->init();

   if (uplim_track!=(rolltrack *)NULL)
   {
       for (i=0; i<uplim_track->numkey; i++)
         uplim_track->rollazioni[i].roll*=(float)(180/M_PI);
	   uplim_track->init();
   }

   if (lowlim_track!=(rolltrack *)NULL)
   {
       for (i=0; i<lowlim_track->numkey; i++)
         lowlim_track->rollazioni[i].roll*=(float)(180/M_PI);
	   lowlim_track->init();
   }

   if (axis==2) axis=1;
   else if (axis==1) axis=2;
}


void AD_TwistModifier::update(float4 framepos)
{
   AD_Vect3D sub_center;
   AD_Matrix mrot, imrot, pret, ipret;

   if (center_track!=(postrack *)NULL)
      center_track->get_data(framepos, &center);

   if (angle_track!=(rolltrack *)NULL)
	   angle_track->get_data(framepos, &angle);

   if (bias_track!=(rolltrack *)NULL)
	   bias_track->get_data(framepos, &bias);

   if (uplim_track!=(rolltrack *)NULL)
	   uplim_track->get_data(framepos, &uplim);

   if (lowlim_track!=(rolltrack *)NULL)
	   lowlim_track->get_data(framepos, &lowlim);

   // costruzione matrice di trasformazione e la sua
   // inversa
   sub_center.x=-center.x;
   sub_center.y=-center.y;
   sub_center.z=-center.z;

   switch (axis)
   {
     case 0:  // asse X
             mat_setmatrix_of_eulerrotationZ(&mrot, (float)(-M_PI/2.0));
			 height=bbx2-bbx1;
		     break;
     case 1:  // asse Y
             mat_identity(&mrot);
			 height=bby2-bby1;
		     break;
     case 2:  // asse Z
             mat_setmatrix_of_eulerrotationX(&mrot, (float)(M_PI/2.0));
			 height=bbz2-bbz1;
		     break;
   }

   mat_transpose(&mrot, &imrot);
   mat_setmatrix_of_pretraslation(&pret, &sub_center);
   mat_setmatrix_of_pretraslation(&ipret, &center);
   mat_mul (&mrot, &pret, &tm);
   mat_mul (&ipret, &imrot, &invtm);
   if (height==0)
   {
	 angle = 0.0f;
	 angleOverHeight = 0.0f;
   }
   else angleOverHeight = angle / height;
}


void AD_TwistModifier::Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   AD_Vect3D v;
   float x, y, z, a, cosine, sine;

   if (angle==0.0f)
   {
	  vect_copy(pos, out);
	  return;
   }
   
   mat_mulvect (&tm, pos, &v);
   x = v.x;
   z = v.z;
	
   if (dolim)
   {
      if (v.y < lowlim)	y = lowlim;
	  else if (v.y > uplim) y = uplim;
	       else y = v.y;
   }
   else y = v.y;
	
   if (bias>0)
   {
      float u = y/height;
      a = angle * (float)pow(fabs(u), bias);
      if (u<0.0) a = -a;
   }
   else a = y * angleOverHeight;

   cosine = cosf(a);
   sine = sinf(a);
   v.x =  cosine*x + sine*z;
   v.z = -sine*x + cosine*z;

   mat_mulvect (&invtm, &v, out);
}



// ***********************************************************
// ********************       RIPPLE     *********************
// ***********************************************************

void AD_RippleModifier::init(void)
{
   int i;

   if (center_track!=(postrack *)NULL)
       center_track->init();

   // devo sistemare perche' la init delle tracce di roll
   // trasforma prima tutte le key in radianti
   if (amp1_track!=(rolltrack *)NULL)
   {
       for (i=0; i<amp1_track->numkey; i++)
         amp1_track->rollazioni[i].roll*=(float)(180/M_PI);
	   amp1_track->init();
   }

   if (amp2_track!=(rolltrack *)NULL)
   {
       for (i=0; i<amp2_track->numkey; i++)
         amp2_track->rollazioni[i].roll*=(float)(180/M_PI);
	   amp2_track->init();
   }

   if (wlen_track!=(rolltrack *)NULL)
   {
       for (i=0; i<wlen_track->numkey; i++)
         wlen_track->rollazioni[i].roll*=(float)(180/M_PI);
	   wlen_track->init();
   }

   if (phase_track!=(rolltrack *)NULL)
   {
       for (i=0; i<phase_track->numkey; i++)
         phase_track->rollazioni[i].roll*=(float)(180/M_PI);
	   phase_track->init();
   }

   if (decay_track!=(rolltrack *)NULL)
   {
       for (i=0; i<decay_track->numkey; i++)
         decay_track->rollazioni[i].roll*=(float)(180/M_PI);
	   decay_track->init();
   }
}


void AD_RippleModifier::update(float4 framepos)
{
   if (center_track!=(postrack *)NULL)
      center_track->get_data(framepos, &center);

   if (amp1_track!=(rolltrack *)NULL)
	   amp1_track->get_data(framepos, &amp1);

   if (amp2_track!=(rolltrack *)NULL)
	   amp2_track->get_data(framepos, &amp2);

   if (wlen_track!=(rolltrack *)NULL)
	   wlen_track->get_data(framepos, &wlen);

   if (phase_track!=(rolltrack *)NULL)
	   phase_track->get_data(framepos, &phase);

   if (decay_track!=(rolltrack *)NULL)
   {
	   decay_track->get_data(framepos, &decay);
	   if (decay<0) decay=0;
   }

  if (wlen == 0) wlen = 0.0000001f;
}


float WaveFunc(float radius, float amp, 
               float waveLen, float phase, float decay )
{
  float res;

  res = (float)(amp*sin(M_PI*2.0*(radius/waveLen + phase)));
  res*=expf(-decay*fabsf(radius));
  return(res);
}


void AD_RippleModifier::Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   AD_Vect3D pt;
   float r, oldZ, u, a, len, u2;

   vect_sub_inline(pos, &center, &pt);

   if (amp1 != amp2)
   {
     len  = vect_lenght(&pt);
     if (len == 0) a = amp1;
	 else
	 {
        u = (float)(acos(pt.x/len)/M_PI);
        u = (u > 0.5) ? (1.0f-u) : u;
        u *= 2.0f;
        u2=u*u;
		//u = smoothstep(0, 1, u);
        u = -2.0f*u2*u+3.0f*u2;
        a = amp1*(1.0f-u) + amp2*u;
	 }
   }
   else	a = amp1;
	
   oldZ = pt.y;
   r = sqrtf(pt.x*pt.x+pt.z*pt.z);
//   pt.y = oldZ + WaveFunc(r, a, wlen, phase, decay);
   pt.y = oldZ + a*sinf((float)(M_PI)*2.0f*(r/wlen + phase))*expf(-decay*fabsf(r));
   vect_add_inline(&pt, &center, out);
}



// ***********************************************************
// *********************       BEND      *********************
// ***********************************************************
void AD_BendModifier::init(void)
{
   int i;

   if (center_track!=(postrack *)NULL)
       center_track->init();

   if (angle_track!=(rolltrack *)NULL)
	   angle_track->init();

   if (dir_track!=(rolltrack *)NULL)
	   dir_track->init();

   if (uplim_track!=(rolltrack *)NULL)
   {
       for (i=0; i<uplim_track->numkey; i++)
         uplim_track->rollazioni[i].roll*=(float)(180/M_PI);
	   uplim_track->init();
   }

   if (lowlim_track!=(rolltrack *)NULL)
   {
       for (i=0; i<lowlim_track->numkey; i++)
         lowlim_track->rollazioni[i].roll*=(float)(180/M_PI);
	   lowlim_track->init();
   }

   if (axis==2) axis=1;
   else if (axis==1) axis=2;
}


void AD_BendModifier::update(float4 framepos)
{
   AD_Vect3D sub_center;
   AD_Matrix mrot, pret, ipret, mr_plus;
   AD_Matrix mrot2, imrot2;
   float raux;

   if (center_track!=(postrack *)NULL)
      center_track->get_data(framepos, &center);

   if (angle_track!=(rolltrack *)NULL)
	   angle_track->get_data(framepos, &angle);

   if (dir_track!=(rolltrack *)NULL)
	   dir_track->get_data(framepos, &dir);

   if (uplim_track!=(rolltrack *)NULL)
	   uplim_track->get_data(framepos, &uplim);

   if (lowlim_track!=(rolltrack *)NULL)
	   lowlim_track->get_data(framepos, &lowlim);

   // costruzione matrice di trasformazione e la sua
   // inversa
   sub_center.x=-center.x;
   sub_center.y=-center.y;
   sub_center.z=-center.z;
   mat_setmatrix_of_eulerrotationY(&mr_plus, dir);

   switch (axis)
   {
     case 0:  // asse X
             mat_setmatrix_of_eulerrotationZ(&mrot, (float)(-M_PI/2.0));
			 raux=bbx2-bbx1;
		     break;
     case 1:  // asse Y
             mat_identity(&mrot);
			 raux=bby2-bby1;
		     break;
     case 2:  // asse Z
             mat_setmatrix_of_eulerrotationX(&mrot, (float)(M_PI/2.0));
			 raux=bbz2-bbz1;
		     break;
   }

   mat_mul(&mr_plus, &mrot, &mrot2);
   mat_transpose(&mrot2, &imrot2);

   if (dolim) raux=uplim-lowlim;
   if (fabs(angle) <0.0001)
      r = 0;
   else	r = raux/angle;

   mat_setmatrix_of_pretraslation(&pret, &sub_center);
   mat_setmatrix_of_pretraslation(&ipret, &center);
   mat_mul (&mrot2, &pret, &tm);
   mat_mul (&ipret, &imrot2, &invtm);

   float len  = uplim-lowlim;
   float rat1, rat2;
//   AD_Vect3D pt;
   if (len==0.0f)
   {
      rat1 = rat2 = 1.0f;
   } 
   else
   {
     rat1 = uplim/len;
     rat2 = lowlim/len;
   }
/*
   tmAbove.IdentityMatrix();
   tmAbove.Translate(Point3(0.0f,0.0f,-to));
   tmAbove.RotateY(DegToRad(angle * rat1));
   tmAbove.Translate(Point3(0.0f,0.0f,to));
   pt = Point3(0.0f,0.0f,to);
   tmAbove.Translate((Map(0,pt*invtm)*tm)-pt);

tmBelow.IdentityMatrix();
tmBelow.Translate(Point3(0.0f,0.0f,-from));
tmBelow.RotateY(DegToRad(angle * rat2));	
tmBelow.Translate(Point3(0.0f,0.0f,from));
pt = Point3(0.0f,0.0f,from);
tmBelow.Translate((Map(0,pt*invtm)*tm)-pt);	
*/
}


void AD_BendModifier::Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   AD_Vect3D p, v;
   float x, y, c, s, yr;
   
    if (r==0)
	{
	   vect_copy(pos, out);
       return;
	}
	
    mat_mulvect (&tm, pos, &p);
	if (dolim)
	{
       if (p.y < lowlim)
	   {
          mat_mulvect (&tmBelow, &p, &v);
          mat_mulvect (&invtm, &v, out);
          return;
	   }
	   else if (p.y > uplim)
	   {
          mat_mulvect (&tmAbove, &p, &v);
          mat_mulvect (&invtm, &v, out);
          return;
	   }
	}	

	x = p.x;
	y = p.y;
	yr = y/r;
	c = cosf((float)(M_PI-yr));
	s = sinf((float)(M_PI-yr));
	p.x = r*c + r - x*c;
	p.y = r*s - x*s;

    mat_mulvect (&invtm, &p, out);
}


// ***********************************************************
// *********************       MELT      *********************
// ***********************************************************

void AD_MeltModifier::init(void)
{
   int i;

   if (center_track!=(postrack *)NULL)
       center_track->init();

   if (amount_track!=(rolltrack *)NULL)
   {
       for (i=0; i<amount_track->numkey; i++)
         amount_track->rollazioni[i].roll*=(float)(180/M_PI/100);
	   amount_track->init();
   }

   if (spread_track!=(rolltrack *)NULL)
   {
       for (i=0; i<spread_track->numkey; i++)
         spread_track->rollazioni[i].roll*=(float)(180/M_PI/100);
	   spread_track->init();
   }

   if (axis==0) axis=1;
   else if (axis==1) axis=0;
}


void AD_MeltModifier::update(float4 framepos)
{
   cx=(bbx2+bbx1)*0.5f;
   cy=(bby2+bby1)*0.5f;
   cz=(bbz2+bbz1)*0.5f;

   xsize = bbx2 - bbx1;
   ysize = bby2 - bby1;
   zsize = bbz2 - bbz1;
   size=(xsize>ysize) ? xsize:ysize;
   size=(zsize>size) ? zsize:size;
   size /= 2.0f;

   if (center_track!=(postrack *)NULL)
      center_track->get_data(framepos, &center);

   if (amount_track!=(rolltrack *)NULL)
   {
	   amount_track->get_data(framepos, &amount);
	   if (amount < 0) amount=0;
   }

   if (spread_track!=(rolltrack *)NULL)
	   spread_track->get_data(framepos, &spread);
   {
	   if (spread < 0) spread=0;
	   else
	   if (spread > 1) spread=1;
   }
}


float hypot (float l1, float l2)
{
	return (sqrtf(l1*l1+l2*l2));
}

float sign (float v)
{
   if (v<0) return(-1);
   else 
   if (v>0) return(1);
   else return(0);
}

void AD_MeltModifier::Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   float x, y, z;
   float xw, yw, zw;
   float vdist, mfac, dx, dy;
   float defsinex, coldef, realmax;
   AD_Vect3D p;

   vect_sub(pos, &center, &p);
//   vect_copy(pos, &p);
	
	x = p.x; y = p.y; z = p.z;
	xw= x-cx; yw= y-cy; zw= z-cz;
//	xw= x-center.x; yw= y-center.y; zw= z-center.z;

	if(xw==0.0 && yw==0.0 && zw==0.0) xw=yw=zw=1.0f; // Kill singularity for XW,YW,ZW
	if(x==0.0 && y==0.0 && z==0.0) x=y=z=1.0f; // Kill singularity for XYZ

	// Find distance from centre
	vdist=sqrtf(xw*xw+yw*yw+zw*zw);
	
	mfac=size/vdist;

	if(axis==0){
		dx = xw+sign(xw)*((float) (fabs(xw*mfac))*(amount*spread));
		dy = yw+sign(yw)*((float) (fabs(yw*mfac))*(amount*spread));
		x=(dx+cx);
		y=(dy+cy);
	}
	if(axis==1){
		dx = xw+sign(xw)*((float) (fabs(xw*mfac))*(amount*spread));
		dy = zw+sign(zw)*((float) (fabs(zw*mfac))*(amount*spread));
		x=(dx+cx);
		z=(dy+cz);
	}
	if(axis==2){
		dx = zw+sign(zw)*((float) (fabs(zw*mfac))*(amount*spread));
		dy = yw+sign(yw)*((float) (fabs(yw*mfac))*(amount*spread));
		z=(dx+cz);
		y=(dy+cy);
	}


	if(axis==0) if(p.z<(bbz1)) goto skipmelt;
	if(axis==1) if(p.y<(bby1)) goto skipmelt;
	if(axis==2) if(p.x<(bbx1)) goto skipmelt;


	if(axis==0) realmax = (float)hypot( (bbx2-cx),(bby2-cy) );
	if(axis==1) realmax = (float)hypot( (bbx2-cx),(bbz2-cz) );
	if(axis==2) realmax = (float)hypot( (bbz2-cz),(bby2-cy) );


	if(axis==0){
	defsinex = (float)hypot( (x-cx),(y-cy) );
	coldef = realmax - (float)hypot( (x-cx),(y-cy) );
	}
	if(axis==1){
	defsinex = (float)hypot( (x-cx),(z-cz) );
	coldef = realmax - (float)hypot( (x-cx),(z-cz) );
	}
	if(axis==2){
	defsinex = (float)hypot( (z-cz),(y-cy) );
	coldef = realmax - (float)hypot( (z-cz),(y-cy) );
	}


	if (coldef<0.0f) coldef=0.0f;


	defsinex+=(coldef/solidity);

	// Melt me!
	if(axis==0){
		if(!negaxis)
		{
			z-=(defsinex*amount);
			if(z<=bbz1) z=bbz1;
//			if(z<=(bbox.pmin.z+zbr)) z=(bbox.pmin.z+zbr);
		}
		else
		{
			z+=(defsinex*amount);
			if(z>=bbz2) z=bbz2;
//			if(z>=(bbox.pmax.z+zbr)) z=(bbox.pmax.z+zbr);
		}
	}
	if(axis==1){
		if(!negaxis)
		{
			y-=(defsinex*amount);
			if(y<=bby1) y=bby1;
//			if(y<=(bbox.pmin.y+zbr)) y=(bbox.pmin.y+zbr);
		}
		else
		{
			y+=(defsinex*amount);
			if(y>=bby2) y=bby2;
//			if(y>=(bbox.pmax.y+zbr)) y=(bbox.pmax.y+zbr);
		}
	}
	if(axis==2){
		if(!negaxis)
		{
			x-=(defsinex*amount);
			if(x<=bbx1) x=bbx1;
//			if(x<=(bbox.pmin.x+zbr)) x=(bbox.pmin.x+zbr);
		}
		else
		{
			x+=(defsinex*amount);
			if(x>=bbx2) x=bbx2;
//			if(x>=(bbox.pmax.x+zbr)) x=(bbox.pmax.x+zbr);
		}
	}


    skipmelt:

    out->x = x+center.x;
    out->y = y+center.y;
    out->z = z+center.z;
}



// ***********************************************************
// **********************      TAPER     *********************
// ***********************************************************

void AD_TaperModifier::init(void)
{
   int i;

   if (center_track!=(postrack *)NULL)
       center_track->init();

   if (amount_track!=(rolltrack *)NULL)
   {
       for (i=0; i<amount_track->numkey; i++)
         amount_track->rollazioni[i].roll*=(float)(180/M_PI);
	   amount_track->init();
   }

   if (curve_track!=(rolltrack *)NULL)
   {
       for (i=0; i<curve_track->numkey; i++)
         curve_track->rollazioni[i].roll*=(float)(180/M_PI);
	   curve_track->init();
   }

   if (uplim_track!=(rolltrack *)NULL)
   {
       for (i=0; i<uplim_track->numkey; i++)
         uplim_track->rollazioni[i].roll*=(float)(180/M_PI);
	   uplim_track->init();
   }

   if (lowlim_track!=(rolltrack *)NULL)
   {
       for (i=0; i<lowlim_track->numkey; i++)
         lowlim_track->rollazioni[i].roll*=(float)(180/M_PI);
	   lowlim_track->init();
   }

   if (axis==2) axis=1;
   else if (axis==1) axis=2;
}


void AD_TaperModifier::update(float4 framepos)
{
   AD_Vect3D sub_center;
   AD_Matrix mrot, imrot, pret, ipret;

   if (center_track!=(postrack *)NULL)
      center_track->get_data(framepos, &center);

   if (amount_track!=(rolltrack *)NULL)
	   amount_track->get_data(framepos, &amount);

   if (curve_track!=(rolltrack *)NULL)
	   curve_track->get_data(framepos, &curve);

   if (uplim_track!=(rolltrack *)NULL)
	   uplim_track->get_data(framepos, &uplim);

   if (lowlim_track!=(rolltrack *)NULL)
	   lowlim_track->get_data(framepos, &lowlim);

   // costruzione matrice di trasformazione e la sua
   // inversa
   sub_center.x=-center.x;
   sub_center.y=-center.y;
   sub_center.z=-center.z;

   switch (axis)
   {
     case 0:  // asse X
             mat_setmatrix_of_eulerrotationZ(&mrot, (float)(-M_PI/2.0));
			 l=bbx2-bbx1;
		     break;
     case 1:  // asse Y
             mat_identity(&mrot);
			 l=bby2-bby1;
		     break;
     case 2:  // asse Z
             mat_setmatrix_of_eulerrotationX(&mrot, (float)(M_PI/2.0));
			 l=bbz2-bbz1;
		     break;
   }

   mat_transpose(&mrot, &imrot);
   mat_setmatrix_of_pretraslation(&pret, &sub_center);
   mat_setmatrix_of_pretraslation(&ipret, &center);
   mat_mul (&mrot, &pret, &tm);
   mat_mul (&ipret, &imrot, &invtm);

   switch (effectaxis)
   {
     case 0: doX = 1;  doY = 0; break;
     case 1: doX = 0;  doY = 1;  break;
     case 2: doX = 1;  doY = 1;  break;
   }
}


void AD_TaperModifier::Map(AD_Vect3D *pos, AD_Vect3D *out)
{
	float f, z;
	AD_Vect3D p;

	if (l == 0)
	{
       vect_copy(pos, out);
       return;
	}
	mat_mulvect(&tm, pos, &p);

	if (dolim)
	{
		if (p.y<lowlim)
		{
			z = lowlim/l;
		}
		else 
		if (p.y>uplim)
		{
			z = uplim/l;
		}
		else
		{
			z = p.y/l;
		}
	}
	else
	{	
		z = p.y/l;
	}	

	if (symmetry && z<0.0f) z = -z;	
	f =  1 + z*amount + curve*z*(1-z);	
  	if (doX) p.x *= f;
  	if (doY) p.z *= f;

	mat_mulvect(&invtm, &p, out);
}



// ***********************************************************
// **********************      NOISE     *********************
// ***********************************************************
#define B 0x100
#define BM 0xff
#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define ADOT(a,b) (a[0] * b[0] + a[1] * b[1] + a[2] * b[2])

#define setup(u,b0,b1,r0,r1) \
	t = u + N; \
	b0 = ((int)t) & BM; \
	b1 = (b0+1) & BM; \
	r0 = t - (float)((int)t); \
	r1 = r0 - 1.0f; \

static int p[B + B + 2];
static float g[B + B + 2][3];

#define at(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )
#define s_curve(t) ( t * t * (3.0f - 2.0f * t) )
#define lerp(t, a, b) ( a + t * (b - a) )

float AD_NoiseModifier::noise3(float f1, float f2, float f3)
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	float rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	int i, j;

	setup(f1, bx0,bx1, rx0,rx1);
	setup(f2, by0,by1, ry0,ry1);
	setup(f3, bz0,bz1, rz0,rz1);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

	q = g[ b00 + bz0 ] ; u = at(rx0,ry0,rz0);
	q = g[ b10 + bz0 ] ; v = at(rx1,ry0,rz0);
	a = lerp(t, u, v);

	q = g[ b01 + bz0 ] ; u = at(rx0,ry1,rz0);
	q = g[ b11 + bz0 ] ; v = at(rx1,ry1,rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);		/* interpolate in y at lo x */

	q = g[ b00 + bz1 ] ; u = at(rx0,ry0,rz1);
	q = g[ b10 + bz1 ] ; v = at(rx1,ry0,rz1);
	a = lerp(t, u, v);

	q = g[ b01 + bz1 ] ; u = at(rx0,ry1,rz1);
	q = g[ b11 + bz1 ] ; v = at(rx1,ry1,rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);		/* interpolate in y at hi x */

	return 1.5f * lerp(sz, c, d);	/* interpolate in z */
}


void AD_NoiseModifier::init(void)
{
   int i, j, k;

   if (center_track!=(postrack *)NULL)
       center_track->init();

   if (strength_track!=(postrack *)NULL)
       strength_track->init();

   if (scale_track!=(rolltrack *)NULL)
   {
       for (i=0; i<scale_track->numkey; i++)
         scale_track->rollazioni[i].roll*=(float)(180/M_PI);
	   scale_track->init();
   }

   if (freq_track!=(rolltrack *)NULL)
   {
       for (i=0; i<freq_track->numkey; i++)
         freq_track->rollazioni[i].roll*=(float)(180/M_PI);
	   freq_track->init();
   }

   if (phase_track!=(rolltrack *)NULL)
   {
       for (i=0; i<phase_track->numkey; i++)
         phase_track->rollazioni[i].roll*=(float)(180/(M_PI*16));
	   phase_track->init();
   }

   scale=1.0f/scale;
   rough=1-rough;

   float v[3], s;

	/* Create an array of random vectors uniformly on the unit sphere */
	srand(1);
	for (i = 0 ; i < B ; i++) {
		do {				/* Choose uniformly in a cube */
			for (j=0 ; j<3 ; j++)
				v[j] = (float)((rand() % (B + B)) - B) / B;
			s = ADOT(v,v);
		} while (s > 1.0);		/* If not in sphere try again */
		s = (float)(sqrt(s));
		for (j = 0 ; j < 3 ; j++)	/* Else normalize */
			g[i][j] = v[j] / s;
	}

	/* Create a pseudorandom permutation of [1..B] */
	for (i = 0 ; i < B ; i++)
		p[i] = i;
	for (i = B ; i > 0 ; i -= 2) {
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	/* Extend g and p arrays to allow for faster indexing */
	for (i = 0 ; i < B + 2 ; i++) {
		p[B + i] = p[i];
		for (j = 0 ; j < 3 ; j++)
			g[B + i][j] = g[i][j];
	}
}


void AD_NoiseModifier::update(float4 framepos)
{
   float fphase;

   if (center_track!=(postrack *)NULL)
      center_track->get_data(framepos, &center);

   if (scale_track!=(rolltrack *)NULL)
   {
	   scale_track->get_data(framepos, &scale);
	   if (scale<0)
	   {
		   scale=0.00001f;
	   }
	   else scale=1.0f/scale;
   }

   if (strength_track!=(postrack *)NULL)
	   strength_track->get_data(framepos, &strength);

   if (phase_track!=(rolltrack *)NULL)
   {
	   phase_track->get_data(framepos, &fphase);
	   phase=(int)fphase;
   }

   if (freq_track!=(rolltrack *)NULL)
   {
	   freq_track->get_data(framepos, &freq);
	   if (freq<0) freq=0;
   }

   if (animate)
   {
      time = fphase * float(0.005) * freq *
			1200.0f/float(160); // + Perm(seed);
   }
   else	time = 0.5; //(float)Perm(seed);
   return;
}


void AD_NoiseModifier::Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   AD_Vect3D d, sp, p;

   p.x=pos->x-center.x;
   p.y=pos->y-center.y;
   p.z=pos->z-center.z;

   sp.x=(0.5f + p.x*scale);
   sp.y=(0.5f + p.y*scale);
   sp.z=(0.5f + p.z*scale);

   if (fractal)
   {
      /*
      d.x = fBm1(Point3(sp.y,sp.z,time),rough,2.0f,iterations);
      d.y = fBm1(Point3(sp.x,sp.z,time),rough,2.0f,iterations);
      d.z = fBm1(Point3(sp.x,sp.y,time),rough,2.0f,iterations);
	  */
      vect_copy(pos, out);
	  return;
   }
   else
   {
      d.x = noise3(sp.z, time, sp.y);
      d.z = noise3(sp.x, time, sp.y);
      d.y = noise3(sp.x, time, sp.z);
   }

   out->x = (p.x + d.x*strength.x) + center.x;
   out->y = (p.y + d.y*strength.y) + center.y;
   out->z = (p.z + d.z*strength.z) + center.z;
}



// ***********************************************************
// **********************     STRETCH    *********************
// ***********************************************************

void AD_StretchModifier::init(void)
{
   int i;

   if (center_track!=(postrack *)NULL)
       center_track->init();

   if (stretch_track!=(rolltrack *)NULL)
   {
       for (i=0; i<stretch_track->numkey; i++)
         stretch_track->rollazioni[i].roll*=(float)(180/M_PI);
	   stretch_track->init();
   }

   if (amplify_track!=(rolltrack *)NULL)
   {
       for (i=0; i<amplify_track->numkey; i++)
         amplify_track->rollazioni[i].roll*=(float)(180/M_PI);
	   amplify_track->init();
   }

   if (uplim_track!=(rolltrack *)NULL)
   {
       for (i=0; i<uplim_track->numkey; i++)
         uplim_track->rollazioni[i].roll*=(float)(180/M_PI);
	   uplim_track->init();
   }

   if (lowlim_track!=(rolltrack *)NULL)
   {
       for (i=0; i<lowlim_track->numkey; i++)
         lowlim_track->rollazioni[i].roll*=(float)(180/M_PI);
	   lowlim_track->init();
   }

   if (axis==2) axis=1;
   else if (axis==1) axis=2;

   return;
}


void AD_StretchModifier::update(float4 framepos)
{
   AD_Vect3D sub_center;
   AD_Matrix mrot, pret, ipret, imrot;
   float amplify_aux;

   if (center_track!=(postrack *)NULL)
       center_track->get_data(framepos, &center);

   if (stretch_track!=(rolltrack *)NULL)
	   stretch_track->get_data(framepos, &stretch);

   if (amplify_track!=(rolltrack *)NULL)
	   amplify_track->get_data(framepos, &amplify);

   if (uplim_track!=(rolltrack *)NULL)
	   uplim_track->get_data(framepos, &uplim);

   if (lowlim_track!=(rolltrack *)NULL)
	   lowlim_track->get_data(framepos, &lowlim);

   // costruzione matrice di trasformazione e la sua
   // inversa
   sub_center.x=-center.x;
   sub_center.y=-center.y;
   sub_center.z=-center.z;

   switch (axis)
   {
     case 0:  // asse X
             mat_setmatrix_of_eulerrotationZ(&mrot, (float)(-M_PI/2.0));
		     break;
     case 1:  // asse Y
             mat_identity(&mrot);
		     break;
     case 2:  // asse Z
             mat_setmatrix_of_eulerrotationX(&mrot, (float)(M_PI/2.0));
		     break;
   }

   mat_transpose(&mrot, &imrot);
   mat_setmatrix_of_pretraslation(&pret, &sub_center);
   mat_setmatrix_of_pretraslation(&ipret, &center);
   mat_mul (&mrot, &pret, &tm);
   mat_mul (&ipret, &imrot, &invtm);

   amplify_aux = (amplify >= 0) ? amplify + 1 : 1.0f / (-amplify + 1.0f);
   amplify=amplify_aux;
   if (!dolim)
   {
      switch ( axis )
	  {
        case 0:
            heightMin = bbx1;
            heightMax = bbx2;
            break;
        case 1:
            heightMin = bby1;
            heightMax = bby2;
            break;
        case 2:
            heightMin = bbz1;
            heightMax = bbz2;
            break;
        }
    } else {
        heightMin = lowlim;
        heightMax = uplim;
    }

   return;
}


void AD_StretchModifier::Map(AD_Vect3D *pos, AD_Vect3D *out)
{
    float fraction, normHeight;
    float xyScale, zScale, a, b, c;
	AD_Vect3D p;
    
    if (stretch == 0 || (heightMax - heightMin == 0))
	{
        vect_copy(pos, out);
		return;
	}
    
    mat_mulvect(&tm, pos, &p);
    
    if (dolim && p.y > uplim)
        normHeight = (uplim - heightMin) / (heightMax - heightMin);
    else if (dolim && p.y < lowlim)
        normHeight = (lowlim - heightMin) / (heightMax - heightMin);
    else
        normHeight = (p.y - heightMin) / (heightMax - heightMin);

    if (stretch < 0) {   // Squash
        xyScale = (amplify * -stretch + 1.0f);
        zScale = (-1.0f / (stretch - 1.0f));
    } else {           // Stretch
        xyScale = 1.0f / (amplify * stretch + 1.0f);
        zScale = stretch + 1.0f;
    }

    // a, b, and c are the coefficients of the quadratic function f(x)
    // such that f(0) = 1, f(1) = 1, and f(0.5) = xyScale
    a = 4.0F * (1.0F - xyScale);
    b = -4.0F * (1.0F - xyScale);
    c = 1.0F;
    fraction = (((a * normHeight) + b) * normHeight) + c;
    p.x *= fraction;
    p.z *= fraction;

    if (dolim && p.y < lowlim)
	p.y += (zScale - 1.0F) * lowlim;
    else if (dolim && p.y <= uplim)
	p.y *= zScale;
    else if (dolim && p.y > uplim)
	p.y += (zScale - 1.0F) * uplim;
    else
	p.y *= zScale;

    mat_mulvect(&invtm, &p, out);
}


// ***********************************************************
// ************************     FFD    ***********************
// ***********************************************************

void AD_FFDModifier::init(void)
{
  int i;

  // ############  INIZIALIZZAZIONI DELLE KEYFRAMER  ##########
  if (positiontrack.init()==-1) return;
  if (rotationtrack.init()==-1) return;
  if (scaletrack.init()==-1)    return;

  for (i=0; i<num_cpoints; i++) cpoints_tracks[i].init();
}


void AD_FFDModifier::update(float4 framepos)
{
  int w;
  AD_Vect3D postmp, stmp;
  AD_Quaternion objrot;
  AD_Matrix posttrans, scaling, maux, mrot;
  AD_Matrix inv_posttrans, inv_scaling, inv_mrot;
  AD_Vect3D size;

  // Matrice di rotazione e la sua inversa
  mat_identity(&mrot);
  if (rotationtrack.numkey>0)
  {
    rotationtrack.get_data(framepos, &objrot);
    quat_rotquat_to_matrix(&objrot, &mrot);
  }
  mat_copy(&mrot, &latticematrix);
  mat_transpose(&mrot, &inv_mrot);

  // Matrice di scaling e la sua inversa
  if (scaletrack.numkey>0)
  {
    scaletrack.get_data(framepos, &stmp);
    mat_setmatrix_of_scaling(&scaling, stmp.x, stmp.y, stmp.z);
    mat_setmatrix_of_scaling(&inv_scaling, 1.0f/stmp.x, 1.0f/stmp.y, 1.0f/stmp.z);
  }
  else
  {
     mat_identity(&scaling);
     mat_identity(&inv_scaling);
  }
   
  // Matrice di traslazione e la sua inversa
  if (positiontrack.numkey>0) positiontrack.get_data(framepos, &currentpos);
  postmp.x=-currentpos.x;
  postmp.y=-currentpos.y;
  postmp.z=-currentpos.z;
  mat_setmatrix_of_pretraslation(&posttrans, &currentpos);
  mat_setmatrix_of_pretraslation(&inv_posttrans, &postmp);

  // Costruzione matrice lattice
  mat_mul(&scaling, &mrot, &maux);
  mat_mul(&posttrans, &maux, &latticematrix);
  // Costruzione matrice inversa del lattice
  mat_mul(&inv_scaling, &inv_posttrans, &maux);
  mat_mul(&inv_mrot, &maux, &inv_latticematrix);

  // Costruzione della size: servirà per portare nello spazio
  // normalizzato (0..1) del lattice il punto da mappare
  size.x=bbx2-bbx1; if (size.x==0) size.x=0.001f;
  size.y=bby2-bby1; if (size.y==0) size.y=0.001f;
  size.z=bbz2-bbz1; if (size.z==0) size.z=0.001f;
  mat_setmatrix_of_scaling(&scaling, size.x, size.y, size.z);
  mat_setmatrix_of_scaling(&inv_scaling, 1.0f/size.x, 1.0f/size.y, 1.0f/size.z);
  // L'origine della TM è l'angolo in basso a sx della bounding
  // box, non il centro.
  stmp.x=currentpos.x-bbx1;
  stmp.y=currentpos.y-bby1;
  stmp.z=currentpos.z-bbz1;
  mat_setmatrix_of_pretraslation(&posttrans, &stmp);
  stmp.x=-stmp.x;
  stmp.y=-stmp.y;
  stmp.z=-stmp.z;
  mat_setmatrix_of_pretraslation(&inv_posttrans, &stmp);

  // Costruzione di tm, e invtm
  mat_mul(&inv_scaling, &posttrans, &maux);
  mat_mul(&maux, &inv_latticematrix, &tm);

  mat_mul(&inv_posttrans, &scaling, &maux);
  mat_mul(&latticematrix, &maux, &invtm);

  for (w=0; w<num_cpoints; w++)
  {
	if (cpoints_tracks[w].numkey>0) cpoints_tracks[w].get_data(framepos, &cpoints[w]);
  }
}


#define EPSILON	0.001f
#define GRIDINDEX44(i,j,k) (((i)<<4) + ((j)<<2) + (k))
#define GRIDINDEX33(i,j,k) (((i)*9) + ((j)*3) + (k))
#define GRIDINDEX22(i,j,k) (((i)<<2) + ((j)<<1) + (k))
inline float BPoly4(int i, float u)
{
	float s = 1.0f-u;
	switch (i) {
		case 0: return s*s*s;
		case 1: return 3.0f*u*s*s;
		case 2: return 3.0f*u*u*s;
		case 3: return u*u*u;
		default: return 0.0f;
		}
}
inline float BPoly3(int i, float u)
{
	float s = 1.0f-u;
	switch (i) {
		case 0: return s*s;
		case 1: return 2.0f*u*s;
		case 2: return u*u;
		default: return 0.0f;
		}
}
inline float BPoly2(int i, float u)
{	
	switch (i) {
		case 0: return 1.0f-u;
		case 1: return u;
		default: return 0.0f;
		}
}


void AD_FFDModifier::Map(AD_Vect3D *pos, AD_Vect3D *out)
{
	AD_Vect3D q, pp, v;
	float kw;
	
    if (dim1==3)
	{
	   Map3(pos, out);
	   return;
	}
	else
	if (dim1==2)
	{
	   Map2(pos, out);
	   return;
	}

    // Map4 inizia
	mat_mulvect(&tm, pos, &pp);
	if (invol)
	{
       if (pp.x<-EPSILON || pp.x>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
       if (pp.y<-EPSILON || pp.y>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
       if (pp.z<-EPSILON || pp.z>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
	}


	q.x=q.y=q.z=0;
	// Compute the deformed point as a weighted average of all
	// 64 control points.
	for (int i=0; i<dim1; i++)
	{
		for (int j=0; j<dim2; j++)
		{
			for (int k=0; k<dim3; k++)
			{
				v=cpoints[GRIDINDEX44(i,j,k)];
				kw=BPoly4(i, pp.x)*BPoly4(j, pp.z)*BPoly4(k, pp.y);
				v.x=v.x*kw;
				v.y=v.y*kw;
				v.z=v.z*kw;
				vect_add_inline(&q, &v, &q);
			}
		}
	}
	
	mat_mulvect(&invtm, &q, out);
	return;
}


void AD_FFDModifier::Map3(AD_Vect3D *pos, AD_Vect3D *out)
{
	AD_Vect3D q, pp, v;
	float kw;
	
	mat_mulvect(&tm, pos, &pp);

	if (invol)
	{
       if (pp.x<-EPSILON || pp.x>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
       if (pp.y<-EPSILON || pp.y>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
       if (pp.z<-EPSILON || pp.z>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
	}


	q.x=q.y=q.z=0;
	// Compute the deformed point as a weighted average of all
	// 64 control points.
	for (int i=0; i<dim1; i++)
	{
		for (int j=0; j<dim2; j++)
		{
			for (int k=0; k<dim3; k++)
			{
				v=cpoints[GRIDINDEX33(i,j,k)];
				kw=BPoly3(i, pp.x)*BPoly3(j, pp.z)*BPoly3(k, pp.y);
				v.x=v.x*kw;
				v.y=v.y*kw;
				v.z=v.z*kw;
				vect_add_inline(&q, &v, &q);
			}
		}
	}
	
	mat_mulvect(&invtm, &q, out);
	return;
}


void AD_FFDModifier::Map2(AD_Vect3D *pos, AD_Vect3D *out)
{
	AD_Vect3D q, pp, v;
	float kw;
	
	mat_mulvect(&tm, pos, &pp);

	if (invol)
	{
       if (pp.x<-EPSILON || pp.x>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
       if (pp.y<-EPSILON || pp.y>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
       if (pp.z<-EPSILON || pp.z>1.0f+EPSILON)
	   {
		  vect_copy(pos, out);
          return;
	   }
	}


	q.x=q.y=q.z=0;
	// Compute the deformed point as a weighted average of all
	// 64 control points.
	for (int i=0; i<dim1; i++)
	{
		for (int j=0; j<dim2; j++)
		{
			for (int k=0; k<dim3; k++)
			{
				v=cpoints[GRIDINDEX22(i,j,k)];
				kw=BPoly2(i, pp.x)*BPoly2(j, pp.z)*BPoly2(k, pp.y);
				v.x=v.x*kw;
				v.y=v.y*kw;
				v.z=v.z*kw;
				vect_add_inline(&q, &v, &q);
			}
		}
	}
	
	mat_mulvect(&invtm, &q, out);
	return;
}