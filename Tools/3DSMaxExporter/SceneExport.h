/*===========================================================================*\
 | 
 |  FILE:	SceneExport.h
 |			A3D Scene Exporter 
 |			for 3D Studio MAX R3.0
 | 
 |  AUTH:   Turbo / Apocalypse Design
 |
 |  HIST:	Started 10-8-99
 | 
\*===========================================================================*/

#ifndef __SCEXPSKEL__H
#define __SCEXPSKEL__H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "imtl.h"
#include "stdmat.h"
#include "decomp.h"
#include "shape.h"
#include "interpik.h"
#include "iparamm2.h"
#include "modstack.h"
#include "simpmod.h"
#include "IIKSys.h"
#include "iSkin.h"
#include "Phyexp.h"
#include "bipexp.h"
#include "gizmoimp.h"
#include "Utility.h"

// IMPORTANT:
// The ClassID must be changed whenever a new project
// is created using this skeleton
#define	SCEXP_CLASSID		Class_ID(0x67cc15e2, 0x29fc6b0c)


TCHAR *GetString(int id);
extern ClassDesc* GetSkeletonSceneExportDesc();

/*===========================================================================*\
 |	SkeletonExporter class defn
\*===========================================================================*/

typedef struct
{
  Mtl *m;
  char name[50];
} Mat;


typedef struct
{
  char name[100];
  int ID;
  BitmapTex *bmptex;
} Tex;


class SkeletonExporter : public SceneExport {
public:

// --------  Codice suxante per ogni plugin di esportazione  ---------
	SkeletonExporter();
	~SkeletonExporter();
	int ExtCount();
	const TCHAR * Ext(int n);
	BOOL SupportsOptions(int ext, DWORD options);
	const TCHAR * LongDesc();
	const TCHAR * ShortDesc();
	const TCHAR * AuthorName();
	const TCHAR * CopyrightMessage();
	const TCHAR * OtherMessage1();
	const TCHAR * OtherMessage2();
	unsigned int Version();
	void ShowAbout(HWND hWnd);
	// Configuration file management
	BOOL LoadExporterConfig();
	void SaveExporterConfig();
	TSTR GetConfigFilename();

//--------------------------------------------------------------------

	// Preferences values
	BOOL makeADM;   // crea anche il .ADM
	BOOL makeADL;   // crea anche il .ADL
	BOOL makeADO;   // crea anche il .ADO
	//BOOL makeADD;   // crea anche il .ADD
	BOOL makeADP;   // crea anche il .ADP
	BOOL makeADS;   // crea anche il .ADP
	BOOL makeRAY;   // crea anche il file .RAY ed esporta le
	                // informazioni estese nello script dei materiali
	int cs_rate;

	// Used in DoExport
	BOOL exportSelected;
	FILE *fA3D, *fRAY, *fADM, *fADL, *fADO,
		 *fTXT, *fADP, *fADS, *fANI;
	//FILE *fADD;
	Interface* ip;
	TCHAR nomefileTXT[50];  // nome del file .TXT
	TCHAR nomefileRAY[50];  // nome del file .RAY
	TCHAR nomefileADM[50];  // nome del file .ADM
	TCHAR nomefileADL[50];  // nome del file .ADL
	TCHAR nomefileADO[50];  // nome del file .ADO
	TCHAR nomefileADS[50];  // nome del file .ADS
	//TCHAR nomefileADD[50];  // nome del file .ADD
	TCHAR nomefileADP[50];  // nome del file .ADP
	TCHAR nomefileANI[50];  // nome del file .ANI

//--------------------------------------------------------------------

	Mat materiali[100];
	int nmat;
	Tex textures[100];
	int ntex;

	int	GetMtlID(Mtl *mtl);
	int	GetTexID(char *name);
	int	GetTexID(Texmap *texmap);
	int	GetMtl_GLOBAL_ID(Mtl* mtl);
	short int global_id;  // usato per calcolare l'id globale dei materiali

//--------------------------------------------------------------------

	int DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);
	BOOL nodeEnum(INode *node,Interface *ip);
	void export_animatable (Animatable *anim, int blanks);
	Control *get_control_from_animatable_name(Animatable *anim, char *name);

	// materiali
	void collect_materials(INode *node);
	void dump_material(Mtl *mtl);
	void dump_material_tracks(Mtl *mtl);
//--------------------------------------------------------------------
	// texture/bitmap e mappe
	void add_bitmap(BitmapTex *bmp);
	void collect_bitmaps(Mtl *m);
    void dump_bitmap(Tex *tex);
	void dump_map(Texmap *texmap, int MAPx, int ID);
	void dump_map_tracks(Texmap *map, char indexMap, char *material_name);
//--------------------------------------------------------------------	
	void dump_joint_parameters(JointParams* joint);
	void GetSceneBoundingBox(INode *node, TimeValue t, Box3 *bb);
//--------------------------------------------------------------------
    int  GetSize_of_material_list_chunk (void);
	void export_materials(void);
	void export_material_chunk(Mtl *mtl);
	void export_general_info(void);
//--------------------------------------------------------------------
	void export_geometric_object(INode *node, int type, char *meshname);
//--------------------------------------------------------------------
	void export_faces_indexes (int i1, int i2, int i3, int sInd, FILE *f);
	void export_solid_object(INode *node);
//--------------------------------------------------------------------
	// Tool di ottimizzazione per le mesh
	void build_valid_vertex (Mesh *mesh, Point3 *vv, int *hash, 
		                     int *num_vv);
	void build_valid_face (Mesh *mesh, int *vf, int *num_vf);
	void build_valid_texture_vertex (Mesh *mesh, int mapping_channel,
		                             UVVert *uv, int *hash, int *num_uv);
//--------------------------------------------------------------------
	void export_patch(INode *node);
//--------------------------------------------------------------------
	void export_text(INode *node);
//--------------------------------------------------------------------
	void export_camera(INode *node);
//--------------------------------------------------------------------
	void export_light(INode *node);
//--------------------------------------------------------------------
    void export_helper_object(INode *node);
//--------------------------------------------------------------------
	void export_particle_spray(INode *node);
//--------------------------------------------------------------------
	void export_WSM_wind(INode *node);
	void export_WSM_bomb(INode *node);
	void export_WSM_gravity(INode *node);
//--------------------------------------------------------------------
	void export_bipedbone(INode *node);
//--------------------------------------------------------------------
	void export_MAXbone(INode *node);
//--------------------------------------------------------------------
	void export_OSM_gizmo_controllers(SimpleMod *sm, INode *node, char ordinale, FILE *f);
	void export_OSM_twist(Modifier *twm, char ordinale, INode *node);
	void export_OSM_ripple(Modifier *twm, char ordinale, INode *node);
	void export_OSM_bend(Modifier *twm, char ordinale, INode *node);
	void export_OSM_taper(Modifier *twm, char ordinale, INode *node);
	void export_OSM_melt(Modifier *twm, char ordinale, INode *node);
	void export_OSM_noise(Modifier *twm, char ordinale, INode *node);
	void export_OSM_stretch(Modifier *twm, char ordinale, INode *node);
	void export_OSM_ffd(Modifier *twm, char ordinale, INode *node);
//--------------------------------------------------------------------

    // Entita' per l'engine raytracing
	void export_ray_sphere(INode *node);
    void export_ray_box(INode *node);
    void export_ray_plane(INode *node);
    void export_ray_cylinder(INode *node);

	int KnownModifiers(INode *node);
	BOOL IsKnownController(Control* cont);
	int MaxORT2MyORT (int maxORT);
	BOOL material_has_transparency_track(Mtl* mtl);

	void export_hide_track(Control *c, float scale, FILE *f);
	void export_point3_track(Control *c, float scale, FILE *f);
    void export_float_track(Control *c, float scale, FILE *f);
    void export_rot_track(Control *c, FILE *f);
	void export_scale_track(Control *c, FILE *f);
	void export_bipedtracks(INode *node);

	void export_1key_point3_track(Control *c, float scale, FILE *f);
    void export_1key_float_track(Control *c, float scale, FILE *f);
    void export_1key_rot_track(Control *c, FILE *f);
	void export_1key_scale_track(Control *c, FILE *f);

	void export_IIKControl(IIKControl *c, FILE *f);
	int  get_IIKControl_size(IIKControl *c);
	void export_IKChain(INode *node);

	// supporto per la tesi
	void export_ANI (INode *root);
    void export_ANI_rottrack(INode *node, Control *c, FILE *f);
    void export_ANI_rottrack2(INode *node, Control *c, FILE *f);
    void export_ANI_rottrack3(INode *node, Control *c, FILE *f);
    void export_ANI_postrack(INode *node, Control *c, FILE *f);
	void export_ANI_count (INode *node, int *cJ, int *cIKC);
	void export_ANI_allJoints (INode *node);
	void export_ANI_Joint (INode *node);
	void export_ANI_allIKChains (INode *node);
	void export_ANI_IKChain (INode *node);
};

#endif