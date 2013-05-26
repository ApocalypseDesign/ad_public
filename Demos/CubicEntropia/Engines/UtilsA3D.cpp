// A3Dfile.cpp: implementation of the A3Dfile class.
//
//////////////////////////////////////////////////////////////////////
#define MAXA3DFILESIZE 1024*1024*2 // max 2 Mb

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <math.h>
#include "UtilsA3D.h"

char material_no_found[5]="";
extern HWND miawin;         // finestra principale; usata qui per le debug_error

char *FileA3D::allocA3D(char *filename)
// carica in memoria il file .A3D
// restituisce la dimensione allocata in byte o -1 se c'e' stato un errore
{
  FILE *bfile;
  int lung, i;
  char *fileA3D;

  if ((bfile=fopen(filename, "rb"))==(FILE *)0x0) return (char *)-1;
  lung=filelength(fileno(bfile));
  fileA3D=new char[lung];
  for (i=0; i<lung; i++) 
	 fread(&fileA3D[i], 1, 1, bfile);

  fclose(bfile);
  return fileA3D;
}


char *FileA3D::get_next_chunk(char *current_chunk)
{
  // NB: la procedura presuppone che current_chunk punti
  // ad un chunk ID

  char *c;
  int size;

  c=current_chunk+sizeof(int);  // salto il chunk ID
  for (; *c!='\0'; c++)
  {
  }

  c++; // salto anche il '\0'
  size=*((int *)c);
  c=c+4+size;  // il +4 e' il campo size

  return(c);
}


void FileA3D::scene_load_general_info(AD_Scene3D *s, char *chunk)
{
  int start_frame, end_frame, *f;

  chunk=chunk+4;                // salto l'ID
  chunk=chunk+strlen(chunk)+1;  // salto il name_owner
                                // il +1 serve per saltare il \0
  chunk=chunk+4;  // salto la size

  f=(int *)chunk;
  start_frame=*f; f++;
  end_frame=*f;
  s->num_frame=end_frame-start_frame+1;

  // TO DO
  // ci sarebbe il frame speed ma per adesso non lo uso
}



// ###########################################################
// ####################  LOAD DI OBJECT3D  ###################
// ###########################################################

void FileA3D::object_load_mesh(AD_Object3D *obj, char *chunk)
{
   int is_a_children, *dati, sz, there_is_material;
   char *c;
   float *n;
   AD_Vect3D localx, localy, localz;


   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   obj->name=new char[sz+1];
   strcpy(obj->name, chunk+4);

   dati=(int *)(chunk+4+(sz+1)+4);
   is_a_children=*dati;
   if (is_a_children)
   {
	 c=(char *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 obj->father_name=new char[sz+1];
	 strcpy(obj->father_name, c);
	 c=c+sz+1;
   }
   else c=(char *)(dati+1);  // non c'e' nome, quindi salto solo il flag

   dati=(int *)c;
   there_is_material=*dati;
   c=c+4;  // punto all'inizio dell'eventuale nome del materiale

   if (there_is_material)
   {
	 sz=strlen(c);
     obj->matbase=myscene->get_material_pointer(c);
	 c=c+(sz+1);
   }

  // lettura del local coordinate system
  n=(float *)c;
  localx.x=*n; n++;
  localx.y=*n; n++;  
  localx.z=*n; n++;

  localy.x=*n; n++;  
  localy.y=*n; n++;
  localy.z=*n; n++;

  localz.x=*n; n++;
  localz.y=*n; n++;
  localz.z=*n; n++;

  obj->currentpos.x=*n; n++;
  obj->currentpos.y=*n; n++;
  obj->currentpos.z=*n; n++;
  vect_swap_3DS(&obj->currentpos);

  // lettura pivot
  obj->pivot.x=*n; n++;
  obj->pivot.y=*n; n++;
  obj->pivot.z=*n;
  vect_swap_3DS(&obj->pivot);
}


void FileA3D::object_load_helper(AD_Object3D *obj, char *chunk)
{
   int sz, *dati;
   char *c;
   float *f;

   // settaggi iniziali per non avere casini
   obj->father=(AD_Object3D *)NULL;
   obj->flare=(texture *)NULL;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   obj->name=new char[sz+1];
   strcpy(obj->name, chunk+4);  // scrivo il nome del'helper

   dati=(int *)(chunk+4+(sz+1)+4);
   if (*dati==0) obj->type=DUMMY;
   else obj->type=BONE;

   c=(chunk+4+(sz+1)+4+4);   // punto al nome del padre
   sz=strlen(c);
   obj->father_name=new char[sz+1];
   strcpy(obj->father_name, c);
   c=c+(sz+1);
   f=(float *)(c);

   // lettura posizione nel mondo
   obj->currentpos.x=*f; f++;
   obj->currentpos.y=*f; f++;
   obj->currentpos.z=*f; f++;
   vect_swap_3DS(&obj->currentpos);

   // lettura punto pivot (inutile, non serve)
   obj->pivot.x=*f; f++;
   obj->pivot.y=*f; f++;
   obj->pivot.z=*f;
   vect_swap_3DS(&obj->pivot);

   if (obj->type==DUMMY)
   {
     // sistemo i vertici e i triangoli per il flare
     obj->num_vertex3D=4;
     obj->num_vertex2D=4;
     obj->vertex3D=new AD_Vertex3D[obj->num_vertex3D];
     obj->vertex2D=new AD_Vertex2D[obj->num_vertex2D];
     obj->num_tria=2;
     obj->tria=new AD_Tria3D[obj->num_tria];
   }
   else
   {
     obj->num_vertex3D=obj->num_tria=obj->num_vertex2D=0;
   }
}


void FileA3D::object_load_vertex_list(AD_Object3D *obj, char *chunk)
{
  int *num_vert, sz, j;
  float *f, x, y, z;

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;  // puntiamo ai dati

  num_vert=(int *)chunk;
  obj->num_vertex3D=*num_vert;
  num_vert=num_vert+1;

  obj->vertex3D=new AD_Vertex3D[obj->num_vertex3D];
  f=(float *)(num_vert);
  for (j=0; j<obj->num_vertex3D; j++)
  {
    x=*f;  f++;
    y=*f;  f++;
    z=*f;  f++;
    vect_set(&obj->vertex3D[j].point, x, y, z);
    vect_swap_3DS(&obj->vertex3D[j].point);
	vect_sub(&obj->vertex3D[j].point, &obj->pivot, &obj->vertex3D[j].point);
  }
}	  


void FileA3D::object_load_face_list(AD_Object3D *obj, char *chunk)
{
  int *numtria, sz, j;
  int *f, a, b, c;

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;  // puntiamo ai dati

  numtria=(int *)chunk;
  obj->num_tria=*numtria;
  numtria=numtria+1;
  obj->tria=new AD_Tria3D[obj->num_tria];
  obj->triasmoothgroup=new int[obj->num_tria];
  // l'allocazione e assegnazione dei vertici 2D avviene nella
  // init normals della object3D, per un motivo: se l'oggetto ha
  // degli OSM o bonez allegate è inutile allocare il triplo di
  // punti2D e normali, perkè nel caso specifico gli smoothing groups
  // non venogno rispettati e c'è una corrispondenza 1-1 tra
  // vertice3D, normale e vertice2D

  f=numtria;
  for (j=0; j<obj->num_tria; j++)
  {
    a=*f; f++;
    b=*f; f++;
    c=*f; f++;
    obj->tria[j].v1=&obj->vertex3D[a];
    obj->tria[j].v2=&obj->vertex3D[b];
    obj->tria[j].v3=&obj->vertex3D[c];
    obj->triasmoothgroup[j]=*f; f++;
    obj->tria[j].materiale=(material *)NULL;
  }
}


void FileA3D::object_load_texture_vertex_list(AD_Object3D *obj, char *chunk)
{
  int *dati, sz, j;
  float *f;

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;
  dati=(int *)chunk;

  obj->num_vertexUV=*dati;
  obj->vertexUV=new AD_VectUV[obj->num_vertexUV];

  chunk=chunk+4;
  f=(float *)chunk;

  for (j=0; j<obj->num_vertexUV; j++)
  {
    obj->vertexUV[j].u=*f;    f++;
    obj->vertexUV[j].v=-(*f); f++;
  }
}


void FileA3D::object_load_texture_face_list(AD_Object3D *obj, char *chunk)
{
  int *dati, sz, j, nt, ta, tb, tc, g_id;

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;
  dati=(int *)chunk;
  nt=*dati;    // leggo il numero di facce che deve essere
               // consistente
  dati++;
  for (j=0; j<nt; j++)
  {
	ta=*dati;     dati++;
	tb=*dati;     dati++;
	tc=*dati;     dati++;
	g_id=*dati;   dati++;
	// TRUCCACCIO: sicome i vertici2D vengono allocati dopo che si
	// è verificato il caso di smoothing groups o OSM/Bones, per
	// ora vi assegno il puntatore ai verticiUV
	obj->tria[j].sp1=(AD_Vertex2D *)(&obj->vertexUV[ta]);
	obj->tria[j].sp2=(AD_Vertex2D *)(&obj->vertexUV[tb]);
	obj->tria[j].sp3=(AD_Vertex2D *)(&obj->vertexUV[tc]);
	obj->tria[j].materiale=myscene->get_material_pointer(get_material_name(g_id));
  }
}


// ###########################################################
// ###################  LOAD DI TELECAMERE  ##################
// ###########################################################
void FileA3D::camera_load_settings (AD_Camera *cam, char *chunk)
{
  float *f;
  int sz;

  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  cam->name=new char[sz+1];
  strcpy(cam->name, chunk+4);

  chunk=chunk+4+(sz+1)+4;  // punto ai dati: salto ID, name_owner, size
  f=(float *)chunk;

  cam->znear=*f; f++;
  cam->zfar=*f; f++;
  cam->currentfov=*f; f++;
  cam->currentroll=*f; f++;

  cam->currentpos.x=*f; f++;
  cam->currentpos.y=*f; f++;
  cam->currentpos.z=*f; f++;
  vect_swap_3DS(&cam->currentpos);

  cam->currenttarget.x=*f; f++;
  cam->currenttarget.y=*f; f++;
  cam->currenttarget.z=*f; f++;
  vect_swap_3DS(&cam->currenttarget);

  // calcolo il FOV/2 e relativa tangente
  cam->currentfov=cam->currentfov/2.0f;
  cam->currenttgfov=(float)tan(fabs(cam->currentfov));
}





// ###########################################################
// ###################  LOAD DI OMNILIGHT  ###################
// ###########################################################

void FileA3D::omnilight_load_settings(AD_Omnilight *ol, char *chunk)
{
  float *f;
  int sz;

  // settaggi iniziali flare
  ol->latoX=ol->latoY=0;
  ol->flare=(texture *)NULL;
  
  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  ol->name=new char[sz+1];
  strcpy(ol->name, chunk+4);

  chunk=chunk+4+(sz+1)+4;  // punto ai dati: salto ID, name_owner, size
  f=(float *)chunk;

  ol->currentcolor.x=(float)(RGB_MAXVALUE*(*f)); f++;
  ol->currentcolor.y=(float)(RGB_MAXVALUE*(*f)); f++;
  ol->currentcolor.z=(float)(RGB_MAXVALUE*(*f)); f++;

  ol->currentpos.x=*f; f++;
  ol->currentpos.y=*f; f++;
  ol->currentpos.z=*f; f++;
  vect_swap_3DS(&ol->currentpos);
}


// ###########################################################
// #####################  LOAD KEYFRAME  #####################
// ###########################################################

void FileA3D::keyframe_load_postrack(postrack *pt, char *chunk)
{
  int sz;
  unsigned char type, *t;
  int nkey, *v;
  float *f;

  sz=strlen(chunk+4);       // punto al nome e leggo la sua lunghezza
  chunk=chunk+4+(sz+1)+4;   // punto direttamente ai dati

  t=(unsigned char *)chunk;
  type=*t; t=t+1;             // leggo il tipo di controller
  
  v=(int *)t;
  nkey=*v; v++;             // leggo il numero di key
  pt->numkey=nkey;
  pt->posizioni=new POSKEY[pt->numkey];

  f=(float *)v;
  for (nkey=0; nkey<pt->numkey; nkey++)
  {
	 v=(int *)f;
	 pt->posizioni[nkey].posintrack=*v;
	 v=v+1;

	 f=(float *)v;
	 pt->posizioni[nkey].p.x=*f; f++;
	 pt->posizioni[nkey].p.y=*f; f++;
	 pt->posizioni[nkey].p.z=*f; f++;
     vect_swap_3DS(&pt->posizioni[nkey].p);

	 if (type==TCB_CONTROLLER)
	 {
	    pt->posizioni[nkey].tension=*f;     f++;
	    pt->posizioni[nkey].continuity=*f;  f++;
	    pt->posizioni[nkey].bias=*f;        f++;
	    pt->posizioni[nkey].easeto=*f;      f++;
	    pt->posizioni[nkey].easefrom=*f;    f++;
        pt->type=pt->type | TCB_TRACK;
	 }
	 else
	 if (type==BEZIER_CONTROLLER)
	 {
	    pt->posizioni[nkey].incomtg.x=(*f); f++;
	    pt->posizioni[nkey].incomtg.y=(*f); f++;
	    pt->posizioni[nkey].incomtg.z=(*f); f++;
	    pt->posizioni[nkey].outcomtg.x=(*f); f++;
	    pt->posizioni[nkey].outcomtg.y=(*f); f++;
	    pt->posizioni[nkey].outcomtg.z=(*f); f++;
	    pt->posizioni[nkey].tension=0;
	    pt->posizioni[nkey].continuity=0;
	    pt->posizioni[nkey].bias=0;
	    pt->posizioni[nkey].easeto=0;
	    pt->posizioni[nkey].easefrom=0;
		vect_swap_3DS(&pt->posizioni[nkey].incomtg);
		vect_swap_3DS(&pt->posizioni[nkey].outcomtg);
        pt->type=pt->type | BEZIER_TRACK;
	 }
	 else
	 if (type==LINEAR_CONTROLLER)
	 {
	    pt->posizioni[nkey].tension=1;
	    pt->posizioni[nkey].continuity=0;
	    pt->posizioni[nkey].bias=0;
	    pt->posizioni[nkey].easeto=0;
	    pt->posizioni[nkey].easefrom=0;
        pt->type=pt->type | LINEAR_TRACK;
	 }
  }
}


void FileA3D::keyframe_load_rottrack(rottrack *rt, char *chunk)
{
  int sz;
  unsigned char type, *t;
  int nkey, *v;
  float *f, angle;

  sz=strlen(chunk+4);       // punto al nome e leggo la sua lunghezza
  chunk=chunk+4+(sz+1)+4;   // punto direttamente ai dati

  t=(unsigned char *)chunk;
  type=*t;                  // leggo il tipo di controller
  t=t+1;             
  
  v=(int *)t;
  nkey=*v;                  // leggo il numero di key
  v=v+1;
  rt->numkey=nkey;
  rt->rotazioni=new ROTKEY[rt->numkey];

  f=(float *)v;
  angle=0;
  for (nkey=0; nkey<rt->numkey; nkey++)
  {
	 v=(int *)f;
	 rt->rotazioni[nkey].posintrack=*v;
	 v=v+1;

	 f=(float *)v;
	 rt->rotazioni[nkey].rotquat.x=*f; f++;
	 rt->rotazioni[nkey].rotquat.y=*f; f++;
	 rt->rotazioni[nkey].rotquat.z=*f; f++;

     if (rt->type & RELATIVE) angle = angle + (*f);
	 else angle=*f;
	 rt->rotazioni[nkey].rotquat.w=*f;
	 rt->rotazioni[nkey].roll=angle;  f++;
     quat_swap_3DS(&rt->rotazioni[nkey].rotquat);

	 if (type==TCB_CONTROLLER)
	 {
	    rt->rotazioni[nkey].tension=*f;     f++;
	    rt->rotazioni[nkey].continuity=*f;  f++;
	    rt->rotazioni[nkey].bias=*f;        f++;
	    rt->rotazioni[nkey].easeto=*f;      f++;
	    rt->rotazioni[nkey].easefrom=*f;    f++;
        rt->type=rt->type | TCB_TRACK;
	 }
	 else
	 if (type==BEZIER_CONTROLLER)
	 {
	    rt->rotazioni[nkey].tension=0;
	    rt->rotazioni[nkey].continuity=0;
	    rt->rotazioni[nkey].bias=0;
	    rt->rotazioni[nkey].easeto=0;
	    rt->rotazioni[nkey].easefrom=0;
        rt->type=rt->type | BEZIER_TRACK;
	 }
	 else
	 if (type==BEZIER_CONTROLLER)
	 {
	    rt->rotazioni[nkey].tension=1;
	    rt->rotazioni[nkey].continuity=0;
	    rt->rotazioni[nkey].bias=0;
	    rt->rotazioni[nkey].easeto=0;
	    rt->rotazioni[nkey].easefrom=0;
        rt->type=rt->type | LINEAR_TRACK;
	 }
  }
}


void FileA3D::keyframe_load_rolltrack(rolltrack *lt, char *chunk)
{
  int sz;
  unsigned char type, *t;
  int nkey, *v;
  float *f;

  sz=strlen(chunk+4);       // punto al nome e leggo la sua lunghezza
  chunk=chunk+4+(sz+1)+4;   // punto direttamente ai dati

  t=(unsigned char *)chunk;
  type=*t;              // leggo il tipo di controller
  t=t+1;
  
  v=(int *)t;
  nkey=*v; v++;             // leggo il numero di key
  lt->numkey=nkey;
  lt->rollazioni=new ROLLKEY[lt->numkey];

  f=(float *)v;
  for (nkey=0; nkey<lt->numkey; nkey++)
  {
	 v=(int *)f;
	 lt->rollazioni[nkey].posintrack=*v; v++;
	 f=(float *)v;
	 lt->rollazioni[nkey].roll=*f; f++;

	 if (type==TCB_CONTROLLER)
	 {
	    lt->rollazioni[nkey].tension=*f;     f++;
	    lt->rollazioni[nkey].continuity=*f;  f++;
	    lt->rollazioni[nkey].bias=*f;        f++;
	    lt->rollazioni[nkey].easeto=*f;      f++;
	    lt->rollazioni[nkey].easefrom=*f;    f++;
		lt->type=lt->type | TCB_TRACK;
	 }
	 else
	 if (type==BEZIER_CONTROLLER)
	 {
	    lt->rollazioni[nkey].incomtg=*f;   f++;
	    lt->rollazioni[nkey].outcomtg=*f;  f++;
		lt->type=lt->type | BEZIER_TRACK;
	 }
	 else
	 if (type==LINEAR_CONTROLLER)
	 {
	    lt->rollazioni[nkey].tension=1;
	    lt->rollazioni[nkey].continuity=0;
	    lt->rollazioni[nkey].bias=0;
	    lt->rollazioni[nkey].easeto=0;
	    lt->rollazioni[nkey].easefrom=0;
		lt->type=lt->type | LINEAR_TRACK;
	 }
  }
}


// ###########################################################
// ###########  LOAD LISTA MATERIALI (Link info)  ############
// ###########################################################

void FileA3D::materials_load_list(char *chunk)
{
  int sz, *dati;
  char *c;

  num_materials=0;  // settaggio di sicurezza

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;

  dati=(int *)chunk;
  c=(((char *)dati)+4);
  while (*dati >= 0)
  {
    materials[num_materials].global_id=*dati;
	strcpy(materials[num_materials].name, c);
	num_materials++;

    sz=strlen(c);
	c=c+(sz+1);
	dati=(int *)c;   // punto al nuovo id
	c=c+4;           // punto al nuovo nome
  }
}


char *FileA3D::get_material_name(int g_id_mat)
{
  int w;

  for (w=0; w<num_materials; w++)
  {
	if (g_id_mat==materials[w].global_id) return(materials[w].name);
  }

  return(material_no_found);
}



// ###########################################################
// ###################  LOAD DI PATCHMESH  ###################
// ###########################################################

void FileA3D::patchobject_load_mesh(AD_PatchObject *pobj, char *chunk)
{
   int is_a_children, *dati, sz, there_is_material, j, k;
   char *c;
   float *n;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   pobj->name=new char[sz+1];
   strcpy(pobj->name, chunk+4);

   dati=(int *)(chunk+4+(sz+1)+4);
   is_a_children=*dati;
   if (is_a_children)
   {
	 c=(char *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 pobj->father_name=new char[sz+1];
	 strcpy(pobj->father_name, c);
	 c=c+sz+1;
   }
   else c=(char *)(dati+1);  // non c'e' nome, quindi salto solo il flag

   dati=(int *)c;
   there_is_material=*dati;
   c=c+4;  // punto all'inizio dell'eventuale nome del materiale

   if (there_is_material)
   {
	 sz=strlen(c);
     pobj->matbase=myscene->get_material_pointer(c);
	 c=c+(sz+1);
   }

  // lettura del pivot
  n=(float *)c;
  pobj->pivot.x=*n; n++;
  pobj->pivot.y=*n; n++;
  pobj->pivot.z=*n; n++;

  // numero di vertici geometrici (corners)
  dati=(int *)n;
  pobj->num_points=*dati; dati++;
  // creo i vertici e relativi controllori
  pobj->verteces=new AD_Vect3D[pobj->num_points];
  pobj->vert_pos=new postrack[pobj->num_points];
  n=(float *)dati;
  for (j=0; j<pobj->num_points; j++)
  {
    pobj->verteces[j].x=*n; n++;
    pobj->verteces[j].y=*n; n++;
    pobj->verteces[j].z=*n; n++;
	vect_swap_3DS(&pobj->verteces[j]);
  }

  // numero di vettori geometrici
  dati=(int *)n;
  pobj->num_vectors=*dati; dati++;
  // creo i vettori e relativi controllori
  pobj->vectors=new AD_Vect3D[pobj->num_vectors];
  pobj->vect_pos=new postrack[pobj->num_vectors];
  n=(float *)dati;
  for (j=0; j<pobj->num_vectors; j++)
  {
    pobj->vectors[j].x=*n; n++;
    pobj->vectors[j].y=*n; n++;
    pobj->vectors[j].z=*n; n++;
	vect_swap_3DS(&pobj->vectors[j]);
  }

  // numero di vertici texture
  dati=(int *)n;
  pobj->num_UVverteces=*dati; dati++;
  pobj->UVverteces=new AD_VectUV[pobj->num_UVverteces];
  n=(float *)dati;
  for (j=0; j<pobj->num_UVverteces; j++)
  {
	 pobj->UVverteces[j].u=*n;    n++;
	 pobj->UVverteces[j].v=-(*n); n++;
  }

  // numero di pezze
  dati=(int *)n;
  pobj->num_patches=*dati; dati++;
  // creo le pezze
  pobj->patches=new AD_Patch[pobj->num_patches];
  for (j=0; j<pobj->num_patches; j++)
  {
	 pobj->patches[j].type=*dati; dati++;
	 if (pobj->patches[j].type != QUADRATIC_PATCH)
	   debug_error(miawin, "Load A3D error: trovata una patch non quadrata");
     
     // lettura indici vertici
	 for (k=0; k<4; k++)
	 {
	   pobj->patches[j].vert[k]=*dati;
	   dati++;
	 }
     // lettura indici vettori
	 for (k=0; k<8; k++)
	 {
	   pobj->patches[j].vect[k]=*dati;
	   dati++;
	 }
     // lettura indici interior
	 for (k=0; k<4; k++)
	 {
	   pobj->patches[j].interior[k]=*dati;
	   dati++;
	 }

     // lettura indici texture
	 if (pobj->num_UVverteces>0)
	   for (k=0; k<4; k++)
	   {
	     pobj->patches[j].UVvert[k]=*dati;
	     dati++;
	   }
  }
}


void FileA3D::keyframe_load_patchobject_vertvect_track(postrack *pt_array, char *chunk)
{
  int sz, *n, quale_controllore;
  unsigned char type, *t;
  int nkey, *v;
  float *f;
  postrack *pt;

  sz=strlen(chunk+4);       // punto al nome e leggo la sua lunghezza
  chunk=chunk+4+(sz+1)+4;   // punto direttamente ai dati

  n=(int *)chunk;           // leggo quale controllore seguira'
  quale_controllore=*n;
  pt=&pt_array[quale_controllore];
  n++;

  t=(unsigned char *)n;
  type=*t; t=t+1;             // leggo il tipo di controller
  
  v=(int *)t;
  nkey=*v; v++;             // leggo il numero di key
  pt->numkey=nkey;
  pt->posizioni=new POSKEY[pt->numkey];

  f=(float *)v;
  for (nkey=0; nkey<pt->numkey; nkey++)
  {
	 v=(int *)f;
	 pt->posizioni[nkey].posintrack=*v;
	 v=v+1;

	 f=(float *)v;
	 pt->posizioni[nkey].p.x=*f; f++;
	 pt->posizioni[nkey].p.y=*f; f++;
	 pt->posizioni[nkey].p.z=*f; f++;
     vect_swap_3DS(&pt->posizioni[nkey].p);

	 if (type==TCB_CONTROLLER)
	 {
	    pt->posizioni[nkey].tension=*f;     f++;
	    pt->posizioni[nkey].continuity=*f;  f++;
	    pt->posizioni[nkey].bias=*f;        f++;
	    pt->posizioni[nkey].easeto=*f;      f++;
	    pt->posizioni[nkey].easefrom=*f;    f++;
	 }
	 else
	 {
	    pt->posizioni[nkey].tension=0;
	    pt->posizioni[nkey].continuity=0;
	    pt->posizioni[nkey].bias=0;
	    pt->posizioni[nkey].easeto=0;
	    pt->posizioni[nkey].easefrom=0;
	 }
  }
}


// ###########################################################
// ################  LOAD DI PARTICLE SYSTEM  ################
// ###########################################################
void FileA3D::particlesystem_load_settings(AD_ParticleSystem *psys, char *chunk)
{
   int sz, *dati, is_a_children, k;
   char *c;
   float *f;
   typedef char *char_ptr;

   // settaggi iniziali per non avere casini
   psys->father=(AD_Object3D *)NULL;
   psys->flare=(texture *)NULL;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   psys->name=new char[sz+1];
   strcpy(psys->name, chunk+4);

   dati=(int *)(chunk+4+(sz+1)+4);
   is_a_children=*dati;
   if (is_a_children)
   {
	 c=(char *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 psys->father_name=new char[sz+1];
	 strcpy(psys->father_name, c);
	 c=c+sz+1;
   }
   else c=(char *)(dati+1);  // non c'e' nome, quindi salto solo il flag

   f=(float *)c;
   psys->emitter_width=*f;     f++;
   psys->emitter_height=*f;    f++;
   psys->emitter_speed=*f;     f++;
   psys->emitter_variation=*f; f++;
   dati=(int *)f;
   sz=*dati;
   psys->life=(float)sz;
   dati++;
   sz=*dati;
   psys->start_time=(float)sz;
   dati++;
   psys->max_particles=*dati; dati++;

   f=(float *)dati;
   // lettura posizione nel mondo
   psys->currentpos.x=*f; f++;
   psys->currentpos.y=*f; f++;
   psys->currentpos.z=*f; f++;
   vect_swap_3DS(&psys->currentpos);

   dati=(int *)f;
   psys->numWSM=*dati; dati++;
   psys->WSMlinked=new AD_WSMObject_ptr[psys->numWSM];
   psys->WSMNames=new char_ptr[psys->numWSM];
   c=(char *)dati;
   // leggo i nomi dei WSM bindati a questo particle system
   for (k=0; k<psys->numWSM; k++)
   {
     sz=strlen(c);
     psys->WSMNames[k]=new char[sz+1];
	 strcpy(psys->WSMNames[k], c);
	 psys->WSMlinked[k]=(AD_WSMObject *)NULL;
	 c=c+sz+1;
   }
}


// ###########################################################
// ######################  LOAD DI WSM  ######################
// ###########################################################
void FileA3D::windmodifier_load_settings(AD_WindModifier *wmod, char *chunk)
{
   int sz, *dati, is_a_children;
   char *c;
   float *f;
   typedef char *char_ptr;

   // settaggi iniziali per non avere casini
   wmod->father=(AD_Object3D *)NULL;
   wmod->flare=(texture *)NULL;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   wmod->name=new char[sz+1];
   strcpy(wmod->name, chunk+4);

   dati=(int *)(chunk+4+(sz+1)+4);
   is_a_children=*dati;
   if (is_a_children)
   {
	 c=(char *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 wmod->father_name=new char[sz+1];
	 strcpy(wmod->father_name, c);
	 c=c+sz+1;
   }
   else c=(char *)(dati+1);  // non c'e' nome, quindi salto solo il flag

   f=(float *)c;
   wmod->strenght=*f; f++;
   wmod->decay=*f; f++;
   dati=(int *)f;
   wmod->mapping=*dati; dati++;
   f=(float *)dati;
   wmod->turbolence=*f; f++;
   wmod->frequency=*f; f++;
   wmod->scale=*f; f++;
   wmod->currentpos.x=*f; f++;
   wmod->currentpos.y=*f; f++;
   wmod->currentpos.z=*f; f++;
   vect_swap_3DS(&wmod->currentpos);
}


void FileA3D::gravitymodifier_load_settings(AD_GravityModifier *gmod, char *chunk)
{
   int sz, *dati, is_a_children;
   char *c;
   float *f;
   typedef char *char_ptr;

   // settaggi iniziali per non avere casini
   gmod->father=(AD_Object3D *)NULL;
   gmod->flare=(texture *)NULL;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   gmod->name=new char[sz+1];
   strcpy(gmod->name, chunk+4);

   dati=(int *)(chunk+4+(sz+1)+4);
   is_a_children=*dati;
   if (is_a_children)
   {
	 c=(char *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 gmod->father_name=new char[sz+1];
	 strcpy(gmod->father_name, c);
	 c=c+sz+1;
   }
   else c=(char *)(dati+1);  // non c'e' nome, quindi salto solo il flag

   f=(float *)c;
   gmod->strenght=*f; f++;
   gmod->decay=*f; f++;
   dati=(int *)f;
   gmod->mapping=*dati; dati++;
   f=(float *)dati;
   gmod->currentpos.x=*f; f++;
   gmod->currentpos.y=*f; f++;
   gmod->currentpos.z=*f; f++;
   vect_swap_3DS(&gmod->currentpos);
}

// ###########################################################
// ######################  LOAD DI OSM  ######################
// ###########################################################
void FileA3D::twistmodifier_load_settings(AD_TwistModifier *tm, char *chunk)
{
   int sz, *dati;
   float *f;

   if (tm==(AD_TwistModifier *)NULL) debug_error(miawin, "Twist OSM load setting: modifier = null");

   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int *)(chunk+4+(sz+1)+4);

   f=(float *)dati;
   tm->angle=(*f)*(float)(M_PI/180.0); f++;
   tm->bias=(*f)*(float)(M_PI/180.0); f++;
   dati=(int *)f;
   tm->axis=*dati; dati++;
   f=(float *)dati;
   tm->uplim=*f; f++;
   tm->lowlim=*f; f++;
   dati=(int *)f;
   tm->dolim=*dati; dati++;
   f=(float *)dati;
   tm->center.x=*f; f++;
   tm->center.y=*f; f++;
   tm->center.z=*f; f++;
   vect_swap_3DS(&tm->center);
   // NB: salto l'ordinale
}


void FileA3D::ripplemodifier_load_settings(AD_RippleModifier *rm, char *chunk)
{
   int sz, *dati;
   float *f;

   if (rm==(AD_RippleModifier *)NULL) debug_error(miawin, "Ripple OSM load setting: modifier = null");

   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int *)(chunk+4+(sz+1)+4);

   f=(float *)dati;
   rm->amp1=*f; f++;
   rm->amp2=*f; f++;
   rm->wlen=*f; f++;
   rm->phase=*f; f++;
   rm->decay=*f; f++;
   rm->center.x=*f; f++;
   rm->center.y=*f; f++;
   rm->center.z=*f;
   vect_swap_3DS(&rm->center);
   // NB: salto l'ordinale
}


void FileA3D::bendmodifier_load_settings(AD_BendModifier *bm, char *chunk)
{
   int sz, *dati;
   float *f;
 
   if (bm==(AD_BendModifier *)NULL) debug_error(miawin, "Bend OSM load setting: modifier = null");

   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int *)(chunk+4+(sz+1)+4);

   f=(float *)dati;
   bm->angle=(*f)*(float)(M_PI/180.0); f++;
   bm->dir=(*f)*(float)(M_PI/180.0); f++;
   dati=(int *)f;
   bm->axis=*dati; dati++;
   f=(float *)dati;
   bm->uplim=*f; f++;
   bm->lowlim=*f; f++;
   dati=(int *)f;
   bm->dolim=*dati; dati++;
   f=(float *)dati;
   bm->center.x=*f; f++;
   bm->center.y=*f; f++;
   bm->center.z=*f; f++;
   vect_swap_3DS(&bm->center);
   // NB: salto l'ordinale
  return;
}


void FileA3D::meltmodifier_load_settings(AD_MeltModifier *mm, char *chunk)
{
   int sz, *dati;
   float *f;
 
   if (mm==(AD_MeltModifier *)NULL) debug_error(miawin, "Melt OSM load setting: modifier = null");

   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int *)(chunk+4+(sz+1)+4);

   f=(float *)dati;
   mm->amount=*f; f++;
   mm->spread=*f; f++;
   mm->solidity=*f; f++;
   dati=(int *)f;
   mm->axis=*dati; dati++;
   mm->negaxis=*dati; dati++;
   f=(float *)dati;
   f++;  // salto confine
   f++;  // salto zbr
   mm->center.x=*f; f++;
   mm->center.y=*f; f++;
   mm->center.z=*f; f++;
   vect_swap_3DS(&mm->center);
   // NB: salto l'ordinale
  return;
}


void FileA3D::tapermodifier_load_settings(AD_TaperModifier *tm, char *chunk)
{
   int sz, *dati;
   float *f;
 
   if (tm==(AD_TaperModifier *)NULL) debug_error(miawin, "Taper OSM load setting: modifier = null");
   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int *)(chunk+4+(sz+1)+4);

   f=(float *)dati;
   tm->amount=*f; f++;
   tm->curve=*f; f++;
   dati=(int *)f;
   tm->axis=*dati; dati++;
   tm->effectaxis=*dati; dati++;
   tm->symmetry=*dati; dati++;
   f=(float *)dati;
   tm->uplim=*f; f++;
   tm->lowlim=*f; f++;
   dati=(int *)f;
   tm->dolim=*dati; dati++;
   f=(float *)dati;
   tm->center.x=*f; f++;
   tm->center.y=*f; f++;
   tm->center.z=*f; f++;
   vect_swap_3DS(&tm->center);
   // NB: salto l'ordinale
  return;
}


void FileA3D::noisemodifier_load_settings(AD_NoiseModifier *nm, char *chunk)
{
  int sz, *dati;
  float *f;
 
  if (nm==(AD_NoiseModifier *)NULL) debug_error(miawin, "Noise OSM load setting: modifier = null");
  // punto al nome dell'oggetto a cui l'OSM e' applicato e
  // vado direttamente ai dati
  sz=strlen(chunk+4);
  dati=(int *)(chunk+4+(sz+1)+4);

  nm->seed=*dati; dati++;
  f=(float *)dati;
  nm->scale=*f; f++;
  dati=(int *)f;
  nm->fractal=*dati; dati++;
  f=(float *)dati;
  nm->rough=*f; f++;
  nm->iterations=*f; f++;
  dati=(int *)f;
  nm->animate=*dati; dati++;
  f=(float *)dati;
  nm->freq=*f; f++;
  dati=(int *)f;
  nm->phase=*dati; dati++;
  f=(float *)dati;
  nm->strength.x=*f; f++;
  nm->strength.y=*f; f++;
  nm->strength.z=*f; f++;
  vect_swap_3DS(&nm->strength);
  nm->center.x=*f; f++;
  nm->center.y=*f; f++;
  nm->center.z=*f; f++;
  vect_swap_3DS(&nm->center);
  // NB: salto l'ordinale
  return;
}


void FileA3D::stretchmodifier_load_settings(AD_StretchModifier *sm, char *chunk)
{
  int sz, *dati;
  float *f;
 
  if (sm==(AD_StretchModifier *)NULL) debug_error(miawin, "Stretch OSM load setting: modifier = null");
  // punto al nome dell'oggetto a cui l'OSM e' applicato e
  // vado direttamente ai dati
  sz=strlen(chunk+4);
  dati=(int *)(chunk+4+(sz+1)+4);

  f=(float *)dati;
  sm->stretch=*f; f++;
  sm->amplify=*f; f++;
  dati=(int *)f;
  sm->axis=*dati; dati++;
  f=(float *)dati;
  sm->uplim=*f; f++;
  sm->lowlim=*f; f++;
  dati=(int *)f;
  sm->dolim=*dati; dati++;
  f=(float *)dati;
  sm->center.x=*f; f++;
  sm->center.y=*f; f++;
  sm->center.z=*f; f++;
  vect_swap_3DS(&sm->center);
   // NB: salto l'ordinale
  return;
}


void FileA3D::ffdmodifier_load_settings(AD_FFDModifier *fm, char *chunk)
{
  int k, sz, *dati;
  float *f;

  if (fm==(AD_FFDModifier *)NULL) debug_error(miawin, "FFD OSM load setting: modifier = null");
  // punto al nome dell'oggetto a cui l'OSM e' applicato e
  // vado direttamente ai dati
  sz=strlen(chunk+4);
  dati=(int *)(chunk+4+(sz+1)+4);

  fm->dim1=*dati; dati++;
  fm->dim2=*dati; dati++;
  fm->dim3=*dati; dati++;
  fm->invol=*dati; dati++;
  fm->num_cpoints=*dati; dati++;
  f=(float *)dati;
  fm->cpoints=new AD_Vect3D[fm->num_cpoints];
  fm->cpoints_tr=new AD_Vect3D[fm->num_cpoints];
  fm->cpoints_tracks=new postrack[fm->num_cpoints];
  for (k=0; k<fm->num_cpoints; k++)
  {
	 fm->cpoints[k].x=*f; f++;
	 fm->cpoints[k].y=*f; f++;
	 fm->cpoints[k].z=*f; f++;
	 vect_swap_3DS(&fm->cpoints[k]);
  }
}


// **********************************************************
// *****************   PHYSIC MODIFIER   ********************
// **********************************************************
void FileA3D::physicmodifier_load_settings(AD_Object3D *obj, char *chunk)
{
  #define RIGID 0
  #define BLEND 1
  typedef AD_Matrix *MATRIX_PTR;
  int k, sz, *dati;
  float *f;
  char bone_name[150], *cp;
  AD_Object3D *obj_aux;

  if (obj==(AD_Object3D *)NULL) debug_error(miawin, "Physic load setting: object = null");
  // punto al nome dell'oggetto a cui il physic e' applicato e
  // vado direttamente ai dati
  sz=strlen(chunk+4);
  dati=(int *)(chunk+4+(sz+1)+4);

  k=*dati; dati++;
  if (k==BLEND) debug_error(miawin, "Physic load setting: BLEND deformation is not supported (only RIGID pls)");
  k=*dati; dati++;
  if (k != obj->num_vertex3D) debug_error(miawin, "Physic load setting: missmatch in vertex3D number");
  f=(float *)dati;

  obj->bones_matrix=new MATRIX_PTR[obj->num_vertex3D];
  obj->bones_matrix_rot=new MATRIX_PTR[obj->num_vertex3D];
  for (k=0; k<obj->num_vertex3D; k++)
  {
     // li salvo momentaneamente in points_tr perche' i punti
     // originali mi servono per ilc alcolo delle normali ai
     // vertici
	 obj->vertex3D[k].tpoint.x=*f; f++;
     obj->vertex3D[k].tpoint.z=*f; f++;
     obj->vertex3D[k].tpoint.y=*f; f++;
     cp=(char *)f;
	 strcpy(bone_name, cp);
	 obj_aux=(AD_Object3D *)NULL;
	 obj_aux=myscene->get_object_pointer(bone_name);
	 if (obj_aux!=(AD_Object3D *)NULL)
	 {
        obj->bones_matrix[k]=&obj_aux->currentmatrix;
        obj->bones_matrix_rot[k]=&obj_aux->incremental_rotmatrix;
	 }
     sz=strlen(cp)+1;
	 f=(float *)(cp+sz);
  }
}


// **********************************************************
// *****************   SKIN   MODIFIER   ********************
// **********************************************************
void FileA3D::skinmodifier_load_settings(AD_Object3D *obj, char *chunk)
{
  typedef Skin_Bone *WEIGHTED_BONE_PTR;
  int k, sz, *dati, j;
  float *f;
  char *cp;
  WEIGHTED_BONE_PTR bptr;
  short int s, *sp;

  if (obj==(AD_Object3D *)NULL) debug_error(miawin, "Skin load setting: object = null");
  sz=strlen(chunk+4);
  sp=(short int *)(chunk+4+(sz+1)+4);

  s=*sp;
  sp++;
  obj->bones_names=new Bone_Name[s];
  cp=(char *)sp;
  for (k=0; k<(int)s; k++)
  {
	 strcpy(obj->bones_names[k], cp);
	 cp=cp+strlen(cp)+1;
  }

  dati=(int *)cp;
  k=*dati;
  dati++;
  if (k != obj->num_vertex3D) debug_error(miawin, "Skin load setting: missmatch in vertex3D number");
  
  obj->skin_modifier=new WEIGHTED_BONE_PTR[obj->num_vertex3D];
  sp=(short int *)dati;
  for (k=0; k<obj->num_vertex3D; k++)
  {
	 s=*sp;
	 sp++;
     // il +1 serve per mettere una struttura Skin_Bone che ha il
	 // campo puntatore alla matrice nullo, in modo che so quando
	 // fermarmi nella sommatoria, visto che ogni vertice puo' avere
	 // un numero di ossa asseganto diverso dagl'altri
	 obj->skin_modifier[k]=new Skin_Bone[s+1];
	 bptr=obj->skin_modifier[k];
     for (j=0; j<(int)s; j++)
	 {
        // copio l'id numerico nel campo skin_matrix; cosi' in fase
		// di init lo ritrovo qui
        bptr[j].skin_matrix=(AD_Matrix *)(*sp);
		sp++;
		f=(float *)sp;
        bptr[j].weight=*f;
		f++;
		sp=(short int *)f;
	 }
     bptr[s].skin_matrix=(AD_Matrix *)NULL;
	 // il peso -1E10 mi indica quando e' finita la catena
     bptr[s].weight=-1E10;
  }
}

// ###########################################################
// #####################  LOAD DI SCENE  #####################
// ###########################################################
void FileA3D::LoadFromMemory(AD_Scene3D *s, char *ptr)
{
  int current_ID, i, j;
  char *chunk_ptr, *name_owner, ordinal, *chptr;
  int finito=0, sz, *f;
  AD_PatchObject *patch_obj;
  AD_ParticleSystem *psys_obj;
  AD_WindModifier *winmod;
  AD_GravityModifier *gravmod;
  AD_TwistModifier *twistmod;
  AD_RippleModifier *ripplemod;
  AD_BendModifier *bendmod;
  AD_MeltModifier *meltmod;
  AD_TaperModifier *tapermod;
  AD_NoiseModifier *noisemod;
  AD_StretchModifier *stretchmod;
  AD_FFDModifier *ffdmod;

  // inizializzazioni di base
  s->num_objects=0;
  s->num_camere=0;
  s->num_omni=0;
  s->num_spot=0;

  chunk_ptr=ptr;
  current_ID=*((int *)chunk_ptr);

  while (!finito)
  {
	switch (current_ID)
	{
	  case SCENE_GENERAL_INFO_ID: 
		   scene_load_general_info(s, chunk_ptr);
		   break;

      case SCENE_MATERIAL_LIST_ID:
		   materials_load_list(chunk_ptr);
		   break;

      case TRI_MESH_ID:
		   s->objects[s->num_objects]=new AD_Object3D;
           s->objects[s->num_objects]->type=TRIMESH;
		   object_load_mesh(s->objects[s->num_objects], chunk_ptr);
		   s->num_objects++;
		   break;

      case HELPER_OBJECT_ID:
		   s->objects[s->num_objects]=new AD_Object3D;
		   object_load_helper(s->objects[s->num_objects], chunk_ptr);
		   s->num_objects++;
		   break;

      case TRI_MESH_VERTEX_LIST_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0) object_load_vertex_list(s->objects[i], chunk_ptr);
		   break;

      case TRI_MESH_TEXTURE_VERTEX_LIST_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0) object_load_texture_vertex_list(s->objects[i], chunk_ptr);
		   break;

      case TRI_MESH_TEXTURE_FACES_LIST_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0) object_load_texture_face_list(s->objects[i], chunk_ptr);
		   break;

	  case TRI_MESH_FACES_LIST_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0) object_load_face_list(s->objects[i], chunk_ptr);
		   break;
     
	  case CAMERA_ID:
           camera_load_settings(&s->telecamere[s->num_camere], chunk_ptr);
           s->num_camere++;
		   break;

      case OMNI_LIGHT_ID:
		   omnilight_load_settings(&s->omni[s->num_omni], chunk_ptr);
           s->num_omni++;
           break;

      case PATCH_MESH_ID:
		   patch_obj=new AD_PatchObject;
		   s->objects[s->num_objects]=(AD_Object3D *)patch_obj;
           s->objects[s->num_objects]->type=PATCHMESH;
		   patchobject_load_mesh(patch_obj, chunk_ptr);
		   s->num_objects++;
		   break;

      case SPRAY_PARTICLE_SYSTEM_ID:
		   psys_obj=new AD_ParticleSystem;
		   s->objects[s->num_objects]=(AD_Object3D *)psys_obj;
           s->objects[s->num_objects]->type=PARTICLESYSTEM;
		   particlesystem_load_settings(psys_obj, chunk_ptr);
		   s->num_objects++;
		   break;

      case WIND_MODIFIER_ID:
		   winmod=new AD_WindModifier;
		   s->objects[s->num_objects]=(AD_Object3D *)winmod;
           s->objects[s->num_objects]->type=WINDMODIFIER;
		   windmodifier_load_settings(winmod, chunk_ptr);
		   s->num_objects++;
	       break;

      case GRAVITY_MODIFIER_ID:
		   gravmod=new AD_GravityModifier;
		   s->objects[s->num_objects]=(AD_Object3D *)gravmod;
           s->objects[s->num_objects]->type=GRAVITYMODIFIER;
		   gravitymodifier_load_settings(gravmod, chunk_ptr);
		   s->num_objects++;
	       break;
      
	  case TWIST_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             twistmod=new AD_TwistModifier;
             s->objects[i]->OSMmods[s->objects[i]->num_OSMmods]=(AD_OSMObject *)twistmod;
	         twistmodifier_load_settings(twistmod, chunk_ptr);
             s->objects[i]->num_OSMmods++;
		   }
		   break;

	  case RIPPLE_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             ripplemod=new AD_RippleModifier;
             s->objects[i]->OSMmods[s->objects[i]->num_OSMmods]=(AD_OSMObject *)ripplemod;
	         ripplemodifier_load_settings(ripplemod, chunk_ptr);
             s->objects[i]->num_OSMmods++;
		   }
		   break;

	  case BEND_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             bendmod=new AD_BendModifier;
             s->objects[i]->OSMmods[s->objects[i]->num_OSMmods]=(AD_OSMObject *)bendmod;
	         bendmodifier_load_settings(bendmod, chunk_ptr);
             s->objects[i]->num_OSMmods++;
		   }
		   break;

	  case MELT_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             meltmod=new AD_MeltModifier;
             s->objects[i]->OSMmods[s->objects[i]->num_OSMmods]=(AD_OSMObject *)meltmod;
	         meltmodifier_load_settings(meltmod, chunk_ptr);
             s->objects[i]->num_OSMmods++;
		   }
		   break;

	  case NOISE_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             noisemod=new AD_NoiseModifier;
             s->objects[i]->OSMmods[s->objects[i]->num_OSMmods]=(AD_OSMObject *)noisemod;
	         noisemodifier_load_settings(noisemod, chunk_ptr);
             s->objects[i]->num_OSMmods++;
		   }
		   break;

	  case TAPER_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             tapermod=new AD_TaperModifier;
             s->objects[i]->OSMmods[s->objects[i]->num_OSMmods]=(AD_OSMObject *)tapermod;
	         tapermodifier_load_settings(tapermod, chunk_ptr);
             s->objects[i]->num_OSMmods++;
		   }
		   break;

	  case STRETCH_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             stretchmod=new AD_StretchModifier;
             s->objects[i]->OSMmods[s->objects[i]->num_OSMmods]=(AD_OSMObject *)stretchmod;
	         stretchmodifier_load_settings(stretchmod, chunk_ptr);
             s->objects[i]->num_OSMmods++;
		   }
		   break;

	  case FFD_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             ffdmod=new AD_FFDModifier;
             s->objects[i]->OSMmods[s->objects[i]->num_OSMmods]=(AD_OSMObject *)ffdmod;
	         ffdmodifier_load_settings(ffdmod, chunk_ptr);
             s->objects[i]->num_OSMmods++;
		   }
		   break;

	  case PHYSIC_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
	          physicmodifier_load_settings(s->objects[i], chunk_ptr);
		   }
		   break;

	  case SKIN_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
	          skinmodifier_load_settings(s->objects[i], chunk_ptr);
		   }
		   break;

// ========================================================
// ###################   KEYFRAME  ########################
// ========================================================

      case POSITION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0) keyframe_load_postrack(&s->objects[i]->positiontrack, chunk_ptr);
		   break;

      case ROTATION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             if (s->objects[i]->type==BONE)
				 s->objects[i]->rotationtrack.type=ABSOLUTE_TRACK;
			 else s->objects[i]->rotationtrack.type=RELATIVE_TRACK;
			 keyframe_load_rottrack(&s->objects[i]->rotationtrack, chunk_ptr);
		   }
		   break;

      case SCALE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0) keyframe_load_postrack(&s->objects[i]->scaletrack, chunk_ptr);
		   break;

      case CAMERA_FOV_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_camera(name_owner);
		   if (i>=0) keyframe_load_rolltrack(&s->telecamere[i].fovtrack, chunk_ptr);
		   break;

      case CAMERA_ROLL_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_camera(name_owner);
		   if (i>=0)
		   {
			 keyframe_load_rolltrack(&s->telecamere[i].rollingtrack, chunk_ptr);
             for (j=0; j<s->telecamere[i].rollingtrack.numkey; j++)
				s->telecamere[i].rollingtrack.rollazioni[j].roll*=-1.0f;
		   }
		   break;

      case COLOR_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_omni(name_owner);
		   if (i>=0) keyframe_load_postrack(&s->omni[i].colortrack, chunk_ptr);
		   break;

      case LIGHT_POSITION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_omni(name_owner);
		   if (i>=0) keyframe_load_postrack(&s->omni[i].positiontrack, chunk_ptr);
		   break;

      case CAMERA_POSITION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_camera(name_owner);
		   if (i>=0) keyframe_load_postrack(&s->telecamere[i].positiontrack, chunk_ptr);
		   break;

      case CAMERA_TARGET_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_camera(name_owner);
		   if (i>=0) keyframe_load_postrack(&s->telecamere[i].targettrack, chunk_ptr);
		   break;

      case PATCH_VERTEX_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             patch_obj=(AD_PatchObject *)s->objects[i];
			 keyframe_load_patchobject_vertvect_track(patch_obj->vert_pos, chunk_ptr);
		   }
		   break;

      case PATCH_VECTOR_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
             patch_obj=(AD_PatchObject *)s->objects[i];
			 keyframe_load_patchobject_vertvect_track(patch_obj->vect_pos, chunk_ptr);
		   }
		   break;

      case MATERIAL_OPACITY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_material(name_owner);
		   if (i>=0)
		   {
             s->material_tracks[i].trasparency_track=new rolltrack;
			 keyframe_load_rolltrack(s->material_tracks[i].trasparency_track, chunk_ptr);
		     for (j=0; j<s->material_tracks[i].trasparency_track->numkey; j++)
			   s->material_tracks[i].trasparency_track->rollazioni[j].roll*=(float)(180.0/M_PI);
		   }
		   break;

	  case OSM_CENTER_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  s->objects[i]->OSMmods[ordinal]->center_track=new postrack;
			  keyframe_load_postrack(s->objects[i]->OSMmods[ordinal]->center_track, chunk_ptr);
		   }
		   break;

	  case TWIST_ANGLE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  twistmod=(AD_TwistModifier *)s->objects[i]->OSMmods[ordinal];
			  twistmod->angle_track=new rolltrack;
			  keyframe_load_rolltrack(twistmod->angle_track, chunk_ptr);
		   }
		   break;

	  case TWIST_BIAS_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  twistmod=(AD_TwistModifier *)s->objects[i]->OSMmods[ordinal];
			  twistmod->bias_track=new rolltrack;
			  keyframe_load_rolltrack(twistmod->bias_track, chunk_ptr);
		   }
		   break;

	  case TWIST_UPLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  twistmod=(AD_TwistModifier *)s->objects[i]->OSMmods[ordinal];
			  twistmod->uplim_track=new rolltrack;
			  keyframe_load_rolltrack(twistmod->uplim_track, chunk_ptr);
		   }
		   break;

	  case TWIST_LOWLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  twistmod=(AD_TwistModifier *)s->objects[i]->OSMmods[ordinal];
			  twistmod->lowlim_track=new rolltrack;
			  keyframe_load_rolltrack(twistmod->lowlim_track, chunk_ptr);
		   }
		   break;

	  case RIPPLE_AMP1_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(AD_RippleModifier *)s->objects[i]->OSMmods[ordinal];
			  ripplemod->amp1_track=new rolltrack;
			  keyframe_load_rolltrack(ripplemod->amp1_track, chunk_ptr);
		   }
		   break;

	  case RIPPLE_AMP2_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(AD_RippleModifier *)s->objects[i]->OSMmods[ordinal];
			  ripplemod->amp2_track=new rolltrack;
			  keyframe_load_rolltrack(ripplemod->amp2_track, chunk_ptr);
		   }
		   break;

	  case RIPPLE_WAVELEN_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(AD_RippleModifier *)s->objects[i]->OSMmods[ordinal];
			  ripplemod->wlen_track=new rolltrack;
			  keyframe_load_rolltrack(ripplemod->wlen_track, chunk_ptr);
		   }
		   break;

	  case RIPPLE_PHASE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(AD_RippleModifier *)s->objects[i]->OSMmods[ordinal];
			  ripplemod->phase_track=new rolltrack;
			  keyframe_load_rolltrack(ripplemod->phase_track, chunk_ptr);
		   }
		   break;

	  case RIPPLE_DECAY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(AD_RippleModifier *)s->objects[i]->OSMmods[ordinal];
			  ripplemod->decay_track=new rolltrack;
			  keyframe_load_rolltrack(ripplemod->decay_track, chunk_ptr);
		   }
		   break;

	  case BEND_ANGLE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  bendmod=(AD_BendModifier *)s->objects[i]->OSMmods[ordinal];
			  bendmod->angle_track=new rolltrack;
			  keyframe_load_rolltrack(bendmod->angle_track, chunk_ptr);
		   }
		   break;

	  case BEND_DIR_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  bendmod=(AD_BendModifier *)s->objects[i]->OSMmods[ordinal];
			  bendmod->dir_track=new rolltrack;
			  keyframe_load_rolltrack(bendmod->dir_track, chunk_ptr);
		   }
		   break;

	  case BEND_UPLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  bendmod=(AD_BendModifier *)s->objects[i]->OSMmods[ordinal];
			  bendmod->uplim_track=new rolltrack;
			  keyframe_load_rolltrack(bendmod->uplim_track, chunk_ptr);
		   }
		   break;

	  case BEND_LOWLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  bendmod=(AD_BendModifier *)s->objects[i]->OSMmods[ordinal];
			  bendmod->lowlim_track=new rolltrack;
			  keyframe_load_rolltrack(bendmod->lowlim_track, chunk_ptr);
		   }
		   break;

	  case MELT_AMOUNT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  meltmod=(AD_MeltModifier *)s->objects[i]->OSMmods[ordinal];
			  meltmod->amount_track=new rolltrack;
			  keyframe_load_rolltrack(meltmod->amount_track, chunk_ptr);
		   }
		   break;

	  case MELT_SPREAD_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  meltmod=(AD_MeltModifier *)s->objects[i]->OSMmods[ordinal];
			  meltmod->spread_track=new rolltrack;
			  keyframe_load_rolltrack(meltmod->spread_track, chunk_ptr);
		   }
		   break;

	  case STRETCH_STRETCH_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  stretchmod=(AD_StretchModifier *)s->objects[i]->OSMmods[ordinal];
			  stretchmod->stretch_track=new rolltrack;
			  keyframe_load_rolltrack(stretchmod->stretch_track, chunk_ptr);
		   }
		   break;

	  case STRETCH_AMPLIFY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  stretchmod=(AD_StretchModifier *)s->objects[i]->OSMmods[ordinal];
			  stretchmod->amplify_track=new rolltrack;
			  keyframe_load_rolltrack(stretchmod->amplify_track, chunk_ptr);
		   }
		   break;

	  case STRETCH_UPLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  stretchmod=(AD_StretchModifier *)s->objects[i]->OSMmods[ordinal];
			  stretchmod->uplim_track=new rolltrack;
			  keyframe_load_rolltrack(stretchmod->uplim_track, chunk_ptr);
		   }
		   break;

	  case STRETCH_LOWLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  stretchmod=(AD_StretchModifier *)s->objects[i]->OSMmods[ordinal];
			  stretchmod->lowlim_track=new rolltrack;
			  keyframe_load_rolltrack(stretchmod->lowlim_track, chunk_ptr);
		   }
		   break;

	  case TAPER_AMOUNT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  tapermod=(AD_TaperModifier *)s->objects[i]->OSMmods[ordinal];
			  tapermod->amount_track=new rolltrack;
			  keyframe_load_rolltrack(tapermod->amount_track, chunk_ptr);
		   }
		   break;

	  case TAPER_CURVE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  tapermod=(AD_TaperModifier *)s->objects[i]->OSMmods[ordinal];
			  tapermod->curve_track=new rolltrack;
			  keyframe_load_rolltrack(tapermod->curve_track, chunk_ptr);
		   }
		   break;

	  case TAPER_UPLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  tapermod=(AD_TaperModifier *)s->objects[i]->OSMmods[ordinal];
			  tapermod->uplim_track=new rolltrack;
			  keyframe_load_rolltrack(tapermod->uplim_track, chunk_ptr);
		   }
		   break;

	  case TAPER_LOWLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  tapermod=(AD_TaperModifier *)s->objects[i]->OSMmods[ordinal];
			  tapermod->lowlim_track=new rolltrack;
			  keyframe_load_rolltrack(tapermod->lowlim_track, chunk_ptr);
		   }
		   break;
 
      case FFD_CONTROLPOINT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-5);
		      ordinal=*chptr; chptr++; // quale modificatore
			  f=(int *)chptr;
			  j=*f; // quale control point
			  ffdmod=(AD_FFDModifier *)s->objects[i]->OSMmods[ordinal];
			  keyframe_load_postrack(&ffdmod->cpoints_tracks[j], chunk_ptr);
		   }
		   break;

      case FFD_POSITION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ffdmod=(AD_FFDModifier *)s->objects[i]->OSMmods[ordinal];
			  keyframe_load_postrack(&ffdmod->positiontrack, chunk_ptr);
		   }
		   break;

      case FFD_ROTATION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ffdmod=(AD_FFDModifier *)s->objects[i]->OSMmods[ordinal];
			  keyframe_load_rottrack(&ffdmod->rotationtrack, chunk_ptr);
		   }
		   break;

      case FFD_SCALE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ffdmod=(AD_FFDModifier *)s->objects[i]->OSMmods[ordinal];
			  keyframe_load_postrack(&ffdmod->scaletrack, chunk_ptr);
		   }
		   break;
      
	  case NOISE_STRENGTH_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  noisemod->strength_track=new postrack;
			  noisemod=(AD_NoiseModifier *)s->objects[i]->OSMmods[ordinal];
			  keyframe_load_postrack(noisemod->strength_track, chunk_ptr);
		   }
		   break;

      case NOISE_FREQ_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  noisemod=(AD_NoiseModifier *)s->objects[i]->OSMmods[ordinal];
			  noisemod->freq_track=new rolltrack;
			  keyframe_load_rolltrack(noisemod->freq_track, chunk_ptr);
		   }
		   break;

      case NOISE_PHASE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  noisemod=(AD_NoiseModifier *)s->objects[i]->OSMmods[ordinal];
			  noisemod->phase_track=new rolltrack;
			  keyframe_load_rolltrack(noisemod->phase_track, chunk_ptr);
		   }
		   break;

      case NOISE_SCALE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_object(name_owner);
		   if (i>=0)
		   {
			  sz=strlen(name_owner);
			  chptr=(char *)(chunk_ptr+4+sz+1);
			  f=(int *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char *)f;
			  chptr=(char *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  noisemod=(AD_NoiseModifier *)s->objects[i]->OSMmods[ordinal];
			  noisemod->scale_track=new rolltrack;
			  keyframe_load_rolltrack(noisemod->scale_track, chunk_ptr);
		   }
		   break;

      case END_SCENE_ID:
		   name_owner=chunk_ptr+4;
		   sz=strlen(name_owner)+1;
		   f=(int *)(name_owner+sz+4);
		   finito=*f;
		   break;
	}

    chunk_ptr=get_next_chunk(chunk_ptr);
    current_ID=*((int *)chunk_ptr);
  }
}


void FileA3D::LoadFromFile(AD_Scene3D *s, char *filename)
{
  char *dumpA3D=allocA3D(filename);

  num_materials=0;
  myscene=s;
  LoadFromMemory(s, dumpA3D);

  delete [] dumpA3D;
  myscene=(AD_Scene3D *)NULL;
  return;
}


void FileA3D::SaveToMemory(char *ptr,unsigned int *size)
{
	char *mem=new char[MAXA3DFILESIZE]; 
	int pos=0;
}


void FileA3D::SaveToFile(char *filename_owner)
{
  FILE *bfile;
  unsigned int lung,i;
  char *fileA3D=NULL;

  SaveToMemory(fileA3D, &lung);
  if ((bfile=fopen(filename_owner, "wb"))==(FILE *)0x0) return;
  for (i=0; i<lung; i++) fwrite(&fileA3D[i], 1, 1, bfile);
  fclose(bfile);
  delete [] fileA3D;
  return;
}
