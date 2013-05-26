#include "UtilsA3D.h"

int32 CFileA3D::m_Load_GeometricObject(char8 *objectname, char8 *A3Dfilename, CGeometricObject *obj)
{
/*  char8 *dumpA3D=NULL;
  
  dumpA3D=m_AllocA3D(A3Dfilename);
  if (dumpA3D) delete [] dumpA3D;*/
  return(1);
}

//--------------------------------------------------------------------

int32 CFileA3D::m_Load_Mesh(char8 *meshname, char8 *A3Dfilename, CMesh *mesh)
{
  char8 *dumpA3D=NULL;
  int32 current_ID;
  char8 *chunk_ptr, *name_owner;
  int32 finito, sz, *f;
  CGeometricObject Gobj;

  if ((!mesh) || (!A3Dfilename)) return(0);
  dumpA3D=m_AllocA3D(A3Dfilename);
  chunk_ptr=dumpA3D;
  current_ID=*((int32 *)chunk_ptr);

  finito=0;
  while (!finito)
  {
	switch (current_ID)
	{
      case TRI_MESH_VERTEX_LIST_ID:
		   name_owner=chunk_ptr+4;
		   if (strcmp(name_owner, meshname)==0)
		     m_Load_Mesh_VertexList(mesh, chunk_ptr);
		   break;

      case TRI_MESH_TEXTURE_VERTEX_LIST_ID:
		   name_owner=chunk_ptr+4;
		   if (strcmp(name_owner, meshname)==0)
		     m_Load_Mesh_TextureVertexList(mesh, chunk_ptr);
		   break;

      case TRI_MESH_TEXTURE_FACES_LIST_ID:
		   name_owner=chunk_ptr+4;
		   if (strcmp(name_owner, meshname)==0)
			 m_Load_Mesh_TextureFaceList(mesh, chunk_ptr);
		   break;

	  case TRI_MESH_FACES_LIST_ID:
		   name_owner=chunk_ptr+4;
		   if (strcmp(name_owner, meshname)==0)
			 m_Load_Mesh_FaceList(mesh, chunk_ptr);
		   break;
     
      case END_SCENE_ID:
		   name_owner=chunk_ptr+4;
		   sz=strlen(name_owner)+1;
		   f=(int32 *)(name_owner+sz+4);
		   finito=*f;
		   break;
	}

    chunk_ptr=m_GetNextChunk(chunk_ptr);
    current_ID=*((int32 *)chunk_ptr);
  }

  if (dumpA3D) delete [] dumpA3D;
  return(1);
}

//--------------------------------------------------------------------

int32 CFileA3D::m_Load_HideTrack(char8 *objectname, char8 *A3Dfilename, CHideTrack *ht)
{
  char8 *dumpA3D=NULL;
  int32 current_ID;
  char8 *chunk_ptr, *name_owner;
  int32 finito, sz, *f, found;

  if ((!objectname) || (!A3Dfilename)) return(0);

  dumpA3D=m_AllocA3D(A3Dfilename);
  chunk_ptr=dumpA3D;
  current_ID=*((int32 *)chunk_ptr);

  found=0;
  finito=0;
  while (!finito)
  {
	switch (current_ID)
	{
      case HIDING_TRACK_ID:
		   name_owner=chunk_ptr+4;
		   if (strcmp(name_owner, objectname)==0)
		   {
              if (!ht) ht=new CHideTrack;
	          m_Load_HideTrack(ht, chunk_ptr);
              found=1;
		   }
		   break;

      case END_SCENE_ID:
		   name_owner=chunk_ptr+4;
		   sz=strlen(name_owner)+1;
		   f=(int32 *)(name_owner+sz+4);
		   finito=*f;
		   break;
	}
    chunk_ptr=m_GetNextChunk(chunk_ptr);
    current_ID=*((int32 *)chunk_ptr);
  }
  if (dumpA3D) delete [] dumpA3D;
  return(found);
}

//--------------------------------------------------------------------

int32 CFileA3D::m_Load_PosTrack(int32 IDTrack, char8 *objectname, char8 *A3Dfilename, CPosTrack *pt)
{
  char8 *dumpA3D=NULL;
  int32 current_ID;
  char8 *chunk_ptr, *name_owner;
  int32 finito, sz, *f, found;

  if ((!objectname) || (!A3Dfilename)) return(0);

  dumpA3D=NULL;
  dumpA3D=m_AllocA3D(A3Dfilename);
  if (!dumpA3D) return(0);
  chunk_ptr=dumpA3D;
  current_ID=*((int32 *)chunk_ptr);

  found=0;
  finito=0;
  while (!finito)
  {
	if (current_ID==IDTrack)
	{
	   name_owner=chunk_ptr+4;
	   if (strcmp(name_owner, objectname)==0)
	   {
          if (!pt) pt=new CPosTrack;
	      m_Load_PosTrack(pt, chunk_ptr);
          found=1;
	   }
	}
	else
    if (current_ID==END_SCENE_ID)
	{
       name_owner=chunk_ptr+4;
       sz=strlen(name_owner)+1;
       f=(int32 *)(name_owner+sz+4);
       finito=*f;
       break;
	}
    chunk_ptr=m_GetNextChunk(chunk_ptr);
    current_ID=*((int32 *)chunk_ptr);
  }
  if (dumpA3D) delete [] dumpA3D;
  return(found);
}

//--------------------------------------------------------------------

int32 CFileA3D::m_Load_RotTrack(int32 IDTrack, char8 *objectname, char8 *A3Dfilename, CRotTrack *rt)
{
  char8 *dumpA3D=NULL;
  int32 current_ID;
  char8 *chunk_ptr, *name_owner;
  int32 finito, sz, *f, found;

  if ((!objectname) || (!A3Dfilename)) return(0);

  dumpA3D=NULL;
  dumpA3D=m_AllocA3D(A3Dfilename);
  if (!dumpA3D) return(0);
  chunk_ptr=dumpA3D;
  current_ID=*((int32 *)chunk_ptr);

  found=0;
  finito=0;
  while (!finito)
  {
	if (current_ID==IDTrack)
	{
	   name_owner=chunk_ptr+4;
	   if (strcmp(name_owner, objectname)==0)
	   {
          if (!rt) rt=new CRotTrack;
	      m_Load_RotTrack(rt, chunk_ptr);
          found=1;
	   }
	}
    else
    if (current_ID==END_SCENE_ID)
	{
	   name_owner=chunk_ptr+4;
	   sz=strlen(name_owner)+1;
	   f=(int32 *)(name_owner+sz+4);
	   finito=*f;
	   if (finito) goto fine;
	}
    chunk_ptr=m_GetNextChunk(chunk_ptr);
    current_ID=*((int32 *)chunk_ptr);
  }
fine:
;
  if (dumpA3D) delete [] dumpA3D;
  return(found);
}

//--------------------------------------------------------------------

int32 CFileA3D::m_Load_RollTrack(int32 IDTrack, char8 *objectname, char8 *A3Dfilename, CRollTrack *lt)
{
  char8 *dumpA3D=NULL;
  int32 current_ID;
  char8 *chunk_ptr, *name_owner;
  int32 finito, sz, *f, found;

  if ((!objectname) || (!A3Dfilename)) return(0);

  dumpA3D=m_AllocA3D(A3Dfilename);
  chunk_ptr=dumpA3D;
  current_ID=*((int32 *)chunk_ptr);

  found=0;
  finito=0;
  while (!finito)
  {
	if (current_ID==IDTrack)
	{
	   name_owner=chunk_ptr+4;
	   if (strcmp(name_owner, objectname)==0)
	   {
          if (!lt) lt=new CRollTrack;
	      m_Load_RollTrack(lt, chunk_ptr);
          found=1;
	   }
	}
    else
	if (current_ID==END_SCENE_ID)
	{
	   name_owner=chunk_ptr+4;
	   sz=strlen(name_owner)+1;
	   f=(int32 *)(name_owner+sz+4);
	   finito=*f;
	}
    chunk_ptr=m_GetNextChunk(chunk_ptr);
    current_ID=*((int32 *)chunk_ptr);
  }
  if (dumpA3D) delete [] dumpA3D;
  return(found);
}

//--------------------------------------------------------------------
