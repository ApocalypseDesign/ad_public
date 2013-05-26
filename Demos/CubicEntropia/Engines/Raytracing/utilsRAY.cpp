// A3Dfile.cpp: implementation of the A3Dfile class.
//
//////////////////////////////////////////////////////////////////////
#define MAXRAYFILESIZE 1024*1024*2 // max 2 Mb

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <math.h>
#include "utilsRAY.h"
#include "..\..\utility.h"

extern HWND miawin;         // finestra principale; usata qui per le debug_error

char *FileRAY::allocRAY(char *filename)
// carica in memoria il file .RAY
// restituisce la dimensione allocata in byte o -1 se c'e' stato un errore
{
  FILE *bfile;
  int lung, i;
  char *fileRAY;

  if ((bfile=fopen(filename, "rb"))==(FILE *)0x0) return (char *)-1;
  lung=filelength(fileno(bfile));
  fileRAY=new char[lung];
  for (i=0; i<lung; i++) 
	 fread(&fileRAY[i], 1, 1, bfile);

  fclose(bfile);
  return fileRAY;
}


char *FileRAY::get_next_chunk(char *current_chunk)
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


void FileRAY::scene_load_general_info(AD_RaytScene *s, char *chunk)
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

void FileRAY::object_load_sphere(AD_RaytSphere *sphere, char *chunk)
{
   int is_a_children, *dati, sz, there_is_material;
   char *c;
   float *n;
   AD_Vect3D localx, localy, localz;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   sphere->name=new char[sz+1];
   strcpy(sphere->name, chunk+4);

   dati=(int *)(chunk+4+(sz+1)+4);
   is_a_children=*dati;
   if (is_a_children)
   {
	 c=(char *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 sphere->father_name=new char[sz+1];
	 strcpy(sphere->father_name, c);
	 c=c+sz+1;
   }
   else c=(char *)(dati+1);  // non c'e' nome, quindi salto solo il flag

   dati=(int *)c;
   there_is_material=*dati;
   c=c+4;  // punto all'inizio dell'eventuale nome del materiale

   if (there_is_material)
   {
	 sz=strlen(c);
     sphere->mymat=myscene->get_material_pointer(c);
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

  sphere->currentpos.x=*n; n++;
  sphere->currentpos.y=*n; n++;
  sphere->currentpos.z=*n; n++;
  vect_swap_3DS(&sphere->currentpos);

  // lettura pivot
  sphere->pivot.x=*n; n++;
  sphere->pivot.y=*n; n++;
  sphere->pivot.z=*n; n++;
  vect_swap_3DS(&sphere->pivot);

  // lettura raggio
  sphere->radius=*n;
}


void FileRAY::object_load_box(AD_RaytBox *box, char *chunk)
{
   int is_a_children, *dati, sz, there_is_material;
   char *c;
   float *n;
   AD_Vect3D localx, localy, localz;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   box->name=new char[sz+1];
   strcpy(box->name, chunk+4);

   dati=(int *)(chunk+4+(sz+1)+4);
   is_a_children=*dati;
   if (is_a_children)
   {
	 c=(char *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 box->father_name=new char[sz+1];
	 strcpy(box->father_name, c);
	 c=c+sz+1;
   }
   else c=(char *)(dati+1);  // non c'e' nome, quindi salto solo il flag

   dati=(int *)c;
   there_is_material=*dati;
   c=c+4;  // punto all'inizio dell'eventuale nome del materiale

   if (there_is_material)
   {
	 sz=strlen(c);
     box->mymat=myscene->get_material_pointer(c);
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

  box->currentpos.x=*n; n++;
  box->currentpos.y=*n; n++;
  box->currentpos.z=*n; n++;
  vect_swap_3DS(&box->currentpos);

  // lettura pivot
  box->pivot.x=*n; n++;
  box->pivot.y=*n; n++;
  box->pivot.z=*n; n++;
  vect_swap_3DS(&box->pivot);

  // lettura 3 dimensioni
  box->lenght=*n; n++;
  box->width=*n;  n++;
  box->height=*n;
}


void FileRAY::object_load_plane(AD_RaytPlane *plane, char *chunk)
{
   int is_a_children, *dati, sz, there_is_material;
   char *c;
   float *n;
   AD_Vect3D localx, localy, localz;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   plane->name=new char[sz+1];
   strcpy(plane->name, chunk+4);

   dati=(int *)(chunk+4+(sz+1)+4);
   is_a_children=*dati;
   if (is_a_children)
   {
	 c=(char *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 plane->father_name=new char[sz+1];
	 strcpy(plane->father_name, c);
	 c=c+sz+1;
   }
   else c=(char *)(dati+1);  // non c'e' nome, quindi salto solo il flag

   dati=(int *)c;
   there_is_material=*dati;
   c=c+4;  // punto all'inizio dell'eventuale nome del materiale

   if (there_is_material)
   {
	 sz=strlen(c);
     plane->mymat=myscene->get_material_pointer(c);
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

  plane->currentpos.x=*n; n++;
  plane->currentpos.y=*n; n++;
  plane->currentpos.z=*n; n++;
  vect_swap_3DS(&plane->currentpos);

  // lettura pivot
  plane->pivot.x=*n; n++;
  plane->pivot.y=*n; n++;
  plane->pivot.z=*n; n++;
  vect_swap_3DS(&plane->pivot);

  // lettura 2 dimensioni
  plane->lenght=*n; n++;
  plane->width=*n;
}

void FileRAY::object_load_cone(AD_RaytCone *cone, char *chunk)
{
   return;
}


// ###########################################################
// ###################  LOAD DI TELECAMERE  ##################
// ###########################################################
void FileRAY::camera_load_settings (AD_Camera *cam, char *chunk)
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

void FileRAY::omnilight_load_settings(AD_Omnilight *ol, char *chunk)
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

  ol->currentcolor.x=(float)((*f)); f++;
  ol->currentcolor.y=(float)((*f)); f++;
  ol->currentcolor.z=(float)((*f)); f++;

  ol->currentpos.x=*f; f++;
  ol->currentpos.y=*f; f++;
  ol->currentpos.z=*f; f++;
  vect_swap_3DS(&ol->currentpos);
}


// ###########################################################
// #####################  LOAD KEYFRAME  #####################
// ###########################################################

void FileRAY::keyframe_load_postrack(postrack *pt, char *chunk)
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
	    pt->posizioni[nkey].incomtg.x=*f; f++;
	    pt->posizioni[nkey].incomtg.y=*f; f++;
	    pt->posizioni[nkey].incomtg.z=*f; f++;
	    pt->posizioni[nkey].outcomtg.x=*f; f++;
	    pt->posizioni[nkey].outcomtg.y=*f; f++;
	    pt->posizioni[nkey].outcomtg.z=*f; f++;
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


void FileRAY::keyframe_load_rottrack(rottrack *rt, char *chunk)
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


void FileRAY::keyframe_load_rolltrack(rolltrack *lt, char *chunk)
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

void FileRAY::materials_load_list(char *chunk)
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


char *FileRAY::get_material_name(int g_id_mat)
{
  int w;

  for (w=0; w<num_materials; w++)
  {
	if (g_id_mat==materials[w].global_id) return(materials[w].name);
  }
  return((char*)NULL);
}


// ###########################################################
// #####################  LOAD DI SCENE  #####################
// ###########################################################
void FileRAY::LoadFromMemory(AD_RaytScene *s, char *ptr)
{
  int current_ID, i, j;
  char *chunk_ptr, *name_owner;
  int finito=0, sz, *f;
  AD_RaytSphere *sphere;
  AD_RaytBox *box;
  AD_RaytPlane *plane;
  AD_RaytCone *cone;

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

      case RAY_SPHERE_ID:
		   sphere=new AD_RaytSphere;
		   s->objects[s->num_objects]=(AD_RaytObject *)sphere;
           //s->objects[s->num_objects]->type=TRIMESH;
		   object_load_sphere(sphere, chunk_ptr);
		   s->num_objects++;
		   break;

      case RAY_BOX_ID:
		   box=new AD_RaytBox;
		   s->objects[s->num_objects]=(AD_RaytObject *)box;
           //s->objects[s->num_objects]->type=TRIMESH;
		   object_load_box(box, chunk_ptr);
		   s->num_objects++;
		   break;

      case RAY_PLANE_ID:
		   plane=new AD_RaytPlane;
		   s->objects[s->num_objects]=(AD_RaytObject *)plane;
           //s->objects[s->num_objects]->type=TRIMESH;
		   object_load_plane(plane, chunk_ptr);
		   s->num_objects++;
		   break;

	  case CAMERA_ID:
           camera_load_settings(&s->telecamere[s->num_camere], chunk_ptr);
           s->num_camere++;
		   break;

      case OMNI_LIGHT_ID:
		   omnilight_load_settings(&s->omni[s->num_omni], chunk_ptr);
           s->num_omni++;
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
			 s->objects[i]->rotationtrack.type=RELATIVE_TRACK;
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


void FileRAY::LoadFromFile(AD_RaytScene *s, char *filename)
{
  char *dumpRAY=allocRAY(filename);

  num_materials=0;
  myscene=s;
  LoadFromMemory(s, dumpRAY);

  delete [] dumpRAY;
  myscene=(AD_RaytScene *)NULL;
  return;
}


void FileRAY::SaveToMemory(char *ptr,unsigned int *size)
{
	char *mem=new char[MAXRAYFILESIZE]; 
	int pos=0;
}


void FileRAY::SaveToFile(char *filename_owner)
{
  FILE *bfile;
  unsigned int lung,i;
  char *fileRAY=NULL;

  SaveToMemory(fileRAY, &lung);
  if ((bfile=fopen(filename_owner, "wb"))==(FILE *)0x0) return;
  for (i=0; i<lung; i++) fwrite(&fileRAY[i], 1, 1, bfile);
  fclose(bfile);
  delete [] fileRAY;
  return;
}
