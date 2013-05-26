#include "SceneExport.h"

// Tool di ottimizzazione per le mesh: rimozione di vertici geometrici
// duplicati
void SkeletonExporter::build_valid_vertex (Mesh *mesh, Point3 *vv,
										   int *hash, int *num_vv)
{
   int i, j , k;
   Point3 vert;
   BOOL found;

   i=0;
   for (j=0; j<mesh->getNumVerts(); j++)
   {
	  vert=mesh->verts[j];
	  found=FALSE;
	  k=0;
	  while ((found==FALSE) && (k<i))
	  {
		 if ((vert.x==vv[k].x) && (vert.y==vv[k].y) && (vert.z==vv[k].z))
		   found=TRUE;
		 else k++;
      }

	  // aggiungo il vertice, creo la sua entry nella tabella hash
	  if (found==FALSE)
	  {
		 vv[i]=vert;
		 hash[j]=i;
		 i++;
	  }
	  else hash[j]=k;
   }
   *num_vv=i;
}

//--------------------------------------------------------------------

void SkeletonExporter::build_valid_texture_vertex (Mesh *mesh,
												   int mapping_channel,
												   UVVert *uv,
												   int *hash,
												   int *num_uv)
{
   int i, j , k, nv;
   UVVert a, *uvVert;
   BOOL found;

   nv=mesh->getNumMapVerts(mapping_channel);
   uvVert=mesh->mapVerts(mapping_channel);
   i=0;

   for (j=0; j<nv; j++)
   {
	  a=uvVert[j];
	  found=FALSE;
	  k=0;
	  while ((found==FALSE) && (k<i))
	  {
		 if ((a.x==uv[k].x) && (a.y==uv[k].y)) found=TRUE;
		 else k++;
      }
	  // aggiungo il vertice, creo la sua entry nella tabella hash
	  if (found==FALSE)
	  {
		 uv[i]=a;
		 hash[j]=i;
		 i++;
	  }
	  else hash[j]=k;
   }
   *num_uv=i;
}

//--------------------------------------------------------------------

// Tool di ottimizzazione per le mesh: rimozione di triangoli
// geometrici degeneri
void SkeletonExporter::build_valid_face (Mesh *mesh, int *vf,
										 int *num_vf)
{
   int nt, i, j;
   Point3 a, b, c, d1, d2, d3;

   nt=mesh->getNumFaces();
   i=0;
   for (j=0; j<nt; j++)
   {
	  a=mesh->verts[mesh->faces[j].v[0]];
	  b=mesh->verts[mesh->faces[j].v[1]];
	  c=mesh->verts[mesh->faces[j].v[2]];
	  d1=a-b;
	  d2=a-c;
	  d3=b-c;
	  if ((d1.x==0) && (d1.y==0) && (d1.z==0)) continue;
	  if ((d2.x==0) && (d2.y==0) && (d2.z==0)) continue;
	  if ((d3.x==0) && (d3.y==0) && (d3.z==0)) continue;
	  vf[i]=j;
	  i++;
   }
   *num_vf=i;
}