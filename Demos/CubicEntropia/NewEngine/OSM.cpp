#include <math.h>
#include "OSM.h"
#include "FastMath.h"

CObjectSpaceModifier::CObjectSpaceModifier()
{
  vect_set(&p_Center, 0, 0, 0);
  p_GizmoPositionTrack=(CPosTrack *)NULL;
  p_GizmoRotationTrack=(CRotTrack *)NULL;
  p_GizmoScaleTrack=(CPosTrack *)NULL;
}


void CObjectSpaceModifier::m_Init (void)
{
  if (p_GizmoPositionTrack)
	p_GizmoPositionTrack->m_Init(1);
  if (p_GizmoRotationTrack)
    p_GizmoRotationTrack->m_Init(1);
  if (p_GizmoScaleTrack)
    p_GizmoScaleTrack->m_Init(1);
}


void CObjectSpaceModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
  AD_Vect3D postmp, stmp;
  AD_Quaternion objrot;
  AD_Matrix posttrans, scaling, maux, mrot;
  AD_Matrix inv_posttrans, inv_scaling, inv_mrot;

  // Matrice di rotazione e la sua inversa
  mat_identity(&mrot);
  if (p_GizmoRotationTrack)
  {
    p_GizmoRotationTrack->m_GetData(framepos, &objrot);
    quat_rotquat_to_matrix(&objrot, &mrot);
  }
  mat_copy(&mrot, &p_GizmoMatrix);
  mat_transpose(&mrot, &inv_mrot);

  // Matrice di scaling e la sua inversa
  if (p_GizmoScaleTrack)
  {
    p_GizmoScaleTrack->m_GetData(framepos, &stmp);
    mat_setmatrix_of_scaling(&scaling, stmp.x, stmp.y, stmp.z);
    mat_setmatrix_of_scaling(&inv_scaling, 1.0f/stmp.x, 1.0f/stmp.y, 1.0f/stmp.z);
  }
  else
  {
     mat_identity(&scaling);
     mat_identity(&inv_scaling);
  }
   
  // Matrice di traslazione e la sua inversa
  if (p_GizmoPositionTrack)
	 p_GizmoPositionTrack->m_GetData(framepos, &p_Center);

  vect_neg(&p_Center, &postmp);
  mat_setmatrix_of_pretraslation(&posttrans, &p_Center);
  mat_setmatrix_of_pretraslation(&inv_posttrans, &postmp);

  // Costruzione matrice lattice (che porta nello spazio del lattice)
  mat_mul(&scaling, &mrot, &maux);
  mat_mul(&posttrans, &maux, &p_GizmoMatrix);

  // Costruzione matrice inversa del lattice
  mat_mul(&inv_scaling, &inv_posttrans, &maux);
  mat_mul(&inv_mrot, &maux, &p_InverseGizmoMatrix);
}


// ***********************************************************
// ********************       TWIST      *********************
// ***********************************************************
void CTwistModifier::m_Init(void)
{
   if (p_AngleTrack!=(CRollTrack *)NULL)
	   p_AngleTrack->m_Init(1);
   if (p_BiasTrack!=(CRollTrack *)NULL)
	   p_BiasTrack->m_Init(1);
   if (p_UpLimTrack!=(CRollTrack *)NULL)
	   p_UpLimTrack->m_Init(1);
   if (p_LowLimTrack!=(CRollTrack *)NULL)
	   p_LowLimTrack->m_Init(1);
   CObjectSpaceModifier::m_Init();
   // swap y<-->z
   if (p_Axis==2) p_Axis=1;
   else if (p_Axis==1) p_Axis=2;
}


void CTwistModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
   AD_Vect3D sub_p_Center;
   AD_Matrix mrot, imrot, pret, ipret, M, iM, maux;

   CObjectSpaceModifier::m_Update(framepos, pivot);

   if (p_AngleTrack!=(CRollTrack *)NULL)
	   p_AngleTrack->m_GetData(framepos, &p_Angle);
   if (p_BiasTrack!=(CRollTrack *)NULL)
	   p_BiasTrack->m_GetData(framepos, &p_Bias);
   if (p_UpLimTrack!=(CRollTrack *)NULL)
	   p_UpLimTrack->m_GetData(framepos, &p_UpLim);
   if (p_LowLimTrack!=(CRollTrack *)NULL)
	   p_LowLimTrack->m_GetData(framepos, &p_LowLim);

   // costruzione matrice di trasformazione e la sua
   // inversa
   sub_p_Center.x=-p_Center.x;
   sub_p_Center.y=-p_Center.y;
   sub_p_Center.z=-p_Center.z;

   switch (p_Axis)
   {
     case 0:  // asse X
             mat_setmatrix_of_eulerrotationZ(&mrot, (float4)(-Pi__*0.5f));
			 p_Height=p_bbx2-p_bbx1;
		     break;
     case 1:  // asse Y
             mat_identity(&mrot);
			 p_Height=p_bby2-p_bby1;
		     break;
     case 2:  // asse Z
             mat_setmatrix_of_eulerrotationX(&mrot, (float4)(Pi__*0.5f));
			 p_Height=p_bbz2-p_bbz1;
		     break;
   }

   mat_transpose(&mrot, &imrot);
   mat_setmatrix_of_pretraslation(&pret, &sub_p_Center);
   mat_setmatrix_of_pretraslation(&ipret, &p_Center);

   // costruzione matrice di offset
   mat_mul (&mrot, &pret, &maux);
   mat_mul (&ipret, &maux, &M);
   mat_mul (&imrot, &pret, &maux);
   mat_mul (&ipret, &maux, &iM);

   mat_mul(&p_InverseGizmoMatrix, &M, &p_TM);
   mat_mul(&iM, &p_GizmoMatrix, &p_InvTM);

   if (p_Height==0)
   {
	 p_Angle = 0.0f;
	 p_AngleOverHeight = 0.0f;
   }
   else p_AngleOverHeight = p_Angle / p_Height;
}


void CTwistModifier::m_Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   AD_Vect3D v;
   float4 x, y, z, a, cosine, sine;

   if (p_Angle==0.0f)
   {
	  vect_copy(pos, out);
	  return;
   }
   
   mat_mulvect (&p_TM, pos, &v);
   x = v.x;
   z = v.z;
	
   if (p_DoLim)
   {
      if (v.y < p_LowLim) y = p_LowLim;
	  else if (v.y > p_UpLim) y = p_UpLim;
	       else y = v.y;
   }
   else y = v.y;
	
   if (p_Bias>0)
   {
      float4 u = y/p_Height;
      a = p_Angle * (float4)pow(fabs(u), p_Bias);
      if (u<0.0) a = -a;
   }
   else a = y * p_AngleOverHeight;

   cosine = fast_cosf(a);
   sine = fast_sinf(a);
   v.x =  cosine*x + sine*z;
   v.z = -sine*x + cosine*z;

   mat_mulvect (&p_InvTM, &v, out);
}



// ***********************************************************
// ********************       RIPPLE     *********************
// ***********************************************************

void CRippleModifier::m_Init(void)
{
   if (p_Amp1Track!=(CRollTrack *)NULL)
	   p_Amp1Track->m_Init(1);
   if (p_Amp2Track!=(CRollTrack *)NULL)
	   p_Amp2Track->m_Init(1);
   if (p_WLenTrack!=(CRollTrack *)NULL)
	   p_WLenTrack->m_Init(1);
   if (p_PhaseTrack!=(CRollTrack *)NULL)
	   p_PhaseTrack->m_Init(1);
   if (p_DecayTrack!=(CRollTrack *)NULL)
	   p_DecayTrack->m_Init(1);
   CObjectSpaceModifier::m_Init();
}


void CRippleModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
   CObjectSpaceModifier::m_Update(framepos, pivot);

   if (p_Amp1Track!=(CRollTrack *)NULL)
	   p_Amp1Track->m_GetData(framepos, &p_Amp1);
   if (p_Amp2Track!=(CRollTrack *)NULL)
	   p_Amp2Track->m_GetData(framepos, &p_Amp2);
   if (p_WLenTrack!=(CRollTrack *)NULL)
	   p_WLenTrack->m_GetData(framepos, &p_WLen);
   if (p_PhaseTrack!=(CRollTrack *)NULL)
	   p_PhaseTrack->m_GetData(framepos, &p_Phase);
   if (p_DecayTrack!=(CRollTrack *)NULL)
   {
	   p_DecayTrack->m_GetData(framepos, &p_Decay);
	   if (p_Decay<0) p_Decay=0;
   }
   if (p_WLen == 0) p_WLen = 0.0000001f;
   mat_copy(&p_InverseGizmoMatrix, &p_TM);
   mat_copy(&p_GizmoMatrix, &p_InvTM);
}


void CRippleModifier::m_Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   AD_Vect3D pt;
   float4 r, oldZ, u, a, len, u2;

   mat_mulvect (&p_TM, pos, &pt);

   if (p_Amp1 != p_Amp2)
   {
     len  = vect_length(&pt);
     if (len==0) a = p_Amp1;
	 else
	 {
		u = (float4)(acos(pt.x/len)*InvPi__);
        u = (u >= 0.5) ? (1.0f-u) : u;
        u *= 2.0f;
        u2=u*u;
		u=u2*(-2.0f*u+3.0f);
        a = p_Amp1*(1.0f-u) + p_Amp2*u;
	 }
   }
   else	a = p_Amp1;
	
   oldZ = pt.y;
   r = sqrtf(pt.x*pt.x+pt.z*pt.z);
   pt.y = oldZ + a*fast_sinf(Pi2__*(r/p_WLen + p_Phase))*expf(-p_Decay*r);
   mat_mulvect (&p_InvTM, &pt, out);
}



// ***********************************************************
// *********************       BEND      *********************
// ***********************************************************
void CBendModifier::m_Init(void)
{
   if (p_AngleTrack!=(CRollTrack *)NULL)
	   p_AngleTrack->m_Init(1);
   if (p_DirTrack!=(CRollTrack *)NULL)
	   p_DirTrack->m_Init(1);
   if (p_UpLimTrack!=(CRollTrack *)NULL)
	   p_UpLimTrack->m_Init(1);
   if (p_LowLimTrack!=(CRollTrack *)NULL)
	   p_LowLimTrack->m_Init(1);
   CObjectSpaceModifier::m_Init();
   // swap y<-->z per il 3DSTudio
   if (p_Axis==2) p_Axis=1;
   else if (p_Axis==1) p_Axis=2;
}


void CBendModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
   AD_Vect3D sub_p_Center;
   AD_Matrix mrot, pret, ipret, mr_plus;
   AD_Matrix M, iM, maux, imrot;
   float4 raux;

   CObjectSpaceModifier::m_Update(framepos, pivot);

   if (p_AngleTrack!=(CRollTrack *)NULL)
	   p_AngleTrack->m_GetData(framepos, &p_Angle);
   if (p_DirTrack!=(CRollTrack *)NULL)
	   p_DirTrack->m_GetData(framepos, &p_Dir);
   if (p_UpLimTrack!=(CRollTrack *)NULL)
	   p_UpLimTrack->m_GetData(framepos, &p_UpLim);
   if (p_LowLimTrack!=(CRollTrack *)NULL)
	   p_LowLimTrack->m_GetData(framepos, &p_LowLim);

   // costruzione matrice di trasformazione e la sua
   // inversa
   sub_p_Center.x=-p_Center.x;
   sub_p_Center.y=-p_Center.y;
   sub_p_Center.z=-p_Center.z;
   mat_setmatrix_of_eulerrotationY(&mr_plus, p_Dir);

   switch (p_Axis)
   {
     case 0:  // asse X
             mat_setmatrix_of_eulerrotationZ(&mrot, (float4)(-Pi__/2.0));
			 raux=p_bbx2-p_bbx1;
		     break;
     case 1:  // asse Y
             mat_identity(&mrot);
			 raux=p_bby2-p_bby1;
		     break;
     case 2:  // asse Z
             mat_setmatrix_of_eulerrotationX(&mrot, (float4)(Pi__/2.0));
			 raux=p_bbz2-p_bbz1;
		     break;
   }

   mat_transpose(&mrot, &imrot);

   mat_setmatrix_of_pretraslation(&pret, &sub_p_Center);
   mat_setmatrix_of_pretraslation(&ipret, &p_Center);

   // costruzione matrice di offset
   mat_mul (&mrot, &pret, &maux);
   mat_mul (&ipret, &maux, &M);
   mat_mul (&imrot, &pret, &maux);
   mat_mul (&ipret, &maux, &iM);

   // la rotazione di p_Dir avviene quando si è nello spazio dell'osm
   mat_transpose(&mr_plus, &imrot);

   mat_mul(&p_InverseGizmoMatrix, &M, &maux);
   mat_mul(&mr_plus, &maux, &p_TM);

   mat_mul(&p_GizmoMatrix, &imrot, &maux);
   mat_mul(&iM, &maux, &p_InvTM);

   if (p_DoLim) raux=p_UpLim-p_LowLim;
   if (fabs(p_Angle) <0.0001) r = 0;
   else	r = raux/p_Angle;

   float4 len  = p_UpLim-p_LowLim;
   float4 rat1, rat2;
//   AD_Vect3D pt;
   if (len==0.0f)
   {
      rat1 = rat2 = 1.0f;
   } 
   else
   {
     rat1 = p_UpLim/len;
     rat2 = p_LowLim/len;
   }
}


void CBendModifier::m_Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   AD_Vect3D p, v;
   float4 x, y, c, s, yr;
   
    if (r==0)
	{
	   vect_copy(pos, out);
       return;
	}
	
    mat_mulvect (&p_TM, pos, &p);
	if (p_DoLim)
	{
       if (p.y < p_LowLim)
	   {
          mat_mulvect (&p_TMBelow, &p, &v);
          mat_mulvect (&p_InvTM, &v, out);
          return;
	   }
	   else if (p.y > p_UpLim)
	   {
          mat_mulvect (&p_TMAbove, &p, &v);
          mat_mulvect (&p_InvTM, &v, out);
          return;
	   }
	}	

	x = p.x;
	y = p.y;
	yr = y/r;
	c = fast_cosf(Pi__-yr);
	s = fast_sinf(Pi__-yr);
	//p.x = r*c + r - x*c;
	//p.y = r*s - x*s;
	p.x = (r-x)*c + r;
	p.y = (r-x)*s;
    mat_mulvect (&p_InvTM, &p, out);
}


// ***********************************************************
// *********************       MELT      *********************
// ***********************************************************

void CMeltModifier::m_Init(void)
{
   if (p_AmountTrack!=(CRollTrack *)NULL)
	   p_AmountTrack->m_Init(1.0f/100.0f);
   p_Amount=p_Amount*0.01f;

   if (p_SpreadTrack!=(CRollTrack *)NULL)
	   p_SpreadTrack->m_Init(1.0f/100.0f);
   p_Spread=p_Spread*0.01f;

   CObjectSpaceModifier::m_Init();
   // swap y<-->z per il 3DSTudio
   if (p_Axis==0) p_Axis=1;
   else if (p_Axis==1) p_Axis=0;
}


void CMeltModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
   CObjectSpaceModifier::m_Update(framepos, pivot);

   p_cx=(p_bbx2+p_bbx1)*0.5f;
   p_cy=(p_bby2+p_bby1)*0.5f;
   p_cz=(p_bbz2+p_bbz1)*0.5f;

   p_XSize = p_bbx2 - p_bbx1;
   p_YSize = p_bby2 - p_bby1;
   p_ZSize = p_bbz2 - p_bbz1;
   p_Size=(p_XSize>p_YSize) ? p_XSize:p_YSize;
   p_Size=(p_ZSize>p_Size) ? p_ZSize:p_Size;
   p_Size /= 2.0f;

   if (p_AmountTrack!=(CRollTrack *)NULL)
   {
	   p_AmountTrack->m_GetData(framepos, &p_Amount);
	   if (p_Amount < 0) p_Amount=0;
   }

   if (p_SpreadTrack!=(CRollTrack *)NULL)
	   p_SpreadTrack->m_GetData(framepos, &p_Spread);
   {
	   if (p_Spread < 0) p_Spread=0;
	   else
	   if (p_Spread > 1) p_Spread=1;
   }
   mat_copy(&p_InverseGizmoMatrix, &p_TM);
   mat_copy(&p_GizmoMatrix, &p_InvTM);
}


float4 hypot (float4 l1, float4 l2)
{
	return (sqrtf(l1*l1+l2*l2));
}

float4 sign (float4 v)
{
   if (v<0) return(-1);
   else 
   if (v>0) return(1);
   else return(0);
}

void CMeltModifier::m_Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   float4 x, y, z;
   float4 xw, yw, zw;
   float4 vdist, mfac, dx, dy;
   float4 defsinex, coldef, realmax;
   AD_Vect3D p, p2;

   mat_mulvect(&p_TM, pos, &p);

   x = p.x; y = p.y; z = p.z;
   xw= x-p_cx; yw= y-p_cy; zw= z-p_cz;

   if (xw==0.0 && yw==0.0 && zw==0.0) xw=yw=zw=1.0f;
   if (x==0.0 && y==0.0 && z==0.0) x=y=z=1.0f;

   // Find distance from centre
   vdist=sqrtf(xw*xw+yw*yw+zw*zw);
	
   mfac=p_Size/vdist;

   if (p_Axis==0)
   {
	  dx = xw+sign(xw)*((float4) (fabs(xw*mfac))*(p_Amount*p_Spread));
	  dy = yw+sign(yw)*((float4) (fabs(yw*mfac))*(p_Amount*p_Spread));
	  x=(dx+p_cx);
	  y=(dy+p_cy);
   }
   if (p_Axis==1)
   {
      dx = xw+sign(xw)*((float4) (fabs(xw*mfac))*(p_Amount*p_Spread));
	  dy = zw+sign(zw)*((float4) (fabs(zw*mfac))*(p_Amount*p_Spread));
	  x=(dx+p_cx);
	  z=(dy+p_cz);
   }
   if (p_Axis==2)
   {
	  dx = zw+sign(zw)*((float4) (fabs(zw*mfac))*(p_Amount*p_Spread));
	  dy = yw+sign(yw)*((float4) (fabs(yw*mfac))*(p_Amount*p_Spread));
	  z=(dx+p_cz);
	  y=(dy+p_cy);
   }

   if (p_Axis==0)
   {
	  if (p.z<(p_bbz1)) goto skipmelt;
      realmax = hypot((p_bbx2-p_cx), (p_bby2-p_cy));
	  defsinex = (float4)hypot( (x-p_cx),(y-p_cy) );
	  coldef = realmax - (float4)hypot( (x-p_cx),(y-p_cy) );
   }
   else
   if (p_Axis==1)
   {
	  if (p.y<(p_bby1)) goto skipmelt;
      realmax = hypot((p_bbx2-p_cx), (p_bbz2-p_cz));
	  defsinex = hypot((x-p_cx), (z-p_cz));
	  coldef = realmax - hypot((x-p_cx), (z-p_cz));
   }
   else
   if (p_Axis==2)
   {
	  if (p.x<(p_bbx1)) goto skipmelt;
      realmax = hypot((p_bbz2-p_cz), (p_bby2-p_cy));
	  defsinex = hypot((z-p_cz), (y-p_cy));
	  coldef = realmax - hypot((z-p_cz), (y-p_cy));
   }

   if (coldef<0.0f) coldef=0.0f;
   defsinex+=(coldef/p_Solidity);

   // Melt me!
   if(p_Axis==0)
   {
		if(!p_NegAxis)
		{
			z-=(defsinex*p_Amount);
			if (z<=p_bbz1) z=p_bbz1;
		}
		else
		{
			z+=(defsinex*p_Amount);
			if (z>=p_bbz2) z=p_bbz2;
		}
   }
   else
   if (p_Axis==1)
   {
		if (!p_NegAxis)
		{
			y-=(defsinex*p_Amount);
			if (y<=p_bby1) y=p_bby1;
		}
		else
		{
			y+=(defsinex*p_Amount);
			if (y>=p_bby2) y=p_bby2;
		}
   }
   else
   if (p_Axis==2)
   {
		if (!p_NegAxis)
		{
			x-=(defsinex*p_Amount);
			if (x<=p_bbx1) x=p_bbx1;
		}
		else
		{
			x+=(defsinex*p_Amount);
			if (x>=p_bbx2) x=p_bbx2;
		}
   }

skipmelt:
;
	vect_set(&p2, x, y, z);
    mat_mulvect(&p_InvTM, &p2, out);
}



// ***********************************************************
// **********************      TAPER     *********************
// ***********************************************************

void CTaperModifier::m_Init(void)
{
   if (p_AmountTrack!=(CRollTrack *)NULL)
	   p_AmountTrack->m_Init(1);

   if (p_CurveTrack!=(CRollTrack *)NULL)
	   p_CurveTrack->m_Init(1);

   if (p_UpLimTrack!=(CRollTrack *)NULL)
	   p_UpLimTrack->m_Init(1);

   if (p_LowLimTrack!=(CRollTrack *)NULL)
	   p_LowLimTrack->m_Init(1);

   CObjectSpaceModifier::m_Init();

   if (p_Axis==2) p_Axis=1;
   else if (p_Axis==1) p_Axis=2;
}


void CTaperModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
   AD_Vect3D sub_p_Center;
   AD_Matrix mrot, imrot, pret, ipret, maux, M, iM;

   CObjectSpaceModifier::m_Update(framepos, pivot);

   if (p_AmountTrack!=(CRollTrack *)NULL)
	   p_AmountTrack->m_GetData(framepos, &p_Amount);

   if (p_CurveTrack!=(CRollTrack *)NULL)
	   p_CurveTrack->m_GetData(framepos, &p_Curve);

   if (p_UpLimTrack!=(CRollTrack *)NULL)
	   p_UpLimTrack->m_GetData(framepos, &p_UpLim);

   if (p_LowLimTrack!=(CRollTrack *)NULL)
	   p_LowLimTrack->m_GetData(framepos, &p_LowLim);

   // costruzione matrice di trasformazione e la sua
   // inversa
   sub_p_Center.x=-p_Center.x;
   sub_p_Center.y=-p_Center.y;
   sub_p_Center.z=-p_Center.z;

   switch (p_Axis)
   {
     case 0:  // asse X
             mat_setmatrix_of_eulerrotationZ(&mrot, (float4)(-Pi__/2.0));
			 l=p_bbx2-p_bbx1;
		     break;
     case 1:  // asse Y
             mat_identity(&mrot);
			 l=p_bby2-p_bby1;
		     break;
     case 2:  // asse Z
             mat_setmatrix_of_eulerrotationX(&mrot, (float4)(-Pi__/2.0));
			 l=p_bbz2-p_bbz1;
		     break;
   }

   mat_transpose(&mrot, &imrot);
   mat_setmatrix_of_pretraslation(&pret, &sub_p_Center);
   mat_setmatrix_of_pretraslation(&ipret, &p_Center);

   // costruzione matrice di offset
   mat_mul (&mrot, &pret, &maux);
   mat_mul (&ipret, &maux, &M);
   mat_mul (&imrot, &pret, &maux);
   mat_mul (&ipret, &maux, &iM);

   mat_mul(&p_InverseGizmoMatrix, &M, &p_TM);
   mat_mul(&iM, &p_GizmoMatrix, &p_InvTM);

   switch (p_EffectAxis)
   {
     case 0: p_DoX = 1;  p_DoY = 0; break;
     case 1: p_DoX = 0;  p_DoY = 1;  break;
     case 2: p_DoX = 1;  p_DoY = 1;  break;
   }
}


void CTaperModifier::m_Map(AD_Vect3D *pos, AD_Vect3D *out)
{
	float4 f, z;
	AD_Vect3D p;

	if (l == 0)
	{
       vect_copy(pos, out);
       return;
	}
	mat_mulvect(&p_TM, pos, &p);

	if (p_DoLim)
	{
		if (p.y<p_LowLim)
			z = p_LowLim/l;
		else 
		if (p.y>p_UpLim)
			z = p_UpLim/l;
		else
			z = p.y/l;
	}
	else
		z = p.y/l;

	if (p_Symmetry && z<0.0f) z = -z;	
	f =  1 + z*p_Amount + p_Curve*z*(1-z);	
  	if (p_DoX) p.x *= f;
  	if (p_DoY) p.z *= f;

	mat_mulvect(&p_InvTM, &p, out);
}



// ***********************************************************
// **********************      NOISE     *********************
// ***********************************************************

void CNoiseModifier::m_Init(void)
{
   if (p_StrengthTrack!=(CPosTrack *)NULL)
       p_StrengthTrack->m_Init(1);

   if (p_ScaleTrack!=(CRollTrack *)NULL)
	   p_ScaleTrack->m_Init(1);
   if (p_Scale<0) p_Scale=0.00001f;
   else p_Scale=1.0f/p_Scale;

   if (p_FreqTrack!=(CRollTrack *)NULL)
	   p_FreqTrack->m_Init(1);
   if (p_Freq<0) p_Freq=0;

   if (p_PhaseTrack!=(CRollTrack *)NULL)
	   p_PhaseTrack->m_Init(1);

   p_Rough=1-p_Rough;

   CObjectSpaceModifier::m_Init();
}


void CNoiseModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
   float4 fp_Phase;

   CObjectSpaceModifier::m_Update(framepos, pivot);

   if (p_ScaleTrack!=(CRollTrack *)NULL)
   {
	   p_ScaleTrack->m_GetData(framepos, &p_Scale);
	   if (p_Scale<0) p_Scale=0.00001f;
	   else p_Scale=1.0f/p_Scale;
   }

   if (p_StrengthTrack!=(CPosTrack *)NULL)
	   p_StrengthTrack->m_GetData(framepos, &p_Strength);

   if (p_PhaseTrack!=(CRollTrack *)NULL)
   {
	   p_PhaseTrack->m_GetData(framepos, &fp_Phase);
	   p_Phase=(int32)fp_Phase;
   }
   else fp_Phase=(float4)p_Phase;

   if (p_FreqTrack!=(CRollTrack *)NULL)
   {
	   p_FreqTrack->m_GetData(framepos, &p_Freq);
	   if (p_Freq<0) p_Freq=0;
   }

   if (p_Animate)
   {
/*      p_Time = fp_Phase * 0.005f * p_Freq *
			1200.0f/160.0f; // + Perm(seed);*/
      p_Time = fp_Phase * 0.004f * p_Freq;
   }
   else	p_Time = 0.5f; //(float4)Perm(seed);

   mat_copy(&p_InverseGizmoMatrix, &p_TM);
   mat_copy(&p_GizmoMatrix, &p_InvTM);
}


void CNoiseModifier::m_Map(AD_Vect3D *pos, AD_Vect3D *out)
{
   AD_Vect3D d, sp, p, p2;

   mat_mulvect(&p_TM, pos, &p);

   sp.x=(0.5f + p.x*p_Scale);
   sp.y=(0.5f + p.y*p_Scale);
   sp.z=(0.5f + p.z*p_Scale);

   if (p_Fractal)
   {
      /*
      d.x = fBm1(Point3(sp.y,sp.z,time),p_Rough,2.0f,iterations);
      d.y = fBm1(Point3(sp.x,sp.z,time),p_Rough,2.0f,iterations);
      d.z = fBm1(Point3(sp.x,sp.y,time),p_Rough,2.0f,iterations);
	  */
      vect_copy(pos, out);
	  return;
   }
   else
   {
	  // la noise3 è dentro fastmath.h/cpp
      d.x = noise3(sp.z, p_Time, sp.y);
      d.z = noise3(sp.x, p_Time, sp.y);
      d.y = noise3(sp.x, p_Time, sp.z);
   }

   p2.x = (p.x + d.x*p_Strength.x);
   p2.y = (p.y + d.y*p_Strength.y);
   p2.z = (p.z + d.z*p_Strength.z);

   mat_mulvect(&p_InvTM, &p2, out);
}



// ***********************************************************
// **********************     STRETCH    *********************
// ***********************************************************

void CStretchModifier::m_Init(void)
{
   if (p_StretchTrack!=(CRollTrack *)NULL)
	   p_StretchTrack->m_Init(1);

   if (p_AmplifyTrack!=(CRollTrack *)NULL)
	   p_AmplifyTrack->m_Init(1);

   float4 amplify_aux = (p_Amplify >= 0) ? p_Amplify + 1 : 1.0f / (-p_Amplify + 1.0f);
   p_Amplify=amplify_aux;

   if (p_UpLimTrack!=(CRollTrack *)NULL)
	   p_UpLimTrack->m_Init(1);

   if (p_LowLimTrack!=(CRollTrack *)NULL)
	   p_LowLimTrack->m_Init(1);

   CObjectSpaceModifier::m_Init();

   if (p_Axis==2) p_Axis=1;
   else if (p_Axis==1) p_Axis=2;
}


void CStretchModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
   AD_Vect3D sub_p_Center;
   AD_Matrix mrot, pret, ipret, imrot, maux, M, iM;
   float4 amplify_aux;

   CObjectSpaceModifier::m_Update(framepos, pivot);

   if (p_StretchTrack!=(CRollTrack *)NULL)
	   p_StretchTrack->m_GetData(framepos, &p_Stretch);

   if (p_AmplifyTrack!=(CRollTrack *)NULL)
   {
	   p_AmplifyTrack->m_GetData(framepos, &p_Amplify);
       amplify_aux = (p_Amplify >= 0) ? p_Amplify + 1 : 1.0f / (-p_Amplify + 1.0f);
       p_Amplify=amplify_aux;
   }

   if (p_UpLimTrack!=(CRollTrack *)NULL)
	   p_UpLimTrack->m_GetData(framepos, &p_UpLim);

   if (p_LowLimTrack!=(CRollTrack *)NULL)
	   p_LowLimTrack->m_GetData(framepos, &p_LowLim);

   // costruzione matrice di trasformazione e la sua
   // inversa
   sub_p_Center.x=-p_Center.x;
   sub_p_Center.y=-p_Center.y;
   sub_p_Center.z=-p_Center.z;

   switch (p_Axis)
   {
     case 0:  // asse X
             mat_setmatrix_of_eulerrotationZ(&mrot, (float4)(-Pi__/2.0));
		     break;
     case 1:  // asse Y
             mat_identity(&mrot);
		     break;
     case 2:  // asse Z
             mat_setmatrix_of_eulerrotationX(&mrot, (float4)(Pi__/2.0));
		     break;
   }

   mat_transpose(&mrot, &imrot);
   mat_setmatrix_of_pretraslation(&pret, &sub_p_Center);
   mat_setmatrix_of_pretraslation(&ipret, &p_Center);

   // costruzione matrice di offset
   mat_mul (&mrot, &pret, &maux);
   mat_mul (&ipret, &maux, &M);
   mat_mul (&imrot, &pret, &maux);
   mat_mul (&ipret, &maux, &iM);

   mat_mul(&p_InverseGizmoMatrix, &M, &p_TM);
   mat_mul(&iM, &p_GizmoMatrix, &p_InvTM);

   if (!p_DoLim)
   {
      switch ( p_Axis )
	  {
        case 0:
            p_HeightMin = p_bbx1;
            p_HeightMax = p_bbx2;
            break;
        case 1:
            p_HeightMin = p_bby1;
            p_HeightMax = p_bby2;
            break;
        case 2:
            p_HeightMin = p_bbz1;
            p_HeightMax = p_bbz2;
            break;
        }
    } else {
        p_HeightMin = p_LowLim;
        p_HeightMax = p_UpLim;
    }
}


void CStretchModifier::m_Map(AD_Vect3D *pos, AD_Vect3D *out)
{
    float4 fraction, normHeight;
    float4 xyScale, zScale, a, b, c;
	AD_Vect3D p;
    
    if (p_Stretch == 0 || (p_HeightMax - p_HeightMin == 0))
	{
        vect_copy(pos, out);
		return;
	}
    
    mat_mulvect(&p_TM, pos, &p);
    
    if (p_DoLim && p.y > p_UpLim)
        normHeight = (p_UpLim - p_HeightMin) / (p_HeightMax - p_HeightMin);
    else if (p_DoLim && p.y < p_LowLim)
        normHeight = (p_LowLim - p_HeightMin) / (p_HeightMax - p_HeightMin);
    else
        normHeight = (p.y - p_HeightMin) / (p_HeightMax - p_HeightMin);

    if (p_Stretch < 0) {   // Squash
        xyScale = (p_Amplify * -p_Stretch + 1.0f);
        zScale = (-1.0f / (p_Stretch - 1.0f));
    } else {           // Stretch
        xyScale = 1.0f / (p_Amplify * p_Stretch + 1.0f);
        zScale = p_Stretch + 1.0f;
    }

    // a, b, and c are the coefficients of the quadratic function f(x)
    // such that f(0) = 1, f(1) = 1, and f(0.5) = xyScale
    a = 4.0f * (1.0f - xyScale);
    b = -4.0f * (1.0f - xyScale);
    c = 1.0f;
    fraction = (((a * normHeight) + b) * normHeight) + c;
    p.x *= fraction;
    p.z *= fraction;

    if (p_DoLim && p.y < p_LowLim)
	  p.y += (zScale - 1.0F) * p_LowLim;
    else if (p_DoLim && p.y <= p_UpLim)
	       p.y *= zScale;
    else if (p_DoLim && p.y > p_UpLim)
	       p.y += (zScale - 1.0f) * p_UpLim;
    else
	p.y *= zScale;

    mat_mulvect(&p_InvTM, &p, out);
}


// ***********************************************************
// ************************     FFD    ***********************
// ***********************************************************

void CFFDModifier::m_Init(void)
{
  int32 i;

  for (i=0; i<p_NumControlPoints; i++) p_ControlPointsTracks[i].m_Init(1);
  CObjectSpaceModifier::m_Init();
}


void CFFDModifier::m_Update(float4 framepos, AD_Vect3D *pivot)
{
  int32 w;
  AD_Vect3D stmp;
  AD_Matrix posttrans, scaling, maux;
  AD_Matrix inv_posttrans, inv_scaling;
  AD_Vect3D size;

  CObjectSpaceModifier::m_Update(framepos, pivot);

  // Costruzione della size: servirà per portare nello spazio
  // normalizzato (0..1) del lattice il punto da mappare
  size.x=p_bbx2-p_bbx1; if (size.x==0) size.x=0.001f;
  size.y=p_bby2-p_bby1; if (size.y==0) size.y=0.001f;
  size.z=p_bbz2-p_bbz1; if (size.z==0) size.z=0.001f;
  mat_setmatrix_of_scaling(&scaling, size.x, size.y, size.z);
  mat_setmatrix_of_scaling(&inv_scaling, 1.0f/size.x, 1.0f/size.y, 1.0f/size.z);
  // L'origine della TM è l'angolo in basso a sx della bounding
  // box, non il centro.
  stmp.x=p_Center.x-p_bbx1;
  stmp.y=p_Center.y-p_bby1;
  stmp.z=p_Center.z-p_bbz1;
  mat_setmatrix_of_pretraslation(&posttrans, &stmp);
  stmp.x=-stmp.x;
  stmp.y=-stmp.y;
  stmp.z=-stmp.z;
  mat_setmatrix_of_pretraslation(&inv_posttrans, &stmp);

  // Costruzione di tm, e p_InvTM
  mat_mul(&inv_scaling, &posttrans, &maux);
  mat_mul(&maux, &p_InverseGizmoMatrix, &p_TM);

  mat_mul(&inv_posttrans, &scaling, &maux);
  mat_mul(&p_GizmoMatrix, &maux, &p_InvTM);

  for (w=0; w<p_NumControlPoints; w++)
	if (p_ControlPointsTracks[w].p_NumKeys>0) p_ControlPointsTracks[w].m_GetData(framepos, &p_ControlPoints[w]);
}


#define EPSILON	0.001f
#define GRIDINDEX44(i,j,k) (((i)<<4) + ((j)<<2) + (k))
#define GRIDINDEX33(i,j,k) (((i)*9) + ((j)*3) + (k))
#define GRIDINDEX22(i,j,k) (((i)<<2) + ((j)<<1) + (k))
inline float4 BPoly4(int32 i, float4 u)
{
	float4 s = 1.0f-u;
	switch (i) 
	{
		case 0: return s*s*s;
		case 1: return 3.0f*u*s*s;
		case 2: return 3.0f*u*u*s;
		case 3: return u*u*u;
		default: return 0.0f;
	}
}

inline float4 BPoly3(int32 i, float4 u)
{
	float4 s = 1.0f-u;
	switch (i)
	{
		case 0: return s*s;
		case 1: return 2.0f*u*s;
		case 2: return u*u;
		default: return 0.0f;
	}
}

inline float4 BPoly2(int32 i, float4 u)
{	
	switch (i) 
	{
		case 0: return 1.0f-u;
		case 1: return u;
		default: return 0.0f;
	}
}


void CFFDModifier::m_Map(AD_Vect3D *pos, AD_Vect3D *out)
{
	AD_Vect3D q, pp, v;
	float4 kw;
	
    if (p_Dim1==3)
	{
	   m_Map3(pos, out);
	   return;
	}
	else
	if (p_Dim1==2)
	{
	   m_Map2(pos, out);
	   return;
	}

    // Map4 inizia
	mat_mulvect(&p_TM, pos, &pp);
	if (p_Invol)
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
	// Compute the deformed point32 as a weighted average of all
	// 64 control points.
	for (int32 i=0; i<p_Dim1; i++)
	{
		for (int32 j=0; j<p_Dim2; j++)
		{
			for (int32 k=0; k<p_Dim3; k++)
			{
				v=p_ControlPoints[GRIDINDEX44(i,j,k)];
				kw=BPoly4(i, pp.x)*BPoly4(j, pp.z)*BPoly4(k, pp.y);
				v.x=v.x*kw;
				v.y=v.y*kw;
				v.z=v.z*kw;
				vect_auto_add(&q, &v);
			}
		}
	}
	
	mat_mulvect(&p_InvTM, &q, out);
}


void CFFDModifier::m_Map3(AD_Vect3D *pos, AD_Vect3D *out)
{
	AD_Vect3D q, pp, v;
	float4 kw;
	
	mat_mulvect(&p_TM, pos, &pp);

	if (p_Invol)
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
	// Compute the deformed point32 as a weighted average of all
	// 64 control points.
	for (int32 i=0; i<p_Dim1; i++)
	{
		for (int32 j=0; j<p_Dim2; j++)
		{
			for (int32 k=0; k<p_Dim3; k++)
			{
				v=p_ControlPoints[GRIDINDEX33(i,j,k)];
				kw=BPoly3(i, pp.x)*BPoly3(j, pp.z)*BPoly3(k, pp.y);
				v.x=v.x*kw;
				v.y=v.y*kw;
				v.z=v.z*kw;
				vect_auto_add(&q, &v);
			}
		}
	}
	
	mat_mulvect(&p_InvTM, &q, out);
}


void CFFDModifier::m_Map2(AD_Vect3D *pos, AD_Vect3D *out)
{
	AD_Vect3D q, pp, v;
	float4 kw;
	
	mat_mulvect(&p_TM, pos, &pp);

	if (p_Invol)
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
	// Compute the deformed point32 as a weighted average of all
	// 64 control points.
	for (int32 i=0; i<p_Dim1; i++)
	{
		for (int32 j=0; j<p_Dim2; j++)
		{
			for (int32 k=0; k<p_Dim3; k++)
			{
				v=p_ControlPoints[GRIDINDEX22(i,j,k)];
				kw=BPoly2(i, pp.x)*BPoly2(j, pp.z)*BPoly2(k, pp.y);
				v.x=v.x*kw;
				v.y=v.y*kw;
				v.z=v.z*kw;
				vect_auto_add(&q, &v);
			}
		}
	}
	
	mat_mulvect(&p_InvTM, &q, out);
}