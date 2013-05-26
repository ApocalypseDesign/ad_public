#include "SceneExport.h"
#include "A3Dfile.h"
#include "OSMs.h"


void SkeletonExporter::export_geometric_object(INode *node, int type,
											   char *meshname)
{
  int sf, sm, mf;
  INode *padre;
  Mtl *materiale;
  Point3 row;
  Matrix3 mat;
  Control *c;
  int size_key;
  ObjectState os;

  if (!node) return;
  os=node->EvalWorldState(0);

  padre=node->GetParentNode();
  materiale=node->GetMtl();

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
    sf=strlen(padre->GetName())+1;
  else sf=0;

  if (materiale) sm=strlen(materiale->GetName())+1;
  else sm=0;

  if (meshname==NULL) mf=0;
  else mf=strlen(meshname)+1;

  
  // tipo + flag padre + nome padre + flag materiale + nome materiale +
  // + pivot + matrice di trasformazione per t=0 +
  // bbox.min + bbox.max + flag mesh + nome mesh
  write_chunk_header(fA3D, GEOMETRIC_OBJECT_ID, node->GetName(),
	                 4+4+sf+4+sm+12+48+12+12+4+mf);
  if (makeADO) fprintf(fADO, "  object %c%s%c \n  {\n", '"', node->GetName(), '"');
  
  // --------------------   scrittura del tipo   ---------------------
  fwrite(&type, sizeof(int), 1, fA3D);
  if (makeADO)
  {
     fprintf(fADO, "    type=%c", '"');
	 switch (type)
	 {
       case DUMMY_HELPER: fprintf(fADO, "DUMMY%c;\n", '"');
                          break;
       case POINT_HELPER: fprintf(fADO, "POINT%c;\n", '"');
                          break;
       case BOXGIZMO_HELPER: fprintf(fADO, "BOXGIZMO%c;\n", '"');
                             break;
       case SPHEREGIZMO_HELPER: fprintf(fADO, "SPHEREGIZMO%c;\n", '"');
                                break;
       case SOLID_OBJECT: fprintf(fADO, "SOLID%c;\n", '"');
                          break;
       case BONE_OBJECT: fprintf(fADO, "BONE%c;\n", '"');
                         break;
       case UNKNOWN_GEOMETRIC_OBJECT:
                                      fprintf(fADO, "DUMMY%c;\n", '"');
                                      break;
	 }
  }

  // -----------    scrivo il padre (flag, nome)   ------------------
  fwrite(&sf, sizeof(int), 1, fA3D);
  if (sf>0)
  {
	 write_string0(fA3D, padre->GetName());
	 if (makeADO) fprintf(fADO, "    father=%c%s%c;\n", '"', padre->GetName(), '"');
  }
  else
	 if (makeADO) fprintf(fADO, "    father=%cNONE%c;\n", '"', '"');

  // ---------   scrivo il materiale di base (flag, nome)   ----------
  fwrite(&sm, sizeof(int), 1, fA3D);
  if (sm>0)
  {
     write_string0(fA3D, materiale->GetName());
	 if (makeADO) fprintf(fADO, "    material=%c%s%c;\n", '"', materiale->GetName(), '"');
  }
  else
	 if (makeADO) fprintf(fADO, "    material=%cNONE%c;\n", '"', '"');

  // stesse informazioni a livello testuale
  fprintf(fTXT, "Geometric object found\n");
  if (materiale) fprintf(fTXT, "Material used : %s\n", node->GetMtl()->GetName());

  // ---------------   scrittura del punto di pivot   ----------------
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  write_point3(&row, fA3D);

  // -------------------   scrittura matrice   -----------------------
  mat = node->GetNodeTM(0);
  write_matrix(&mat, fA3D);
  
  // -----------------   scrittura bounding box   --------------------
  Box3 bbox;
  os.obj->GetDeformBBox(0, bbox, NULL, FALSE);
  write_bbox(&bbox, fA3D);

  // ---------   scrittura dell'eventuale nome della mesh   ----------
  fwrite(&mf, sizeof(int), 1, fA3D);
  if (mf>0)
  {
    write_string0(fA3D, meshname);
    if (makeADO) fprintf(fADO, "    lod=%c%s, 0, 99999.99%c;\n", '"', meshname, '"');
  }


  //------------------------------------------------------------------
  //                      ESPORTAZIONE KEYFRAMER
  //------------------------------------------------------------------

  // --------------------  HIDING CONTROLLER  ----------------------
  c=node->GetVisController();
  if ((c) && (c->NumKeys()>0) &&
	  (c->ClassID()==Class_ID(0x984b8d27,0x938f3e43)))
  {
	 size_key=4;
	 fprintf(fTXT, "Hiding track present.");
     write_chunk_header(fA3D, HIDING_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_hide_track(c, 1, fA3D);
  }

  c=node->GetTMController();
  if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
      (c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
      (c->ClassID() == FOOTPRINT_CLASS_ID))
  {
       export_bipedtracks(node);
	   return;
  }
  
  // NB: per gli oggetti mesh e quant'altre tipologie di
  // oggetti che possono essere linkati (ovvero dove e'
  // possibile implmenetare gerarchie), ad esempio patch
  // di Bezier, esporto SEMPRE una key di posizione, una
  // di rotazione ed una di scaling
  
  // esportiamo l'animazione della posizione dell'oggetto
  // se ci sono un numero di key > 0


  // --------------------  POSITION CONTROLLER  ----------------------
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object position track present.");
     write_chunk_header(fA3D, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fA3D);
  } // esportiamo comunque una key di posizione TCB
  else
  {
	fprintf(fTXT, "Object position track present. (1 key case)");
	size_key=36;
    write_chunk_header(fA3D, POSITION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_point3_track(c, 1, fA3D);
  }
  fprintf(fTXT, "position controller exporting done\n");

  // --------------------  ROTATION CONTROLLER  ----------------------
  c=node->GetTMController()->GetRotationController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=40;
	 else size_key=20;
	 fprintf(fTXT, "Object rotation track present.");
     write_chunk_header(fA3D, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fA3D);
  } // esportiamo comunque una key di rotazione TCB
  else
  {
	fprintf(fTXT, "Object rotation track present. (1 key case)");
	size_key=40;
    write_chunk_header(fA3D, ROTATION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_rot_track(c, fA3D);
  }
  
  // ---------------------  SCALE CONTROLLER  ------------------------
  c=node->GetTMController()->GetScaleController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object scaling track present.");
     write_chunk_header(fA3D, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fA3D);
  } // esportiamo comunque una key di scaling TCB
  else
  {
	fprintf(fTXT, "Object scaling track present. (1 key case)");
	size_key=36;
    write_chunk_header(fA3D, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fA3D);
  }
}