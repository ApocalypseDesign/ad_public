// Metodi per le classi definite in KEYFRAME.H
// By [Turbo]

#include <math.h>
#include "keyframe.h"
#include "..\Math\fastmath.h"

extern HWND miawin;         // finestra principale; usata qui per le debug_error

float4 spline_ease (float4 t, float4 easefrom, float4 easeto)
// spline ease per rimappare i parametri tra le key. Codice preso
// spudoratamente da CLAX, causa assenza totale di info al riguardo
{
  float4 k, s;

  s=easeto+easefrom;
  if (s==0) return(t);

  if (s>1.0)
  {
	s=1.0f/s;
    easeto=easeto*s;
    easefrom=easefrom*s;
  }

  k=(float4)(1.0/(2.0-easeto-easefrom));
  if (t<easefrom) return((k/easefrom)*t*t);
  else
  {
    if (t<(1.0-easeto)) return((float4)(k*(2.0*t-easefrom)));
    else
    {
      t=1.0f-t;
      return(1.0f-(k/easeto)*t*t);
    }
  }
}


int32 CKeyTrack::m_NumCycles(int32 start, int32 end, float4 t)
{
  int32 int_t, k;

  if (t<start)
  {
     int_t=(int32)(fceil(t));
	 k=start-int_t;
	 return(k / (end-start));
  }
  else
  {
     int_t=(int32)(t);
	 k=int_t-start;
	 return(k / (end-start));
  }
}


float4 CKeyTrack::m_CycleTimeFloat(int32 start, int32 end, float4 t)
{

  float4 fstart, fend, fract, k1, k2, k3;
  int32 int_t, h, i1, i2, i3;

  fstart=(float4)start;
  fend=(float4)end;

  if (t<fstart)
  {
	k3=fceil(t);
	i1=(int32)(k3);
    i2=start-i1;
	i3=i2 / (end-start);
	k1=i2-(fend-fstart)*(float4)i3;
	k2=k1+(k3-t);
	return(fend-k2);
  }

  int_t=(int32)(t);
  fract=t-(float4)(int_t);
  h=(int_t-start) % (end-start);
  h=h+start;
  return(fract+(float4)h);
}


int32 CKeyTrack::m_CycleTimeInt(int32 start, int32 end, int32 t)
{
  return(1);
}

// ###########################################################################
// #                              HIDING                             #
// ###########################################################################

CHideTrack::CHideTrack()
{
  p_Keys=(int32 *)NULL;
  p_InitialState=1; // stato iniziale = visibile
}

//--------------------------------------------------------------------

int32 CHideTrack::m_Init(float4 scale)
{ 
  m_SortKeys();
  return(1);
}

//--------------------------------------------------------------------

void CHideTrack::m_GetTimeRange(float4 *timeStart, float4 *timeEnd)
{
  if ((!timeStart) || (!timeEnd)) return;

  if (p_NumKeys<=0)
  {
	 *timeStart=0;
	 *timeEnd=0;
  }
  else
  {
	 *timeStart=(float4)p_Keys[0];
	 *timeEnd=(float4)p_Keys[p_NumKeys-1];
  }
}

//--------------------------------------------------------------------

void CHideTrack::m_SortKeys(void)
{
   int32 i, j;
   HIDEKEY swap;

   for (i=0; i<p_NumKeys-1; i++)
   {
	 for (j=i+1; j<p_NumKeys; j++)
	 {
		 if (p_Keys[j] < p_Keys[i])
		 {
            swap=p_Keys[i];
			p_Keys[i]=p_Keys[j];
			p_Keys[j]=swap;
		 }
	 }
   }
}

//--------------------------------------------------------------------

void CHideTrack::m_GetData(float4 framepos, int32 *out)
{
  int32 intpos, i, state;

  if ((framepos<0.0) || (p_NumKeys<1)) return;
  if (p_Keys==NULL) return;

  intpos=myfist(fceil(framepos));

  i=0;
  while ((i<p_NumKeys) && (p_Keys[i]<intpos))
  {
    i++;
  }

  state=(i & 1);
  // se state=0 allora ho p_InitialState
  // se state=1 allora ho neg(p_InitialState)
  if (state==0) *out=p_InitialState;
  else *out=1-p_InitialState;
}



// ###########################################################################
// #                               POSIZIONI                                 #
// ###########################################################################

CPosTrack::CPosTrack()
{
  p_Keys=NULL;
  p_NumKeys=-1;
}

//--------------------------------------------------------------------

void CPosTrack::m_GetTimeRange(float4 *timeStart, float4 *timeEnd)
{
  if ((!timeStart) || (!timeEnd)) return;

  if (p_NumKeys<=0)
  {
	 *timeStart=0;
	 *timeEnd=0;
  }
  else
  {
	 *timeStart=(float4)p_Keys[0].posintrack;
	 *timeEnd=(float4)p_Keys[p_NumKeys-1].posintrack;
  }
}

//--------------------------------------------------------------------

void CPosTrack::m_SortKeys(void)
{
   int32 i, j;
   POSKEY swap;

   for (i=0; i<p_NumKeys-1; i++)
   {
	 for (j=i+1; j<p_NumKeys; j++)
	 {
		 if (p_Keys[j].posintrack < p_Keys[i].posintrack)
		 {
            swap=p_Keys[i];
			p_Keys[i]=p_Keys[j];
			p_Keys[j]=swap;
		 }
	 }
   }
}

//--------------------------------------------------------------------

int32 CPosTrack::m_Init(float4 scale)
{
  int32 j, i2, i1, i0;
  float4 c1, c2;
  float4 csi, cso;
  AD_Vect3D k, o;

  if (p_NumKeys<1) return(0);
  m_SortKeys();

  for (j=0; j<p_NumKeys; j++)
  {
	  vect_auto_scale(&p_Keys[j].p, scale);
      vect_auto_scale(&p_Keys[j].incomtg, scale);
      vect_auto_scale(&p_Keys[j].outcomtg, scale);
  }

  if (p_NumKeys==1) return(1);
  if (p_Type & BEZIER_TRACK) return(1);
  if (p_NumKeys==2)
  {
    c1=1.0f-p_Keys[0].tension;
	vect_sub(&p_Keys[1].p, &p_Keys[0].p, &k);
	vect_scale(&k, c1, &p_Keys[0].outcomtg);

    c2=1.0f-p_Keys[1].tension;
	vect_sub(&p_Keys[1].p, &p_Keys[0].p, &k);
	vect_scale(&k, c2, &p_Keys[1].incomtg);
	return(1);
  }

  // caso di 3 o piu' key  
  for (j=0; j<p_NumKeys; j++)
  {
    // caso della prima key
    if (j==0)
    {
      i2=j+2;
      i1=j+1;
      i0=j;
	  csi=(float4)p_Keys[i2].posintrack-p_Keys[i0].posintrack;
	  cso=(float4)p_Keys[i1].posintrack-p_Keys[i0].posintrack;
      vect_sub(&p_Keys[i2].p, &p_Keys[i0].p, &k);
      vect_sub(&p_Keys[i1].p, &p_Keys[i0].p, &o);
	  vect_scale(&k, -cso/(2.0f*csi), &k);
	  vect_scale(&o, 3.0f/2.0f, &o);
	  vect_add(&k, &o, &p_Keys[i0].outcomtg);
	  vect_scale(&p_Keys[i0].outcomtg, (1.0f-p_Keys[i0].tension), &p_Keys[i0].outcomtg);
    }
    else
    {
	  // caso dell'ultima key
	  if (j==p_NumKeys-1)
	  {
	    i2=j-2;
	    i1=j-1;
	    i0=j;
		csi=(float4)p_Keys[i0].posintrack-p_Keys[i2].posintrack;
	    cso=(float4)p_Keys[i0].posintrack-p_Keys[i1].posintrack;
        vect_sub(&p_Keys[i0].p, &p_Keys[i2].p, &k);
        vect_sub(&p_Keys[i0].p, &p_Keys[i1].p, &o);
		vect_scale(&k, -cso/(2.0f*csi), &k);
	    vect_scale(&o, 3.0f/2.0f, &o);
	    vect_add(&k, &o, &p_Keys[i0].incomtg);
	    vect_scale(&p_Keys[i0].incomtg, (1.0f-p_Keys[i0].tension), &p_Keys[i0].incomtg);
	  }
	  else
	  // caso di una key intermezza
	  {
	    i2=j+1;
	    i1=j;
	    i0=j-1;
		cso=2.0f*(p_Keys[j+1].posintrack-p_Keys[j].posintrack)/(p_Keys[j+1].posintrack-p_Keys[j-1].posintrack);
		csi=2.0f*(p_Keys[j].posintrack-p_Keys[j-1].posintrack)/(p_Keys[j+1].posintrack-p_Keys[j-1].posintrack);

		// CALCOLO TANGENTE OUTCOMING
        c1=cso*(1.0f-p_Keys[j].tension)*
               (1.0f-p_Keys[j].continuity)*
               (1.0f-p_Keys[j].bias)/2.0f;

        c2=cso*(1.0f-p_Keys[j].tension)*
               (1.0f+p_Keys[j].continuity)*
               (1.0f+p_Keys[j].bias)/2.0f;

        vect_sub(&p_Keys[i2].p, &p_Keys[i1].p, &k);
        vect_sub(&p_Keys[i1].p, &p_Keys[i0].p, &o);
	    vect_scale(&k, c1, &k);
        vect_scale(&o, c2, &o);
	    vect_add(&k, &o, &p_Keys[j].outcomtg);

	    // CALCOLO TANGENTE INCOMING
        c1=csi*(1.0f-p_Keys[j].tension)*
               (1.0f+p_Keys[j].continuity)*
               (1.0f-p_Keys[j].bias)/2.0f;

        c2=csi*(1.0f-p_Keys[j].tension)*
               (1.0f-p_Keys[j].continuity)*
               (1.0f+p_Keys[j].bias)/2.0f;

	    vect_sub(&p_Keys[i2].p, &p_Keys[i1].p, &k);
        vect_sub(&p_Keys[i1].p, &p_Keys[i0].p, &o);
	    vect_scale(&k, c1, &k);
        vect_scale(&o, c2, &o);
	    vect_add(&k, &o, &p_Keys[j].incomtg);
      }
	}
  } // end ciclo for
  return(1);
}

//--------------------------------------------------------------------

void CPosTrack::m_GetData(float4 framepos, AD_Vect3D *out)
{
  float4 h1, h2, h3, h4, t, t2, t3;
  int32 i, cycles;

  if ((framepos<0.0) || (p_NumKeys<1)) return;
  if (p_Keys==NULL) return;

  if (p_NumKeys==1)
  {
	vect_copy(&p_Keys[0].p, out);
	return;
  }


  if (framepos<p_Keys[0].posintrack)
  {
	 switch (p_ORTBefore)
	 {
       case OOR_HOLD: vect_copy(&p_Keys[0].p, out);
                      return;
                      break;

       case OOR_CYCLE:
       case OOR_LOOP: framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack, framepos);
                      break;

       case OOR_PINGPONG:
                      cycles=m_NumCycles(p_Keys[0].posintrack,
                                         p_Keys[p_NumKeys-1].posintrack,
								         framepos);
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack,
												framepos);
					  if ((cycles+1) & 1)
						  framepos=p_Keys[p_NumKeys-1].posintrack-(framepos-p_Keys[0].posintrack);
                      break;

       /*case OOR_LINEAR:
                        break;
       case OOR_RELATIVE_REPEAT:
                                 break;*/
	 }
  }
  else
  if (framepos>p_Keys[p_NumKeys-1].posintrack)
  {
	 switch (p_ORTAfter)
	 {
       case OOR_HOLD: vect_copy(&p_Keys[p_NumKeys-1].p, out);
                      return;
                      break;

       case OOR_CYCLE:
       case OOR_LOOP:
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						               p_Keys[p_NumKeys-1].posintrack, framepos);
                      break;

       case OOR_PINGPONG:
                      cycles=m_NumCycles(p_Keys[0].posintrack,
                                         p_Keys[p_NumKeys-1].posintrack,
								         framepos);
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack,
												framepos);
					  if (cycles & 1)
						  framepos=p_Keys[p_NumKeys-1].posintrack-(framepos-p_Keys[0].posintrack);
                      break;

       /*case OOR_LINEAR:
                        break;
       case OOR_RELATIVE_REPEAT:
                                 break;*/
	 }
  }

  i=0;
  //while ((i<p_NumKeys) && (p_Keys[i].posintrack<intpos))
  while ((i<p_NumKeys) && ((float4)p_Keys[i].posintrack<framepos))
    i++;
  if (i==0) i=1;  // caso della key in posizione zero

  // calcolo il valore del parametro locale tra le key [i-1, i]
  t=(framepos-p_Keys[i-1].posintrack)/(1.0f*p_Keys[i].posintrack-p_Keys[i-1].posintrack);
  t=spline_ease(t, p_Keys[i-1].easefrom, p_Keys[i].easeto);
  t2=t*t;
  t3=t2*t;

  // caloclo delle funzioni base
  h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
  h2=-2.0f*t3 + 3.0f*t2;
  h3=t3 - 2.0f*t2 + t;
  h4=t3 - t2;

  // Interpolazione Hermite
  out->x=(h1*p_Keys[i-1].p.x + h2*p_Keys[i].p.x +
          h3*p_Keys[i-1].outcomtg.x + h4*p_Keys[i].incomtg.x);
  out->y=(h1*p_Keys[i-1].p.y + h2*p_Keys[i].p.y +
          h3*p_Keys[i-1].outcomtg.y + h4*p_Keys[i].incomtg.y);
  out->z=(h1*p_Keys[i-1].p.z + h2*p_Keys[i].p.z +
          h3*p_Keys[i-1].outcomtg.z + h4*p_Keys[i].incomtg.z);
}



// ###########################################################################
// #                               ROTAZIONI                                                                                               #
// ###########################################################################
CRotTrack::CRotTrack()
{
  p_Keys=NULL;
  p_Type=RELATIVE_TRACK;
  p_NumKeys=-1;
}

//--------------------------------------------------------------------

void CRotTrack::m_GetTimeRange(float4 *timeStart, float4 *timeEnd)
{
  if ((!timeStart) || (!timeEnd)) return;

  if (p_NumKeys<=0)
  {
	 *timeStart=0;
	 *timeEnd=0;
  }
  else
  {
	 *timeStart=(float4)p_Keys[0].posintrack;
	 *timeEnd=(float4)p_Keys[p_NumKeys-1].posintrack;
  }
}

//--------------------------------------------------------------------

void CRotTrack::m_SortKeys(void)
{
   int32 i, j;
   ROTKEY swap;

   for (i=0; i<p_NumKeys-1; i++)
   {
	 for (j=i+1; j<p_NumKeys; j++)
	 {
		 if (p_Keys[j].posintrack < p_Keys[i].posintrack)
		 {
            swap=p_Keys[i];
			p_Keys[i]=p_Keys[j];
			p_Keys[j]=swap;
		 }
	 }
   }
}

//--------------------------------------------------------------------

int32 CRotTrack::m_Init(float4 scale)
{
  int32 j;
  AD_Quaternion qaux, qold, Tn0, Tn1, qinv1, qinv2;
  AD_Quaternion qaux1, qaux2;
  AD_Quaternion qaux11, log1, log2, qprev, qnext;
  float s1, s2, cso, csi;

  if (p_NumKeys<1) return(0);
  m_SortKeys();

  // sistemazione a quaternioni unitari assoluti (non riferiti
  // al quat precedente)
  if (p_NumKeys>0)
  for (j=0; j<p_NumKeys; j++)
  {
    // setta un AD_Quaternione corrispondente alla key
    quat_copy(&p_Keys[j].rotquat, &qaux);
	qaux.w*=scale;

    // trasformalo in un AD_Quaternion di rotazione
    quat_quat_to_rotquat(&qaux);
	if (p_Type & RELATIVE_TRACK)
	{
      if (j==0) quat_copy(&qaux, &qold);
      else quat_mul(&qaux, &qold, &qold);
	}
	else quat_copy(&qaux, &qold);
	quat_copy(&qold, &p_Keys[j].rotquat);
  }
  if (p_NumKeys==1) return(1);

  // calcolo delle tangenti incoming e outcoming da usare poi
  // nell'interpolazione di Beziér alla TCB

  // caso di due sole key, si ottiene uno slerp come deve essere
  quat_copy(&p_Keys[0].rotquat, &p_Keys[0].outcomtg);
  quat_copy(&p_Keys[p_NumKeys-1].rotquat, &p_Keys[p_NumKeys-1].incomtg);
  if (p_NumKeys==2) return(1);

  for (j=1; j<p_NumKeys-1; j++)
  {
    quat_copy (&p_Keys[j-1].rotquat, &qprev);
    quat_copy (&p_Keys[j+1].rotquat, &qnext);

	cso=2.0f*(p_Keys[j+1].posintrack-p_Keys[j].posintrack)/(p_Keys[j+1].posintrack-p_Keys[j-1].posintrack);
	csi=2.0f*(p_Keys[j].posintrack-p_Keys[j-1].posintrack)/(p_Keys[j+1].posintrack-p_Keys[j-1].posintrack);

	quat_inverse(&p_Keys[j].rotquat, &qinv1);
	quat_inverse(&qprev, &qinv2);
	quat_mul(&qinv1, &qnext, &qaux1);
	quat_mul(&qinv2, &p_Keys[j].rotquat, &qaux2);
    quat_log(&qaux1, &log1);
    quat_log(&qaux2, &log2);


	// calcolo tangente outcoming
	s1=cso*0.5f*(1.0f-p_Keys[j].tension)*(1.0f-p_Keys[j].continuity)*(1.0f-p_Keys[j].bias);
	s2=cso*0.5f*(1.0f-p_Keys[j].tension)*(1.0f+p_Keys[j].continuity)*(1.0f+p_Keys[j].bias);
	quat_scale(&log1, s1, &qaux1);
	quat_scale(&log2, s2, &qaux2);
	quat_add(&qaux1, &qaux2, &Tn0);
	quat_sub(&Tn0, &log1, &qaux);
	quat_scale(&qaux, 0.5f, &qaux);
	quat_exp(&qaux, &qaux11);
	quat_mul(&p_Keys[j].rotquat, &qaux11, &p_Keys[j].outcomtg);


	// calcolo tangente incoming
	s1=csi*0.5f*(1.0f-p_Keys[j].tension)*(1.0f+p_Keys[j].continuity)*(1.0f-p_Keys[j].bias);
	s2=csi*0.5f*(1.0f-p_Keys[j].tension)*(1.0f-p_Keys[j].continuity)*(1.0f+p_Keys[j].bias);
	quat_scale(&log1, s1, &qaux1);
	quat_scale(&log2, s2, &qaux2);
	quat_add(&qaux1, &qaux2, &Tn1);
	quat_sub(&log2, &Tn1, &qaux);
	quat_scale(&qaux, 0.5f, &qaux);
	quat_exp(&qaux, &qaux11);
	quat_mul(&p_Keys[j].rotquat, &qaux11, &p_Keys[j].incomtg);
  }
  return(1);
}

//--------------------------------------------------------------------

void CRotTrack::m_GetData(float4 framepos, AD_Quaternion *out)
{
  float4 t, spin, angle;
  int32 i, cycles;
  AD_Quaternion q1, q2;

  if ((framepos<0.0) || (p_NumKeys<1)) return;
  if (p_Keys==NULL) return;
  if (p_NumKeys==1)
  {
    quat_copy(&p_Keys[0].rotquat, out);
	return;
  }

  if (framepos<p_Keys[0].posintrack)
  {
	 switch (p_ORTBefore)
	 {
       case OOR_HOLD: quat_copy(&p_Keys[0].rotquat, out);
                      return;
                      break;

       case OOR_CYCLE:
       case OOR_LOOP: framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack, framepos);
                      break;

       case OOR_PINGPONG:
                      cycles=m_NumCycles(p_Keys[0].posintrack,
                                         p_Keys[p_NumKeys-1].posintrack,
								         framepos);
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack,
												framepos);
					  if ((cycles+1) & 1)
						  framepos=p_Keys[p_NumKeys-1].posintrack-(framepos-p_Keys[0].posintrack);
                      break;
       /*case OOR_LINEAR:
                        break;
       case OOR_RELATIVE_REPEAT:
                                 break;*/
	 }
  }
  else
  if (framepos>p_Keys[p_NumKeys-1].posintrack)
  {
	 switch (p_ORTAfter)
	 {
       case OOR_HOLD: quat_copy(&p_Keys[p_NumKeys-1].rotquat, out);
                      return;
                      break;

       case OOR_CYCLE:
       case OOR_LOOP:
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						               p_Keys[p_NumKeys-1].posintrack, framepos);
                      break;

       case OOR_PINGPONG:
                      cycles=m_NumCycles(p_Keys[0].posintrack,
                                         p_Keys[p_NumKeys-1].posintrack,
								         framepos);
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack,
												framepos);
					  if (cycles & 1)
						  framepos=p_Keys[p_NumKeys-1].posintrack-(framepos-p_Keys[0].posintrack);
                      break;
       /*case OOR_LINEAR:
                        break;
       case OOR_RELATIVE_REPEAT:
                                 break;*/
	 }
  }

  
  i=0;
  while ((i<p_NumKeys) && (p_Keys[i].posintrack<framepos))
    i++;
  
  if (i==0) i=1;  // caso della key in posizione zero
  t=(framepos-p_Keys[i-1].posintrack)/(1.0f*p_Keys[i].posintrack-p_Keys[i-1].posintrack);
  t=spline_ease(t, p_Keys[i-1].easefrom, p_Keys[i].easeto);
  angle=p_Keys[i].roll-p_Keys[i-1].roll;

  if (angle > 0.0) spin = (float4)floor (angle / (2.0*Pi__));
  else spin = (float4)ceil (angle / (2.0*Pi__));

//  slerp normale
//  quat_slerp(&p_Keys[i-1].rotquat, &p_Keys[i].rotquat, t, spin, out);

  // squad
  if (p_Type & RELATIVE_TRACK)
  {
    quat_slerp(&p_Keys[i-1].rotquat, &p_Keys[i].rotquat, t, spin, &q1);
    quat_slerp(&p_Keys[i-1].outcomtg, &p_Keys[i].incomtg, t, spin, &q2);
    quat_slerp(&q1, &q2, 2.0f*t*(1-t), 0, out);
  }
  else
  {
    quat_slerp_flip(&p_Keys[i-1].rotquat, &p_Keys[i].rotquat, t, spin, &q1);
    quat_slerp_flip(&p_Keys[i-1].outcomtg, &p_Keys[i].incomtg, t, spin, &q2);
    quat_slerp_flip(&q1, &q2, 2.0f*t*(1-t), 0, out);
  }
}

// -------------------------------------------------------------------

void CRotTrack::m_GetData(float4 framepos, AD_Matrix *out)
{
  AD_Quaternion q;

  m_GetData(framepos, &q);
  quat_rotquat_to_matrix(&q, out);
}


// ###########################################################################
// #                                ROLLING                                  #
// ###########################################################################

CRollTrack::CRollTrack()
{
  p_Keys=NULL;
  p_NumKeys=-1;
}

//--------------------------------------------------------------------

void CRollTrack::m_GetTimeRange(float4 *timeStart, float4 *timeEnd)
{
  if ((!timeStart) || (!timeEnd)) return;

  if (p_NumKeys<=0)
  {
	 *timeStart=0;
	 *timeEnd=0;
  }
  else
  {
	 *timeStart=(float4)p_Keys[0].posintrack;
	 *timeEnd=(float4)p_Keys[p_NumKeys-1].posintrack;
  }
}

//--------------------------------------------------------------------

void CRollTrack::m_SortKeys(void)
{
   int32 i, j;
   ROLLKEY swap;

   for (i=0; i<p_NumKeys-1; i++)
   {
	 for (j=i+1; j<p_NumKeys; j++)
	 {
		 if (p_Keys[j].posintrack < p_Keys[i].posintrack)
		 {
            swap=p_Keys[i];
			p_Keys[i]=p_Keys[j];
			p_Keys[j]=swap;
		 }
	 }
   }
}

//--------------------------------------------------------------------

int32 CRollTrack::m_Init(float4 scale)
{
  int32 j, i2, i1, i0;
  float4 c1, c2, csi, cso, k, o;

  if (p_NumKeys<1) return(0);
  m_SortKeys();

  for (j=0; j<p_NumKeys; j++)
  {
	  p_Keys[j].roll*=scale;
      p_Keys[j].incomtg*=scale;
      p_Keys[j].outcomtg*=scale;
  }
  if (p_NumKeys==1) return(1);
  if (p_Type & BEZIER_TRACK) return(1);

  if (p_NumKeys==2)
  {
    c1=1.0f-p_Keys[0].tension;
    p_Keys[0].outcomtg=c1*(p_Keys[1].roll-p_Keys[0].roll);
    c2=1.0f-p_Keys[1].tension;
    p_Keys[1].incomtg=c2*(p_Keys[1].roll-p_Keys[0].roll);
	return(1);
  }

  // caso di 3 o piu' key
  for (j=0; j<p_NumKeys; j++)
  {
    // caso della prima key
    if (j==0)
    {
      i2=j+2;
      i1=j+1;
      i0=j;
	  csi=(float4)p_Keys[i2].posintrack-p_Keys[i0].posintrack;
	  cso=(float4)p_Keys[i1].posintrack-p_Keys[i0].posintrack;

	  k=p_Keys[i2].roll - p_Keys[i0].roll;
      o=p_Keys[i1].roll - p_Keys[i0].roll;
	  k*=-cso/(2.0f*csi);
	  o*=3.0f/2.0f;
	  p_Keys[i0].outcomtg=o+k;
	  p_Keys[i0].outcomtg*=(1.0f-p_Keys[i0].tension);
    }
    else
    {
	  // caso dell'ultima key
	  if (j==p_NumKeys-1)
	  {
	    i2=j-2;
	    i1=j-1;
	    i0=j;
		csi=(float4)p_Keys[i0].posintrack-p_Keys[i2].posintrack;
	    cso=(float4)p_Keys[i0].posintrack-p_Keys[i1].posintrack;

	    k=p_Keys[i0].roll - p_Keys[i2].roll;
        o=p_Keys[i0].roll - p_Keys[i1].roll;
	    k*=-cso/(2.0f*csi);
	    o*=3.0f/2.0f;
	    p_Keys[i0].incomtg=o+k;
	    p_Keys[i0].incomtg*=(1.0f-p_Keys[i0].tension);
	  }
	  else
	  // caso di una key intermezza
	  {
	    i2=j+1;
	    i1=j;
	    i0=j-1;
	    csi=2.0f*(p_Keys[j+1].posintrack-p_Keys[j].posintrack)/(p_Keys[j+1].posintrack-p_Keys[j-1].posintrack);
	    cso=2.0f*(p_Keys[j].posintrack-p_Keys[j-1].posintrack)/(p_Keys[j+1].posintrack-p_Keys[j-1].posintrack);

        // CALCOLO TANGENTE OUTCOMING
        c1=cso*(1.0f-p_Keys[j].tension)*
               (1.0f-p_Keys[j].continuity)*
               (1.0f-p_Keys[j].bias)/2.0f;

        c2=cso*(1.0f-p_Keys[j].tension)*
               (1.0f+p_Keys[j].continuity)*
               (1.0f+p_Keys[j].bias)/2.0f;

        p_Keys[j].outcomtg=c1*(p_Keys[i2].roll-p_Keys[i1].roll) +
			                   c2*(p_Keys[i1].roll-p_Keys[i0].roll);


        // CALCOLO TANGENTE INCOMING
        c1=csi*(1.0f-p_Keys[j].tension)*
               (1.0f+p_Keys[j].continuity)*
               (1.0f-p_Keys[j].bias)/2.0f;

        c2=csi*(1.0f-p_Keys[j].tension)*
               (1.0f-p_Keys[j].continuity)*
               (1.0f+p_Keys[j].bias)/2.0f;

        p_Keys[j].incomtg=c1*(p_Keys[i2].roll-p_Keys[i1].roll) +
			                  c2*(p_Keys[i1].roll-p_Keys[i0].roll);
	  }
	}
  } // end ciclo for
  return(1);
}

//--------------------------------------------------------------------

void CRollTrack::m_GetData(float4 framepos, float4 *out)
{
  float4 h1, h2, h3, h4, t, t2, t3;
  int32 i, cycles;

  if ((framepos<0.0) || (p_NumKeys<1)) return;
  if (p_Keys==NULL) return;
  if (p_NumKeys==1)
  {
	*out=p_Keys[0].roll;
	return;
  }

  if (framepos<p_Keys[0].posintrack)
  {
	 switch (p_ORTBefore)
	 {
       case OOR_HOLD: *out=p_Keys[0].roll;
                      return;
                      break;

       case OOR_CYCLE:
       case OOR_LOOP: framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack, framepos);
                      break;

       case OOR_PINGPONG:
                      cycles=m_NumCycles(p_Keys[0].posintrack,
                                         p_Keys[p_NumKeys-1].posintrack,
								         framepos);
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack,
												framepos);
					  if ((cycles+1) & 1)
						  framepos=p_Keys[p_NumKeys-1].posintrack-(framepos-p_Keys[0].posintrack);
                      break;
       /*case OOR_LINEAR:
                        break;
       case OOR_RELATIVE_REPEAT:
                                 break;*/
	 }
  }
  else
  if (framepos>p_Keys[p_NumKeys-1].posintrack)
  {
	 switch (p_ORTAfter)
	 {
       case OOR_HOLD: *out=p_Keys[p_NumKeys-1].roll;
                      return;
                      break;

       case OOR_CYCLE:
       case OOR_LOOP:
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						               p_Keys[p_NumKeys-1].posintrack, framepos);
                      break;

       case OOR_PINGPONG:
                      cycles=m_NumCycles(p_Keys[0].posintrack,
                                         p_Keys[p_NumKeys-1].posintrack,
								         framepos);
                      framepos=m_CycleTimeFloat(p_Keys[0].posintrack,
						                        p_Keys[p_NumKeys-1].posintrack,
												framepos);
					  if (cycles & 1)
						  framepos=p_Keys[p_NumKeys-1].posintrack-(framepos-p_Keys[0].posintrack);
                      break;
       /*case OOR_LINEAR:
                        break;
       case OOR_RELATIVE_REPEAT:
                                 break;*/
	 }
  }

  i=0;
  while ((i<p_NumKeys) && ((float4)p_Keys[i].posintrack<framepos))
    i++;

  if (i==0) i=1;  // caso della key in posizione zero
  t=(framepos-p_Keys[i-1].posintrack)/(1.0f*p_Keys[i].posintrack-p_Keys[i-1].posintrack);
  t=spline_ease(t, p_Keys[i-1].easefrom, p_Keys[i].easeto);
  t2=t*t;
  t3=t2*t;

  // caloclo delle funzioni base
  h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
  h2=-2.0f*t3 + 3.0f*t2;
  h3=t3 - 2.0f*t2 + t;
  h4=t3 - t2;

  *out=(h1*p_Keys[i-1].roll + h2*p_Keys[i].roll +
        h3*p_Keys[i-1].outcomtg + h4*p_Keys[i].incomtg);
}