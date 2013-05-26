#include "BoundVolumes.h"
#include "Mesh.h"

CBoundVolume::CBoundVolume()
{
  p_Root=(BoundNode *)NULL;
  p_IsVertexBufferCreated=0;
  p_NumBoundNodes=0;
  p_VertexBuffer=(IDirect3DVertexBuffer8 *)NULL;
  p_EnclosedVolume=0;
}

//--------------------------------------------------------------------

int32 CBoundVolume::m_BoundTriangles(BoundNode **node,
                                     int32 currentDepth,
						             int32 maxDepth,
									 AD_Vect3D *vertex,
									 AD_Tria3D *triangles,
	                                 int32 *indexes,
									 int32 start, int32 end)
{
   AD_Vect3D planeNormal;
   int32 k, j, w, pivot;
   int32 longestAxisIndex;
   float4 dist, centroidDist;
   BoundNode *l_node;

   if ((currentDepth >= maxDepth) || (end<start)) return(0);
   if (!(*node))
   {
	   l_node=new BoundNode;
       l_node->leftChild=(BoundNode *)NULL;
       l_node->rightChild=(BoundNode *)NULL;
       l_node->start=start;
       l_node->end=end;
       l_node->hit=0;
       switch (p_primitivesType)
	   {
         case SPHERE_PRIMITIVE: l_node->primitive=new CSpherePrimitive;
		                        break;
         case AABB_PRIMITIVE: l_node->primitive=new CAABBPrimitive;
		                      break;
         case OBB_PRIMITIVE: l_node->primitive=new COBBPrimitive;
                             break;
	   }
	   *node=l_node;
       p_NumBoundNodes++;
   }

   (*node)->primitive->m_Bound(vertex, triangles, indexes, start, end);
   // ho boundato un singolo triangolo
   if (end==start) return(0);


   longestAxisIndex=0;
partitioning:
;
   // 2) calcolo del piano di split che passa dal centroide
   // va calcolata la normale al piano! per ora scelgo uno dei tre
   // assi principali
   (*node)->primitive->m_GetLongestAxis(longestAxisIndex, &planeNormal);

   vect_auto_normalize(&planeNormal);
   centroidDist=vect_dot(&((*node)->primitive->p_Center), &planeNormal);

   // 3) calcolo dei triangoli che stanno da una parte e dall'altra
   //    del piano di split e ordinamento degli indici
   //j=start;
   j=start-1;
   w=end;
   for (k=start; k<=end; k++)
   {
      dist=vect_dot(&triangles[indexes[k]].midpoint, &planeNormal);
      if (dist<=centroidDist)
	  {
         j++;
         p_indexesAux[j]=indexes[k];
	  }
	  else
      if (dist>centroidDist)
	  {
         p_indexesAux[w--]=indexes[k];
	  }
   }
   // 'flag' segna se l'ultimo indice ad essere modificato è stato
   // j oppure w
   pivot=j;

   for (k=start; k<=end; k++) indexes[k]=p_indexesAux[k];
   // il piano di splitting non separa triangoli
   if ((pivot==end) || (j==start-1))
   {
        longestAxisIndex++;
		if (longestAxisIndex>2) return(0);
	    else goto partitioning;
   }
   else
   {
        m_BoundTriangles(&((*node)->leftChild), currentDepth+1, maxDepth,
                         vertex, triangles, indexes, start, pivot);
        m_BoundTriangles(&((*node)->rightChild), currentDepth+1, maxDepth,
                         vertex, triangles, indexes, pivot+1, end);
   }
   return(1);
}

//--------------------------------------------------------------------

void CBoundVolume::m_CountLeaves (BoundNode *node)
{
  if (!node) return;

  if ((!node->leftChild) && (!node->rightChild)) p_NumLeaves++;
  else
  {
     m_CountLeaves(node->leftChild);
	 m_CountLeaves(node->rightChild);
  }
}

//--------------------------------------------------------------------

int32 CBoundVolume::m_Bound(void *mesh, int32 maxDepth, int32 primitiveType)
{
  int32 i, *indexes;
  CMesh *l_mesh;

  // controlli di sicurezza
  l_mesh=(CMesh *)mesh;
  if ((!l_mesh) || (maxDepth<=0)) return(0);

  p_primitivesType=primitiveType;
  p_indexesAux=new int32[l_mesh->p_NumTriangles];
  indexes=new int32[l_mesh->p_NumTriangles];

  for (i=0; i<l_mesh->p_NumTriangles; i++) p_indexesAux[i]=indexes[i]=i;

  p_NumBoundNodes=0;
  m_BoundTriangles(&p_Root, 0, maxDepth,
	               l_mesh->p_ObjspaceVertex,
                   l_mesh->p_Triangles,
				   indexes,
				   0, l_mesh->p_NumTriangles-1);

  // contiamo il numero di foglie generato dal bounding
  p_NumLeaves=0;
  m_CountLeaves(p_Root);
  p_Leaves=new BoundNode_Ptr[p_NumLeaves];
  p_NumLeaves=0;
  m_CollectNodes(p_Root);

  //m_TXTDump("bound.txt");

  delete [] indexes;
  delete [] p_indexesAux;
  return(1);
}

//--------------------------------------------------------------------

void CBoundVolume::m_UpdateTree(BoundNode *node, AD_Matrix *obj2world, BoundNode *out)
{
  if ((!node) || (!obj2world) || (!out))
    return;

  node->primitive->m_Update(obj2world, out->primitive);
  m_UpdateTree(node->leftChild, obj2world, out->leftChild);
  m_UpdateTree(node->rightChild, obj2world, out->rightChild);
}

//--------------------------------------------------------------------

void CBoundVolume::m_Update(AD_Matrix *obj2world, CBoundVolume *out)
{
  if ((!obj2world) || (!out))
    return;
  m_UpdateTree(p_Root, obj2world, out->p_Root);
}

//--------------------------------------------------------------------

void CBoundVolume::m_FillVertexBuffer(D3DBoundVertex *buffer)
{
  int32 i, j;
  int32 l_FillIndex;

  l_FillIndex=0;
  for (j=0; j<p_NumLeaves; j++)
  {
    for (i=0; i<p_Leaves[j]->primitive->p_NumPaintVertex; i++)
	{
      vect_copy(&p_Leaves[j]->primitive->p_PaintVertex[i], &buffer[l_FillIndex].point);
	  if (!p_Leaves[j]->hit)
		buffer[l_FillIndex].ARGB=0xFFFFFFFF;
	  else buffer[l_FillIndex].ARGB=0xFFFF0000;
      l_FillIndex++;
	}
  }
  p_NumPaintPrimitives=l_FillIndex/2;
}

//--------------------------------------------------------------------

void CBoundVolume::m_Paint (CRenderLib *RL)
{
  HRESULT hr;
  void *l_VertexData;
  D3DBoundVertex *l_BoundVertex;
  int32 np;
  DWORD alphaState;

  if (!RL) return;
  if (!p_IsVertexBufferCreated)
  {
     if (p_primitivesType==SPHERE_PRIMITIVE) np=6;
	 else if (p_primitivesType==AABB_PRIMITIVE) np=24;
	 else if (p_primitivesType==OBB_PRIMITIVE) np=24;
     hr=RL->p_Device->CreateVertexBuffer(np*p_NumLeaves*sizeof(D3DBoundVertex),
		                                 D3DUSAGE_WRITEONLY,
										 FVFBoundVertex,
                                         D3DPOOL_DEFAULT, &p_VertexBuffer);
     p_IsVertexBufferCreated=1;
  }

  hr=p_VertexBuffer->Lock(0, 0, (BYTE **)&l_VertexData, 0);
  l_BoundVertex=(D3DBoundVertex *)l_VertexData;
  m_FillVertexBuffer(l_BoundVertex);

  // uso il vertex buffer riempito
  hr=p_VertexBuffer->Unlock();
  hr=RL->p_Device->SetStreamSource(0, p_VertexBuffer, sizeof(D3DBoundVertex));

  hr=RL->p_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
  hr=RL->p_Device->GetRenderState(D3DRS_ALPHABLENDENABLE, &alphaState);
  hr=RL->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

  // nessuna texture; prendiamo il colore della linea nel campo diffuso
  hr=RL->p_Device->SetTexture(0, NULL);
  hr=RL->p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  hr=RL->p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

  hr=RL->p_Device->SetTexture(1, NULL);
  hr=RL->p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  hr=RL->p_Device->SetTexture(2, NULL);
  hr=RL->p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);

  // disegnamo le linee
  hr=RL->p_Device->SetVertexShader(FVFBoundVertex);
  //float4 PointSize=4;
  //hr=RL->p_Device->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&PointSize));
  hr=RL->p_Device->DrawPrimitive(D3DPT_LINELIST, 0, p_NumPaintPrimitives);
  hr=RL->p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaState);
}

//--------------------------------------------------------------------

void CBoundVolume::m_DumpNode(FILE *fd, BoundNode *node)
{
  if (!node) return;

  fprintf(fd, "Center=%f,  %f,  %f     ", node->primitive->p_Center.x,
	      node->primitive->p_Center.y, node->primitive->p_Center.z);
  fprintf(fd, "Start=%d     End=%d", node->start, node->end);
  if ((!node->leftChild) && (!node->rightChild))
     fprintf(fd, "   (foglia)");

  fprintf(fd, "\n");
  m_DumpNode(fd, node->leftChild);
  m_DumpNode(fd, node->rightChild);
}

//--------------------------------------------------------------------

void CBoundVolume::m_TXTDump(char8 *filename)
{
   FILE *f;

   f=fopen(filename, "wt");
   fprintf(f, "Numero nodi dell'albero: %d\n", p_NumBoundNodes);
   fprintf(f, "Numero foglie dell'albero: %d\n", p_NumLeaves);
   m_DumpNode(f, p_Root);
   fclose(f);
}

//--------------------------------------------------------------------

float4 CBoundVolume::m_GetEnclosedVolume(void)
{
  int32 k;

  p_EnclosedVolume=0;
  for (k=0; k<p_NumLeaves; k++)
     p_EnclosedVolume+=p_Leaves[k]->primitive->m_GetEnclosedVolume();
  return(p_EnclosedVolume);
}

//--------------------------------------------------------------------

void CBoundVolume::m_CollectNodes (BoundNode *node)
{
  if (!node) return;

  if ((!node->leftChild) && (!node->rightChild))
  {
	  p_Leaves[p_NumLeaves++]=node;
  }
  else
  {
     m_CollectNodes(node->leftChild);
	 m_CollectNodes(node->rightChild);
  }
}

//--------------------------------------------------------------------

int32 CBoundVolume::m_IntersectRayNode(BoundNode *node, Ray *ray)
{
   int32 hit, hitL, hitR;

   hit=node->primitive->m_IntersectRay(ray);
   if (hit)
   {
      node->hit=1;
	  if ((node->rightChild) && (node->leftChild))
	  {
	     hitL=m_IntersectRayNode(node->leftChild, ray);
		 if (!hitL)
		 {
		   hitR=m_IntersectRayNode(node->rightChild, ray);
		   if (!hitR) return(0);
		   else return(1);
		 }
		 else return(1);
	  }
	  else return(1);  // sono a una foglia
   }
   else return(0);
}

//--------------------------------------------------------------------

int32 CBoundVolume::m_IntersectRay (Ray *ray)
{
  if (m_IntersectRayNode(p_Root, ray))
	return(1);
  else return(0);
}

//----------------------------------------------------------------------

int32 CBoundVolume::m_IntersectVolume (CBoundVolume *volume)
{
  return(0);
}

//----------------------------------------------------------------------

void CBoundVolume::m_CopyNode (BoundNode *node, int32 copyType, BoundNode **out)
{
  BoundNode *l_out;
  CSpherePrimitive *l_sphere1, *l_sphere2;
  CAABBPrimitive *l_aabb1, *l_aabb2;
  COBBPrimitive *l_obb1, *l_obb2;

  if (copyType==CLONE)
  {
    if (!(*out))
    {
       l_out=new BoundNode;
       l_out->leftChild=(BoundNode *)NULL;
       l_out->rightChild=(BoundNode *)NULL;
       switch (p_primitivesType)
       {
         case SPHERE_PRIMITIVE: l_out->primitive=new CSpherePrimitive;
                                l_sphere1=(CSpherePrimitive *)node->primitive;
                                l_sphere2=(CSpherePrimitive *)l_out->primitive;
                                *(l_sphere2)=*(l_sphere1);
                                break;
         case AABB_PRIMITIVE: l_out->primitive=new CAABBPrimitive;
                              l_aabb1=(CAABBPrimitive *)node->primitive;
                              l_aabb2=(CAABBPrimitive *)l_out->primitive;
                              *(l_aabb2)=*(l_aabb1);
                              break;
         case OBB_PRIMITIVE: l_out->primitive=new COBBPrimitive;
                             l_obb1=(COBBPrimitive *)node->primitive;
                             l_obb2=(COBBPrimitive *)l_out->primitive;
                             *(l_obb2)=*(l_obb1);
                             break;
       }
    }
    l_out->start=node->start;
    l_out->end=node->end;
    l_out->hit=0;
    *out=l_out;
    if (node->leftChild)
      m_CopyNode(node->leftChild, copyType, &((*out)->leftChild));
    if (node->rightChild)
      m_CopyNode(node->rightChild, copyType, &((*out)->rightChild));
  }
}

//----------------------------------------------------------------------

int32 CBoundVolume::m_Copy (int32 copyType, CBoundVolume *dest)
{
  if (!dest) return(0);

  dest->p_primitivesType=p_primitivesType;
  dest->p_NumPaintPrimitives=p_NumPaintPrimitives;
  dest->p_IsVertexBufferCreated=p_IsVertexBufferCreated;
  dest->p_VertexBuffer=p_VertexBuffer;
  dest->p_EnclosedVolume=p_EnclosedVolume;

  dest->p_NumLeaves=p_NumLeaves;
  dest->p_NumBoundNodes=p_NumBoundNodes;
  if (copyType==HISTANCE)
  {
    dest->p_Root=p_Root;
    dest->p_Leaves=p_Leaves;
    dest->p_Nodes=p_Nodes;
  }
  else
    m_CopyNode(p_Root, copyType, &dest->p_Root);
  return(1);
}
