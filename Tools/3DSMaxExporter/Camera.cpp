
#include "SceneExport.h"
#include "A3Dfile.h"

void SkeletonExporter::export_camera(INode *node)
{
  Control *c;
  int size_key;
  float camera_znear, camera_zfar;
  CameraState cs;
//  Interval valid = FOREVER;
  ObjectState os;
  CameraObject *cam;
  GenCamera *gencam;
  float roll0;
  Matrix3 mat;
  Point3 row;

  os = node->EvalWorldState(0);
  cam = (CameraObject *)os.obj;
  gencam = (GenCamera *)os.obj;
  if (gencam->Type() != TARGETED_CAMERA)
  {
	 fprintf(fTXT, "Only targeted camera are supported!\n\n");
	 return;
  }

  fprintf(fTXT, "Targeted camera found\n");
  write_chunk_header(fA3D, TARGETED_CAMERA_ID, node->GetName(), 40);
  if (makeRAY) write_chunk_header(fRAY, TARGETED_CAMERA_ID, node->GetName(), 40);

  INode* target = node->GetTarget();
  fprintf(fTXT, "Name : %s\n", node->GetName());
  cam->EvalCameraState(0, FOREVER, &cs);

  // ------------------  salviamo znear e zfar  ----------------------
  camera_znear=2;
  camera_zfar=2000;
  if (cs.manualClip)
  {
	camera_znear=cs.hither;
	camera_zfar=cs.yon;
  }
  fprintf(fTXT, "Znear = %f \n", camera_znear);
  fwrite(&camera_znear, sizeof(float), 1, fA3D);
  if (makeRAY) fwrite(&camera_znear, sizeof(float), 1, fRAY);
  fprintf(fTXT, "Zfar = %f \n", camera_zfar);
  fwrite(&camera_zfar, sizeof(float), 1, fA3D);
  if (makeRAY) fwrite(&camera_zfar, sizeof(float), 1, fRAY);

  // -----------------  salviamo l'angolo di FOV  --------------------
  fprintf(fTXT, "FOV (rad) = %f \n", cs.fov);
  fwrite(&cs.fov, sizeof(float), 1, fA3D);
  if (makeRAY) fwrite(&cs.fov, sizeof(float), 1, fRAY);

  // ------------------  salviamo l'angolo di roll  ------------------
  c=node->GetTMController()->GetRollController();
  c->GetValue(0, &roll0, FOREVER);	
  fprintf(fTXT, "Roll (rad) = %f \n", roll0);
  fwrite(&roll0, sizeof(float), 1, fA3D);
  if (makeRAY) fwrite(&roll0, sizeof(float), 1, fRAY);

  // salviamo la posizione nel mondo
  mat = node->GetNodeTM(0);
  row = mat.GetRow(3);
  fprintf(fTXT, "Camera world position : x=%f, y=%f, z=%f\n", row.x, row.y, row.z);
  write_point3(&row, fA3D);
  if (makeRAY) write_point3(&row, fRAY);

  // ---------  salviamo la posizione del target nel mondo  ----------
  if (target)
  {
    mat = target->GetNodeTM(0);
    row = mat.GetRow(3);
    fprintf(fTXT, "Target world position : x=%f, y=%f, z=%f\n", row.x, row.y, row.z);
    write_point3(&row, fA3D);
    if (makeRAY) write_point3(&row, fRAY);
  }

  
  // esportiamo l'animazione della posizione della camera
  // se ci sono un numero di key > 0
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Camera position track present.");
     write_chunk_header(fA3D, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fA3D);
     if (makeRAY) write_chunk_header(fRAY, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     if (makeRAY) export_point3_track(c, 1, fRAY);
  }

  // -----  esportiamo l'animazione della posizione del target  ------
  if (target)
  {
    c=target->GetTMController()->GetPositionController();
    if ((c) && (c->NumKeys()>0))
	{
	  if (IsTCBControl(c)) size_key=36;
	  else
	  if (IsBezierControl(c)) size_key=40;
	  else size_key=16;
	  fprintf(fTXT, "Target position track present.");
	  write_chunk_header(fA3D, CAMERA_TARGET_TRACK_ID,
		                 node->GetName(), 1+2+4+c->NumKeys()*size_key);
	  export_point3_track(c, 1, fA3D);

	  if (makeRAY) write_chunk_header(fRAY, CAMERA_TARGET_TRACK_ID,
		                 node->GetName(), 1+2+4+c->NumKeys()*size_key);
	  if (makeRAY) export_point3_track(c, 1, fRAY);
	}
  }

  // ---------------  esportiamo le tracce di FOV  -------------------
  c=gencam->GetFOVControl();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Camera FOV track present.");
     write_chunk_header(fA3D, CAMERA_FOV_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);  // radianti
     if (makeRAY) write_chunk_header(fRAY, CAMERA_FOV_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     if (makeRAY) export_float_track(c, 1, fRAY);  // radianti
  }


  // ---------------  esportiamo le tracce di roll  ------------------
  c=node->GetTMController()->GetRollController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Camera roll track present.");
     write_chunk_header(fA3D, CAMERA_ROLL_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D); // radianti

     if (makeRAY) write_chunk_header(fRAY, CAMERA_ROLL_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     if (makeRAY) export_float_track(c, 1, fRAY); // radianti
  }
  fprintf(fTXT, "\n\n--------------------------------------------------\n");
}