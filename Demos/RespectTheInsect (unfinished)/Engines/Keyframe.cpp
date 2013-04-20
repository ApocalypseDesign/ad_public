// Metodi per le classi definite in KEYFRAME.H
// By [Turbo]

#include <math.h>
#include "keyframe.h"
#include "..\Math\fastmath.h"

extern HWND miawin;         // finestra principale; usata qui per le debug_error
extern FILE *fdeb;

float4 spline_ease (float4 t, float4 easefrom, float4 easeto)
// spline ease per rimappare i parametri tra le key. Codice preso
// spudoratamente da CLAX, causa assenza totale di info al riguardo
{
  float4 k, s;

  s=easeto+easefrom;
  if (s==0) return(t);

  if (s>1.0)
  {
    easeto=easeto/s;
    easefrom=easefrom/s;
  }

  k=(float)(1.0/(2.0-easeto-easefrom));
  if (t<easefrom) return((k/easefrom)*t*t);
  else
  {
    if (t<(1.0-easeto)) return((float)(k*(2.0*t-easefrom)));
    else
    {
      t=1.0f-t;
      return(1.0f-(k/easeto)*t*t);
    }
  }
}


// ###########################################################################
// #                               POSIZIONI                                 #
// ###########################################################################

int postrack::init(void)
{
  int32 j, i2, i1, i0;
  float4 c1, c2;
  float csi, cso;
  AD_Vect3D k, o;

  if (numkey<=1) return(0);
  if (type & BEZIER_TRACK) return(0);
  if (numkey==2)
  {
    c1=1.0f-posizioni[0].tension;
	vect_sub(&posizioni[1].p, &posizioni[0].p, &k);
	vect_scale(&k, c1, &posizioni[0].outcomtg);

    c2=1.0f-posizioni[1].tension;
	vect_sub(&posizioni[1].p, &posizioni[0].p, &k);
	vect_scale(&k, c2, &posizioni[1].incomtg);
	return(1);
  }

  // caso di 3 o piu' key  
  for (j=0; j<numkey; j++)
  {
    // caso della prima key
    if (j==0)
    {
      i2=j+2;
      i1=j+1;
      i0=j;
	  csi=(float)posizioni[i2].posintrack-posizioni[i0].posintrack;
	  cso=(float)posizioni[i1].posintrack-posizioni[i0].posintrack;
      vect_sub(&posizioni[i2].p, &posizioni[i0].p, &k);
      vect_sub(&posizioni[i1].p, &posizioni[i0].p, &o);
	  vect_scale(&k, -cso/(2.0f*csi), &k);
	  vect_scale(&o, 3.0f/2.0f, &o);
	  vect_add(&k, &o, &posizioni[i0].outcomtg);
	  vect_scale(&posizioni[i0].outcomtg, (1.0f-posizioni[i0].tension), &posizioni[i0].outcomtg);
    }
    else
    {
	  // caso dell'ultima key
	  if (j==numkey-1)
	  {
	    i2=j-2;
	    i1=j-1;
	    i0=j;
		csi=(float)posizioni[i0].posintrack-posizioni[i2].posintrack;
	    cso=(float)posizioni[i0].posintrack-posizioni[i1].posintrack;
        vect_sub(&posizioni[i0].p, &posizioni[i2].p, &k);
        vect_sub(&posizioni[i0].p, &posizioni[i1].p, &o);
		vect_scale(&k, -cso/(2.0f*csi), &k);
	    vect_scale(&o, 3.0f/2.0f, &o);
	    vect_add(&k, &o, &posizioni[i0].incomtg);
	    vect_scale(&posizioni[i0].incomtg, (1.0f-posizioni[i0].tension), &posizioni[i0].incomtg);
	  }
	  else
	  // caso di una key intermezza
	  {
	    i2=j+1;
	    i1=j;
	    i0=j-1;
		cso=2.0f*(posizioni[j+1].posintrack-posizioni[j].posintrack)/(posizioni[j+1].posintrack-posizioni[j-1].posintrack);
		csi=2.0f*(posizioni[j].posintrack-posizioni[j-1].posintrack)/(posizioni[j+1].posintrack-posizioni[j-1].posintrack);

		// CALCOLO TANGENTE OUTCOMING
        c1=cso*(1.0f-posizioni[j].tension)*
               (1.0f-posizioni[j].continuity)*
               (1.0f-posizioni[j].bias)/2.0f;

        c2=cso*(1.0f-posizioni[j].tension)*
               (1.0f+posizioni[j].continuity)*
               (1.0f+posizioni[j].bias)/2.0f;

        vect_sub(&posizioni[i2].p, &posizioni[i1].p, &k);
        vect_sub(&posizioni[i1].p, &posizioni[i0].p, &o);
	    vect_scale(&k, c1, &k);
        vect_scale(&o, c2, &o);
	    vect_add(&k, &o, &posizioni[j].outcomtg);

	    // CALCOLO TANGENTE INCOMING
        c1=csi*(1.0f-posizioni[j].tension)*
               (1.0f+posizioni[j].continuity)*
               (1.0f-posizioni[j].bias)/2.0f;

        c2=csi*(1.0f-posizioni[j].tension)*
               (1.0f-posizioni[j].continuity)*
               (1.0f+posizioni[j].bias)/2.0f;

	    vect_sub(&posizioni[i2].p, &posizioni[i1].p, &k);
        vect_sub(&posizioni[i1].p, &posizioni[i0].p, &o);
	    vect_scale(&k, c1, &k);
        vect_scale(&o, c2, &o);
	    vect_add(&k, &o, &posizioni[j].incomtg);
      }
	}
  } // end ciclo for
  return(0);
}


void postrack::get_data(float4 framepos, AD_Vect3D *out)
{
  float4 h1, h2, h3, h4, t, t2, t3;
  int32 intpos, i;

  if ((framepos<0.0) || (numkey<1)) return;
  if (posizioni==NULL) return;

  if ((numkey==1) || (framepos<posizioni[0].posintrack))
  {
	vect_copy(&posizioni[0].p, out);
	return;
  }
  if (numkey<2) return;

  intpos=myfist(fceil(framepos));
  // ceil(x) trova il piu' piccolo intero non minore ad x; se
  // x e' 13.56 riotorna 14 se x=16.0 ritorna 16

  i=0;
  while ((i<numkey) && (posizioni[i].posintrack<intpos))
  {
    i++;
  }
  
  if (i>=numkey) 
  {
	vect_copy(&posizioni[numkey-1].p, out);
	return;
  }

  if (i==0) i=1;  // caso della key in posizione zero
  t=(framepos-posizioni[i-1].posintrack)/(1.0f*posizioni[i].posintrack-posizioni[i-1].posintrack);
  t=spline_ease(t, posizioni[i-1].easefrom, posizioni[i].easeto);
  t2=t*t;
  t3=t2*t;

  // caloclo delle funzioni base
  h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
  h2=-2.0f*t3 + 3.0f*t2;
  h3=t3 - 2.0f*t2 + t;
  h4=t3 - t2;

// Interpolazione Hermite
  out->x=(h1*posizioni[i-1].p.x + h2*posizioni[i].p.x +
          h3*posizioni[i-1].outcomtg.x + h4*posizioni[i].incomtg.x);
  out->y=(h1*posizioni[i-1].p.y + h2*posizioni[i].p.y +
          h3*posizioni[i-1].outcomtg.y + h4*posizioni[i].incomtg.y);
  out->z=(h1*posizioni[i-1].p.z + h2*posizioni[i].p.z +
          h3*posizioni[i-1].outcomtg.z + h4*posizioni[i].incomtg.z);

  // Interpolazione lineare
//  aux=(1.0-t)*posizioni[i-1].p+t*posizioni[i].p;
  return;
}



// ###########################################################################
// #                               ROTAZIONI                                                                                               #
// ###########################################################################

int rottrack::init(void)
{
  int32 j;
  AD_Quaternion qaux, qold, Tn0, Tn1, qinv1, qinv2;
  AD_Quaternion qaux1, qaux2;
  AD_Quaternion qaux11, log1, log2, qprev, qnext;
  float s1, s2, cso, csi;

  // sistemazione a quaternioni unitari assoluti (non riferiti
  // al quat precedente)
  if (numkey>0)
  for (j=0; j<numkey; j++)
  {
    // setta un AD_Quaternione corrispondente alla key
    quat_copy(&rotazioni[j].rotquat, &qaux);

    // trasformalo in un AD_Quaternion di rotazione
    quat_quat_to_rotquat(&qaux);
	if (type & RELATIVE_TRACK)
	{
      if (j==0) quat_copy(&qaux, &qold);
      else quat_mul(&qaux, &qold, &qold);
	}
	else quat_copy(&qaux, &qold);
	quat_copy(&qold, &rotazioni[j].rotquat);
  }
  if (numkey<=1) return(0);

  // calcolo delle tangenti incoming e outcoming da usare poi
  // nell'interpolazione di Beziér alla TCB

  // caso di due sole key, si ottiene uno slerp come deve essere
  quat_copy(&rotazioni[0].rotquat, &rotazioni[0].outcomtg);
  quat_copy(&rotazioni[numkey-1].rotquat, &rotazioni[numkey-1].incomtg);
  if (numkey<=2) return(0);

  for (j=1; j<numkey-1; j++)
  {
    quat_copy (&rotazioni[j-1].rotquat, &qprev);
    quat_copy (&rotazioni[j+1].rotquat, &qnext);

	cso=2.0f*(rotazioni[j+1].posintrack-rotazioni[j].posintrack)/(rotazioni[j+1].posintrack-rotazioni[j-1].posintrack);
	csi=2.0f*(rotazioni[j].posintrack-rotazioni[j-1].posintrack)/(rotazioni[j+1].posintrack-rotazioni[j-1].posintrack);

	quat_inverse(&rotazioni[j].rotquat, &qinv1);
	quat_inverse(&qprev, &qinv2);
	quat_mul(&qinv1, &qnext, &qaux1);
	quat_mul(&qinv2, &rotazioni[j].rotquat, &qaux2);
    quat_log(&qaux1, &log1);
    quat_log(&qaux2, &log2);


	// calcolo tangente outcoming
	s1=cso*0.5f*(1.0f-rotazioni[j].tension)*(1.0f-rotazioni[j].continuity)*(1.0f-rotazioni[j].bias);
	s2=cso*0.5f*(1.0f-rotazioni[j].tension)*(1.0f+rotazioni[j].continuity)*(1.0f+rotazioni[j].bias);
	quat_scale(&log1, s1, &qaux1);
	quat_scale(&log2, s2, &qaux2);
	quat_add(&qaux1, &qaux2, &Tn0);
	quat_sub(&Tn0, &log1, &qaux);
	quat_scale(&qaux, 0.5f, &qaux);
	quat_exp(&qaux, &qaux11);
	quat_mul(&rotazioni[j].rotquat, &qaux11, &rotazioni[j].outcomtg);


	// calcolo tangente incoming
	s1=csi*0.5f*(1.0f-rotazioni[j].tension)*(1.0f+rotazioni[j].continuity)*(1.0f-rotazioni[j].bias);
	s2=csi*0.5f*(1.0f-rotazioni[j].tension)*(1.0f-rotazioni[j].continuity)*(1.0f+rotazioni[j].bias);
	quat_scale(&log1, s1, &qaux1);
	quat_scale(&log2, s2, &qaux2);
	quat_add(&qaux1, &qaux2, &Tn1);
	quat_sub(&log2, &Tn1, &qaux);
	quat_scale(&qaux, 0.5f, &qaux);
	quat_exp(&qaux, &qaux11);
	quat_mul(&rotazioni[j].rotquat, &qaux11, &rotazioni[j].incomtg);
  }
  return(0);
}


void rottrack::get_data(float4 framepos, AD_Quaternion *out)
{
  float4 t, spin, angle;
  int32 intpos, i;
  AD_Quaternion q1, q2;

  if ((framepos<0.0) || (numkey<1)) return;
  if (rotazioni==NULL) return;
  if ((numkey==1) || (framepos<rotazioni[0].posintrack))
  {
    quat_copy(&rotazioni[0].rotquat, out);
	return;
  }
  if (numkey<2) return;
  
  intpos=myfist(fceil(framepos));

  i=0;
  while ((i<numkey) && (rotazioni[i].posintrack<intpos))
  {
    i++;
  }
  
  if (i>=numkey) 
  {
    quat_copy(&rotazioni[numkey-1].rotquat, out);
	return;
  }
  
  if (i==0) i=1;  // caso della key in posizione zero
  t=(framepos-rotazioni[i-1].posintrack)/(1.0f*rotazioni[i].posintrack-rotazioni[i-1].posintrack);
  t=spline_ease(t, rotazioni[i-1].easefrom, rotazioni[i].easeto);
  angle=rotazioni[i].roll-rotazioni[i-1].roll;

  if (angle > 0.0) spin = (float)floor (angle / (2.0*M_PI));
  else spin = (float)ceil (angle / (2.0*M_PI));

//  slerp normale
//  quat_slerp(&rotazioni[i-1].rotquat, &rotazioni[i].rotquat, t, spin, out);

  // squad
  if (type & RELATIVE_TRACK)
  {
    quat_slerp(&rotazioni[i-1].rotquat, &rotazioni[i].rotquat, t, spin, &q1);
    quat_slerp(&rotazioni[i-1].outcomtg, &rotazioni[i].incomtg, t, spin, &q2);
    quat_slerp(&q1, &q2, 2.0f*t*(1-t), 0, out);
  }
  else
  {
    quat_slerp_flip(&rotazioni[i-1].rotquat, &rotazioni[i].rotquat, t, spin, &q1);
    quat_slerp_flip(&rotazioni[i-1].outcomtg, &rotazioni[i].incomtg, t, spin, &q2);
    quat_slerp_flip(&q1, &q2, 2.0f*t*(1-t), 0, out);
  }
}



// ###########################################################################
// #                                ROLLING                                  #
// ###########################################################################

int rolltrack::init(void)
{
  int32 j, i2, i1, i0;
  float4 c1, c2, csi, cso, k, o;

  for (j=0; j<numkey; j++)
  {
	  rollazioni[j].roll*=(float)(M_PI/180.0f);
  }

  if (numkey<=1) return(0);
  if (type & BEZIER_TRACK) return(0);

  if (numkey==2)
  {
    c1=1.0f-rollazioni[0].tension;
    rollazioni[0].outcomtg=c1*(rollazioni[1].roll-rollazioni[0].roll);

    c2=1.0f-rollazioni[1].tension;
    rollazioni[1].incomtg=c2*(rollazioni[1].roll-rollazioni[0].roll);
	return(1);
   }

  // caso di 3 o piu' key
  for (j=0; j<numkey; j++)
  {
    // caso della prima key
    if (j==0)
    {
      i2=j+2;
      i1=j+1;
      i0=j;
	  csi=(float)rollazioni[i2].posintrack-rollazioni[i0].posintrack;
	  cso=(float)rollazioni[i1].posintrack-rollazioni[i0].posintrack;

	  k=rollazioni[i2].roll - rollazioni[i0].roll;
      o=rollazioni[i1].roll - rollazioni[i0].roll;
	  k*=-cso/(2.0f*csi);
	  o*=3.0f/2.0f;
	  rollazioni[i0].outcomtg=o+k;
	  rollazioni[i0].outcomtg*=(1.0f-rollazioni[i0].tension);
    }
    else
    {
	  // caso dell'ultima key
	  if (j==numkey-1)
	  {
	    i2=j-2;
	    i1=j-1;
	    i0=j;
		csi=(float)rollazioni[i0].posintrack-rollazioni[i2].posintrack;
	    cso=(float)rollazioni[i0].posintrack-rollazioni[i1].posintrack;

	    k=rollazioni[i0].roll - rollazioni[i2].roll;
        o=rollazioni[i0].roll - rollazioni[i1].roll;
	    k*=-cso/(2.0f*csi);
	    o*=3.0f/2.0f;
	    rollazioni[i0].incomtg=o+k;
	    rollazioni[i0].incomtg*=(1.0f-rollazioni[i0].tension);
	  }
	  else
	  // caso di una key intermezza
	  {
	    i2=j+1;
	    i1=j;
	    i0=j-1;
	    csi=2.0f*(rollazioni[j+1].posintrack-rollazioni[j].posintrack)/(rollazioni[j+1].posintrack-rollazioni[j-1].posintrack);
	    cso=2.0f*(rollazioni[j].posintrack-rollazioni[j-1].posintrack)/(rollazioni[j+1].posintrack-rollazioni[j-1].posintrack);

        // CALCOLO TANGENTE OUTCOMING
        c1=cso*(1.0f-rollazioni[j].tension)*
               (1.0f-rollazioni[j].continuity)*
               (1.0f-rollazioni[j].bias)/2.0f;

        c2=cso*(1.0f-rollazioni[j].tension)*
               (1.0f+rollazioni[j].continuity)*
               (1.0f+rollazioni[j].bias)/2.0f;

        rollazioni[j].outcomtg=c1*(rollazioni[i2].roll-rollazioni[i1].roll) +
			                   c2*(rollazioni[i1].roll-rollazioni[i0].roll);


        // CALCOLO TANGENTE INCOMING
        c1=csi*(1.0f-rollazioni[j].tension)*
               (1.0f+rollazioni[j].continuity)*
               (1.0f-rollazioni[j].bias)/2.0f;

        c2=csi*(1.0f-rollazioni[j].tension)*
               (1.0f-rollazioni[j].continuity)*
               (1.0f+rollazioni[j].bias)/2.0f;

        rollazioni[j].incomtg=c1*(rollazioni[i2].roll-rollazioni[i1].roll) +
			                  c2*(rollazioni[i1].roll-rollazioni[i0].roll);
	  }
	}
  } // end ciclo for

  return(0);
}


void rolltrack::get_data(float4 framepos, float4 *out)
{
  float4 h1, h2, h3, h4, t, t2, t3;
  int32 intpos, i;

  if ((framepos<0.0) || (numkey<1)) return;
  if (rollazioni==NULL) return;
  if ((numkey==1) || (framepos<rollazioni[0].posintrack))
  {
	*out=rollazioni[0].roll;
	return;
  }
  if (numkey<2) return;

  intpos=myfist(fceil(framepos));

  i=0;
  while ((i<numkey) && (rollazioni[i].posintrack<intpos))
  {
    i++;
  }
  if (i>=numkey)
  {
    *out=rollazioni[numkey-1].roll;
	return;
  }

  if (i==0) i=1;  // caso della key in posizione zero
  t=(framepos-rollazioni[i-1].posintrack)/(1.0f*rollazioni[i].posintrack-rollazioni[i-1].posintrack);
  t=spline_ease(t, rollazioni[i-1].easefrom, rollazioni[i].easeto);
  t2=t*t;
  t3=t2*t;

  // caloclo delle funzioni base
  h1= 2.0f*t3 - 3.0f*t2 + 1.0f;
  h2=-2.0f*t3 + 3.0f*t2;
  h3=t3 - 2.0f*t2 + t;
  h4=t3 - t2;

  *out=(h1*rollazioni[i-1].roll + h2*rollazioni[i].roll +
        h3*rollazioni[i-1].outcomtg + h4*rollazioni[i].incomtg);

// aux=(1.0-t)*rollazioni[i-1].roll+t*rollazioni[i].roll;
  return;
}


void material_parameters_tracks::init(void)
{
  if (trasparency_track!=(rolltrack *)NULL)
       trasparency_track->init();
}