// Metodi della classe AD_Object3D
// By Here & [Turbo]

#include <windows.h>
#include <math.h>
#include "..\..\utility.h"
#include "..\..\Math\matrix.h"
#include "..\scene3d.h"
#include "..\object3D.h"
#include "..\lights.h"
#include "..\..\Math\fastmath.h"
//#include "lod.h"

float CUL_EPS=1E-6f;
// int scrivi_deb=1;
extern float zfar, znear, rcpznear, screen_Xadd, screen_Yadd;
extern float inv_zfar_znear;

#define VERTEX_PIPELINE_ENVRGB(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     mat_mulvect(&matrixtransform_all, &v->point, &v->tpoint); \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       v->pjpoint.x=(v->tpoint.x*invz) + screen_Xadd; \
       v->pjpoint.y=screen_Yadd - (v->tpoint.y*invz); \
	   v->pjpoint.z=(v->tpoint.z-znear)*inv_zfar_znear; \
	   v->w=invz; \
     } \
	 else v->flags=v->flags | 1; \
  } \
  p2D->xs=v->pjpoint.x; \
  p2D->ys=v->pjpoint.y; \
  p2D->z=v->pjpoint.z; \
  p2D->dist=v->w; \
  mat_mulvectenv(&matrixrot_all, n, &vtmp); \
  p2D->envu=(vtmp.x*ENVMAP_PRECISION+ENVMAP_PRECISION); \
  p2D->envv=(vtmp.y*ENVMAP_PRECISION+ENVMAP_PRECISION); \
  vtmp.x=vtmp.y=vtmp.z=5; \
  for (wl=0; wl<num_omni; wl++) \
  { \
       vect_sub_inline(&omnilight[wl].currentpos_inobject, &v->point, &light_vertex_vector); \
       cosalpha=vect_dot(n, &light_vertex_vector); \
       if (is_float_positive(cosalpha)) \
       { \
		 dist=1.0f/vect_fast_lenght(&light_vertex_vector); \
	     cosalpha*=dist; \
	     if (dist < omnilight[wl].inv_near_radius) \
         { \
	       cosalpha*=dist*omnilight[wl].near_radius; \
	       if (dist < omnilight[wl].inv_far_radius) \
	         cosalpha*=dist*omnilight[wl].far_radius; \
         } \
         vtmp.x+=cosalpha*omnilight[wl].currentcolor.x; \
         vtmp.y+=cosalpha*omnilight[wl].currentcolor.y; \
		 vtmp.z+=cosalpha*omnilight[wl].currentcolor.z; \
       } \
  } \
  if (FP_BITS(vtmp.x)>FP_BITS(RGB_MAXVALUE)) FP_BITS(p2D->R)=FP_BITS(RGB_MAXVALUE); \
  else FP_BITS(p2D->R)=FP_BITS(vtmp.x); \
  if (FP_BITS(vtmp.y)>FP_BITS(RGB_MAXVALUE)) FP_BITS(p2D->G)=FP_BITS(RGB_MAXVALUE); \
  else FP_BITS(p2D->G)=FP_BITS(vtmp.y); \
  if (FP_BITS(vtmp.z)>FP_BITS(RGB_MAXVALUE)) FP_BITS(p2D->B)=FP_BITS(RGB_MAXVALUE); \
  else FP_BITS(p2D->B)=FP_BITS(vtmp.z); \


#define VERTEX_PIPELINE_RGB(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     mat_mulvect(&matrixtransform_all, &v->point, &v->tpoint); \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       v->pjpoint.x=(v->tpoint.x*invz) + screen_Xadd; \
       v->pjpoint.y=screen_Yadd - (v->tpoint.y*invz); \
	   v->pjpoint.z=(v->tpoint.z-znear)*inv_zfar_znear; \
	   v->w=invz; \
     } \
	 else v->flags=v->flags | 1; \
  } \
  p2D->xs=v->pjpoint.x; \
  p2D->ys=v->pjpoint.y; \
  p2D->z=v->pjpoint.z; \
  p2D->dist=v->w; \
  vtmp.x=vtmp.y=vtmp.z=5; \
  for (wl=0; wl<num_omni; wl++) \
  { \
       vect_sub_inline(&omnilight[wl].currentpos_inobject, &v->point, &light_vertex_vector); \
       cosalpha=vect_dot(n, &light_vertex_vector); \
       if (is_float_positive(cosalpha)) \
       { \
	     dist=1.0f/vect_fast_lenght(&light_vertex_vector); \
	     cosalpha*=dist; \
	     if (dist < omnilight[wl].inv_near_radius) \
         { \
	       cosalpha*=dist*omnilight[wl].near_radius; \
	       if (dist < omnilight[wl].inv_far_radius) \
	         cosalpha*=dist*omnilight[wl].far_radius; \
         } \
         vtmp.x+=cosalpha*omnilight[wl].currentcolor.x; \
         vtmp.y+=cosalpha*omnilight[wl].currentcolor.y; \
		 vtmp.z+=cosalpha*omnilight[wl].currentcolor.z; \
       } \
  } \
  if (FP_BITS(vtmp.x)>FP_BITS(RGB_MAXVALUE)) FP_BITS(p2D->R)=FP_BITS(RGB_MAXVALUE); \
  else FP_BITS(p2D->R)=FP_BITS(vtmp.x); \
  if (FP_BITS(vtmp.y)>FP_BITS(RGB_MAXVALUE)) FP_BITS(p2D->G)=FP_BITS(RGB_MAXVALUE); \
  else FP_BITS(p2D->G)=FP_BITS(vtmp.y); \
  if (FP_BITS(vtmp.z)>FP_BITS(RGB_MAXVALUE)) FP_BITS(p2D->B)=FP_BITS(RGB_MAXVALUE); \
  else FP_BITS(p2D->B)=FP_BITS(vtmp.z); \


#define VERTEX_PIPELINE_ENVMAP(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     mat_mulvect(&matrixtransform_all, &v->point, &v->tpoint); \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       v->pjpoint.x=(v->tpoint.x*invz) + screen_Xadd; \
       v->pjpoint.y=screen_Yadd - (v->tpoint.y*invz); \
	   v->pjpoint.z=(v->tpoint.z-znear)*inv_zfar_znear; \
	   v->w=invz; \
     } \
	 else v->flags=v->flags | 1; \
  } \
  p2D->xs=v->pjpoint.x; \
  p2D->ys=v->pjpoint.y; \
  p2D->z=v->pjpoint.z; \
  p2D->dist=v->w; \
  mat_mulvectenv(&matrixrot_all, n, &vtmp); \
  p2D->envu=(vtmp.x*ENVMAP_PRECISION+ENVMAP_PRECISION); \
  p2D->envv=(vtmp.y*ENVMAP_PRECISION+ENVMAP_PRECISION); \


#define VERTEX_PIPELINE_ELSE(p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     mat_mulvect(&matrixtransform_all, &v->point, &v->tpoint); \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       v->pjpoint.x=(v->tpoint.x*invz) + screen_Xadd; \
       v->pjpoint.y=screen_Yadd - (v->tpoint.y*invz); \
	   v->pjpoint.z=(v->tpoint.z-znear)*inv_zfar_znear; \
	   v->w=invz; \
     } \
	 else v->flags=v->flags | 1; \
  } \
  p2D->xs=v->pjpoint.x; \
  p2D->ys=v->pjpoint.y; \
  p2D->z=v->pjpoint.z; \
  p2D->dist=v->w; \

/*
#define VERTEX_PIPELINE_ELSE \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     mat_mulvect(&matrixtransform_all, v->point, v3D); \
     if (v3D->z>znear) \
	 { \
       invz=1.0f/v3D->z; \
       v2D->xs=(v3D->x*invz) + screen_Xadd; \
       v2D->ys=screen_Yadd - (v3D->y*invz); \
	   v2D->z=(v3D->z-znear)*inv_zfar_znear; \
       v2D->dist=invz; \
     } \
	 else v->flags=v->flags | 1; \
	 v->tpoint=v3D; \
	 v->p2D=v2D; \
	 v3D++; \
     v2D++; \
  } \
*/


#define TRIA_PIPELINE_ENVRGB \
  for (w=0; w<num_tria_envRGB; w++) \
  { \
    vect_sub_inline(&camerapos_inobject, &tria_envRGB[w].midpoint, &vtmp); \
    if ((tria_envRGB[w].materiale->double_facing) || (vect_dot(&tria_envRGB[w].normal, &vtmp)>CUL_EPS)) \
	{ \
      mat_mulvect(&matrixtall_clip, &tria_envRGB[w].midpoint, &vtmp); \
	  { \
        tria_envRGB[w].mid_z=vtmp.z; \
	    v=tria_envRGB[w].v1; \
	    VERTEX_PIPELINE_ENVRGB(tria_envRGB[w].n1, tria_envRGB[w].sp1) \
        v=tria_envRGB[w].v2; \
	    VERTEX_PIPELINE_ENVRGB(tria_envRGB[w].n2, tria_envRGB[w].sp2) \
        v=tria_envRGB[w].v3; \
	    VERTEX_PIPELINE_ENVRGB(tria_envRGB[w].n3, tria_envRGB[w].sp3) \
        if (((tria_envRGB[w].v1->flags + tria_envRGB[w].v2->flags + tria_envRGB[w].v3->flags + 1) & 3) !=0 ) \
		{ \
		  if (tria_envRGB[w].materiale->flags & IS_TRASPARENT) \
		    *list_to_paint_trasparent++=&tria_envRGB[w]; \
          else \
		    *tria_envRGB[w].materiale->my_tria_list++=&tria_envRGB[w]; \
		} \
	  } \
	} \
  } \
/* if (!triaobj_isclipped_bounding_inline(&vtmp, tria_envRGB[w].radius*max_scale, telecamera)) \ */


#define TRIA_PIPELINE_RGB \
  for (w=0; w<num_tria_RGB; w++) \
  { \
    vect_sub_inline(&camerapos_inobject, &tria_RGB[w].midpoint, &vtmp); \
    if ((tria_RGB[w].materiale->double_facing) || (vect_dot(&tria_RGB[w].normal, &vtmp)>CUL_EPS)) \
	{ \
      mat_mulvect(&matrixtall_clip, &tria_RGB[w].midpoint, &vtmp); \
	  { \
        tria_RGB[w].mid_z=vtmp.z; \
	    v=tria_RGB[w].v1; \
	    VERTEX_PIPELINE_RGB(tria_RGB[w].n1, tria_RGB[w].sp1) \
        v=tria_RGB[w].v2; \
	    VERTEX_PIPELINE_RGB(tria_RGB[w].n2, tria_RGB[w].sp2) \
        v=tria_RGB[w].v3; \
	    VERTEX_PIPELINE_RGB(tria_RGB[w].n3, tria_RGB[w].sp3) \
        if (((tria_RGB[w].v1->flags + tria_RGB[w].v2->flags + tria_RGB[w].v3->flags + 1) & 3) !=0 ) \
		{ \
		  if (tria_RGB[w].materiale->flags & IS_TRASPARENT) \
		    *list_to_paint_trasparent++=&tria_RGB[w]; \
          else \
		    *tria_RGB[w].materiale->my_tria_list++=&tria_RGB[w]; \
		} \
	  } \
	} \
  } \

/*
  if (!triaobj_isclipped_bounding_inline(&vtmp, tria_RGB[w].radius*max_scale, telecamera)) \
*/


#define TRIA_PIPELINE_ENVMAP \
  for (w=0; w<num_tria_envmap; w++) \
  { \
    vect_sub_inline(&camerapos_inobject, &tria_envmap[w].midpoint, &vtmp); \
    if ((tria_envmap[w].materiale->double_facing) || (vect_dot(&tria_envmap[w].normal, &vtmp)>CUL_EPS)) \
	{ \
      mat_mulvect(&matrixtall_clip, &tria_envmap[w].midpoint, &vtmp); \
	  { \
        tria_envmap[w].mid_z=vtmp.z; \
	    v=tria_envmap[w].v1; \
	    VERTEX_PIPELINE_ENVMAP(tria_envmap[w].n1, tria_envmap[w].sp1) \
        v=tria_envmap[w].v2; \
	    VERTEX_PIPELINE_ENVMAP(tria_envmap[w].n2, tria_envmap[w].sp2) \
        v=tria_envmap[w].v3; \
	    VERTEX_PIPELINE_ENVMAP(tria_envmap[w].n3, tria_envmap[w].sp3) \
        if (((tria_envmap[w].v1->flags + tria_envmap[w].v2->flags + tria_envmap[w].v3->flags + 1) & 3) !=0 ) \
		{ \
		  if (tria_envmap[w].materiale->flags & IS_TRASPARENT) \
		    *list_to_paint_trasparent++=&tria_envmap[w]; \
          else \
		    *tria_envmap[w].materiale->my_tria_list++=&tria_envmap[w]; \
		} \
	  } \
	} \
  } \
/*	  if (!triaobj_isclipped_bounding_inline(&vtmp, tria_envmap[w].radius*max_scale, telecamera)) \ */

#define TRIA_PIPELINE_ELSE \
  for (w=0; w<num_tria; w++) \
  { \
    vect_sub_inline(&camerapos_inobject, &tria[w].midpoint, &vtmp); \
    if ((tria[w].materiale->double_facing) || (vect_dot(&tria[w].normal, &vtmp)>CUL_EPS)) \
	{ \
      mat_mulvect(&matrixtall_clip, &tria[w].midpoint, &vtmp); \
	  { \
        tria[w].mid_z=vtmp.z; \
	    v=tria[w].v1; \
	    VERTEX_PIPELINE_ELSE(tria[w].sp1) \
        v=tria[w].v2; \
	    VERTEX_PIPELINE_ELSE(tria[w].sp2) \
        v=tria[w].v3; \
	    VERTEX_PIPELINE_ELSE(tria[w].sp3) \
        if (((tria[w].v1->flags + tria[w].v2->flags + tria[w].v3->flags + 1) & 3) !=0 ) \
		{ \
		  if (tria[w].materiale->flags & IS_TRASPARENT) \
		    *list_to_paint_trasparent++=&tria[w]; \
          else \
		    *tria[w].materiale->my_tria_list++=&tria[w]; \
		} \
	  } \
	} \
  } \

/*	  if (!triaobj_isclipped_bounding_inline(&vtmp, tria[w].radius*max_scale, telecamera)) \ */

extern int counter_for_resetting_vertex;
extern AD_Tria3D **list_to_paint_trasparent;  // lista dei triangoli trasparenti
extern HWND miawin;         // finestra principale; usata qui per le debug_error
extern FILE *fdeb;

// ---------------------------------------------------------
AD_Object3D::AD_Object3D()   // costruttore
{
   num_vertex3D=num_vertexUV=num_vertex2D=num_normals=0;
   num_tria=num_tria_envRGB=num_tria_RGB=num_tria_envmap=0;

   vertex3D=(AD_Vertex3D *)NULL;
   vertex2D=(AD_Vertex2D *)NULL;
   vertexUV=(AD_VectUV *)NULL;

   tria_envRGB=tria_RGB=tria_envmap=tria=(AD_Tria3D *)NULL;
   flare=(texture *)NULL;
   latoX=latoY=0;
   flare_scale_x=flare_scale_y=160.0f;

   father_name=name=(char *)NULL;
   father=(AD_Object3D *)NULL;
   have_childrens=0;

   num_omni=num_spot=0;

   matbase=(material *)NULL;
   vect_set(&accum_scale, 1, 1, 1);
   radius=0;
   
   for (int i=0; i<MAX_OSM_MODIFIERS; i++)
     OSMmods[i]=(AD_OSMObject *)NULL;
   
   num_OSMmods=0;
   bones_matrix=(AD_Matrix **)NULL;
   bones_matrix_rot=(AD_Matrix **)NULL;
   skin_modifier=(Skin_Bone **)NULL;
   skinned_object=(AD_Object3D *)NULL;
   mat_identity(&currentmatrix);
   mat_identity(&currentmatrix_rot);
   mat_identity(&inverse_rotmatrix);
   mat_identity(&incremental_rotmatrix);
}


int AD_Object3D::get_vertex_index_from_pointer (AD_Vertex3D *v)
{
  int h;

  for (h=0; h<num_vertex3D; h++)
  {
	if (&vertex3D[h] == v) return(h);
  }
  return(0);
}


void AD_Object3D::get_vertex_normal (int16 quale, AD_Vect3D *vnorm)
{
  int16 tt;
  AD_Vect3D somma;
  AD_Vertex3D *quale_ptr;

  vect_set(&somma, 0, 0, 0);
  quale_ptr=&vertex3D[quale];
  for (tt=0; tt<num_tria; tt++)
  {  
    if ((tria[tt].v1==quale_ptr) || (tria[tt].v2==quale_ptr) || (tria[tt].v3==quale_ptr))
        vect_add(&somma, &tria[tt].normal, &somma);
  }
  vect_auto_normalize(&somma);
  vect_copy(&somma, vnorm);
}


float fmax(float4 a, float4 b)
{
  if (a>=b) return a;
  else return b;
}


void AD_Object3D::init_normals(void)
// le normali dei triangoli devono essere gia' state calcolate
// vertici triangoli e smoothing groups devono già essare in memoria
{ 
   #define normal_err 0.0001f
   // indica l'errore massimo possibile x riciclare una normale: + e' alto il numero piu'
   // si perde qualita' visiva e si guadagna velocita': bisogna trovare un buon compromesso

  int *condivisi;
  int *smooth, *nosmooth; // array di triangoli condivisi da smoothare 
  AD_Vect3D *tempnormal;
  AD_Vect3D normadd;
  int num_condivisi, i, j, tr, num_smooth, num_nosmooth;
  int num_normal, norm;
  float maxerr, err;

  condivisi=new int[num_tria]; // nel caso peggiore tutti i triangoli sono condivisi 
  smooth=new int[num_tria];
  nosmooth=new int[num_tria];
  tempnormal=new AD_Vect3D[num_tria*3]; // nel caso peggiore ci sono 3 normali per triangolo
  num_normal=0;
      
  for (i=0; i<num_vertex3D; i++)
  {
	// trovo i triangoli che condividono il vertice i
	num_condivisi=0;
	for (j=0; j<num_tria; j++)
	{
	    if ((tria[j].v1==&vertex3D[i]) ||
			(tria[j].v2==&vertex3D[i]) ||
			(tria[j].v3==&vertex3D[i]))
	    {
		   condivisi[num_condivisi]=j;
		   num_condivisi++;          
	    }
	}

	while (num_condivisi>0)
	{
	   tr=condivisi[0]; // triangolo di riferimento
	   num_smooth=0;
	   num_nosmooth=0;
	   smooth[num_smooth]=tr;
	   num_smooth++;
	   for(j=1; j<num_condivisi; j++)
	   {
	     if ((triasmoothgroup[tr] & triasmoothgroup[condivisi[j]])!=0)
	     {
		    smooth[num_smooth]=condivisi[j];
		    num_smooth++;
	     }
		 else
		 {
		    nosmooth[num_nosmooth]=condivisi[j];
		    num_nosmooth++;
		 }
	   }

	   // cacolo la normale
	   vect_set(&normadd, 0, 0, 0);
	   for (j=0; j<num_smooth; j++)
          vect_add(&normadd, &tria[smooth[j]].normal, &normadd);
	   vect_auto_normalize(&normadd);

	   // cerco se ne esiste gia' una molto simile
	   j=0;
	   norm=-1;
	   maxerr=normal_err;
	   while (j < num_normal)
	   { 
	      // trovo l'errore massimo della normale j
	      err=fmax(fmax(fabsf(normadd.x-tempnormal[j].x),
			            fabsf(normadd.y-tempnormal[j].y)),
			       fabsf(normadd.z-tempnormal[j].z));
	      if (err < maxerr)
		  { 
              // trovata normale + precisa
			  maxerr=err;
			  norm=j;
		  }
	      j++;
	   }
	   if (norm==-1)
	   { // non trovata: la creo
	      vect_copy(&normadd, &tempnormal[num_normal]);
		  norm=num_normal;
	      num_normal++;
	   }

	   for (j=0; j<num_smooth; j++)
	   {
	     // la assegno al triangolo (cercando il vertice giusto)
	     if (tria[smooth[j]].v1==&vertex3D[i]) tria[smooth[j]].n1=&tempnormal[norm];
	     if (tria[smooth[j]].v2==&vertex3D[i]) tria[smooth[j]].n2=&tempnormal[norm];
	     if (tria[smooth[j]].v3==&vertex3D[i]) tria[smooth[j]].n3=&tempnormal[norm];
	   }
	      
	   // tolgo quelli assegnati e ricompatto gli altri
	   for (j=0; j<num_nosmooth; j++) condivisi[j]=nosmooth[j];
	   num_condivisi-=num_smooth;
	}
  }
 
  // copio la parte usata di tempnormal in normal
  normals = new AD_Vect3D[num_normal];
  num_normals=num_normal;
  for (j=0; j<num_normal; j++)
  {
	  vect_copy(&tempnormal[j], &normals[j]);
	  for (i=0; i<num_tria; i++)
	  {
		 if (tria[i].n1==&tempnormal[j]) tria[i].n1=&normals[j];
		 if (tria[i].n2==&tempnormal[j]) tria[i].n2=&normals[j];
		 if (tria[i].n3==&tempnormal[j]) tria[i].n3=&normals[j];
	  }
  }
  delete [] condivisi;
  delete [] smooth;
  delete [] nosmooth;
  delete [] tempnormal;
  delete [] triasmoothgroup;  // non servono piu'
}


void AD_Object3D::precalc_radius(void)
{
  int16 i;
  float4 aux=-1.0, mn;
  AD_Vect3D midp, paux;

  vect_set(&midp, 0, 0, 0);
  for (i=0; i<num_vertex3D; i++)
  {
	vect_add(&midp, &vertex3D[i].point, &midp);
  }
  vect_scale(&midp, 1.0f/(float)num_vertex3D, &midp);
  vect_copy(&midp, &mid_point);

  for (i=0; i<num_vertex3D; i++)
  {
	vect_sub(&vertex3D[i].point, &midp, &paux);
	mn=vect_length(&paux);
    if (mn>aux) aux=mn;
  }
  radius=aux;
}


void AD_Object3D::split_tria_list (void)
{
  int i, num_tria_else, j1, j2, j3, j4;
  AD_Tria3D *tria_aux;

  num_tria_envRGB=0;
  num_tria_RGB=0;
  num_tria_envmap=0;
  num_tria_else=0;

  for (i=0; i<num_tria; i++)
  {
	if ((tria[i].materiale->flags & (PAINT_ENVMAP | PAINT_LIGHT))
		==(PAINT_ENVMAP | PAINT_LIGHT)) num_tria_envRGB++;
	else 
	if (tria[i].materiale->flags & PAINT_ENVMAP) num_tria_envmap++;
	else
	if (tria[i].materiale->flags & PAINT_LIGHT) num_tria_RGB++;
	else num_tria_else++;
  }

  if (num_tria_envRGB>0) tria_envRGB=new AD_Tria3D[num_tria_envRGB];
  if (num_tria_RGB>0)    tria_RGB=new AD_Tria3D[num_tria_RGB];
  if (num_tria_envmap>0) tria_envmap=new AD_Tria3D[num_tria_envmap];
  if (num_tria_else>0) tria_aux=new AD_Tria3D[num_tria_else];

  j1=0;
  j2=0;
  j3=0;
  j4=0;
  for (i=0; i<num_tria; i++)
  {
	if ((tria[i].materiale->flags & (PAINT_ENVMAP | PAINT_LIGHT))
        ==(PAINT_ENVMAP | PAINT_LIGHT))
	{
	  tria_envRGB[j1]=tria[i];
	  j1++;
	}
	else
	if (tria[i].materiale->flags & PAINT_LIGHT)
	{
	  tria_RGB[j2]=tria[i];
	  j2++;
	}
	else
	if (tria[i].materiale->flags & PAINT_ENVMAP)
	{
	  tria_envmap[j3]=tria[i];
	  j3++;
	}
	else
	{
	  tria_aux[j4]=tria[i];
	  j4++;
	}
  }

  if (tria!=(AD_Tria3D *)NULL) delete [] tria;
  tria=tria_aux;
  num_tria=num_tria_else;
}
 

void AD_Object3D::init_tria(void)
{
  int i, j;
  AD_Tria3D taux;

  // PRECALCOLO DATI GEOMETRICI DEI TRIANGOLI (raggio, punto_medio, ecc)
  for (i=0; i<num_tria; i++)
  {
	if ((tria[i].v1==(AD_Vertex3D *)NULL) ||
	    (tria[i].v2==(AD_Vertex3D *)NULL) ||
	    (tria[i].v3==(AD_Vertex3D *)NULL))
	{ debug_error(miawin, "Lod error, NULL vertex reference"); }

    if (tria_init(&tria[i])==-1) return;
  }

  // ordino i triangoli per materiale usato
  for (i=0; i<num_tria-1; i++)
  {
	for (j=i+1; j<num_tria; j++)
	{
	  if (tria[i].materiale  < tria[j].materiale)
	  {
		taux=tria[i];
		tria[i]=tria[j];
		tria[j]=taux;
	  }
	}
  }
}


void AD_Object3D::init_vertex(void)
{
  int i, j;
  AD_VectUV *uv1, *uv2, *uv3;

  // se l'oggetto ha degli OSM o bonez allegate è inutile allocare il
  // triplo di punti2D e normali, perkè nel caso specifico gli
  // smoothing groups non venogno rispettati e c'è una corrispondenza
  // 1-1 tra vertice3D, normale e vertice2D
  if (
	  (num_OSMmods > 0) ||
	  (bones_matrix!=(AD_Matrix **)NULL) ||
	  (skin_modifier!=(Skin_Bone **)NULL)
     )
  {
     num_vertex2D=num_vertex3D;
     num_normals=num_vertex3D;
     vertex2D=new AD_Vertex2D[num_vertex2D];
	 normals=new AD_Vect3D[num_normals];
	 for (i=0; i<num_tria; i++)
	 {
       uv1=(AD_VectUV *)(tria[i].sp1);
       uv2=(AD_VectUV *)(tria[i].sp2);
       uv3=(AD_VectUV *)(tria[i].sp3);
       j=get_vertex_index_from_pointer(tria[i].v1);
	   tria[i].sp1=&vertex2D[j];
	   tria[i].n1=&normals[j];
       j=get_vertex_index_from_pointer(tria[i].v2);
	   tria[i].sp2=&vertex2D[j];
	   tria[i].n2=&normals[j];
       j=get_vertex_index_from_pointer(tria[i].v3);
	   tria[i].sp3=&vertex2D[j];
	   tria[i].n3=&normals[j];
	   if ((uv1) && (uv2) && (uv3) && (vertexUV))
	   {
          tria[i].sp1->u=uv1->u;
          tria[i].sp1->v=uv1->v;
          tria[i].sp2->u=uv2->u;
          tria[i].sp2->v=uv2->v;
          tria[i].sp3->u=uv3->u;
          tria[i].sp3->v=uv3->v;
	   }
	 }
	 for (i=0; i<num_normals; i++) get_vertex_normal(i, &normals[i]);
  }
  else
  {
     num_vertex2D=num_tria*3;
     vertex2D=new AD_Vertex2D[num_vertex2D];
	 for (i=0; i<num_tria; i++)
	 {
       uv1=(AD_VectUV *)(tria[i].sp1);
       uv2=(AD_VectUV *)(tria[i].sp2);
       uv3=(AD_VectUV *)(tria[i].sp3);
	   tria[i].sp1=&vertex2D[i*3];
       tria[i].sp2=&vertex2D[i*3+1];
       tria[i].sp3=&vertex2D[i*3+2];
	   if ((uv1) && (uv2) && (uv3) && (vertexUV))
	   {
	      tria[i].sp1->u=uv1->u;
	      tria[i].sp1->v=uv1->v;
	      tria[i].sp2->u=uv2->u;
	      tria[i].sp2->v=uv2->v;
	      tria[i].sp3->u=uv3->u;
	      tria[i].sp3->v=uv3->v;
		  // incorporiamo lo scaling UV
		  if (tria[i].materiale)
		  {
             tria[i].sp1->u*=tria[i].materiale->tile_u;
             tria[i].sp1->v*=tria[i].materiale->tile_v;
             tria[i].sp2->u*=tria[i].materiale->tile_u;
             tria[i].sp2->v*=tria[i].materiale->tile_v;
             tria[i].sp3->u*=tria[i].materiale->tile_u;
             tria[i].sp3->v*=tria[i].materiale->tile_v;
		  }
	   }
	 }
     init_normals(); // calcola e alloca l'array di normali tenendo conto
                     // degli smoothing groups; assegna in modo opportuno
                     // i campi n1, n2, n3 dei triangoli
  }

  // inizializzazione dei vertici (flag)
  for (i=0; i<num_vertex3D; i++) vertex3D[i].flags=0;
}


void AD_Object3D::init_texture_coordinate(void)
{
  typedef struct
  {
	int vUV;
	int tria;
  } UV_Duplicate;


  float4 minu=1E10, minv=1E10, addu, addv;
  int i;
//  int j, w;
//  material *k;
//  UV_Duplicate *UVdup;
//  int num_dup;


  if (vertexUV!=(AD_VectUV *)NULL)
  {
/*
    UVdup=new UV_Duplicate[num_tria*3];  // sovrastima massima
    num_dup=0;

    // --------------------------------------------------------
    // -----    SISTEMO I VERTICI TEXTURE DA DUPLICARE  -------
    // --------------------------------------------------------
    for (i=0; i<num_vertexUV; i++)
    {
	  w=-1;
	  for (j=0; ((j<num_tria) && (w==-1)); j++)
	  {
        if ( (tria[j].uv1==&vertexUV[i]) ||
		     (tria[j].uv2==&vertexUV[i]) ||
             (tria[j].uv3==&vertexUV[i]) )
		{
		  w=j;
		  k=tria[j].materiale;
		}
	  }

	  for (j=0; j<num_tria; j++)
	  {
        if (
		    (tria[j].materiale!=k) &&
		    ((tria[j].uv1==&vertexUV[i]) ||
		     (tria[j].uv2==&vertexUV[i]) ||
             (tria[j].uv3==&vertexUV[i]))
		   )
		{
		  // mi "segno" che del vertice i devo farne una copia
		  // ed assegnarla al triangolo j
		  UVdup[num_dup].vUV=i;
		  UVdup[num_dup].tria=j;
		  num_dup++;
		}
	  }
	}
    delete [] UVdup;
  */
   
    // ############  SISTEMA TUTTE LE COPPIE (u,v) A POSITIVE ############
    // ------------             MAPPING DI MAX          ------------------

    for (i=0; i<num_vertexUV; i++)
    {
       if (vertexUV[i].u<minu) minu=vertexUV[i].u;
       if (vertexUV[i].v<minv) minv=vertexUV[i].v;
    }

     // conto il numero di 1.0 da aggiungere per portare tutto a >0
     addu=addv=0.0;
     if ((minu<0) && (minu>-1.0))  addu=-1.0;
     if ((minu<0) && (minu<=-1.0)) addu=(float)floor(minu);
     if ((minv<0) && (minv>-1.0))  addv=-1.0;
     if ((minv<0) && (minv<=-1.0)) addv=(float)floor(minv);

     for (i=0; i<num_vertexUV; i++)
     {
	   vertexUV[i].u=(vertexUV[i].u-addu);
	   vertexUV[i].v=(vertexUV[i].v-addv);
     }
  }
  else
  {
  }
}


int AD_Object3D::init(void)
{
//  AD_Lod lodder;
  int k;
  AD_Quaternion q;
  AD_Matrix M;
  //AD_Vect3D v;
  float x1, y1, z1, x2, y2, z2;
  AD_OSMObject *OSMaux[100];

// ############  INIZIALIZZAZIONI DELLE KEYFRAMER  ############
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);
  
  // cistruisco le quantita' che mi permettono di costruire una
  // eventuale matrice di skin se questo oggetto fosse usato come
  // osso
  if (rotationtrack.numkey>0)
  {
     rotationtrack.get_data(0, &q);
     quat_rotquat_to_matrix(&q, &M);
	 mat_transpose(&M, &rot0);
  }else mat_identity(&rot0);
  if (scaletrack.numkey>0) scaletrack.get_data(0, &scale0);
  else vect_set(&scale0, 1, 1, 1);
  if (positiontrack.numkey>0) positiontrack.get_data(0, &pos0);
  else vect_set(&pos0, 0, 0, 0);

/*
  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0) &&
	  (father==(AD_Object3D *)NULL) && (have_childrens<=0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }
*/

  if (type==DUMMY)
  {
  // sistemo il flare connesso; devo creare i 2 triangoli,
  // i vertici 2D, 3D (solo per linkare i v2D), il materiale
  // trasparente
    if (flare!=(texture *)NULL)
	{
	  latoX=(float)flare->dimx;
	  latoY=(float)flare->dimy;

	  // setto le uv dei vertici per il flare; i vertici sono cosi'
	  // disposti
	  // 0----1
	  // |    |
	  // |    |
	  // 2----3

	  vertex2D[0].u=0.0f*TEXTURE_PRECISION;
	  vertex2D[0].v=0.0f*TEXTURE_PRECISION;
	  vertex2D[1].u=0.99f*TEXTURE_PRECISION;
	  vertex2D[1].v=0.0f*TEXTURE_PRECISION;
	  vertex2D[2].u=0.0f*TEXTURE_PRECISION;
	  vertex2D[2].v=0.99f*TEXTURE_PRECISION;
	  vertex2D[3].u=0.99f*TEXTURE_PRECISION;
	  vertex2D[3].v=0.99f*TEXTURE_PRECISION;

	
	  // i vertici 3D li setto sempre da non clippare visto
	  // che quando devo disegnare i flare sono sicuro che non
	  // devo clipparli e solo quando disegno li inserisco nella
	  // lista dei triangoli trasparenti
	  vertex3D[0].flags=0;
	  vertex3D[1].flags=0;
	  vertex3D[2].flags=0;
	  vertex3D[3].flags=0;
	
	  // setto il materiale
	  matflare.texture_ptr=flare;
	  matflare.flags=(0 | PAINT_TEXTURE | IS_TRASPARENT);
	  matflare.trasparencytype=MIXTYPE_ADD;
	  AddUpdate_Material(&matflare);

      // ATTENZIONE: non cambiare gli indici perche' a causa
	  // del settaggio di cull2D della scheda3D i triangoli
	  // devono essere in senso antiorario
	  // setto i triangoli
	  tria[0].materiale=&matflare;
	  tria[0].v1=&vertex3D[0];
	  tria[0].v2=&vertex3D[2];
	  tria[0].v3=&vertex3D[3];
	  tria[0].sp1=&vertex2D[0];
	  tria[0].sp2=&vertex2D[2];
	  tria[0].sp3=&vertex2D[3];
	  tria[1].materiale=&matflare;
	  tria[1].v1=&vertex3D[3];
	  tria[1].v2=&vertex3D[1];
	  tria[1].v3=&vertex3D[0];
	  tria[1].sp1=&vertex2D[3];
	  tria[1].sp2=&vertex2D[1];
	  tria[1].sp3=&vertex2D[0];
	}
	return(0);
  }
  if (type==BONE) return(0);

/* NON PIU' FATTIBILE A CAUSA DEGLI OSM
------------------------------------------
-----------------------------------------
  // OTTIMIZAZIONE !!!
  if ((rotationtrack.numkey==1) && (rotationtrack.rotazioni[0].posintrack==0) &&
	  (father==(AD_Object3D *)NULL) && (have_childrens<=0))
  {
	q=rotationtrack.rotazioni[0].rotquat;
    quat_rotquat_to_matrix(&q, &M);
	rotationtrack.numkey=0;
    mat_identity(&currentmatrix_rot);
	for (k=0; k<num_vertex; k++)
	{
	  mat_mulvect(&M, &points[k], &v);
	  vect_copy(&v, &points[k]);
	}
  }
  if ((scaletrack.numkey==1) && (scaletrack.posizioni[0].posintrack==0) &&
	  (father==(AD_Object3D *)NULL) && (have_childrens<=0))
  {
	for (k=0; k<num_vertex; k++)
	{
	  points[k].x*=scaletrack.posizioni[0].p.x;
	  points[k].y*=scaletrack.posizioni[0].p.y;
	  points[k].z*=scaletrack.posizioni[0].p.z;
	}
	scaletrack.numkey=0;
  }
*/

  // calcolo bounding box per poi settarla negli OSM
  x1=y1=z1=1E10;
  x2=y2=z2=-1E10;
  for (k=0; k<num_vertex3D; k++)
  {
	 if (vertex3D[k].point.x < x1) x1 = vertex3D[k].point.x;
	 if (vertex3D[k].point.y < y1) y1 = vertex3D[k].point.y;
	 if (vertex3D[k].point.z < z1) z1 = vertex3D[k].point.z;
	 if (vertex3D[k].point.x > x2) x2 = vertex3D[k].point.x;
	 if (vertex3D[k].point.y > y2) y2 = vertex3D[k].point.y;
	 if (vertex3D[k].point.z > z2) z2 = vertex3D[k].point.z;
  }

  // inverto l'ordine degli OSM perche' vanno applicati
  // al contrario
  for (k=0; k<(int)num_OSMmods; k++)
    OSMaux[k]=OSMmods[k];

  for (k=0; k<(int)num_OSMmods; k++)
    OSMmods[num_OSMmods-k-1]=OSMaux[k];

  // Inizializzazione OSM modifiers
  for (k=0; k<(int)num_OSMmods; k++)
  {
	OSMmods[k]->init();
	OSMmods[k]->set_bbox(x1, y1, z1, x2, y2, z2);
  }

  precalc_radius();  // precalcola raggio sfera circoscritta
  init_tria();       // calcola: normale, punto medio e raggio circoscritto
  init_texture_coordinate(); // rende positive le uv
  init_vertex();  // alloca e prepara le normali ai vertici tenendo
                  // conto degli smoothing groups; inoltre setta
                  // opportunamente i campi n1, n2, n3, sp1, sp2, sp3
                  // dei triangoli
  if (vertexUV!=(AD_VectUV *)NULL) delete [] vertexUV;
  
  if (bones_matrix!=(AD_Matrix **)NULL)
  for (k=0; k<num_vertex3D; k++)
  {
     // questo perchè in fase di lettura (UtilsA3D) i punti nello
	 // spazio delle bonez vengono messi in points_tr
	 vect_copy(&vertex3D[k].tpoint, &vertex3D[k].point);
  }


// NB: IL LOD VA INSERITO IN QUESTO PUNTO !!!!
// NE' PRIMA (perche' le uv ai vertici devono essere sistemate)
// NE' DOPO (perche' i tringoli devono eseere inizializzati
// tutti (anche quelli del modello semplificato) dopo)
// [ad]TURBO ROXXXXXXXXXXXXXXXXX   
/*
  lodder.apply_lod(vertex3D, num_vertex,
	                tria, num_tria,
					70,
					&vertex3D, &num_vertex,
					&tria, &num_tria,
					GEOMETRIC_LOD);
*/
  split_tria_list();
  return(0);
}


void AD_Object3D::build_objectmatrix (float4 framepos)
// costruisce la matrice di trasformazione, che servira' poi per trasformare
// i vertici dell'oggetto;
{
   AD_Vect3D postmp, stmp, ptmp;
   AD_Quaternion objrot;
   AD_Matrix posttrans, scaling, maux, pretrans;

   accum_scale.x=accum_scale.y=accum_scale.z=1.0f;
   mat_identity(&currentmatrix_rot);

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
   } else mat_identity(&scaling);
   if (positiontrack.numkey>0) positiontrack.get_data(framepos, &currentpos);
   mat_setmatrix_of_pretraslation(&posttrans, &currentpos);

   if (type==BONE)
   {
      mat_mul(&scaling, &currentmatrix_rot, &maux);
      mat_mul(&posttrans, &maux, &currentmatrix);
   }
   else 
   {
      mat_mul(&currentmatrix_rot, &scaling, &maux);
      mat_mul(&posttrans, &maux, &currentmatrix);
   }

   // per le bone si costruiscono gia' le matrici comprese
   // di padre; per oggetti non bone invece si costruiscono
   // matrici relative e quindi devo 'accodarci' pure
   // la trasformazione del padre
   if (type!=BONE)
   {
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
   }
   mat_transpose(&currentmatrix_rot, &inverse_rotmatrix);

   if (skinned_object!=(AD_Object3D *)NULL)
   {
      if (father==(AD_Object3D *)NULL)
	  {
		  vect_copy(&currentpos, &ptmp);
		  vect_neg(&ptmp, &ptmp);
          mat_setmatrix_of_pretraslation(&pretrans, &ptmp);
          vect_neg(&ptmp, &ptmp);
          mat_setmatrix_of_pretraslation(&posttrans, &ptmp);

          mat_copy(&pretrans, &maux);

		  mat_mul(&currentmatrix_rot, &maux, &maux);
          mat_mul(&rot0, &maux, &maux);
          mat_mul(&scaling, &maux, &maux);
          mat_setmatrix_of_scaling(&scaling, 1.0f/scale0.x, 1.0f/scale0.y, 1.0f/scale0.z);
          mat_mul(&scaling, &maux, &maux);
		  //vect_sub(&currentpos, &pos0);
		  mat_mul(&posttrans, &maux, &skin_matrix);
	  }
	  else
	  {
		  /*
		  if (positiontrack.numkey>0) positiontrack.get_data(framepos, &ptmp);
		  vect_neg(&ptmp, &ptmp);
          mat_setmatrix_of_pretraslation(&pretrans, &ptmp);
          vect_neg(&ptmp, &ptmp);
          mat_setmatrix_of_pretraslation(&posttrans, &ptmp);

          mat_mul(&father->inverse_rotmatrix, &currentmatrix_rot, &maux);
		  maux.a[0][3]=maux.a[1][3]=maux.a[2][3]=0;
		  mat_mul(&maux, &pretrans, &maux);
          mat_mul(&scaling, &maux, &maux);
          mat_mul(&posttrans, &maux, &skin_matrix);

	      mat_mul(&father->skin_matrix, &skin_matrix, &skin_matrix);
		  */
          mat_copy(&father->skin_matrix, &skin_matrix);
	  }
   }
}


void AD_Object3D::paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight)
{
  int16 w, wl;
  float4 invz, cosalpha, nlX, nlY, max_scale, dist;
  AD_Vect3D inv_accum_scale;
  AD_Vertex3D *v;
  AD_Matrix matrixrot_all, matrixtransform_all;
  AD_Matrix matrixtall_clip;
  AD_Vect3D light_vertex_vector, camerapos_inobject, vtmp, paux;
//  AD_Vertex2D *v2D=vertex2D;
//  AD_Vect3D *v3D=points_tr;


  build_objectmatrix(pos);
  if (type==BONE)
  {
	 return;
  }
  else
  if (type==DUMMY)
  {
    // mat_mulvect(&currentmatrix, &currentpos, &paux);
    mat_mulvect(&telecamera->currentmatrix, &currentpos, &vtmp);
    if ( (flare!=(texture *)NULL) &&
	     (vtmp.z > telecamera->znear) &&
	     (vtmp.z < telecamera->zfar) )
	{
      // proietto in 2D su schermo la luce (flare)
	  invz=1.0f/vtmp.z;
      vtmp.x=telecamera->prospettivaX*(vtmp.x*invz) + (telecamera->screenX);
      vtmp.y=(telecamera->screenY) - telecamera->prospettivaY*(vtmp.y*invz);

	  nlX=(flare_scale_x*latoX)/(vtmp.z - telecamera->znear + 1.0f);
      nlY=(flare_scale_y*latoY)/(vtmp.z - telecamera->znear + 1.0f);

	  // mi calcolo i punti dei 2 triangoli
      vertex2D[0].xs=vtmp.x - (nlX/2.0f);
	  if (vertex2D[0].xs > telecamera->screenX*2) return;
      vertex2D[0].ys=vtmp.y - (nlY/2.0f);
	  if (vertex2D[0].ys > telecamera->screenY*2) return;
      vertex2D[0].dist=invz;
	  vertex2D[0].z=vtmp.z/zfar;

      vertex2D[1].xs=vtmp.x + (nlX/2.0f);
	  if (vertex2D[1].xs < 0) return;
      vertex2D[1].ys=vtmp.y - (nlY/2.0f);
      vertex2D[1].dist=invz;
	  vertex2D[1].z=vtmp.z/zfar;

      vertex2D[2].xs=vtmp.x - (nlX/2.0f);
      vertex2D[2].ys=vtmp.y + (nlY/2.0f);
	  if (vertex2D[2].ys < 0) return;
      vertex2D[2].dist=invz;
	  vertex2D[2].z=vtmp.z/zfar;

      vertex2D[3].xs=vtmp.x + (nlX/2.0f);
      vertex2D[3].ys=vtmp.y + (nlY/2.0f);
      vertex2D[3].dist=invz;
	  vertex2D[3].z=vtmp.z/zfar;

      tria[0].mid_z=vtmp.z;
      tria[1].mid_z=vtmp.z;
	  (*list_to_paint_trasparent)=&tria[0];
      list_to_paint_trasparent++;
	  (*list_to_paint_trasparent)=&tria[1];
      list_to_paint_trasparent++;
	}
    return;
  }

  
  if ((bones_matrix!=(AD_Matrix **)NULL) ||
	  (skin_modifier!=(Skin_Bone **)NULL))
  {
	  paint_bones(pos, telecamera, omnilight);
	  return;
  }
  else
  if (num_OSMmods > 0)
  {
	  paint_modifiers(pos, telecamera, omnilight);
	  return;
  }


  max_scale=fabsf(accum_scale.x);
  if (fabsf(accum_scale.y) > max_scale)
	 max_scale=fabsf(accum_scale.y);
  if (fabsf(accum_scale.z) > max_scale)
	 max_scale=fabsf(accum_scale.z);
  inv_accum_scale.x=1.0f/accum_scale.x;
  inv_accum_scale.y=1.0f/accum_scale.y;
  inv_accum_scale.z=1.0f/accum_scale.z;



  // *****************************************************
  // ***   CASO DI OGGETTI NORMALE (mesh triangolari)  ***
  // *****************************************************

  // test di bounding clip 3d sull'oggetto intero
  mat_mulvect(&currentmatrix, &mid_point, &paux);
  mat_mulvect(&telecamera->currentmatrix, &paux, &vtmp);
  if (triaobj_isclipped_bounding(&vtmp, radius*max_scale, telecamera)) return;

  // si porta la camera nello spazio oggetto
  vect_sub(&telecamera->currentpos, &currentpos, &vtmp);
  vtmp.x*=inv_accum_scale.x;
  vtmp.y*=inv_accum_scale.y;
  vtmp.z*=inv_accum_scale.z;
  mat_mulvectaffine(&inverse_rotmatrix, &vtmp, &camerapos_inobject);

  // si calcolano le matrici di trasformazione totali
  mat_mulaffine(&telecamera->currentmatrix_rot, &currentmatrix_rot, &matrixrot_all);
  mat_mul(&telecamera->currentmatrix, &currentmatrix, &matrixtall_clip);

  // calcolo della matrice di trasformazione con inclusi i
  // fattori di aspect ratio;
  mat_copy(&matrixtall_clip, &matrixtransform_all);
  for (w=0; w<4; w++)
  {
    matrixtransform_all.a[0][w]=matrixtransform_all.a[0][w]*telecamera->prospettivaX;
    matrixtransform_all.a[1][w]=matrixtransform_all.a[1][w]*telecamera->prospettivaY;
  }

  // si portano le luci nello spazio oggetto
  for (w=0; w<num_omni; w++)
  {
    vect_sub(&omnilight[w].currentpos, &currentpos, &vtmp);
    vtmp.x*=inv_accum_scale.x;
    vtmp.y*=inv_accum_scale.y;
    vtmp.z*=inv_accum_scale.z;
	mat_mulvectaffine(&inverse_rotmatrix, &vtmp, &omnilight[w].currentpos_inobject);
  }
  TRIA_PIPELINE_ENVRGB
  TRIA_PIPELINE_RGB
  TRIA_PIPELINE_ENVMAP
  TRIA_PIPELINE_ELSE
}



// ####################     residui utili  ############################àà
/*
	   for (wl=0; wl<num_spot; wl++)
	   {
	     trv3=trv2-spotlight[wl].currentpos_inobject; trv3.normalize();
	     cosalpha=spotlight[wl].view_normalized_vector*=trv3;
	     if ((cosalpha>0.0) && (cosalpha>spotlight[wl].cos_currentfalloff))
	     {
		light_vertex_vector=spotlight[wl].currentpos_inobject-trv2;
		light_vertex_vector.normalize();
		cos2=(trv.normale*=light_vertex_vector);
		if (cos2>0.0) luce_accumulata=luce_accumulata+(cosalpha*spotlight[wl].currentcolor);
	     }
	   }
*/

   // **************************************************************
   // Pipeline a livello di triangolo ma che non hanno ancora le
   // corrispettive a vertice
   // **************************************************************
#define TRIA_PIPELINE_ENVRGB_MODIFIERS \
  for (w=0; w<num_tria_envRGB; w++) \
  { \
     vect_sub(tria_envRGB[w].v1->tpoint, tria_envRGB[w].v2->tpoint, &p1); \
     vect_sub(tria_envRGB[w].v3->tpoint, tria_envRGB[w].v2->tpoint, &p2); \
     vect_cross(&p1, &p2, &tria_envRGB[w].normal); \
     vect_table_auto_normalize(&tria_envRGB[w].normal); \
	 tria_envRGB[w].v1->normal->x+=tria_envRGB[w].normal.x; \
	 tria_envRGB[w].v1->normal->y+=tria_envRGB[w].normal.y; \
	 tria_envRGB[w].v1->normal->z+=tria_envRGB[w].normal.z; \
	 tria_envRGB[w].v2->normal->x+=tria_envRGB[w].normal.x; \
	 tria_envRGB[w].v2->normal->y+=tria_envRGB[w].normal.y; \
	 tria_envRGB[w].v2->normal->z+=tria_envRGB[w].normal.z; \
	 tria_envRGB[w].v3->normal->x+=tria_envRGB[w].normal.x; \
	 tria_envRGB[w].v3->normal->y+=tria_envRGB[w].normal.y; \
	 tria_envRGB[w].v3->normal->z+=tria_envRGB[w].normal.z; \
	 *tria_envRGB[w].materiale->my_tria_list++=&tria_envRGB[w]; \
  } \


#define TRIA_PIPELINE_RGB_MODIFIERS \
  for (w=0; w<num_tria_RGB; w++) \
  { \
     vect_sub(tria_RGB[w].v1->tpoint, tria_RGB[w].v2->tpoint, &p1); \
     vect_sub(tria_RGB[w].v3->tpoint, tria_RGB[w].v2->tpoint, &p2); \
     vect_cross(&p1, &p2, &tria_envRGB[w].normal); \
     vect_table_auto_normalize(&tria_envRGB[w].normal); \
	 tria_RGB[w].v1->normal->x+=tria_RGB[w].normal.x; \
	 tria_RGB[w].v1->normal->y+=tria_RGB[w].normal.y; \
	 tria_RGB[w].v1->normal->z+=tria_RGB[w].normal.z; \
	 tria_RGB[w].v2->normal->x+=tria_RGB[w].normal.x; \
	 tria_RGB[w].v2->normal->y+=tria_RGB[w].normal.y; \
	 tria_RGB[w].v2->normal->z+=tria_RGB[w].normal.z; \
	 tria_RGB[w].v3->normal->x+=tria_RGB[w].normal.x; \
	 tria_RGB[w].v3->normal->y+=tria_RGB[w].normal.y; \
	 tria_RGB[w].v3->normal->z+=tria_RGB[w].normal.z; \
	 *tria_RGB[w].materiale->my_tria_list++=&tria_RGB[w]; \
  } \


// *********************************************************
// *************    ENVMAP    ******************************
// *********************************************************

#define VERTEX_PIPELINE_ENVMAP_MODIFIERS(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     mat_mulvect(&matrixtransform_all, &v->tpoint, &p); \
	 vect_copy(&p, &v->tpoint); \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       p2D->xs=(v->tpoint.x*invz) + screen_Xadd; \
       p2D->ys=screen_Yadd - (v->tpoint.y*invz); \
	   p2D->z=(v->tpoint.z-znear)*inv_zfar_znear; \
	   p2D->dist=invz; \
     } \
	 else v->flags=v->flags | 1; \
     mat_mulvectenv(&matrixrot_all, n, &vtmp); \
     p2D->envu=(vtmp.x*ENVMAP_PRECISION+ENVMAP_PRECISION); \
     p2D->envv=(vtmp.y*ENVMAP_PRECISION+ENVMAP_PRECISION); \
  } \


#define TRIA_PIPELINE_ENVMAP_MODIFIERS \
  for (w=0; w<num_tria_envmap; w++) \
  { \
     vect_sub(&tria_envmap[w].v1->tpoint, &tria_envmap[w].v2->tpoint, &p1); \
     vect_sub(&tria_envmap[w].v3->tpoint, &tria_envmap[w].v2->tpoint, &p2); \
     vect_cross(&p1, &p2, &tria_envmap[w].normal); \
	 tria_envmap[w].n1->x+=tria_envmap[w].normal.x; \
	 tria_envmap[w].n1->y+=tria_envmap[w].normal.y; \
	 tria_envmap[w].n1->z+=tria_envmap[w].normal.z; \
	 tria_envmap[w].n2->x+=tria_envmap[w].normal.x; \
	 tria_envmap[w].n2->y+=tria_envmap[w].normal.y; \
	 tria_envmap[w].n2->z+=tria_envmap[w].normal.z; \
	 tria_envmap[w].n3->x+=tria_envmap[w].normal.x; \
	 tria_envmap[w].n3->y+=tria_envmap[w].normal.y; \
	 tria_envmap[w].n3->z+=tria_envmap[w].normal.z; \
  } \
  for (w=0; w<num_normals; w++) \
     vect_table_auto_normalize(&normals[w]); \
  for (w=0; w<num_tria_envmap; w++) \
  { \
  	    v=tria_envmap[w].v1; \
	    VERTEX_PIPELINE_ENVMAP_MODIFIERS(tria_envmap[w].n1, tria_envmap[w].sp1) \
        v=tria_envmap[w].v2; \
	    VERTEX_PIPELINE_ENVMAP_MODIFIERS(tria_envmap[w].n2, tria_envmap[w].sp2) \
        v=tria_envmap[w].v3; \
	    VERTEX_PIPELINE_ENVMAP_MODIFIERS(tria_envmap[w].n3, tria_envmap[w].sp3) \
        if (((tria_envmap[w].v1->flags + tria_envmap[w].v2->flags + tria_envmap[w].v3->flags + 1) & 3) !=0 ) \
		{ \
		  if (tria_envmap[w].materiale->flags & IS_TRASPARENT) \
		    *list_to_paint_trasparent++=&tria_envmap[w]; \
          else \
		    *tria_envmap[w].materiale->my_tria_list++=&tria_envmap[w]; \
        } \
  } \



// *********************************************************
// *************    ELSE    ********************************
// *********************************************************

#define VERTEX_PIPELINE_ELSE_MODIFIERS(p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     mat_mulvect(&matrixtransform_all, &v->tpoint, &p); \
	 vect_copy(&p, &v->tpoint); \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       p2D->xs=(v->tpoint.x*invz) + screen_Xadd; \
       p2D->ys=screen_Yadd - (v->tpoint.y*invz); \
	   p2D->z=(v->tpoint.z-znear)*inv_zfar_znear; \
       p2D->dist=invz; \
     } \
	 else v->flags=v->flags | 1; \
  } \


// NB: finche' non fixo il culling calcolare normali del triangolo
// nonche' punto medi e raggio e' inutile
#define TRIA_PIPELINE_ELSE_MODIFIERS \
  for (w=0; w<num_tria; w++) \
  { \
	 { \
	    v=tria[w].v1; \
	    VERTEX_PIPELINE_ELSE_MODIFIERS(tria[w].sp1) \
        v=tria[w].v2; \
	    VERTEX_PIPELINE_ELSE_MODIFIERS(tria[w].sp2) \
        v=tria[w].v3; \
	    VERTEX_PIPELINE_ELSE_MODIFIERS(tria[w].sp3) \
        if (((tria[w].v1->flags + tria[w].v2->flags + tria[w].v3->flags + 1) & 3) !=0 ) \
		{ \
		  if (tria[w].materiale->flags & IS_TRASPARENT) \
		    *list_to_paint_trasparent++=&tria[w]; \
          else \
		    *tria[w].materiale->my_tria_list++=&tria[w]; \
        } \
     } \
 } \

/*
  vect_sub(tria[w].v1->tpoint, tria[w].v2->tpoint, &p1); \
  vect_sub(tria[w].v3->tpoint, tria[w].v2->tpoint, &p2); \
  vect_cross(&p1, &p2, &tria[w].normal); \
  vect_normalize(&tria[w].normal); \
  vect_copy(tria[w].v1->tpoint, &tria[w].midpoint); \
  vect_add(&tria[w].midpoint, tria[w].v2->tpoint, &tria[w].midpoint); \
  vect_add(&tria[w].midpoint, tria[w].v3->tpoint, &tria[w].midpoint); \
  vect_scale(&tria[w].midpoint, 1.0f/3.0f, &tria[w].midpoint); \
  vect_sub_inline(&camerapos_inobject, &tria[w].midpoint, &vtmp); \
  if ((tria[w].materiale->double_facing) || (vect_dot(&tria[w].normal, &vtmp)>CUL_EPS)) \
*/



void AD_Object3D::paint_modifiers(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight)
{
  int i, j, w;
  AD_Matrix matrixrot_all, matrixtransform_all;
  AD_Matrix matrixtall_clip;
  AD_Vect3D p1, p2, vtmp, p;
  AD_Vertex3D *v;
  float invz;
  // float x1, y1, z1, x2, y2, z2;

  // calcolo delle matrici di traspfromazione tra spazi
  mat_mulaffine(&telecamera->currentmatrix_rot, &currentmatrix_rot, &matrixrot_all);
  mat_mul(&telecamera->currentmatrix, &currentmatrix, &matrixtall_clip);

  // calcolo della matrice di trasformazione con inclusi i
  // fattori di aspect ratio;
  mat_copy(&matrixtall_clip, &matrixtransform_all);
  for (w=0; w<4; w++)
  {
    matrixtransform_all.a[0][w]=matrixtransform_all.a[0][w]*telecamera->prospettivaX;
    matrixtransform_all.a[1][w]=matrixtransform_all.a[1][w]*telecamera->prospettivaY;
  }

/*
  for (j=0; j<num_OSMmods; j++)
  {
//     if (j>0)
//       OSMmods[j]->set_bbox(x1, y1, z1, x2, y2, z2);
	 OSMmods[j]->update(pos);

	 // calcolo bounding box per poi settarla negli OSM
     x1=y1=z1=1E10;
     x2=y2=z2=-1E10;
     for (i=0; i<num_vertex; i++)
	 {
       OSMmods[j]->Map(&points_tr[i], &points_tr[i]);
	   if (points_tr[i].x < x1) x1 = points_tr[i].x;
	   if (points_tr[i].y < y1) y1 = points_tr[i].y;
	   if (points_tr[i].z < z1) z1 = points_tr[i].z;
	   if (points_tr[i].x > x2) x2 = points_tr[i].x;
	   if (points_tr[i].y > y2) y2 = points_tr[i].y;
	   if (points_tr[i].z > z2) z2 = points_tr[i].z;
	 }
  }
*/
  for (j=0; j<num_OSMmods; j++)
	 OSMmods[j]->update(pos);

  for (i=0; i<num_vertex3D; i++)
  {
     // trasformazione tramite OSM di tutti i vertici e
     // salvataggio dentro points_tr (i points originali devono
     // essere preservati)
     vect_copy(&vertex3D[i].point, &vertex3D[i].tpoint);
	 vertex3D[i].flags=0;
	 vect_set(&normals[i], 0, 0, 0);
     for (j=0; j<num_OSMmods; j++)
	 {
       OSMmods[j]->Map(&vertex3D[i].tpoint, &vertex3D[i].tpoint);
	 }
  }

//  TRIA_PIPELINE_ENVRGB_MODIFIERS
//  TRIA_PIPELINE_RGB_MODIFIERS
  TRIA_PIPELINE_ENVMAP_MODIFIERS
  TRIA_PIPELINE_ELSE_MODIFIERS
}


void AD_Object3D::paint_bones(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight)
{
  int w, wl, i;
  AD_Vect3D p1, p2, vtmp, p3;
  AD_Vect3D light_vertex_vector;
  float invz, cosalpha, dist;
  Skin_Bone *b;
  AD_Matrix mat_obj_cam;


  // si portano le luci nello spazio della telecamera
  for (w=0; w<num_omni; w++)
  {
	mat_mulvect(&telecamera->currentmatrix,
		        &omnilight[w].currentpos,
				&omnilight[w].currentpos_inobject);
  }

  if ((bones_matrix!=(AD_Matrix **)NULL) &&
	  (skin_modifier==(Skin_Bone **)NULL))
  {
    // calcolo matrice di trasformazione dell'oggetto
    for (w=0; w<num_vertex3D; w++)
	{
       vertex3D[w].flags=counter_for_resetting_vertex;
	   mat_mulvect(bones_matrix[w], &vertex3D[w].point, &p1);
	   mat_mulvect(&telecamera->currentmatrix, &p1, &vertex3D[w].tpoint);
       if (vertex3D[w].tpoint.z > znear)
	   {
         invz=1.0f/vertex3D[w].tpoint.z;
         vertex2D[w].xs=telecamera->prospettivaX*(vertex3D[w].tpoint.x*invz) + screen_Xadd;
         vertex2D[w].ys=screen_Yadd - telecamera->prospettivaY*(vertex3D[w].tpoint.y*invz);
	     vertex2D[w].z=(vertex3D[w].tpoint.z-znear)*inv_zfar_znear;
         vertex2D[w].dist=invz;
	   }
	   else vertex3D[w].flags|=1;
       vtmp.x=vtmp.y=vtmp.z=5;
       for (wl=0; wl<num_omni; wl++)
	   {
          vect_sub_inline(&omnilight[wl].currentpos, &p1, &light_vertex_vector);
          mat_mulvectenv(bones_matrix_rot[w], &normals[w], &p2);
          cosalpha=vect_dot(&p2, &light_vertex_vector);
          //       cosalpha=vect_dot(&normals[w], &light_vertex_vector);
          if (is_float_positive(cosalpha))
		  {
		      dist=1.0f/vect_fast_lenght(&light_vertex_vector);
	          cosalpha*=dist;
              vtmp.x+=cosalpha*omnilight[wl].currentcolor.x;
              vtmp.y+=cosalpha*omnilight[wl].currentcolor.y;
		      vtmp.z+=cosalpha*omnilight[wl].currentcolor.z;
		  }
	   }
	   if (FP_BITS(vtmp.x)>FP_BITS(RGB_MAXVALUE)) FP_BITS(vertex2D[w].R)=FP_BITS(RGB_MAXVALUE);
	   else FP_BITS(vertex2D[w].R)=FP_BITS(vtmp.x);
	   if (FP_BITS(vtmp.y)>FP_BITS(RGB_MAXVALUE)) FP_BITS(vertex2D[w].G)=FP_BITS(RGB_MAXVALUE);
	   else FP_BITS(vertex2D[w].G)=FP_BITS(vtmp.y);
	   if (FP_BITS(vtmp.z)>FP_BITS(RGB_MAXVALUE)) FP_BITS(vertex2D[w].B)=FP_BITS(RGB_MAXVALUE);
	   else FP_BITS(vertex2D[w].B)=FP_BITS(vtmp.z);
	}

    for (w=0; w<num_tria; w++)
	{
       if (((tria[w].v1->flags + tria[w].v2->flags + tria[w].v3->flags + 1) & 3) !=0 )
	   {
	      if (tria[w].materiale->flags & IS_TRASPARENT)
		      *list_to_paint_trasparent++=&tria[w];
          else  *tria[w].materiale->my_tria_list++=&tria[w];
	   }
	}
    for (w=0; w<num_tria_RGB; w++)
	{
       if (((tria_RGB[w].v1->flags + tria_RGB[w].v2->flags + tria_RGB[w].v3->flags + 1) & 3) !=0 )
	   {
	      if (tria_RGB[w].materiale->flags & IS_TRASPARENT)
		      *list_to_paint_trasparent++=&tria_RGB[w];
          else  *tria_RGB[w].materiale->my_tria_list++=&tria_RGB[w];
	   }
	}
  }
  else
  if ((bones_matrix==(AD_Matrix **)NULL) &&
	  (skin_modifier!=(Skin_Bone **)NULL))
  {
    mat_mul(&telecamera->currentmatrix, &currentmatrix, &mat_obj_cam);
    for (w=0; w<num_vertex3D; w++)
	{
       vertex3D[w].flags=counter_for_resetting_vertex;
	   b=skin_modifier[w];
	   i=0;
	   vect_set(&p3, 0, 0, 0);
	   mat_mulvect(&currentmatrix, &vertex3D[w].point, &p1);
	   while (b[i].skin_matrix!=(AD_Matrix *)NULL)
	   {
	      mat_mulvect(b[i].skin_matrix, &p1, &p2);
		  p2.x*=b[i].weight;
		  p2.y*=b[i].weight;
		  p2.z*=b[i].weight;
		  vect_add(&p3, &p2, &p3);
		  i++;
	   }
       mat_mulvect(&telecamera->currentmatrix, &p3, &vertex3D[w].tpoint);
   
       if (vertex3D[w].tpoint.z > znear)
	   {
         invz=1.0f/vertex3D[w].tpoint.z;
         vertex2D[w].xs=telecamera->prospettivaX*(vertex3D[w].tpoint.x*invz) + screen_Xadd;
         vertex2D[w].ys=screen_Yadd - telecamera->prospettivaY*(vertex3D[w].tpoint.y*invz);
	     vertex2D[w].z=(vertex3D[w].tpoint.z-znear)*inv_zfar_znear;
	     vertex2D[w].dist=invz;
	     vertex2D[w].R=vertex2D[w].z*255;
	     vertex2D[w].G=vertex2D[w].z*255;
	     vertex2D[w].B=vertex2D[w].z*255;
	   }
	   else vertex3D[w].flags|=1;
	}

    for (w=0; w<num_tria; w++)
	{
       if (((tria[w].v1->flags + tria[w].v2->flags + tria[w].v3->flags + 1) & 3) !=0 )
	   {
	      if (tria[w].materiale->flags & IS_TRASPARENT)
		      *list_to_paint_trasparent++=&tria[w];
          else  *tria[w].materiale->my_tria_list++=&tria[w];
	   }
	}
    for (w=0; w<num_tria_RGB; w++)
	{
       if (((tria_RGB[w].v1->flags + tria_RGB[w].v2->flags + tria_RGB[w].v3->flags + 1) & 3) !=0 )
	   {
	      if (tria_RGB[w].materiale->flags & IS_TRASPARENT)
		      *list_to_paint_trasparent++=&tria_RGB[w];
          else  *tria_RGB[w].materiale->my_tria_list++=&tria_RGB[w];
	   }
	}
  }
}
