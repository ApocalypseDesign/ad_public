// Entita' per l'engine raytracing
#include "SceneExport.h"
#include "A3Dfile.h"
#include "simpobj.h"

#define PB_SPHERE_RADIUS	0

void SkeletonExporter::export_ray_sphere(INode *node)
{
  ObjectState os = node->EvalWorldState(0);
  if (!os.obj) return;
	
  INode *padre=node->GetParentNode();
  Mtl *materiale=node->GetMtl();
  int sf;
  int sm;
  SimpleObject *so;
  float radius;
  Point3 row;

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
  sf=strlen(padre->GetName())+1;
  else sf=0;
  if (materiale) sm=strlen(materiale->GetName())+1;
  else sm=0;
  
  // flag padre + nome padre + flag materiale + nome materiale +
  // + matrice di trasformazione per t=0 + world position +
  // + pivot + raggio
  write_chunk_header(fRAY, RAY_SPHERE_ID, node->GetName(),
	                 4+sf+4+sm+36+12+12+4);

  // scrivo il padre (flag, nome)
  fwrite(&sf, sizeof(int), 1, fRAY);
  if (sf>0) write_string0(fRAY, padre->GetName());

  // scrivo il materiale di base (flag, nome)
  fwrite(&sm, sizeof(int), 1, fRAY);
  if (sm>0) write_string0(fRAY, materiale->GetName());

  Matrix3 mat = node->GetNodeTM(0);
  
  // scrittura riga 0 della matrice locale (local coordinate system)
  row = mat.GetRow(0);
  fprintf(fTXT, "Local coord. system row0 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura riga 1 della matrice locale (local coordinate system)
  row = mat.GetRow(1);
  fprintf(fTXT, "Local coord. system row1 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura riga 2 della matrice locale (local coordinate system)
  row = mat.GetRow(2);
  fprintf(fTXT, "Local coord. system row2 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura posizione nel mondo (riga 3)
  row = mat.GetRow(3);
  fprintf(fTXT, "World position : x=%f, y=%f, z=%f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);
  
  // scrittura del punto di pivot
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // raggio al tempo 0
  so=(SimpleObject *)os.obj;
  so->pblock->GetValue(PB_SPHERE_RADIUS, 0, radius, FOREVER);
  fwrite(&radius, sizeof(float), 1, fRAY);

  //------------------------
  // ESPORTAZIONE KEYFRAMER
  //------------------------
  Control *c;
  int size_key;

  // NB: per gli oggetti mesh e quant'altre tipologie di
  // oggetti che possono essere linkati (ovvero dove e'
  // possibile implmenetare gerarchie), ad esempio patch
  // di Bezier, esporto SEMPRE una key di posizione, una
  // di rotazione ed una di scaling

  // POSITION CONTROLLER
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object position track present.\n");
     write_chunk_header(fRAY, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fRAY);
  }
  // esportiamo comunque una key di posizione TCB
  else
  {
	fprintf(fTXT, "Object position track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fRAY, POSITION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_point3_track(c, 1, fRAY);
  }



  // ROTATION CONTROLLER
  c=node->GetTMController()->GetRotationController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=40;
	 else size_key=20;
	 fprintf(fTXT, "Object rotation track present.\n");
     write_chunk_header(fRAY, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fRAY);
  }
  // esportiamo comunque una key di rotazione TCB
  else
  {
	fprintf(fTXT, "Object rotation track present. (1 key case)\n");
	size_key=40;
    write_chunk_header(fRAY, ROTATION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_rot_track(c, fRAY);
  }


  // SCALE CONTROLLER
  c=node->GetTMController()->GetScaleController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object scaling track present.\n");
     write_chunk_header(fRAY, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fRAY);
  }
  // esportiamo comunque una key di scaling TCB
  else
  {
	fprintf(fTXT, "Object scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fRAY, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fRAY);
  }
}


// ******************************************************************
// ******************************************************************
// ******************************************************************

#define PB_BOX_LENGTH	0
#define PB_BOX_WIDTH	1
#define PB_BOX_HEIGHT	2

void SkeletonExporter::export_ray_box(INode *node)
{
  ObjectState os = node->EvalWorldState(0);
  if (!os.obj) return;
	
  INode *padre=node->GetParentNode();
  Mtl *materiale=node->GetMtl();
  int sf;
  int sm;
  SimpleObject *so;
  float dim123;
  Point3 row;

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
  sf=strlen(padre->GetName())+1;
  else sf=0;
  if (materiale) sm=strlen(materiale->GetName())+1;
  else sm=0;
  
  // flag padre + nome padre + flag materiale + nome materiale +
  // + matrice di trasformazione per t=0 + world position +
  // + pivot + larghezza + altezza + profondita'
  write_chunk_header(fRAY, RAY_BOX_ID, node->GetName(),
	                 4+sf+4+sm+36+12+12+4+4+4);

  // scrivo il padre (flag, nome)
  fwrite(&sf, sizeof(int), 1, fRAY);
  if (sf>0) write_string0(fRAY, padre->GetName());

  // scrivo il materiale di base (flag, nome)
  fwrite(&sm, sizeof(int), 1, fRAY);
  if (sm>0) write_string0(fRAY, materiale->GetName());

  Matrix3 mat = node->GetNodeTM(0);
  
  // scrittura riga 0 della matrice locale (local coordinate system)
  row = mat.GetRow(0);
  fprintf(fTXT, "Local coord. system row0 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura riga 1 della matrice locale (local coordinate system)
  row = mat.GetRow(1);
  fprintf(fTXT, "Local coord. system row1 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura riga 2 della matrice locale (local coordinate system)
  row = mat.GetRow(2);
  fprintf(fTXT, "Local coord. system row2 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura posizione nel mondo (riga 3)
  row = mat.GetRow(3);
  fprintf(fTXT, "World position : x=%f, y=%f, z=%f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);
  
  // scrittura del punto di pivot
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // raggio al tempo 0
  so=(SimpleObject *)os.obj;
  so->pblock->GetValue(PB_BOX_LENGTH, 0, dim123, FOREVER);
  fwrite(&dim123, sizeof(float), 1, fRAY);
  so->pblock->GetValue(PB_BOX_WIDTH, 0, dim123, FOREVER);
  fwrite(&dim123, sizeof(float), 1, fRAY);
  so->pblock->GetValue(PB_BOX_HEIGHT, 0, dim123, FOREVER);
  fwrite(&dim123, sizeof(float), 1, fRAY);

  //------------------------
  // ESPORTAZIONE KEYFRAMER
  //------------------------
  Control *c;
  int size_key;

  // NB: per gli oggetti mesh e quant'altre tipologie di
  // oggetti che possono essere linkati (ovvero dove e'
  // possibile implmenetare gerarchie), ad esempio patch
  // di Bezier, esporto SEMPRE una key di posizione, una
  // di rotazione ed una di scaling

  // POSITION CONTROLLER
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object position track present.\n");
     write_chunk_header(fRAY, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fRAY);
  }
  // esportiamo comunque una key di posizione TCB
  else
  {
	fprintf(fTXT, "Object position track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fRAY, POSITION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_point3_track(c, 1, fRAY);
  }



  // ROTATION CONTROLLER
  c=node->GetTMController()->GetRotationController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=40;
	 else size_key=20;
	 fprintf(fTXT, "Object rotation track present.\n");
     write_chunk_header(fRAY, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fRAY);
  }
  // esportiamo comunque una key di rotazione TCB
  else
  {
	fprintf(fTXT, "Object rotation track present. (1 key case)\n");
	size_key=40;
    write_chunk_header(fRAY, ROTATION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_rot_track(c, fRAY);
  }

  // SCALE CONTROLLER
  c=node->GetTMController()->GetScaleController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object scaling track present.\n");
     write_chunk_header(fRAY, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fRAY);
  }
  // esportiamo comunque una key di scaling TCB
  else
  {
	fprintf(fTXT, "Object scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fRAY, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fRAY);
  }
}


// ******************************************************************
// ******************************************************************
// ******************************************************************
#define PB_PLANE_LENGTH	0
#define PB_PLANE_WIDTH	1

void SkeletonExporter::export_ray_plane(INode *node)
{
  ObjectState os = node->EvalWorldState(0);
  if (!os.obj) return;
	
  INode *padre=node->GetParentNode();
  Mtl *materiale=node->GetMtl();
  int sf;
  int sm;
  SimpleObject2 *so;
  float dim123;
  Point3 row;

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
  sf=strlen(padre->GetName())+1;
  else sf=0;
  if (materiale) sm=strlen(materiale->GetName())+1;
  else sm=0;
  
  // flag padre + nome padre + flag materiale + nome materiale +
  // + matrice di trasformazione per t=0 + world position +
  // + pivot + lunghezza + larghezza
  write_chunk_header(fRAY, RAY_PLANE_ID, node->GetName(),
	                 4+sf+4+sm+36+12+12+4+4);

  // scrivo il padre (flag, nome)
  fwrite(&sf, sizeof(int), 1, fRAY);
  if (sf>0) write_string0(fRAY, padre->GetName());

  // scrivo il materiale di base (flag, nome)
  fwrite(&sm, sizeof(int), 1, fRAY);
  if (sm>0) write_string0(fRAY, materiale->GetName());

  Matrix3 mat = node->GetNodeTM(0);
  
  // scrittura riga 0 della matrice locale (local coordinate system)
  row = mat.GetRow(0);
  fprintf(fTXT, "Local coord. system row0 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura riga 1 della matrice locale (local coordinate system)
  row = mat.GetRow(1);
  fprintf(fTXT, "Local coord. system row1 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura riga 2 della matrice locale (local coordinate system)
  row = mat.GetRow(2);
  fprintf(fTXT, "Local coord. system row2 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura posizione nel mondo (riga 3)
  row = mat.GetRow(3);
  fprintf(fTXT, "World position : x=%f, y=%f, z=%f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);
  
  // scrittura del punto di pivot
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);


  // dimensioni al tempo 0
  so=(SimpleObject2 *)os.obj;
  so->pblock2->GetValue(PB_PLANE_LENGTH, 0, dim123, FOREVER);
  fwrite(&dim123, sizeof(float), 1, fRAY);
  so->pblock2->GetValue(PB_PLANE_WIDTH, 0, dim123, FOREVER);
  fwrite(&dim123, sizeof(float), 1, fRAY);


  //------------------------
  // ESPORTAZIONE KEYFRAMER
  //------------------------
  Control *c;
  int size_key;

  // NB: per gli oggetti mesh e quant'altre tipologie di
  // oggetti che possono essere linkati (ovvero dove e'
  // possibile implmenetare gerarchie), ad esempio patch
  // di Bezier, esporto SEMPRE una key di posizione, una
  // di rotazione ed una di scaling

  // POSITION CONTROLLER
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object position track present.\n");
     write_chunk_header(fRAY, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fRAY);
  }
  // esportiamo comunque una key di posizione TCB
  else
  {
	fprintf(fTXT, "Object position track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fRAY, POSITION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_point3_track(c, 1, fRAY);
  }



  // ROTATION CONTROLLER
  c=node->GetTMController()->GetRotationController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=40;
	 else size_key=20;
	 fprintf(fTXT, "Object rotation track present.\n");
     write_chunk_header(fRAY, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fRAY);
  }
  // esportiamo comunque una key di rotazione TCB
  else
  {
	fprintf(fTXT, "Object rotation track present. (1 key case)\n");
	size_key=40;
    write_chunk_header(fRAY, ROTATION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_rot_track(c, fRAY);
  }

  // SCALE CONTROLLER
  c=node->GetTMController()->GetScaleController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object scaling track present.\n");
     write_chunk_header(fRAY, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fRAY);
  }
  // esportiamo comunque una key di scaling TCB
  else
  {
	fprintf(fTXT, "Object scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fRAY, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fRAY);
  }
}


// ******************************************************************
// ******************************************************************
// ******************************************************************
#define PB_CYLINDER_RADIUS   0
#define PB_CYLINDER_HEIGHT   1

void SkeletonExporter::export_ray_cylinder(INode *node)
{
  ObjectState os = node->EvalWorldState(0);
  if (!os.obj) return;
	
  INode *padre=node->GetParentNode();
  Mtl *materiale=node->GetMtl();
  int sf;
  int sm;
  SimpleObject *so;
  float radius, height;
  Point3 row;

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
  sf=strlen(padre->GetName())+1;
  else sf=0;
  if (materiale) sm=strlen(materiale->GetName())+1;
  else sm=0;
  
  // flag padre + nome padre + flag materiale + nome materiale +
  // + matrice di trasformazione per t=0 + world position +
  // + pivot + raggio + height
  write_chunk_header(fRAY, RAY_CYLINDER_ID, node->GetName(),
	                 4+sf+4+sm+36+12+12+4+4);

  // scrivo il padre (flag, nome)
  fwrite(&sf, sizeof(int), 1, fRAY);
  if (sf>0) write_string0(fRAY, padre->GetName());

  // scrivo il materiale di base (flag, nome)
  fwrite(&sm, sizeof(int), 1, fRAY);
  if (sm>0) write_string0(fRAY, materiale->GetName());

  Matrix3 mat = node->GetNodeTM(0);
  
  // scrittura riga 0 della matrice locale (local coordinate system)
  row = mat.GetRow(0);
  fprintf(fTXT, "Local coord. system row0 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura riga 1 della matrice locale (local coordinate system)
  row = mat.GetRow(1);
  fprintf(fTXT, "Local coord. system row1 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura riga 2 della matrice locale (local coordinate system)
  row = mat.GetRow(2);
  fprintf(fTXT, "Local coord. system row2 : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // scrittura posizione nel mondo (riga 3)
  row = mat.GetRow(3);
  fprintf(fTXT, "World position : x=%f, y=%f, z=%f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);
  
  // scrittura del punto di pivot
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  fwrite(&row.x, sizeof(float), 1, fRAY);
  fwrite(&row.y, sizeof(float), 1, fRAY);
  fwrite(&row.z, sizeof(float), 1, fRAY);

  // raggio al tempo 0
  so=(SimpleObject *)os.obj;
  so->pblock->GetValue(PB_CYLINDER_RADIUS, 0, radius, FOREVER);
  fwrite(&radius, sizeof(float), 1, fRAY);
  so->pblock->GetValue(PB_CYLINDER_HEIGHT, 0, height, FOREVER);
  fwrite(&height, sizeof(float), 1, fRAY);

  //------------------------
  // ESPORTAZIONE KEYFRAMER
  //------------------------
  Control *c;
  int size_key;

  // NB: per gli oggetti mesh e quant'altre tipologie di
  // oggetti che possono essere linkati (ovvero dove e'
  // possibile implmenetare gerarchie), ad esempio patch
  // di Bezier, esporto SEMPRE una key di posizione, una
  // di rotazione ed una di scaling

  // POSITION CONTROLLER
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object position track present.\n");
     write_chunk_header(fRAY, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fRAY);
  }
  // esportiamo comunque una key di posizione TCB
  else
  {
	fprintf(fTXT, "Object position track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fRAY, POSITION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_point3_track(c, 1, fRAY);
  }



  // ROTATION CONTROLLER
  c=node->GetTMController()->GetRotationController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=40;
	 else size_key=20;
	 fprintf(fTXT, "Object rotation track present.\n");
     write_chunk_header(fRAY, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fRAY);
  }
  // esportiamo comunque una key di rotazione TCB
  else
  {
	fprintf(fTXT, "Object rotation track present. (1 key case)\n");
	size_key=40;
    write_chunk_header(fRAY, ROTATION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_rot_track(c, fRAY);
  }


  // SCALE CONTROLLER
  c=node->GetTMController()->GetScaleController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Object scaling track present.\n");
     write_chunk_header(fRAY, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fRAY);
  }
  // esportiamo comunque una key di scaling TCB
  else
  {
	fprintf(fTXT, "Object scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fRAY, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fRAY);
  }
}