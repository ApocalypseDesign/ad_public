#include "SceneExport.h"
#include "A3Dfile.h"
#include "OSMs.h"

void SkeletonExporter::export_faces_indexes (int i1, int i2, int i3, int sInd, FILE *f)
{
   unsigned char c1, c2, c3;
   short int s1, s2, s3;

   switch (sInd)
   {
     case 8:
        c1=(unsigned char)i1;
        c2=(unsigned char)i2;
        c3=(unsigned char)i3;
		fwrite(&c1, sizeof(unsigned char), 1, f);
		fwrite(&c2, sizeof(unsigned char), 1, f);
		fwrite(&c3, sizeof(unsigned char), 1, f);
		break;
     case 16:
        s1=(short int)i1;
        s2=(short int)i2;
        s3=(short int)i3;
		fwrite(&s1, sizeof(short int), 1, f);
		fwrite(&s2, sizeof(short int), 1, f);
		fwrite(&s3, sizeof(short int), 1, f);
		break;
     case 32:
		fwrite(&i1, sizeof(int), 1, f);
		fwrite(&i2, sizeof(int), 1, f);
		fwrite(&i3, sizeof(int), 1, f);
		break;
   }
}

// -------------------------------------------------------------------

void SkeletonExporter::export_solid_object(INode *node)
{
  int i, j, k;
  int tf1, tf2, tf3;
  int vx1, vx2, vx3;
  int size_phys, type_phys, has_known_modifiers;
  int size_skin, num_faces;
  int sizeIndexes;
  Modifier *physmod=(Modifier *)NULL,
	       *skinmod=(Modifier *)NULL;
  BOOL needDel, ndelPhys=FALSE, negScale;
  TriObject *tri, *triphys=(TriObject *)NULL;
  Mesh *mesh, *meshphys=(Mesh *)NULL;
  long fpos;
  Mtl *materiale;
  // per le ottimizzazioni e rimozioni di roba degenere
  Point3 row, *valid_vertex;
  int *vertex_hash, *valid_faces;
  int nv_non_optimised, nv_optimised;
  UVVert *uv;
  int *hash_uv, numTEXvert;


  ObjectState os = node->EvalWorldState(0);
  if (!os.obj) return;
	
  // Targets are actually geomobjects, but we will export them
  // from the camera and light objects, so we skip them here.
  if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
  { fprintf(fTXT, "\n"); return; }


  
  //************************************************************
//                  ESPORTAZIONE DELLA MESH
//************************************************************
  materiale=node->GetMtl();
  // vale come segue:
  // 1 = non ha nessun modificatore
  // 2 = ha solo il physic
  // 3 = non ha il physic ma in cascata ha solo modificatori di
  //     tipo TWISTER (e quelli che supportero' nell'engine)
  // 4 = ha modificatori implementati ma anche non implementati
  // 5 = ha lo Skin (3DS MAX 4.x)
  // NB: se la mesh ha un physic modifier o tutti OSM implementati
  // devo esportare i vertici dell'oggetto base e non
  // dell'oggetto valutato al tempo 0, perche' la deformazione
  // la applico a runtime nell'engine

  has_known_modifiers=KnownModifiers(node);
  if (has_known_modifiers==2)
  {
     physmod=FindPhysiqueModifier(node);
     if (physmod) fprintf(fTXT, "Physisc modifier found\n");
  }
  else
  if (has_known_modifiers==5)
  {
     skinmod=FindSkinModifier(node);
     if (skinmod) fprintf(fTXT, "Skin modifier found\n");
  }
  
  // in caso di modificatori OSM...
  Matrix3 mat_checkneg;
  if (has_known_modifiers==3)
  {  
	mat_checkneg = node->GetObjTMBeforeWSM(0);
	negScale=FALSE;
  }
  else
  { 
	mat_checkneg = node->GetNodeTM(0);
	negScale = TMNegParity(mat_checkneg);
  }
  if (negScale) { vx1 = 2; vx2 = 1; vx3 = 0; }
  else { vx1 = 0; vx2 = 1; vx3 = 2; }

  // ------------   esportazione vertici geometrici   ----------------
  if (!(tri= GetTriObjectFromNode(node, needDel))) return;
  mesh = &tri->GetMesh();

  // ---------------   scriviamo sul .ADO della mesh   ---------------
  if (makeADO)
  {
     // quando esportero' l'oggetto geometrico usero' come nome della
     //mesh proprio il nome del nodo
	 fprintf(fADO, "  mesh %c%s%c \n  {\n", '"', node->GetName(), '"');
	 fprintf(fADO, "    normalize=%cOFF%c;\n", '"', '"');
	 fprintf(fADO, "    boundvolumes_depths=%c0, 1, 0%c;\n", '"', '"');
	 fprintf(fADO, "  }\n\n");
  }


  nv_non_optimised=mesh->getNumVerts();
  fprintf(fTXT, "Number of vertex (non optimized) : %d\n", nv_non_optimised);
  valid_vertex=new Point3[nv_non_optimised];
  vertex_hash=new int[nv_non_optimised];

  if ((has_known_modifiers==2)||  // Physic
      (has_known_modifiers==5)||  // Skin
	  (has_known_modifiers==3))   // altri (ripple, bend, etc)
  {
	  // scrivo i vertici dell'oggetto base (not modified)
	  if (!(triphys = GetTriObjectFromObject(GetBaseObjectFromNode(node), ndelPhys))) return;
      meshphys = &triphys->GetMesh();

	  build_valid_vertex(meshphys, valid_vertex, vertex_hash, &nv_optimised);
      fprintf(fTXT, "Number of vertex (optimized): %d\n", nv_optimised);
      write_chunk_header(fA3D, TRI_MESH_VERTEX_LIST_ID, node->GetName(),
	                     4+nv_optimised*12);
      fwrite(&nv_optimised, sizeof(int), 1, fA3D);
      for (i=0; i<nv_optimised; i++)
	     // fprintf(fTXT, "Vertex %d : x=%f,  y=%f,  z=%f\n", i, v.x, v.y, v.z);
		 write_point3(&valid_vertex[i], fA3D);
      if (ndelPhys) delete triphys;
  }
  else
  {
	// scrivo i vertici dell'oggetto che ha seguito gia' la
	// pipeline geometrica del MAX
	build_valid_vertex(mesh, valid_vertex, vertex_hash, &nv_optimised);
    fprintf(fTXT, "Number of vertex (optimized): %d\n", nv_optimised);
    write_chunk_header(fA3D, TRI_MESH_VERTEX_LIST_ID, node->GetName(),
	                   4+nv_optimised*12);
    fwrite(&nv_optimised, sizeof(int), 1, fA3D);
    for (i=0; i<nv_optimised; i++)
      write_point3(&valid_vertex[i], fA3D);
  }


  // --------  scrittura del chunk delle facce geometriche  ----------
  num_faces=mesh->getNumFaces();
  valid_faces=new int[num_faces];  
  fprintf(fTXT, "Number of triangles (non optimized): %d\n", num_faces);
  build_valid_face(mesh, valid_faces, &num_faces);
  fprintf(fTXT, "Number of triangles (optimized): %d\n", num_faces);
  if (nv_optimised < 256) sizeIndexes=8;
  else
  if (nv_optimised < 65536) sizeIndexes=16;
  else sizeIndexes=32;

  // numero di triangoli + size degli indici + facce
  write_chunk_header(fA3D, TRI_MESH_FACES_LIST_ID, node->GetName(),
	                 4+4+num_faces*(4+3*sizeIndexes/8));
  fwrite(&num_faces, sizeof(int), 1, fA3D);
  fwrite(&sizeIndexes, sizeof(int), 1, fA3D);
  for (j=0; j<num_faces; j++)
  {
	tf1=vertex_hash[mesh->faces[valid_faces[j]].v[vx1]];
	tf2=vertex_hash[mesh->faces[valid_faces[j]].v[vx2]];
	tf3=vertex_hash[mesh->faces[valid_faces[j]].v[vx3]];
    export_faces_indexes(tf1, tf2, tf3, sizeIndexes, fA3D);
    //fprintf(fTXT, "Face %d : a=%d,  b=%d, c=%d,  sg=%X\n",
	//        j, tf1, tf2, tf3, mesh->faces[valid_faces[j]].smGroup);
	fwrite(&mesh->faces[valid_faces[j]].smGroup, sizeof(int), 1, fA3D);
  }


  // ------------  esportazione dei canali di mapping  ---------------
  fprintf(fTXT, "Numer of mapping channels used: %d\n", mesh->getNumMaps());
  for (i=0; i<mesh->getNumMaps(); i++)
  {
      uv=(UVVert *)NULL;
      hash_uv=(int *)NULL;
      numTEXvert=-1;
	  if (i!=0)
	  {
	     fprintf(fTXT, "Channel %d\n", i);
         // ---------------  scrittura dei vertici texture  ----------
         numTEXvert = mesh->getNumMapVerts(i);  
		 if (numTEXvert <= 0) goto dopo;

         uv=new UVVert[numTEXvert];
         hash_uv=new int[numTEXvert];
         fprintf(fTXT, "Numero di vertici texture (non optimized): %d\n", numTEXvert);
         if (has_known_modifiers==5)
		 {
	         // scrivo i vertici dell'oggetto base (not modified)
	         if (!(triphys = GetTriObjectFromObject(GetBaseObjectFromNode(node), ndelPhys))) return;
             meshphys = &triphys->GetMesh();
             build_valid_texture_vertex(meshphys, i, uv, hash_uv, &numTEXvert);
			 if (ndelPhys) delete triphys;
		 }
         else build_valid_texture_vertex(mesh, i, uv, hash_uv, &numTEXvert);
         
		 fprintf(fTXT, "Numero di vertici texture (optimized): %d\n", numTEXvert);
         if (numTEXvert>0)
		 {
            write_chunk_header(fA3D, TRI_MESH_TEXTURE_VERTEX_LIST_ID, node->GetName(),
		                       4+4+numTEXvert*8);
            fwrite(&i, sizeof(int), 1, fA3D);  // canale di mapping
            fwrite(&numTEXvert, sizeof(int), 1, fA3D);
            for (j=0; j<numTEXvert; j++)
			{
	           fwrite(&uv[j].x, sizeof(float), 1, fA3D);
	           fwrite(&uv[j].y, sizeof(float), 1, fA3D);
			}

            // -------------   scrittura facce texturizzate    --------
            if (numTEXvert < 256) sizeIndexes=8;
            else
            if (numTEXvert < 65536) sizeIndexes=16;
            else sizeIndexes=32;

	        int mat_mod = 1;
	        if (materiale) mat_mod=materiale->NumSubMtls();
	        if (mat_mod==0) mat_mod=1;

            fprintf(fTXT, "Numero di facce texture : %d\n", num_faces);
            // canale + numero facce + size degli indici + facce texture
            write_chunk_header(fA3D, TRI_MESH_TEXTURE_FACES_LIST_ID,
		                       node->GetName(),
							   4+4+4+num_faces*(2+3*sizeIndexes/8));
            fwrite(&i, sizeof(int), 1, fA3D);  // canale di mapping
            fwrite(&num_faces, sizeof(int), 1, fA3D);
			fwrite(&sizeIndexes, sizeof(int), 1, fA3D);

            int g_id_ref;
	        g_id_ref = GetMtl_GLOBAL_ID(materiale);
			// facce texturizzate del canale i-esimo
			TVFace *tvFaces=mesh->mapFaces(i);
	        for (j=0; j<num_faces; j++)
			{
              short int face_g_id;
			  face_g_id=(short int)(g_id_ref + (mesh->faces[valid_faces[j]].getMatID() % mat_mod));
	          tf1=hash_uv[tvFaces[valid_faces[j]].t[vx1]];
	          tf2=hash_uv[tvFaces[valid_faces[j]].t[vx2]];
	          tf3=hash_uv[tvFaces[valid_faces[j]].t[vx3]];
              export_faces_indexes(tf1, tf2, tf3, sizeIndexes, fA3D);
	          fwrite(&face_g_id, sizeof(short int), 1, fA3D);
			}
		 }
	  }
	  else fprintf(fTXT, "Skipping channel 0 (unused)\n");
dopo:
;
      if (uv) delete [] uv;
      if (hash_uv) delete [] hash_uv;
  }
  fprintf(fTXT, "Finito di esportare i canali di mapping\n");


//************************************************************
//            ESPORTAZIONE OGGETTO GEOMETRICO   
//************************************************************
  // esporto come nome della mesh il nome dell'oggetto geometrico
  export_geometric_object(node, SOLID_OBJECT, node->GetName());



//************************************************************
//            ESPORTAZIONE OBJECT SPACE MODIFIER
//************************************************************
  typedef char Bone_Name[40];
  Bone_Name bones_names[70];
  int num_bones_names;
  int assignedBone, numBones;
  short int numBones_short;

  // ESPORTAZIONE TWIST, BEND, etc
  if (has_known_modifiers==3)
  {
     int ModStackIndex;
	 char ordinal;
     Object* ObjectPtr;
     ObjectPtr = node->GetObjectRef();
	 ordinal=0;
     while ((ObjectPtr) && (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID))
	 {
       IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);
       ModStackIndex = 0;
       while (ModStackIndex < DerivedObjectPtr->NumModifiers())
	   {
         Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

		 // TWIST OSM
         if ((ModifierPtr->ClassID() == Class_ID(TWISTOSM_CLASS_ID, 1)) ||
		     (ModifierPtr->ClassID() == Class_ID(TWISTOSM_CLASS_ID, 0)))
		 {
            fprintf(fTXT, "Twist OSM found at order %d...\n", ordinal);
			export_OSM_twist(ModifierPtr, ordinal, node);
            fprintf(fTXT, "OK!\n");
			ordinal++;
		 }
		 else
		 // RIPPLE OSM
		 if (ModifierPtr->ClassID() == Class_ID(SINEWAVE_OMOD_CLASS_ID, 0))
		 {
            fprintf(fTXT, "Ripple OSM found at order %d...\n", ordinal);
			export_OSM_ripple(ModifierPtr, ordinal, node);
            fprintf(fTXT, "OK!\n");
			ordinal++;
		 }
		 else
		 // BEND OSM
		 if (ModifierPtr->ClassID() == Class_ID(BENDOSM_CLASS_ID, 0))
		 {
            fprintf(fTXT, "Bend OSM found at order %d...\n", ordinal);
			export_OSM_bend(ModifierPtr, ordinal, node);
            fprintf(fTXT, "OK!\n");
			ordinal++;
		 }
		 else
		 // MELT OSM
		 if (ModifierPtr->ClassID() == Class_ID(MELT_ID1, MELT_ID2))
		 {
            fprintf(fTXT, "Melt OSM found at order %d...\n", ordinal);
			export_OSM_melt(ModifierPtr, ordinal, node);
            fprintf(fTXT, "OK!\n");
			ordinal++;
		 }
		 else
		 // STRETCH OSM
		 if (ModifierPtr->ClassID() == Class_ID(STRETCHOSM_CLASS_ID1, STRETCHOSM_CLASS_ID2))
		 {
            fprintf(fTXT, "Stretch OSM found at order %d...\n", ordinal);
			export_OSM_stretch(ModifierPtr, ordinal, node);
            fprintf(fTXT, "OK!\n");
			ordinal++;
		 }
		 else
		 // TAPER OSM
		 if (ModifierPtr->ClassID() == Class_ID(TAPEROSM_CLASS_ID, 0))
		 {
            fprintf(fTXT, "Taper OSM found at order %d...\n", ordinal);
			export_OSM_taper(ModifierPtr, ordinal, node);
            fprintf(fTXT, "OK!\n");
			ordinal++;
		 }
		 else
		 // FFD4x4x4, FFD3x3x3, FFD2x2x2
		 if ((ModifierPtr->ClassID() == FFD44_CLASS_ID) ||
			 (ModifierPtr->ClassID() == FFD33_CLASS_ID) ||
			 (ModifierPtr->ClassID() == FFD22_CLASS_ID))
		 {
            fprintf(fTXT, "FFD(irule!) OSM found at order %d...\n", ordinal);
			export_OSM_ffd(ModifierPtr, ordinal, node);
            fprintf(fTXT, "OK!\n");
			ordinal++;
		 }
		 else
		 // NOISE OSM
         if (ModifierPtr->ClassID() == Class_ID(NOISEOSM_CLASS_ID, 0))
		 {
            fprintf(fTXT, "Noise(irule!) OSM found at order %d...\n", ordinal);
			export_OSM_noise(ModifierPtr, ordinal, node);
            fprintf(fTXT, "OK!\n");
			ordinal++;
		 }
         ModStackIndex++;
	   }
       ObjectPtr = DerivedObjectPtr->GetObjRef();
	 }
  }
  else
  // ESPORTAZIONE PHYSIC MODIFIER (if doppio ridondante ma per sicurezza)
  if ((physmod) && (has_known_modifiers==2))
  {
     IPhysiqueExport *phyExport = (IPhysiqueExport *)(physmod->GetInterface(I_PHYINTERFACE));
     if (phyExport)
	 {
       // create a ModContext Export Interface for the specific node of the Physique Modifier
       IPhyContextExport *mcExport = (IPhyContextExport *)phyExport->GetContextInterface(node);
       if (mcExport)
	   {
		 fflush(fA3D);
		 fpos=ftell(fA3D);
         type_phys=0;
         // type_phys + numero_vertici
		 size_phys=4+4;

         write_chunk_header(fA3D, PHYSIC_MODIFIER_ID,
                     node->GetName(),
	                 0);
		 fwrite(&type_phys, sizeof(int), 1, fA3D);
		 type_phys=os.obj->NumPoints();
		 fwrite(&type_phys, sizeof(int), 1, fA3D);

         // we convert all vertices to Rigid
         mcExport->ConvertToRigid(TRUE);
         // compute the transformed Point3 at time t
         for (i = 0; i<os.obj->NumPoints(); i++)
         {
           IPhyVertexExport *vtxExport = mcExport->GetVertexInterface(i);
           if (vtxExport)
           {
             // need to check if vertex has blending
		     if (vtxExport->GetVertexType() & BLENDED_TYPE)
		     {
			    IPhyBlendedRigidVertex *vtxBlend = (IPhyBlendedRigidVertex *)vtxExport;

			    Point3 BlendP(0.0f, 0.0f, 0.0f);
			    for (j=0; j<vtxBlend->GetNumberNodes(); j++)
				{
			       INode *Bone = vtxBlend->GetNode(j);
                   Point3 Offset = vtxBlend->GetOffsetVector(j);
			       float Weight = vtxBlend->GetWeight(j);
        		   BlendP += (Bone->GetNodeTM(0) * Offset) * Weight;
				}
				fprintf(fTXT, "Vertex %d BLENDED_TYPE: %f, %f, %f\n", i, BlendP.x, BlendP.y, BlendP.z);
			    // set the Point of the object (to test the export is correct)
                // os->obj->SetPoint(i, BlendP);
                mcExport->ReleaseVertexInterface(vtxExport);
                vtxExport = NULL;
			 }
		     else
		     {
			    IPhyRigidVertex *vtxNoBlend = (IPhyRigidVertex *)vtxExport;
                INode *Bone = vtxNoBlend->GetNode();
                Point3 Offset = vtxNoBlend->GetOffsetVector();
				fprintf(fTXT, "Vertex %d RIGID: %f, %f, %f %s\n", i, Offset.x, Offset.y, Offset.z, Bone->GetName());
				write_point3(&Offset, fA3D);
			    write_string0(fA3D, Bone->GetName());
				size_phys+=3*sizeof(float)+strlen(Bone->GetName())+1;
			    // set the Point of the object (to test the export is correct)
                // os->obj->SetPoint(i, Bone->GetNodeTM(t) * Offset);
              	mcExport->ReleaseVertexInterface(vtxExport);
               	vtxExport = NULL;
			 }
		   }
		 }
         phyExport->ReleaseContextInterface(mcExport);
		 fflush(fA3D);
		 fseek(fA3D, fpos, SEEK_SET);
         write_chunk_header(fA3D, PHYSIC_MODIFIER_ID,
                     node->GetName(),
	                 size_phys);
		 fflush(fA3D);
		 fseek(fA3D, 0, SEEK_END);
	   }
       physmod->ReleaseInterface(I_PHYINTERFACE, phyExport);
	 }
  }
  else
  // ESPORTAZIONE Skin MODIFIER
  if ((skinmod) && (has_known_modifiers==5))
  {
	ISkin *pSkin = (ISkin*)skinmod->GetInterface(I_SKIN);
	if(pSkin == NULL)
	{
	   fprintf(fTXT, "pSkin NULL!\n");
	   return;
	}
	ISkinContextData *pSkinContext = pSkin->GetContextInterface(node);
	if(pSkinContext == NULL)
	{
	   fprintf(fTXT, "pSkinContext NULL!\n");
	   return;
	}

	fflush(fA3D);
	fpos=ftell(fA3D);
    write_chunk_header(fA3D, PHYSIC_MODIFIER_ID, node->GetName(), 0);

	num_bones_names=pSkin->GetNumBones();
	numBones_short=(short int)num_bones_names;
	fprintf(fTXT, "Num Bones: %d\n", numBones_short);
	fwrite(&numBones_short, sizeof(short int), 1, fA3D);
	size_skin=2;  // num bones
    for(i=0; i<num_bones_names; i++)
	{
	   strcpy(bones_names[i], pSkin->GetBone(i)->GetName());
       write_string0(fA3D, bones_names[i]);
	   fprintf(fTXT, "  Bone Name %d: %s\n", i, bones_names[i]);
	   size_skin+=strlen(bones_names[i])+1; // nome della bone
	}
	fprintf(fTXT, "NumPoints: %d\n", nv_optimised);
	fwrite(&nv_optimised, sizeof(int), 1, fA3D);
    size_skin+=4; // numero_vertici

	for (i=0; i<nv_optimised; i++)
	{
		numBones = pSkinContext->GetNumAssignedBones(vertex_hash[i]);
		numBones_short=0;
		// conto le bonez effettive usate dal vertice i-esimo
        for (j=0; j<numBones; j++)
		{
           assignedBone = pSkinContext->GetAssignedBone(vertex_hash[i], j);
           fprintf(fTXT, "Assigned bone: %d\n", assignedBone); fflush(fTXT);
		   if (assignedBone>=0) numBones_short++;
		}
	    fwrite(&numBones_short, sizeof(short int), 1, fA3D);
		size_skin+=2;
		fprintf(fTXT, "NumBones assigned for vertex %d : %d\n", i, numBones_short);
		// esporto i pesi per il vertic i-esimo
		for (j=0; j<numBones; j++)
		{
			assignedBone = pSkinContext->GetAssignedBone(vertex_hash[i], j);
			if (assignedBone>=0)
			{
			   INode *pBone = pSkin->GetBone(assignedBone);
			   k=0;
			   while ((k<num_bones_names) &&
				      (strcmp(bones_names[k], pBone->GetName())!=0))
			   { k++; }
			   if (k<num_bones_names)
			   {
			     float weight = pSkinContext->GetBoneWeight(vertex_hash[i], j);
                 fprintf(fTXT, "  Bone %d: %s, index %d, weight=%f\n",
				         j, bones_names[k], k, weight); fflush(fTXT);
			     short int short_k=(short int)k;
			     fwrite(&short_k, sizeof(short int), 1, fA3D);
			     fwrite(&weight, sizeof(float), 1, fA3D);
			     size_skin+=2+4;
			   }
			}
		}
	}
	fflush(fA3D);
	fseek(fA3D, fpos, SEEK_SET);
    write_chunk_header(fA3D, SKIN_MODIFIER_ID, node->GetName(), size_skin);
    fflush(fA3D);
	fseek(fA3D, 0, SEEK_END);
	skinmod->ReleaseInterface(I_SKIN, pSkin);
  }

  fprintf(fTXT, "\n\n\n----------------------------------------------------\n");
  delete [] valid_vertex;
  delete [] vertex_hash;
  delete [] valid_faces;
  if (makeADO)
  {
	if ((has_known_modifiers==1) || (has_known_modifiers==4))
      fprintf(fADO, "    static_vertex=%cYES%c;\n  }\n\n", '"', '"');
	else fprintf(fADO, "    static_vertex=%cNO%c;\n  }\n\n", '"', '"');
  }
  fflush(fTXT);
  fflush(fA3D);
}