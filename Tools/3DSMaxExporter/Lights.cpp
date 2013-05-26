
#include "SceneExport.h"
#include "A3Dfile.h"

void SkeletonExporter::export_light(INode *node)
{
  Control *c;
  int size_key, sf;
  ObjectState os;
  GenLight* light;
  struct LightState ls;
  float near_radius, far_radius;
  Point3 row;
  Matrix3 mat;

  os=node->EvalWorldState(0);
  if (!os.obj) return;  // per sicurezza
  light=(GenLight*)os.obj;

  // controlliamo se esiste un padre
  INode *padre=node->GetParentNode();
  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
    sf=strlen(padre->GetName())+1;
  else sf=0;

  light->EvalLightState(0, FOREVER, &ls);
  fprintf(fTXT, "Name : %s\n", node->GetName());
  switch (ls.type)
  {
    case OMNI_LGT: fprintf(fTXT, "Omni light found\n");
		           // flag padre + nome padre + pivot + NodeTM(0) +
		           // + colore
		           write_chunk_header(fA3D, OMNI_LIGHT_ID,
					                  node->GetName(), 4+sf+12+48+12);
		           if (makeRAY) write_chunk_header(fRAY, OMNI_LIGHT_ID,
						              node->GetName(), 4+sf+12+48+12);
		           break;

    case SPOT_LGT: fprintf(fTXT, "Spot light found\n");
		           // flag padre + nome padre + pivot + NodeTM(0) +
		           // + colore + falloff + hotspot
		           write_chunk_header(fA3D, SPOT_LIGHT_ID,
					                  node->GetName(), 4+sf+12+48+12+8);
		           if (makeRAY) write_chunk_header(fRAY, SPOT_LIGHT_ID,
					                  node->GetName(), 4+sf+12+48+12+8);
		           break;
  }

  // -----------    scrivo il padre (flag, nome)   ------------------
  fwrite(&sf, sizeof(int), 1, fA3D);
  if (sf>0) write_string0(fA3D, padre->GetName());
  if (makeRAY) fwrite(&sf, sizeof(int), 1, fRAY);
  if (makeRAY) 
	if (sf>0) write_string0(fRAY, padre->GetName());
  
  // ---------------   scrittura del punto di pivot   ----------------
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  write_point3(&row, fA3D);
  if (makeRAY) write_point3(&row, fRAY);

  // -------------------  salviamo la NodeTM(0)  ---------------------
  mat=node->GetNodeTM(0);
  row=mat.GetRow(3);
  fprintf(fTXT, "World position : x=%f, y=%f, z=%f\n", row.x, row.y, row.z);
  write_matrix(&mat, fA3D);
  if (makeRAY) write_matrix(&mat, fRAY);

  // --------------------  salviamo il colore  -----------------------
  fprintf(fTXT, "Color : R=%f, G=%f, B=%f\n", ls.color.r, ls.color.g, ls.color.b);
  fwrite(&ls.color.r, sizeof(float), 1, fA3D);
  fwrite(&ls.color.g, sizeof(float), 1, fA3D);
  fwrite(&ls.color.b, sizeof(float), 1, fA3D);
  if (makeRAY) fwrite(&ls.color.r, sizeof(float), 1, fRAY);
  if (makeRAY) fwrite(&ls.color.g, sizeof(float), 1, fRAY);
  if (makeRAY) fwrite(&ls.color.b, sizeof(float), 1, fRAY);

  // ----------------------  falloff e hotpsot  ----------------------
  if (ls.type == SPOT_LGT)
  {
	 float hotspot = (float)(ls.hotsize*3.14159265358979323846/180.0);
	 float falloff = (float)(ls.fallsize*3.14159265358979323846/180.0);
	 fwrite(&hotspot, sizeof(float), 1, fA3D);
	 if (makeRAY) fwrite(&hotspot, sizeof(float), 1, fRAY);
	 fwrite(&falloff, sizeof(float), 1, fA3D);
	 if (makeRAY) fwrite(&falloff, sizeof(float), 1, fRAY);
  }

  fprintf(fTXT, "Intensity : %f\n", ls.intens);

  // --------------  esportazione tracce di posizione  ---------------
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	if (IsTCBControl(c)) size_key=36;
    else
	if (IsBezierControl(c)) size_key=40;
	else size_key=16;
	fprintf(fTXT, "Light position track present.");
    write_chunk_header(fA3D, POSITION_TRACK_ID,
                       node->GetName(), 1+2+4+c->NumKeys()*size_key);
    export_point3_track(c, 1, fA3D);
    if (makeRAY) write_chunk_header(fRAY, POSITION_TRACK_ID,
                       node->GetName(), 1+2+4+c->NumKeys()*size_key);
    if (makeRAY) export_point3_track(c, 1, fRAY);
  }


  // ---------------  esportazione tracce di colore  -----------------
  c=light->GetColorControl();
  if ((c) && (c->NumKeys()>0))
  {
	if (IsTCBControl(c)) size_key=36;
	else
	if (IsBezierControl(c)) size_key=40;
	else size_key=16;
	fprintf(fTXT, "Light color track present.");
    write_chunk_header(fA3D, COLOR_TRACK_ID,
                       node->GetName(), 1+2+4+c->NumKeys()*size_key);
    export_point3_track(c, 1.0f, fA3D);
    if (makeRAY) write_chunk_header(fRAY, COLOR_TRACK_ID,
                       node->GetName(), 1+2+4+c->NumKeys()*size_key);
    if (makeRAY) export_point3_track(c, 1.0f, fRAY);
  }

  // --------------  esportazione tracce di hotspot  -----------------
  c=light->GetHotSpotControl();
  if ((c) && (c->NumKeys()>0) && (ls.type == SPOT_LGT))
  {
	if (IsTCBControl(c)) size_key=28;
	else
	if (IsBezierControl(c)) size_key=16;
	else size_key=8;
	fprintf(fTXT, "Light hotspot track present.");
    write_chunk_header(fA3D, HOTSPOT_TRACK_ID,
                       node->GetName(), 1+2+4+c->NumKeys()*size_key);
    export_float_track(c, (float)(3.14159265358979323846/180.0), fA3D);  // in radianti
    if (makeRAY) write_chunk_header(fRAY, HOTSPOT_TRACK_ID,
                       node->GetName(), 1+2+4+c->NumKeys()*size_key);
    if (makeRAY) export_float_track(c, (float)(3.14159265358979323846/180.0), fRAY);  // in radianti
  }

  // --------------  esportazione tracce di falloff  -----------------
  c=light->GetFalloffControl();
  if ((c) && (c->NumKeys()>0) && (ls.type == SPOT_LGT))
  {
	if (IsTCBControl(c)) size_key=28;
	else
	if (IsBezierControl(c)) size_key=16;
	else size_key=8;
	fprintf(fTXT, "Light falloff track present.");
    write_chunk_header(fA3D, FALLOFF_TRACK_ID,
                       node->GetName(), 1+2+4+c->NumKeys()*size_key);
    export_float_track(c, (float)(3.14159265358979323846/180.0), fA3D);  // in radianti
    if (makeRAY) write_chunk_header(fRAY, FALLOFF_TRACK_ID,
                       node->GetName(), 1+2+4+c->NumKeys()*size_key);
    if (makeRAY) export_float_track(c, (float)(3.14159265358979323846/180.0), fRAY);  // in radianti
  }


  // -------------------------  script ADL  --------------------------
  Mtl *materiale=node->GetMtl();
  // NO far + NO near
  if ((!light->GetUseAttenNear()) && (!light->GetUseAtten()))
  {
     far_radius=near_radius=999999;
  }
  else
  // NO far + SI near
  if ((light->GetUseAttenNear()) && (!light->GetUseAtten()))
  {
     near_radius=light->GetAtten(0, ATTEN1_START, FOREVER);
	 far_radius=999999;
  }
  else
  // SI far + NO near
  if ((!light->GetUseAttenNear()) && (light->GetUseAtten()))
  {
     far_radius=light->GetAtten(0, ATTEN_START, FOREVER);
     near_radius=far_radius;
  }
  // SI far + SI near
  if ((light->GetUseAttenNear()) && (light->GetUseAtten()))
  {
     near_radius=light->GetAtten(0, ATTEN1_START, FOREVER);
	 far_radius=light->GetAtten(0, ATTEN_START, FOREVER);
  }
  if (makeADL)
  {
    fprintf(fADL, "  light %c%s%c\n  {\n", '"', node->GetName(), '"');
	if (materiale)
	  fprintf(fADL, "    material=%c%s%c;\n", '"', materiale->GetName(), '"');
    else
	  fprintf(fADL, "    material=%cNONE%c;\n", '"', '"');
    //fprintf(fADL, "    scale_x=%c160%c;\n", '"', '"');
    //fprintf(fADL, "    scale_y=%c160%c;\n", '"', '"');
    //fprintf(fADL, "    near_radius=%c%f%c;\n", '"', near_radius, '"');
    //fprintf(fADL, "    far_radius=%c%f%c;\n", '"', far_radius, '"');
	switch (light->GetDecayType())
	{
	   case 0: fprintf(fADL, "    attenuation0=%c1.0%c;\n", '"', '"');
               fprintf(fADL, "    attenuation1=%c0%c;\n", '"', '"');
               fprintf(fADL, "    attenuation2=%c0%c;\n", '"', '"');
		       break;
	   case 1: fprintf(fADL, "    attenuation0=%c0%c;\n", '"', '"');
               fprintf(fADL, "    attenuation1=%c1.0%c;\n", '"', '"');
               fprintf(fADL, "    attenuation2=%c0%c;\n", '"', '"');
		       break;
	   case 2: fprintf(fADL, "    attenuation0=%c0%c;\n", '"', '"');
               fprintf(fADL, "    attenuation1=%c0%c;\n", '"', '"');
               fprintf(fADL, "    attenuation2=%c1.0%c;\n", '"', '"');
		       break;
	}
	char range[20];
	my_ftoa(far_radius, range);
    fprintf(fADL, "    max_range=%c%s%c;\n", '"', range, '"');
	fprintf(fADL, "  }\n\n");
  }
  fprintf(fTXT, "\n\n\n----------------------------------------------------\n");
}