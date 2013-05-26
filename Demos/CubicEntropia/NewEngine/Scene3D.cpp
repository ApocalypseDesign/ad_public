#include <windows.h>
#include <conio.h>
#include "Scene3D.h"
#include "utilsA3D.h"
#include "utilsscript.h"

extern FILE *fdeb;
extern HWND miawin;         // finestra principale; usata qui per le debug_error


// #########################################################
// procedure per l'estrazione di singole entita' della scena
// #########################################################

CScene3D::CScene3D()
{
  int32 i;

  for (i=0; i<MAXMESHES; i++) p_Meshes[i]=(CMesh *)NULL;
  p_NumMeshes=0;

  for (i=0; i<MAXOBJECT; i++) p_Objects[i]=(CGeometricObject *)NULL;
  p_NumObjects=0;

  p_NumCamere=0;
  p_NumOmniLights=0;
  p_NumSpotLights=0;
  p_Textures=(CGraphicSurface *)NULL;
  p_NumTextures=0;
  p_Materials=(CMaterial *)NULL;
  p_NumMaterials=0;
  zclear=0;
  p_CurrentCamera=(CCamera *)NULL;
  p_FramesElapsed=0;
  p_NumFrames=0;
}

//--------------------------------------------------------------------

int32 CScene3D::index_of_mesh(char8 *stringa0)
// ritorna l'indice(la posizione nell'array) della mesh che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int32 k;

  for (k=0; k<p_NumMeshes; k++)
    if (strcmp(p_Meshes[k]->p_Name, stringa0)==0) return(k);
  return(-1);
}

//--------------------------------------------------------------------

int32 CScene3D::index_of_object(char8 *stringa0)
// ritorna l'indice(la posizione nell'array) dell'oggetto che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int32 k;

  for (k=0; k<p_NumObjects; k++)
    if (strcmp(p_Objects[k]->p_Name, stringa0)==0) return(k);

  for (k=0; k<p_NumOmniLights; k++)
    if (strcmp(p_OmniLights[k].p_Name, stringa0)==0) return(k);

  for (k=0; k<p_NumSpotLights; k++)
    if (strcmp(p_SpotLights[k].p_Name, stringa0)==0) return(k);

  for (k=0; k<p_NumCamere; k++)
    if (strcmp(p_Telecamere[k].p_Name, stringa0)==0) return(k);

  return(-1);
}

//--------------------------------------------------------------------

int32 CScene3D::index_of_camera(char8 *stringa0)
// ritorna l'indice(la posizione nell'array) della telecamera che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int32 k;

  for (k=0; k<p_NumCamere; k++)
    if (strcmp(p_Telecamere[k].p_Name, stringa0)==0) return(k);
  return(-1);
}

//--------------------------------------------------------------------

int32 CScene3D::index_of_omni(char8 *stringa0)
// ritorna l'indice(la posizione nell'array) della luce omni che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int32 k;

  for (k=0; k<p_NumOmniLights; k++)
    if (strcmp(p_OmniLights[k].p_Name, stringa0)==0) return(k);
  return(-1);
}

//--------------------------------------------------------------------

int32 CScene3D::index_of_spot(char8 *stringa0)
// ritorna l'indice(la posizione nell'array) della luce spot che ha nome pari
// a quello puntato da stringa0 (che e' una stringa "zero termined")
{
  int32 k;

  for (k=0; k<p_NumSpotLights; k++)
  {
    if (strcmp(p_SpotLights[k].p_Name, stringa0)==0) return(k);
  }
  return(-1);
}

//--------------------------------------------------------------------

int32 CScene3D::index_of_material(char8 *stringa0)
{
  int32 k;

  for (k=0; k<p_NumMaterials; k++)
  {
    if (strcmp(p_Materials[k].p_Name, stringa0)==0) return(k);
  }
  return(-1);
}

//--------------------------------------------------------------------

int32 CScene3D::index_of_texture(char8 *stringa0)
{
  int32 k;

  for (k=0; k<p_NumTextures; k++)
  {
    if (strcmp(p_Textures[k].p_Name, stringa0)==0) return(k);
  }
  return(-1);
}

//--------------------------------------------------------------------

CMesh *CScene3D::get_mesh_pointer (char8 *stringa0)
{
  int32 j;

  j=0; 
  while (j<p_NumMeshes)
  {
    if (strcmp(stringa0, p_Meshes[j]->p_Name)==0)
        return(p_Meshes[j]);
    j++;
  }
  return((CMesh *)NULL);
}

//--------------------------------------------------------------------

CGeometricObject *CScene3D::get_object_pointer (char8 *stringa0)
{
  int32 j;

  j=0; 
  while (j<p_NumObjects)
  {
    if (strcmp(stringa0, p_Objects[j]->p_Name)==0)
        return(p_Objects[j]);
    j++;
  }
  j=0;
  while (j<p_NumOmniLights)
  {
    if (strcmp(stringa0, p_OmniLights[j].p_Name)==0)
        return((CGeometricObject *)&p_OmniLights[j]);
    j++;
  }
  j=0;
  while (j<p_NumSpotLights)
  {
    if (strcmp(stringa0, p_SpotLights[j].p_Name)==0)
        return((CGeometricObject *)&p_SpotLights[j]);
    j++;
  }
  j=0;
  while (j<p_NumCamere)
  {
    if (strcmp(stringa0, p_Telecamere[j].p_Name)==0)
        return((CGeometricObject *)&p_Telecamere[j]);
    j++;
  }
  return((CGeometricObject *)NULL);
}

//--------------------------------------------------------------------

CCamera* CScene3D::get_camera_pointer (char8 *stringa0)
{
  int32 j;

  j=0; 
  while (j<p_NumCamere)
  {
    if (strcmp(stringa0, p_Telecamere[j].p_Name)==0)
        return(&p_Telecamere[j]);
    j++;
  }
  return((CCamera *)NULL);
}

//--------------------------------------------------------------------

CLight* CScene3D::get_omni_pointer (char8 *stringa0)
{
  int32 j;

  j=0; 
  while (j<p_NumOmniLights)
  {
    if (strcmp(stringa0, p_OmniLights[j].p_Name)==0)
        return(&p_OmniLights[j]);
    j++;
  }
  return((CLight *)NULL);
}

//--------------------------------------------------------------------

CSpotLight* CScene3D::get_spot_pointer (char8 *stringa0)
{
  int32 j;

  j=0; 
  while (j<p_NumSpotLights)
  {
    if (strcmp(stringa0, p_SpotLights[j].p_Name)==0)
        return(&p_SpotLights[j]);
    j++;
  }
  return((CSpotLight *)NULL);
}

//--------------------------------------------------------------------

CGraphicSurface* CScene3D::get_texture_pointer (char8 *stringa0)
{
  int32 j;

  
  // controllo nelle texture dei materiali
  j=0; 
  while (j<p_NumTextures)
  {
    if (strcmp(stringa0, p_Textures[j].p_Name)==0)
        return(&p_Textures[j]);
    j++;
  }

  return((CGraphicSurface *)NULL);
}

//--------------------------------------------------------------------

CMaterial* CScene3D::get_material_pointer (char8 *stringa0)
{
  int32 j;

  j=0; 
  while (j<p_NumMaterials)
  {
    if (strcmp(stringa0, p_Materials[j].p_Name)==0)
        return(&p_Materials[j]);
    j++;
  }
  return((CMaterial *)NULL);
}

//--------------------------------------------------------------------

void CScene3D::m_AddAuxiliaryMaterials(void)
{
  int32 i;
  CMaterial *mataux;

  // aggiungiamo un materiale per i tipi di rendering
  // flat e RGB opaco
  mataux=new CMaterial[p_NumMaterials+2];
  for (i=0; i <p_NumMaterials; i++) mataux[i]=p_Materials[i];
  if (p_Materials!=(CMaterial *)NULL) delete [] p_Materials;
  p_Materials=mataux;

  // materiale flat
  strcpy(p_Materials[p_NumMaterials].p_Name, "FLAT");
  p_Materials[p_NumMaterials].p_TransparencyType=OFF;
  p_Materials[p_NumMaterials].p_LightMixType=MIXTYPE_NONE;
  p_Materials[p_NumMaterials].p_NextMaterial=NULL;
  p_Materials[p_NumMaterials].p_WireFrame=1;
  p_Materials[p_NumMaterials].p_WireFrameColor=0xFFFFFFFF;
  strcpy(p_Materials[p_NumMaterials].p_ChildrenName, "");

  // materiale RGB opaco
  strcpy(p_Materials[p_NumMaterials+1].p_Name, "RGB");
  p_Materials[p_NumMaterials+1].p_TransparencyType=OFF;
  p_Materials[p_NumMaterials+1].p_LightMixType=MIXTYPE_MODULATE;
  p_Materials[p_NumMaterials+1].p_NextMaterial=NULL;
  p_Materials[p_NumMaterials].p_WireFrame=0;
  p_Materials[p_NumMaterials].p_WireFrameColor=0xFFFFFFFF;
  //p_Materials[p_NumMaterials+1].p_Specular=ON;
  strcpy(p_Materials[p_NumMaterials+1].p_ChildrenName, "");
}

//--------------------------------------------------------------------

int32 CScene3D::m_Load(char8 *nfile)
{
  char8 fileA3D[150]="";
  char8 fileADM[150]="";
  char8 fileADL[150]="";
  //char8 fileADD[150]="";
  char8 fileADO[150]="";
  char8 fileADP[150]="";
  CFileA3D fA3D;

  fprintf(fdeb,"################################\n");
  fprintf(fdeb,"Printing debug information\n");
  fprintf(fdeb,"################################\n\n");
  fflush(fdeb);

  strcpy(fileA3D, nfile); strcat(fileA3D, ".A3D");
  strcpy(fileADM, nfile); strcat(fileADM, ".ADM");
  strcpy(fileADL, nfile); strcat(fileADL, ".ADL");
  //strcpy(fileADD, nfile); strcat(fileADD, ".ADD");
  strcpy(fileADO, nfile); strcat(fileADO, ".ADO");
  strcpy(fileADP, nfile); strcat(fileADP, ".ADP");

  DumpDebugPoint (fdeb, "Loading ADM..", 0);
  if (esiste_file(fileADM))
	  load_ADM(fileADM, this);
  DumpDebugPoint (fdeb, "ADM loading done", 0);
  m_AddAuxiliaryMaterials();  // aggiungiamo i materiali flat
                              // ed RGB opaco
  DumpDebugPoint (fdeb, "Auxiliary materials added", 0);

  if (esiste_file(fileA3D))
  {
    fA3D.m_LoadFromFile(this, fileA3D);
    DumpDebugPoint (fdeb, "A3D loading done", 0);
  }
  else debug_error(miawin, "File .A3D not present!");

  if (esiste_file(fileADL))
	load_ADL(fileADL, this);
  DumpDebugPoint (fdeb, "ADL loading done", 0);

  if (esiste_file(fileADO))
	load_ADO(fileADO, this);
  DumpDebugPoint (fdeb, "ADO loading done", 0);

  if (esiste_file(fileADP))
	load_ADP(fileADP, this);
  DumpDebugPoint (fdeb, "ADP loading done", 0);

/*
  if (esiste_file(fileADD)) load_ADD(fileADD, this);
*/
  return(1);
}

//--------------------------------------------------------------------

void CScene3D::m_InitMeshes(void)
{
  int32 i;
  char8 msg[150];

  for (i=0; i<p_NumMeshes; i++)
  {
    p_Meshes[i]->m_Init();
	strcpy(msg, p_Meshes[i]->p_Name);
	strcat(msg, " mesh initialization done");
    DumpDebugPoint (fdeb, msg, 0);
  }
}

//--------------------------------------------------------------------

void CScene3D::m_InitObjects(void)
{
  int32 i, j;
  CGeometricObject *fptr;
  CParticleSystem *psys;

  for (i=0; i<p_NumObjects; i++)
  {
    // il linking padre-figlio e' na cazzata col A3D
    // NB : nel .A3D gli oggetti sono presenti e caricati gia'
    // nell'ordine gerarchico giusto, quindi non devo sortarli
	// come faccio nella Init_from_3DS
    if (p_Objects[i]->p_FatherName != (char8 *)NULL)
	{
	  fptr=get_object_pointer(p_Objects[i]->p_FatherName);
      p_Objects[i]->p_Father=fptr;
	  if (fptr!=(CGeometricObject *)NULL) fptr->p_HasChildrens++;
	}
	// sistemo l'eventualità di materiale di base nullo
	if (!p_Objects[i]->p_BaseMaterial)
	{
       // assegno il materiale RGB o FLAT a seconda che ci siano
       // una o piu' luci
		if (p_NumOmniLights+p_NumSpotLights > 0)
          p_Objects[i]->p_BaseMaterial=&p_Materials[p_NumMaterials+1];
	   else
          p_Objects[i]->p_BaseMaterial=&p_Materials[p_NumMaterials];
	}

    if (p_Objects[i]->p_Type==PARTICLE_SYSTEM_OBJECT)
	{
	   psys=(CParticleSystem *)p_Objects[i];
	   for (j=0; j<psys->p_NumWSM; j++)
		   psys->p_LinkedWSM[j]=(CWSMObject *)get_object_pointer(psys->p_WSMNames[j]);

	}
  }

  // questa init non la posso fare prima di contare il numero
  // di tria trasparenti e non a causa della split_tria_list
  for (i=0; i<p_NumObjects; i++)
	if (!p_Objects[i]->m_Init((void *)p_RenderLib)) return;
}

//--------------------------------------------------------------------

void CScene3D::m_InitCamere(void)
{
  int32 i, j;
  CCamera caux;

  // creiamo una telecamera
  if (p_NumCamere<=0)
  {
    /*p_Telecamere[0].p_CurrentFov=1;
    vect_set(&p_Telecamere[0].p_CurrentPosition, 500, 500, 500);
    vect_set(&p_Telecamere[0].p_CurrentTargetPosition, 0, 0, 0);
    p_Telecamere[0].p_Znear=1;
    p_Telecamere[0].p_Zfar=5000;
    p_NumCamere=1;*/
	debug_error(miawin, "No cameras defined! please add at least one camera to your scene!");
  }

  for (j=0; j<p_NumCamere; j++)
  {
     if (!p_Telecamere[j].m_Init((void *)p_RenderLib)) return;
  }

  // ordino le p_Telecamere in ordine di ultima-key crescente
  // per eseguire il multicamera in maniera corretta
  for (i=0; i<p_NumCamere-1; i++)
  {
	for (j=i+1; j<p_NumCamere; j++)
	{
	  if (p_Telecamere[i].p_LastKeyPos > p_Telecamere[j].p_LastKeyPos)
	  {
		caux=p_Telecamere[i];
		p_Telecamere[i]=p_Telecamere[j];
		p_Telecamere[j]=caux;
	  }
	}
  }
  p_NavigationCamera=p_Telecamere[0];
}

//--------------------------------------------------------------------

void CScene3D::m_InitMaterials(void)
{
  int32 i;

  // settiamo i materiali per essere disegnati secondo
  // il metodo previsto/scriptato
  for (i=0; i<p_NumMaterials+2; i++)
  {
	if ((p_NumOmniLights+p_NumSpotLights)<=0)
    // se nella scena non ci sono luci setto i materiali in modo
	// che non richiedano illuminazione, anche se avevano un
	// LightMixType diverso da NONE
    p_Materials[i].p_LightMixType = MIXTYPE_NONE;
	p_Materials[i].p_NextMaterial=get_material_pointer(p_Materials[i].p_ChildrenName);
	p_Materials[i].m_Init(NULL);
    p_RenderLib->m_AddUpdate_Material(&p_Materials[i]);
  }
}

//--------------------------------------------------------------------

void CScene3D::m_InitLights(void)
{
  int32 j;

  for (j=0; j<p_NumOmniLights; j++)
	 p_OmniLights[j].m_Init((void *)p_RenderLib);
  for (j=0; j<p_NumSpotLights; j++)
	 p_SpotLights[j].m_Init((void *)p_RenderLib);
}

//--------------------------------------------------------------------

void CScene3D::m_Init (void)
{
  if (!p_RenderLib)
	 debug_error(miawin, "CScene3D.m_Init: you MUST to setup a RenderLib before init!");

  fprintf(fdeb, "\n\n");
  DumpDebugPoint (fdeb, "Scene Init begin..", 0);
  m_InitMaterials();
  DumpDebugPoint (fdeb, "Materials initialization done", 0);
  fprintf(fdeb, "\n");
  m_InitMeshes();
  DumpDebugPoint (fdeb, "Meshes initialization done", 0);
  fprintf(fdeb, "\n");
  m_InitLights();
  DumpDebugPoint (fdeb, "Lights initialization done", 0);
  fprintf(fdeb, "\n");
  m_InitCamere();
  DumpDebugPoint (fdeb, "Cameras initialization done", 0);
  fprintf(fdeb, "\n");
  m_InitObjects();
  DumpDebugPoint (fdeb, "Objects initialization done", 0);
  fprintf(fdeb, "\n");
}

//--------------------------------------------------------------------

CCamera *CScene3D::m_ChooseCurrentCamera (float4 pos)
// sceglie la telecamera per il frame corrente
{
  int32 cc, flag=0;

  cc=0;
  while ((cc<p_NumCamere) && (!flag))
  {
    if (p_Telecamere[cc].p_LastKeyPos<=pos) cc++;
    else flag=1;
  }

  if (flag==0) return(&p_Telecamere[p_NumCamere-1]);
  else return(&p_Telecamere[cc]);
}

// -------------------------------------------------------------------

void CScene3D::m_TranslateCamera(float4 dx, float4 dy, float4 dz)
{
  AD_Vect3D x, y, z, sum;
  AD_Vect3D ptmp;
  AD_Matrix pretrans;

  if (!p_CurrentCamera) return;
   mat_get_row(&p_CurrentCamera->p_WorldMatrix, 0, &x);
   mat_get_row(&p_CurrentCamera->p_WorldMatrix, 1, &y);
   mat_get_row(&p_CurrentCamera->p_WorldMatrix, 2, &z);

  vect_auto_scale(&x, dx);
  vect_auto_scale(&y, dy);
  vect_auto_scale(&z, dz);
  vect_add(&x, &y, &sum);
  vect_auto_add(&sum, &z);

  vect_auto_add(&p_CurrentCamera->p_CurrentPosition, &sum);
  vect_auto_add(&p_CurrentCamera->p_CurrentTargetPosition, &sum);

  vect_neg(&p_CurrentCamera->p_CurrentPosition, &ptmp);
  mat_setmatrix_of_pretraslation(&pretrans, &ptmp);
  mat_mul(&p_CurrentCamera->p_CurrentRotationMatrix,
	      &pretrans,
		  &p_CurrentCamera->p_WorldMatrix);
}

// -------------------------------------------------------------------

void CScene3D::m_RotateCamera(float4 ax, float4 ay, float4 az)
{
  AD_Vect3D cameradir, xAxis, yAxis, zAxis, v;
  AD_Vect3D vx, vy, vz, ptmp;
  AD_Quaternion q1, q2;
  AD_Matrix rot1, rot2, rot12, pretrans;

  if (!p_CurrentCamera) return;

  // estrazione assi locali
  mat_get_row(&p_CurrentCamera->p_CurrentRotationMatrix, 0, &xAxis);
  mat_get_row(&p_CurrentCamera->p_CurrentRotationMatrix, 1, &yAxis);
  mat_get_row(&p_CurrentCamera->p_CurrentRotationMatrix, 2, &zAxis);

  // creazioni delle matrici di rotazione
  quat_set(&q1, xAxis.x, xAxis.y, xAxis.z, ax);
  quat_set(&q2, yAxis.x, yAxis.y, yAxis.z, ay);

  quat_quat_to_rotquat(&q1); quat_rotquat_to_matrix(&q1, &rot1);
  quat_quat_to_rotquat(&q2); quat_rotquat_to_matrix(&q2, &rot2);
  mat_mul(&rot1, &rot2, &rot12);

  // ruoto la posizione
  vect_sub(&p_CurrentCamera->p_CurrentPosition,
           &p_CurrentCamera->p_CurrentTargetPosition,
		   &cameradir);
  mat_mulvect(&rot12, &cameradir, &v);
  vect_add(&v, &p_CurrentCamera->p_CurrentTargetPosition,
		   &p_CurrentCamera->p_CurrentPosition);
  
  // ruoto il target
  /*vect_sub(&p_CurrentCamera->p_CurrentTargetPosition,
           &p_CurrentCamera->p_CurrentPosition,
		   &cameradir);
  mat_mulvect(&rot12, &cameradir, &v);
  vect_add(&v, &p_CurrentCamera->p_CurrentPosition,
		   &p_CurrentCamera->p_CurrentTargetPosition);*/

  mat_mulvect(&rot12, &xAxis, &vx); vect_auto_normalize(&vx);
  mat_mulvect(&rot12, &yAxis, &vy); vect_auto_normalize(&vy);
  mat_mulvect(&rot12, &zAxis, &vz); vect_auto_normalize(&vz);
  mat_insert_row(&p_CurrentCamera->p_CurrentRotationMatrix, 0, &vx);
  mat_insert_row(&p_CurrentCamera->p_CurrentRotationMatrix, 1, &vy);
  mat_insert_row(&p_CurrentCamera->p_CurrentRotationMatrix, 2, &vz);
  vect_neg(&p_CurrentCamera->p_CurrentPosition, &ptmp);
  mat_setmatrix_of_pretraslation(&pretrans, &ptmp);
  mat_mul(&p_CurrentCamera->p_CurrentRotationMatrix,
	      &pretrans,
		  &p_CurrentCamera->p_WorldMatrix);
}

//--------------------------------------------------------------------

CGeometricObject *l_TranspObj[300];
CGeometricObject *l_SwapObj;
void l_QsortTransparentObjects (int32 lo, int32 hi)
// questa procedura fa il quick sort dei triangoli ordinandoli in base allo
// zmedio; per ordinare tutti i triangoli qsort_triangoli(0, num_tria-1)
// N.B.: i triangoli trasparenti DEVONO essere ordinati dal
// piu' lontano al piu' vicino e apintati in quest'ordine
{
  int32 i, j;
  float4 zq;
  
  i = lo;
  j = hi;
  // (lo+hi) >> 1 e' l'indice medio tra lo e hi
  zq = l_TranspObj[(lo+hi) >> 1]->p_CurrentPositionInCameraSpace.z;
  do
  {
    while (l_TranspObj[i]->p_CurrentPositionInCameraSpace.z > zq) i++;
	while (zq > l_TranspObj[j]->p_CurrentPositionInCameraSpace.z) j--;
	if (i <= j) 
	{
      l_SwapObj = l_TranspObj[i];
      l_TranspObj[i] = l_TranspObj[j];
      l_TranspObj[j] = l_SwapObj;
      i++;
      j--;
	}
  } while (i<=j);
  if (lo<j) l_QsortTransparentObjects(lo, j);
  if (i<hi) l_QsortTransparentObjects(i, hi);
}

//--------------------------------------------------------------------

void CScene3D::m_DoEfx2D(double pos)
// si suppone pos compreso tra [0..1]
{
  float4 frame;
  int32 j;
  int32 l_NumTranspObj;

  p_FramesElapsed++;
  frame=(float4)(m_GetRelPos(pos)*(p_NumFrames-1));

  p_RenderLib->p_Device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

  // estraggo e setto la telecamera corrente
  p_CurrentCamera=m_ChooseCurrentCamera(frame);
  //if (p_CurrentCamera->p_Type != FREE_CAMERA)
	  p_CurrentCamera->m_Update(frame);

  p_RenderLib->m_SetViewMatrix(&p_CurrentCamera->p_ViewMatrix);
  p_RenderLib->m_SetProjMatrix(p_CurrentCamera->p_CurrentFov,
	                           1.3333333f,
							   p_CurrentCamera->p_Znear,
                               p_CurrentCamera->p_Zfar);

  // update per le texture
  for (j=0; j<p_NumTextures; j++)
     p_Textures[j].m_Update(frame);

  // update per i materiali
  for (j=0; j<p_NumMaterials; j++)
	 p_Materials[j].m_Update(frame);

  for (j=0; j<p_NumOmniLights; j++)
  {
	 p_OmniLights[j].m_Update(frame);
  }

  //for (j=0; j<p_NumOmniLights; j++)
//	 p_OmniLights[j].m_Paint((void *)p_RenderLib, NULL, NULL);

  l_NumTranspObj=0;
  for (j=0; j<p_NumObjects; j++)
  {
	 p_Objects[j]->m_Update(frame);
	 mat_mulvect(&p_CurrentCamera->p_WorldMatrix,
                 &p_Objects[j]->p_CurrentPosition,
		         &p_Objects[j]->p_CurrentPositionInCameraSpace);
	 if ((p_Objects[j]->p_BaseMaterial->p_TransparencyType != OFF) ||
         (p_Objects[j]->p_Type==PARTICLE_SYSTEM_OBJECT))
	 {
        l_TranspObj[l_NumTranspObj]=p_Objects[j];
		l_NumTranspObj++;
	 }
  }

  // disegno degli oggetti NON trasparenti
  int32 l_MaxLight;
  if (p_NumOmniLights > p_RenderLib->p_MaxLightsAllowed)
     l_MaxLight=p_RenderLib->p_MaxLightsAllowed;
  else l_MaxLight=p_NumOmniLights;
  
  for (j=0; j<p_NumObjects; j++)
  {
	 if ((p_Objects[j]->p_BaseMaterial->p_TransparencyType==OFF) &&
         (p_Objects[j]->p_Type!=PARTICLE_SYSTEM_OBJECT))
     p_Objects[j]->m_Paint((void *)p_RenderLib, (void *)p_OmniLights,
		                   (void *)&l_MaxLight);
  }

  // disegno degli oggetti trasparenti
  if (l_NumTranspObj > 0)
  {
     p_RenderLib->m_BeginTransparency();
     l_QsortTransparentObjects(0, l_NumTranspObj-1);
     for (j=0; j<l_NumTranspObj; j++)
	 {
        l_TranspObj[j]->m_Paint((void *)p_RenderLib, (void *)p_OmniLights,
		                        (void *)&l_MaxLight);
	 }
     p_RenderLib->m_EndTransparency();
  }
}