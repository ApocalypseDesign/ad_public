#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "triangle.h"
#include "..\efx2D.h"
#include "camera.h"
#include "object3d.h"
#include "lights.h"
#include "ADrender.h"
#include "particle.h"

#define MAXOBJECT 5000
#define MAXTELECAMERE 20
#define MAXPARTICLESYSTEM 20
#define MAXOMNI 30
#define MAXSPOT 30

class AD_Scene3D : public efx2D
{
public:

 AD_Object3D *objects[MAXOBJECT];      // array di oggetti
 int num_objects;


 AD_Camera telecamere[MAXTELECAMERE];  // array delle telecamere
 int num_camere;


 AD_Omnilight omni[MAXOMNI];           // array delle luci omni
 int num_omni;


 AD_Spotlight spot[MAXSPOT];           // array delle luci spot
 int num_spot;


 texture *textures; //array di textures/envmaps
 int num_textures;


 material *materials;  // array di materiali
 material_parameters_tracks *material_tracks;
 int num_materials;

 AD_Tria3DPtr *list_trasparent;
 
 int zclear;
 int num_frame;

 AD_Camera *current_telecamera;  // telecamera della scena corrente
 
 // costruttore e distruttore
 AD_Scene3D()
 {
    num_objects=num_camere=num_omni=num_spot=0;
	num_textures=num_materials=num_frame=0;
	materials=(material *)NULL;
    material_tracks=(material_parameters_tracks *)NULL;
    current_telecamera=(AD_Camera *)NULL;
	zclear=0;
 }

 // --------------- metodi

 int load_scene(char *nfile);
 void choose_current_camera (float4 pos);  // sceglie la telecamera per quella posizione
 void init_objects(void);
 void init_camere(void);
 void add_auxiliary_materials(void);
 void init_materials(void);
 void init_lights(void);
 void init();
 void do_efx2D(double pos); // richiama le varie paint(pos) degli oggetti

 int index_of_object(char *stringa0);
 int index_of_camera(char *stringa0);
 int index_of_omni(char *stringa0);
 int index_of_spot(char *stringa0);
 int index_of_material(char *stringa0);
 int index_of_texture(char *stringa0);
 int index_of_particlesystem(char *stringa0);

 AD_Object3D    *get_object_pointer (char *stringa0); 
 AD_Camera      *get_camera_pointer (char *stringa0);
 AD_Omnilight   *get_omni_pointer (char *stringa0);
 AD_Spotlight   *get_spot_pointer (char *stringa0);
 texture        *get_texture_pointer (char *stringa0);
 material       *get_material_pointer (char *stringa0);
 AD_ParticleSystem  *get_particlesystem_pointer (char *stringa0);
};

#endif