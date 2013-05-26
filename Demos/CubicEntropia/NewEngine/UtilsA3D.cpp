// A3Dfile.cpp: implementation of the A3Dfile class.
//
//////////////////////////////////////////////////////////////////////
#define MAXA3DFILESIZE 1024*1024*2 // max 2 Mb

#include <stdio.h>
#include <io.h>
#include <string.h>
#include <math.h>
#include "UtilsA3D.h"

char8 material_no_found[5]="";
extern HWND miawin;         // finestra principale; usata qui per le debug_error

CFileA3D::CFileA3D()
{
  p_NumMaterials=0;
  strcpy(p_CurrentA3DFileName, "");
}

//--------------------------------------------------------------------

char8 *CFileA3D::m_AllocA3D(char8 *filename)
// carica in memoria il file .A3D
// restituisce la dimensione allocata in byte o -1 se c'e' stato un errore
{
  FILE *bfile;
  int32 lung, i;
  char8 *fileA3D;

  if ((bfile=fopen(filename, "rb"))==(FILE *)0x0) return (char8 *)0;
  lung=filelength(fileno(bfile));
  fileA3D=new char8[lung];
  for (i=0; i<lung; i++) 
	 fread(&fileA3D[i], 1, 1, bfile);

  fclose(bfile);
  return (fileA3D);
}


char8 *CFileA3D::m_GetNextChunk(char8 *current_chunk)
{
  // NB: la procedura presuppone che current_chunk punti
  // ad un chunk ID
  char8 *c;
  int32 size;

  c=current_chunk+sizeof(int32);  // salto il chunk ID
  for (; *c!='\0'; c++) {}

  c++; // salto anche il '\0'
  size=*((int32 *)c);
  c=c+4+size;  // il +4 e' il campo size

  return(c);
}


void CFileA3D::m_Load_GeneralInfo(char8 *chunk)
{
  int32 *dati, sz, ff, lf;

  sz=strlen(chunk+4);
  dati=(int32 *)(chunk+4+(sz+1)+4);

  ff=*dati; dati++;  // first frame
  lf=*dati; dati++;  // last frame
  p_MyScene->p_NumFrames=lf-ff;  // numero di frames totali
  p_MyScene->p_FramesElapsed=0;
  // to do: leggere anche la bounding box totale al tempo t=0
}

// ###########################################################
// #####  LOAD DI OGGETTI GEOMETRICI (helper(s), solid  ######
// ###########################################################

void CFileA3D::m_Load_GeometricObject(CGeometricObject *obj, char8 *chunk)
{
  int32 is_a_children, *dati, sz, there_is_material;
  int32 has_a_mesh;
  char8 *c;
  float4 *n;
  AD_Vect3D localx, localy, localz;
  CMesh *mesh;


  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  strcpy(obj->p_Name, chunk+4);

  dati=(int32 *)(chunk+4+(sz+1)+4);
  // leggo il tipo di oggetto
  obj->p_Type=*dati; dati++;
  // leggo se ha padre, e se si leggo il nome di tale padre
  is_a_children=*dati;
  if (is_a_children)
  {
	 c=(char8 *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 strcpy(obj->p_FatherName, c);
	 c=c+sz+1;
  }
  else c=(char8 *)(dati+1);  // non c'e' nome, quindi salto solo il flag

  // leggo se ha un materiale
  dati=(int32 *)c;
  there_is_material=*dati;
  c=c+4;  // punto all'inizio dell'eventuale nome del materiale

  if (there_is_material)
  {
	 sz=strlen(c);
     obj->p_BaseMaterial=p_MyScene->get_material_pointer(c);
	 c=c+(sz+1);
  }

  // ----------------    lettura pivot       -------------------------
  n=(float4 *)c;
  obj->p_Pivot.x=*n; n++;
  obj->p_Pivot.y=*n; n++;
  obj->p_Pivot.z=*n; n++;
  vect_swap_3DS(&obj->p_Pivot);
  // --------   lettura del local coordinate system   ---------------
  localx.x=*n; n++;
  localx.y=*n; n++;  
  localx.z=*n; n++;
  localy.x=*n; n++;  
  localy.y=*n; n++;
  localy.z=*n; n++;
  localz.x=*n; n++;
  localz.y=*n; n++;
  localz.z=*n; n++;
  obj->p_CurrentPosition.x=*n; n++;
  obj->p_CurrentPosition.y=*n; n++;
  obj->p_CurrentPosition.z=*n; n++;
  vect_swap_3DS(&obj->p_CurrentPosition);

  // -------------------   lettura bounding box   --------------------
  n=n+6; // salto la bmin e bmax (bounding box)
  dati=(int32 *)n;

  // --------------   leggo se ha una mesh associata   ---------------
  has_a_mesh=*dati; dati++;
  if ((has_a_mesh) && (obj->p_NumLods < MAX_LODS))
  {
	 c=(char8 *)dati;
	 mesh=p_MyScene->get_mesh_pointer(c);
     int32 flag=0;
	 if (mesh)
	 {
		for (sz=0; sz<obj->p_NumLods; sz++)
		   if (obj->p_Lods[sz].Mesh==mesh) flag=1;
	    if (!flag) obj->p_Lods[obj->p_NumLods++].Mesh=mesh;
	 }
  }
}

/*
void CFileA3D::m_Load_Helper(CHelper *helper, char8 *chunk)
{
   int32 sz, *dati;
   char8 *c;
   float4 *f;

   // settaggi iniziali per non avere casini
   helper->p_Father=(CGeometricObject *)NULL;
   //obj->flare=(texture *)NULL;

   sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
   helper->p_Name=new char[sz+1];
   strcpy(helper->p_Name, chunk+4);  // scrivo il nome del'helper

   dati=(int32 *)(chunk+4+(sz+1)+4);
   // if (*dati==0) helper->p_type=DUMMY;
   // else helper->p_type=BONE;

   c=(chunk+4+(sz+1)+4+4);   // punto al nome del padre
   sz=strlen(c);
   helper->p_FatherName=new char[sz+1];
   strcpy(helper->p_FatherName, c);
   c=c+(sz+1);
   f=(float4 *)(c);

   // lettura posizione nel mondo
   helper->p_CurrentPosition.x=*f; f++;
   helper->p_CurrentPosition.y=*f; f++;
   helper->p_CurrentPosition.z=*f; f++;
   vect_swap_3DS(&helper->p_CurrentPosition);

   // lettura punto pivot (inutile, non serve)
   helper->p_Pivot.x=*f; f++;
   helper->p_Pivot.y=*f; f++;
   helper->p_Pivot.z=*f;
   vect_swap_3DS(&helper->p_Pivot);
}
*/



// ###########################################################
// ####################   LOAD DI MESH   #####################
// ###########################################################

void CFileA3D::m_Load_Mesh_VertexList(CMesh *mesh, char8 *chunk)
{
  int32 *num_vert, sz, j;
  float4 *f, x, y, z;

  
  if (mesh == (CMesh *)NULL) return;

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;  // puntiamo ai dati

  num_vert=(int32 *)chunk;
  mesh->p_NumObjspaceVertex=*num_vert;
  num_vert=num_vert+1;

  mesh->p_ObjspaceVertex=new AD_Vect3D[mesh->p_NumObjspaceVertex];
  f=(float4 *)(num_vert);
  for (j=0; j<mesh->p_NumObjspaceVertex; j++)
  {
    x=*f;  f++;
    y=*f;  f++;
    z=*f;  f++;
    vect_set(&mesh->p_ObjspaceVertex[j], x, y, z);
    vect_swap_3DS(&mesh->p_ObjspaceVertex[j]);
  }
}	  

//--------------------------------------------------------------------

void CFileA3D::m_Load_Mesh_FaceList(CMesh *mesh, char8 *chunk)
{
  int32 *data, sz, j;
  int32 size;
  uchar8 c1, c2, c3, *c;
  uint16 s1, s2, s3, *s;

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;  // puntiamo ai dati

  data=(int32 *)chunk;
  mesh->p_NumTriangles=*data; data++;
  size=*data; data++;

  mesh->p_Triangles=new AD_Tria3D[mesh->p_NumTriangles];
  mesh->p_SmoothingGroups=new int32[mesh->p_NumTriangles];

  if (size==8)
  {
    for (j=0; j<mesh->p_NumTriangles; j++)
	{
      c=(uchar8 *)data; c1=*c; c++; c2=*c; c++; c3=*c; c++;
      mesh->p_Triangles[j].i1=(int32)c1;
      mesh->p_Triangles[j].i2=(int32)c2;
      mesh->p_Triangles[j].i3=(int32)c3;
	  data=(int32 *)c;
      mesh->p_SmoothingGroups[j]=*data; data++;
      mesh->p_Triangles[j].materiale=(CMaterial *)NULL;
	}
  }
  else
  if (size==16)
  {
    for (j=0; j<mesh->p_NumTriangles; j++)
	{
      s=(uint16 *)data; s1=*s; s++; s2=*s; s++; s3=*s; s++;
      mesh->p_Triangles[j].i1=(int32)s1;
      mesh->p_Triangles[j].i2=(int32)s2;
      mesh->p_Triangles[j].i3=(int32)s3;
	  data=(int32 *)s;
      mesh->p_SmoothingGroups[j]=*data; data++;
      mesh->p_Triangles[j].materiale=(CMaterial *)NULL;
	}
  }
  else
  {
    for (j=0; j<mesh->p_NumTriangles; j++)
	{
      mesh->p_Triangles[j].i1=*data; data++;
      mesh->p_Triangles[j].i2=*data; data++;
      mesh->p_Triangles[j].i3=*data; data++;
      mesh->p_SmoothingGroups[j]=*data; data++;
      mesh->p_Triangles[j].materiale=(CMaterial *)NULL;
	}
  }
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_Mesh_TextureVertexList(CMesh *mesh, char8 *chunk)
{
  int32 *dati, sz, j, channel;
  float4 *f;

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;
  dati=(int32 *)chunk;

  channel=*dati; dati++;
  if (channel==1)
  {
     mesh->p_NumUV1vertex=*dati; dati++;
     mesh->p_UV1vertex=new AD_VectUV[mesh->p_NumUV1vertex];
     f=(float4 *)dati;
     for (j=0; j<mesh->p_NumUV1vertex; j++)
	 {
        mesh->p_UV1vertex[j].u=(*f); f++;
        mesh->p_UV1vertex[j].v=(*f); f++;
	 }
  }
  else
  {
     mesh->p_NumUV2vertex=*dati; dati++;
     mesh->p_UV2vertex=new AD_VectUV[mesh->p_NumUV2vertex];
     f=(float4 *)dati;
     for (j=0; j<mesh->p_NumUV2vertex; j++)
	 {
        mesh->p_UV2vertex[j].u=(*f); f++;
        mesh->p_UV2vertex[j].v=(*f); f++;
	 }
  }
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_Mesh_TextureFaceList(CMesh *mesh, char8 *chunk)
{
  int32 *dati, sz, j, nt, ta, tb, tc;
  int32 channel, size;
  uchar8 *c, c1, c2, c3;
  uint16 *s, s1, s2, s3, g_id;

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;
  dati=(int32 *)chunk;

  // leggo il canale di mapping
  channel=*dati;
  dati++;
  nt=*dati;    // leggo il numero di facce che deve essere consistente
  if (nt != mesh->p_NumTriangles)
	 debug_error(miawin, "CFileA3D::m_Load_Mesh_TextureFaceList, numero triangoli inconsistente!");
  dati++;
  // lego la size di ogni indice
  size=*dati; dati++;

  if (size==8)
  {
    for (j=0; j<nt; j++)
	{
      c=(uchar8 *)dati;
	  c1=*c; c++; c2=*c; c++; c3=*c; c++;
	  if (channel==1)
	  {
        mesh->p_Triangles[j].UV1_i1=(int32)c1;
        mesh->p_Triangles[j].UV1_i2=(int32)c2;
        mesh->p_Triangles[j].UV1_i3=(int32)c3;
	  }
	  else
	  {
        mesh->p_Triangles[j].UV2_i1=(int32)c1;
        mesh->p_Triangles[j].UV2_i2=(int32)c2;
        mesh->p_Triangles[j].UV2_i3=(int32)c3;
	  }
	  s=(uint16 *)c;
	  g_id=*s; s++;
	  dati=(int32 *)s;
	  mesh->p_Triangles[j].materiale=p_MyScene->get_material_pointer(m_GetMaterialName(g_id));
	}
  }
  else
  if (size==16)
  {
    for (j=0; j<nt; j++)
	{
      s=(uint16 *)dati;
	  s1=*s; s++; s2=*s; s++; s3=*s; s++;
	  if (channel==1)
	  {
        mesh->p_Triangles[j].UV1_i1=(int32)s1;
        mesh->p_Triangles[j].UV1_i2=(int32)s2;
        mesh->p_Triangles[j].UV1_i3=(int32)s3;
	  }
	  else
	  {
        mesh->p_Triangles[j].UV2_i1=(int32)s1;
        mesh->p_Triangles[j].UV2_i2=(int32)s2;
        mesh->p_Triangles[j].UV2_i3=(int32)s3;
	  }
	  g_id=*s; s++;
	  dati=(int32 *)s;
	  mesh->p_Triangles[j].materiale=p_MyScene->get_material_pointer(m_GetMaterialName(g_id));
	}
  }
  else
  for (j=0; j<nt; j++)
  {
	ta=*dati;     dati++;
	tb=*dati;     dati++;
	tc=*dati;     dati++;
	s=(uint16 *)dati;
	g_id=*s; s++;
	dati=(int32 *)s;
	if (channel==1)
	{
       mesh->p_Triangles[j].UV1_i1=ta;
       mesh->p_Triangles[j].UV1_i2=tb;
       mesh->p_Triangles[j].UV1_i3=tc;
	}
	else
	{
       mesh->p_Triangles[j].UV2_i1=ta;
       mesh->p_Triangles[j].UV2_i2=tb;
       mesh->p_Triangles[j].UV2_i3=tc;
	}
	mesh->p_Triangles[j].materiale=p_MyScene->get_material_pointer(m_GetMaterialName(g_id));
  }
}



// ###########################################################
// ###################  LOAD DI TELECAMERE  ##################
// ###########################################################
void CFileA3D::m_Load_Camera_Settings (CCamera *cam, char8 *chunk)
{
  float4 *f;
  int32 sz;

  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  strcpy(cam->p_Name, chunk+4);

  chunk=chunk+4+(sz+1)+4;  // punto ai dati: salto ID, name_owner, size
  f=(float4 *)chunk;

  cam->p_Znear=*f; f++;
  cam->p_Zfar=*f; f++;
  cam->p_CurrentFov=*f; f++;
  cam->p_CurrentRoll=*f; f++;

  cam->p_CurrentPosition.x=*f; f++;
  cam->p_CurrentPosition.y=*f; f++;
  cam->p_CurrentPosition.z=*f; f++;
  vect_swap_3DS(&cam->p_CurrentPosition);

  cam->p_CurrentTargetPosition.x=*f; f++;
  cam->p_CurrentTargetPosition.y=*f; f++;
  cam->p_CurrentTargetPosition.z=*f; f++;
  vect_swap_3DS(&cam->p_CurrentTargetPosition);

  // calcolo il FOV/2 e relativa tangente
  cam->p_CurrentFov=cam->p_CurrentFov;
  cam->p_CurrentTanFov=(float4)tan((float4)fabs(cam->p_CurrentFov));
}




// ###########################################################
// ###################  LOAD DI OMNILIGHT  ###################
// ###########################################################

void CFileA3D::m_Load_Omnilight_Settings(CLight *ol, char8 *chunk)
{
  float4 *f;
  int32 sz, *dati, is_a_children;
  char8 *c;
  AD_Vect3D localx, localy, localz;

  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  strcpy(ol->p_Name, chunk+4);

  dati=(int32 *)(chunk+4+(sz+1)+4);
  is_a_children=*dati;
  if (is_a_children)
  {
	 c=(char8 *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 strcpy(ol->p_FatherName, c);
	 c=c+sz+1;
  }
  else c=(char8 *)(dati+1);  // non c'e' nome, quindi salto solo il flag

  // lettura pivot
  f=(float4 *)c;
  ol->p_Pivot.x=*f; f++;
  ol->p_Pivot.y=*f; f++;
  ol->p_Pivot.z=*f; f++;
  vect_swap_3DS(&ol->p_Pivot);
  // lettura del local coordinate system
  localx.x=*f; f++;
  localx.y=*f; f++;  
  localx.z=*f; f++;
  localy.x=*f; f++;  
  localy.y=*f; f++;
  localy.z=*f; f++;
  localz.x=*f; f++;
  localz.y=*f; f++;
  localz.z=*f; f++;
  // lettura world position al tempo t=0
  ol->p_CurrentPosition.x=*f; f++;
  ol->p_CurrentPosition.y=*f; f++;
  ol->p_CurrentPosition.z=*f; f++;
  vect_swap_3DS(&ol->p_CurrentPosition);
  // lettura del colore al tempo t=0
  ol->p_CurrentColor.x=(float4)(RGB_MAXVALUE*(*f)); f++;
  ol->p_CurrentColor.y=(float4)(RGB_MAXVALUE*(*f)); f++;
  ol->p_CurrentColor.z=(float4)(RGB_MAXVALUE*(*f));
}


// ###########################################################
// ###################  LOAD DI SPOTLIGHT  ###################
// ###########################################################
void CFileA3D::m_Load_Spotlight_Settings(CSpotLight *sl, char8 *chunk)
{
  float4 *f;
  int32 sz, *dati, there_is_material, is_a_children;
  char8 *c;


  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  strcpy(sl->p_Name, chunk+4);

  dati=(int32 *)(chunk+4+(sz+1)+4);
  is_a_children=*dati;
  if (is_a_children)
  {
	 c=(char8 *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 strcpy(sl->p_FatherName, c);
	 c=c+sz+1;
  }
  else c=(char8 *)(dati+1);  // non c'e' nome, quindi salto solo il flag

  dati=(int32 *)c;
  there_is_material=*dati;
  c=c+4;  // punto all'inizio dell'eventuale nome del materiale

  if (there_is_material)
  {
	 sz=strlen(c);
     sl->p_BaseMaterial=p_MyScene->get_material_pointer(c);
	 c=c+(sz+1);
  }
  chunk=c;
  f=(float4 *)chunk;

  sl->p_CurrentColor.x=(float4)(RGB_MAXVALUE*(*f)); f++;
  sl->p_CurrentColor.y=(float4)(RGB_MAXVALUE*(*f)); f++;
  sl->p_CurrentColor.z=(float4)(RGB_MAXVALUE*(*f)); f++;
  sl->p_CurrentFalloff=*f; f++;
  sl->p_CurrentHotspot=*f; f++;
}


// ###########################################################
// #################  LOAD PARTICLE SYSTEM  ##################
// ###########################################################
void CFileA3D::m_Load_ParticleSystem_Settings(CParticleSystem *ps, char8 *chunk)
{
  int32 is_a_children, *dati, sz, there_is_material, has_mesh;
  int32 k;
  char8 *c;
  float4 *f;
  AD_Vect3D localx, localy, localz;

  typedef char8 *char_ptr;

  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  strcpy(ps->p_Name, chunk+4);

  dati=(int32 *)(chunk+4+(sz+1)+4);
  // leggo se ha padre, e se si leggo il nome di tale padre
  is_a_children=*dati;
  if (is_a_children)
  {
	 c=(char8 *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 strcpy(ps->p_FatherName, c);
	 c=c+sz+1;
  }
  else c=(char8 *)(dati+1);  // non c'e' nome, quindi salto solo il flag

  // leggo se ha un materiale
  dati=(int32 *)c;
  there_is_material=*dati;
  c=c+4;  // punto all'inizio dell'eventuale nome del materiale

  if (there_is_material)
  {
	 sz=strlen(c);
     ps->p_BaseMaterial=p_MyScene->get_material_pointer(c);
	 c=c+(sz+1);
  }

  // ----------------    lettura pivot       -------------------------
  f=(float4 *)c;
  ps->p_Pivot.x=*f; f++;
  ps->p_Pivot.y=*f; f++;
  ps->p_Pivot.z=*f; f++;
  vect_swap_3DS(&ps->p_Pivot);
  // --------   lettura del local coordinate system   ---------------
  localx.x=*f; f++;
  localx.y=*f; f++;  
  localx.z=*f; f++;
  localy.x=*f; f++;  
  localy.y=*f; f++;
  localy.z=*f; f++;
  localz.x=*f; f++;
  localz.y=*f; f++;
  localz.z=*f; f++;
  ps->p_CurrentPosition.x=*f; f++;
  ps->p_CurrentPosition.y=*f; f++;
  ps->p_CurrentPosition.z=*f; f++;
  vect_swap_3DS(&ps->p_CurrentPosition);

  dati=(int32 *)f;
  has_mesh=*dati; dati++;
  if (has_mesh)
  {
	 // TO DO
  }

  f=(float4 *)dati;
  ps->p_EmitterWidth=*f;     f++;
  ps->p_EmitterHeight=*f;    f++;
  ps->p_EmitterSpeed=*f;     f++;
  ps->p_EmitterVariation=*f; f++;
  dati=(int32 *)f;
  sz=*dati;
  ps->p_Life=(float4)sz;
  dati++;
  sz=*dati;
  ps->p_StartTime=(float4)sz;
  dati++;
  sz=*dati;
  ps->p_EndTime=(float4)sz;
  dati++;
  ps->p_MaxParticles=*dati;
  dati++;

  ps->p_NumWSM=*dati; dati++;
  if (ps->p_NumWSM<=0) return;

  ps->p_LinkedWSM=new CWSMObjectPtr[ps->p_NumWSM];
  ps->p_WSMNames=new char_ptr[ps->p_NumWSM];
  c=(char8 *)dati;
  // leggo i nomi dei WSM bindati a questo particle system
  for (k=0; k<ps->p_NumWSM; k++)
  {
     sz=strlen(c);
     ps->p_WSMNames[k]=new char8[sz+1];
	 strcpy(ps->p_WSMNames[k], c);
	 ps->p_LinkedWSM[k]=(CWSMObject *)NULL;
	 c=c+sz+1;
  }
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_WindModifier_Settings(CWindModifier *wind, char8 *chunk)
{
   int32 sz, *dati, is_a_children;
   char8 *c;
   float4 *f;
   AD_Vect3D localx, localy, localz;

  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  strcpy(wind->p_Name, chunk+4);

  dati=(int32 *)(chunk+4+(sz+1)+4);
  // leggo se ha padre, e se si leggo il nome di tale padre
  is_a_children=*dati;
  if (is_a_children)
  {
	 c=(char8 *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 strcpy(wind->p_FatherName, c);
	 c=c+sz+1;
  }
  else c=(char8 *)(dati+1);  // non c'e' nome, quindi salto solo il flag

  // ----------------    lettura pivot       -------------------------
  f=(float4 *)c;
  wind->p_Pivot.x=*f; f++;
  wind->p_Pivot.y=*f; f++;
  wind->p_Pivot.z=*f; f++;
  vect_swap_3DS(&wind->p_Pivot);
  // --------   lettura del local coordinate system   ---------------
  localx.x=*f; f++;
  localx.y=*f; f++;  
  localx.z=*f; f++;
  localy.x=*f; f++;  
  localy.y=*f; f++;
  localy.z=*f; f++;
  localz.x=*f; f++;
  localz.y=*f; f++;
  localz.z=*f; f++;
  wind->p_CurrentPosition.x=*f; f++;
  wind->p_CurrentPosition.y=*f; f++;
  wind->p_CurrentPosition.z=*f; f++;
  vect_swap_3DS(&wind->p_CurrentPosition);

  wind->p_Strength=*f; f++;
  wind->p_Decay=*f;    f++;
  dati=(int32 *)f;
  wind->p_Mapping=*dati; dati++;
  f=(float4 *)dati;
  wind->p_Turbolence=*f;   f++;
  wind->p_Frequency=*f;    f++;
  wind->p_Scale=*f;
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_GravityModifier_Settings(CGravityModifier *gravity, char8 *chunk)
{
   int32 sz, *dati, is_a_children;
   char8 *c;
   float4 *f;
   AD_Vect3D localx, localy, localz;

  sz=strlen(chunk+4);    // punto al nome e leggo la sua lunghezza
  strcpy(gravity->p_Name, chunk+4);

  dati=(int32 *)(chunk+4+(sz+1)+4);
  // leggo se ha padre, e se si leggo il nome di tale padre
  is_a_children=*dati;
  if (is_a_children)
  {
	 c=(char8 *)(dati+1);   // puntiamo all'inizio del nome del padre
     sz=strlen(c);
	 strcpy(gravity->p_FatherName, c);
	 c=c+sz+1;
  }
  else c=(char8 *)(dati+1);  // non c'e' nome, quindi salto solo il flag

  // ----------------    lettura pivot       -------------------------
  f=(float4 *)c;
  gravity->p_Pivot.x=*f; f++;
  gravity->p_Pivot.y=*f; f++;
  gravity->p_Pivot.z=*f; f++;
  vect_swap_3DS(&gravity->p_Pivot);
  // --------   lettura del local coordinate system   ---------------
  localx.x=*f; f++;
  localx.y=*f; f++;  
  localx.z=*f; f++;
  localy.x=*f; f++;  
  localy.y=*f; f++;
  localy.z=*f; f++;
  localz.x=*f; f++;
  localz.y=*f; f++;
  localz.z=*f; f++;
  gravity->p_CurrentPosition.x=*f; f++;
  gravity->p_CurrentPosition.y=*f; f++;
  gravity->p_CurrentPosition.z=*f; f++;
  vect_swap_3DS(&gravity->p_CurrentPosition);

  gravity->p_Strength=*f; f++;
  gravity->p_Decay=*f;    f++;
  dati=(int32 *)f;
  gravity->p_Mapping=*dati;
}

// ###########################################################
// #####################  LOAD KEYFRAME  #####################
// ###########################################################

void CFileA3D::m_Load_HideTrack(CHideTrack *ht, char8 *chunk)
{
  int32 sz;
  uchar8 type, ORT, *t;
  int32 nkey, *v;

  sz=strlen(chunk+4);       // punto al nome e leggo la sua lunghezza
  chunk=chunk+4+(sz+1)+4;   // punto direttamente ai dati

  t=(uchar8 *)chunk;
  type=*t; t=t++;           // leggo il tipo di controller
                            // che tanto per le hiding track non ha senso
  ht->p_Type=BOOLEAN_TRACK;

  // lettura delle opzioni per l'"out of range"
  ORT=*t; ht->p_ORTBefore=(int32)ORT; t++;
  ORT=*t; ht->p_ORTAfter=(int32)ORT; t++;

  v=(int32 *)t;
  nkey=*v; v++;             // leggo il numero di key
  ht->p_NumKeys=nkey;
  ht->p_Keys=new HIDEKEY[ht->p_NumKeys];

  for (nkey=0; nkey<ht->p_NumKeys; nkey++)
  {
	 ht->p_Keys[nkey]=*v;
	 v=v++;
  }
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_PosTrack(CPosTrack *pt, char8 *chunk)
{
  int32 sz;
  uchar8 type, ORT, *t;
  int32 nkey, *v;
  float4 *f;

  sz=strlen(chunk+4);       // punto al nome e leggo la sua lunghezza
  chunk=chunk+4+(sz+1)+4;   // punto direttamente ai dati

  t=(uchar8 *)chunk;
  type=*t; t=t++;             // leggo il tipo di controller
  switch (type)
  {
    case TCB_CONTROLLER:
           pt->p_Type=pt->p_Type | TCB_TRACK;
		   break;
	case BEZIER_CONTROLLER:
           pt->p_Type=pt->p_Type | BEZIER_TRACK;
		   break;
	case LINEAR_CONTROLLER:
           pt->p_Type=pt->p_Type | BEZIER_TRACK;
		   break;
  }

  // lettura delle opzioni per l'"out of range"
  ORT=*t; pt->p_ORTBefore=(int32)ORT; t++;
  ORT=*t; pt->p_ORTAfter=(int32)ORT; t++;

  v=(int32 *)t;
  nkey=*v; v++;             // leggo il numero di key
  pt->p_NumKeys=nkey;
  pt->p_Keys=new POSKEY[pt->p_NumKeys];

  f=(float4 *)v;
  for (nkey=0; nkey<pt->p_NumKeys; nkey++)
  {
	 v=(int32 *)f;
	 pt->p_Keys[nkey].posintrack=*v;
	 v=v+1;

	 f=(float4 *)v;
	 pt->p_Keys[nkey].p.x=*f; f++;
	 pt->p_Keys[nkey].p.y=*f; f++;
	 pt->p_Keys[nkey].p.z=*f; f++;
     vect_swap_3DS(&pt->p_Keys[nkey].p);

	 if (type==TCB_CONTROLLER)
	 {
	    pt->p_Keys[nkey].tension=*f;     f++;
	    pt->p_Keys[nkey].continuity=*f;  f++;
	    pt->p_Keys[nkey].bias=*f;        f++;
	    pt->p_Keys[nkey].easeto=*f;      f++;
	    pt->p_Keys[nkey].easefrom=*f;    f++;
	 }
	 else
	 if (type==BEZIER_CONTROLLER)
	 {
	    pt->p_Keys[nkey].incomtg.x=(*f); f++;
	    pt->p_Keys[nkey].incomtg.y=(*f); f++;
	    pt->p_Keys[nkey].incomtg.z=(*f); f++;
	    pt->p_Keys[nkey].outcomtg.x=(*f); f++;
	    pt->p_Keys[nkey].outcomtg.y=(*f); f++;
	    pt->p_Keys[nkey].outcomtg.z=(*f); f++;
	    pt->p_Keys[nkey].tension=0;
	    pt->p_Keys[nkey].continuity=0;
	    pt->p_Keys[nkey].bias=0;
	    pt->p_Keys[nkey].easeto=0;
	    pt->p_Keys[nkey].easefrom=0;
		vect_swap_3DS(&pt->p_Keys[nkey].incomtg);
		vect_swap_3DS(&pt->p_Keys[nkey].outcomtg);
	 }
	 else
	 if (type==LINEAR_CONTROLLER)
	 {
	    pt->p_Keys[nkey].tension=1;
	    pt->p_Keys[nkey].continuity=0;
	    pt->p_Keys[nkey].bias=0;
	    pt->p_Keys[nkey].easeto=0;
	    pt->p_Keys[nkey].easefrom=0;
	 }
  }
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_RotTrack(CRotTrack *rt, char8 *chunk)
{
  int32 sz;
  uchar8 type, *t, ORT;
  int32 nkey, *v;
  float4 *f, angle;

  sz=strlen(chunk+4);       // punto al nome e leggo la sua lunghezza
  chunk=chunk+4+(sz+1)+4;   // punto direttamente ai dati

  t=(uchar8 *)chunk;
  type=*t;                  // leggo il tipo di controller
  t=t++;
  
  switch (type)
  {
    case TCB_CONTROLLER:
           rt->p_Type=rt->p_Type | TCB_TRACK;
		   break;
	case BEZIER_CONTROLLER:
           rt->p_Type=rt->p_Type | BEZIER_TRACK;
		   break;
	case LINEAR_CONTROLLER:
           rt->p_Type=rt->p_Type | BEZIER_TRACK;
		   break;
  }

  // lettura delle opzioni per l'"out of range"
  ORT=*t; rt->p_ORTBefore=(int32)ORT; t++;
  ORT=*t; rt->p_ORTAfter=(int32)ORT; t++;

  v=(int32 *)t;
  nkey=*v;                  // leggo il numero di key
  v=v+1;
  rt->p_NumKeys=nkey;
  rt->p_Keys=new ROTKEY[rt->p_NumKeys];

  f=(float4 *)v;
  angle=0;
  for (nkey=0; nkey<rt->p_NumKeys; nkey++)
  {
	 v=(int32 *)f;
	 rt->p_Keys[nkey].posintrack=*v;
	 v=v+1;

	 f=(float4 *)v;
	 rt->p_Keys[nkey].rotquat.x=*f; f++;
	 rt->p_Keys[nkey].rotquat.y=*f; f++;
	 rt->p_Keys[nkey].rotquat.z=*f; f++;

     if (rt->p_Type & RELATIVE) angle = angle + (*f);
	 else angle=*f;
	 rt->p_Keys[nkey].rotquat.w=*f;
	 rt->p_Keys[nkey].roll=angle;  f++;
     quat_swap_3DS(&rt->p_Keys[nkey].rotquat);

	 if (type==TCB_CONTROLLER)
	 {
	    rt->p_Keys[nkey].tension=*f;     f++;
	    rt->p_Keys[nkey].continuity=*f;  f++;
	    rt->p_Keys[nkey].bias=*f;        f++;
	    rt->p_Keys[nkey].easeto=*f;      f++;
	    rt->p_Keys[nkey].easefrom=*f;    f++;
	 }
	 else
	 if (type==BEZIER_CONTROLLER)
	 {
	    rt->p_Keys[nkey].tension=0;
	    rt->p_Keys[nkey].continuity=0;
	    rt->p_Keys[nkey].bias=0;
	    rt->p_Keys[nkey].easeto=0;
	    rt->p_Keys[nkey].easefrom=0;
	 }
	 else
	 if (type==LINEAR_CONTROLLER)
	 {
	    rt->p_Keys[nkey].tension=1;
	    rt->p_Keys[nkey].continuity=0;
	    rt->p_Keys[nkey].bias=0;
	    rt->p_Keys[nkey].easeto=0;
	    rt->p_Keys[nkey].easefrom=0;
	 }
  }
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_RollTrack(CRollTrack *lt, char8 *chunk)
{
  int32 sz;
  uchar8 type, *t, ORT;
  int32 nkey, *v;
  float4 *f;

  sz=strlen(chunk+4);       // punto al nome e leggo la sua lunghezza
  chunk=chunk+4+(sz+1)+4;   // punto direttamente ai dati

  t=(uchar8 *)chunk;
  type=*t;              // leggo il tipo di controller
  t=t++;

  switch (type)
  {
    case TCB_CONTROLLER:
           lt->p_Type=lt->p_Type | TCB_TRACK;
		   break;
	case BEZIER_CONTROLLER:
           lt->p_Type=lt->p_Type | BEZIER_TRACK;
		   break;
	case LINEAR_CONTROLLER:
           lt->p_Type=lt->p_Type | BEZIER_TRACK;
		   break;
  }

  // lettura delle opzioni per l'"out of range"
  ORT=*t; lt->p_ORTBefore=(int32)ORT; t++;
  ORT=*t; lt->p_ORTAfter=(int32)ORT; t++;

  v=(int32 *)t;
  nkey=*v; v++;             // leggo il numero di key
  lt->p_NumKeys=nkey;
  lt->p_Keys=new ROLLKEY[lt->p_NumKeys];

  f=(float4 *)v;
  for (nkey=0; nkey<lt->p_NumKeys; nkey++)
  {
	 v=(int32 *)f;
	 lt->p_Keys[nkey].posintrack=*v; v++;
	 f=(float4 *)v;
	 lt->p_Keys[nkey].roll=*f; f++;

	 if (type==TCB_CONTROLLER)
	 {
	    lt->p_Keys[nkey].tension=*f;     f++;
	    lt->p_Keys[nkey].continuity=*f;  f++;
	    lt->p_Keys[nkey].bias=*f;        f++;
	    lt->p_Keys[nkey].easeto=*f;      f++;
	    lt->p_Keys[nkey].easefrom=*f;    f++;
	 }
	 else
	 if (type==BEZIER_CONTROLLER)
	 {
	    lt->p_Keys[nkey].incomtg=*f;   f++;
	    lt->p_Keys[nkey].outcomtg=*f;  f++;
	    lt->p_Keys[nkey].tension=0;
	    lt->p_Keys[nkey].continuity=0;
	    lt->p_Keys[nkey].bias=0;
	    lt->p_Keys[nkey].easeto=0;
	    lt->p_Keys[nkey].easefrom=0;
	 }
	 else
	 if (type==LINEAR_CONTROLLER)
	 {
	    lt->p_Keys[nkey].tension=1;
	    lt->p_Keys[nkey].continuity=0;
	    lt->p_Keys[nkey].bias=0;
	    lt->p_Keys[nkey].easeto=0;
	    lt->p_Keys[nkey].easefrom=0;
	 }
  }
}

// ###########################################################
// ######################  LOAD DI OSM  ######################
// ###########################################################
void CFileA3D::m_Load_TwistModifier_Settings(CTwistModifier *tm, char8 *chunk)
{
   int32 sz, *dati;
   float4 *f;

   if (!tm) debug_error(miawin, "Twist OSM load setting: modifier = null");

   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int32 *)(chunk+4+(sz+1)+4);

   f=(float4 *)dati;
   tm->p_Angle=(*f)*(float4)(Pi__/180.0); f++;
   tm->p_Bias=(*f)*(float4)(Pi__/180.0); f++;
   dati=(int32 *)f;
   tm->p_Axis=*dati; dati++;
   f=(float4 *)dati;
   tm->p_UpLim=*f; f++;
   tm->p_LowLim=*f; f++;
   dati=(int32 *)f;
   tm->p_DoLim=*dati; dati++;
   f=(float4 *)dati;
   tm->p_Center.x=*f; f++;
   tm->p_Center.y=*f; f++;
   tm->p_Center.z=*f; f++;
   vect_swap_3DS(&tm->p_Center);
   // NB: salto l'ordinale
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_RippleModifier_Settings(CRippleModifier *rm, char *chunk)
{
   int32 sz, *dati;
   float4 *f;

   if (!rm) debug_error(miawin, "Ripple OSM load setting: modifier = null");

   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int32 *)(chunk+4+(sz+1)+4);

   f=(float4 *)dati;
   rm->p_Amp1=*f; f++;
   rm->p_Amp2=*f; f++;
   rm->p_WLen=*f; f++;
   rm->p_Phase=*f; f++;
   rm->p_Decay=*f; f++;
   rm->p_Center.x=*f; f++;
   rm->p_Center.y=*f; f++;
   rm->p_Center.z=*f;
   vect_swap_3DS(&rm->p_Center);
   // NB: salto l'ordinale
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_BendModifier_Settings(CBendModifier *bm, char8 *chunk)
{
   int32 sz, *dati;
   float4 *f;
 
   if (!bm) debug_error(miawin, "Bend OSM load setting: modifier = null");

   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int32 *)(chunk+4+(sz+1)+4);

   f=(float4 *)dati;
   bm->p_Angle=(*f)*(float4)(Pi__/180.0); f++;
   bm->p_Dir=(*f)*(float4)(Pi__/180.0); f++;
   dati=(int32 *)f;
   bm->p_Axis=*dati; dati++;
   f=(float4 *)dati;
   bm->p_UpLim=*f; f++;
   bm->p_LowLim=*f; f++;
   dati=(int32 *)f;
   bm->p_DoLim=*dati; dati++;
   f=(float4 *)dati;
   bm->p_Center.x=*f; f++;
   bm->p_Center.y=*f; f++;
   bm->p_Center.z=*f; f++;
   vect_swap_3DS(&bm->p_Center);
   // NB: salto l'ordinale
  return;
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_MeltModifier_Settings(CMeltModifier *mm, char8 *chunk)
{
   int32 sz, *dati;
   float4 *f;
 
   if (!mm) debug_error(miawin, "Melt OSM load setting: modifier = null");

   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int32 *)(chunk+4+(sz+1)+4);

   f=(float4 *)dati;
   mm->p_Amount=*f; f++;
   mm->p_Spread=*f; f++;
   mm->p_Solidity=*f; f++;
   dati=(int32 *)f;
   mm->p_Axis=*dati; dati++;
   mm->p_NegAxis=*dati; dati++;
   f=(float4 *)dati;
   f++;  // salto confine
   f++;  // salto zbr
   mm->p_Center.x=*f; f++;
   mm->p_Center.y=*f; f++;
   mm->p_Center.z=*f; f++;
   vect_swap_3DS(&mm->p_Center);
   // NB: salto l'ordinale
  return;
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_TaperModifier_Settings(CTaperModifier *tm, char8 *chunk)
{
   int32 sz, *dati;
   float4 *f;
 
   if (!tm) debug_error(miawin, "Taper OSM load setting: modifier = null");
   // punto al nome dell'oggetto a cui l'OSM e' applicato e
   // vado direttamente ai dati
   sz=strlen(chunk+4);
   dati=(int32 *)(chunk+4+(sz+1)+4);

   f=(float4 *)dati;
   tm->p_Amount=*f; f++;
   tm->p_Curve=*f; f++;
   dati=(int32 *)f;
   tm->p_Axis=*dati; dati++;
   tm->p_EffectAxis=*dati; dati++;
   tm->p_Symmetry=*dati; dati++;
   f=(float4 *)dati;
   tm->p_UpLim=*f; f++;
   tm->p_LowLim=*f; f++;
   dati=(int32 *)f;
   tm->p_DoLim=*dati; dati++;
   f=(float4 *)dati;
   tm->p_Center.x=*f; f++;
   tm->p_Center.y=*f; f++;
   tm->p_Center.z=*f; f++;
   vect_swap_3DS(&tm->p_Center);
   // NB: salto l'ordinale
  return;
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_NoiseModifier_Settings(CNoiseModifier *nm, char8 *chunk)
{
  int32 sz, *dati;
  float4 *f;
 
  if (!nm) debug_error(miawin, "Noise OSM load setting: modifier = null");
  // punto al nome dell'oggetto a cui l'OSM e' applicato e
  // vado direttamente ai dati
  sz=strlen(chunk+4);
  dati=(int32 *)(chunk+4+(sz+1)+4);

  nm->p_Seed=*dati; dati++;
  f=(float4 *)dati;
  nm->p_Scale=*f; f++;
  dati=(int32 *)f;
  nm->p_Fractal=*dati; dati++;
  f=(float4 *)dati;
  nm->p_Rough=*f; f++;
  nm->p_Iterations=*f; f++;
  dati=(int32 *)f;
  nm->p_Animate=*dati; dati++;
  f=(float4 *)dati;
  nm->p_Freq=*f; f++;
  dati=(int32 *)f;
  nm->p_Phase=*dati; dati++;
  f=(float4 *)dati;
  nm->p_Strength.x=*f; f++;
  nm->p_Strength.y=*f; f++;
  nm->p_Strength.z=*f;
  vect_swap_3DS(&nm->p_Strength);
  // NB: salto l'ordinale
  return;
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_StretchModifier_Settings(CStretchModifier *sm, char8 *chunk)
{
  int32 sz, *dati;
  float4 *f;
 
  if (!sm) debug_error(miawin, "Stretch OSM load setting: modifier = null");
  // punto al nome dell'oggetto a cui l'OSM e' applicato e
  // vado direttamente ai dati
  sz=strlen(chunk+4);
  dati=(int32 *)(chunk+4+(sz+1)+4);

  f=(float4 *)dati;
  sm->p_Stretch=*f; f++;
  sm->p_Amplify=*f; f++;
  dati=(int32 *)f;
  sm->p_Axis=*dati; dati++;
  f=(float4 *)dati;
  sm->p_UpLim=*f; f++;
  sm->p_LowLim=*f; f++;
  dati=(int32 *)f;
  sm->p_DoLim=*dati; dati++;
  f=(float4 *)dati;
  sm->p_Center.x=*f; f++;
  sm->p_Center.y=*f; f++;
  sm->p_Center.z=*f; f++;
  vect_swap_3DS(&sm->p_Center);
   // NB: salto l'ordinale
  return;
}

//--------------------------------------------------------------------

void CFileA3D::m_Load_FFDModifier_Settings(CFFDModifier *fm, char8 *chunk)
{
  int k, sz, *dati;
  float4 *f;

  if (!fm) debug_error(miawin, "FFD OSM load setting: modifier = null");
  // punto al nome dell'oggetto a cui l'OSM e' applicato e
  // vado direttamente ai dati
  sz=strlen(chunk+4);
  dati=(int32 *)(chunk+4+(sz+1)+4);

  fm->p_Dim1=*dati; dati++;
  fm->p_Dim2=*dati; dati++;
  fm->p_Dim3=*dati; dati++;
  fm->p_Invol=*dati; dati++;
  fm->p_NumControlPoints=*dati; dati++;
  f=(float4 *)dati;
  fm->p_ControlPoints=new AD_Vect3D[fm->p_NumControlPoints];
  fm->p_ControlPointsTr=new AD_Vect3D[fm->p_NumControlPoints];
  fm->p_ControlPointsTracks=new CPosTrack[fm->p_NumControlPoints];
  for (k=0; k<fm->p_NumControlPoints; k++)
  {
	 fm->p_ControlPoints[k].x=*f; f++;
	 fm->p_ControlPoints[k].y=*f; f++;
	 fm->p_ControlPoints[k].z=*f; f++;
	 vect_swap_3DS(&fm->p_ControlPoints[k]);
  }
}

// ###########################################################
// ###########  LOAD LISTA MATERIALI (Link info)  ############
// ###########################################################

void CFileA3D::m_Load_Materials_List(char8 *chunk)
{
  int32 sz;
  char8 *c;
  int16 *dati;

  p_NumMaterials=0;  // settaggio di sicurezza

  sz=strlen(chunk+4);
  chunk=chunk+4+(sz+1)+4;

  dati=(int16 *)chunk;
  c=(((char8 *)dati)+sizeof(int16));
  while (*dati >= 0)
  {
    p_Materials[p_NumMaterials].global_id=(int32)(*dati);
	strcpy(p_Materials[p_NumMaterials].name, c);
	p_NumMaterials++;

    sz=strlen(c);
	c=c+(sz+1);
	dati=(int16 *)c;    // punto al nuovo id
	c=c+sizeof(int16);  // punto al nuovo nome
  }
}

// -------------------------------------------------------------------

char8 *CFileA3D::m_GetMaterialName(int32 g_id_mat)
{
  for (int32 w=0; w<p_NumMaterials; w++)
	if (g_id_mat==p_Materials[w].global_id) return(p_Materials[w].name);

  return(material_no_found);
}


// ###########################################################
// #####################  LOAD DI SCENE  #####################
// ###########################################################
void CFileA3D::m_LoadFromMemory(CScene3D *s, char8 *ptr)
{
  int32 current_ID, i, j;
  char8 *chunk_ptr, *name_owner, *chptr, mapIndex, ordinal;
  int32 finito=0, sz, *f;
  CMaterial *material;
  CGeometricObject *Gobj;
  CTwistModifier *twistmod;
  CRippleModifier *ripplemod;
  CBendModifier *bendmod;
  CMeltModifier *meltmod;
  CTaperModifier *tapermod;
  CNoiseModifier *noisemod;
  CStretchModifier *stretchmod;
  CFFDModifier *ffdmod;
  CParticleSystem *particleSystem;
  CWindModifier *wind;
  CGravityModifier *gravity;

  // inizializzazioni di base
  s->p_NumMeshes=0;
  s->p_NumObjects=0;
  s->p_NumCamere=0;
  s->p_NumOmniLights=0;
  s->p_NumSpotLights=0;

  chunk_ptr=ptr;
  current_ID=*((int32 *)chunk_ptr);

  while (!finito)
  {
	switch (current_ID)
	{
      case SCENE_GENERAL_INFO_ID:
           m_Load_GeneralInfo(chunk_ptr);
		   break;

      case SCENE_MATERIAL_LIST_ID:
		   m_Load_Materials_List(chunk_ptr);
		   break;

      case GEOMETRIC_OBJECT_ID:
		   s->p_Objects[s->p_NumObjects]=new CGeometricObject;
           // s->p_Objects[s->p_NumObjects]->p_type=TRIMESH;
		   m_Load_GeometricObject(s->p_Objects[s->p_NumObjects], chunk_ptr);
		   s->p_NumObjects++;
		   break;
/*
      case HELPER_OBJECT_ID:
		   s->p_Objects[s->p_NumObjects]=(CGeometricObject *)(new CHelper);
		   m_Load_Helper((CHelper *)(s->p_Objects[s->p_NumObjects]), chunk_ptr);
		   s->p_NumObjects++;
		   break;
*/
      case TRI_MESH_VERTEX_LIST_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_mesh(name_owner);
		   if (i<0)
		   {
		      s->p_Meshes[s->p_NumMeshes]=new CMesh;
			  strcpy(s->p_Meshes[s->p_NumMeshes]->p_Name, name_owner);
			  i=s->p_NumMeshes;
              s->p_NumMeshes++;
		   }
		   m_Load_Mesh_VertexList(s->p_Meshes[i], chunk_ptr);
		   break;

      case TRI_MESH_TEXTURE_VERTEX_LIST_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_mesh(name_owner);
		   if (i<0)
		   {
		      s->p_Meshes[s->p_NumMeshes]=new CMesh;
			  strcpy(s->p_Meshes[s->p_NumMeshes]->p_Name, name_owner);
			  i=s->p_NumMeshes;
              s->p_NumMeshes++;
		   }
		   m_Load_Mesh_TextureVertexList(s->p_Meshes[i], chunk_ptr);
		   break;

      case TRI_MESH_TEXTURE_FACES_LIST_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_mesh(name_owner);
		   if (i<0)
		   {
		      s->p_Meshes[s->p_NumMeshes]=new CMesh;
			  strcpy(s->p_Meshes[s->p_NumMeshes]->p_Name, name_owner);
			  i=s->p_NumMeshes;
              s->p_NumMeshes++;
		   }
		   m_Load_Mesh_TextureFaceList(s->p_Meshes[i], chunk_ptr);
		   break;

	  case TRI_MESH_FACES_LIST_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_mesh(name_owner);
		   if (i<0)
		   {
		      s->p_Meshes[s->p_NumMeshes]=new CMesh;
			  strcpy(s->p_Meshes[s->p_NumMeshes]->p_Name, name_owner);
			  i=s->p_NumMeshes;
              s->p_NumMeshes++;
		   }
		   m_Load_Mesh_FaceList(s->p_Meshes[i], chunk_ptr);
		   break;
     
	  case TARGETED_CAMERA_ID:
           m_Load_Camera_Settings(&s->p_Telecamere[s->p_NumCamere], chunk_ptr);
           s->p_NumCamere++;
		   break;

      case OMNI_LIGHT_ID:
		   m_Load_Omnilight_Settings(&s->p_OmniLights[s->p_NumOmniLights], chunk_ptr);
           s->p_NumOmniLights++;
           break;

      case SPOT_LIGHT_ID:
		   m_Load_Spotlight_Settings(&s->p_SpotLights[s->p_NumSpotLights], chunk_ptr);
           s->p_NumSpotLights++;
           break;

      case SPRAY_PARTICLE_SYSTEM_ID:
           particleSystem=new CParticleSystem;
           m_Load_ParticleSystem_Settings(particleSystem, chunk_ptr);
		   s->p_Objects[s->p_NumObjects]=particleSystem;
		   s->p_Objects[s->p_NumObjects]->p_Type=PARTICLE_SYSTEM_OBJECT;
		   s->p_NumObjects++;
		   break;

	  case WIND_MODIFIER_ID:
           wind=new CWindModifier;
		   s->p_Objects[s->p_NumObjects]=wind;
           m_Load_WindModifier_Settings(wind, chunk_ptr);
		   s->p_NumObjects++;
		   break;

	  case GRAVITY_MODIFIER_ID:
           gravity=new CGravityModifier;
		   s->p_Objects[s->p_NumObjects]=gravity;
           m_Load_GravityModifier_Settings(gravity, chunk_ptr);
		   s->p_NumObjects++;
		   break;

	  case TWIST_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer(name_owner);
		   if (Gobj)
		   {
             twistmod=new CTwistModifier;
             Gobj->p_OSMs[Gobj->p_NumOSMs]=(CObjectSpaceModifier *)twistmod;
	         m_Load_TwistModifier_Settings(twistmod, chunk_ptr);
             Gobj->p_NumOSMs++;
		   }
		   break;

	  case RIPPLE_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer(name_owner);
		   if (Gobj)
		   {
             ripplemod=new CRippleModifier;
             Gobj->p_OSMs[Gobj->p_NumOSMs]=(CObjectSpaceModifier *)ripplemod;
	         m_Load_RippleModifier_Settings(ripplemod, chunk_ptr);
             Gobj->p_NumOSMs++;
		   }
		   break;

	  case BEND_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer(name_owner);
		   if (Gobj)
		   {
             bendmod=new CBendModifier;
             Gobj->p_OSMs[Gobj->p_NumOSMs]=(CObjectSpaceModifier *)bendmod;
	         m_Load_BendModifier_Settings(bendmod, chunk_ptr);
             Gobj->p_NumOSMs++;
		   }
		   break;

	  case MELT_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer(name_owner);
		   if (Gobj)
		   {
             meltmod=new CMeltModifier;
             Gobj->p_OSMs[Gobj->p_NumOSMs]=(CObjectSpaceModifier *)meltmod;
	         m_Load_MeltModifier_Settings(meltmod, chunk_ptr);
             Gobj->p_NumOSMs++;
		   }
		   break;

	  case NOISE_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer(name_owner);
		   if (Gobj)
		   {
             noisemod=new CNoiseModifier;
             Gobj->p_OSMs[Gobj->p_NumOSMs]=(CObjectSpaceModifier *)noisemod;
	         m_Load_NoiseModifier_Settings(noisemod, chunk_ptr);
             Gobj->p_NumOSMs++;
		   }
		   break;

	  case TAPER_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer(name_owner);
		   if (Gobj)
		   {
             tapermod=new CTaperModifier;
             Gobj->p_OSMs[Gobj->p_NumOSMs]=(CObjectSpaceModifier *)tapermod;
	         m_Load_TaperModifier_Settings(tapermod, chunk_ptr);
             Gobj->p_NumOSMs++;
		   }
		   break;

	  case STRETCH_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer(name_owner);
		   if (Gobj)
		   {
             stretchmod=new CStretchModifier;
             Gobj->p_OSMs[Gobj->p_NumOSMs]=(CObjectSpaceModifier *)stretchmod;
	         m_Load_StretchModifier_Settings(stretchmod, chunk_ptr);
             Gobj->p_NumOSMs++;
		   }
		   break;

	  case FFD_MODIFIER_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer(name_owner);
		   if (Gobj)
		   {
             ffdmod=new CFFDModifier;
             Gobj->p_OSMs[Gobj->p_NumOSMs]=(CObjectSpaceModifier *)ffdmod;
	         m_Load_FFDModifier_Settings(ffdmod, chunk_ptr);
             Gobj->p_NumOSMs++;
		   }
		   break;

// ========================================================
// ###################   KEYFRAME  ########################
// ========================================================

      case HIDING_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              if (!Gobj->p_HideTrack) Gobj->p_HideTrack=new CHideTrack;
			  m_Load_HideTrack(Gobj->p_HideTrack, chunk_ptr);
		   }
		   break;

      case POSITION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              if (!Gobj->p_PosTrack) Gobj->p_PosTrack=new CPosTrack;
			  m_Load_PosTrack(Gobj->p_PosTrack, chunk_ptr);
		   }
		   break;

      case ROTATION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
             /*
             if (s->p_Objects[i]->p_type==BONE)
				 s->p_Objects[i]->rotationtrack.type=ABSOLUTE_TRACK;
			 else s->p_Objects[i]->rotationtrack.type=RELATIVE_TRACK;
			 */
              if (!Gobj->p_RotTrack) Gobj->p_RotTrack=new CRotTrack;
			  Gobj->p_RotTrack->p_Type=RELATIVE_TRACK;
			  m_Load_RotTrack(Gobj->p_RotTrack, chunk_ptr);
		   }
		   break;

      case SCALE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              if (!Gobj->p_ScaleTrack) Gobj->p_ScaleTrack=new CPosTrack;
			  m_Load_PosTrack(Gobj->p_ScaleTrack, chunk_ptr);
		   }
		   break;

      case CAMERA_TARGET_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_camera(name_owner);
		   if (i>=0)
		   {
			   if (!s->p_Telecamere[i].p_TargetTrack) s->p_Telecamere[i].p_TargetTrack=new CPosTrack;
			   m_Load_PosTrack(s->p_Telecamere[i].p_TargetTrack, chunk_ptr);
		   }
		   break;

      case CAMERA_FOV_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_camera(name_owner);
		   if (i>=0)
		   {
			   if (!s->p_Telecamere[i].p_FovTrack) s->p_Telecamere[i].p_FovTrack=new CRollTrack;
			   m_Load_RollTrack(s->p_Telecamere[i].p_FovTrack, chunk_ptr);
		   }
		   break;

      case CAMERA_ROLL_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_camera(name_owner);
		   if (i>=0)
		   {
             if (!s->p_Telecamere[i].p_RollTrack) s->p_Telecamere[i].p_RollTrack=new CRollTrack;
			 m_Load_RollTrack(s->p_Telecamere[i].p_RollTrack, chunk_ptr);
             for (j=0; j<s->p_Telecamere[i].p_RollTrack->p_NumKeys; j++)
				s->p_Telecamere[i].p_RollTrack->p_Keys[j].roll*=-1.0f;
		   }
		   break;

      case COLOR_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   i=s->index_of_omni(name_owner);
		   if (i>=0)
		   {
			   if (!s->p_OmniLights[i].p_ColorTrack) s->p_OmniLights[i].p_ColorTrack=new CPosTrack;
			   m_Load_PosTrack(s->p_OmniLights[i].p_ColorTrack, chunk_ptr);
		   }
		   break;

      case U_TILE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   material=s->get_material_pointer(name_owner);
		   if (material)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      mapIndex=*chptr;
			  if (mapIndex==1)
			  {
			     if (!material->p_U1TileTrack)
                   material->p_U1TileTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_U1TileTrack, chunk_ptr);
			  }
			  else
			  {
			     if (!material->p_U2TileTrack)
                   material->p_U2TileTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_U2TileTrack, chunk_ptr);
			  }
		   }
		   break;

      case V_TILE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   material=s->get_material_pointer(name_owner);
		   if (material)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      mapIndex=*chptr;
			  if (mapIndex==1)
			  {
			     if (!material->p_V1TileTrack)
                   material->p_V1TileTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_V1TileTrack, chunk_ptr);
			  }
			  else
			  {
			     if (!material->p_V2TileTrack)
                   material->p_V2TileTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_V2TileTrack, chunk_ptr);
			  }
		   }
		   break;

      case W_ANGLE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   material=s->get_material_pointer(name_owner);
		   if (material)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      mapIndex=*chptr;
			  if (mapIndex==1)
			  {
			     if (!material->p_W1AngleTrack)
                   material->p_W1AngleTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_W1AngleTrack, chunk_ptr);
			  }
			  else
			  {
			     if (!material->p_W2AngleTrack)
                   material->p_W2AngleTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_W2AngleTrack, chunk_ptr);
			  }
		   }
		   break;

      case U_OFFSET_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   material=s->get_material_pointer(name_owner);
		   if (material)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      mapIndex=*chptr;
			  if (mapIndex==1)
			  {
			     if (!material->p_U1OffsetTrack)
                   material->p_U1OffsetTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_U1OffsetTrack, chunk_ptr);
			  }
			  else
			  {
			     if (!material->p_U2OffsetTrack)
                   material->p_U2OffsetTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_U2OffsetTrack, chunk_ptr);
			  }
		   }
		   break;

      case V_OFFSET_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   material=s->get_material_pointer(name_owner);
		   if (material)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      mapIndex=*chptr;
			  if (mapIndex==1)
			  {
			     if (!material->p_V1OffsetTrack)
                   material->p_V1OffsetTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_V1OffsetTrack, chunk_ptr);
			  }
			  else
			  {
			     if (!material->p_V2OffsetTrack)
                   material->p_V2OffsetTrack=new CRollTrack;
				 m_Load_RollTrack(material->p_V2OffsetTrack, chunk_ptr);
			  }
		   }
		   break;

      case MATERIAL_OPACITY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   material=s->get_material_pointer(name_owner);
		   if (material)
		   {
              material->p_TransparencyTrack=new CRollTrack;
			  m_Load_RollTrack(material->p_TransparencyTrack, chunk_ptr);
		   }
		   break;

// ========================================================
// ###################   OSM Keyframe   ###################
// ========================================================
		   
	  case OSM_POSITION_TRACK_ID:
		   name_owner=chunk_ptr+4;
           Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  Gobj->p_OSMs[ordinal]->p_GizmoPositionTrack=new CPosTrack;
			  m_Load_PosTrack(Gobj->p_OSMs[ordinal]->p_GizmoPositionTrack, chunk_ptr);
		   }
		   break;

      case OSM_ROTATION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  Gobj->p_OSMs[ordinal]->p_GizmoRotationTrack=new CRotTrack;
			  m_Load_RotTrack(Gobj->p_OSMs[ordinal]->p_GizmoRotationTrack, chunk_ptr);
		   }
		   break;

      case OSM_SCALE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  Gobj->p_OSMs[ordinal]->p_GizmoScaleTrack=new CPosTrack;
			  m_Load_PosTrack(Gobj->p_OSMs[ordinal]->p_GizmoScaleTrack, chunk_ptr);
		   }
		   break;

	  case TWIST_ANGLE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  twistmod=(CTwistModifier *)Gobj->p_OSMs[ordinal];
			  twistmod->p_AngleTrack=new CRollTrack;
			  m_Load_RollTrack(twistmod->p_AngleTrack, chunk_ptr);
		   }
		   break;

	  case TWIST_BIAS_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  twistmod=(CTwistModifier *)Gobj->p_OSMs[ordinal];
			  twistmod->p_BiasTrack=new CRollTrack;
			  m_Load_RollTrack(twistmod->p_BiasTrack, chunk_ptr);
		   }
		   break;

	  case TWIST_UPLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  twistmod=(CTwistModifier *)Gobj->p_OSMs[ordinal];
			  twistmod->p_UpLimTrack=new CRollTrack;
			  m_Load_RollTrack(twistmod->p_UpLimTrack, chunk_ptr);
		   }
		   break;

	  case TWIST_LOWLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  twistmod=(CTwistModifier *)Gobj->p_OSMs[ordinal];
			  twistmod->p_LowLimTrack=new CRollTrack;
			  m_Load_RollTrack(twistmod->p_LowLimTrack, chunk_ptr);
		   }
		   break;

	  case RIPPLE_AMP1_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(CRippleModifier *)Gobj->p_OSMs[ordinal];
			  ripplemod->p_Amp1Track=new CRollTrack;
			  m_Load_RollTrack(ripplemod->p_Amp1Track, chunk_ptr);
		   }
		   break;

	  case RIPPLE_AMP2_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(CRippleModifier *)Gobj->p_OSMs[ordinal];
			  ripplemod->p_Amp2Track=new CRollTrack;
			  m_Load_RollTrack(ripplemod->p_Amp2Track, chunk_ptr);
		   }
		   break;

	  case RIPPLE_WAVELEN_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(CRippleModifier *)Gobj->p_OSMs[ordinal];
			  ripplemod->p_WLenTrack=new CRollTrack;
			  m_Load_RollTrack(ripplemod->p_WLenTrack, chunk_ptr);
		   }
		   break;

	  case RIPPLE_PHASE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(CRippleModifier *)Gobj->p_OSMs[ordinal];
			  ripplemod->p_PhaseTrack=new CRollTrack;
			  m_Load_RollTrack(ripplemod->p_PhaseTrack, chunk_ptr);
		   }
		   break;

	  case RIPPLE_DECAY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  ripplemod=(CRippleModifier *)Gobj->p_OSMs[ordinal];
			  ripplemod->p_DecayTrack=new CRollTrack;
			  m_Load_RollTrack(ripplemod->p_DecayTrack, chunk_ptr);
		   }
		   break;

	  case BEND_ANGLE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  bendmod=(CBendModifier *)Gobj->p_OSMs[ordinal];
			  bendmod->p_AngleTrack=new CRollTrack;
			  m_Load_RollTrack(bendmod->p_AngleTrack, chunk_ptr);
		   }
		   break;

	  case BEND_DIR_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  bendmod=(CBendModifier *)Gobj->p_OSMs[ordinal];
			  bendmod->p_DirTrack=new CRollTrack;
			  m_Load_RollTrack(bendmod->p_DirTrack, chunk_ptr);
		   }
		   break;

	  case BEND_UPLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  bendmod=(CBendModifier *)Gobj->p_OSMs[ordinal];
			  bendmod->p_UpLimTrack=new CRollTrack;
			  m_Load_RollTrack(bendmod->p_UpLimTrack, chunk_ptr);
		   }
		   break;

	  case BEND_LOWLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  bendmod=(CBendModifier *)Gobj->p_OSMs[ordinal];
			  bendmod->p_LowLimTrack=new CRollTrack;
			  m_Load_RollTrack(bendmod->p_LowLimTrack, chunk_ptr);
		   }
		   break;

	  case MELT_AMOUNT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  meltmod=(CMeltModifier *)Gobj->p_OSMs[ordinal];
			  meltmod->p_AmountTrack=new CRollTrack;
			  m_Load_RollTrack(meltmod->p_AmountTrack, chunk_ptr);
		   }
		   break;

	  case MELT_SPREAD_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  meltmod=(CMeltModifier *)Gobj->p_OSMs[ordinal];
			  meltmod->p_SpreadTrack=new CRollTrack;
			  m_Load_RollTrack(meltmod->p_SpreadTrack, chunk_ptr);
		   }
		   break;

	  case STRETCH_STRETCH_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  stretchmod=(CStretchModifier *)Gobj->p_OSMs[ordinal];
			  stretchmod->p_StretchTrack=new CRollTrack;
			  m_Load_RollTrack(stretchmod->p_StretchTrack, chunk_ptr);
		   }
		   break;

	  case STRETCH_AMPLIFY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  stretchmod=(CStretchModifier *)Gobj->p_OSMs[ordinal];
			  stretchmod->p_AmplifyTrack=new CRollTrack;
			  m_Load_RollTrack(stretchmod->p_AmplifyTrack, chunk_ptr);
		   }
		   break;

	  case STRETCH_UPLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  stretchmod=(CStretchModifier *)Gobj->p_OSMs[ordinal];
			  stretchmod->p_UpLimTrack=new CRollTrack;
			  m_Load_RollTrack(stretchmod->p_UpLimTrack, chunk_ptr);
		   }
		   break;

	  case STRETCH_LOWLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  stretchmod=(CStretchModifier *)Gobj->p_OSMs[ordinal];
			  stretchmod->p_LowLimTrack=new CRollTrack;
			  m_Load_RollTrack(stretchmod->p_LowLimTrack, chunk_ptr);
		   }
		   break;

	  case TAPER_AMOUNT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  tapermod=(CTaperModifier *)Gobj->p_OSMs[ordinal];
			  tapermod->p_AmountTrack=new CRollTrack;
			  m_Load_RollTrack(tapermod->p_AmountTrack, chunk_ptr);
		   }
		   break;

	  case TAPER_CURVE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  tapermod=(CTaperModifier *)Gobj->p_OSMs[ordinal];
			  tapermod->p_CurveTrack=new CRollTrack;
			  m_Load_RollTrack(tapermod->p_CurveTrack, chunk_ptr);
		   }
		   break;

	  case TAPER_UPLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  tapermod=(CTaperModifier *)Gobj->p_OSMs[ordinal];
			  tapermod->p_UpLimTrack=new CRollTrack;
			  m_Load_RollTrack(tapermod->p_UpLimTrack, chunk_ptr);
		   }
		   break;

	  case TAPER_LOWLIMIT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  tapermod=(CTaperModifier *)Gobj->p_OSMs[ordinal];
			  tapermod->p_LowLimTrack=new CRollTrack;
			  m_Load_RollTrack(tapermod->p_LowLimTrack, chunk_ptr);
		   }
		   break;
 
      case FFD_CONTROLPOINT_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-5);
		      ordinal=*chptr; chptr++; // quale modificatore
			  f=(int32 *)chptr;
			  j=*f; // quale control point
			  ffdmod=(CFFDModifier *)Gobj->p_OSMs[ordinal];
			  m_Load_PosTrack(&ffdmod->p_ControlPointsTracks[j], chunk_ptr);
		   }
		   break;
      
	  case NOISE_STRENGTH_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  noisemod->p_StrengthTrack=new CPosTrack;
			  noisemod=(CNoiseModifier *)Gobj->p_OSMs[ordinal];
			  m_Load_PosTrack(noisemod->p_StrengthTrack, chunk_ptr);
		   }
		   break;

      case NOISE_FREQ_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  noisemod=(CNoiseModifier *)Gobj->p_OSMs[ordinal];
			  noisemod->p_FreqTrack=new CRollTrack;
			  m_Load_RollTrack(noisemod->p_FreqTrack, chunk_ptr);
		   }
		   break;

      case NOISE_PHASE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  noisemod=(CNoiseModifier *)Gobj->p_OSMs[ordinal];
			  noisemod->p_PhaseTrack=new CRollTrack;
			  m_Load_RollTrack(noisemod->p_PhaseTrack, chunk_ptr);
		   }
		   break;

      case NOISE_SCALE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
			  sz=strlen(name_owner);
			  chptr=(char8 *)(chunk_ptr+4+sz+1);
			  f=(int32 *)chptr;
			  sz=*f;  // campo size del chunk
			  chptr=(char8 *)f;
			  chptr=(char8 *)((chptr+4)+sz-1);
		      ordinal=*chptr;
			  noisemod=(CNoiseModifier *)Gobj->p_OSMs[ordinal];
			  noisemod->p_ScaleTrack=new CRollTrack;
			  m_Load_RollTrack(noisemod->p_ScaleTrack, chunk_ptr);
		   }
		   break;

      case PARTICLE_EMITTER_WIDTH_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              particleSystem=(CParticleSystem *)Gobj;
              if (!particleSystem->p_EmitterWidthTrack)
                 particleSystem->p_EmitterWidthTrack=new CRollTrack;
              m_Load_RollTrack(particleSystem->p_EmitterWidthTrack, chunk_ptr);

		   }
           break;

      case PARTICLE_EMITTER_LENGTH_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              particleSystem=(CParticleSystem *)Gobj;
              if (!particleSystem->p_EmitterHeightTrack)
                 particleSystem->p_EmitterHeightTrack=new CRollTrack;
              m_Load_RollTrack(particleSystem->p_EmitterHeightTrack, chunk_ptr);
		   }
           break;

      case PARTICLE_EMITTER_SPEED_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              particleSystem=(CParticleSystem *)Gobj;
              if (!particleSystem->p_EmitterSpeedTrack)
                 particleSystem->p_EmitterSpeedTrack=new CRollTrack;
              m_Load_RollTrack(particleSystem->p_EmitterSpeedTrack, chunk_ptr);
		   }
           break;

      case PARTICLE_EMITTER_VARIATION_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              particleSystem=(CParticleSystem *)Gobj;
              if (!particleSystem->p_EmitterVariationTrack)
                 particleSystem->p_EmitterVariationTrack=new CRollTrack;
              m_Load_RollTrack(particleSystem->p_EmitterVariationTrack, chunk_ptr);
		   }
           break;

      case WIND_STRENGTH_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              wind=(CWindModifier *)Gobj;
              if (!wind->p_StrengthTrack)
                 wind->p_StrengthTrack=new CRollTrack;
              m_Load_RollTrack(wind->p_StrengthTrack, chunk_ptr);
		   }
           break;

      case WIND_DECAY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              wind=(CWindModifier *)Gobj;
              if (!wind->p_DecayTrack)
                 wind->p_DecayTrack=new CRollTrack;
              m_Load_RollTrack(wind->p_DecayTrack, chunk_ptr);
		   }
           break;

      case WIND_TURBOLENCE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              wind=(CWindModifier *)Gobj;
              if (!wind->p_TurbolenceTrack)
                 wind->p_TurbolenceTrack=new CRollTrack;
              m_Load_RollTrack(wind->p_TurbolenceTrack, chunk_ptr);
		   }
           break;

      case WIND_FREQUENCY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              wind=(CWindModifier *)Gobj;
              if (!wind->p_FrequencyTrack)
                 wind->p_FrequencyTrack=new CRollTrack;
              m_Load_RollTrack(wind->p_FrequencyTrack, chunk_ptr);
		   }
           break;

      case WIND_SCALE_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              wind=(CWindModifier *)Gobj;
              if (!wind->p_ScaleTrack)
                 wind->p_ScaleTrack=new CRollTrack;
              m_Load_RollTrack(wind->p_ScaleTrack, chunk_ptr);
		   }
           break;

	  case GRAVITY_STRENGTH_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              gravity=(CGravityModifier *)Gobj;
              if (!gravity->p_StrengthTrack)
                 gravity->p_StrengthTrack=new CRollTrack;
              m_Load_RollTrack(gravity->p_StrengthTrack, chunk_ptr);
		   }
           break;

      case GRAVITY_DECAY_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   Gobj=s->get_object_pointer (name_owner);
		   if (Gobj)
		   {
              gravity=(CGravityModifier *)Gobj;
              if (!gravity->p_DecayTrack)
                 gravity->p_DecayTrack=new CRollTrack;
              m_Load_RollTrack(gravity->p_DecayTrack, chunk_ptr);
		   }
           break;

      case END_SCENE_ID:
		   name_owner=chunk_ptr+4;
		   sz=strlen(name_owner)+1;
		   f=(int32 *)(name_owner+sz+4);
		   finito=*f;
		   if (finito) goto fine;
		   break;
	}
    chunk_ptr=m_GetNextChunk(chunk_ptr);
    current_ID=*((int32 *)chunk_ptr);
  }
fine:
;
}

//--------------------------------------------------------------------

void CFileA3D::m_LoadFromFile(CScene3D *s, char8 *filename)
{
  char8 *dumpA3D=m_AllocA3D(filename);

  p_NumMaterials=0;
  p_MyScene=s;
  m_LoadFromMemory(s, dumpA3D);

  delete [] dumpA3D;
  p_MyScene=(CScene3D *)NULL;
}

//--------------------------------------------------------------------

void CFileA3D::m_SaveToMemory(char8 *ptr,uint32 *size)
{
	char8 *mem=new char[MAXA3DFILESIZE]; 
	int32 pos=0;
}

//--------------------------------------------------------------------

void CFileA3D::m_SaveToFile(char8 *filename_owner)
{
  FILE *bfile;
  uint32 lung,i;
  char8 *fileA3D=NULL;

  m_SaveToMemory(fileA3D, &lung);
  if ((bfile=fopen(filename_owner, "wb"))==(FILE *)0x0) return;
  for (i=0; i<lung; i++) fwrite(&fileA3D[i], 1, 1, bfile);
  fclose(bfile);
  delete [] fileA3D;
}
