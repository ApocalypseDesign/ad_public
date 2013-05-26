#include "SceneExport.h"
#include "A3Dfile.h"

void SkeletonExporter::export_patch(INode *node)
{
  int i, j, numcont;
  ObjectState os = node->EvalWorldState(0);
  PatchObject *patch_obj;
  PatchObject *patch_obj_base;

  if (!os.obj) return;
  patch_obj=(PatchObject *)os.obj;
  patch_obj_base=(PatchObject *)GetBaseObjectFromNode(node);

  Mtl *materiale=node->GetMtl();
  PatchMesh *pm=&patch_obj->patch;
  PatchMesh *pm_base=&patch_obj_base->patch;

  int num_vert=pm->numVerts;
  int num_vec=pm->numVecs;
  int num_patch=pm->numPatches;
  int num_UVvert=pm->getNumTVerts();

  INode *padre=node->GetParentNode();
  int so=strlen(node->GetName());
  int sf, sm;
  Point3 row;

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
  sf=strlen(padre->GetName())+1;
  else sf=0;
  if (materiale) sm=strlen(materiale->GetName())+1;
  else sm=0;

  // NB: SI SUPPONGONO TUTTE LE PATCH QUADRATE !!!
  if (num_UVvert>0)  // se le patch sono mappate
  write_chunk_header(fA3D, PATCH_MESH_ID, node->GetName(),
	                 4+sf+4+sm+
					 +12    // pivot
					 +4     // numero vertici geometrici
					 +num_vert*12
					 +4     // numero di vettori geometrici
					 +num_vec*12
					 +4     // numero di vertici texture
					 +num_UVvert*8
					 +4     // numero di pezze
					 +num_patch*21*4
					 );  
  else
  write_chunk_header(fA3D, PATCH_MESH_ID, node->GetName(),
	                 4+sf+4+sm+
					 +12    // pivot
					 +4     // numero vertici geometrici
					 +num_vert*12
					 +4     // numero di vettori geometrici
					 +num_vec*12
					 +4     // numero di vertici texture
					 +num_UVvert*8
					 +4     // numero di pezze
					 +num_patch*17*4
					 );  


  // scrivo il padre (flag, nome)
  fwrite(&sf, sizeof(int), 1, fA3D);
  if (sf>0) write_string0(fA3D, padre->GetName());

  // scrivo il materiale di base (flag, nome)
  fwrite(&sm, sizeof(int), 1, fA3D);
  if (sm>0) write_string0(fA3D, materiale->GetName());

  // nome oggetto e nome materiale
  fprintf(fTXT, "Patch object found\n");
  fprintf(fTXT, "Name : %s\n", node->GetName());
  fprintf(fTXT, "Father name : %s\n", padre->GetName());
  if (materiale) fprintf(fTXT, "Materiale: %s\n", materiale->GetName());

  // punto pivot
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fA3D);
  fwrite(&row.y, sizeof(float), 1, fA3D);
  fwrite(&row.z, sizeof(float), 1, fA3D);


  // vertici geometrici
  fprintf(fTXT, "Numero di vertici geometrici %d\n", num_vert);
  fwrite(&num_vert, sizeof(int), 1, fA3D);
  for (i=0; i<num_vert; i++)
  {
	fprintf(fTXT, "vertice geometrico %d : %f, %f, %f \n",
	        i, pm->verts[i].p.x, pm->verts[i].p.y, pm->verts[i].p.z);
	fwrite(&pm->verts[i].p.x, sizeof(float), 1, fA3D);
	fwrite(&pm->verts[i].p.y, sizeof(float), 1, fA3D);
	fwrite(&pm->verts[i].p.z, sizeof(float), 1, fA3D);
  }

  // vettori geometrici
  fprintf(fTXT, "Numero di vettori gemetrici %d\n", num_vec);
  fwrite(&num_vec, sizeof(int), 1, fA3D);
  for (i=0; i<num_vec; i++)
  {
	fprintf(fTXT, "vettore geometrico %d : %f, %f, %f \n",
	        i, pm->vecs[i].p.x, pm->vecs[i].p.y, pm->vecs[i].p.z);
	fwrite(&pm->vecs[i].p.x, sizeof(float), 1, fA3D);
	fwrite(&pm->vecs[i].p.y, sizeof(float), 1, fA3D);
	fwrite(&pm->vecs[i].p.z, sizeof(float), 1, fA3D);
  }

  
  // vertici texture
  fprintf(fTXT, "Numero di vertici texture: %d\n", num_UVvert);
  fwrite(&num_UVvert, sizeof(int), 1, fA3D);
  for (i=0; i<num_UVvert; i++)
  {
	UVVert map;

	map=pm->getMapVert(1, i);  // mapping channel 1
	fprintf(fTXT, "vertice texture %d : %f, %f\n", i, map.x, map.y);
	fwrite(&map.x, sizeof(float), 1, fA3D);
	fwrite(&map.y, sizeof(float), 1, fA3D);
  }

  
  // pezze
  fprintf(fTXT, "Numero di pezze %d\n", num_patch);
  fwrite(&num_patch, sizeof(int), 1, fA3D);
  int type0=0, type1=1;
  for (i=0; i<num_patch; i++)
  {
    Patch *p=&pm->patches[i];

	fprintf(fTXT, "pezza %d\n", i);
	switch (p->type)
	{
	  case PATCH_QUAD:   
		 fprintf(fTXT, "tipo: pezza quadrata\n");
		 fwrite(&type0, sizeof(int), 1, fA3D);

	     fprintf(fTXT, "Indici dei vertici geometrici (corners): %d, %d, %d, %d\n",
		         p->v[0], p->v[1], p->v[2], p->v[3]);
		 for (j=0; j<4; j++) fwrite(&p->v[j], sizeof(int), 1, fA3D);

         fprintf(fTXT, "Indici dei vettori geometrici: %d, %d, %d, %d, %d, %d, %d, %d\n",
		          p->vec[0], p->vec[1], p->vec[2], p->vec[3],
                  p->vec[4], p->vec[5], p->vec[6], p->vec[7]);
		 for (j=0; j<8; j++) fwrite(&p->vec[j], sizeof(int), 1, fA3D);

	     fprintf(fTXT, "Indici dei vertici interior: %d, %d, %d, %d\n",
		         p->interior[0], p->interior[1], p->interior[2], p->interior[3]);
		 for (j=0; j<4; j++) fwrite(&p->interior[j], sizeof(int), 1, fA3D);

		 if (num_UVvert>0)
		 {
		   TVPatch tp=pm->getMapPatch(1, i);
		   fprintf(fTXT, "Indici dei vertici texture: %d, %d, %d, %d\n",
			       tp.tv[0], tp.tv[1], tp.tv[2], tp.tv[3]);
		   for (j=0; j<4; j++) fwrite(&tp.tv[j], sizeof(int), 1, fA3D);
		 }
		 break;


	  case PATCH_TRI:
		 fprintf(fTXT, "tipo: pezza triangolare\n");
		 fwrite(&type1, sizeof(int), 1, fA3D);

	     fprintf(fTXT, "Indici dei vertici geometrici (corners): %d, %d, %d\n",
		         p->v[0], p->v[1], p->v[2]);
		 for (j=0; j<3; j++) fwrite(&p->v[j], sizeof(int), 1, fA3D);
		 
         fprintf(fTXT, "Indici dei vettori geometrici: %d, %d, %d, %d, %d, %d\n",
		         p->vec[0], p->vec[1], p->vec[2], p->vec[3],
                 p->vec[4], p->vec[5]);
		 for (j=0; j<6; j++) fwrite(&p->vec[j], sizeof(int), 1, fA3D);

	     fprintf(fTXT, "Indici dei vertici interior: %d, %d, %d\n",
		         p->interior[0], p->interior[1], p->interior[2]);
		 for (j=0; j<3; j++) fwrite(&p->interior[j], sizeof(int), 1, fA3D);
		 if (num_UVvert>0)
		 {
		   TVPatch tp=pm->getMapPatch(1, i);
		   fprintf(fTXT, "Indici dei vertici texture: %d, %d, %d\n",
			       tp.tv[0], tp.tv[1], tp.tv[2]);
		   for (j=0; j<3; j++) fwrite(&tp.tv[j], sizeof(int), 1, fA3D);
		 }
		 break;


	  case PATCH_UNDEF:  fprintf(fTXT, "tipo: pezza indefinita\n");
		                 break;
	}
  }



  //------------------------
  // ESPORTAZIONE KEYFRAMER |
  //------------------------
  Control *c;
  int size_key;

  // NB: per gli oggetti mesh e quant'altre tipologie di
  // oggetti che possono essere linkati (ovvero dove e'
  // possibile implmenetare gerarchie), ad esempio patch
  // di Bezier, esporto SEMPRE una key di posizione, una
  // di rotazione ed una di scaling
  
  // esportiamo l'animazione della posizione dell'oggetto
  // se ci sono un numero di key > 0


  // POSITION CONTROLLER
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Patch position track present.\n");
     write_chunk_header(fA3D, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fA3D);
  }
  // esportiamo comunque una key di posizione TCB
  else
  {
	fprintf(fTXT, "Patch position track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fA3D, POSITION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_point3_track(c, 1, fA3D);
  }



  // ROTATION CONTROLLER
  c=node->GetTMController()->GetRotationController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=40;
	 else size_key=20;
	 fprintf(fTXT, "Patch rotation track present.\n");
     write_chunk_header(fA3D, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fA3D);
  }
  // esportiamo comunque una key di rotazione TCB
  else
  {
	fprintf(fTXT, "Patch rotation track present. (1 key case)\n");
	size_key=40;
    write_chunk_header(fA3D, ROTATION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_rot_track(c, fA3D);
  }


  
  // SCALE CONTROLLER
  c=node->GetTMController()->GetScaleController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Patch scaling track present.\n");
     write_chunk_header(fA3D, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fA3D);
  }
  // esportiamo comunque una key di scaling TCB
  else
  {
	fprintf(fTXT, "Patch scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fA3D, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fA3D);
  }


  // NB: i controlli di vertici e vettori risiedono nella
  // patch base, per questo devo estrarla
  patch_obj=(PatchObject *)GetBaseObjectFromNode(node);
  // controllore master
  numcont = patch_obj->masterCont->GetNumSubControllers();
  fprintf(fTXT, "Numero di sotto-controller (from master) : %d\n", numcont);
/*
  for (i=0; i<numcont; i++)
  {
	Control *c=patch_obj->masterCont->GetSubController(i);
	if (c) fprintf(fTXT, "Controller %d : numkeys = %d\n", i, c->NumKeys());
  }
*/
  // *********************************************************
  // KEYFRAMER DEI VERTICI GEOMETRICI (CORNERS), this is cool!
  // *********************************************************
  numcont = patch_obj->vertCont.Count();
  fprintf(fTXT, "Numero di controller vertici geometrici : %d\n", numcont);
  for (i=0; i<numcont; i++)
  {
	Control *c=patch_obj->vertCont[i];
	if ((c) && (c->NumKeys()>0))
	{
	  fprintf(fTXT, "Controller vertice geometrico %d : numkeys = %d\n", i, c->NumKeys());
	  if (IsTCBControl(c)) size_key=36;
	  else
	  if (IsBezierControl(c)) size_key=40;
	  else size_key=16;

      write_chunk_header(fA3D, PATCH_VERTEX_TRACK_ID,
	                     node->GetName(), 1+2+4+c->NumKeys()*size_key+4);
	  fwrite(&i, sizeof(int), 1, fA3D);
      export_point3_track(c, 1, fA3D);
	}
  }


  // *********************************************************
  // KEYFRAMER DEI VETTORI GEOMETRICI, this is damn cool !!!!!
  // *********************************************************
  numcont = patch_obj->vecCont.Count();
  fprintf(fTXT, "Numero di controller vettori geometrici : %d\n", numcont);
  for (i=0; i<numcont; i++)
  {
	Control *c=patch_obj->vecCont[i];
	if ((c) && (c->NumKeys()>0))
	{
	  fprintf(fTXT, "Controller vettore geometrico %d : numkeys = %d\n", i, c->NumKeys());
	  if (IsTCBControl(c)) size_key=36;
	  else
	  if (IsBezierControl(c)) size_key=40;
	  else size_key=16;

      write_chunk_header(fA3D, PATCH_VECTOR_TRACK_ID,
	                     node->GetName(), 1+2+4+c->NumKeys()*size_key+4);
	  fwrite(&i, sizeof(int), 1, fA3D);
      export_point3_track(c, 1, fA3D);
	}
  }

  fprintf(fTXT, "\n\n");
  if (makeADO)
  {
    fprintf(fADO, "  patch %c%s%c\n  {\n", '"', node->GetName(), '"');
	fprintf(fADO, "      u_steps=%c10%c;\n", '"', '"');
	fprintf(fADO, "      v_steps=%c10%c;\n", '"', '"');
	fprintf(fADO, "      swap_normals=%cOFF%c;\n", '"', '"');
    fprintf(fADO, "  }\n\n", '"', '"');
  }
}