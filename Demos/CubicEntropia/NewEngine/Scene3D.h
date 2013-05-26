#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "efx2D.h"
#include "camera.h"
#include "lights.h"
#include "RenderLib.h"

#define MAXMESHES 100
#define MAXOBJECT 500
#define MAXTELECAMERE 20
#define MAXPARTICLESYSTEM 20
#define MAXOMNI 30
#define MAXSPOT 30

class CScene3D : public CEfx2D
{
private:
  CCamera *m_ChooseCurrentCamera (float4 pos);  // sceglie la telecamera per quella posizione
  void m_InitMeshes(void);  
  void m_InitObjects(void);
  void m_InitCamere(void);
  void m_AddAuxiliaryMaterials(void);
  void m_InitMaterials(void);
  void m_InitLights(void);

public:
  CMesh *p_Meshes[MAXMESHES];
  int32 p_NumMeshes;

  CGeometricObject *p_Objects[MAXOBJECT];      // array di oggetti
  int32 p_NumObjects;

  CCamera p_Telecamere[MAXTELECAMERE];  // array delle telecamere
  CCamera p_NavigationCamera;
  int32 p_NumCamere;

  CLight p_OmniLights[MAXOMNI];           // array delle luci omni
  int32 p_NumOmniLights;

  CSpotLight p_SpotLights[MAXSPOT];           // array delle luci spot
  int32 p_NumSpotLights;

  CGraphicSurface *p_Textures; //array di bitmap
  int32 p_NumTextures;

  CMaterial *p_Materials;  // array di materiali
  int32 p_NumMaterials;

  int32 zclear;
  int32 p_FramesElapsed, p_NumFrames;

  CCamera *p_CurrentCamera;  // telecamera della scena corrente

  // costruttore e distruttore
  CScene3D();

  // --------------- metodi -----------------------------------------
  int32 m_Load(char8 *nfile);
  void m_Init();
  void m_DoEfx2D(double pos); // richiama le varie paint(pos) degli oggetti

  int32 index_of_mesh(char8 *stringa0);
  int32 index_of_object(char8 *stringa0);
  int32 index_of_camera(char8 *stringa0);
  int32 index_of_omni(char8 *stringa0);
  int32 index_of_spot(char8 *stringa0);
  int32 index_of_material(char8 *stringa0);
  int32 index_of_texture(char8 *stringa0);
  int32 index_of_particlesystem(char8 *stringa0);

  CMesh             *get_mesh_pointer (char8 *stringa0);
  CGeometricObject  *get_object_pointer (char8 *stringa0);
  CCamera           *get_camera_pointer (char8 *stringa0);
  CLight            *get_omni_pointer (char8 *stringa0);
  CSpotLight        *get_spot_pointer (char8 *stringa0);
  CGraphicSurface   *get_texture_pointer (char8 *stringa0);
  CMaterial         *get_material_pointer (char8 *stringa0);

  void m_TranslateCamera(float4 dx, float4 dy, float4 dz);
  void m_RotateCamera(float4 ax, float4 ay, float4 az);
};

#endif