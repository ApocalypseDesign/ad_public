#ifndef _BOUNDVOLUMES_H_
#define _BOUNDVOLUMES_H_

#include "BoundPrimitives.h"

typedef struct
{
  AD_Vect3D point;
  uint32 ARGB;
} D3DBoundVertex;
#define FVFBoundVertex D3DFVF_XYZ | D3DFVF_DIFFUSE


struct BoundNode
{
   CBoundPrimitive *primitive;
   BoundNode *leftChild, *rightChild;
   int32 start, end;
   int32 hit;
};
typedef BoundNode *BoundNode_Ptr;

// modalita' di copia
#define HISTANCE 0
#define CLONE    1

class CBoundVolume
{
private:
  int32 m_BoundTriangles(BoundNode **node,
                         int32 currentDepth,
						 int32 maxDepth,
                         AD_Vect3D *vertex, AD_Tria3D *triangles,
	                     int32 *indexes, int32 start, int32 end);
  void m_CountLeaves (BoundNode *node);
  void m_CollectNodes (BoundNode *node);
  void m_UpdateTree(BoundNode *node, AD_Matrix *obj2world, BoundNode *out);
  void m_DumpNode (FILE *fd, BoundNode *node);
  int32 m_IntersectRayNode(BoundNode *node, Ray *ray);
  void m_CopyNode (BoundNode *node, int32 copyType, BoundNode **out);
  void m_FillVertexBuffer(D3DBoundVertex *buffer);

  int32 *p_indexesAux;
  int32 p_primitivesType;
  int32 p_NumPaintPrimitives;
  int32 p_IsVertexBufferCreated;
  IDirect3DVertexBuffer8 *p_VertexBuffer;
  float4 p_EnclosedVolume;

public:
  BoundNode *p_Root;

  BoundNode **p_Leaves;
  int32 p_NumLeaves;

  BoundNode **p_Nodes;
  int32 p_NumBoundNodes;

  CBoundVolume();
  int32 m_Bound(void *mesh, int32 maxDepth, int32 primitiveType);
  int32 m_IntersectRay (Ray *ray);
  int32 m_IntersectVolume (CBoundVolume *volume);
  void m_TXTDump (char8 *filename);
  void m_Update(AD_Matrix *obj2world, CBoundVolume *out);
  void m_Paint (CRenderLib *RL);
  float4 m_GetEnclosedVolume(void);
  int32 m_Copy (int32 copyType, CBoundVolume *dest);
};

#endif