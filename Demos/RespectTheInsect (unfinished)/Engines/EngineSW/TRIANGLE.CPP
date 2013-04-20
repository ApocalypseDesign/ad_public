// Metodi delle classi definite in TYPES3D.H
// By [Turbo] & [Herebit]

#include <math.h>
#include <stdlib.h>
#include "..\camera.h"
#include "..\triangle.h"
#include "..\types.h"

AD_Vertex2D vclip[2];  // vertici nuovi generati dal clip 3D
AD_Vertex2D *vnew[5];  // puntatori ai vertici che verranno passati al raster
AD_Vertex3D *vcopy3D[3];
AD_Vertex2D *vcopy2D[3];

float znear, rcpznear, screen_Xadd, screen_Yadd;
int currentmaterial_flags;
DrawTriangleProc currentmaterial_drawproc;

#define LOCAL_VERTEX_COPY \
  vcopy2D[0]=t->sp1; \
  vcopy2D[1]=t->sp2; \
  vcopy2D[2]=t->sp3; \
  vcopy3D[0]=t->v1; \
  vcopy3D[1]=t->v2; \
  vcopy3D[2]=t->v3; \


// le macro che seguono vanno bene nel caso di vertice k
// non clipato e vertice k2 clippato (z<znear); per il
// caso simmetrico nelle tria_paint_xxxx verra' fatto
// lo swap dei due indici

#define PROJECT_NEW_VERTEX \
  clipfactor=(znear - vcopy3D[k2]->tpoint.z) / (vcopy3D[k]->tpoint.z - vcopy3D[k2]->tpoint.z); \
  vclip[cc].xs=screen_Xadd + rcpznear*(vcopy3D[k2]->tpoint.x+clipfactor*(vcopy3D[k]->tpoint.x-vcopy3D[k2]->tpoint.x)); \
  vclip[cc].ys=screen_Yadd - rcpznear*(vcopy3D[k2]->tpoint.y+clipfactor*(vcopy3D[k]->tpoint.y-vcopy3D[k2]->tpoint.y)); \
  vclip[cc].z=znear; \
  vclip[cc].dist=rcpznear; \

#define PROJECT_RGB \
  vclip[cc].R=vcopy2D[k2]->R + clipfactor*(vcopy2D[k]->R - vcopy2D[k2]->R); \
  vclip[cc].G=vcopy2D[k2]->G + clipfactor*(vcopy2D[k]->G - vcopy2D[k2]->G); \
  vclip[cc].B=vcopy2D[k2]->B + clipfactor*(vcopy2D[k]->B - vcopy2D[k2]->B); \

#define PROJECT_TEXTURE \
  vclip[cc].u=vcopy2D[k2]->u + clipfactor*(vcopy2D[k]->u - vcopy2D[k2]->u); \
  vclip[cc].v=vcopy2D[k2]->v + clipfactor*(vcopy2D[k]->v - vcopy2D[k2]->v); \

#define PROJECT_ENVMAP \
  vclip[cc].envu=vcopy2D[k2]->envu + clipfactor*(vcopy2D[k]->envu - vcopy2D[k2]->envv); \
  vclip[cc].envv=vcopy2D[k2]->envv + clipfactor*(vcopy2D[k]->envv - vcopy2D[k2]->envv); \


int tria_isculled3D(AD_Tria3D *t, AD_Vect3D *camera_pos)
{
  float4 c;
  AD_Vect3D vaux;

  vect_sub(camera_pos, &t->midpoint, &vaux);
  c=vect_dot(&t->normal, &vaux);
  if (c<0) return(1); // triangolo non visibile
  else return(0);   // triangolo visibile
}


int tria_isculled2D(AD_Vertex2D *w1, AD_Vertex2D *w2, AD_Vertex2D *w3)
{
  float4 vx, vy, wx, wy;

  vx=(w1->xs)-(w2->xs);
  vy=(w1->ys)-(w2->ys);
  wx=(w2->xs)-(w3->xs);
  wy=(w2->ys)-(w3->ys);
  if ((vx*wy)<(vy*wx)) return(0);
  else return(1);  
}


int triaobj_isclipped_bounding(AD_Vect3D *v, float4 raggio, AD_Camera *telecamera)
{
  // controllo piani z del frustrum; notare che un oggetto e' clippato
  // appena risulta fuori da uno dei 4 piani del frustrum;

  if (v->z + raggio<=telecamera->znear) return(1);  // controllo piano znear
  if (v->z - raggio>=telecamera->zfar) return(1);  // controllo piano zfar

  raggio=-raggio;
  // nei prodotto scalari posso evitare di considerare certe
  // componenti, visto che per i piani left e right la
  // componente y delle normali e' zero, cosi' come le
  // componenti x delle normali dei piano up e down
  if ((v->x*telecamera->frustrum_left_normal.x+
	   v->z*telecamera->frustrum_left_normal.z) <=raggio) return(1);

  if ((v->x*telecamera->frustrum_right_normal.x+
	   v->z*telecamera->frustrum_right_normal.z) <=raggio) return(1);

  if ((v->y*telecamera->frustrum_up_normal.y+
	   v->z*telecamera->frustrum_up_normal.z) <=raggio) return(1);

  if ((v->y*telecamera->frustrum_down_normal.y+
	   v->z*telecamera->frustrum_down_normal.z) <=raggio) return(1);

  return(0);
}


void tria_precalc_normal(AD_Tria3D *t)
{
  AD_Vect3D p1, p2, norm;

  // NB: si suppone come sempre che i vertici siano in senso orario
  vect_sub(&t->v1->point, &t->v2->point, &p1);
  vect_sub(&t->v3->point, &t->v2->point, &p2);

  vect_cross(&p1, &p2, &norm);
  vect_normalize(&norm);
  vect_copy (&norm, &t->normal);
}

void tria_precalc_radius(AD_Tria3D *t)
{
  AD_Vect3D p1, p2, p3, punto_centrale;
  float4 m1, m2, m3;

  vect_copy(&t->v1->point, &punto_centrale);
  vect_add(&punto_centrale, &t->v2->point, &punto_centrale);
  vect_add(&punto_centrale, &t->v3->point, &punto_centrale);
  vect_scale(&punto_centrale, 1.0f/3.0f, &punto_centrale);

  vect_sub(&t->v1->point, &punto_centrale, &p1);   m1=vect_lenght(&p1);
  vect_sub(&t->v2->point, &punto_centrale, &p2);   m2=vect_lenght(&p2);
  vect_sub(&t->v3->point, &punto_centrale, &p3);   m3=vect_lenght(&p3);

  t->radius=m1;
  if (m2>t->radius) t->radius=m2;
  if (m3>t->radius) t->radius=m3;
}

void tria_precalc_puntomedio(AD_Tria3D *t)
{
  vect_copy(&t->v1->point, &t->midpoint);
  vect_add(&t->midpoint, &t->v2->point, &t->midpoint);
  vect_add(&t->midpoint, &t->v3->point, &t->midpoint);
  vect_scale(&t->midpoint, 1.0f/3.0f, &t->midpoint);
}

int tria_init(AD_Tria3D *t)
{
  tria_precalc_normal(t);
  tria_precalc_radius(t);
  tria_precalc_puntomedio(t);
  return(0);
}



// ####################################################################
// ######### PROCEDURE DI DISEGNO DEI TRIANGOLI (clippatori) ##########
// ####################################################################

void tria_paint_general(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale, flag2, cc, k, k2, kaux, ww;

//  color_flat=0xffffffff;

  quale=0;
  flag2=0;
  cc=0;

  LOCAL_VERTEX_COPY
  for (ww=0; ww<3; ww++)
  {
    k=ww;
	k2=ww+1;
	if (k2==3) k2=0;
	if ((vcopy3D[k]->flags & 1) == 0)  // primo non clippato
	{
	  vnew[quale]=vcopy2D[k]; quale++;
      if ((vcopy3D[k2]->flags & 1) != 0) // secondo clippato
	  {
		  PROJECT_NEW_VERTEX
		  if (currentmaterial_flags & PAINT_LIGHT)   { PROJECT_RGB }
		  if (currentmaterial_flags & PAINT_TEXTURE) { PROJECT_TEXTURE }
		  if (currentmaterial_flags & PAINT_ENVMAP)  { PROJECT_ENVMAP }
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
	  }
	  else flag2=1; // secondo non clippato
	}

    else  // primo clippato
	{
		if ((vcopy3D[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
		  if (currentmaterial_flags & PAINT_LIGHT)   { PROJECT_RGB }
		  if (currentmaterial_flags & PAINT_TEXTURE) { PROJECT_TEXTURE }
		  if (currentmaterial_flags & PAINT_ENVMAP)  { PROJECT_ENVMAP }
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}  // se il secondo e' clippato non si deve fare niente
	}
  } // enf for

  _asm pushad
  currentmaterial_drawproc(vnew[0], vnew[1], vnew[2]);
  _asm popad
  if (flag2)
  {
      _asm pushad
	  currentmaterial_drawproc(vnew[2], vnew[3], vnew[0]);
	  _asm popad
  }
}

/*
void tria_paint_general_trasparent(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale, flag2, cc, k, k2, kaux, ww;

  quale=0;
  flag2=0;
  cc=0;

  LOCAL_VERTEX_COPY
  for (ww=0; ww<3; ww++)
  {
    k=ww;
	k2=ww+1;
	if (k2==3) k2=0;
	if ((vcopy[k]->flags & 1) == 0)  // primo non clippato
	{
	  vnew[quale]=vcopy[k]->p2D; quale++;
      if ((vcopy[k2]->flags & 1) != 0) // secondo clippato
	  {
		  PROJECT_NEW_VERTEX
		  if (current_flags & PAINT_LIGHT)   { PROJECT_RGB }
		  if (current_flags & PAINT_TEXTURE) { PROJECT_TEXTURE }
		  if (current_flags & PAINT_ENVMAP)  { PROJECT_ENVMAP }
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
	  }
	  else flag2=1; // secondo non clippato
	}

    else  // primo clippato
	{
		if ((vcopy[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
		  if (current_flags & PAINT_LIGHT)   { PROJECT_RGB }
		  if (current_flags & PAINT_TEXTURE) { PROJECT_TEXTURE }
		  if (current_flags & PAINT_ENVMAP)  { PROJECT_ENVMAP }
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}  // se il secondo e' clippato non si deve fare niente
	}
  } // enf for
    
  current_raster_proc(vnew[0], vnew[1], vnew[2]);
  if (flag2) current_raster_proc(vnew[2], vnew[3], vnew[0]);
}


void tria_paint_flat(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale, flag2, cc, k, k2, kaux, ww;

  flag2=quale=cc=0;
//  color_flat=(t->color) | (t->color<<8) | (t->color<<16);
  color_flat=0xffffffff;

  if (t->materiale!=matold)
  {
    matold=t->materiale;
	current_flags=matold->flags;
	current_scanline_proc=matold->myscanline;
  }

  if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)
	   tria_raster_flat(t->v1->p2D, t->v2->p2D, t->v3->p2D);
  else
  {
    LOCAL_VERTEX_COPY
	for (ww=0; ww<3; ww++)
	{
	  k=ww;
	  k2=ww+1;
	  if (k2==3) k2=0;
	  if ((vcopy[k]->flags & 1) == 0)  // primo non clippato
	  {
		vnew[quale]=vcopy[k]->p2D; quale++;
        if ((vcopy[k2]->flags & 1) != 0) // secondo clippato
		{
		  PROJECT_NEW_VERTEX
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
		}
		else flag2=1; // secondo non clippato
	  }

	  else  // primo clippato
	  {
		if ((vcopy[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}  // se il secondo e' clippato non si deve fare niente
	  }
	} // enf for
    
	tria_raster_flat(vnew[0], vnew[1], vnew[2]);
	if (flag2) tria_raster_flat(vnew[2], vnew[3], vnew[4]);
  }
}


void tria_paint_RGB(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale=0, flag2=0, cc=0, k, k2, kaux, ww;

  if (t->materiale!=matold)
  {
    matold=t->materiale;
	current_flags=matold->flags;
	current_scanline_proc=matold->myscanline;
  }

  if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)
      tria_raster_RGB(t->v1->p2D, t->v2->p2D, t->v3->p2D);
  else
  {
    LOCAL_VERTEX_COPY
	for (ww=0; ww<3; ww++)
	{
	  k=ww;
	  k2=ww+1; if (k2==3) k2=0;
	  if ((vcopy[k]->flags & 1) == 0)  // primo non clippato
	  {
		vnew[quale]=vcopy[k]->p2D; quale++;
        if ((vcopy[k2]->flags & 1) != 0) // secondo clippato
		{
          PROJECT_NEW_VERTEX
          PROJECT_RGB
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
		}
		else flag2=1; // secondo non clippato
	  }

	  else  // primo clippato
	  {
		if ((vcopy[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
          PROJECT_RGB
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}
	  }
	} // enf for
    
	tria_raster_RGB(vnew[0], vnew[1], vnew[2]);
	if (flag2) tria_raster_RGB(vnew[2], vnew[3], vnew[0]);
  }
}


void tria_paint_texture(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale=0, flag2=0, cc=0, k, k2, kaux, ww;

  if (t->materiale!=matold)
  {
    matold=t->materiale;
	current_flags=matold->flags;
	current_scanline_proc=matold->myscanline;
    UPDATE_TEXTURE_GLOBALDATA
  }

  if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)
      tria_raster_texture(t->v1->p2D, t->v2->p2D, t->v3->p2D);
  else
  {
    LOCAL_VERTEX_COPY
    for (ww=0; ww<3; ww++)
	{
	  k=ww;
	  k2=ww+1; if (k2==3) k2=0;
	  if ((vcopy[k]->flags & 1) == 0)  // primo non clippato
	  {
		vnew[quale]=vcopy[k]->p2D; quale++;
        if ((vcopy[k2]->flags & 1) != 0) // secondo clippato
		{
          PROJECT_NEW_VERTEX
          PROJECT_TEXTURE
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
		}
		else flag2=1; // secondo non clippato
	  }

	  else  // primo clippato
	  {
		if ((vcopy[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
          PROJECT_TEXTURE
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}
	  }
	} // enf for
    
	tria_raster_texture(vnew[0], vnew[1], vnew[2]);
	if (flag2) tria_raster_texture(vnew[2], vnew[3], vnew[0]);
  }
}


void tria_paint_envmap(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale=0, flag2=0, cc=0, k, k2, kaux, ww;

  if (t->materiale!=matold)
  {
    matold=t->materiale;
	current_flags=matold->flags;
	current_scanline_proc=matold->myscanline;
    UPDATE_ENVMAP_GLOBALDATA
  }

  if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)
     tria_raster_envmap(t->v1->p2D, t->v2->p2D, t->v3->p2D);
  else
  {
    LOCAL_VERTEX_COPY
	for (ww=0; ww<3; ww++)
	{
	  k=ww;
	  k2=ww+1; if (k2==3) k2=0;
	  if ((vcopy[k]->flags & 1) == 0)  // primo non clippato
	  {
		vnew[quale]=vcopy[k]->p2D; quale++;
        if ((vcopy[k2]->flags & 1) != 0) // secondo clippato
		{
          PROJECT_NEW_VERTEX
          PROJECT_ENVMAP
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
		}
		else flag2=1; // secondo non clippato
	  }

	  else  // primo clippato
	  {
		if ((vcopy[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
          PROJECT_ENVMAP
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}
	  }
	} // enf for
    
	tria_raster_envmap(vnew[0], vnew[1], vnew[2]);
	if (flag2) tria_raster_envmap(vnew[2], vnew[3], vnew[0]);
  }
}


void tria_paint_texRGB(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale=0, flag2=0, cc=0, k, k2, kaux, ww;

  if (t->materiale!=matold)
  {
    matold=t->materiale;
	current_flags=matold->flags;
	current_scanline_proc=matold->myscanline;
    UPDATE_TEXTURE_GLOBALDATA
  }

  if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)  
     tria_raster_texRGB(t->v1->p2D, t->v2->p2D, t->v3->p2D);
  else
  {
    LOCAL_VERTEX_COPY
	for (ww=0; ww<3; ww++)
	{
	  k=ww;
	  k2=ww+1; if (k2==3) k2=0;
	  if ((vcopy[k]->flags & 1) == 0)  // primo non clippato
	  {
		vnew[quale]=vcopy[k]->p2D; quale++;
        if ((vcopy[k2]->flags & 1) != 0) // secondo clippato
		{
          PROJECT_NEW_VERTEX
          PROJECT_RGB
		  PROJECT_TEXTURE
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
		}
		else flag2=1; // secondo non clippato
	  }

	  else  // primo clippato
	  {
		if ((vcopy[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
          PROJECT_RGB
		  PROJECT_TEXTURE
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}
	  }
	} // enf for
    
	tria_raster_texRGB(vnew[0], vnew[1], vnew[2]);
	if (flag2) tria_raster_texRGB(vnew[2], vnew[3], vnew[0]);
  }
}


void tria_paint_envRGB(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale=0, flag2=0, cc=0, k, k2, kaux, ww;

  if (t->materiale!=matold)
  {
    matold=t->materiale;
	current_flags=matold->flags;
	current_scanline_proc=matold->myscanline;
    UPDATE_ENVMAP_GLOBALDATA
  }
  
  if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)  
     tria_raster_envRGB(t->v1->p2D, t->v2->p2D, t->v3->p2D);
  else
  {
    LOCAL_VERTEX_COPY
	for (ww=0; ww<3; ww++)
	{
	  k=ww;
	  k2=k+1; if (k2==3) k2=0;
	  if ((vcopy[k]->flags & 1) == 0)  // primo non clippato
	  {
		vnew[quale]=vcopy[k]->p2D; quale++;
        if ((vcopy[k2]->flags & 1) != 0) // secondo clippato
		{
          PROJECT_NEW_VERTEX
          PROJECT_RGB
		  PROJECT_ENVMAP
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
		}
		else flag2=1; // secondo non clippato
	  }

	  else  // primo clippato
	  {
		if ((vcopy[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
          PROJECT_RGB
		  PROJECT_ENVMAP
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}
	  }
	} // enf for
    
	tria_raster_envRGB(vnew[0], vnew[1], vnew[2]);
	if (flag2) tria_raster_envRGB(vnew[2], vnew[3], vnew[0]);
  }
}


void tria_paint_texenv(AD_Tria3D *t)
{
  float4 clipfactor;
  int quale=0, flag2=0, cc=0, k, k2, kaux, ww;

  if (t->materiale!=matold)
  {
    matold=t->materiale;
	current_flags=matold->flags;
	current_scanline_proc=matold->myscanline;
    UPDATE_TEXTURE_GLOBALDATA
    UPDATE_ENVMAP_GLOBALDATA
  }

  if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)  
     tria_raster_texenv(t->v1->p2D, t->v2->p2D, t->v3->p2D);
  else
  {
    LOCAL_VERTEX_COPY
	for (ww=0; ww<3; ww++)
	{
	  k=ww;
	  k2=ww+1; if (k2==3) k2=0;
	  if ((vcopy[k]->flags & 1) == 0)  // primo non clippato
	  {
		vnew[quale]=vcopy[k]->p2D; quale++;
        if ((vcopy[k2]->flags & 1) != 0) // secondo clippato
		{
          PROJECT_NEW_VERTEX
          PROJECT_TEXTURE
		  PROJECT_ENVMAP
		  vnew[quale]=&vclip[cc]; 
		  cc++; quale++;
		}
		else flag2=1; // secondo non clippato
	  }

	  else  // primo clippato
	  {
		if ((vcopy[k2]->flags & 1) == 0)  // secondo non clippato
		{
		  kaux=k2; k2=k; k=kaux;  // swap degl'indici per far quadrare le macro
		  PROJECT_NEW_VERTEX
          PROJECT_TEXTURE
		  PROJECT_ENVMAP
		  vnew[quale]=&vclip[cc];
		  cc++; quale++;
		}
	  }
	} // enf for
    
	tria_raster_texenv(vnew[0], vnew[1], vnew[2]);
	if (flag2) tria_raster_texenv(vnew[2], vnew[3], vnew[0]);
  }
}
*/
