#ifndef _UTILSRAY_H_
#define _UTILSRAY_H_

#include "..\keyframe.h"
#include "..\camera.h"
#include "..\lights.h"
#include "rayt_object.h"
#include "rayt_scene.h"


// costanti definite per i tipi di chunk "statici"
#define SCENE_GENERAL_INFO_ID  0
#define SCENE_MATERIAL_LIST_ID  50

// chuck per il raytracing
#define RAY_SPHERE_ID 500
#define RAY_BOX_ID 501
#define RAY_PLANE_ID 502
#define RAY_CYLINDER_ID 503
#define RAY_CONE_ID 504
#define RAY_TORUS_ID 505


#define HELPER_OBJECT_ID  6
#define CAMERA_ID  8
#define OMNI_LIGHT_ID  9

// costanti definite per i tipi di chunk "dinamici" (keyframer)
#define POSITION_TRACK_ID  11
#define ROTATION_TRACK_ID  12
#define SCALE_TRACK_ID  20

#define CAMERA_FOV_TRACK_ID  13
#define CAMERA_ROLL_TRACK_ID  14
#define COLOR_TRACK_ID  15
#define LIGHT_POSITION_TRACK_ID  16
#define CAMERA_POSITION_TRACK_ID  17
#define CAMERA_TARGET_TRACK_ID  18


// indica il fine scena (cmq controllare il suo flag di dato)
#define END_SCENE_ID 100

// costanti definite per il tipo di controller
#define LINEAR_CONTROLLER   0
#define BEZIER_CONTROLLER   1
#define TCB_CONTROLLER      2

// per gli oggetti helper definisco i seguenti tipi
#define DUMMY_HELPER 0
#define BONE_HELPER  1


typedef struct
{
	int ID;
	char *name_owner;
	int size;
	char *data;
} RAYchunk;


typedef struct
{
  int global_id;
  char name[80];
} RAY_Material_Link;


class FileRAY
{
public:

	FileRAY()
	{}


    // ********************************************************
    // *************  PROCEDURE DI BASSO LIVELLO  *************
    // ********************************************************

    char *allocRAY(char *filename);
    char *get_next_chunk(char *current_chunk);


    // ********************************************************
    // *************  PROCEDURE DI MEDIO LIVELLO  *************
    // ********************************************************

    void scene_load_general_info(AD_RaytScene *s, char *chunk);
	void camera_load_settings (AD_Camera *cam, char *chunk);
    void omnilight_load_settings(AD_Omnilight *ol, char *chunk);
    void spotlight_load_settings(AD_Spotlight *sl, char *chunk);

	// keyframer generale
	void keyframe_load_postrack(postrack *pt, char *chunk);
	void keyframe_load_rottrack(rottrack *rt, char *chunk);
	void keyframe_load_rolltrack(rolltrack *lt, char *chunk);

	// oggetti geometrici
	void object_load_sphere(AD_RaytSphere *sphere, char *chunk);
	void object_load_box(AD_RaytBox *box, char *chunk);
	void object_load_plane(AD_RaytPlane *plane, char *chunk);
	void object_load_cone(AD_RaytCone *cone, char *chunk);


    void materials_load_list(char *chunk);
	char *get_material_name(int g_id_mat);

    // ********************************************************
    // **************  PROCEDURE DI ALTO LIVELLO  *************
    // ********************************************************

	void LoadFromMemory(AD_RaytScene *s, char *ptr);
	void LoadFromFile(AD_RaytScene *s, char *filename);
	void SaveToMemory(char *ptr, unsigned int *size);
	void SaveToFile(char *filename);


private:
	  AD_RaytScene *myscene;   // copia del parametro di LoadFromFile
	  RAY_Material_Link materials[600];  // elenco materiali
	  int num_materials;
};


#endif