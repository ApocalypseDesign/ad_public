#include "SceneExport.h"
#include "A3Dfile.h"

int SkeletonExporter::GetSize_of_material_list_chunk (void)
{
   int result=0, i, j;
   Mtl* mtl, *subm;

   for (i=0; i<nmat; i++)
   {
	  mtl=materiali[i].m;
      if (mtl->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
	  {
         for (j=0; j<mtl->NumSubMtls(); j++)
		 {
           subm=mtl->GetSubMtl(j);
		   // aggiungo l'id + lunghezza nome + '\0'
		   result+=sizeof(short int);
		   result+=(1 + strlen(subm->GetName()));
		 }
	  }
	  else
	  {
		// aggiungo l'id + lunghezza nome + '\0'
		result+=sizeof(short int);
		result+=(1 + strlen(mtl->GetName()));
	  }
   }

   // aggiungo l'id negativo che indica fine lista
   result+=sizeof(short int);
   //fprintf(fTXT, "Size del chunk lista materiali = %d\n", result);
   return(result);
}

//--------------------------------------------------------------------

int	SkeletonExporter::GetMtlID(Mtl* mtl)
{
  for (int k=0; k<nmat; k++)
	if (materiali[k].m==mtl) return(k);
  return(-1);
}

//--------------------------------------------------------------------

int	SkeletonExporter::GetMtl_GLOBAL_ID(Mtl* mtl)
{
  int g_id=0, subm, k;

  for (k=0; k<nmat; k++)
  {
	if (materiali[k].m==mtl) return(g_id);
	subm=materiali[k].m->NumSubMtls();
	if (subm>0)	g_id=g_id+subm;
	else g_id=g_id+1;
  }
  return(-1);
}

//--------------------------------------------------------------------

void SkeletonExporter::collect_materials(INode *node)
{
  Mtl *mm;
  int w, flag;

  mm=node->GetMtl();
  flag=0;

  if (mm)
  {
	for (w=0; w<nmat; w++)
	{
	  if (materiali[w].m==mm) flag=1;
	}
    if (flag==0)  // materiale non ancora inserito
	{
      materiali[nmat].m=mm;
	  strcpy(materiali[nmat].name, mm->GetName());
	  nmat++;
	}
  }

  // For each child of this node, we recurse into ourselves 
  // and increment the counter until no more children are found.
  for (int c = 0; c < node->NumberOfChildren(); c++)
  {
 	collect_materials(node->GetChildNode(c));
  }
}

//--------------------------------------------------------------------


void SkeletonExporter::dump_material(Mtl *mtl)
{
  Color col;
  int i, j;
  float amount;
  TSTR className;
  #define ACTIVE(a) \
    ((mtl->GetSubTexmap(a)) && (stdm->MapEnabled(a))) \

  #define COMPOSITE(a) \
    ((mtl->GetSubTexmap(a)->ClassID() == Class_ID(COMPOSITE_CLASS_ID, 0))) \


  if (!mtl) return;
  mtl->GetClassName(className);
  fprintf(fTXT, "Material Class Name: %s\n", className);
  fprintf(fTXT, "Material ClassID: (%X, %X)\n",
	      mtl->ClassID().PartA(), mtl->ClassID().PartB());

  // se in realta' il materiale e' un Multi/Sub_Object, ovvero
  // e' un materiale che contiene sottomateriali non lo devo
  // dumpare (nome, mappe, ecc), ma devo dumpare i suoi
  // sottomateriali; su file (script, ascii, binario) scrivo
  // solo quando raggiungo materiali standard !!!
  if (mtl->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
  {
     fprintf(fTXT, "Materiale composto, numero sotto-materiali : %d\n", mtl->NumSubMtls());
     for (j=0; j<mtl->NumSubMtls(); j++)
	   if (mtl->GetSubMtl(j)) dump_material(mtl->GetSubMtl(j));
  }
  else if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
  {
    // materiale standard : dumpo tutte le info	
	fprintf(fTXT, "Material Global ID = %d\n", global_id);
	global_id++;

    fprintf(fTXT, "Material name : %s \n", mtl->GetName());
    if (makeADM) fprintf(fADM, "  material %c%s%c \n  {\n", '"', mtl->GetName(), '"');

	StdMat* stdm = (StdMat*)mtl;
	fprintf(fTXT,"Opacity : %d%c \n", (int)(100*stdm->GetOpacity(0)), '%');

	if (stdm->GetTwoSided()) fprintf(fTXT, "Two sided : yes \n"); 
	else fprintf(fTXT, "Two sided : no \n"); 

	if (stdm->GetWire()) fprintf(fTXT, "Wireframe : yes");

    if (makeADM)
	{	
      if (ACTIVE(ID_DI))
	  {
		 if (COMPOSITE(ID_DI))
		 {
			Texmap *DI=mtl->GetSubTexmap(ID_DI);
	        fprintf(fADM, "    map1=%c%d%c;\n", '"',
		         GetTexID(DI->GetSubTexmap(0)), '"');
		    dump_map(DI->GetSubTexmap(0), 1, ID_DI);
            dump_map_tracks(DI->GetSubTexmap(0), 1, mtl->GetName());

	        fprintf(fADM, "    map2=%c%d%c;\n", '"',
		         GetTexID(DI->GetSubTexmap(1)), '"');
		    dump_map(DI->GetSubTexmap(1), 2, ID_DI);
            dump_map_tracks(DI->GetSubTexmap(1), 2, mtl->GetName());

	        fprintf(fADM, "    mapsmixtype=%cMODULATE%c;\n", '"', '"');
		 }
		 else
		 // una sola texture o texture+envmap
		 {
			// texyure + envmap
			if ((ACTIVE(ID_RL)) && (!COMPOSITE(ID_RL)))
			{
	          fprintf(fADM, "    map1=%c%d%c;\n", '"',
		              GetTexID(mtl->GetSubTexmap(ID_DI)), '"');
		      dump_map(mtl->GetSubTexmap(ID_DI), 1, ID_DI);
			  dump_map_tracks(mtl->GetSubTexmap(ID_DI), 1, mtl->GetName());

	          fprintf(fADM, "    map2=%c%d%c;\n", '"',
		              GetTexID(mtl->GetSubTexmap(ID_RL)), '"');
		      dump_map(mtl->GetSubTexmap(ID_RL), 2, ID_RL);
			  dump_map_tracks(mtl->GetSubTexmap(ID_RL), 2, mtl->GetName());

	          fprintf(fADM, "    mapsmixtype=%cMODULATE%c;\n", '"', '"');
			}
			// solo una texture diffusa
			else
			{
	          fprintf(fADM, "    map1=%c%d%c;\n", '"',
		              GetTexID(mtl->GetSubTexmap(ID_DI)), '"');
		      dump_map(mtl->GetSubTexmap(ID_DI), 1, ID_DI);
              dump_map_tracks(mtl->GetSubTexmap(ID_DI), 1, mtl->GetName());
			}
		 }
	  }
	  else
	  // solo envamp (1 o 2)
	  if (ACTIVE(ID_RL))
	  {
		 // 2 envmap
		 if (COMPOSITE(ID_RL))
		 {
			Texmap *RL=mtl->GetSubTexmap(ID_RL);
	        fprintf(fADM, "    map1=%c%d%c;\n", '"',
		            GetTexID(RL->GetSubTexmap(0)), '"');
		    dump_map(RL->GetSubTexmap(0), 1, ID_RL);
			dump_map_tracks(RL->GetSubTexmap(0), 1, mtl->GetName());

	        fprintf(fADM, "    map2=%c%d%c;\n", '"',
		            GetTexID(RL->GetSubTexmap(1)), '"');
		    dump_map(RL->GetSubTexmap(1), 2, ID_RL);
			dump_map_tracks(RL->GetSubTexmap(1), 2, mtl->GetName());

	        fprintf(fADM, "    mapsmixtype=%cMODULATE%c;\n", '"', '"');
		 }
		 // una sola envmap
		 else
		 {
	        fprintf(fADM, "    map1=%c%d%c;\n", '"',
		            GetTexID(mtl->GetSubTexmap(ID_RL)), '"');
		    dump_map(mtl->GetSubTexmap(ID_RL), 1, ID_RL);
            dump_map_tracks(mtl->GetSubTexmap(ID_RL), 1, mtl->GetName());
		 }
	  }

	  // se c'e' mappa di autoilluminazione metto NONE come
	  // fattore di mixing nelle luci
	  if (stdm->MapEnabled(ID_SI))
	     fprintf(fADM, "    lightmixtype=%cNONE%c;\n", '"', '"');
	  else
	     fprintf(fADM, "    lightmixtype=%cMODULATE%c;\n", '"', '"');
      if (stdm->GetTwoSided())
          fprintf(fADM, "    backfaceculling=%cNONE%c;\n", '"', '"');
	  else
          fprintf(fADM, "    backfaceculling=%cBACKFACE%c;\n", '"', '"');
	  if (stdm->GetWire())
         fprintf(fADM, "    wireframe=%cON%c;\n", '"', '"');

      fprintf(fADM, "    specular=%cOFF%c;\n", '"', '"');

	  // scrivo una trasparenza numerica solo se il materiale non
	  // ha mappa di trasparenza e ha una opacità non massima
	  if ( ((stdm->GetOpacity(0)<1) && (!stdm->MapEnabled(ID_OP))) ||
		    (material_has_transparency_track(mtl)))
	  {
         if (stdm->GetTransparencyType()==TRANSP_ADDITIVE)
		     fprintf(fADM, "    transparency_type=%cADD%c;\n", '"', '"');
		 else
         if (stdm->GetTransparencyType()==TRANSP_SUBTRACTIVE)
		     fprintf(fADM, "    transparency_type=%cSUB%c;\n", '"', '"');
		     fprintf(fADM, "    transparency_type=%cALPHA%c;\n", '"', '"');
	     fprintf(fADM, "    transparency_amount=%c%d%c;\n", '"', (int)(255*stdm->GetOpacity(0)), '"');
	  }
	  else
      if (stdm->MapEnabled(ID_OP))
	  {
         fprintf(fADM, "    transparency_type=%c%d%c;\n", '"', 
			     GetTexID(mtl->GetSubTexmap(ID_OP)), '"');
	  }

	  // per l'engio raytrazza
	  if (makeRAY)
	  {
	    col=mtl->GetDiffuse();
	    fprintf(fADM, "    diffuse_r=%c%d%c;\n", '"', (int)(col.r*255), '"');
	    fprintf(fADM, "    diffuse_g=%c%d%c;\n", '"', (int)(col.g*255), '"');
	    fprintf(fADM, "    diffuse_b=%c%d%c;\n", '"', (int)(col.b*255), '"');
	    fprintf(fADM, "    diffuse_kd=%c1.0%c;\n", '"', '"');
	    col=mtl->GetSpecular();
	    fprintf(fADM, "    specular_r=%c%d%c;\n", '"', (int)(col.r*255), '"');
	    fprintf(fADM, "    specular_g=%c%d%c;\n", '"', (int)(col.g*255), '"');
	    fprintf(fADM, "    specular_b=%c%d%c;\n", '"', (int)(col.b*255), '"');
	    fprintf(fADM, "    specular_ks=%c1.0%c;\n", '"', '"');
	    fprintf(fADM, "    refraction_index=%c%f%c;\n", '"', stdm->GetIOR(0), '"');
	    fprintf(fADM, "    glossiness=%c%f%c;\n", '"', stdm->GetShininess(0), '"');
	  }
      fprintf(fADM, "    next_material=%cNONE%c;\n", '"', '"');
      fprintf(fADM, "  }\n\n");
	}

	for (i=0; i<mtl->NumSubTexmaps(); i++)
	{
      Texmap *subTex = mtl->GetSubTexmap(i);
      if ((subTex) && (stdm->MapEnabled(i)))
	  {
	     amount=stdm->GetTexmapAmt(i, 0);
		 // tipo di mapping: textue o environment
		 if (stdm->MapSlotType(i) == MAPSLOT_TEXTURE)
		     fprintf(fTXT, "Mapping Type required: texture mapping\n");
		 else fprintf(fTXT, "Mapping Type required: environment mapping\n");
	     //dump_texture(subTex, stdm->MapSlotType(i), i);
	  }
	}
  }
  fprintf(fTXT, "\n----------------------------------------------------\n\n");
}


void SkeletonExporter::export_material_chunk(Mtl *mtl)
{
  int j;

  if (!mtl) return;
  if (mtl->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
  {
     fprintf(fTXT, "Materiale composto, numero sotto-materiali : %d\n", mtl->NumSubMtls());
     for (j=0; j<mtl->NumSubMtls(); j++)
	   if (mtl->GetSubMtl(j)) export_material_chunk(mtl->GetSubMtl(j));
  }
  else if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
  {
    // scrivo nella lista dei materiali la coppia
	// (global_id, nome)
	fwrite(&global_id, sizeof(short int), 1, fA3D);
	write_string0(fA3D, mtl->GetName());

    // materiale standard : dumpo tutte le info	
	fprintf(fTXT, "Material Global ID = %d\n", global_id);
	global_id++;
  }
}

//--------------------------------------------------------------------

void SkeletonExporter::export_materials(void)
{
  short int meno1=-1;
  int size;
  int i;

  nmat=0;
  collect_materials(ip->GetRootNode());
  ntex=0;
  for (i=0; i<nmat; i++) collect_bitmaps(materiali[i].m);
  for (i=0; i<ntex; i++) dump_bitmap(&textures[i]);

  fprintf(fTXT, "\n\n");
  fprintf(fTXT, "Material list \n");
  fprintf(fTXT, "Numero di materiali : %d\n", nmat);
  fprintf(fTXT, "\n----------------------------------------------------\n\n");

  size=GetSize_of_material_list_chunk();
  write_chunk_header(fA3D, SCENE_MATERIAL_LIST_ID, "Scene Root", size);
  global_id=0;
  for (i=0; i<nmat; i++) 
	export_material_chunk(materiali[i].m);
  // segnalo la fine del chunk lista materiali mettendo un
  // id negativo
  fwrite(&meno1, sizeof(short int), 1, fA3D);

  for (i=0; i<nmat; i++) 
 	dump_material(materiali[i].m);

  for (i=0; i<nmat; i++) 
 	dump_material_tracks(materiali[i].m);
}