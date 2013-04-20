#include <windows.h>
#include <conio.h>
#include "..\scene3D.h"
#include "..\object3d.h"
#include "..\lights.h"
#include "..\..\Math\fastmath.h"
#include "..\utilsA3D.h"
#include "..\utilsscript.h"


extern FILE *fdeb;
extern HWND miawin;         // finestra principale; usata qui per le debug_error

int counter_for_resetting_vertex;
AD_Tria3D **list_to_paint_trasparent;  // lista dei triangoli trasparenti
AD_Tria3D **list_to_paint_trasparent_copy;
AD_Tria3D **list_to_paint;
AD_Tria3D *swap_tria;  // variabile utilizzata nello swapping del quicksort
int32 num_to_paint_trasparent, num_to_paint;

// sono in triangle.cpp
extern float zfar, znear, rcpznear, screen_Xadd, screen_Yadd;
extern float inv_zfar_znear;
extern int currentmaterial_flags;
extern DrawTriangleProc currentmaterial_drawproc;


// #########################################################
// procedure per l'estrazione di singole entita' della scena
// #########################################################


int AD_Scene3D::index_of_object(char *stringa0)
// ritorna l'indice(la posizione nell'array) dell'oggetto che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int k;

  if (strcmp(stringa0, "$$$DUMMY")==0) return(-2);
  for (k=0; k<num_objects; k++)
  {
    if (strcmp(objects[k]->name, stringa0)==0) return(k);
  }

  return(-1);
}


int AD_Scene3D::index_of_camera(char *stringa0)
// ritorna l'indice(la posizione nell'array) della telecamera che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int k;

  for (k=0; k<num_camere; k++)
  {
    if (strcmp(telecamere[k].name, stringa0)==0) return(k);
  }
  return(-1);
}


int AD_Scene3D::index_of_omni(char *stringa0)
// ritorna l'indice(la posizione nell'array) della luce omni che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int k;

  for (k=0; k<num_omni; k++)
  {
    if (strcmp(omni[k].name, stringa0)==0) return(k);
  }
  return(-1);
}


int AD_Scene3D::index_of_spot(char *stringa0)
// ritorna l'indice(la posizione nell'array) della luce spot che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int k;

  for (k=0; k<num_spot; k++)
  {
    if (strcmp(spot[k].name, stringa0)==0) return(k);
  }
  return(-1);
}


int AD_Scene3D::index_of_material(char *stringa0)
{
  int k;

  for (k=0; k<num_materials; k++)
  {
    if (strcmp(materials[k].name, stringa0)==0) return(k);
  }
  return(-1);
}


int AD_Scene3D::index_of_texture(char *stringa0)
{
  int k;

  for (k=0; k<num_textures; k++)
  {
    if (strcmp(textures[k].name, stringa0)==0) return(k);
  }
  return(-1);
}


AD_Object3D* AD_Scene3D::get_object_pointer (char *stringa0)
{
  int j;

  j=0; 
  while (j<num_objects)
  {
    if (strcmp(stringa0, objects[j]->name)==0)
        return(objects[j]);
    j++;
  }
  return((AD_Object3D *)NULL);
}


AD_Camera* AD_Scene3D::get_camera_pointer (char *stringa0)
{
  int j;

  j=0; 
  while (j<num_camere)
  {
    if (strcmp(stringa0, telecamere[j].name)==0)
        return(&telecamere[j]);
    j++;
  }
  return((AD_Camera *)NULL);
}


AD_Omnilight* AD_Scene3D::get_omni_pointer (char *stringa0)
{
  int j;

  j=0; 
  while (j<num_omni)
  {
    if (strcmp(stringa0, omni[j].name)==0)
        return(&omni[j]);
    j++;
  }
  return((AD_Omnilight *)NULL);
}


AD_Spotlight* AD_Scene3D::get_spot_pointer (char *stringa0)
{
  int j;

  j=0; 
  while (j<num_spot)
  {
    if (strcmp(stringa0, spot[j].name)==0)
        return(&spot[j]);
    j++;
  }
  return((AD_Spotlight *)NULL);
}


texture* AD_Scene3D::get_texture_pointer (char *stringa0)
{
  int j;

  
  // controllo nelle texture dei materiali
  j=0; 
  while (j<num_textures)
  {
    if (strcmp(stringa0, textures[j].name)==0)
        return(&textures[j]);
    j++;
  }

  // controllo nelle texture dei flare delle luci
  for (j=0; j<num_omni; j++)
  {
	if ((omni[j].flare != (texture *)NULL) &&
		(strcmp(omni[j].flare->name, stringa0)==0))
	      return(omni[j].flare);
  }

  // controllo nelle texture dei flare degli oggetti (dummy)
  for (j=0; j<num_objects; j++)
  {
	if ((objects[j]->flare != (texture *)NULL) &&
		(strcmp(objects[j]->flare->name, stringa0)==0))
	      return(objects[j]->flare);
  }

  return((texture *)NULL);
}


material* AD_Scene3D::get_material_pointer (char *stringa0)
{
  int j;

  j=0; 
  while (j<num_materials)
  {
    if (strcmp(stringa0, materials[j].name)==0)
        return(&materials[j]);
    j++;
  }
  return((material *)NULL);
}


void AD_Scene3D::add_auxiliary_materials(void)
{
  int i;
  material *mataux;

  // aggiungiamo un materiale per i tipi di rendering
  // flat e RGB opaco
  mataux=new material[num_materials+2];
  for (i=0; i <num_materials; i++) mataux[i]=materials[i];
  if (materials!=(material *)NULL) delete [] materials;
  materials=mataux;

  // materiale flat
  materials[num_materials].name=new char[5];
  strcpy(materials[num_materials].name, "FLAT");
  materials[num_materials].trasparencytype=OFF;
  materials[num_materials].flags=PAINT_FLAT;
  materials[num_materials].zsorting=OFF;
  AddUpdate_Material(&materials[num_materials]);

  // materiale RGB opaco
  materials[num_materials+1].name=new char[4];
  strcpy(materials[num_materials+1].name, "RGB");
  materials[num_materials+1].trasparencytype=OFF;
  materials[num_materials+1].flags=PAINT_LIGHT;
  materials[num_materials+1].zsorting=OFF;
  AddUpdate_Material(&materials[num_materials+1]);
}


int AD_Scene3D::load_scene(char *nfile)
{
  char fileA3D[150]="";
  char fileADM[150]="";
  char fileADL[150]="";
  char fileADD[150]="";
  char fileADO[150]="";
  char fileADP[150]="";
  FileA3D fA3D;
/*
  fprintf(fdeb,"################################\n");
  fprintf(fdeb,"Printing debug information\n");
  fprintf(fdeb,"################################\n\n");
  fflush(fdeb);
*/
  strcpy(fileA3D, nfile); strcat(fileA3D, ".A3D");
  strcpy(fileADM, nfile); strcat(fileADM, ".ADM");
  strcpy(fileADL, nfile); strcat(fileADL, ".ADL");
  strcpy(fileADD, nfile); strcat(fileADD, ".ADD");
  strcpy(fileADO, nfile); strcat(fileADO, ".ADO");
  strcpy(fileADP, nfile); strcat(fileADP, ".ADP");
  material_tracks=(material_parameters_tracks *)NULL;

  if (esiste_file(fileADM)) load_ADM(fileADM, this);
  add_auxiliary_materials();  // aggiungiamo i materiali flat
                              // ed RGB opaco
  if (esiste_file(fileA3D))
  {
    fA3D.LoadFromFile(this, fileA3D);
  }
  else debug_error(miawin, "File .A3D not present!");

  if (esiste_file(fileADL)) load_ADL(fileADL, this);
  if (esiste_file(fileADD)) load_ADD(fileADD, this);
  if (esiste_file(fileADO)) load_ADO(fileADO, this);
  if (esiste_file(fileADP)) load_ADP(fileADP, this);

  return(1);
}


void AD_Scene3D::init_objects(void)
{
  int i, j, patch_tria, psys_tria, k;
  int num_tot_tria_trasparent;
  AD_Object3D *fptr;
  AD_PatchObject *pobj;
  AD_ParticleSystem *psys;
  Skin_Bone *sb;
  short int bi;

  num_tot_tria_trasparent=0;

  for (i=0; i<num_objects; i++)
  {
    objects[i]->num_omni=num_omni;
    objects[i]->num_spot=num_spot;

	if (objects[i]->type==WINDMODIFIER) {}
	if (objects[i]->type==GRAVITYMODIFIER) {}

	if (objects[i]->type==PARTICLESYSTEM)
	{
	   psys=(AD_ParticleSystem *)objects[i];
	   psys_tria=psys->max_particles*2;
       num_tot_tria_trasparent+=psys_tria;
	   for (j=0; j<psys->numWSM; j++)
	   {
		 psys->WSMlinked[j]=(AD_WSMObject *)get_object_pointer(psys->WSMNames[j]);
	   }
	}
    else
	if (objects[i]->type==PATCHMESH)
	{
	   pobj=(AD_PatchObject *)objects[i];
	   patch_tria=(int)((pobj->u_evaluations-1)*(pobj->v_evaluations-1)*2.0f*pobj->num_patches);

	   if ((pobj->matbase==(material *)NULL) && (num_omni<=0))
		    pobj->matbase=&materials[num_materials];
	   if ((pobj->matbase==(material *)NULL) && (num_omni>0))
		    pobj->matbase=&materials[num_materials+1];

	   if (pobj->matbase->trasparencytype!=OFF)
           num_tot_tria_trasparent+=patch_tria;
	    else pobj->matbase->my_num_tria+=patch_tria;
	}
    else
	if (objects[i]->type==TRIMESH)
	{
	  // se e' una TRIMESH conosco gia' il numero di triangoli
	  // massimo (LOD level 0) assieme al loro materiale
	  for (j=0; j<objects[i]->num_tria; j++)
	  {
	    // caso del materiale solo env per tutto l'oggetto (A3D)
	    if ((objects[i]->tria[j].materiale==(material *)NULL) &&
		    (objects[i]->matbase!=(material *)NULL))
             objects[i]->tria[j].materiale=objects[i]->matbase;
		else
	    if ((objects[i]->tria[j].materiale==(material *)NULL) && (num_omni<=0))
             objects[i]->tria[j].materiale=&materials[num_materials];
        else
	    if ((objects[i]->tria[j].materiale==(material *)NULL) && (num_omni>0))
		     objects[i]->tria[j].materiale=&materials[num_materials+1];
        
		if (objects[i]->tria[j].materiale->trasparencytype!=OFF)
           num_tot_tria_trasparent++;
	    else objects[i]->tria[j].materiale->my_num_tria++;
	  }
	  // sistemo il link delle ossa dello skin
	  if (objects[i]->skin_modifier!=(Skin_Bone **)NULL)
	  {
		 for (j=0; j<objects[i]->num_vertex3D; j++)
		 {
			sb=objects[i]->skin_modifier[j];
		    k=0;
		    while (sb[k].weight!=-1E10)
			{
			   AD_Object3D *so;

               bi=(short int)(sb[k].skin_matrix);
			   so=get_object_pointer(objects[i]->bones_names[bi]);
			   // informo l'osso chis ta skinnando
			   so->skinned_object=objects[i];
			   // collego la matrice giusta
               sb[k].skin_matrix=&so->skin_matrix;
			   k++;
			}
		 }
		 delete [] objects[i]->bones_names;
	  }
	}
	else
	if (objects[i]->type==DUMMY) num_tot_tria_trasparent+=2;
		  

    // il linking padre-figlio e' na cazzata col A3D
    // NB : nel .A3D gli oggetti sono presenti e caricati gia'
    // nell'ordine gerarchico giusto, quindi non devo sortarli
	// come faccio nella Init_from_3DS
    if (objects[i]->father_name != (char *)NULL)
	{
	  fptr=get_object_pointer(objects[i]->father_name);
      objects[i]->father=fptr;
	  if (fptr!=(AD_Object3D *)NULL) fptr->have_childrens++;
	}
  }

  // questa init non la posso fare prima di contare il numero
  // di tria trasparenti e non a causa della split_tria_list
  for (i=0; i<num_objects; i++)
	if (objects[i]->init()==-1) return;


  // aggiungo anche i 2 triangoli delle luci per i flare
  num_tot_tria_trasparent+=num_omni*2;
  num_tot_tria_trasparent+=2;  // per sicurezza
  list_trasparent=new AD_Tria3DPtr[num_tot_tria_trasparent];
}


void AD_Scene3D::init_camere(void)
{
  int i, j;
  AD_Camera caux;

  for (j=0; j<num_camere; j++)
  {
	  if (telecamere[j].init(image)==-1) return;
  }

  // ordino le telecamere in ordine di ultima-key crescente
  // per eseguire il multicamera in maniera corretta
  for (i=0; i<num_camere-1; i++)
  {
	for (j=i+1; j<num_camere; j++)
	{
	  if (telecamere[i].lastkey_pos > telecamere[j].lastkey_pos)
	  {
		caux=telecamere[i];
		telecamere[i]=telecamere[j];
		telecamere[j]=caux;
	  }
	}
  }
}


void AD_Scene3D::init_materials(void)
{
  int i;

  // settiamo i materiali per essere disegnati secondo
  // il metodo previsto/scriptato
  for (i=0; i<num_materials; i++)
  {
	if ((num_omni+num_spot)>0)
	{
	  if (materials[i].lightmixtype!=LIGHTMIXTYPE_NONE)
            materials[i].flags |= PAINT_LIGHT;
	}
    if (material_tracks) material_tracks[i].init();
    AddUpdate_Material(&materials[i]);
  }
}


void AD_Scene3D::init_lights(void)
{
  int j;

  for (j=0; j<num_omni; j++) omni[j].init();
  for (j=0; j<num_spot; j++) spot[j].init();
}


void AD_Scene3D::init (void)
{
  int k;

  init_materials();
  init_lights();
  init_camere();
  init_objects();

  for (k=0; k<num_materials+2; k++)
  {
	if (materials[k].trasparencytype==OFF)
	{
	  materials[k].my_tria_list=(void **) new AD_Tria3DPtr[materials[k].my_num_tria];
	  materials[k].my_tria_list_copy=materials[k].my_tria_list;
	}
  }

  init_fast_math_library();
  counter_for_resetting_vertex=0;
  zclear=-1;
  return;
}


void AD_Scene3D::choose_current_camera (float4 pos)
// sceglie la telecamera per il frame corrente
{
  int cc, flag=0;

  cc=0;
  while ((cc<num_camere) && (!flag))
  {
    if (telecamere[cc].lastkey_pos<=pos) cc++;
    else flag=1;
  }

  if (flag==0) current_telecamera=&telecamere[num_camere-1];
  else current_telecamera=&telecamere[cc];

  zfar=current_telecamera->zfar;
  znear=current_telecamera->znear;
  rcpznear=1.0f/znear;
  inv_zfar_znear=1.0f/(zfar-znear);
}


void qsort_trasparent_tria (int lo, int hi)
// questa procedura fa il quick sort dei triangoli ordinandoli in base allo
// zmedio; per ordinare tutti i triangoli qsort_triangoli(0, num_tria-1)
// N.B.: i triangoli trasparenti DEVONO essere ordinati dal
// piu' lontano al piu' vicino e apintati in quest'ordine
{
  int i, j;
  float4 zq;
  
  i = lo;
  j = hi;
  // (lo+hi) >> 1 e' l'indice medio tra lo e hi
  zq = list_to_paint_trasparent_copy[(lo+hi) >> 1]->mid_z;
  do
  {
//    while (list_to_paint_trasparent_copy[i]->mid_z < zq) i++;
//	while (zq < list_to_paint_trasparent_copy[j]->mid_z) j--;
    while (list_to_paint_trasparent_copy[i]->mid_z > zq) i++;
	while (zq > list_to_paint_trasparent_copy[j]->mid_z) j--;
	if (i <= j) 
	{
      swap_tria = list_to_paint_trasparent_copy[i];
      list_to_paint_trasparent_copy[i] = list_to_paint_trasparent_copy[j];
      list_to_paint_trasparent_copy[j] = swap_tria;
      i++;
      j--;
	}
  } while (i<=j);
  if (lo<j) qsort_trasparent_tria(lo, j);
  if (i<hi) qsort_trasparent_tria(i, hi);
}


void qsort_nontrasparent_tria (int lo, int hi)
// questa procedura fa il quick sort dei triangoli ordinandoli in base allo
// zmedio; per ordinare tutti i triangoli qsort_triangoli(0, num_tria-1)
// N.B.: i traingoli NON trasparenti vengono ordinati dal piu'
// vicino al piu' lontano per ottimizzare lo zbuffer
{
  int iiii, jjjj;
  float4 zq;
  
  iiii = lo;
  jjjj = hi;
  // (lo+hi) >> 1 e' l'indice medio tra lo e hi
  zq = list_to_paint[(lo+hi) >> 1]->mid_z;
  do
  {
    while (list_to_paint[iiii]->mid_z < zq) iiii++;
	while (zq < list_to_paint[jjjj]->mid_z) jjjj--;
	if (iiii <= jjjj) 
	{
      swap_tria = list_to_paint[iiii];
      list_to_paint[iiii] = list_to_paint[jjjj];
      list_to_paint[jjjj] = swap_tria;
      iiii++;
      jjjj--;
	}
  } while (iiii<=jjjj);
  if (lo<jjjj) qsort_nontrasparent_tria(lo, jjjj);
  if (iiii<hi) qsort_nontrasparent_tria(iiii, hi);
}


void AD_Scene3D::do_efx2D(double pos)
// si suppone pos compreso tra [0..1]
{
  float4 frame, ft;
  AD_Tria3D *t;
  int j, m, num_tria;

  frame=(float)( ((pos-fstartpos)/(fendpos-fstartpos))
	              *(num_frame-1));

  // consideriamo la pos nel range [0;1]
  pos=(((float)(pos)-fstartpos)/(fendpos-fstartpos));

  num_to_paint_trasparent=0;
  list_to_paint_trasparent=list_trasparent;
  list_to_paint_trasparent_copy=list_trasparent;
  for (j=0; j<num_materials; j++) 
  {
	 ft=128;
     materials[j].my_tria_list=materials[j].my_tria_list_copy;
	 // se il materiale e' trasparente non additivo
	 if ((materials[j].flags & IS_TRASPARENT) &&
		 (materials[j].trasparencytype!=ALPHA_MAP))
	 {
		 if ((material_tracks) &&
			 (material_tracks[j].trasparency_track) &&
			 (material_tracks[j].trasparency_track->numkey>1)
			 )
		 {
           material_tracks[j].trasparency_track->get_data(frame, &ft);
		   if (ft<0) ft=0;
           if (ft>255) ft=255;
		   materials[j].trasparency=(int)ft;
		 }
	 }
  }
  for (j=num_materials; j<num_materials+2; j++) 
     materials[j].my_tria_list=materials[j].my_tria_list_copy;
  

  // PIPELINE GEOMETRICA
  screen_Xadd=((float)image->width)/2.0f;
  screen_Yadd=((float)image->height)/2.0f;
  choose_current_camera(frame);
  current_telecamera->build_cameramatrix(frame);
  
  // ADrender !!!
  Set_ZRanges(current_telecamera->znear, current_telecamera->zfar);

  for (j=0; j<num_omni; j++) omni[j].update(frame, current_telecamera);
  counter_for_resetting_vertex+=4;   // NB: cosi' non sovrascriviamo
                                     // i bit di zclip dei vertici

  for (j=0; j<num_objects; j++)
  {
    if (objects[j]->skinned_object==(AD_Object3D *)NULL)
	  objects[j]->build_objectmatrix(frame);
  }
  for (j=0; j<num_objects; j++)
  {
    if (objects[j]->skinned_object!=(AD_Object3D *)NULL)
	  objects[j]->build_objectmatrix(frame);
  }


  for (j=0; j<num_objects; j++)
  {
    objects[j]->paint(frame, current_telecamera, omni);
  }


  // ADrender !!!
  Begin_Scene();

  // FASE DI RASTERING
  for (j=0; j<num_materials+2; j++)
  {
	num_tria=(materials[j].my_tria_list - materials[j].my_tria_list_copy);
	if ((num_tria>0) && (materials[j].trasparencytype==OFF) && (!(materials[j].flags & IS_TRASPARENT)))
	{
	   if (materials[j].zsorting==ON)
	   {
	     list_to_paint=(AD_Tria3D **)materials[j].my_tria_list_copy;
         qsort_nontrasparent_tria (0, num_tria-1);
	   }

	   currentmaterial_flags=materials[j].flags;
	   // ADrender !!!
       currentmaterial_drawproc=Set_Material(&materials[j]);
	   for (m=0; m<num_tria; m++)
	   {
		  t=(AD_Tria3D *)materials[j].my_tria_list_copy[m];
	      if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)
		  {
            currentmaterial_drawproc(t->sp1, t->sp2, t->sp3);
		  }
		  else
		  {
			tria_paint_general(t);
		  }
	   }
	}
  }


  // Ora disegno i triangoli trasparenti
  num_tria=(list_to_paint_trasparent-list_to_paint_trasparent_copy);
  if (num_tria>0)
  {
    Begin_ExclusiveTransparency();
    qsort_trasparent_tria(0, num_tria-1);
    for (j=0; j<num_tria; j++)
	{
      t=list_to_paint_trasparent_copy[j];
	  currentmaterial_flags=t->materiale->flags;
	  // ADrender !!!
      currentmaterial_drawproc=Set_Material(t->materiale);
      if (((t->v1->flags + t->v2->flags + t->v3->flags) & 3)==0)
	  {
	    currentmaterial_drawproc(t->sp1, t->sp2, t->sp3);
	  }
      else tria_paint_general(list_to_paint_trasparent_copy[j]);
	}
  }

  End_Scene();
  End_ExclusiveTransparency();
}