#ifndef _RAYTSCENE_H_
#define _RAYTSCENE_H_

#include "..\..\efx2D.h"
#include "..\camera.h"
#include "..\lights.h"
#include "rayt_object.h"
#include "ADrender.h"

#define MAXRAYOBJECT 100
#define MAXRAYTELECAMERE 20
#define MAXRAYOMNI 30
#define MAXRAYSPOT 30

class AD_RaytScene : public efx2D
{
public:

 AD_RaytObject *objects[MAXRAYOBJECT];      // array di oggetti
 int num_objects;


 AD_Camera telecamere[MAXRAYTELECAMERE];  // array delle telecamere
 int num_camere;


 AD_Omnilight omni[MAXRAYOMNI];           // array delle luci omni
 int num_omni;


 AD_Spotlight spot[MAXRAYSPOT];           // array delle luci spot
 int num_spot;


 texture *textures; //array di textures/envmaps
 int num_textures;

 rayt_material *materials;  // array di materiali
 int num_materials;

 int num_frame;
 AD_Camera *current_telecamera;  // telecamera della scena corrente


 // Dati per la suddivisione spaziale (SEAD)
 AD_Vect3D minb, maxb;
 
 // costruttore e distruttore
 AD_RaytScene()
 {
    num_objects=num_camere=num_omni=num_spot=0;
	num_textures=num_materials=num_frame=0;
	materials=(rayt_material *)NULL;
    current_telecamera=(AD_Camera *)NULL;
 }

 // --------------- metodi
 int load_scene(char *nfile);
 void choose_current_camera (float4 pos);  // sceglie la telecamera per quella posizione
 void init_objects(void);
 void init_camere(void);
 void init_materials(void);
 void init_lights(void);
 void init();
 void build_world_volume(void);
 int trace (int level, AD_Ray *r, AD_Vect3D *col);
 int shade (AD_Vect3D *hitpoint, AD_Vect3D *normal, AD_Vect3D *view_dir,
	         AD_RaytObject *obj, AD_Vect3D *color);
 void do_efx2D(double pos);

 int index_of_object(char *stringa0);
 int index_of_camera(char *stringa0);
 int index_of_omni(char *stringa0);
 int index_of_spot(char *stringa0);
 int index_of_material(char *stringa0);
 int index_of_texture(char *stringa0);
 int index_of_particlesystem(char *stringa0);

 AD_RaytObject  *get_object_pointer (char *stringa0); 
 AD_Camera      *get_camera_pointer (char *stringa0);
 AD_Omnilight   *get_omni_pointer (char *stringa0);
 AD_Spotlight   *get_spot_pointer (char *stringa0);
 texture        *get_texture_pointer (char *stringa0);
 rayt_material  *get_material_pointer (char *stringa0);
};

#endif