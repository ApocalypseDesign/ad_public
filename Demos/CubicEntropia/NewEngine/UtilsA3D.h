// A3Dfile.h: interface for the A3Dfile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _UTILSA3D_H_
#define _UTILSA3D_H_

#include "Scene3d.h"
#include "Helpers.h"
#include "ParticleSystem.h"
#include "A3Dfile.h"   // per la definizone dei chunk
#include "FastMath.h"

typedef struct
{
  int32 ID;
  char8 *name_owner;
  int32 size;
  char8 *data;
} A3Dchunk;


typedef struct
{
  int32 global_id;
  char8 name[80];
} A3D_Material_Link;


class CFileA3D
{
public:
	CFileA3D();

    // ********************************************************
    // *************  PROCEDURE DI BASSO LIVELLO  *************
    // ********************************************************
    char8 *m_AllocA3D(char8 *filename);
    char8 *m_GetNextChunk(char8 *current_chunk);


    // ********************************************************
    // *************  PROCEDURE DI MEDIO LIVELLO  *************
    // ********************************************************
	void m_Load_Camera_Settings (CCamera *cam, char8 *chunk);
    void m_Load_Omnilight_Settings(CLight *ol, char8 *chunk);
    void m_Load_Spotlight_Settings(CSpotLight *sl, char8 *chunk);

    // metodi per mesh triangolari e dummy
    void m_Load_Mesh_VertexList(CMesh *mesh, char8 *chunk);
    void m_Load_Mesh_FaceList(CMesh *mesh, char8 *chunk);
    void m_Load_Mesh_TextureVertexList(CMesh *mesh, char8 *chunk);
    void m_Load_Mesh_TextureFaceList(CMesh *mesh, char8 *chunk);

    void m_Load_GeometricObject(CGeometricObject *obj, char8 *chunk);
    //void m_Load_Helper(CHelper *helper, char8 *chunk);
    void m_Load_ParticleSystem_Settings(CParticleSystem *ps, char8 *chunk);
    void m_Load_WindModifier_Settings(CWindModifier *wind, char8 *chunk);
    void m_Load_GravityModifier_Settings(CGravityModifier *gravity, char8 *chunk);

	// keyframer generale
	void m_Load_HideTrack(CHideTrack *ht, char8 *chunk);
	void m_Load_PosTrack(CPosTrack *pt, char8 *chunk);
	void m_Load_RotTrack(CRotTrack *rt, char8 *chunk);
	void m_Load_RollTrack(CRollTrack *lt, char8 *chunk);

    // Object space modifiers (OSM)
	void m_Load_TwistModifier_Settings(CTwistModifier *tm, char8 *chunk);
    void m_Load_RippleModifier_Settings(CRippleModifier *rm, char8 *chunk);
    void m_Load_BendModifier_Settings(CBendModifier *bm, char8 *chunk);
    void m_Load_MeltModifier_Settings(CMeltModifier *mm, char8 *chunk);
    void m_Load_TaperModifier_Settings(CTaperModifier *tm, char8 *chunk);
    void m_Load_NoiseModifier_Settings(CNoiseModifier *nm, char8 *chunk);
    void m_Load_StretchModifier_Settings(CStretchModifier *sm, char8 *chunk);
    void m_Load_FFDModifier_Settings(CFFDModifier *fm, char8 *chunk);

    void m_Load_Materials_List(char8 *chunk);
    void m_Load_GeneralInfo(char8 *chunk);
	char8 *m_GetMaterialName(int32 g_id_mat);

    // ********************************************************
    // **************  PROCEDURE DI ALTO LIVELLO  *************
    // ********************************************************

	int32 m_Load_HideTrack(char8 *objectname, char8 *A3Dfilename, CHideTrack *ht);
	int32 m_Load_PosTrack(int32 IDTrack, char8 *objectname, char8 *A3Dfilename, CPosTrack *pt);
	int32 m_Load_RotTrack(int32 IDTrack, char8 *objectname, char8 *A3Dfilename, CRotTrack *rt);
	int32 m_Load_RollTrack(int32 IDTrack, char8 *objectname, char8 *A3Dfilename, CRollTrack *lt);
    int32 m_Load_GeometricObject(char8 *objectname, char8 *A3Dfilename, CGeometricObject *obj);
    int32 m_Load_Mesh(char8 *meshname, char8 *A3Dfilename, CMesh *mesh);

	void m_LoadFromMemory(CScene3D *s, char8 *ptr);
	void m_LoadFromFile(CScene3D *s, char8 *filename);
	void m_SaveToMemory(char8 *ptr, uint32 *size);
	void m_SaveToFile(char8 *filename);


private:
	  A3D_Material_Link p_Materials[600];  // elenco materiali
	  int32 p_NumMaterials;
	  char8 p_CurrentA3DFileName[150];
	  CScene3D *p_MyScene;
};


// utility generiche per i file .A3D
void write_chunk_header(FILE *f, int32 id, char8 *no, int32 s);

#endif 