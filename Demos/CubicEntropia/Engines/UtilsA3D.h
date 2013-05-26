// A3Dfile.h: interface for the A3Dfile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _UTILSA3D_H_
#define _UTILSA3D_H_

#include "keyframe.h"
#include "camera.h"
#include "object3d.h"
#include "scene3d.h"
#include "patch.h"
#include "lights.h"
#include "..\utility.h"

// costanti definite per i tipi di chunk "statici"
#define SCENE_GENERAL_INFO_ID  0
#define SCENE_MATERIAL_LIST_ID  50

#define TRI_MESH_ID  1
#define TRI_MESH_VERTEX_LIST_ID 2
#define TRI_MESH_FACES_LIST_ID  3
#define TRI_MESH_TEXTURE_VERTEX_LIST_ID 4
#define TRI_MESH_TEXTURE_FACES_LIST_ID 5
#define HELPER_OBJECT_ID  6
#define BONE_OBJECT_ID  80
#define IKCHAIN_OBJECT_ID  81
#define PATCH_MESH_ID  7
#define CAMERA_ID  8
#define OMNI_LIGHT_ID  9
#define SPRAY_PARTICLE_SYSTEM_ID 10
#define WIND_MODIFIER_ID  60
#define GRAVITY_MODIFIER_ID  61
#define PHYSIC_MODIFIER_ID   62
#define TWIST_MODIFIER_ID   63
#define RIPPLE_MODIFIER_ID   64
#define BEND_MODIFIER_ID   65
#define TAPER_MODIFIER_ID   66
#define MELT_MODIFIER_ID   67
#define NOISE_MODIFIER_ID   68
#define STRETCH_MODIFIER_ID   69
#define FFD_MODIFIER_ID       70
#define SKIN_MODIFIER_ID      71

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
#define PATCH_VERTEX_TRACK_ID  19
#define PATCH_VECTOR_TRACK_ID  21
#define MATERIAL_OPACITY_TRACK_ID  22

#define OSM_CENTER_TRACK_ID      23
#define TWIST_ANGLE_TRACK_ID     24
#define TWIST_BIAS_TRACK_ID      25
#define TWIST_UPLIMIT_TRACK_ID   26
#define TWIST_LOWLIMIT_TRACK_ID  27

#define RIPPLE_AMP1_TRACK_ID      28
#define RIPPLE_AMP2_TRACK_ID      29
#define RIPPLE_WAVELEN_TRACK_ID   30
#define RIPPLE_PHASE_TRACK_ID     31
#define RIPPLE_DECAY_TRACK_ID     32

#define BEND_ANGLE_TRACK_ID     33
#define BEND_DIR_TRACK_ID       34
#define BEND_UPLIMIT_TRACK_ID   35
#define BEND_LOWLIMIT_TRACK_ID  36

#define MELT_AMOUNT_TRACK_ID     37
#define MELT_SPREAD_TRACK_ID     38

#define STRETCH_STRETCH_TRACK_ID   39
#define STRETCH_AMPLIFY_TRACK_ID   40
#define STRETCH_UPLIMIT_TRACK_ID   41
#define STRETCH_LOWLIMIT_TRACK_ID  42

#define TAPER_AMOUNT_TRACK_ID      43
#define TAPER_CURVE_TRACK_ID       44
#define TAPER_UPLIMIT_TRACK_ID     45
#define TAPER_LOWLIMIT_TRACK_ID    46

#define FFD_CONTROLPOINT_TRACK_ID  47
#define FFD_POSITION_TRACK_ID      48
#define FFD_ROTATION_TRACK_ID      49
#define FFD_SCALE_TRACK_ID         51  // il 50 è occupato

#define NOISE_STRENGTH_TRACK_ID    52
#define NOISE_FREQ_TRACK_ID        53
#define NOISE_PHASE_TRACK_ID       54
#define NOISE_SCALE_TRACK_ID       55

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
} A3Dchunk;


typedef struct
{
  int global_id;
  char name[80];
} A3D_Material_Link;


class FileA3D
{
public:

	FileA3D()
	{}


    // ********************************************************
    // *************  PROCEDURE DI BASSO LIVELLO  *************
    // ********************************************************

    char *allocA3D(char *filename);
    char *get_next_chunk(char *current_chunk);


    // ********************************************************
    // *************  PROCEDURE DI MEDIO LIVELLO  *************
    // ********************************************************

    void scene_load_general_info(AD_Scene3D *s, char *chunk);

	void camera_load_settings (AD_Camera *cam, char *chunk);

    void omnilight_load_settings(AD_Omnilight *ol, char *chunk);

    void spotlight_load_settings(AD_Spotlight *sl, char *chunk);

    // metodi per mesh triangolari e dummy
    void object_load_vertex_list(AD_Object3D *obj, char *chunk);
    void object_load_face_list(AD_Object3D *obj, char *chunk);
    void object_load_texture_vertex_list(AD_Object3D *obj, char *chunk);
    void object_load_texture_face_list(AD_Object3D *obj, char *chunk);
    void object_load_mesh(AD_Object3D *obj, char *chunk);
    void object_load_helper(AD_Object3D *obj, char *chunk);

    // metodi per mesh fatte di pezze di Bezier
	void patchobject_load_mesh(AD_PatchObject *pobj, char *chunk);
	void keyframe_load_patchobject_vertvect_track(postrack *pt_array, char *chunk);

    // metodi per particles systems
	void particlesystem_load_settings(AD_ParticleSystem *psys, char *chunk);
	void windmodifier_load_settings(AD_WindModifier *wmod, char *chunk);
	void gravitymodifier_load_settings(AD_GravityModifier *gmod, char *chunk);

	// metodi per gli OSM modifiers
	void twistmodifier_load_settings(AD_TwistModifier *tm, char *chunk);
	void ripplemodifier_load_settings(AD_RippleModifier *rm, char *chunk);
	void bendmodifier_load_settings(AD_BendModifier *bm, char *chunk);
	void tapermodifier_load_settings(AD_TaperModifier *tm, char *chunk);
	void meltmodifier_load_settings(AD_MeltModifier *mm, char *chunk);
	void noisemodifier_load_settings(AD_NoiseModifier *nm, char *chunk);
	void stretchmodifier_load_settings(AD_StretchModifier *sm, char *chunk);
	void ffdmodifier_load_settings(AD_FFDModifier *fm, char *chunk);
	void physicmodifier_load_settings(AD_Object3D *obj, char *chunk);
	void skinmodifier_load_settings(AD_Object3D *obj, char *chunk);

	// keyframer generale
	void keyframe_load_postrack(postrack *pt, char *chunk);
	void keyframe_load_rottrack(rottrack *rt, char *chunk);
	void keyframe_load_rolltrack(rolltrack *lt, char *chunk);

    void materials_load_list(char *chunk);
	char *get_material_name(int g_id_mat);

    // ********************************************************
    // **************  PROCEDURE DI ALTO LIVELLO  *************
    // ********************************************************

	void LoadFromMemory(AD_Scene3D *s, char *ptr);
	void LoadFromFile(AD_Scene3D *s, char *filename);
	void SaveToMemory(char *ptr, unsigned int *size);
	void SaveToFile(char *filename);


private:
	  AD_Scene3D *myscene;   // copia del parametro di LoadFromFile
	  A3D_Material_Link materials[600];  // elenco materiali
	  int num_materials;
};


// utility generiche per i file .A3D
void write_chunk_header(FILE *f, int id, char *no, int s);
void write_string0 (FILE *f, char *st);


#endif 