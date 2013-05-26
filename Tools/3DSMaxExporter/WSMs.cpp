#include "SceneExport.h"
#include "extraSDK.h"
#include "A3Dfile.h"

// ***********************************************************
// ******************       VENTO      ***********************
// ***********************************************************
void SkeletonExporter::export_WSM_wind(INode *node)
{
  Point3 row;
  Matrix3 mat;
  Control *c;
  int size_key;
  char sval[50];

  ObjectState os = node->EvalWorldState(0);
  if (!os.obj) return;

  INode *padre=node->GetParentNode();
  int sf;

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
  sf=strlen(padre->GetName())+1;
  else sf=0;
  
  fprintf(fTXT, "Wind space modifier found\n");
  fprintf(fTXT, "Name : %s\n", node->GetName());
  // flag padre + nome padre + pivot + matrice(t=0) +
  // strenght + decay + mapping + turbolence + frequency + scale
  write_chunk_header(fA3D, WIND_MODIFIER_ID, node->GetName(),
	                 4+sf+12+48+4*6);
  fwrite(&sf, sizeof(int), 1, fA3D);
  if (sf>0) write_string0(fA3D, padre->GetName());

  // ---------------   scrittura del punto di pivot   ----------------
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  write_point3(&row, fA3D);

  // -------------------   scrittura matrice   -----------------------
  mat = node->GetNodeTM(0);
  write_matrix(&mat, fA3D);

  float turb, decay, stren, freq, scale;
  int type=0;
  SimpleWSMObject2 *obj=(SimpleWSMObject2 *)os.obj;
  fflush(fTXT);

  export_animatable(obj->pblock2, 5);

  // -----------------------   strenght   -----------------------
  c=obj->pblock2->GetController(WPB_STRENGTH);
  if (c) c->GetValue(0, &stren, FOREVER);
  else obj->pblock2->GetValue(WPB_STRENGTH, 0, stren, FOREVER);
  fprintf(fTXT, "Strenght: %f\n", stren);   fflush(fTXT);
  
  // -----------------------   decay   -----------------------

  c=obj->pblock2->GetController(WPB_DECAY);
  if (c) c->GetValue(0, &decay, FOREVER);
  else obj->pblock2->GetValue(WPB_DECAY, 0, decay, FOREVER);
  fprintf(fTXT, "Decay: %f\n", decay);   fflush(fTXT);

  // -----------------------   type   -----------------------
  //obj->pblock2->GetValue(WPB_TYPE, 0, type, FOREVER);
  //obj->pblock2->GetController(WPB_TYPE)->GetValue(0, &type, FOREVER);
  type=0;
  fprintf(fTXT, "Type: %d\n", type);   fflush(fTXT);

  // -----------------------   turbulence   -----------------------
  c=obj->pblock2->GetController(WPB_TURBULENCE);
  if (c) c->GetValue(0, &turb, FOREVER);
  else obj->pblock2->GetValue(WPB_TURBULENCE, 0, turb, FOREVER);
  fprintf(fTXT, "Turbulence: %f\n", turb);   fflush(fTXT);

  // -----------------------   frequency   -----------------------
  c=obj->pblock2->GetController(WPB_FREQUENCY);
  if (c) c->GetValue(0, &freq, FOREVER);
  else obj->pblock2->GetValue(WPB_FREQUENCY, 0, freq, FOREVER);
  fprintf(fTXT, "Frequency: %f\n", freq);   fflush(fTXT);

  // -----------------------   scale   -----------------------
  c=obj->pblock2->GetController(WPB_SCALE);
  if (c) c->GetValue(0, &scale, FOREVER);
  else obj->pblock2->GetValue(WPB_SCALE, 0, scale, FOREVER);
  fprintf(fTXT, "Scale: %f\n", scale); fflush(fTXT);

  fwrite(&stren, sizeof(float), 1, fA3D);
  fwrite(&decay, sizeof(float), 1, fA3D);
  fwrite(&type, sizeof(int), 1, fA3D);
  fwrite(&turb, sizeof(float), 1, fA3D);
  fwrite(&freq, sizeof(float), 1, fA3D);
  fwrite(&scale, sizeof(float), 1, fA3D);


  //------------------------
  // ESPORTAZIONE KEYFRAMER
  //------------------------

  // POSITION CONTROLLER
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object position track present.\n");
     write_chunk_header(fA3D, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fA3D);
  }
  else
  {
	fprintf(fTXT, "Object position track present. (1 key case)\n");
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
	 fprintf(fTXT, "Object rotation track present.\n");
     write_chunk_header(fA3D, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fA3D);
  }
  else
  {
	fprintf(fTXT, "Object rotation track present. (1 key case)\n");
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
	 fprintf(fTXT, "Wind scaling track present.\n");
     write_chunk_header(fA3D, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fA3D);
  }
  else
  {
	fprintf(fTXT, "Wind scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fA3D, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fA3D);
  }

  // wind strength keyframer
  c=obj->pblock2->GetController(WPB_STRENGTH);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Wind strength track present.\n");
     write_chunk_header(fA3D, WIND_STRENGTH_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  // wind decay keyframer
  c=obj->pblock2->GetController(WPB_DECAY);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Wind decay track present.\n");
     write_chunk_header(fA3D, WIND_DECAY_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  // wind turbolence keyframer
  c=obj->pblock2->GetController(WPB_TURBULENCE);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Wind turbolence track present.\n");
     write_chunk_header(fA3D, WIND_TURBOLENCE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  // wind frequency keyframer
  c=obj->pblock2->GetController(WPB_FREQUENCY);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Wind frequency track present.\n");
     write_chunk_header(fA3D, WIND_FREQUENCY_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  // wind scale keyframer
  c=obj->pblock2->GetController(WPB_SCALE);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Wind scale(parameter) track present.\n");
     write_chunk_header(fA3D, WIND_SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  if (makeADP)
  {
    fprintf(fADP, "  windWSM %c%s%c\n  {\n", '"', node->GetName(), '"');
	if (type==0)
      fprintf(fADP, "     type=%c%PLANAR%c;\n", '"', '"');
	else
    fprintf(fADP, "     type=%c%SPHERICAL%c;\n", '"', '"');
    my_ftoa(stren, sval);
    fprintf(fADP, "     strength=%c%s%c;\n", '"', sval, '"');
    my_ftoa(decay, sval);
    fprintf(fADP, "     decay=%c%s%c;\n", '"', sval, '"');
    my_ftoa(turb, sval);
    fprintf(fADP, "     turbolence=%c%s%c;\n", '"', sval, '"');
    my_ftoa(freq, sval);
    fprintf(fADP, "     frequency=%c%s%c;\n", '"', sval, '"');
    my_ftoa(scale, sval);
    fprintf(fADP, "     scale=%c%s%c;\n", '"', sval, '"');
	fprintf(fADP, "  }\n\n");
  }
  fprintf(fTXT, "\n\n");
  fflush(fTXT);
  if (makeADP) fflush(fADP);
}



// ***********************************************************
// ******************      GRAVITA'    ***********************
// ***********************************************************
void SkeletonExporter::export_WSM_gravity(INode *node)
{
  Matrix3 mat;
  Point3 row;
  float decay, stren;
  int type;
  char sval[50];

  ObjectState os = node->EvalWorldState(0);
  if (!os.obj) return;

  mat = node->GetNodeTM(0);

  INode *padre=node->GetParentNode();
  int sf;

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
  sf=strlen(padre->GetName())+1;
  else sf=0;

  fprintf(fTXT, "Gravity space modifier found\n");
  fprintf(fTXT, "Name : %s\n", node->GetName());
  // flag padre + nome padre + strenght + decay + mapping +
  // world position
  write_chunk_header(fA3D, GRAVITY_MODIFIER_ID, node->GetName(),
	                 4+sf+12+48+3*4);
  fwrite(&sf, sizeof(int), 1, fA3D);
  if (sf>0) write_string0(fA3D, padre->GetName());

  // ---------------   scrittura del punto di pivot   ----------------
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  write_point3(&row, fA3D);

  // -------------------   scrittura matrice   -----------------------
  mat = node->GetNodeTM(0);
  write_matrix(&mat, fA3D);

  SimpleWSMObject2 *obj=(SimpleWSMObject2 *)os.obj;

  obj->pblock2->GetValue(GPB_STRENGTH, 0, stren, FOREVER);
  obj->pblock2->GetValue(GPB_DECAY, 0, decay, FOREVER);
  //obj->pblock2->GetValue(GPB_TYPE, 0, type, FOREVER);
  type=0;

  fprintf(fTXT, "Strength: %f\n", stren);
  fprintf(fTXT, "Decay: %f\n", decay);
  fprintf(fTXT, "Type: %d\n", type);

  fwrite(&stren, sizeof(float), 1, fA3D);
  fwrite(&decay, sizeof(float), 1, fA3D);
  fwrite(&type, sizeof(int), 1, fA3D);

  //------------------------
  // ESPORTAZIONE KEYFRAMER
  //------------------------
  Control *c;
  int size_key;

  // POSITION CONTROLLER
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object position track present.\n");
     write_chunk_header(fA3D, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fA3D);
  }
  else
  {
	fprintf(fTXT, "Object position track present. (1 key case)\n");
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
	 fprintf(fTXT, "Object rotation track present.\n");
     write_chunk_header(fA3D, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fA3D);
  }
  else
  {
	fprintf(fTXT, "Object rotation track present. (1 key case)\n");
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
	 fprintf(fTXT, "Gravity scaling track present.\n");
     write_chunk_header(fA3D, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fA3D);
  }
  else
  {
	fprintf(fTXT, "Gravity scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fA3D, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fA3D);
  }

  // gravity strength keyframer
  c=obj->pblock2->GetController(GPB_STRENGTH);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Gravity strength track present.\n");
     write_chunk_header(fA3D, GRAVITY_STRENGTH_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  // gravity decay keyframer
  c=obj->pblock2->GetController(GPB_DECAY);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Gravity decay track present.\n");
     write_chunk_header(fA3D, GRAVITY_DECAY_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  if (makeADP)
  {
    fprintf(fADP, "  gravityWSM %c%s%c\n  {\n", '"', node->GetName(), '"');
	if (type==0)
      fprintf(fADP, "     type=%c%PLANAR%c;\n", '"', '"');
	else
    fprintf(fADP, "     type=%c%SPHERICAL%c;\n", '"', '"');
	my_ftoa(stren, sval);
    fprintf(fADP, "     strength=%c%s%c;\n", '"', sval, '"');
    my_ftoa(decay, sval);
    fprintf(fADP, "     decay=%c%s%c;\n", '"', sval, '"');
	fprintf(fADP, "  }\n\n");
  }
  fprintf(fTXT, "\n\n");
}




// ***********************************************************
// ******************       BOMBA      ***********************
// ***********************************************************
void SkeletonExporter::export_WSM_bomb(INode *node)
{
}