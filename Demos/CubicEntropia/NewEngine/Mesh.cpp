#include "UtilsA3D.h"
#include "Utility.h"
#include "Mesh.h"

extern HWND miawin;         // finestra principale
extern FILE *fdeb;

CMesh::CMesh()
{
   strcpy(p_Name, "");
   p_ObjspaceVertex=(AD_Vect3D *)NULL;
   p_ObjspaceNormals=(AD_Vect3D *)NULL;
   p_UV1vertex=(AD_VectUV *)NULL;
   p_UV2vertex=(AD_VectUV *)NULL;
   p_DriverVertex=(DriverVertex *)NULL;
   p_NumDriverVertex=0;
   p_NumObjspaceVertex=0;
   p_NumObjspaceNormals=0;

   p_NumUV1vertex=0;
   p_NumUV2vertex=0;

   p_Triangles=(AD_Tria3D *)NULL;
   p_DriverTriangles=(DriverTriangle *)NULL;
   p_NumTriangles=0;
   p_NumDriverTriangles=0;
   p_SmoothingGroups=(int32 *)NULL;

   p_SPHEREVolumeTreeDepth=1;
   p_AABBVolumeTreeDepth=1;
   p_OBBVolumeTreeDepth=1;

   p_MinX=p_MinY=p_MinZ=0;
   p_MaxX=p_MaxY=p_MaxZ=0;
}

//-------------------------------------------------------------------

int32 CMesh::m_NormalizeObjspaceVertex(void)
{
   int32 k;
   float4 maxDist, dist, invRadius;

   if ((!p_ObjspaceVertex) || (p_NumObjspaceVertex<=0)) return(0);

   // calcolo del raggio massimo
   maxDist=-1E10;
   for (k=0; k<p_NumObjspaceVertex; k++)
   {
	  dist=vect_dot(&p_ObjspaceVertex[k], &p_ObjspaceVertex[k]);
	  if (dist>maxDist) maxDist=dist;
   }
   invRadius=1.0f/(float4)sqrt(maxDist);

   for (k=0; k<p_NumObjspaceVertex; k++)
     vect_auto_scale(&p_ObjspaceVertex[k], invRadius);
   return(1);
}

//-------------------------------------------------------------------

void CMesh::m_InitTriangle(AD_Tria3D *t)
{
  AD_Vect3D p1, p2, midp;

  // Calcolo della normale alla faccia
  // NB: si suppone come sempre che i vertici siano in senso orario
  vect_sub(&p_ObjspaceVertex[t->i1], &p_ObjspaceVertex[t->i2], &p1);
  vect_sub(&p_ObjspaceVertex[t->i3], &p_ObjspaceVertex[t->i2], &p2);
  vect_cross(&p1, &p2, &t->normal);
  vect_auto_normalize(&t->normal);

  // Calcolo del del punto medio
  vect_add(&p_ObjspaceVertex[t->i1], &p_ObjspaceVertex[t->i2], &midp);
  vect_add(&midp, &p_ObjspaceVertex[t->i3], &midp);
  vect_scale(&midp, 1.0f/3.0f, &t->midpoint);
}

//--------------------------------------------------------------------

void CMesh::m_InitTriangleBump(AD_Tria3D *t)
{
  AD_Vect3D p1, p2, midp;
  AD_VectUV vtex1, vtex2;

  // Calcolo della normale alla faccia
  // NB: si suppone come sempre che i vertici siano in senso orario
  vect_sub(&p_ObjspaceVertex[t->i1], &p_ObjspaceVertex[t->i2], &p1);
  vect_sub(&p_ObjspaceVertex[t->i3], &p_ObjspaceVertex[t->i2], &p2);
  vect_cross(&p1, &p2, &t->normal);
  vect_auto_normalize(&t->normal);

  // Calcolo del del punto medio
  vect_add(&p_ObjspaceVertex[t->i1], &p_ObjspaceVertex[t->i2], &midp);
  vect_add(&midp, &p_ObjspaceVertex[t->i3], &midp);
  vect_scale(&midp, 1.0f/3.0f, &t->midpoint);

  // calcolo del bump space
  vtex1.u = (p_UV1vertex[t->UV1_i1].u - p_UV1vertex[t->UV1_i2].u);
  vtex1.v = (p_UV1vertex[t->UV1_i1].v - p_UV1vertex[t->UV1_i2].v);
  vtex2.u = (p_UV1vertex[t->UV1_i3].u - p_UV1vertex[t->UV1_i2].u);
  vtex2.v = (p_UV1vertex[t->UV1_i3].v - p_UV1vertex[t->UV1_i2].v);

  t->bumpspace.T.x=vtex1.u*p2.x-vtex2.u*p1.x;
  t->bumpspace.T.y=vtex1.u*p2.y-vtex2.u*p1.y;
  t->bumpspace.T.z=vtex1.u*p2.z-vtex2.u*p1.z;

  vect_auto_normalize(&t->bumpspace.T);
  //vect_cross(&t->bumpspace.T, &t->normal, &t->bumpspace.S);
  //vect_cross(&t->bumpspace.S, &t->bumpspace.T, &t->bumpspace.SxT);


  // allineiamo la direzione della binormale del bump space con la
  // direzione della normale del triangolo
//  if (vect_dot(&t->bumpspace.SxT, &t->normal) < 0)
//  {
//     vect_auto_neg(&t->bumpspace.SxT);
//  }
}

//-------------------------------------------------------------------

void CMesh::m_InitTriangles(void)
{
  int32 i;

  if (p_UV1vertex)
  {
    for (i=0; i<p_NumTriangles; i++)
       m_InitTriangleBump(&p_Triangles[i]);
  }
  else
  {
    for (i=0; i<p_NumTriangles; i++)
       m_InitTriangle(&p_Triangles[i]);
  }
}

//-------------------------------------------------------------------

void CMesh::m_InitTextureCoordinates(void)
{
  //float4 minu, minv, addu, addv;
  //int32 i;

  // ############  SISTEMA TUTTE LE COPPIE (u,v) A POSITIVE ############
  if (p_UV1vertex)
  {
     //minu=1E10;
	 //minv=1E10;
	 /*
     for (i=0; i<p_NumUV1vertex; i++)
     {
        if (p_UV1vertex[i].u<minu) minu=p_UV1vertex[i].u;
        if (p_UV1vertex[i].v<minv) minv=p_UV1vertex[i].v;
     }

     // conto il numero di 1.0 da aggiungere per portare tutto a >0
     addu=addv=0.0;
     if ((minu<0) && (minu>-1.0))  addu=-1.0;
     if ((minu<0) && (minu<=-1.0)) addu=(float4)floor(minu);
     if ((minv<0) && (minv>-1.0))  addv=-1.0;
     if ((minv<0) && (minv<=-1.0)) addv=(float4)floor(minv);

     for (i=0; i<p_NumUV1vertex; i++)
     {
	   //p_UV1vertex[i].u=(p_UV1vertex[i].u-addu);
	   //p_UV1vertex[i].v=(p_UV1vertex[i].v-addv);
     }*/
  }

  if (p_UV2vertex)
  {
     //minu=1E10;
	 //minv=1E10;
	 /*
     for (i=0; i<p_NumUV2vertex; i++)
     {
        if (p_UV2vertex[i].u<minu) minu=p_UV2vertex[i].u;
        if (p_UV2vertex[i].v<minv) minv=p_UV2vertex[i].v;
     }

     // conto il numero di 1.0 da aggiungere per portare tutto a >0
     addu=addv=0.0;
     if ((minu<0) && (minu>-1.0))  addu=-1.0;
     if ((minu<0) && (minu<=-1.0)) addu=(float4)floor(minu);
     if ((minv<0) && (minv>-1.0))  addv=-1.0;
     if ((minv<0) && (minv<=-1.0)) addv=(float4)floor(minv);

     for (i=0; i<p_NumUV2vertex; i++)
     {
	   //p_UV2vertex[i].u=(p_UV2vertex[i].u-addu);
	   //p_UV2vertex[i].v=(p_UV2vertex[i].v-addv);
     }*/
  }
}

//-------------------------------------------------------------------

float4 fmax(float4 a, float4 b) { if (a>=b) return a; else return b; }

void CMesh::m_BuildVertexNormalsBump(void)
{
  const float4 normal_err = 0.0001f;

   // indica l'errore massimo possibile x riciclare una normale: + e' alto il numero piu'
   // si perde qualita' visiva e si guadagna velocita': bisogna trovare un buon compromesso

  int32 *condivisi;
  int32 *smooth, *nosmooth; // array di triangoli condivisi da smoothare
  AD_Vect3D *tempnormal;
  BUMPSPACE temp_bumpspace, *temp_bumpspaces;
  int32 num_bumpspaces;
  AD_Vect3D normadd;
  int32 num_condivisi, i, j, tr, num_smooth, num_nosmooth;
  int32 num_normal, norm;
  float4 maxerr, err;

  condivisi=new int32[p_NumTriangles]; // nel caso peggiore tutti i triangoli sono condivisi
  smooth=new int32[p_NumTriangles];
  nosmooth=new int32[p_NumTriangles];
  tempnormal=new AD_Vect3D[p_NumTriangles*3]; // caso peggiore ci sono 3 normali per triangolo
  num_normal=0;
  temp_bumpspaces=new BUMPSPACE[p_NumTriangles*3];
  num_bumpspaces=0;
      
  for (i=0; i<p_NumObjspaceVertex; i++)
  {
	// trovo i triangoli che condividono il vertice i
	num_condivisi=0;
	for (j=0; j<p_NumTriangles; j++)
	{
	    if ((p_Triangles[j].i1 == i) ||
			(p_Triangles[j].i2 == i) ||
			(p_Triangles[j].i3 == i))
	    {
		   condivisi[num_condivisi]=j;
		   num_condivisi++;          
	    }
	}

	while (num_condivisi>0)
	{
	   tr=condivisi[0]; // triangolo di riferimento
	   num_smooth=0;
	   num_nosmooth=0;
	   smooth[num_smooth]=tr;
	   num_smooth++;
	   for(j=1; j<num_condivisi; j++)
	   {
	      if ((p_SmoothingGroups[tr] & p_SmoothingGroups[condivisi[j]])!=0)
		  {
		     smooth[num_smooth]=condivisi[j];
		     num_smooth++;
		  }
		  else
		  {
		     nosmooth[num_nosmooth]=condivisi[j];
		     num_nosmooth++;
		  }
	   }

	   // cacolo la normale e bump space
	   vect_set(&normadd, 0, 0, 0);
       vect_set(&temp_bumpspace.S, 0, 0, 0);
       vect_set(&temp_bumpspace.T, 0, 0, 0);
       vect_set(&temp_bumpspace.SxT, 0, 0, 0);
	   for (j=0; j<num_smooth; j++)
	   {
          vect_auto_add(&normadd, &p_Triangles[smooth[j]].normal);
          vect_auto_add(&temp_bumpspace.T, &p_Triangles[smooth[j]].bumpspace.T);
	   }
	   vect_auto_normalize(&normadd);
       vect_auto_normalize(&temp_bumpspace.T);

	   // cerco se ne esiste gia' una molto simile
	   j=0;
	   norm=-1;
	   maxerr=normal_err;
	   while (j < num_normal)
	   { 
	      // trovo l'errore massimo della normale j
	      err=fmax(fmax((float4)fabs(normadd.x-tempnormal[j].x),
			            (float4)fabs(normadd.y-tempnormal[j].y)),
			       (float4)fabs(normadd.z-tempnormal[j].z));
	      if (err < maxerr)
		  { 
             // trovata normale + precisa
			 maxerr=err;
			 norm=j;
		  }
	      j++;
	   }
	   if (norm==-1)
	   { // non trovata: la creo
	      vect_copy(&normadd, &tempnormal[num_normal]);
		  norm=num_normal;
	      num_normal++;
	   }

       // per i bump space aggiungo sempre, perchè fare controlli
	   // di uguaglianza per 3 assi a è dispendioso
       temp_bumpspaces[num_bumpspaces++]=temp_bumpspace;


	   for (j=0; j<num_smooth; j++)
	   {
	     // la assegno al triangolo (cercando il vertice giusto)
	     if (p_Triangles[smooth[j]].i1==i)
		 {
            p_Triangles[smooth[j]].n1=&tempnormal[norm];
            p_Triangles[smooth[j]].bump1=&temp_bumpspaces[num_bumpspaces-1];
		 }
	     if (p_Triangles[smooth[j]].i2==i)
		 {
            p_Triangles[smooth[j]].n2=&tempnormal[norm];
            p_Triangles[smooth[j]].bump2=&temp_bumpspaces[num_bumpspaces-1];
		 }
	     if (p_Triangles[smooth[j]].i3==i)
		 {
            p_Triangles[smooth[j]].n3=&tempnormal[norm];
            p_Triangles[smooth[j]].bump3=&temp_bumpspaces[num_bumpspaces-1];
		 }
	   }
	      
	   // tolgo quelli assegnati e ricompatto gli altri
	   for (j=0; j<num_nosmooth; j++) condivisi[j]=nosmooth[j];
	   num_condivisi-=num_smooth;
	}
  }
 
  // copio la parte usata di tempnormal in normal e anche per i bump
  p_ObjspaceNormals = new AD_Vect3D[num_normal];
  p_NumObjspaceNormals=num_normal;
  for (j=0; j<p_NumObjspaceNormals; j++)
  {
	  vect_copy(&tempnormal[j], &p_ObjspaceNormals[j]);
	  for (i=0; i<p_NumTriangles; i++)
	  {
		 if (p_Triangles[i].n1==&tempnormal[j])
			 p_Triangles[i].n1=&p_ObjspaceNormals[j];

		 if (p_Triangles[i].n2==&tempnormal[j])
			 p_Triangles[i].n2=&p_ObjspaceNormals[j];

		 if (p_Triangles[i].n3==&tempnormal[j])
			 p_Triangles[i].n3=&p_ObjspaceNormals[j];
	  }
  }

  p_VertexBumpSpaces=new BUMPSPACE[num_bumpspaces];
  p_NumVertexBumpSpaces=num_bumpspaces;
  for (j=0; j<p_NumVertexBumpSpaces; j++)
  {
	  p_VertexBumpSpaces[j]=temp_bumpspaces[j];
	  for (i=0; i<p_NumTriangles; i++)
	  {
		 if (p_Triangles[i].bump1==&temp_bumpspaces[j])
			 p_Triangles[i].bump1=&p_VertexBumpSpaces[j];

		 if (p_Triangles[i].bump2==&temp_bumpspaces[j])
			 p_Triangles[i].bump2=&p_VertexBumpSpaces[j];

		 if (p_Triangles[i].bump3==&temp_bumpspaces[j])
			 p_Triangles[i].bump3=&p_VertexBumpSpaces[j];
	  }
  }

  delete [] condivisi;
  delete [] smooth;
  delete [] nosmooth;
  delete [] tempnormal;
  delete [] temp_bumpspaces;
}

//-------------------------------------------------------------------

void CMesh::m_BuildVertexNormals(void)
{
  const float4 normal_err = 0.0001f;

   // indica l'errore massimo possibile x riciclare una normale: + e' alto il numero piu'
   // si perde qualita' visiva e si guadagna velocita': bisogna trovare un buon compromesso

  int32 *condivisi;
  int32 *smooth, *nosmooth; // array di triangoli condivisi da smoothare
  AD_Vect3D *tempnormal;
  AD_Vect3D normadd;
  int32 num_condivisi, i, j, tr, num_smooth, num_nosmooth;
  int32 num_normal, norm;
  float4 maxerr, err;

  condivisi=new int32[p_NumTriangles]; // nel caso peggiore tutti i triangoli sono condivisi
  smooth=new int32[p_NumTriangles];
  nosmooth=new int32[p_NumTriangles];
  tempnormal=new AD_Vect3D[p_NumTriangles*3]; // nel caso peggiore ci sono 3 normali per triangolo
  num_normal=0;
      
  for (i=0; i<p_NumObjspaceVertex; i++)
  {
	// trovo i triangoli che condividono il vertice i
	num_condivisi=0;
	for (j=0; j<p_NumTriangles; j++)
	{
	    if ((p_Triangles[j].i1 == i) ||
			(p_Triangles[j].i2 == i) ||
			(p_Triangles[j].i3 == i))
	    {
		   condivisi[num_condivisi]=j;
		   num_condivisi++;          
	    }
	}

	while (num_condivisi>0)
	{
	   tr=condivisi[0]; // triangolo di riferimento
	   num_smooth=0;
	   num_nosmooth=0;
	   smooth[num_smooth]=tr;
	   num_smooth++;
	   for(j=1; j<num_condivisi; j++)
	   {
	      if ((p_SmoothingGroups[tr] & p_SmoothingGroups[condivisi[j]])!=0)
		  {
		     smooth[num_smooth]=condivisi[j];
		     num_smooth++;
		  }
		  else
		  {
		     nosmooth[num_nosmooth]=condivisi[j];
		     num_nosmooth++;
		  }
	   }

	   // cacolo la normale
	   vect_set(&normadd, 0, 0, 0);
	   for (j=0; j<num_smooth; j++)
          vect_auto_add(&normadd, &p_Triangles[smooth[j]].normal);
	   vect_auto_normalize(&normadd);

	   // cerco se ne esiste gia' una molto simile
	   j=0;
	   norm=-1;
	   maxerr=normal_err;
	   while (j < num_normal)
	   { 
	      // trovo l'errore massimo della normale j
	      err=fmax(fmax((float4)fabs(normadd.x-tempnormal[j].x),
			            (float4)fabs(normadd.y-tempnormal[j].y)),
			       (float4)fabs(normadd.z-tempnormal[j].z));
	      if (err < maxerr)
		  { 
              // trovata normale + precisa
			  maxerr=err;
			  norm=j;
		  }
	      j++;
	   }
	   if (norm==-1)
	   { // non trovata: la creo
	      vect_copy(&normadd, &tempnormal[num_normal]);
		  norm=num_normal;
	      num_normal++;
	   }

	   for (j=0; j<num_smooth; j++)
	   {
	     // la assegno al triangolo (cercando il vertice giusto)
	     if (p_Triangles[smooth[j]].i1==i) p_Triangles[smooth[j]].n1=&tempnormal[norm];
	     if (p_Triangles[smooth[j]].i2==i) p_Triangles[smooth[j]].n2=&tempnormal[norm];
	     if (p_Triangles[smooth[j]].i3==i) p_Triangles[smooth[j]].n3=&tempnormal[norm];
	   }
	      
	   // tolgo quelli assegnati e ricompatto gli altri
	   for (j=0; j<num_nosmooth; j++) condivisi[j]=nosmooth[j];
	   num_condivisi-=num_smooth;
	}
  }
 
  // copio la parte usata di tempnormal in normal
  p_ObjspaceNormals = new AD_Vect3D[num_normal];
  p_NumObjspaceNormals=num_normal;
  for (j=0; j<p_NumObjspaceNormals; j++)
  {
	  vect_copy(&tempnormal[j], &p_ObjspaceNormals[j]);
	  for (i=0; i<p_NumTriangles; i++)
	  {
		 if (p_Triangles[i].n1==&tempnormal[j])
			 p_Triangles[i].n1=&p_ObjspaceNormals[j];

		 if (p_Triangles[i].n2==&tempnormal[j])
			 p_Triangles[i].n2=&p_ObjspaceNormals[j];

		 if (p_Triangles[i].n3==&tempnormal[j])
			 p_Triangles[i].n3=&p_ObjspaceNormals[j];
	  }
  }
  delete [] condivisi;
  delete [] smooth;
  delete [] nosmooth;
  delete [] tempnormal;
}

//-------------------------------------------------------------------

int32 CMesh::m_Init(void)
{
   int32 l_result;
   char8 msg[150];

   // una mesh deve avere almeno l'informazione dei vertici e dei
   // triangoli geometrici
   if (!p_ObjspaceVertex)
     debug_error(miawin, "CMesh::m_Init(), p_ObjspaceVertex is NULL!");
   if (!p_Triangles)
     debug_error(miawin, "CMesh::m_Init(), p_Triangles is NULL!");

   // una mesh non puo' avere il secondo canale di mapping se non
   // ha anche il primo
   if ((p_UV2vertex) && (!p_UV1vertex))
     debug_error(miawin, "CMesh::m_Init(), p_UV1vertex is NULL and p_UV2vertex is NOT NULL!");

   m_CalcExtension();
   sprintf(msg, "%s extension calculation done", p_Name);
   DumpDebugPoint (fdeb, msg, 0);

   // setto a positive tutte le UV
   m_InitTextureCoordinates();
   sprintf(msg, "%s m_InitTextureCoordinates done", p_Name);
   DumpDebugPoint (fdeb, msg, 0);

   // inizializzo i triangoli
   m_InitTriangles();
   sprintf(msg, "%s m_InitTriangles done", p_Name);
   DumpDebugPoint (fdeb, msg, 0);
   
   // costruisco le normali tenendo conto degli (eventuali)
   // smoothing groups...
   if (p_SmoothingGroups)
   {
      if (p_UV1vertex) m_BuildVertexNormalsBump();
	  else m_BuildVertexNormals();
      sprintf(msg, "%s m_BuildVertexNormals done", p_Name);
      DumpDebugPoint (fdeb, msg, 0);
   }
   else return(0);

   // bound volumes
   p_SPHEREBoundVolume.m_Bound(this, p_SPHEREVolumeTreeDepth, SPHERE_PRIMITIVE);
   p_AABBBoundVolume.m_Bound(this, p_AABBVolumeTreeDepth, AABB_PRIMITIVE);
   p_OBBBoundVolume.m_Bound(this, p_OBBVolumeTreeDepth, OBB_PRIMITIVE);
   sprintf(msg, "%s bounding volumes done", p_Name);
   DumpDebugPoint (fdeb, msg, 0);


   // costruisco le strutture dati adatte a Direct3D 8.x
   l_result=m_BuildDriverStructures();
   sprintf(msg, "%s m_BuildDriverStructures done", p_Name);
   DumpDebugPoint (fdeb, msg, 0);

   // cancello i dati della mesh letti da file .A3D
   if (p_ObjspaceVertex)
   {
      delete [] p_ObjspaceVertex;
      p_ObjspaceVertex=NULL;
   }
   if (p_ObjspaceNormals)
   {
      delete [] p_ObjspaceNormals;
      p_ObjspaceNormals=NULL;
   }
   if (p_UV1vertex)
   {
      delete [] p_UV1vertex;
      p_UV1vertex=NULL;
   }
   if (p_UV2vertex)
   {
     delete [] p_UV2vertex;
     p_UV2vertex=NULL;
   }
   if (p_Triangles)
   {
     delete [] p_Triangles;
     p_Triangles=NULL;
   }
   if (p_SmoothingGroups)
   {
     delete [] p_SmoothingGroups;
     p_SmoothingGroups=NULL;
   }
   sprintf(msg, "%s clearing stuff done", p_Name);
   DumpDebugPoint (fdeb, msg, 0);
   return(l_result);
}

//-------------------------------------------------------------------

int32 CMesh::m_Load (char8 *filenameA3D, char8 *meshname)
{
   CFileA3D fA3D;

   if (p_Triangles)
   {
     delete p_Triangles;
     p_NumTriangles=0;
   }
   if (p_ObjspaceVertex)
   {
      delete [] p_ObjspaceVertex;
      p_ObjspaceVertex=NULL;
   }
   if (p_ObjspaceNormals)
   {
      delete [] p_ObjspaceNormals;
      p_ObjspaceNormals=NULL;
   }
   if (p_UV1vertex)
   {
      delete [] p_UV1vertex;
      p_UV1vertex=NULL;
   }
   if (p_UV2vertex)
   {
     delete [] p_UV2vertex;
     p_UV2vertex=NULL;
   }
   if (p_SmoothingGroups)
   {
     delete [] p_SmoothingGroups;
     p_SmoothingGroups=NULL;
   }

   return(fA3D.m_Load_Mesh(meshname, filenameA3D, this));
}

//-------------------------------------------------------------------

int32 CMesh::m_CompareDriverVertex (DriverVertex *v1, DriverVertex *v2)
{
   if ((v1->normal.x == v2->normal.x) &&
       (v1->normal.y == v2->normal.y) &&
       (v1->normal.z == v2->normal.z))
   {
	   if ((v1->point.x == v2->point.x) &&
           (v1->point.y == v2->point.y) &&
           (v1->point.z == v2->point.z))
	   {
		   //if ((v1->RGB.x == v2->RGB.x) &&
           //    (v1->RGB.y == v2->RGB.y) &&
           //    (v1->RGB.z == v2->RGB.z))
		   {
			   if ((v1->uv1.u == v2->uv1.u) &&
                   (v1->uv1.v == v2->uv1.v) &&
                   (v1->uv2.u == v2->uv2.u) && 
                   (v1->uv2.v == v2->uv2.v))
			   {
				   return(1);
			   }
			   else return(0);
		   }
		   //else return(0);
	   }
	   else return(0);
   }
   else return(0);
}

//-------------------------------------------------------------------

int32 CMesh::m_Optimize(void)
{
   // NB: questa funzione ottimizza il buffer di vertici e triangoli
   // "driver" eliminando vertici driver uguali
   int32 i, j, k;
   int32 l_quantiUguali, *index;

   index=new int32[p_NumDriverVertex];
   for (i=0; i<p_NumDriverVertex; i++) index[i]=0;

   l_quantiUguali=0;
   for (i=0; i<p_NumDriverVertex-1; i++)
   {
      for (j=i+1; j<p_NumDriverVertex; j++)
	  {
         if (m_CompareDriverVertex(&p_DriverVertex[i], &p_DriverVertex[j]))
		 {
			// se i vertici i e j sono uguali...
			for (k=0; k<p_NumDriverTriangles; k++)
			{
			   if (p_DriverTriangles[k].driverV1 == j)
                   p_DriverTriangles[k].driverV1=i;

			   if (p_DriverTriangles[k].driverV2 == j)
                   p_DriverTriangles[k].driverV2=i;

			   if (p_DriverTriangles[k].driverV3 == j)
                   p_DriverTriangles[k].driverV3=i;
			}
			if (index[j]==0)
			{
			   l_quantiUguali++;
			   index[j]=1;
			}
		 }
	  }
   }
   delete [] index;
   return(l_quantiUguali+1);
}

//-------------------------------------------------------------------

void CMesh::m_CalcExtension(void)
{
  int32 i;

  if (!p_ObjspaceVertex) return;

  p_MinX=p_MinY=p_MinZ=(float4)1E10;
  p_MaxX=p_MaxY=p_MaxZ=(float4)-1E10;

  for (i=0; i<p_NumObjspaceVertex; i++)
  {
     if (p_ObjspaceVertex[i].x < p_MinX) p_MinX=p_ObjspaceVertex[i].x;
     if (p_ObjspaceVertex[i].y < p_MinY) p_MinY=p_ObjspaceVertex[i].y;
     if (p_ObjspaceVertex[i].z < p_MinZ) p_MinZ=p_ObjspaceVertex[i].z;

     if (p_ObjspaceVertex[i].x >= p_MaxX) p_MaxX=p_ObjspaceVertex[i].x;
     if (p_ObjspaceVertex[i].y >= p_MaxY) p_MaxY=p_ObjspaceVertex[i].y;
     if (p_ObjspaceVertex[i].z >= p_MaxZ) p_MaxZ=p_ObjspaceVertex[i].z;
  }
}

//-------------------------------------------------------------------

int32 CMesh::m_BuildDriverStructures(void)
{
   int32 j;
   int32 i1, i2, i3;
   int32 UVi1, UVi2, UVi3;
   int32 equal;

   // alloco i vertici "driver"
   if (p_DriverVertex) delete [] p_DriverVertex;
   p_DriverVertex=new DriverVertex[p_NumTriangles*3];
   p_NumDriverVertex=p_NumTriangles*3;

   // alloco i triangoli "driver"
   if (p_DriverTriangles) delete [] p_DriverTriangles;
   p_DriverTriangles=new DriverTriangle[p_NumTriangles];
   p_NumDriverTriangles=p_NumTriangles;

   for (j=0; j<p_NumDriverTriangles; j++)
   {
	  i1=p_Triangles[j].i1;
	  i2=p_Triangles[j].i2;
	  i3=p_Triangles[j].i3;

	  // copia dei vertici in object space
	  vect_copy(&p_ObjspaceVertex[i1], &p_DriverVertex[j*3+0].point);
	  vect_copy(&p_ObjspaceVertex[i2], &p_DriverVertex[j*3+1].point);
	  vect_copy(&p_ObjspaceVertex[i3], &p_DriverVertex[j*3+2].point);

	  // copia delle normali in object space
	  vect_copy(p_Triangles[j].n1, &p_DriverVertex[j*3+0].normal);
	  vect_copy(p_Triangles[j].n2, &p_DriverVertex[j*3+1].normal);
	  vect_copy(p_Triangles[j].n3, &p_DriverVertex[j*3+2].normal);

	  if (p_UV1vertex)
	  {
	     // copio il primo canale di mapping e il bump space
	     UVi1=p_Triangles[j].UV1_i1;
	     UVi2=p_Triangles[j].UV1_i2;
	     UVi3=p_Triangles[j].UV1_i3;
	     p_DriverVertex[j*3+0].uv1.u=p_UV1vertex[UVi1].u;
	     p_DriverVertex[j*3+0].uv1.v=p_UV1vertex[UVi1].v;
         p_DriverVertex[j*3+0].bumpspace=*p_Triangles[j].bump1;
	     p_DriverVertex[j*3+1].uv1.u=p_UV1vertex[UVi2].u;
	     p_DriverVertex[j*3+1].uv1.v=p_UV1vertex[UVi2].v;
         p_DriverVertex[j*3+1].bumpspace=*p_Triangles[j].bump2;
	     p_DriverVertex[j*3+2].uv1.u=p_UV1vertex[UVi3].u;
	     p_DriverVertex[j*3+2].uv1.v=p_UV1vertex[UVi3].v;
         p_DriverVertex[j*3+2].bumpspace=*p_Triangles[j].bump3;
	  }

	  if (p_UV2vertex)
	  {
	     // copio il secondo canale di mapping
	     UVi1=p_Triangles[j].UV2_i1;
	     UVi2=p_Triangles[j].UV2_i2;
	     UVi3=p_Triangles[j].UV2_i3;
	     p_DriverVertex[j*3+0].uv2.u=p_UV2vertex[UVi1].u;
	     p_DriverVertex[j*3+0].uv2.v=p_UV2vertex[UVi1].v;
	     p_DriverVertex[j*3+1].uv2.u=p_UV2vertex[UVi2].u;
	     p_DriverVertex[j*3+1].uv2.v=p_UV2vertex[UVi2].v;
	     p_DriverVertex[j*3+2].uv2.u=p_UV2vertex[UVi3].u;
	     p_DriverVertex[j*3+2].uv2.v=p_UV2vertex[UVi3].v;
	  }
      // linko i vertici driver ai triangoli driver
	  p_DriverTriangles[j].driverV1=j*3+0;
      p_DriverTriangles[j].driverV2=j*3+1;
      p_DriverTriangles[j].driverV3=j*3+2;
   }

   equal=m_Optimize();
   return(1);
}

//-------------------------------------------------------------------
