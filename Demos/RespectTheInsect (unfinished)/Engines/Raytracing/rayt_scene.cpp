#include <windows.h>
#include <conio.h>
#include <math.h>
#include "rayt_scene.h"
#include "rayt_object.h"
#include "..\lights.h"
#include "..\..\Math\fastmath.h"
#include "utilsRAY.h"
#include "rayt_utilsscript.h"
#include "..\..\utility.h"
#include "rayt_kernel.h"

extern FILE *fdeb;
extern HWND miawin;         // finestra principale; usata qui per le debug_error


// #########################################################
// procedure per l'estrazione di singole entita' della scena
// #########################################################

int AD_RaytScene::index_of_object(char *stringa0)
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


int AD_RaytScene::index_of_camera(char *stringa0)
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


int AD_RaytScene::index_of_omni(char *stringa0)
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


int AD_RaytScene::index_of_spot(char *stringa0)
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


int AD_RaytScene::index_of_material(char *stringa0)
{
  int k;

  for (k=0; k<num_materials; k++)
  {
    if (strcmp(materials[k].name, stringa0)==0) return(k);
  }
  return(-1);
}


int AD_RaytScene::index_of_texture(char *stringa0)
{
  int k;

  for (k=0; k<num_textures; k++)
  {
    if (strcmp(textures[k].name, stringa0)==0) return(k);
  }
  return(-1);
}


AD_RaytObject* AD_RaytScene::get_object_pointer (char *stringa0)
{
  int j;

  j=0; 
  while (j<num_objects)
  {
    if (strcmp(stringa0, objects[j]->name)==0)
        return(objects[j]);
    j++;
  }
  return((AD_RaytObject *)NULL);
}


AD_Camera* AD_RaytScene::get_camera_pointer (char *stringa0)
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


AD_Omnilight* AD_RaytScene::get_omni_pointer (char *stringa0)
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


AD_Spotlight* AD_RaytScene::get_spot_pointer (char *stringa0)
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


texture* AD_RaytScene::get_texture_pointer (char *stringa0)
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


rayt_material* AD_RaytScene::get_material_pointer (char *stringa0)
{
  int j;

  j=0; 
  while (j<num_materials)
  {
    if (strcmp(stringa0, materials[j].name)==0)
        return(&materials[j]);
    j++;
  }
  return((rayt_material *)NULL);
}


int AD_RaytScene::load_scene(char *nfile)
{
  char fileRAY[50]="";
  char fileADM[50]="";
  char fileADL[50]="";
  char fileADD[50]="";
  char fileADO[50]="";
  char fileADP[50]="";
  FileRAY fRAY;

//  fdeb=fopen("debug.txt", "wt");
/*
  fprintf(fdeb,"################################\n");
  fprintf(fdeb,"Printing debug information\n");
  fprintf(fdeb,"################################\n\n");
  fflush(fdeb); */

  strcpy(fileRAY, nfile); strcat(fileRAY, ".RAY");
  strcpy(fileADM, nfile); strcat(fileADM, ".ADM");
  strcpy(fileADL, nfile); strcat(fileADL, ".ADL");
  strcpy(fileADD, nfile); strcat(fileADD, ".ADD");
  strcpy(fileADO, nfile); strcat(fileADO, ".ADO");
  strcpy(fileADP, nfile); strcat(fileADP, ".ADP");

  if (esiste_file(fileADM))
	  rayt_load_ADM(fileADM, this);
  if (esiste_file(fileRAY))
  {
	fRAY.LoadFromFile(this, fileRAY);
  }
  else debug_error(miawin, "File .RAY not present/found!");
  if (esiste_file(fileADL)) rayt_load_ADL(fileADL, this);
/*
  if (esiste_file(fileADD)) load_ADD(fileADD, this);
  if (esiste_file(fileADO)) load_ADO(fileADO, this);
  if (esiste_file(fileADP)) load_ADP(fileADP, this);
*/
  return(1);
}


void AD_RaytScene::init_objects(void)
{
  int i;
  AD_RaytObject *fptr;

  for (i=0; i<num_objects; i++)
  {
    if (objects[i]->init()==-1) return;
    // il linking padre-figlio e' na cazzata col .RAY
    // NB : nel .RAY gli oggetti sono presenti e caricati gia'
    // nell'ordine gerarchico giusto, quindi non devo sortarli
    if (objects[i]->father_name != (char *)NULL)
	{
	  fptr=get_object_pointer(objects[i]->father_name);
      objects[i]->father=fptr;
	  if (fptr!=(AD_RaytObject *)NULL) fptr->have_childrens++;
	}
  }
}


void AD_RaytScene::init_camere(void)
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


void AD_RaytScene::init_materials(void)
{
  int i;

  // settiamo i materiali per essere disegnati secondo
  // il metodo previsto/scriptato
  for (i=0; i<num_materials; i++)
  {
  }
}


void AD_RaytScene::init_lights(void)
{
  int j, i;

  for (j=0; j<num_omni; j++)
  {
	 for (i=0; i<omni[j].colortrack.numkey; i++)
	 {
		// mi serve per avere i colori delle luci nel range [0;255]
		vect_scale(&omni[j].colortrack.posizioni[i].p,
			       (float)(1.0/RGB_PRECISION),
                   &omni[j].colortrack.posizioni[i].p);
	 }
     omni[j].init();
  }

  for (j=0; j<num_spot; j++)
  {
	 for (i=0; i<spot[j].colortrack.numkey; i++)
	 {
		// mi serve per avere i colori delle luci nel range [0;255]
		vect_scale(&spot[j].colortrack.posizioni[i].p,
			       (float)(1.0/RGB_PRECISION),
                   &spot[j].colortrack.posizioni[i].p);
	 }
     spot[j].init();
  }
}


void AD_RaytScene::init(void)
{
  init_materials();
  init_lights();
  init_camere();
  init_objects();

  init_fast_math_library();
  return;
}


void AD_RaytScene::choose_current_camera (float4 pos)
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
}


int AD_RaytScene::shade (AD_Vect3D *hitpoint, AD_Vect3D *normal,
                         AD_Vect3D *view_dir,
						 AD_RaytObject *obj, AD_Vect3D *color)
{
   int i, j, shadow;
   AD_Vect3D light_hitpoint;
   AD_Vect3D diffuse, specular;
   AD_Vect3D H;
   AD_Ray r;
   rayt_material *m;
   float cosalpha, q, len;

   m=obj->mymat;
   diffuse.x=diffuse.y=diffuse.z=0;
   specular.x=specular.y=specular.z=0;
   vect_copy(hitpoint, &r.base);

   shadow=num_omni;
   for (i=0; i<num_omni; i++)
   {
      vect_sub(&omni[i].currentpos, hitpoint, &light_hitpoint);
	  len=vect_lenght(&light_hitpoint);
	  q=1.0f/len;
	  light_hitpoint.x*=q;
	  light_hitpoint.y*=q;
	  light_hitpoint.z*=q;
      vect_copy(&light_hitpoint, &r.direction);

      for (j=0; j<num_objects; j++)
	  {
		    if ((objects[j]!=obj) && (objects[j]->intersect(&r)))
			{
		       if (r.distance<len)
			   {
				   shadow--;
				   goto dopo;
			   }
			}
	  }

      cosalpha=vect_dot(&light_hitpoint, normal);
	  if (is_float_positive(cosalpha))
	  {
         // componente diffusa
		 diffuse.x+=cosalpha*m->omni_prec_diffcol[i].x;
		 diffuse.y+=cosalpha*m->omni_prec_diffcol[i].y;
		 diffuse.z+=cosalpha*m->omni_prec_diffcol[i].z;

		 // componente speculare
/*
		 vect_scale(normal, cosalpha*2.0f, &reflected_light);
		 vect_sub(&reflected_light, &light_hitpoint, &reflected_light);
		 vect_normalize(&reflected_light);

         specular_angle=vect_dot(&reflected_light, view_dir);
		 if (specular_angle<0)
		 {
			 vect_scale(&omni[i].currentcolor,
				        powf(specular_angle, obj->mymat->glossiness),
						&aux);
			 specular.x=specular.x + aux.x*m->specular_color.x;
			 specular.y=specular.y + aux.y*m->specular_color.y;
			 specular.z=specular.z + aux.z*m->specular_color.z;
		 }
*/
		 vect_sub(&light_hitpoint, view_dir, &H);
		 vect_normalize(&H);
	     q=powf(vect_dot(normal, &H), obj->mymat->glossiness);
		 specular.x=specular.x + q*m->omni_prec_speccol[i].x;
		 specular.y=specular.y + q*m->omni_prec_speccol[i].y;
		 specular.z=specular.z + q*m->omni_prec_speccol[i].z;
	  }
dopo:
;
   }
   vect_scale(&diffuse, m->kdr, &diffuse);
   vect_scale(&specular, m->ksr, &specular);
   vect_add(&diffuse, &specular, color);
   return(shadow);
}


int AD_RaytScene::trace (int level, AD_Ray *r, AD_Vect3D *col)
{
  int i, hit, hitobject;
  float toofar;
  AD_Vect3D hitpoint, normal, objectcol;
  AD_Vect3D shade_color, reflected_color;
  AD_Ray reflected_ray;


  if (level>1)
  {
     col->x=col->y=col->z=0;
	 return(0);
  }

  objectcol.x=objectcol.y=objectcol.z=0;
  toofar=1E10;
  hitobject=-1;

  if (level>0)
  for(i=0; i<num_objects; i++)
  {
     hit = objects[i]->intersect(r);
	 if ((hit) && (r->distance < toofar))
	 {
       toofar=r->distance;
       hitobject=i;
	 }
  }
  else
  for(i=0; i<num_objects; i++)
  {
     hit = objects[i]->intersect_first(r);
	 if ((hit) && (r->distance < toofar))
	 {
       toofar=r->distance;
       hitobject=i;
	 }
  }


  if (hitobject>=0)
  {
     vect_scale(&r->direction, toofar, &hitpoint);
	 vect_add(&hitpoint, &r->base, &reflected_ray.base);

	 // calcolo della normale al punto di intersezione
	 objects[hitobject]->get_normal(&reflected_ray.base, &normal);

	 // in base al punto di intersezione, alla normale ad esso,
	 // all'oggetto colpito (al suo materiale) calcolo il colore
	 // del punto
	 if (shade(&reflected_ray.base, &normal, &r->direction, objects[hitobject], &shade_color))
	 {
        vect_add(&objectcol, &shade_color, &objectcol);

        // calcolo il raggio riflesso e il suo colore solo se il
		// punto colpito non e' in ombra
	    ReflectedRay(&r->direction, &normal, &reflected_ray.direction);

	   // traccio il raggio riflesso ottenendo il contributo di colore
	   // che esso apporta; lo ficco dentro a reflected_color
	   trace(level+1, &reflected_ray, &reflected_color);
       vect_add(&objectcol, &reflected_color, &objectcol);
	 }
  }

  vect_copy(&objectcol, col);
  if (toofar==1E10) return(0);
  else return(1);
}


void AD_RaytScene::build_world_volume(void)
{
   int i;

   vect_set(&minb, 1E10, 1E10, 1E10);
   vect_set(&maxb, -1E10, -1E10, -1E10);
   for (i=0; i<num_objects; i++)
   {
      if (objects[i]->currentpos.x < minb.x) minb.x=objects[i]->currentpos.x;
      if (objects[i]->currentpos.y < minb.y) minb.y=objects[i]->currentpos.y;
      if (objects[i]->currentpos.z < minb.z) minb.z=objects[i]->currentpos.z;

      if (objects[i]->currentpos.x > maxb.x) maxb.x=objects[i]->currentpos.x;
      if (objects[i]->currentpos.y > maxb.y) maxb.y=objects[i]->currentpos.y;
      if (objects[i]->currentpos.z > maxb.z) maxb.z=objects[i]->currentpos.z;
   }
}


void AD_RaytScene::do_efx2D(double pos)
// si suppone pos compreso tra [0..1]
{
  float frame, fx, fy, l;
  AD_Vect3D color, cam_dx, cam_dy, cam_z, inc_y, aux;
  AD_Ray ray;
  int i, j, x, y, R, G, B, width, height;
  unsigned int *image32;

  frame=(float)( ((pos-fstartpos)/(fendpos-fstartpos))
	              *(num_frame-1));

  // consideriamo la pos nel range [0;1]
  pos=(((float)(pos)-fstartpos)/(fendpos-fstartpos));


  choose_current_camera(frame);
  current_telecamera->build_cameramatrix(frame);

  for (j=0; j<num_omni; j++)
  {
     omni[j].update(frame, current_telecamera);
	 for (i=0; i<num_materials; i++)
	 {
	 	materials[i].omni_prec_diffcol[j].x=omni[j].currentcolor.x*materials[i].diffuse_color.x;
	 	materials[i].omni_prec_diffcol[j].y=omni[j].currentcolor.y*materials[i].diffuse_color.y;
	 	materials[i].omni_prec_diffcol[j].z=omni[j].currentcolor.z*materials[i].diffuse_color.z;
	 	materials[i].omni_prec_speccol[j].x=omni[j].currentcolor.x*materials[i].specular_color.x;
	 	materials[i].omni_prec_speccol[j].y=omni[j].currentcolor.y*materials[i].specular_color.y;
	 	materials[i].omni_prec_speccol[j].z=omni[j].currentcolor.z*materials[i].specular_color.z;
	 }
  }


  for (j=0; j<num_objects; j++)
  {
    objects[j]->update(frame, &current_telecamera->currentpos);
  }
  build_world_volume();

  mat_get_row(&current_telecamera->currentmatrix_rot, 0, &cam_dx);
  mat_get_row(&current_telecamera->currentmatrix_rot, 1, &cam_dy);
  mat_get_row(&current_telecamera->currentmatrix_rot, 2, &cam_z);

  fx=(float)((2*tan(current_telecamera->currentfov))/(float)image->width);
  fy=(float)((2*tan(current_telecamera->currentfov))/(float)image->width);
  vect_scale(&cam_dx, fx, &cam_dx);
  vect_scale(&cam_dy, -fy, &cam_dy);
  vect_copy(&current_telecamera->currentpos, &ray.base);

  width=image->width;
  height=image->height;
  image32=image->uint32ptr;

  
  fy=(float)(0.5*(1-image->height));
  vect_scale(&cam_dy, fy, &inc_y);
  vect_add(&inc_y, &cam_z, &inc_y);
  fx=(float)(0.5*(1-image->width));

  for (y=0; y<height; y++)
  {
	vect_set(&aux, cam_dx.x*fx, cam_dx.y*fx, cam_dx.z*fx);
	vect_add(&aux, &inc_y, &aux);
    for (x=0; x<width; x++)
	{
      l=1.0f/mysqrt(aux.x*aux.x + aux.y*aux.y + aux.z*aux.z);
      ray.direction.x=aux.x*l;
      ray.direction.y=aux.y*l;
      ray.direction.z=aux.z*l;

	  trace (0, &ray, &color);
      if (color.x>255) R=255;
	  else R=myfist(color.x);
	  if (color.y>255) G=255;
	  else G=myfist(color.y);
	  if (color.z>255) B=255;
	  else B=myfist(color.z);
      *image32++=(B | (G<<8) | (R<<16));
      vect_add(&aux, &cam_dx, &aux);
	}
	vect_add(&inc_y, &cam_dy, &inc_y);
  }
}