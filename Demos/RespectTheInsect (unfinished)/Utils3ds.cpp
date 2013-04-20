// procedure utili per caricare roba da .3DS by HereBit

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include "utils3ds.h"
#include "shared.h"
  
extern HWND miawin;         // finestra principale; usata qui per le debug_error


// *** PROCEDURE UTILISSIME HEREBITTOLOSE ****************************************
// Here sux

unsigned short int File3DS::get_chunk_type(char *file3DS, int offset)
{ 
  short int *cktype;

  cktype=(short int *)(file3DS+offset);
  return *cktype;
}


char *File3DS::alloc3DS(char *filename)
// alloca in memoria il file aggiungendo un word di valore 0 alla fine 
// (usato per capire quando sono finiti i chunks)
// restituisce la dimensione allocata in byte o -1 se c'e' stato un errore
{
  FILE *bfile;
  int lung, i;
  char *file3DS;

  if ((bfile=fopen(filename, "rb"))==(FILE *)0x0) return (char *)-1;
  lung=filelength(fileno(bfile));
  file3DS=new char[lung+2];
  for (i=0; i<lung; i++) fread(&file3DS[i], 1, 1, bfile);
  file3DS[lung]=file3DS[lung+1]=0;
  fclose(bfile);
  return file3DS;
}


int File3DS::get_chunk_fullsize(char *file3DS, int offset)
// restituisce la dimensione del chunk + i subchunks + i 6 byte iniziali
{
  int *cksize;

  cksize=(int*)(file3DS+offset+2);
  return *cksize;
}


int File3DS::get_chunk_size(char *file3DS, int offset)
// restituisce la dimensione del chunk, esclusi i 6 byte iniziali e i subchunks
// purtroppo alcuni chunks hanno dimensione variabile, per questo servono
// i valori di *file3DS e offset ...
// ATTENZIONE: per ora ho fatto solo quelli che mi pare che ci servono!!
{  
   switch(get_chunk_type(file3DS,offset))
         // se la dimensione non è variabile restituisco il valore
         // altrimenti mi faccio i calcoli pallosi...
   {
     case MAIN3DS : return 0;
     case VERSION3DS: return 4;
     case EDIT3DS : return 0;
     case EDIT_OBJECT : { // VARIABILE!!!
                          // è costrituito da una stringa zero terminated.
                          unsigned char *tempos;
                          tempos=(unsigned char *) (file3DS+offset+6);
                          while (*tempos != 0) tempos++;
                          return (int)(tempos)-((int)(file3DS)+offset+6)+1; // ecco la dimensione!
                        }
     case OBJ_TRIMESH : return 0;
     case OBJ_LIGHT   : return 3*4;  // la tripletta float della posizione
     case TRI_VERTEXL : { // VARIABILE!!!
                          unsigned short int *vxnum;
                          vxnum=(unsigned short int *)(file3DS+offset+6);
                          return 3*4*(*vxnum)+2; // ecco la dimensione!
                        }
     case TRI_FACEL1 : { // VARIABILE!!!
                         unsigned short int *fcnum;
                         fcnum=(unsigned short int *)(file3DS+offset+6);
                         return 2*4*(*fcnum)+2; // ecco la dimensione!
                        }
     case EDIT_MATERIAL : return 0;
     
     case TRI_MATERIAL:
     case MAT_NAME : { // VARIABILE!!!
                       // è costrituito da una stringa zero terminated.
                          unsigned char *tempos;
                          tempos=(unsigned char *)(file3DS+offset+6);
                          while (*tempos != 0) tempos++;
                          return int(tempos)-((int)(file3DS)+offset+6)+1; // ecco la dimensione!
                     }

     case MAT_MAP1 : return 0;
     case MAT_MAP2 : return 0;
     case KEYF3DS : return 0; 
     case KEYF_MESH : return 0;
     case KEYF_CAMERA : return 0;
     case KEYF_TARGET : return 0;
     case KEYF_OMNI : return 0;
     case KEYF_SPOT : return 0;
     case KEYF_SPOTTARGET : return 0;


     // TODO: aggiungere man mano quelli che servono.... !

     default : return 0; // qui in realtà ci vorrebbe un messaggio di errore
   }
}


int File3DS::find_next_chunk(char *file3DS, int offset)
// restituisce l'offset del prossimo chunk di stesso livello o inferiore
// a partire da offset dove offset punta a un chunk-id
// (salta i subchunks)
// ATTENZIONE: non controlla se si va oltre la fine del file!!!
{ 
  return offset+get_chunk_fullsize(file3DS,offset);
}


int File3DS::find_next_chunk_oftype(char *file3DS, int offset, short int cktype)
// restituisce l'offset del prossimo chunk a partire da offset del tipo cktype
// dove offset punta a un chunk-id
// (salta i subchunks) e restituisce -1 se non trovato
{ 
  int myofs=offset;
  short int mytype;

  do {
       myofs=find_next_chunk(file3DS,myofs);
       mytype=get_chunk_type(file3DS,myofs);
     } while ((mytype!=cktype) && (mytype!=0));

  if (mytype==0) return -1;
     else return myofs;
}


int File3DS::find_first_subchunk(char *file3DS, int offset)
// restituisce la posizione del primo subchunk del chunk offset
// dove offset punta a un chunk-id
// se il chunk non ha subchunks ritorna -1
{ int ckfullsize,cksize;

  ckfullsize=get_chunk_fullsize(file3DS, offset); // dim con subchunks
  cksize=get_chunk_size(file3DS, offset)+6;  // dim senza subchunks

  if (ckfullsize==cksize) return -1; // non ha subchunks!

  return offset+cksize; // subchunk trovato
}


int File3DS::find_first_subchunk_oftype(char *file3DS, int offset, short int cktype)
// restituisce la posizione del primo subchunk del chunk offset
// dove offset punta a un chunk-id, del tipo cktype
// se il chunk non ha subchunks o quel tipo non viene trovato ritorna -1
{
  int myofs=find_first_subchunk(file3DS,offset);
  if (myofs==-1) return -1; // non ha subchunks
  if (get_chunk_type(file3DS,myofs)!=cktype) 
      myofs=find_next_chunk_oftype(file3DS,myofs,cktype); 

  return myofs; 
}


int File3DS::riconosci_oggetto (char *file3DS, int offset)
// riconosce il tipo di oggetto; file3ds+offset puntano ad un chunk_id
// 0x4000 (EDIT_OBJECT)
{
 int obj_size, aux, aux2;

 obj_size=get_chunk_fullsize(file3DS, offset);

 aux=find_first_subchunk_oftype(file3DS, offset, OBJ_CAMERA);
 if ((aux!=-1) && (aux<offset+obj_size)) return(CAMERA);

 aux=find_first_subchunk_oftype(file3DS, offset, OBJ_LIGHT);
 if ((aux!=-1) && (aux<offset+obj_size)) 
 {
   aux2=find_first_subchunk_oftype(file3DS, aux, OBJ_SPOTLIGHT);
   if (aux2==-1) return(OMNILIGHT);
   else return(SPOTLIGHT);

 }

 aux=find_first_subchunk_oftype(file3DS, offset, OBJ_TRIMESH);
 if ((aux!=-1) && (aux<offset+obj_size)) return(TRIMESH);

 return(-1); // oggetto sconosciuto
}


// ###########################################################
// ###################  LOAD DI TELECAMERE  ##################
// ###########################################################

void File3DS::camera_load_settings (AD_Camera *cam, char *file3DS, int offset)
{
 int dati_ofs, sz;
 float4 *n;

  if (get_chunk_type(file3DS,offset)!=EDIT_OBJECT) 
    debug_error(miawin, "camera::load_settings_3DS: Tipo di chunk errato!");

  // ora mi leggo il nome (dell'oggetto) telecamera
  sz=get_chunk_size(file3DS, offset);
  cam->name=new char[sz];
  strcpy(cam->name, (file3DS+offset+6));

  dati_ofs=find_first_subchunk_oftype(file3DS, offset, OBJ_CAMERA);
  if (dati_ofs==-1) return;

  n=(float4 *)(file3DS+dati_ofs+6);
  cam->currentpos.x=*n; n++;
  cam->currentpos.y=*n; n++;
  cam->currentpos.z=*n; n++;
  vect_swap_3DS(&cam->currentpos);

  cam->currenttarget.x=*n; n++;
  cam->currenttarget.y=*n; n++;
  cam->currenttarget.z=*n; n++;
  vect_swap_3DS(&cam->currenttarget);

  cam->currentroll=*n; n++;
  cam->currentfov=*n;
  // conversione lens->fov (e' fov/2 in radianti)
  cam->currentfov=(float)(cam->lens_to_fov(cam->currentfov)*M_PI/360.0);
  cam->currenttgfov=(float)tan(fabs(cam->currentfov));
}


void File3DS::camera_load_keytracks (AD_Camera *cam, char *file3DS, int offset)
{
  int cur_pos;

  if (get_chunk_type(file3DS,offset)!=KEYF_CAMERA)
    debug_error(miawin, "camera::loadkeytrack_3DS: Tipo di chunk errato!");

  // lettura delle eventuali tracce di posizione, roll e FOV
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_POSTRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&cam->positiontrack, file3DS, cur_pos);

  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_ROLLTRACK);
  if (cur_pos!=-1) keyframe_load_rolltrack(&cam->rollingtrack, file3DS, cur_pos);

  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_FOVTRACK);
  if (cur_pos!=-1) keyframe_load_rolltrack(&cam->fovtrack, file3DS, cur_pos);
}


void File3DS::camera_load_keytargettracks (AD_Camera *cam, char *file3DS, int offset)
{
  int cur_pos;

  if (get_chunk_type(file3DS,offset)!=KEYF_TARGET)
    debug_error(miawin, "camera::loadkeytargettrack_3DS: Tipo di chunk errato!");

  // lettura delle eventuali tracce di posizione(target)
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_POSTRACK);
  if (cur_pos!=-1) keyframe_load_postrack(&cam->targettrack, file3DS, cur_pos);
}



// ###########################################################
// ###################  LOAD DI OMNILIGHT  ###################
// ###########################################################

void File3DS::omnilight_load_settings(AD_Omnilight *ol, char *file3DS, int offset)
{
  float4 *v, *cols;
  int sz, dati, daticol;

  // settaggi iniziali flare
  ol->latoX=ol->latoY=0;
  ol->flare=(texture *)NULL;

// caricamento del nome
  sz=get_chunk_size(file3DS, offset);
  ol->name=new char[sz];
  strcpy(ol->name, (file3DS+offset+6));

  dati=find_first_subchunk_oftype(file3DS, offset, OBJ_LIGHT);
  if (dati==-1) debug_error(miawin, "Caricamento settaggi di luce omni errati !!!");

// lettura posizione
  v=(float4 *)(file3DS+dati+6);
  ol->currentpos.x=*v; v++;
  ol->currentpos.y=*v; v++;
  ol->currentpos.z=*v; v++;
  vect_swap_3DS(&ol->currentpos);

  daticol=find_first_subchunk_oftype(file3DS, dati, fCOL_RGB);
  if (daticol!=-1)
  {
    cols=(float *)(file3DS+daticol+6);
    // il range del colore e' normalizzato nell'intervallo [0;1]
    ol->currentcolor.x=(float)(RGB_MAXVALUE*(*cols)); cols++;
    ol->currentcolor.y=(float)(RGB_MAXVALUE*(*cols)); cols++;
    ol->currentcolor.z=(float)(RGB_MAXVALUE*(*cols)); cols++;
  }
}


void File3DS::omnilight_load_keytracks (AD_Omnilight *ol, char *file3DS, int offset)
{
  int cur_pos;

  // lettura tracce di posizione
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_POSTRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&ol->positiontrack, file3DS, cur_pos);

  // lettura tracce di colore
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_COLORTRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&ol->colortrack, file3DS, cur_pos);
}  



// ###########################################################
// ###################  LOAD DI SPOTLIGHT  ###################
// ###########################################################
  
void File3DS::spotlight_load_settings(AD_Spotlight *sl, char *file3DS, int offset)
{
  float4 *v, *cols;
  int dati, daticol, sz;

// caricamento del nome
  sz=get_chunk_size(file3DS, offset);
  sl->name=new char[sz];
  strcpy(sl->name, (file3DS+offset+6));

  dati=find_first_subchunk_oftype(file3DS, offset, (short)OBJ_LIGHT);
  if (dati==-1) debug_error(miawin, "Caricamento settaggi di luce spot errati !!!");

// lettura posizione
  v=(float4 *)(file3DS+dati+6);
  sl->currentpos.x=*v; v++;
  sl->currentpos.y=*v; v++;
  sl->currentpos.z=*v; v++;
  vect_swap_3DS(&sl->currentpos);

  daticol=find_first_subchunk_oftype(file3DS, dati, (short)fCOL_RGB);
  if (daticol!=-1)
  {
    cols=(float4 *)(file3DS+daticol+6);
    // il range del colore e' normalizzato nell'intervallo [0;1]
    sl->currentcolor.x=(float)(RGB_MAXVALUE*(*cols)); cols++;
    sl->currentcolor.y=(float)(RGB_MAXVALUE*(*cols)); cols++;
    sl->currentcolor.z=(float)(RGB_MAXVALUE*(*cols)); cols++;
  }

  dati=find_first_subchunk_oftype(file3DS, dati, (short)OBJ_SPOTLIGHT);
  if (dati==-1) debug_error(miawin, "Caricamento settaggi(2) di luce spot errati !!!");

  v=(float4 *)(file3DS+dati+6);
  sl->currenttarget.x=*v; v++;
  sl->currenttarget.y=*v; v++;
  sl->currenttarget.z=*v; v++;
  vect_swap_3DS(&sl->currenttarget);
  sl->currenthotspot=*v; v++;
  sl->currentfalloff=*v;
}


void File3DS::spotlight_load_keytracks (AD_Spotlight *sl, char *file3DS, int offset)
{
  int cur_pos;

  // lettura tracce di posizione
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_POSTRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&sl->positiontrack, file3DS, cur_pos);

  // lettura tracce di colore
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_COLORTRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&sl->colortrack, file3DS, cur_pos);
}


void File3DS::spotlight_load_keytargettracks (AD_Spotlight *sl, char *file3DS, int offset)
{
  int cur_pos;

  // lettura tracce di posizione
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_POSTRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&sl->targettrack, file3DS, cur_pos);
}



// ###########################################################
// ####################  LOAD DI OBJECT3D  ###################
// ###########################################################

void File3DS::object_load_vertex_list(AD_Object3D *obj, char *file3DS, int offset)
{
  int16 i, *aux, v;
  float4 x, y, z, *f;

  aux=(int16 *)(file3DS+offset+6);
  v=*aux;
  obj->num_vertex3D=(int)v;
  obj->vertex3D=new AD_Vertex3D[obj->num_vertex3D];

  for(i=0; i<obj->num_vertex3D; i++)
  {
	f=(float4 *)(file3DS+offset+8+i*12);
    x=*f;  f++;
    y=*f;  f++;
    z=*f;
    vect_set(&obj->vertex3D[i].point, x, y, z);
    vect_swap_3DS(&obj->vertex3D[i].point);
  }
}


void File3DS::object_load_face_list(AD_Object3D *obj, char *file3DS, int offset)
{
  int16 i, a, b, c, *n, v;
  int p2D;
		       
  n=(int16 *)(file3DS+offset+6);
  v=*n;
  obj->num_tria=(int)v;
  obj->tria=new AD_Tria3D[obj->num_tria];
  obj->num_vertex2D=obj->num_tria*3;
  obj->vertex2D=new AD_Vertex2D[obj->num_vertex2D];
  p2D=0;

  for(i=0; i<obj->num_tria; i++)
  {
    n=(int16 *)(file3DS+offset+8+i*8);
    a=*n; n++;
    b=*n; n++;
    c=*n;
    obj->tria[i].v1=&obj->vertex3D[a];
    obj->tria[i].v2=&obj->vertex3D[b];
    obj->tria[i].v3=&obj->vertex3D[c];
    obj->tria[i].materiale=(material *)NULL;
    obj->tria[i].sp1=&obj->vertex2D[p2D]; p2D++;
    obj->tria[i].sp2=&obj->vertex2D[p2D]; p2D++;
    obj->tria[i].sp3=&obj->vertex2D[p2D]; p2D++;
  }
}


void File3DS::object_load_material_list(AD_Object3D *obj, char *file3DS, int offset)
{
  // ATTENZIONE: i materiali devono essere già stati caricati

  int16 nentry, entry, *p, j;
  int sz;
  material *mptr;

  sz=get_chunk_size(file3DS, offset);
  mptr=myscene->get_material_pointer((file3DS+offset+6));
  if (mptr==(material *)NULL) return;

  p=(int16 *)(file3DS+offset+6+sz);
  nentry=*p;   // numero di entry
  p++;
  for (j=0; j<nentry; j++)
  {
     entry=*p;
     obj->tria[entry].materiale=mptr;
     p++;
  }
}


void File3DS::object_load_smooth_list(AD_Object3D *obj, char *file3DS, int offset)
{
  // ATTENZIONE: num_tria deve già essere settata correttamente
   if(obj->num_tria<=0) debug_error(miawin, "AD_Object3D::load3DS_smooth_list: num_tria ha un valore non valido!");

  uint32 *nn;
  int16 i;

  nn=(uint32 *)(file3DS+offset+6);
  obj->triasmoothgroup=new int[obj->num_tria];
  for(i=0; i<obj->num_tria; i++)
  {
     obj->triasmoothgroup[i]=*nn;
     nn++;
  }
}


void File3DS::object_load_mapcoords_list(AD_Object3D *obj, char *file3DS, int offset)
{
  int16 i, j, *n;
  float4 *f, u, v;

  n=(int16 *)(file3DS+offset+6);
  j=*n;
  if ((int)j!=obj->num_vertex3D) debug_error(miawin, "AD_Object3D::load3DS_mapcoord_list: num_vertex3D != numero entry della lista!");

  // convertiamo il mapping 3DS in quello di MAX
  obj->vertexUV=new AD_VectUV[obj->num_vertex3D];
  obj->num_vertexUV=obj->num_vertex3D;

  for(i=0; i<obj->num_vertexUV; i++)
  {
    f=(float4 *)(file3DS+offset+8 + i*2*sizeof(float4));
    u=*f; f++;
    v=*f;
    v=-v;
    obj->vertexUV[i].u=u;
    obj->vertexUV[i].v=v;
  }
}


void File3DS::object_load_local_coordinate_system(AD_Object3D *obj, char *file3DS, int offset)
{
  AD_Vect3D localx, localy, localz;
  AD_Vect3D origin, local;
  float4 *n;
  AD_Matrix trm;
  int16 j;

  n=(float4 *)(file3DS+offset+6);
  localx.x=*n; n++;  localx.y=*n; n++;  localx.z=*n; n++;
  localy.x=*n; n++;  localy.y=*n; n++;  localy.z=*n; n++;
  localz.x=*n; n++;  localz.y=*n; n++;  localz.z=*n; n++;
  origin.x=*n; n++;  origin.y=*n; n++;  origin.z=*n; n++;
  vect_swap_3DS(&origin);
  vect_copy(&origin, &obj->currentpos);
  mat_identity(&trm);
  mat_insert_row(&trm, 0, &localx);
  mat_insert_row(&trm, 1, &localy);
  mat_insert_row(&trm, 2, &localz);
  mat_swap_3DS(&trm);
  for (j=0; j<obj->num_vertex3D; j++)
  {
    vect_sub(&obj->vertex3D[j].point, &origin, &obj->vertex3D[j].point);
    mat_mulvect(&trm, &obj->vertex3D[j].point, &local);
    vect_copy(&local, &obj->vertex3D[j].point);
  }
}


void File3DS::object_load_dummy(AD_Object3D *obj, char *file3DS, int offset)
{
  int cur_pos;

  obj->name=new char[10];
  strcpy(obj->name, "$$$DUMMY\0");

  // lettura tracce di rotazione
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_ROTTRACK);
  if (cur_pos!=-1)  keyframe_load_rottrack(&obj->rotationtrack, file3DS, cur_pos);

  // lettura tracce di posizione
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_POSTRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&obj->positiontrack, file3DS, cur_pos);

  // lettura delle eventuali tracce di scaling
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_SCALETRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&obj->scaletrack, file3DS, cur_pos);
}


void File3DS::object_load_mesh(AD_Object3D *obj, char *file3DS, int offset)
{
  // file3DS[offset] punta al chunkID dell' Object-Block (chunk EDIT_OBJECT=0x4000)
  // ATTENZIONE: bisogna passargli un object-chunk che contiene un oggetto,
  // non una telecamera o una luce, altrimenti... ci siamo capiti :P

  int sz, meshofs, tempofs, i, tempofs2;

  // ora mi leggo il nome dell'oggetto
  sz=get_chunk_size(file3DS,offset);
  obj->name=new char[sz];
  strcpy(obj->name, (file3DS+offset+6));


  // ora cerco il subchunk Triangular-mesh (chunk OBJ_TRIMESH=0x4100)
  meshofs=find_first_subchunk_oftype(file3DS, offset, (short)OBJ_TRIMESH);

  // ora meshofs punta al Triangular-Mesh chunk
  if (meshofs==-1) debug_error(miawin, "AD_Object3D::loadmesh_3DS: Triangular-Mesh chunk non trovato!");

  // ora cerco la lista dei vertici (chunk TRI_VERTEXL=0x4110)
  tempofs=find_first_subchunk_oftype(file3DS, meshofs, (short)TRI_VERTEXL);

  // ora tempofs punta al Vertex-List chunk
  if (tempofs==-1) debug_error(miawin, "AD_Object3D::loadmesh_3DS: Vertex-List chunk non trovato!");

  //qui si deve caricare la lista di verici
  object_load_vertex_list(obj, file3DS, tempofs);

  // ora cerco la lista dei triangoli (chunk TRI_FACEL1=0x4120)
  tempofs=find_first_subchunk_oftype(file3DS, meshofs, (short)TRI_FACEL1);

  // ora tempofs punta al Face-List chunk
  if (tempofs==-1) debug_error(miawin, "AD_Object3D::loadmesh_3DS: Face-List chunk non trovato!");
  // la linea qui sopra si può togliere se si prevede il caso in cui un oggetto abbia
  // solo vertici e niente triangoli
  if (tempofs!=-1) // un oggetto potrebbe non avere triangoli! in tal caso salto pure i subc.
  {
   //qui si deve caricare la lista di facce
   object_load_face_list(obj, file3DS, tempofs);

   // ora cerco le liste dei materiali (chunk TRI_MATERIAL=0x4130)
   // che sono subchunks di TRI_FACEL1
   tempofs2=find_first_subchunk_oftype(file3DS, tempofs, (short)TRI_MATERIAL);

   // ora tempofs2 punta al primo Material-List chunk
   if(tempofs2==-1) 
   {
     // nel caso non ci sia il chunk tutti i materiali sono "Default" cioe' 0
     for (i=0; i<obj->num_tria; i++)
	 {
	   obj->tria[i].materiale=(material *)NULL;
	 }
   }
   while(tempofs2 != -1)
   {
    //qui si deve caricare la lista di materiali
    object_load_material_list(obj, file3DS, tempofs2);
    tempofs2=find_next_chunk_oftype(file3DS, tempofs2, (short)TRI_MATERIAL);
   }


  // ora cerco le liste dei gruppi di smoothing (chunk TRI_SMOOTH=0x4150)
  // che sono subchunks di TRI_FACEL1
/*  
   tempofs2=find_first_subchunk_oftype(file3DS, tempofs, TRI_SMOOTH);
  
  // ora tempofs2 punta al primo Smooth-List chunk
   if(tempofs2==-1) {
		      //nel caso non ci sia il chunk tutti gli smooth li setto a 0
		      triasmoothgroup=(uint32 *)new int32[num_tria];
		      for (i=0; i<num_tria; i++) triasmoothgroup[i]=0;
		    }
		    else  triasmoothgroup = (uint32 *)new int32[num_tria];

   while(tempofs2 != -1)
   {
     //qui si deve caricare la lista di smoothings
     load3DS_smooth_list(file3DS, tempofs2);
     tempofs2=find_next_chunk_oftype(file3DS, tempofs2, TRI_SMOOTH);
   }*/
  } 
 
  // ora cerco il Local-Coord-System (chunk TRI_LOCAL=0x4160)
  tempofs=find_first_subchunk_oftype(file3DS, meshofs, (short)TRI_LOCAL);
  if (tempofs != -1) object_load_local_coordinate_system(obj, file3DS, tempofs);

  // ora cerco la lista delle coordinate di mapping (chunk TRI_MAPCOORD=0x4140)
  tempofs=find_first_subchunk_oftype(file3DS, meshofs, (short)TRI_MAPCOORD);
  if (tempofs != -1) object_load_mapcoords_list(obj, file3DS, tempofs);
}


void File3DS::object_load_keytracks(AD_Object3D *obj, char *file3DS, int offset)
{
  int cur_pos;
  int16 m;
  float4 *tmp;

  if (get_chunk_type(file3DS,offset)!=KEYF_MESH)
    debug_error(miawin, "AD_Object3D::loadkeytrack_3DS: Tipo di chunk errato!");

  // lettura dell'eventuale pivot
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_PIVOT);
  vect_set(&obj->pivot, 0, 0, 0);
  if (cur_pos!=-1)
  {
    // settaggio del pivot
    tmp=(float4 *)(file3DS+cur_pos+6);
    obj->pivot.x=*tmp; tmp++;
	obj->pivot.y=*tmp; tmp++;
	obj->pivot.z=*tmp; tmp++;
    vect_swap_3DS(&obj->pivot);
    for (m=0; m<obj->num_vertex3D; m++) 
	   vect_sub(&obj->vertex3D[m].point, &obj->pivot, &obj->vertex3D[m].point);
 }

  // lettura delle eventuali tracce di posizione
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_POSTRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&obj->positiontrack, file3DS, cur_pos);

  // lettura delle eventuali tracce di rotazione
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_ROTTRACK);
  if (cur_pos!=-1)  keyframe_load_rottrack(&obj->rotationtrack, file3DS, cur_pos);

  // lettura delle eventuali tracce di scaling
  cur_pos=find_first_subchunk_oftype(file3DS, offset, (short)KEYF_SCALETRACK);
  if (cur_pos!=-1)  keyframe_load_postrack(&obj->scaletrack, file3DS, cur_pos);
}



// ###########################################################
// #####################  LOAD KEYFRAME  #####################
// ###########################################################

void File3DS::keyframe_load_postrack(postrack *pt, char *file3DS, int offset)
{
  // si suppone che file3DS+doffset punti ad un chunk id KEYF_POSTRACK

  int32 j, *k;
  int16 i, f, *p;
  char *aux;
  float4 *w;
  int doffset=offset+6;

  p=(int16 *)(file3DS+doffset);    pt->trackflags=*p;
  k=(int32 *)(file3DS+doffset+10); pt->numkey=*k;
  pt->posizioni=new POSKEY[pt->numkey];

  aux=file3DS+doffset+14;
  for (j=0; j<pt->numkey; j++)
  {
    k=(int32 *)(aux);  
    aux=aux+sizeof(int32);
    pt->posizioni[j].posintrack=*k;      // lettura dword
    pt->posizioni[j].tension=0.0;
    pt->posizioni[j].bias=0.0;
    pt->posizioni[j].continuity=0.0;
    pt->posizioni[j].easeto=0.0;
    pt->posizioni[j].easefrom=0.0;
    p=(int16 *)(aux);
    f=pt->posizioni[j].splineflags=*p;   // lettura word
    aux=aux+sizeof(int16);  // punta all'inizio degli eventuali parametri spline
    for (i=0; i<16; i++)
    {
      w=(float4 *)(aux);
      if (f & (1<<i))
      {
	    switch (i)
		{
	      case 0: pt->posizioni[j].tension=*w;
	      case 1: pt->posizioni[j].continuity=*w;
	      case 2: pt->posizioni[j].bias=*w;
	      case 3: pt->posizioni[j].easeto=*w;
	      case 4: pt->posizioni[j].easefrom=*w;
		}
	    aux=aux+sizeof(float4);
      }
    }  // end for lettura acceleration flag
    
    w=(float4 *)(aux);
    pt->posizioni[j].p.x=*w; aux=aux+sizeof(float4); w++;
    pt->posizioni[j].p.y=*w; aux=aux+sizeof(float4); w++;
    pt->posizioni[j].p.z=*w; aux=aux+sizeof(float4); w++;
    vect_swap_3DS(&pt->posizioni[j].p);
  } // end for lettura key
}


void File3DS::keyframe_load_rottrack(rottrack *rt, char *file3DS, int offset)
{
  // si suppone che file3DS+doffset punti a un chuck id KEYF_ROTTRACK

  int32 j, *k;
  int16 i, f, *p;
  char *aux;
  float4 *w, angle;
  int doffset=offset+6;

  if (get_chunk_type(file3DS,offset)!=KEYF_ROTTRACK)
     debug_error(miawin, "rottrack::load_3DS: Tipo di chunk errato!");

  p=(int16 *)(file3DS+doffset);    rt->trackflags=*p;
  k=(int32 *)(file3DS+doffset+10); rt->numkey=*k;
  rt->rotazioni=new ROTKEY[rt->numkey];

  angle=0.0;
  aux=file3DS+doffset+14;

  for (j=0; j<rt->numkey; j++)
  {
    k=(int32 *)(aux);  
    aux=aux+sizeof(int32);
    rt->rotazioni[j].posintrack=*k;    // lettura dword
    rt->rotazioni[j].tension=0.0;
    rt->rotazioni[j].continuity=0.0;
    rt->rotazioni[j].bias=0.0;
    rt->rotazioni[j].easeto=0.0;
    rt->rotazioni[j].easefrom=0.0;

    p=(int16 *)(aux);
    f=rt->rotazioni[j].splineflags=*p;   // lettura word
    aux=aux+sizeof(int16);  // punta all'inizio degli eventuali parametri spline
    for (i=0; i<16; i++)
    {
      w=(float4 *)(aux);
      if (f & (1<<i))
      {
	    switch (i)
		{
	      case 0: rt->rotazioni[j].tension=*w;
	      case 1: rt->rotazioni[j].continuity=*w;
	      case 2: rt->rotazioni[j].bias=*w;
	      case 3: rt->rotazioni[j].easeto=*w;
	      case 4: rt->rotazioni[j].easefrom=*w;
		}
	    aux=aux+sizeof(float4);
      }
    }  // end for lettura acceleration flag

    // gli angoli non sono assoluti, ma additivi !!!
    w=(float4 *)(aux);
    angle = angle + (*w);
    rt->rotazioni[j].roll=angle;    // serve per lo slerp
    rt->rotazioni[j].rotquat.w=*w;  aux=aux+sizeof(float4); w++;
    rt->rotazioni[j].rotquat.x=*w;  aux=aux+sizeof(float4); w++;
    rt->rotazioni[j].rotquat.y=*w;  aux=aux+sizeof(float4); w++;
    rt->rotazioni[j].rotquat.z=*w;  aux=aux+sizeof(float4); w++;
    quat_swap_3DS(&rt->rotazioni[j].rotquat);
  } // end for lettura key
}


void File3DS::keyframe_load_rolltrack(rolltrack *lt, char *file3DS, int offset)
{
  // si suppone che file3DS+doffset punti a un chuck id KEYF_ROLLRACK

  int32 j, *k;
  int16 i, f, *p;
  char *aux;
  float4 *w, angle;
  int doffset=offset+6;

//  if (get_chunk_type(file3DS,offset)!=KEYF_ROLLTRACK)
//       debug_error(miawin, "rolltrack::load_3DS: Tipo di chunk errato!");

  p=(int16 *)(file3DS+doffset);    lt->trackflags=*p;
  k=(int32 *)(file3DS+doffset+10); lt->numkey=*k;
  lt->rollazioni=new ROLLKEY[lt->numkey];

  aux=file3DS+doffset+14;
  for (j=0; j<lt->numkey; j++)
  {
    k=(int32 *)(aux);  
    aux=aux+sizeof(int32);
    lt->rollazioni[j].posintrack=*k;    // lettura dword
    lt->rollazioni[j].tension=0.0;
    lt->rollazioni[j].bias=0.0;
    lt->rollazioni[j].continuity=0.0;
    lt->rollazioni[j].easeto=0.0;
    lt->rollazioni[j].easefrom=0.0;
    p=(int16 *)(aux);
    f=lt->rollazioni[j].splineflags=*p;   // lettura word
    aux=aux+sizeof(int16);  // punta all'inizio degli eventuali parametri spline
    for (i=0; i<16; i++)
    {
      w=(float4 *)(aux);   
      if (f & (1<<i))
      {
	switch (i)
	{
	  case 0: lt->rollazioni[j].tension=*w;
	  case 1: lt->rollazioni[j].continuity=*w;
	  case 2: lt->rollazioni[j].bias=*w;
	  case 3: lt->rollazioni[j].easeto=*w;
	  case 4: lt->rollazioni[j].easefrom=*w;
	}
	aux=aux+sizeof(float4);
      }
    }  // end for lettura acceleration flag
    // lettura angolo
    w=(float4 *)(aux);   
    angle=*w;
    lt->rollazioni[j].roll=angle;  // viene messo in radianti dalla init
    aux=aux+sizeof(float4);
  } // end for lettura key
}



// ###########################################################
// #####################  LOAD DI SCENE  #####################
// ###########################################################

void File3DS::LoadFromMemory(AD_Scene3D *s, char *ptr)
{
  int edit_ofs, edit_size, cur_ofs, c2, kobj, kcam, kcamt, k1, k2, k3;
  int komni, kspot, kspott, k4, k5, k6;
  int obj_type;
  int keyf_ofs, i, i2;
  int32 *p, startf, endf;
  int16 *q;
  int16 c_id;

  // inizializzazioni di base
  s->num_objects=0;
  s->num_camere=0;
  s->num_omni=0;
  s->num_spot=0;

  // il file 3ds e' fraido se non c'e' il chunk edit3ds !!!
  if ((edit_ofs=find_first_subchunk_oftype(ptr, 0, EDIT3DS))==-1) return;
  edit_size=get_chunk_fullsize(ptr, edit_ofs);

  // gli OBJECT block sono figli di EDIT3DS
  cur_ofs=find_first_subchunk_oftype(ptr, edit_ofs, EDIT_OBJECT);
  // --------------------------------------------------------------------
  // ##########   LETTURA DEGLI OGGETTI, TELECAMERA, LUCI   #############
  // --------------------------------------------------------------------
  while ((cur_ofs!=-1) && (cur_ofs<edit_ofs+edit_size))
  {

    if ((cur_ofs!=-1) && (cur_ofs<edit_ofs+edit_size))
    {
      obj_type=riconosci_oggetto(ptr, cur_ofs);
      switch (obj_type)
      {
	    case TRIMESH  : // incrementare il numero degli oggetti, allocare
			            // memoria e leggere l'oggetto
			         s->objects[s->num_objects]=new AD_Object3D;
                     s->objects[s->num_objects]->type=TRIMESH;
			         object_load_mesh(s->objects[s->num_objects], ptr, cur_ofs);
			         s->num_objects++;
			         break;

	    case CAMERA:
                     camera_load_settings(&s->telecamere[s->num_camere], ptr, cur_ofs);
                     s->num_camere++;
			         break;

	    case OMNILIGHT: 
                     omnilight_load_settings(&s->omni[s->num_omni], ptr, cur_ofs);
                     s->num_omni++;
                     break;

        case SPOTLIGHT: 
                     spotlight_load_settings(&s->spot[s->num_spot], ptr, cur_ofs);
                     s->num_spot++;
                     break;
      }
    }
    cur_ofs=find_next_chunk_oftype(ptr, cur_ofs, EDIT_OBJECT);
  }


  // --------------------------------------------------------------------
  // ###############   CICLO DI LETTURA DELLE ANIMAZIONI   ##############
  // --------------------------------------------------------------------

//  fprintf(fdeb, "\nloading animation...\n");
//  fflush(fdeb);
  c_id=0; // settaggio per il loading delle gerarchie
  keyf_ofs=find_first_subchunk_oftype(ptr, 0, (short)KEYF3DS);
  if (keyf_ofs!=-1)
  {
    // lettura del numero di frame totali dell'animazione
    cur_ofs=find_first_subchunk_oftype(ptr, keyf_ofs, (short)KEYF_FRAMES);
    p=(int32 *)(ptr+cur_ofs+6);
    startf=*p; p++; endf=*p;
    s->num_frame=endf-startf+1;
	if (s->num_frame<=0) s->num_frame=100;

    // lettura delle animazioni degli oggetti: si cercano (e caricano) tutti
    // i meshblock del keyframer

    kobj=find_first_subchunk_oftype(ptr, keyf_ofs, (short)KEYF_MESH);
    kcam=find_first_subchunk_oftype(ptr, keyf_ofs, (short)KEYF_CAMERA);
    kcamt=find_first_subchunk_oftype(ptr, keyf_ofs, (short)KEYF_TARGET);
    komni=find_first_subchunk_oftype(ptr, keyf_ofs, (short)KEYF_OMNI);
    kspot=find_first_subchunk_oftype(ptr, keyf_ofs, (short)KEYF_SPOT);
    kspott=find_first_subchunk_oftype(ptr, keyf_ofs, (short)KEYF_SPOTTARGET);

    while ((kobj!=-1) || (kcam!=-1) || (kcamt!=-1) ||
           (komni!=-1) || (kspot!=-1) || (kspott!=-1))
    {
      k1=kobj;  if (k1==-1) k1=2000000000;
      k2=kcam;  if (k2==-1) k2=2000000000;
      k3=kcamt; if (k3==-1) k3=2000000000;
      k4=komni; if (k4==-1) k4=2000000000;
      k5=kspot; if (k5==-1) k5=2000000000;
      k6=kspott; if (k6==-1) k6=2000000000;

// #######################  LOADING OGGETTI + DUMMY  #########################
// ###########################################################################
      if ((kobj!=-1) && (kobj<k2) && (kobj<k3) && (kobj<k4) && (kobj<k5) && (kobj<k6))
      {
        // lettura nome oggetto
        c2=find_first_subchunk_oftype(ptr, kobj, (short)KEYF_OBJNAME);
        // si ricava quale oggetto e' descritto (come animazioni)
        i = s->index_of_object(ptr+c2+6); // gli si passa da dove parte
					   // la stringa del nome

        if (i==-2) // se e' un oggetto DUMMY caricalo negli oggetti
        {
          s->objects[s->num_objects]=new AD_Object3D;
          s->objects[s->num_objects]->type=DUMMY;
          s->objects[s->num_objects]->id=c_id;
	      object_load_dummy(s->objects[s->num_objects], ptr, kobj);
	      q=(int16 *)(ptr+c2+6+8+1+4);  // lunghezza parola $$$DUMMY=8
	      s->objects[s->num_objects]->id_father=*q;
          s->objects[s->num_objects]->father=(AD_Object3D *)NULL;
          s->num_objects++;
          c_id++;
        }
        else
        if (i>=0)   // se e' un oggetto valido
        {
          object_load_keytracks(s->objects[i], ptr, kobj);
          s->objects[i]->id=c_id;
          c_id++;
          i2=strlen(s->objects[i]->name);
          q=(int16 *)(ptr+c2+6+i2+1+4);
          s->objects[i]->id_father=*q;
        }

        kobj=find_next_chunk_oftype(ptr, kobj, (short)KEYF_MESH);
      }

// ##########################  LOADING TELECAMERE  ###########################
// ###########################################################################
      if ((kcam!=-1) && (kcam<k1) && (kcam<k3) && (kcam<k4) && (kcam<k5) && (kcam<k6))
      {
        // lettura camera info
        c2=find_first_subchunk_oftype(ptr, kcam, (short)KEYF_OBJNAME);
        i = s->index_of_camera(ptr+c2+6); // gli si passa da dove parte
					   // la stringa del nome
        camera_load_keytracks(&s->telecamere[i], ptr, kcam);
        c_id++;
        kcam=find_next_chunk_oftype(ptr, kcam, (short)KEYF_CAMERA);
      }

// #######################  LOADING TELECAMERE TARGET  #######################
// ###########################################################################
      if ((kcamt!=-1) && (kcamt<k1) && (kcamt<k2) && (kcamt<k4) && (kcamt<k5) && (kcamt<k6))
      {
        // lettura camera target info
        c2=find_first_subchunk_oftype(ptr, kcamt, (short)KEYF_OBJNAME);
        i = s->index_of_camera(ptr+c2+6); // gli si passa da dove parte
					   // la stringa del nome
        camera_load_keytargettracks(&s->telecamere[i], ptr, kcamt);
        c_id++;
        kcamt=find_next_chunk_oftype(ptr, kcamt, (short)KEYF_TARGET);
      }

// ########################  LOADING OMNI LIGHT(s)  ##########################
// ###########################################################################
      if ((komni!=-1) && (komni<k1) && (komni<k2) && (komni<k3) && (komni<k5) && (komni<k6))
      {
        c2=find_first_subchunk_oftype(ptr, komni, (short)KEYF_OBJNAME);
        i=s->index_of_omni(ptr+c2+6);
        omnilight_load_keytracks(&s->omni[i], ptr, komni);
        c_id++;
        komni=find_next_chunk_oftype(ptr, komni, (short)KEYF_OMNI);
      }

// ########################  LOADING SPOT LIGHT(s)  ##########################
// ###########################################################################
      if ((kspot!=-1) && (kspot<k1) && (kspot<k2) && (kspot<k3) && (kspot<k4) && (kspot<k6))
      {
        c2=find_first_subchunk_oftype(ptr, kspot, (short)KEYF_OBJNAME);
        i=s->index_of_spot(ptr+c2+6);
        spotlight_load_keytracks(&s->spot[i], ptr, kspot);
        c_id++;
        kspot=find_next_chunk_oftype(ptr, kspot, (short)KEYF_SPOT);
      }

// #####################  LOADING SPOT LIGHT(s) TARGET #######################
// ###########################################################################
      if ((kspott!=-1) && (kspott<k1) && (kspott<k2) && (kspott<k3) && (kspott<k4) && (kspott<k5))
      {
        c2=find_first_subchunk_oftype(ptr, kspott, (short)KEYF_OBJNAME);
        i=s->index_of_spot(ptr+c2+6);
        spotlight_load_keytargettracks(&s->spot[i], ptr, kspott);
        c_id++;
        kspott=find_next_chunk_oftype(ptr, kspott, (short)KEYF_SPOTTARGET);
      }
    }
  }
  else s->num_frame=1;   // se non eisistono animazioni si setta a uno il numero
                         // di frame; cosi' nella paint si otterra' sempre
                         // posizione 0
  
 
//  fprintf(fdeb, "animation loaded \n");
//  fflush(fdeb);
}


void File3DS::LoadFromFile(AD_Scene3D *s, char *filename)
{
  char *dump3ds=alloc3DS(filename);
  myscene=s;

  LoadFromMemory(s, dump3ds);
  // si libera la memoria alloca e si esce
  delete [] dump3ds;
}