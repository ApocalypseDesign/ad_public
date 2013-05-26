#include "SceneExport.h"
#include "A3Dfile.h"

void SkeletonExporter::export_bipedbone(INode *node)
{
  export_geometric_object(node, BONE_OBJECT, NULL);
  if (makeADO) fprintf(fADO, "  }\n\n");
  fprintf(fTXT, "\n\n");
}


void SkeletonExporter::export_bipedtracks(INode *node)
{
// Il metodo usato è quello di ricavarsi il tempo nella timeline
// di ciascuna key (visto che il controller non permette di
// avere direttamente le key; una volta ottenuto il tempo della
// key estraggo la NodeTM relativa e da questa decompongo
// le parti affini
  Matrix3 m, mf, m_af;
  Matrix3 srtm, rtm, ptm, stm, ftm, mat;
  Point3 row, qaxis;
  AffineParts parts, ap;
  TimeValue tv;
  int size_key, num_keys, keytime, i;
  unsigned char type;
  float qang;
  Quat quat;
  
  INode *p=node->GetParentNode();
  Control *c = node->GetTMController();
  ObjectState os = node->EvalWorldState(0);

  m=node->GetNodeTM(0);
  fprintf(fTXT, "GetNodeTM(0)\n");
  row = m.GetRow(0);
  fprintf(fTXT, "Row0 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = m.GetRow(1);
  fprintf(fTXT, "Row1 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = m.GetRow(2);
  fprintf(fTXT, "Row2 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = m.GetRow(3);
  fprintf(fTXT, "Row3 : %f,  %f,  %f\n", row.x, row.y, row.z);

  decomp_affine(m, &ap);
  ftm = ScaleMatrix(Point3(ap.f,ap.f,ap.f)); 
  ptm.IdentityMatrix();
  ptm.SetTrans(ap.t);	
  ap.q.MakeMatrix(rtm);
  ap.u.MakeMatrix(srtm);
  stm = ScaleMatrix(ap.k);
  mat = Inverse(srtm) * stm * srtm * rtm * ftm * ptm;
  fprintf(fTXT, "Ricostruzione totale\n");
  row = mat.GetRow(0);
  fprintf(fTXT, "Row0 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = mat.GetRow(1);
  fprintf(fTXT, "Row1 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = mat.GetRow(2);
  fprintf(fTXT, "Row2 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = mat.GetRow(3);
  fprintf(fTXT, "Row3 : %f,  %f,  %f\n", row.x, row.y, row.z);

  decomp_affine(m, &ap);
  ptm.IdentityMatrix();
  ptm.SetTrans(ap.t);	
  ap.q.MakeMatrix(rtm);
  row=ScaleValue(ap.k*ap.f, ap.u).s;
  stm = ScaleMatrix(row);
  mat = stm * rtm * ptm;
  fprintf(fTXT, "Ricostruzione parziale\n");
  row = mat.GetRow(0);
  fprintf(fTXT, "Row0 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = mat.GetRow(1);
  fprintf(fTXT, "Row1 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = mat.GetRow(2);
  fprintf(fTXT, "Row2 : %f,  %f,  %f\n", row.x, row.y, row.z);
  row = mat.GetRow(3);
  fprintf(fTXT, "Row3 : %f,  %f,  %f\n", row.x, row.y, row.z);


  // Get the Biped Export Interface from the controller 
  IBipedExport *BipIface = (IBipedExport *) c->GetInterface(I_BIPINTERFACE);
 
  // Remove the non uniform scale
  // BipIface->RemoveNonUniformScale(1);

  // Esportiamo comunque una key in posizione 0
  // m=node->GetNodeTM(0);
  // mf=p->GetNodeTM(0);
  // m_af=m*Inverse(mf);


/*  
  // ***********    KEYFRAME    *****************
  //
  // Conto il numero di key
  // Almeno una key: key in posizione 0
  num_keys=1; 
  tv=0;
  cic=c->GetNextKeyTime(tv, NEXTKEY_RIGHT, tv2);
  while ((cic) && (tv2>tv))
  {
	num_keys++;
	tv=tv2;
    cic=c->GetNextKeyTime(tv, NEXTKEY_RIGHT, tv2);
  }
  fprintf(fTXT, "Key counting: %d\n", num_keys);
*/

  ULONG start = ip->GetAnimRange().Start();
  ULONG end   = ip->GetAnimRange().End();
  num_keys=(end/GetTicksPerFrame() - start/GetTicksPerFrame())/cs_rate;
  ULONG delta = (end-start)/num_keys;
  type=LINEAR_CONTROLLER;

  // ---------------------------------------------------------
  // ########################  POSIZIONE  ####################
  // ---------------------------------------------------------
  fprintf(fTXT, "Biped position track present.\n");
  size_key=16;
  write_chunk_header(fA3D, POSITION_TRACK_ID,
	                    node->GetName(), 1+4+num_keys*size_key);
  fwrite(&type, sizeof(unsigned char), 1, fA3D);
  fwrite(&num_keys, sizeof(int), 1, fA3D);

  tv=0;
  for (i=0; i<num_keys; i++)
  {
    m=node->GetNodeTM(tv);
    if(IsNodeBone(node->GetParentNode()))
	{
	  mf = GetBoneTM(node->GetParentNode(), tv);
      // m = m * Inverse(mf);
	}
    row = m.GetTrans();
	keytime=tv/GetTicksPerFrame();
    fwrite(&keytime, sizeof(int), 1, fA3D);
    fwrite(&row.x, sizeof(float), 1, fA3D);
    fwrite(&row.y, sizeof(float), 1, fA3D);
    fwrite(&row.z, sizeof(float), 1, fA3D);
	fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
            keytime, row.x, row.y, row.z);
	tv=tv+delta;
  }
/*
  // Scrittura key 0
  keytime=0;
  m=node->GetNodeTM(0);
  if(IsNodeBone(node->GetParentNode()))
  {
	mf = GetBoneTM(node->GetParentNode(), 0);
	m = m * Inverse(mf);
  }
  row = m.GetTrans();
  fwrite(&keytime, sizeof(int), 1, fA3D);
  fwrite(&row.x, sizeof(float), 1, fA3D);
  fwrite(&row.y, sizeof(float), 1, fA3D);
  fwrite(&row.z, sizeof(float), 1, fA3D);
  fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
	      keytime, row.x, row.y, row.z);

  tv=0;
  cic=c->GetNextKeyTime(tv, NEXTKEY_RIGHT, tv2);
  while ((cic) && (tv2>tv))
  {
    m=node->GetNodeTM(tv2);
    if(IsNodeBone(node->GetParentNode()))
	{
	  mf = GetBoneTM(node->GetParentNode(), tv2);
	  m = m * Inverse(mf);
	}
    row = m.GetTrans();
	keytime=tv2/GetTicksPerFrame();
    fwrite(&keytime, sizeof(int), 1, fA3D);
    fwrite(&row.x, sizeof(float), 1, fA3D);
    fwrite(&row.y, sizeof(float), 1, fA3D);
    fwrite(&row.z, sizeof(float), 1, fA3D);
    fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
            keytime, row.x, row.y, row.z);

	tv=tv2;
    cic=c->GetNextKeyTime(tv, NEXTKEY_RIGHT, tv2);
  }
*/

  // ---------------------------------------------------------
  // ######################  ROTAZIONE  ######################
  // ---------------------------------------------------------
  Quat rq;
  fprintf(fTXT, "Biped rotation track present.\n");
  size_key=20;
  write_chunk_header(fA3D, ROTATION_TRACK_ID,
	                    node->GetName(), 1+4+num_keys*size_key);
  fwrite(&type, sizeof(unsigned char), 1, fA3D);
  fwrite(&num_keys, sizeof(int), 1, fA3D);

  tv=0;
  for (i=0; i<num_keys; i++)
  {
    m=node->GetNodeTM(tv);
    if(IsNodeBone(node->GetParentNode()))
    {
	  mf = GetBoneTM(node->GetParentNode(), tv);
      // m = m * Inverse(mf);
	}
    decomp_affine(m, &parts);
//	rq=parts.q*parts.u;
    AngAxisFromQ(parts.q, &qang, qaxis);
    quat.x=qaxis.x;
    quat.y=qaxis.y;
    quat.z=qaxis.z;
    quat.w=qang;
	keytime=tv/GetTicksPerFrame();
    fprintf(fTXT, "timepos : %d,  value %f, %f, %f,  %f\n",
	      keytime, quat.x, quat.y, quat.z, quat.w);
    fwrite(&keytime, sizeof(int), 1, fA3D);  // timepos=0
    fwrite(&quat.x, sizeof(float), 1, fA3D);
    fwrite(&quat.y, sizeof(float), 1, fA3D);
    fwrite(&quat.z, sizeof(float), 1, fA3D);
    fwrite(&quat.w, sizeof(float), 1, fA3D);
	tv=tv+delta;
  }
/*
  // Scrittura key 0
  keytime=0;
  m=node->GetNodeTM(0);
  if(IsNodeBone(node->GetParentNode()))
  {
	mf = GetBoneTM(node->GetParentNode(), 0);
	m = m * Inverse(mf);
  }
  decomp_affine(m, &parts);
  AngAxisFromQ(parts.q, &qang, qaxis);
  Quat quat;
  quat.x=qaxis.x;
  quat.y=qaxis.y;
  quat.z=qaxis.z;
  quat.w=qang;

  fprintf(fTXT, "timepos : %d,  value %f, %f, %f,  %f\n",
	      keytime, quat.x, quat.y, quat.z, quat.w);

  fwrite(&keytime, sizeof(int), 1, fA3D);  // timepos=0
  fwrite(&quat.x, sizeof(float), 1, fA3D);
  fwrite(&quat.y, sizeof(float), 1, fA3D);
  fwrite(&quat.z, sizeof(float), 1, fA3D);
  fwrite(&quat.w, sizeof(float), 1, fA3D);

  tv=0;
  cic=c->GetNextKeyTime(tv, NEXTKEY_RIGHT, tv2);
  while ((cic) && (tv2>tv))
  {
    m=node->GetNodeTM(tv2);
    if(IsNodeBone(node->GetParentNode()))
    {
	  mf = GetBoneTM(node->GetParentNode(), tv2);
	  m = m * Inverse(mf);
	}

    decomp_affine(m, &parts);
    AngAxisFromQ(parts.q, &qang, qaxis);
    quat.x=qaxis.x;
    quat.y=qaxis.y;
    quat.z=qaxis.z;
    quat.w=qang;

	keytime=tv2/GetTicksPerFrame();
    fprintf(fTXT, "timepos : %d,  value %f, %f, %f,  %f\n",
	      keytime, quat.x, quat.y, quat.z, quat.w);
    fwrite(&keytime, sizeof(int), 1, fA3D);  // timepos=0
    fwrite(&quat.x, sizeof(float), 1, fA3D);
    fwrite(&quat.y, sizeof(float), 1, fA3D);
    fwrite(&quat.z, sizeof(float), 1, fA3D);
    fwrite(&quat.w, sizeof(float), 1, fA3D);

	tv=tv2;
    cic=c->GetNextKeyTime(tv, NEXTKEY_RIGHT, tv2);
  }
*/


  // ---------------------------------------------------------  
  // ########################  SCALING  ######################
  // ---------------------------------------------------------
  fprintf(fTXT, "Biped scaling track present.\n");
  size_key=16;
  write_chunk_header(fA3D, SCALE_TRACK_ID,
	                    node->GetName(), 1+4+num_keys*size_key);
  fwrite(&type, sizeof(unsigned char), 1, fA3D);
  fwrite(&num_keys, sizeof(int), 1, fA3D);

  tv=0;
  for (i=0; i<num_keys; i++)
  {
    m=node->GetNodeTM(tv);
    if(IsNodeBone(node->GetParentNode()))
    {
	  mf = GetBoneTM(node->GetParentNode(), tv);
      // m = m * Inverse(mf);
	}
    decomp_affine(m, &parts);
    //row=parts.k;
	row=ScaleValue(parts.k*parts.f, parts.u).s;
	keytime=tv/GetTicksPerFrame();
    fwrite(&keytime, sizeof(int), 1, fA3D);
    fwrite(&row.x, sizeof(float), 1, fA3D);
    fwrite(&row.y, sizeof(float), 1, fA3D);
    fwrite(&row.z, sizeof(float), 1, fA3D);
    fprintf(fTXT, "scale axis: %f, %f, %f, %f\n",
            parts.u.x, parts.u.y, parts.u.z, parts.u.w);
    fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
            keytime, row.x, row.y, row.z);
	tv=tv+delta;
  }

/*
  // Scrittura key 0
  keytime=0;
  m=node->GetNodeTM(0);
  if(IsNodeBone(node->GetParentNode()))
  {
	mf = GetBoneTM(node->GetParentNode(), 0);
	m = m * Inverse(mf);
  }
  decomp_affine(m, &parts);
  row=parts.k;
  fwrite(&keytime, sizeof(int), 1, fA3D);
  fwrite(&row.x, sizeof(float), 1, fA3D);
  fwrite(&row.y, sizeof(float), 1, fA3D);
  fwrite(&row.z, sizeof(float), 1, fA3D);
  fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
	      keytime, row.x, row.y, row.z);

  tv=0;
  cic=c->GetNextKeyTime(tv, NEXTKEY_RIGHT, tv2);
  while ((cic) && (tv2>tv))
  {
    m=node->GetNodeTM(tv2);
    if(IsNodeBone(node->GetParentNode()))
	{
	  mf = GetBoneTM(node->GetParentNode(), tv2);
	  m = m * Inverse(mf);
	}
    decomp_affine(m, &parts);
    row=parts.k;
	keytime=tv2/GetTicksPerFrame();
    fwrite(&keytime, sizeof(int), 1, fA3D);
    fwrite(&row.x, sizeof(float), 1, fA3D);
    fwrite(&row.y, sizeof(float), 1, fA3D);
    fwrite(&row.z, sizeof(float), 1, fA3D);
    fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
            keytime, row.x, row.y, row.z);

	tv=tv2;
    cic=c->GetNextKeyTime(tv, NEXTKEY_RIGHT, tv2);
  }
*/

  // Release the interface when you are done with it
  // BipIface->RemoveNonUniformScale(0);
  c->ReleaseInterface(I_BIPINTERFACE, BipIface);
}