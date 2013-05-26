#include "Utility.h"
#include "Lights.h"

extern FILE *fdeb;
extern HWND miawin;  // finestra principale; usata qui per le debug_error

CGeometricObject::CGeometricObject()
{
   strcpy(p_Name, "");
   strcpy(p_FatherName, "");
   p_Father=(CGeometricObject *)NULL;
   p_HasChildrens=0;

   p_BaseMaterial=(CMaterial *)NULL;
   for (int32 i=0; i<MAX_LODS; i++)
   {
	  p_Lods[i].Mesh=(CMesh *)NULL;
	  p_Lods[i].DistMin=0;
	  p_Lods[i].DistMax=10000.0f;
      p_Lods[i].VBStart=-1;
      p_Lods[i].VBLong=-1;
      p_Lods[i].IBStart=-1;
      p_Lods[i].IBLong=-1;
   }
   p_NumLods=0;
   p_VertexBuffer=(IDirect3DVertexBuffer8 *)NULL;
   p_StaticVertex=1;  // non sono soggetti a modificatori

   p_HideTrack=(CHideTrack *)NULL;
   p_PosTrack=(CPosTrack *)NULL;
   p_RotTrack=(CRotTrack *)NULL;
   p_ScaleTrack=(CPosTrack *)NULL;

   p_Visible=1;
   vect_set(&p_Pivot, 0, 0, 0);
   vect_set(&p_CurrentPosition, 0, 0, 0);
   quat_set(&p_CurrentRotationQuaternion, 0, 0, 0, 0);
   mat_identity(&p_CurrentRotationMatrix);
   vect_set(&p_CurrentScale, 1, 1, 1);
   vect_set(&p_TotalScale, 1, 1, 1);
   mat_identity(&p_WorldMatrix);
   mat_identity(&p_InverseWorldMatrix);

   p_SPHEREBoundVolume=(CBoundVolume *)NULL;
   p_AABBBoundVolume=(CBoundVolume *)NULL;
   p_OBBBoundVolume=(CBoundVolume *)NULL;

   for (i=0; i<MAX_OSM_MODIFIERS; i++)
      p_OSMs[i]=(CObjectSpaceModifier *)NULL;
   p_NumOSMs=0;
}

//--------------------------------------------------------------------

int32 CGeometricObject::m_Clone(CGeometricObject *source)
{
   if (!source) return(0);

   strcpy(p_FatherName, source->p_FatherName);
   
   p_Father=source->p_Father;
   p_HasChildrens=source->p_HasChildrens;

   p_BaseMaterial=source->p_BaseMaterial;
   for (int32 i=0; i<MAX_LODS; i++)
	  p_Lods[i]=source->p_Lods[i];
   p_NumLods=source->p_NumLods;

   p_VertexBuffer=source->p_VertexBuffer;
   p_StaticVertex=source->p_StaticVertex;

   p_PosTrack=source->p_PosTrack;
   p_RotTrack=source->p_RotTrack;
   p_ScaleTrack=source->p_ScaleTrack;

   vect_copy(&source->p_Pivot, &p_Pivot);
   vect_copy(&source->p_CurrentPosition, &p_CurrentPosition);
   quat_copy(&source->p_CurrentRotationQuaternion, &p_CurrentRotationQuaternion);
   mat_copy(&source->p_CurrentRotationMatrix, &p_CurrentRotationMatrix);
   vect_copy(&source->p_CurrentScale, &p_CurrentScale);
   vect_copy(&source->p_TotalScale, &p_TotalScale);
   mat_copy(&source->p_WorldMatrix, &p_WorldMatrix);

   if (source->p_SPHEREBoundVolume)
   {
      p_SPHEREBoundVolume=new CBoundVolume;
      source->p_SPHEREBoundVolume->m_Copy(CLONE, p_SPHEREBoundVolume);
   }
   if (source->p_AABBBoundVolume)
   {
      p_AABBBoundVolume=new CBoundVolume;
      source->p_AABBBoundVolume->m_Copy(CLONE, p_AABBBoundVolume);
   }
   if (source->p_OBBBoundVolume)
   {
      p_OBBBoundVolume=new CBoundVolume;
      source->p_OBBBoundVolume->m_Copy(CLONE, p_OBBBoundVolume);
   }
   return(1);
}

//--------------------------------------------------------------------

int32 CGeometricObject::m_CopyVertex(int32 baseIndex, void *dest,
									 CMesh *mesh)
{
   FVFOnlyGeometry *l_dest;
   int32 i;

//   if (p_ScaleTrack)
//	 p_ScaleTrack->m_GetData(0, &p_CurrentScale);

   l_dest=(FVFOnlyGeometry *)dest;
   for (i=0; i<mesh->p_NumDriverVertex; i++)
   {
	  //vect_copy(&mesh->p_DriverVertex[i].point, &l_dest[i+baseIndex].point);
      vect_sub(&mesh->p_DriverVertex[i].point, &p_Pivot, &l_dest[i+baseIndex].point);
	  vect_copy(&mesh->p_DriverVertex[i].normal, &l_dest[i+baseIndex].normal);
/*
	  l_dest[i+baseIndex].normal.x*=p_CurrentScale.x;
      l_dest[i+baseIndex].normal.y*=p_CurrentScale.y;
	  l_dest[i+baseIndex].normal.z*=p_CurrentScale.z;
*/
   }
   return (mesh->p_NumDriverVertex);
}

//--------------------------------------------------------------------

int32 CGeometricObject::m_CopyTEX1Vertex(int32 baseIndex, void *dest,
										 CMesh *mesh)
{
   FVFGeometryUV1 *l_dest;
   int32 i;

   l_dest=(FVFGeometryUV1 *)dest;
   /*if (!p_BaseMaterial->m_MapNeedUVTransform(1))
     for (i=0; i<mesh->p_NumDriverVertex; i++)
	 {
	    vect_copy(&mesh->p_DriverVertex[i].point, &l_dest[i+baseIndex].point);
	    vect_copy(&mesh->p_DriverVertex[i].normal, &l_dest[i+baseIndex].normal);
        l_dest[i+baseIndex].uv1.u=mesh->p_DriverVertex[i].uv1.u;
        l_dest[i+baseIndex].uv1.v=-mesh->p_DriverVertex[i].uv1.v;
	 }
   else*/
     for (i=0; i<mesh->p_NumDriverVertex; i++)
	 {
	    //vect_copy(&mesh->p_DriverVertex[i].point, &l_dest[i+baseIndex].point);
        vect_sub(&mesh->p_DriverVertex[i].point, &p_Pivot, &l_dest[i+baseIndex].point);
	    vect_copy(&mesh->p_DriverVertex[i].normal, &l_dest[i+baseIndex].normal);
        l_dest[i+baseIndex].uv1.u=mesh->p_DriverVertex[i].uv1.u;
        l_dest[i+baseIndex].uv1.v=mesh->p_DriverVertex[i].uv1.v;
	 }
   return (mesh->p_NumDriverVertex);
}

//--------------------------------------------------------------------

int32 CGeometricObject::m_CopyTEX2Vertex(int32 baseIndex, void *dest,
										 CMesh *mesh)
{
   FVFGeometryUV2 *l_dest;
   int32 i;

   l_dest=(FVFGeometryUV2 *)dest;

   for (i=0; i<mesh->p_NumDriverVertex; i++)
   {
	  //vect_copy(&mesh->p_DriverVertex[i].point, &l_dest[i+baseIndex].point);
      vect_sub(&mesh->p_DriverVertex[i].point, &p_Pivot, &l_dest[i+baseIndex].point);
	  vect_copy(&mesh->p_DriverVertex[i].normal, &l_dest[i+baseIndex].normal);
      l_dest[i+baseIndex].uv1.u=mesh->p_DriverVertex[i].uv1.u;
      l_dest[i+baseIndex].uv1.v=mesh->p_DriverVertex[i].uv1.v;
      l_dest[i+baseIndex].uv2.u=mesh->p_DriverVertex[i].uv2.u;
      l_dest[i+baseIndex].uv2.v=mesh->p_DriverVertex[i].uv2.v;
   }
   return (mesh->p_NumDriverVertex);
}

//--------------------------------------------------------------------

int32 CGeometricObject::m_CopyTEX1DOT3Vertex(int32 baseIndex, void *dest, CMesh *mesh)
{
   VSGeometryUV1DOT3 *l_dest;
   int32 i;

   l_dest=(VSGeometryUV1DOT3 *)dest;

   for (i=0; i<mesh->p_NumDriverVertex; i++)
   {
	 //vect_copy(&mesh->p_DriverVertex[i].point, &l_dest[i+baseIndex].point);
     vect_sub(&mesh->p_DriverVertex[i].point, &p_Pivot, &l_dest[i+baseIndex].point);
	 vect_copy(&mesh->p_DriverVertex[i].normal, &l_dest[i+baseIndex].normal);
     l_dest[i+baseIndex].uv1.u=mesh->p_DriverVertex[i].uv1.u;
     l_dest[i+baseIndex].uv1.v=mesh->p_DriverVertex[i].uv1.v;
     vect_copy(&mesh->p_DriverVertex[i].bumpspace.T, &l_dest[i+baseIndex].T);
   }
   return (mesh->p_NumDriverVertex);
}

//--------------------------------------------------------------------

int32 CGeometricObject::m_CopyTEX2DOT3Vertex(int32 baseIndex, void *dest, CMesh *mesh)
{
   VSGeometryUV2DOT3 *l_dest;
   int32 i;

   l_dest=(VSGeometryUV2DOT3 *)dest;

   for (i=0; i<mesh->p_NumDriverVertex; i++)
   {
	  //vect_copy(&mesh->p_DriverVertex[i].point, &l_dest[i+baseIndex].point);
      vect_sub(&mesh->p_DriverVertex[i].point, &p_Pivot, &l_dest[i+baseIndex].point);
	  vect_copy(&mesh->p_DriverVertex[i].normal, &l_dest[i+baseIndex].normal);
      l_dest[i+baseIndex].uv1.u=mesh->p_DriverVertex[i].uv1.u;
      l_dest[i+baseIndex].uv1.v=mesh->p_DriverVertex[i].uv1.v;
      l_dest[i+baseIndex].uv2.u=mesh->p_DriverVertex[i].uv2.u;
      l_dest[i+baseIndex].uv2.v=mesh->p_DriverVertex[i].uv2.v;
      vect_copy(&mesh->p_DriverVertex[i].bumpspace.T, &l_dest[i+baseIndex].T);
   }
   return (mesh->p_NumDriverVertex);
}

//--------------------------------------------------------------------

int32 CGeometricObject::m_Init(void *arg1)
{
  int32 l_NumTotVert, i, VBStart, VBLong;
  CRenderLib *RL;
  HRESULT hr;
  char8 msg[150], chInt[3];
  void *l_VertexData;
  CObjectSpaceModifier *OSMaux[20];

  strcpy(msg, "CGeometricObject m_Init error: ");
  if (p_Name)
  {
	  strcat(msg, p_Name);
  }
  strcat(msg, " ");

  // inizializzo le eventuali tracce del keyframer
  if (p_PosTrack)
    if (!p_PosTrack->m_Init(1)) return(0);
  if (p_RotTrack)
	if (!p_RotTrack->m_Init(1)) return(0);
  if (p_ScaleTrack)
	if (!p_ScaleTrack->m_Init(1)) return(0);
  if (p_NumLods <= 0) return(1);

  // inizializzo le eventuali mesh associate
  l_NumTotVert=0;
  for (i=0; i<p_NumLods; i++)
  {
    l_NumTotVert+=p_Lods[i].Mesh->p_NumDriverVertex;
  }

  if (l_NumTotVert <= 0) return(1);
  RL=(CRenderLib *)arg1; if (!RL) return(0);

  // copio i volumi consistenti (profondita' > 0) del LOD 0
  if (p_Lods[0].Mesh->p_SPHEREVolumeTreeDepth > 0)
  {
    p_SPHEREBoundVolume=new CBoundVolume;
    p_Lods[0].Mesh->p_SPHEREBoundVolume.m_Copy(CLONE, p_SPHEREBoundVolume);
  }
  if (p_Lods[0].Mesh->p_AABBVolumeTreeDepth > 0)
  {
    p_AABBBoundVolume=new CBoundVolume;
    p_Lods[0].Mesh->p_AABBBoundVolume.m_Copy(CLONE, p_AABBBoundVolume);
  }
  if (p_Lods[0].Mesh->p_OBBVolumeTreeDepth > 0)
  {
    p_OBBBoundVolume=new CBoundVolume;
    p_Lods[0].Mesh->p_OBBBoundVolume.m_Copy(CLONE, p_OBBBoundVolume);
  }

  sprintf(msg, "%s cloning bounding volumes done", p_Name);
  DumpDebugPoint (fdeb, msg, 0);


  if (p_NumOSMs>0)
  {
    // inverto l'ordine degli OSM perche' vanno applicati
    // al contrario
    for (i=0; i<p_NumOSMs; i++) OSMaux[i]=p_OSMs[i];
    for (i=0; i<p_NumOSMs; i++) p_OSMs[p_NumOSMs-i-1]=OSMaux[i];

    // Inizializzazione OSM modifiers
	float4 x1, y1, z1, x2, y2, z2;
	p_Lods[0].Mesh->m_GetExtension(&x1, &y1, &z1, &x2, &y2, &z2);
    for (i=0; i<p_NumOSMs; i++)
	{
	  p_OSMs[i]->m_Init();
	  p_OSMs[i]->m_SetBBox(x1-p_Pivot.x, y1-p_Pivot.y, z1-p_Pivot.z,
		                   x2-p_Pivot.x, y2-p_Pivot.y, z2-p_Pivot.z);
	}
  }
  sprintf(msg, "%s object space modifiers initialization done", p_Name);
  DumpDebugPoint (fdeb, msg, 0);

  // se ha almeno una mesh allora il materiale non deve essere nullo
  if (!p_BaseMaterial)
  {
	 strcat(msg, "has a Mesh but p_BaseMaterial NULL!");
	 debug_error(miawin, msg);
  }
  // controlliamo che se il materiale necessita di due canali di
  // mapping UV espliciti la mesh li abbia
  else
  {
     if ((p_BaseMaterial->p_InputVertexFormat.enumType==FVF_GEOMETRY_UV2) ||
		 (p_BaseMaterial->p_InputVertexFormat.enumType==VS_GEOMETRY_UV2_DOT3) ||
		 (p_BaseMaterial->p_InputVertexFormat.enumType==FVF_GEOMETRY_DIFFUSE_UV2))
	 {
		for (i=0; i<p_NumLods; i++)
		{
	       if (!(p_Lods[i].Mesh->p_NumUV1vertex>0 &&
			     p_Lods[i].Mesh->p_NumUV2vertex>0))
		   {
	         strcat(msg, "wants 2 mapping channel but lod(mesh)");
             itoa(i, chInt, 10);
             strcat(msg, chInt);
		     strcat(msg, " hasn't they!");
	         debug_error(miawin, msg);
		   }
		}
	 }
  }


  // creiamo il vertex buffer
//  if (p_StaticVertex)
  if (RL->p_VSHardwareSupport)
  {
	if (p_NumOSMs<=0)
      hr=RL->p_Device->CreateVertexBuffer(l_NumTotVert*p_BaseMaterial->p_InputVertexFormat.vertexSize,
                                          D3DUSAGE_WRITEONLY,
									      p_BaseMaterial->p_InputVertexFormat.FVF,
                                          D3DPOOL_DEFAULT,
									      &p_VertexBuffer);
	else
      hr=RL->p_Device->CreateVertexBuffer(l_NumTotVert*p_BaseMaterial->p_InputVertexFormat.vertexSize,
                                          D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
									      p_BaseMaterial->p_InputVertexFormat.FVF,
                                          D3DPOOL_DEFAULT,
									      &p_VertexBuffer);
  }
  else
  {
	int32 flag;
	if (p_NumOSMs<=0) flag=0;
	else flag=D3DUSAGE_DYNAMIC;

	if (p_BaseMaterial->p_InputVertexFormat.FVF==0)
      hr=RL->p_Device->CreateVertexBuffer(l_NumTotVert*p_BaseMaterial->p_InputVertexFormat.vertexSize,
                                          D3DUSAGE_WRITEONLY | D3DUSAGE_SOFTWAREPROCESSING | flag,
									      p_BaseMaterial->p_InputVertexFormat.FVF,
                                          D3DPOOL_DEFAULT,
       									      &p_VertexBuffer);
       	else
      hr=RL->p_Device->CreateVertexBuffer(l_NumTotVert*p_BaseMaterial->p_InputVertexFormat.vertexSize,
                                          D3DUSAGE_WRITEONLY | flag,
									      p_BaseMaterial->p_InputVertexFormat.FVF,
                                          D3DPOOL_DEFAULT,
									      &p_VertexBuffer);
  }
  sprintf(msg, "%s vertex buffer creation done", p_Name);
  DumpDebugPoint (fdeb, msg, 0);
  hr=p_VertexBuffer->Lock(0, 0, (BYTE **)&l_VertexData, 0);

  VBStart=0;
  // copiamo i lod in successione
  for (i=0; i<p_NumLods; i++)
  {
    switch (p_BaseMaterial->p_InputVertexFormat.enumType)
	{
	  case FVF_ONLY_GEOMETRY:
        VBLong=m_CopyVertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	  case FVF_GEOMETRY_DIFFUSE:
        VBLong=m_CopyVertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	  case FVF_GEOMETRY_UV1:
        VBLong=m_CopyTEX1Vertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	  case FVF_GEOMETRY_DIFFUSE_UV1:
        VBLong=m_CopyTEX1Vertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	  case FVF_GEOMETRY_UV2:
        VBLong=m_CopyTEX2Vertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	  case FVF_GEOMETRY_DIFFUSE_UV2:
        VBLong=m_CopyTEX2Vertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	  case VS_GEOMETRY_UV1_DOT3:
        VBLong=m_CopyTEX1DOT3Vertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	  case VS_GEOMETRY_UV2_DOT3:
        VBLong=m_CopyTEX2DOT3Vertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	  default:
        VBLong=m_CopyVertex(VBStart, l_VertexData, p_Lods[i].Mesh);
		break;
	}
	p_Lods[i].VBStart=VBStart;
	p_Lods[i].VBLong=VBLong;
	VBStart+=VBLong;
  }
  hr=p_VertexBuffer->Unlock();
  sprintf(msg, "%s vertex buffer has been filled", p_Name);
  DumpDebugPoint (fdeb, msg, 0);
  if (hr != D3D_OK) return(0);
  else return(1);
}

//--------------------------------------------------------------------

int32 CGeometricObject::m_IntersectRay (Ray *ray)
{
  int32 hit1=0, hit2=0, hit3=0;

  if (p_SPHEREBoundVolume)
  {
    hit1=p_SPHEREBoundVolume->m_IntersectRay(ray);
    if (!hit1) return(0);
  }
  if (p_AABBBoundVolume)
  {
    hit2=p_AABBBoundVolume->m_IntersectRay(ray);
    if (!hit2) return(0);
  }
  if (p_OBBBoundVolume)
  {
    hit3=p_OBBBoundVolume->m_IntersectRay(ray);
    if (!hit3) return(0);
  }
  return(hit1 | hit2 | hit3);
}

//--------------------------------------------------------------------

void CGeometricObject::m_BuildWorldMatrix(void)
{
   AD_Vect3D postmp;
   AD_Matrix posttrans, scaling, maux;
   AD_Matrix iRot, iScale, iTrans;

   // matrice di rotazione e sua inversa
   quat_rotquat_to_matrix(&p_CurrentRotationQuaternion, &p_CurrentRotationMatrix);
   mat_copy(&p_CurrentRotationMatrix, &p_WorldMatrix);
   mat_transpose(&p_CurrentRotationMatrix, &iRot);
   
   // matrice di scaling e sua inversa
   mat_setmatrix_of_scaling(&scaling, p_CurrentScale.x, p_CurrentScale.y, p_CurrentScale.z);
   mat_setmatrix_of_scaling(&iScale, 1.0f/p_CurrentScale.x, 1.0f/p_CurrentScale.y, 1.0f/p_CurrentScale.z);

   // matrice di traslazione e sua inversa
   mat_setmatrix_of_pretraslation(&posttrans, &p_CurrentPosition);
   vect_neg(&p_CurrentPosition, &postmp);
   mat_setmatrix_of_pretraslation(&iTrans, &postmp);

   // prima pivot, poi scaling, poi rotazione, e infine traslazione
   //vect_neg(&p_Pivot, &postmp);
   //mat_setmatrix_of_pretraslation(&pivot, &postmp);
   //mat_mul(&scaling, &pivot, &maux);
   //mat_mul(&p_CurrentRotationMatrix, &maux, &maux);
   mat_mul(&p_CurrentRotationMatrix, &scaling, &maux);
   mat_mul(&posttrans, &maux, &p_WorldMatrix);

   // per la inversa: prima traslazione, rotazione, scaling e pivot
   //mat_setmatrix_of_pretraslation(&iPivot, &p_Pivot);
   mat_mul(&iRot, &iTrans, &maux);
   mat_mul(&iScale, &maux, &p_InverseWorldMatrix);
   //mat_mul(&iScale, &maux, &maux);
   //mat_mul(&iPivot, &maux, &p_InverseWorldMatrix);

   if (p_Father)
   {
     mat_mulaffine(&p_Father->p_CurrentRotationMatrix,
                   &p_CurrentRotationMatrix,
                   &p_CurrentRotationMatrix);
     mat_mul(&p_Father->p_WorldMatrix, &p_WorldMatrix, &p_WorldMatrix);
     mat_mul(&p_InverseWorldMatrix, &p_Father->p_InverseWorldMatrix, &p_InverseWorldMatrix);
     p_TotalScale.x*=p_Father->p_TotalScale.x;
     p_TotalScale.y*=p_Father->p_TotalScale.y;
     p_TotalScale.z*=p_Father->p_TotalScale.z;
   }
}

//--------------------------------------------------------------------

void CGeometricObject::m_Update(float4 frame)
{
   AD_Vect3D postmp;
   int32 i;

   if (p_HideTrack)
     p_HideTrack->m_GetData(frame, &p_Visible);

   if (p_RotTrack)
     p_RotTrack->m_GetData(frame, &p_CurrentRotationQuaternion);

   vect_set(&p_TotalScale, 1, 1, 1);
   if (p_ScaleTrack)
   {
      p_ScaleTrack->m_GetData(frame, &p_CurrentScale);
      p_TotalScale.x*=p_CurrentScale.x;
      p_TotalScale.y*=p_CurrentScale.y;
      p_TotalScale.z*=p_CurrentScale.z;
   }

   if (p_PosTrack)
       p_PosTrack->m_GetData(frame, &p_CurrentPosition);

   m_BuildWorldMatrix();

   if (p_Father)
   {
     mat_mulvect(&p_Father->p_WorldMatrix, &p_CurrentPosition, &postmp);
     vect_copy(&postmp, &p_CurrentPosition);
   }

   // update degli OSMs
   for (i=0; i<p_NumOSMs; i++) p_OSMs[i]->m_Update(frame, &p_Pivot);

   // update e del volume gerarchico + fine di tutti
   if (p_SPHEREBoundVolume)
        p_Lods[0].Mesh->p_SPHEREBoundVolume.m_Update(&p_WorldMatrix, p_SPHEREBoundVolume);
   if (p_AABBBoundVolume)
        p_Lods[0].Mesh->p_AABBBoundVolume.m_Update(&p_WorldMatrix, p_AABBBoundVolume);
   if (p_OBBBoundVolume)
        p_Lods[0].Mesh->p_OBBBoundVolume.m_Update(&p_WorldMatrix, p_OBBBoundVolume);
}

//--------------------------------------------------------------------

void CGeometricObject::m_DoOSMs(int32 witchLod)
{
  int32 i, j, k;
  AD_Vect3D *point_tr, pp;
  DriverVertex *l_DriverVertex;
  HRESULT hr;
  uint32 start_lock, end_lock, vsize;
  BYTE *l_Data;

  k=p_Lods[witchLod].Mesh->p_NumDriverVertex;
  l_DriverVertex=p_Lods[witchLod].Mesh->p_DriverVertex;

  vsize=p_BaseMaterial->p_InputVertexFormat.vertexSize;
  start_lock=p_Lods[witchLod].VBStart*vsize;
  end_lock=p_Lods[witchLod].VBLong*vsize;

locka:
;
  hr=p_VertexBuffer->Lock(start_lock, end_lock,
                          (BYTE **)&l_Data, D3DLOCK_NOOVERWRITE );// 0);
  if (hr!=D3D_OK) goto locka;

  for (i=0; i<k; i++)
  {
	point_tr=(AD_Vect3D *)l_Data;
	//vect_copy(&l_DriverVertex[i].point, &pp);
    vect_sub(&l_DriverVertex[i].point, &p_Pivot, &pp);
	for (j=0; j<p_NumOSMs; j++)
	{
      p_OSMs[j]->m_Map(&pp, &pp);
	}
	vect_copy(&pp, point_tr);
	l_Data+=vsize;
  }

  hr=p_VertexBuffer->Unlock();
}

//--------------------------------------------------------------------

void CGeometricObject::m_Paint(void *arg1, void *arg2, void *arg3)
{
   CRenderLib *RL;
   int32 l_NumLights, i, k, light_index;
   CLight *l_Lights;
   HRESULT hr;

   // se l'oggetto non è visibile allora non lo disegno
   if (!p_Visible) return;
   
   if (p_NumLods>0)
   {
      RL=(CRenderLib *)arg1;
	  if (!RL) return;

	  // abilitazione delle luci: si puo' attuare la politica di
	  // attivare solo le 2/3 piu' vicine
	  if (p_BaseMaterial->p_LightMixType != MIXTYPE_NONE)
	  {
        l_Lights=(CLight *)arg2;
	    l_NumLights=*((int32 *)arg3);
        if (l_Lights)
		{
          light_index=0;
	      for (i=0; i<l_NumLights; i++)
		  {
			 mat_mulvect(&p_InverseWorldMatrix,
				         &l_Lights[i].p_CurrentPosition,
                         (AD_Vect3D *)&l_Lights[i].p_D3DLight.Direction);
             RL->p_Device->SetLight(i, &l_Lights[i].p_D3DLight);
			 hr=RL->p_Device->LightEnable(i, TRUE);
		  }
		}
	  }

	  RL->m_SetWorldMatrix(&p_WorldMatrix);
	  hr=RL->p_Device->SetStreamSource(0, p_VertexBuffer,
		                               p_BaseMaterial->p_InputVertexFormat.vertexSize);
	  
	  // eseuo gli OSM
	  if (p_NumOSMs>0)
	  {
		 m_DoOSMs(0);
	  }

	  // TO DO: selezionare il LOD
	  k=RL->m_GetMaterialPasses(p_BaseMaterial);
	  for (i=0; i<k; i++)
	  {
	     if (RL->m_SetMaterial(p_BaseMaterial, i))
	       hr=RL->p_Device->DrawPrimitive(D3DPT_TRIANGLELIST,
                                          p_Lods[0].VBStart,
                                          p_Lods[0].VBLong/3);
	  }
/*
  int32 ss, aa, oo;
	  ss=p_Lods[0].Mesh->p_SPHEREVolumeTreeDepth;
      oo=p_Lods[0].Mesh->p_OBBVolumeTreeDepth;
	  aa=p_Lods[0].Mesh->p_AABBVolumeTreeDepth;
	  if ((ss+aa+oo)==0) return;
 	  if ((ss > aa) && (ss > oo))
         p_Lods[0].Mesh->p_SPHEREBoundVolume.m_Paint(RL);
	  else
      if (oo >= aa)
         p_Lods[0].Mesh->p_OBBBoundVolume.m_Paint(RL);
	  else
         p_Lods[0].Mesh->p_AABBBoundVolume.m_Paint(RL);
*/
   }
}