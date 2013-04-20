// Classe che gestisce un oggetto fatto da pezze di Bezier
// importato dal 3DSMax 3.0; non riadatto ilc odice di
// bezier.cpp in quanto il max contempla pezze bicubiche,
// quindi posso utilizzare alcune ottimizzazioni aggiuntive
// by [AD]Turbo

#include <windows.h>
#include <math.h>
#include "..\patch.h"
#include "..\..\Math\fastmath.h"
#include "..\..\ADrender.h"

extern int counter_for_resetting_vertex;
extern AD_Tria3D **list_to_paint_trasparent;  // lista dei triangoli trasparenti
extern HWND miawin;         // finestra principale; usata qui per le debug_error
extern FILE *fdeb;
extern float znear, rcpznear, screen_Xadd, screen_Yadd;
extern float inv_zfar_znear;
extern float CUL_EPS;


#define VERTEX_PIPELINE_ENVRGB(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       p2D->xs=(v->tpoint.x*invz) + screen_Xadd; \
       p2D->ys=screen_Yadd - (v->tpoint.y*invz); \
	   p2D->z=(v->tpoint.z-znear)*inv_zfar_znear; \
       p2D->dist=invz; \
     } \
	 else v->flags=v->flags | 1; \
     p2D->envu=(n->x*ENVMAP_PRECISION+ENVMAP_PRECISION); \
     p2D->envv=(n->y*ENVMAP_PRECISION+ENVMAP_PRECISION); \
     vtmp.x=vtmp.y=vtmp.z=5; \
     vaux.x=v->tpoint.x*inv_prospX; \
     vaux.y=v->tpoint.y*inv_prospY; \
	 vaux.z=v->tpoint.z; \
     for (wl=0; wl<num_omni; wl++) \
     { \
       vect_sub_inline(&omnilight[wl].currentpos_inobject, &vaux, &light_vertex_vector); \
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
  } \


#define VERTEX_PIPELINE_ENVRGB_SWAP(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       p2D->xs=(v->tpoint.x*invz) + screen_Xadd; \
       p2D->ys=screen_Yadd - (v->tpoint.y*invz); \
	   p2D->z=(v->tpoint.z-znear)*inv_zfar_znear; \
       p2D->dist=invz; \
     } \
	 else v->flags=v->flags | 1; \
     p2D->envu=(n->x*ENVMAP_PRECISION+ENVMAP_PRECISION); \
     p2D->envv=(n->y*ENVMAP_PRECISION+ENVMAP_PRECISION); \
     vtmp.x=vtmp.y=vtmp.z=5; \
     vaux.x=v->tpoint.x*inv_prospX; \
     vaux.y=v->tpoint.y*inv_prospY; \
	 vaux.z=v->tpoint.z; \
     for (wl=0; wl<num_omni; wl++) \
     { \
       vect_sub_inline(&vaux, &omnilight[wl].currentpos_inobject, &light_vertex_vector); \
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
  } \


#define VERTEX_PIPELINE_RGB_SWAP(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       p2D->xs=(v->tpoint.x*invz) + screen_Xadd; \
       p2D->ys=screen_Yadd - (v->tpoint.y*invz); \
	   p2D->z=(v->tpoint.z-znear)*inv_zfar_znear; \
       p2D->dist=invz; \
     } \
	 else v->flags=v->flags | 1; \
     vtmp.x=vtmp.y=vtmp.z=1; \
     vaux.x=v->tpoint.x*inv_prospX; \
     vaux.y=v->tpoint.y*inv_prospY; \
	 vaux.z=v->tpoint.z; \
     for (wl=0; wl<num_omni; wl++) \
     { \
       vect_sub_inline(&vaux, &omnilight[wl].currentpos_inobject, &light_vertex_vector); \
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
  } \


#define VERTEX_PIPELINE_RGB(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       p2D->xs=(v->tpoint.x*invz) + screen_Xadd; \
       p2D->ys=screen_Yadd - (v->tpoint.y*invz); \
	   p2D->z=(v->tpoint.z-znear)*inv_zfar_znear; \
       p2D->dist=invz; \
     } \
	 else v->flags=v->flags | 1; \
     vtmp.x=vtmp.y=vtmp.z=1; \
     vaux.x=v->tpoint.x*inv_prospX; \
     vaux.y=v->tpoint.y*inv_prospY; \
	 vaux.z=v->tpoint.z; \
     for (wl=0; wl<num_omni; wl++) \
     { \
       vect_sub_inline(&omnilight[wl].currentpos_inobject, &vaux, &light_vertex_vector); \
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
  } \


#define VERTEX_PIPELINE_ENVMAP(n, p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
     if (v->tpoint.z > znear) \
	 { \
       invz=1.0f/v->tpoint.z; \
       p2D->xs=(v->tpoint.x*invz) + screen_Xadd; \
       p2D->ys=screen_Yadd - (v->tpoint.y*invz); \
	   p2D->z=(v->tpoint.z-znear)*inv_zfar_znear; \
       p2D->dist=invz; \
     } \
	 else v->flags=v->flags | 1; \
     p2D->envu=(n->x*ENVMAP_PRECISION+ENVMAP_PRECISION); \
     p2D->envv=(n->y*ENVMAP_PRECISION+ENVMAP_PRECISION); \
  } \


#define VERTEX_PIPELINE_ELSE(p2D) \
  if (v->flags < counter_for_resetting_vertex) \
  { \
     v->flags=counter_for_resetting_vertex; \
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


#define TRIA_PIPELINE_ENVRGB \
  for (w=0; w<num_tria_envRGB; w++) \
  { \
    if ((tria_envRGB[w].materiale->double_facing) || (vect_dot(&tria_envRGB[w].normal, &tria_envRGB[w].v1->tpoint)>CUL_EPS)) \
	{ \
	    v=tria_envRGB[w].v1; \
	    VERTEX_PIPELINE_ENVRGB(tria_envRGB[w].n1, tria_envRGB[w].sp1) \
        v=tria_envRGB[w].v2; \
	    VERTEX_PIPELINE_ENVRGB(tria_envRGB[w].n2, tria_envRGB[w].sp2) \
        v=tria_envRGB[w].v3; \
	    VERTEX_PIPELINE_ENVRGB(tria_envRGB[w].n3, tria_envRGB[w].sp3) \
        if (((tria_envRGB[w].v1->flags + tria_envRGB[w].v2->flags + tria_envRGB[w].v3->flags + 1) & 3) !=0 ) \
		{ \
	      *right_list++=&tria_envRGB[w]; \
		} \
	} \
  } \


#define TRIA_PIPELINE_ENVRGB_SWAP \
  for (w=0; w<num_tria_envRGB; w++) \
  { \
    if ((tria_envRGB[w].materiale->double_facing) || (vect_dot(&tria_envRGB[w].normal, &tria_envRGB[w].v1->tpoint)>CUL_EPS)) \
	{ \
	    v=tria_envRGB[w].v1; \
	    VERTEX_PIPELINE_ENVRGB_SWAP(tria_envRGB[w].n1, tria_envRGB[w].sp1) \
        v=tria_envRGB[w].v2; \
	    VERTEX_PIPELINE_ENVRGB_SWAP(tria_envRGB[w].n2, tria_envRGB[w].sp2) \
        v=tria_envRGB[w].v3; \
	    VERTEX_PIPELINE_ENVRGB_SWAP(tria_envRGB[w].n3, tria_envRGB[w].sp3) \
        if (((tria_envRGB[w].v1->flags + tria_envRGB[w].v2->flags + tria_envRGB[w].v3->flags + 1) & 3) !=0 ) \
		{ \
	      *right_list++=&tria_envRGB[w]; \
		} \
	} \
  } \


#define TRIA_PIPELINE_RGB \
  for (w=0; w<num_tria_RGB; w++) \
  { \
    if ((tria_RGB[w].materiale->double_facing) || (vect_dot(&tria_RGB[w].normal, &tria_RGB[w].v1->tpoint)>CUL_EPS)) \
	{ \
	    v=tria_RGB[w].v1; \
	    VERTEX_PIPELINE_RGB(tria_RGB[w].n1, tria_RGB[w].sp1) \
        v=tria_RGB[w].v2; \
	    VERTEX_PIPELINE_RGB(tria_RGB[w].n2, tria_RGB[w].sp2) \
        v=tria_RGB[w].v3; \
	    VERTEX_PIPELINE_RGB(tria_RGB[w].n3, tria_RGB[w].sp3) \
        if (((tria_RGB[w].v1->flags + tria_RGB[w].v2->flags + tria_RGB[w].v3->flags + 1) & 3) !=0 ) \
		{ \
		  *right_list++=&tria_RGB[w]; \
		} \
	} \
  } \


#define TRIA_PIPELINE_RGB_SWAP \
  for (w=0; w<num_tria_RGB; w++) \
  { \
    if ((tria_RGB[w].materiale->double_facing) || (vect_dot(&tria_RGB[w].normal, &tria_RGB[w].v1->tpoint)>CUL_EPS)) \
	{ \
	    v=tria_RGB[w].v1; \
	    VERTEX_PIPELINE_RGB_SWAP(tria_RGB[w].n1, tria_RGB[w].sp1) \
        v=tria_RGB[w].v2; \
	    VERTEX_PIPELINE_RGB_SWAP(tria_RGB[w].n2, tria_RGB[w].sp2) \
        v=tria_RGB[w].v3; \
	    VERTEX_PIPELINE_RGB_SWAP(tria_RGB[w].n3, tria_RGB[w].sp3) \
        if (((tria_RGB[w].v1->flags + tria_RGB[w].v2->flags + tria_RGB[w].v3->flags + 1) & 3) !=0 ) \
		{ \
		  *right_list++=&tria_RGB[w]; \
		} \
	} \
  } \


#define TRIA_PIPELINE_ENVMAP \
  for (w=0; w<num_tria_envmap; w++) \
  { \
    if ((tria_envmap[w].materiale->double_facing) || (vect_dot(&tria_envmap[w].normal, &tria_envmap[w].v1->tpoint)>CUL_EPS)) \
	{ \
	    v=tria_envmap[w].v1; \
	    VERTEX_PIPELINE_ENVMAP(tria_envmap[w].n1, tria_envmap[w].sp1) \
        v=tria_envmap[w].v2; \
	    VERTEX_PIPELINE_ENVMAP(tria_envmap[w].n2, tria_envmap[w].sp2) \
        v=tria_envmap[w].v3; \
	    VERTEX_PIPELINE_ENVMAP(tria_envmap[w].n3, tria_envmap[w].sp3) \
        if (((tria_envmap[w].v1->flags + tria_envmap[w].v2->flags + tria_envmap[w].v3->flags + 1) & 3) !=0 ) \
		{ \
	       *right_list++=&tria_envmap[w]; \
		} \
	} \
  } \



#define TRIA_PIPELINE_ELSE \
  for (w=0; w<num_tria; w++) \
  { \
    if ((tria[w].materiale->double_facing) || (vect_dot(&tria[w].normal, &tria[w].v1->tpoint)>CUL_EPS)) \
	{ \
	    v=tria[w].v1; \
	    VERTEX_PIPELINE_ELSE(tria[w].sp1) \
        v=tria[w].v2; \
	    VERTEX_PIPELINE_ELSE(tria[w].sp2) \
        v=tria[w].v3; \
	    VERTEX_PIPELINE_ELSE(tria[w].sp3) \
        if (((tria[w].v1->flags + tria[w].v2->flags + tria[w].v3->flags + 1) & 3) !=0 ) \
		{ \
	      *right_list++=&tria[w]; \
		} \
	} \
  } \


AD_PatchObject::AD_PatchObject()  // costruttore
{
  num_patches=num_points=num_vectors=0;
  vertex3D=(AD_Vertex3D *)NULL;
  vertex2D=(AD_Vertex2D *)NULL;
  normals=(AD_Vect3D *)NULL;

  father_name=name=(char *)NULL;
  father=(AD_Object3D *)NULL;
  have_childrens=0;

  num_omni=num_spot=0;

  matbase=(material *)NULL;
  accum_scale.x=accum_scale.y=accum_scale.z=1;
  radius=0;

  u_evaluations=v_evaluations=5;
  swap_normals=FALSE;
}


// valuta la pezza in (pu, pv) e restituisce il risultato in r
void AD_PatchObject::Evaluate_Patch(AD_Patch *p, float pu, float pv, AD_Vect3D *r)
{
  AD_Vect3D t;
  AD_Vect3D *vp = verteces_tr;
  AD_Vect3D *vecp = vectors_tr;
  int *v=p->vert;
  int *vec=p->vect;
  int *interior=p->interior;

  float pu2 = pu * pu;
  float pu1 = 1.0f - pu;
  float pu12 = pu1 * pu1;
  float u0 = pu12 * pu1;
  float u1 = 3.0f * pu * pu12;
  float u2 = 3.0f * pu2 * pu1;
  float u3 = pu2 * pu;
  float pv2 = pv * pv;
  float pv1 = 1.0f - pv;
  float pv12 = pv1 * pv1;
  float v0 = pv12 * pv1;
  float v1 = 3.0f * pv * pv12;
  float v2 = 3.0f * pv2 * pv1;
  float v3 = pv2 * pv;

  vect_scale_inline(&vp[v[0]], u0*v0, r);
  vect_scale_inline(&vecp[vec[7]], u1*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[6]], u2*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[3]], u3*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[0]], u0*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[0]], u1*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[3]], u2*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[5]], u3*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[1]], u0*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[1]], u1*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[2]], u2*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[4]], u3*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[1]], u0*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[2]], u1*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[3]], u2*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[2]], u3*v3, &t);
  vect_add_inline(r, &t, r);
}


void AD_PatchObject::Evaluate_uDerivate(AD_Patch *p, float pu, float pv, AD_Vect3D *r)
{
  AD_Vect3D t;
  AD_Vect3D *vp = verteces_trn;
  AD_Vect3D *vecp = vectors_trn;
  int *v=p->vert;
  int *vec=p->vect;
  int *interior=p->interior;

  // calcolo la derivata in u delle funzioni base
  float pu2 = pu * pu;
  float pu1 = 1.0f - pu;
  float u0 = -3.0f * pu1 * pu1;
  float u1 = 3.0f * pu1 * (1.0f -3*pu);
  float u2 = 3.0f * pu * (2.0f - 3*pu);
  float u3 = 3.0f * pu2;

  float pv2 = pv * pv;
  float pv1 = 1.0f - pv;
  float pv12 = pv1 * pv1;
  float v0 = pv12 * pv1;
  float v1 = 3.0f * pv * pv12;
  float v2 = 3.0f * pv2 * pv1;
  float v3 = pv2 * pv;

  vect_scale_inline(&vp[v[0]], u0*v0, r);
  vect_scale_inline(&vecp[vec[7]], u1*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[6]], u2*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[3]], u3*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[0]], u0*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[0]], u1*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[3]], u2*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[5]], u3*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[1]], u0*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[1]], u1*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[2]], u2*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[4]], u3*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[1]], u0*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[2]], u1*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[3]], u2*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[2]], u3*v3, &t);
  vect_add_inline(r, &t, r);
}


void AD_PatchObject::Evaluate_vDerivate(AD_Patch *p, float pu, float pv, AD_Vect3D *r)
{
  AD_Vect3D t;
  AD_Vect3D *vp = verteces_trn;
  AD_Vect3D *vecp = vectors_trn;
  int *v=p->vert;
  int *vec=p->vect;
  int *interior=p->interior;

  float pu2 = pu * pu;
  float pu1 = 1.0f - pu;
  float pu12 = pu1 * pu1;
  float u0 = pu12 * pu1;
  float u1 = 3.0f * pu * pu12;
  float u2 = 3.0f * pu2 * pu1;
  float u3 = pu2 * pu;

  // calcolo la derivata in v delle funzioni base
  float pv2 = pv * pv;
  float pv1 = 1.0f - pv;
  float v0 = -3.0f * pv1 * pv1;
  float v1 = 3.0f * pv1 * (1.0f - 3*pv);
  float v2 = 3.0f * pv * (2.0f - 3*pv);
  float v3 = 3.0f * pv2;

  vect_scale_inline(&vp[v[0]], u0*v0, r);
  vect_scale_inline(&vecp[vec[7]], u1*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[6]], u2*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[3]], u3*v0, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[0]], u0*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[0]], u1*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[3]], u2*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[5]], u3*v1, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[1]], u0*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[1]], u1*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[interior[2]], u2*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[4]], u3*v2, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[1]], u0*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[2]], u1*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vecp[vec[3]], u2*v3, &t);
  vect_add_inline(r, &t, r);
  vect_scale_inline(&vp[v[2]], u3*v3, &t);
  vect_add_inline(r, &t, r);
}


int AD_PatchObject::init(void)
{
  int i;

  // creo le strutture della mesh triangolare generata
  num_vertex3D=num_vertex2D=num_normals=(int)(u_evaluations*v_evaluations*num_patches);
  normals=new AD_Vect3D[num_normals];
  vertex3D=new AD_Vertex3D[num_vertex3D];
  vertex2D=new AD_Vertex2D[num_vertex3D];

  // creo vertici e vettori trasformati in camera space
  verteces_tr=new AD_Vect3D[num_points];
  vectors_tr=new AD_Vect3D[num_vectors];

  // creo i vertici e vettori utilizzati per il calcolo delle
  // normali ai vertici dei triangoli tassellizati in camera
  // space
  verteces_trn=new AD_Vect3D[num_points];
  vectors_trn=new AD_Vect3D[num_vectors];

  num_tria=(int)((u_evaluations-1)*(v_evaluations-1)*2.0f*num_patches);
  tria=new AD_Tria3D[num_tria];
  tria_envRGB=tria;
  tria_RGB=tria;
  tria_envmap=tria;

  // collego i puntatori dentro la struttura AD_vertex3D
  // ai corrispettivi array
  for (i=0; i<num_vertex3D; i++)
  {
	//vertex3D[i].normal=&normals[i];
	//vertex3D[i].p2D=&vertex2D[i];
    vertex3D[i].flags=0;
  }

  AddUpdate_Material(matbase);

  if (matbase->flags & PAINT_TEXTURE)
    for (i=0; i<num_UVverteces; i++)
	{
	   UVverteces[i].u*=TEXTURE_PRECISION;
	   UVverteces[i].v*=TEXTURE_PRECISION;
	}


  // setto i campi che saranno fissi nei triangoli
  for (i=0; i<num_tria; i++) tria[i].materiale=matbase;

// ############  INIZIALIZZAZIONI DEL KEYFRAMER  ############
  if (positiontrack.init()==-1) return(-1);
  if (rotationtrack.init()==-1) return(-1);
  if (scaletrack.init()==-1)    return(-1);

  for (i=0; i<num_points; i++) vert_pos[i].init();
  for (i=0; i<num_vectors; i++) vect_pos[i].init();

  return(0);
}


int AD_PatchObject::Tassellate_Texture(void)
{
// ***************************************************************
// TASSELLIZZAZIONE con generazione anche delle coordinate texture
// ***************************************************************
  AD_Vertex3D *points_already_calculated, *points_to_calculate;
  AD_Vertex2D *UVpoints_already_calculated, *UVpoints_to_calculate;
  float4 u_step, v_step, u, v;
  float4 tu0, tu1, tv0, tv1, dtu, dtv;
  int num_u_step, num_v_step, w, i, j;
  int ntria=0;
  AD_Vect3D p1, p2;

  u_step=1.0f/(u_evaluations-1); num_u_step=(int)(u_evaluations);
  v_step=1.0f/(v_evaluations-1); num_v_step=(int)(v_evaluations);

  points_already_calculated=&vertex3D[0];
  points_to_calculate=&vertex3D[num_u_step];

  UVpoints_already_calculated=&vertex2D[0];
  UVpoints_to_calculate=&vertex2D[num_u_step];
  ntria=0;
  
  for (w=0; w<num_patches; w++)
  {
    int patcul=Is_Patch_Culled(&patches[w]);

    if (!patcul)
	{
	  // precalcolo fila iniziale (isoparametrica v=0)
	  v=u=0;
	  dtu=(UVverteces[patches[w].UVvert[3]].u-
		   UVverteces[patches[w].UVvert[0]].u)/(u_evaluations-1);
	  dtv=(UVverteces[patches[w].UVvert[3]].v-
		   UVverteces[patches[w].UVvert[0]].v)/(u_evaluations-1);
	  tu0=UVverteces[patches[w].UVvert[0]].u;
	  tv0=UVverteces[patches[w].UVvert[0]].v;
	  for (i=0; i<num_u_step; i++)
	  {
        Evaluate_Patch(&patches[w], u, 0, &points_already_calculated[i].tpoint);
        UVpoints_already_calculated[i].u=tu0;
        UVpoints_already_calculated[i].v=tv0;
	    tu0+=dtu;
	    tv0+=dtv;
	    u+=u_step;
	  }

	  v=0;
	  for (j=0; j<num_v_step-1; j++)
	  {
        v+=v_step;
	    u=0;

	    tu0=   v*UVverteces[patches[w].UVvert[1]].u+
	       (1-v)*UVverteces[patches[w].UVvert[0]].u;

	    tv0=   v*UVverteces[patches[w].UVvert[1]].v+
	       (1-v)*UVverteces[patches[w].UVvert[0]].v;

	    tu1=   v*UVverteces[patches[w].UVvert[2]].u+
	       (1-v)*UVverteces[patches[w].UVvert[3]].u;

	    tv1=   v*UVverteces[patches[w].UVvert[2]].v+
	       (1-v)*UVverteces[patches[w].UVvert[3]].v;

	    dtu=(tu1-tu0)/(u_evaluations-1);
	    dtv=(tv1-tv0)/(u_evaluations-1);

	    for (i=0; i<num_u_step; i++)
		{
           Evaluate_Patch(&patches[w], u, v, &points_to_calculate[i].tpoint);
           UVpoints_to_calculate[i].u=tu0;
           UVpoints_to_calculate[i].v=tv0;
		   tu0+=dtu;
		   tv0+=dtv;
		   u+=u_step;
		}
	    // creazione dei triangoli
        for (i=0; i<num_u_step-1; i++)
		{
          tria[ntria].v1=&points_already_calculated[i];
          tria[ntria].v2=&points_already_calculated[i+1];
          tria[ntria].v3=&points_to_calculate[i+1];
		  // calcolo della normale
          vect_sub_inline(&tria[ntria].v1->tpoint, &tria[ntria].v2->tpoint, &p1);
          vect_sub_inline(&tria[ntria].v3->tpoint, &tria[ntria].v2->tpoint, &p2);
          vect_cross(&p1, &p2, &tria[ntria].normal);
          vect_normalize(&tria[ntria].normal);
		  tria[ntria].sp1=&UVpoints_already_calculated[i];
		  tria[ntria].sp2=&UVpoints_already_calculated[i+1];
		  tria[ntria].sp3=&UVpoints_to_calculate[i+1];
	      ntria++;


		  tria[ntria].v1=&points_to_calculate[i+1];
          tria[ntria].v2=&points_to_calculate[i];
	      tria[ntria].v3=&points_already_calculated[i];
		  // calcolo della normale
          vect_sub_inline(&tria[ntria].v1->tpoint, &tria[ntria].v2->tpoint, &p1);
          vect_sub_inline(&tria[ntria].v3->tpoint, &tria[ntria].v2->tpoint, &p2);
          vect_cross(&p1, &p2, &tria[ntria].normal);
          vect_normalize(&tria[ntria].normal);
		  tria[ntria].sp1=&UVpoints_to_calculate[i+1];
		  tria[ntria].sp2=&UVpoints_to_calculate[i];
		  tria[ntria].sp3=&UVpoints_already_calculated[i];
	      ntria++;
		}
        points_already_calculated+=num_u_step;
        points_to_calculate+=num_u_step;
        UVpoints_already_calculated+=num_u_step;
        UVpoints_to_calculate+=num_u_step;
	  }
      points_already_calculated+=num_u_step;
      points_to_calculate+=num_u_step;
      UVpoints_already_calculated+=num_u_step;
      UVpoints_to_calculate+=num_u_step;
    }
  }
  return(ntria);
}


int AD_PatchObject::Tassellate_Normals(void)
{
// ***************************************************************
// TASSELLIZZAZIONE con generazione anche delle normali ai vertici
// ***************************************************************
  AD_Vertex3D *points_already_calculated, *points_to_calculate;
  AD_Vect3D *normals_already_calculated, *normals_to_calculate;
  float4 u_step, v_step, u, v;
  int num_u_step, num_v_step, w, i, j;
  int ntria=0;
  AD_Vect3D p1, p2;

  u_step=1.0f/(u_evaluations-1); num_u_step=(int)(u_evaluations);
  v_step=1.0f/(v_evaluations-1); num_v_step=(int)(v_evaluations);

  points_already_calculated=&vertex3D[0];
  points_to_calculate=&vertex3D[num_u_step];
  normals_already_calculated=&normals[0];
  normals_to_calculate=&normals[num_u_step];
  ntria=0;
  
  for (w=0; w<num_patches; w++)
  {
    int patcul=Is_Patch_Culled(&patches[w]);

    if (!patcul)
	{
	  // precalcolo fila iniziale (isoparametrica v=0)
	  v=u=0;
	  for (i=0; i<num_u_step; i++)
	  {
        Evaluate_Patch(&patches[w], u, 0, &points_already_calculated[i].tpoint);
        Evaluate_uDerivate(&patches[w], u, 0, &p1);
        Evaluate_vDerivate(&patches[w], u, 0, &p2);
	    vect_cross(&p1, &p2, &normals_already_calculated[i]);
	    vect_normalize(&normals_already_calculated[i]);
	    u+=u_step;
	  }

	  v=0;
	  for (j=0; j<num_v_step-1; j++)
	  {
        v+=v_step;
	    u=0;
	    for (i=0; i<num_u_step; i++)
		{
           Evaluate_Patch(&patches[w], u, v, &points_to_calculate[i].tpoint);
           Evaluate_uDerivate(&patches[w], u, v, &p1);
           Evaluate_vDerivate(&patches[w], u, v, &p2);
	       vect_cross(&p1, &p2, &normals_to_calculate[i]);
	       vect_normalize(&normals_to_calculate[i]);
		   u+=u_step;
		}
	    // creazione dei triangoli
        for (i=0; i<num_u_step-1; i++)
		{
          tria[ntria].v1=&points_already_calculated[i];
          tria[ntria].v2=&points_already_calculated[i+1];
          tria[ntria].v3=&points_to_calculate[i+1];
	      tria[ntria].n1=&normals_already_calculated[i];
          tria[ntria].n2=&normals_already_calculated[i+1];
	      tria[ntria].n3=&normals_to_calculate[i+1];
		  // calcolo della normale
          vect_sub_inline(&tria[ntria].v1->tpoint, &tria[ntria].v2->tpoint, &p1);
          vect_sub_inline(&tria[ntria].v3->tpoint, &tria[ntria].v2->tpoint, &p2);
          vect_cross(&p1, &p2, &tria[ntria].normal);
          vect_normalize(&tria[ntria].normal);
	      ntria++;


		  tria[ntria].v1=&points_to_calculate[i+1];
          tria[ntria].v2=&points_to_calculate[i];
	      tria[ntria].v3=&points_already_calculated[i];
	      tria[ntria].n1=&normals_to_calculate[i+1];
          tria[ntria].n2=&normals_to_calculate[i];
	      tria[ntria].n3=&normals_already_calculated[i];
		  // calcolo della normale
          vect_sub_inline(&tria[ntria].v1->tpoint, &tria[ntria].v2->tpoint, &p1);
          vect_sub_inline(&tria[ntria].v3->tpoint, &tria[ntria].v2->tpoint, &p2);
          vect_cross(&p1, &p2, &tria[ntria].normal);
          vect_normalize(&tria[ntria].normal);
	      ntria++;
		}
        points_already_calculated+=num_u_step;
        points_to_calculate+=num_u_step;
        normals_already_calculated+=num_u_step;
        normals_to_calculate+=num_u_step;
	  }
      points_already_calculated+=num_u_step;
      points_to_calculate+=num_u_step;
      normals_already_calculated+=num_u_step;
      normals_to_calculate+=num_u_step;
	}
  }
  return(ntria);
}


int AD_PatchObject::Tassellate_NormalsTexture(void)
{
// ***************************************************************
// TASSELLIZZAZIONE con generazione anche delle normali ai vertici
// e coordinate texture
// ***************************************************************
  AD_Vertex3D *points_already_calculated, *points_to_calculate;
  AD_Vertex2D *UVpoints_already_calculated, *UVpoints_to_calculate;
  AD_Vect3D *normals_already_calculated, *normals_to_calculate;
  float4 u_step, v_step, u, v;
  float4 tu0, tu1, tv0, tv1, dtu, dtv;
  int num_u_step, num_v_step, w, i, j;
  int ntria=0;
  AD_Vect3D p1, p2;

  u_step=1.0f/(u_evaluations-1); num_u_step=(int)(u_evaluations);
  v_step=1.0f/(v_evaluations-1); num_v_step=(int)(v_evaluations);

  points_already_calculated=&vertex3D[0];
  points_to_calculate=&vertex3D[num_u_step];
  UVpoints_already_calculated=&vertex2D[0];
  UVpoints_to_calculate=&vertex2D[num_u_step];
  normals_already_calculated=&normals[0];
  normals_to_calculate=&normals[num_u_step];
  ntria=0;
  
  for (w=0; w<num_patches; w++)
  {
    int patcul=Is_Patch_Culled(&patches[w]);

    if (!patcul)
	{
	  // precalcolo fila iniziale (isoparametrica v=0)
	  v=u=0;
	  dtu=(UVverteces[patches[w].UVvert[3]].u-
		   UVverteces[patches[w].UVvert[0]].u)/(u_evaluations-1);
	  dtv=(UVverteces[patches[w].UVvert[3]].v-
		   UVverteces[patches[w].UVvert[0]].v)/(u_evaluations-1);
	  tu0=UVverteces[patches[w].UVvert[0]].u;
	  tv0=UVverteces[patches[w].UVvert[0]].v;
	  for (i=0; i<num_u_step; i++)
	  {
        Evaluate_Patch(&patches[w], u, 0, &points_already_calculated[i].tpoint);
        Evaluate_uDerivate(&patches[w], u, 0, &p1);
        Evaluate_vDerivate(&patches[w], u, 0, &p2);
	    vect_cross(&p1, &p2, &normals_already_calculated[i]);
	    vect_normalize(&normals_already_calculated[i]);
        UVpoints_already_calculated[i].u=tu0;
        UVpoints_already_calculated[i].v=tv0;
	    tu0+=dtu;
	    tv0+=dtv;
	    u+=u_step;
	  }

	  v=0;
	  for (j=0; j<num_v_step-1; j++)
	  {
        v+=v_step;
	    u=0;

	    tu0=   v*UVverteces[patches[w].UVvert[1]].u+
	       (1-v)*UVverteces[patches[w].UVvert[0]].u;

	    tv0=   v*UVverteces[patches[w].UVvert[1]].v+
	       (1-v)*UVverteces[patches[w].UVvert[0]].v;

	    tu1=   v*UVverteces[patches[w].UVvert[2]].u+
	       (1-v)*UVverteces[patches[w].UVvert[3]].u;

	    tv1=   v*UVverteces[patches[w].UVvert[2]].v+
	       (1-v)*UVverteces[patches[w].UVvert[3]].v;

	    dtu=(tu1-tu0)/(u_evaluations-1);
	    dtv=(tv1-tv0)/(u_evaluations-1);

	    for (i=0; i<num_u_step; i++)
		{
           Evaluate_Patch(&patches[w], u, v, &points_to_calculate[i].tpoint);
           Evaluate_uDerivate(&patches[w], u, v, &p1);
           Evaluate_vDerivate(&patches[w], u, v, &p2);
	       vect_cross(&p1, &p2, &normals_to_calculate[i]);
	       vect_normalize(&normals_to_calculate[i]);
           UVpoints_to_calculate[i].u=tu0;
           UVpoints_to_calculate[i].v=tv0;
		   tu0+=dtu;
		   tv0+=dtv;
		   u+=u_step;
		}
	    // creazione dei triangoli
        for (i=0; i<num_u_step-1; i++)
		{
          tria[ntria].v1=&points_already_calculated[i];
          tria[ntria].v2=&points_already_calculated[i+1];
          tria[ntria].v3=&points_to_calculate[i+1];
	      tria[ntria].n1=&normals_already_calculated[i];
          tria[ntria].n2=&normals_already_calculated[i+1];
	      tria[ntria].n3=&normals_to_calculate[i+1];
		  tria[ntria].sp1=&UVpoints_already_calculated[i];
		  tria[ntria].sp2=&UVpoints_already_calculated[i+1];
		  tria[ntria].sp3=&UVpoints_to_calculate[i+1];
		  // calcolo della normale
          vect_sub_inline(&tria[ntria].v1->tpoint, &tria[ntria].v2->tpoint, &p1);
          vect_sub_inline(&tria[ntria].v3->tpoint, &tria[ntria].v2->tpoint, &p2);
          vect_cross(&p1, &p2, &tria[ntria].normal);
          vect_normalize(&tria[ntria].normal);
	      ntria++;


		  tria[ntria].v1=&points_to_calculate[i+1];
          tria[ntria].v2=&points_to_calculate[i];
	      tria[ntria].v3=&points_already_calculated[i];
	      tria[ntria].n1=&normals_to_calculate[i+1];
          tria[ntria].n2=&normals_to_calculate[i];
	      tria[ntria].n3=&normals_already_calculated[i];
		  tria[ntria].sp1=&UVpoints_to_calculate[i+1];
		  tria[ntria].sp2=&UVpoints_to_calculate[i];
		  tria[ntria].sp3=&UVpoints_already_calculated[i];
		  // calcolo della normale
          vect_sub_inline(&tria[ntria].v1->tpoint, &tria[ntria].v2->tpoint, &p1);
          vect_sub_inline(&tria[ntria].v3->tpoint, &tria[ntria].v2->tpoint, &p2);
          vect_cross(&p1, &p2, &tria[ntria].normal);
          vect_normalize(&tria[ntria].normal);
	      ntria++;
		}
        points_already_calculated+=num_u_step;
        points_to_calculate+=num_u_step;
        normals_already_calculated+=num_u_step;
        normals_to_calculate+=num_u_step;
        UVpoints_already_calculated+=num_u_step;
        UVpoints_to_calculate+=num_u_step;
	  }
      points_already_calculated+=num_u_step;
      points_to_calculate+=num_u_step;
      normals_already_calculated+=num_u_step;
      normals_to_calculate+=num_u_step;
      UVpoints_already_calculated+=num_u_step;
      UVpoints_to_calculate+=num_u_step;
    }
  }

  return(ntria);
}


int AD_PatchObject::Tassellate(void)
{
  int nt;

  num_tria_envRGB=0;
  num_tria_RGB=0;
  num_tria_envmap=0;
  num_tria=0;

  // caso di nessun materiale: genero le normali nel
  // caso di luci
  if (matbase==NULL)
  {
	nt=Tassellate_Normals();
	num_tria_RGB=nt;
    return(nt);
  }

  if ((!(matbase->flags & PAINT_TEXTURE)) &&
      ((matbase->flags & PAINT_LIGHT)) &&
	  (!(matbase->flags & PAINT_ENVMAP)))
  {
	nt=Tassellate_Normals();
	num_tria_RGB=nt;
    return(nt);
  }


  // caso di solo texture e nessuna luce
  if ((matbase->flags & PAINT_TEXTURE) &&
      (!(matbase->flags & PAINT_LIGHT)) &&
	  (!(matbase->flags & PAINT_ENVMAP)))
  {
	  nt=Tassellate_Texture();
	  num_tria=nt;
	  return(nt);
  }

  // caso di solo envmap con o senza luci
  if ((matbase->flags & PAINT_ENVMAP) &&
      (!(matbase->flags & PAINT_TEXTURE))) 
  {
	  nt=Tassellate_Normals();
	  if (matbase->flags & PAINT_LIGHT) num_tria_envRGB=nt;
	  else num_tria_envmap=nt;
	  return(nt);
  }

  // caso di texture+envmap
  if ((matbase->flags & PAINT_TEXTURE) &&
	  (matbase->flags & PAINT_ENVMAP))
  {
	  nt=Tassellate_NormalsTexture();
	  if (matbase->flags & PAINT_LIGHT) num_tria_envRGB=nt;
	  else num_tria_envmap=nt;
	  return(nt);
  }

  // caso di texture+luci
  if ((matbase->flags & PAINT_TEXTURE) &&
	  (matbase->flags & PAINT_LIGHT))
  {
	 nt=Tassellate_NormalsTexture();
	 num_tria_RGB=nt;
	 return(nt);
  }

  // NB: il caso texture+envmap+luci e' contemplato da uno
  // dei 2 casi sopra

  return(0);
}


void AD_PatchObject::build_objectmatrix (float4 framepos)
// costruisce la matrice di trasformazione, che servira' poi per trasformare
// i vertici dell'oggetto;
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


int AD_PatchObject::Is_Patch_Culled (AD_Patch *p)
{
/*
  // Serve per sapere se una patch e' cullata per intero, cosi' posso evitare
  // di tassellizzarla
  AD_Vect3D midp, n;
  AD_Vect3D p1, p2;

  // calcolo della mormale del piano fatto dai vertici 0, 1, 2
  vect_sub(&verteces_trn[p->vert[0]], &verteces_trn[p->vert[1]], &p1);
  vect_sub(&verteces_trn[p->vert[2]], &verteces_trn[p->vert[1]], &p2);
  vect_cross(&p1, &p2, &n);
  vect_normalize(&n);
  vect_copy(&verteces_trn[p->vert[0]], &midp);
  vect_add(&midp, &verteces_trn[p->vert[1]], &midp);
  vect_add(&midp, &verteces_trn[p->vert[2]], &midp);
  vect_scale(&midp, 1.0f/3.0f, &midp);
  vect_add(&midp, &currentpos_incamera, &midp);
  vect_normalize(&midp);
  if (vect_dot(&midp, &n) < 0) return(0);


  // calcolo della mormale del piano fatto dai vertici 2, 3, 0
  vect_sub(&verteces_trn[p->vert[2]], &verteces_trn[p->vert[3]], &p1);
  vect_sub(&verteces_trn[p->vert[0]], &verteces_trn[p->vert[3]], &p2);
  vect_cross(&p1, &p2, &n);
  vect_normalize(&n);
  vect_copy(&verteces_trn[p->vert[2]], &midp);
  vect_add(&midp, &verteces_trn[p->vert[3]], &midp);
  vect_add(&midp, &verteces_trn[p->vert[0]], &midp);
  vect_scale(&midp, 1.0f/3.0f, &midp);
  vect_add(&midp, &currentpos_incamera, &midp);
  vect_normalize(&midp);
  if (vect_dot(&midp, &n) < 0) return(0);


  // calcolo della mormale del piano fatto dai vertici 3, 0, 1
  vect_sub(&verteces_trn[p->vert[3]], &verteces_trn[p->vert[0]], &p1);
  vect_sub(&verteces_trn[p->vert[1]], &verteces_trn[p->vert[0]], &p2);
  vect_cross(&p1, &p2, &n);
  vect_normalize(&n);
  vect_copy(&verteces_trn[p->vert[3]], &midp);
  vect_add(&midp, &verteces_trn[p->vert[0]], &midp);
  vect_add(&midp, &verteces_trn[p->vert[1]], &midp);
  vect_scale(&midp, 1.0f/3.0f, &midp);
  vect_add(&midp, &currentpos_incamera, &midp);
  vect_normalize(&midp);
  if (vect_dot(&midp, &n) < 0) return(0);

  return(1);
*/
	return(0);
}


void AD_PatchObject::paint(float4 pos, AD_Camera *telecamera, AD_Omnilight *omnilight)
{
  int w, ntria_generated, wl;
  float4 invz, cosalpha, dist, inv_prospX, inv_prospY;
  AD_Matrix matrixrot_all, matrixtransform_all;
  AD_Matrix matrixtall_clip;
  AD_Vect3D vtmp, light_vertex_vector, inv_accum_scale, vaux;
  AD_Vertex3D *v;
  AD_Tria3D **right_list;


  build_objectmatrix(pos);
  inv_accum_scale.x=1.0f/accum_scale.x;
  inv_accum_scale.y=1.0f/accum_scale.y;
  inv_accum_scale.z=1.0f/accum_scale.z;
  mat_mulvect(&telecamera->currentmatrix, &currentpos, &currentpos_incamera);

  // si calcolano le matrici di trasformazione totali
  mat_mulaffine(&telecamera->currentmatrix_rot, &currentmatrix_rot, &matrixrot_all);
  mat_mul(&telecamera->currentmatrix, &currentmatrix, &matrixtall_clip);

  // calcolo della matrice di trasformazione con inclusi i
  // fattori di aspect ratio; pX e pY si trovano in render.cpp
  mat_copy(&matrixtall_clip, &matrixtransform_all);
  for (w=0; w<4; w++)
  {
    matrixtransform_all.a[0][w]=matrixtransform_all.a[0][w]*telecamera->prospettivaX;
    matrixtransform_all.a[1][w]=matrixtransform_all.a[1][w]*telecamera->prospettivaY;
  }
  inv_prospX=1.0f/telecamera->prospettivaX;
  inv_prospY=1.0f/telecamera->prospettivaY;

  // si portano le luci nello spazio oggetto (ma sempre nello
  // spazio camera)
  for (w=0; w<num_omni; w++)
	mat_mulvect(&telecamera->currentmatrix, &omnilight[w].currentpos, &omnilight[w].currentpos_inobject);

  // trasformiamo tutti i vertici geometrici
  for (w=0; w<num_points; w++)
  {
	// ottengo le posizioni dei vertici dal keyframer
	if (vert_pos[w].numkey>0) vert_pos[w].get_data(pos, &verteces[w]);

	// li trasformo in camera space
	mat_mulvect(&matrixtransform_all, &verteces[w], &verteces_tr[w]);

	// calcolo i rispettivi per il calcolo delle normali
	// in camera space
	mat_mulvect(&matrixtall_clip, &verteces[w], &verteces_trn[w]);
	vect_sub(&verteces_trn[w], &currentpos_incamera, &verteces_trn[w]);
  }
  

  // trasformiamo tutti i vettori
  for (w=0; w<num_vectors; w++)
  {
	// ottengo le posizioni dei vettori dal keyframer
	if (vect_pos[w].numkey>0) vect_pos[w].get_data(pos, &vectors[w]);

	// li trasformo in camera space
	mat_mulvect(&matrixtransform_all, &vectors[w], &vectors_tr[w]);

	// calcolo i rispettivi per il calcolo delle normali
	// in camera space
	mat_mulvect(&matrixtall_clip, &vectors[w], &vectors_trn[w]);
	vect_sub(&vectors_trn[w], &currentpos_incamera, &vectors_trn[w]);
  }


  ntria_generated=Tassellate();
  if (matbase->flags & IS_TRASPARENT)
	  right_list=list_to_paint_trasparent;
  else
	  right_list=(AD_Tria3D **)matbase->my_tria_list;

  if (swap_normals==TRUE)
  {
    TRIA_PIPELINE_ENVRGB_SWAP
    TRIA_PIPELINE_RGB_SWAP
  }
  else
  {
    TRIA_PIPELINE_ENVRGB
    TRIA_PIPELINE_RGB
  }
  TRIA_PIPELINE_ENVMAP
  TRIA_PIPELINE_ELSE

  if (matbase->flags & IS_TRASPARENT)
	  list_to_paint_trasparent=right_list;
  else
	  matbase->my_tria_list=(void **)right_list;
}