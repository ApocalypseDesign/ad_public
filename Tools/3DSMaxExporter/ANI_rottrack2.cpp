#include "SceneExport.h"
#include "A3Dfile.h"

void SkeletonExporter::export_ANI_rottrack2(INode *node, Control *c, FILE *f)
{
  int numkeys, i, keytime;
  ITCBRotKey keys[30];
  INode *father;
  Matrix3 TM, fTM, InvTM0;
  AffineParts parts;
  Quat quat;
  
  if (!IsTCBControl(c))
  {
	 fprintf(fTXT, "ANI traccia di rotazione bone non TCB!!!\n");
	 return;
  }

  numkeys = c->NumKeys();
  if (numkeys>29) numkeys=29;
  IKeyControl *ikeys = GetKeyControlInterface(c);
  fwrite(&numkeys, sizeof(int), 1, f);
  fprintf(fTXT, "ANI rot number of keys : %d\n", numkeys);
  father=node->GetParentNode();

  // raccoglimento key(s)
  for (i=0; i<numkeys; i++) ikeys->GetKey(i, &keys[i]);

  TM=node->GetNodeTM(keys[0].time);
  if (!father->IsRootNode())
  {
    fTM=father->GetNodeTM(keys[0].time);
	TM=TM*Inverse(fTM);
  }
  InvTM0=Inverse(TM);

  for (i=0; i<numkeys; i++)
  {
     keytime=keys[i].time/GetTicksPerFrame();
	 TM=node->GetNodeTM(keys[i].time);
	 if (!father->IsRootNode())
	 {
	    fTM=father->GetNodeTM(keys[i].time);
		TM=TM*Inverse(fTM);
	 }
	 else fTM.IdentityMatrix();
	 TM=InvTM0*TM;
	 fTM.NoScale();
	 fTM.NoTrans();

     decomp_affine(TM, &parts);
	 quat=TransformQuat(fTM, parts.q);

	 fprintf(fTXT, "RotQuat %d: %d, q.x=%f, q.y=%f, q.z=%f, q.w=%f\n", i, keytime,
		     parts.q.x, parts.q.z, parts.q.y, parts.q.w);
	 fprintf(fTXT, "sign=%f, u.x=%f, u.y=%f, q.z=%f, u.w=%f\n",
		     parts.f, parts.u.x, parts.u.z, parts.u.y, parts.u.w);
	 fprintf(fTXT, "k.x=%f, k.y=%f, k.z=%f\n",
		     parts.k.x, parts.k.z, parts.k.y);
	 //fprintf(fTXT, "Rotkey %d: %d, %f, %f, %f, %f\n", i, keytime, qaxis.x, qaxis.z, qaxis.y, qang);

     fwrite(&keytime, sizeof(int), 1, f);
	 fwrite(&quat.x, sizeof(float), 1, f);
	 fwrite(&quat.z, sizeof(float), 1, f);
	 fwrite(&quat.y, sizeof(float), 1, f);
	 fwrite(&quat.w, sizeof(float), 1, f);
	 fwrite(&keys[i].tens, sizeof(float), 1, f);
	 fwrite(&keys[i].bias, sizeof(float), 1, f);
	 fwrite(&keys[i].cont, sizeof(float), 1, f);
	 fwrite(&keys[i].easeIn, sizeof(float), 1, f);
	 fwrite(&keys[i].easeOut, sizeof(float), 1, f);
  }
}


void SkeletonExporter::export_ANI_rottrack3(INode *node, Control *c, FILE *f)
{
  IKeyControl *ikeys;
  Quat q0inv, quat, qaux;
  ITCBRotKey kTCB;
  int i, numkeys, keytime;
  INode *father;
  float f0=0;
  Matrix3 fTM;
  Point3 axis, newaxis;
  //float ang;
  //TimeValue t1, t2,  t;

  if (!IsTCBControl(c))
  {
	 fprintf(fTXT, "ANI traccia di rotazione bone non TCB!!!\n");
	 return;
  }
  father=node->GetParentNode();

  numkeys = c->NumKeys();
  if (numkeys>29) numkeys=29;
  ikeys = GetKeyControlInterface(c);

  // ikeys->GetKey(0, &kTCB); t1=kTCB.time;
  // ikeys->GetKey(numkeys-1, &kTCB); t2=kTCB.time;
  // numkeys=20;
  // t=(t2-t1)/numkeys;

  fprintf(fTXT, "%s, ANI rot number of keys : %d\n", node->GetName(), numkeys);
  c->GetValue(0, &quat, FOREVER, CTRL_ABSOLUTE);
  // fprintf(fTXT, "q0: x=%f, y=%f, z=%f, w=%f\n", quat.x, quat.z, quat.y, quat.w);
  q0inv=Inverse(quat);
  // fprintf(fTXT, "q0inv: x=%f, y=%f, z=%f, w=%f\n", q0inv.x, q0inv.z, q0inv.y, q0inv.w);
  fwrite(&numkeys, sizeof(int), 1, f);

  for (i=0; i<numkeys; i++)
  {
     ikeys->GetKey(i, &kTCB);
     keytime=kTCB.time/GetTicksPerFrame();
     // keytime=t1/GetTicksPerFrame();
	 c->GetValue(kTCB.time, &quat, FOREVER, CTRL_ABSOLUTE);
     // c->GetValue(t1, &quat, FOREVER, CTRL_ABSOLUTE);
	 // fprintf(fTXT, "RotQuat %d: %d, x=%f, y=%f, z=%f, w=%f\n", i, keytime,
     //         quat.x, quat.z, quat.y, quat.w);
	 quat=q0inv*quat;
	 // fprintf(fTXT, "RotQuat*q0inv: x=%f, y=%f, z=%f, w=%f\n",
     //         quat.x, quat.z, quat.y, quat.w);
     // AngAxisFromQ(quat, &ang, axis);
	 // fprintf(fTXT, "AngAxis version(RotQuat*q0inv): x=%f, y=%f, z=%f, ang=%f\n",
     //        axis.x, axis.z, axis.y, ang);

     if (!father->IsRootNode())
	 {
        fTM=father->GetNodeTM(kTCB.time);
        // fTM=father->GetNodeTM(t1);
		fTM.NoScale();
		fTM.NoTrans();
	 } else fTM.IdentityMatrix();
	 quat=TransformQuat(fTM, quat);
/*
	 newaxis=fTM*axis;
	 fprintf(fTXT, "New AngAxis: x=%f, y=%f, z=%f, ang=%f\n",
             newaxis.x, newaxis.z, newaxis.y, ang);
     quat=QFromAngAxis(ang, newaxis);
*/
	 // fprintf(fTXT, "NewQuat: x=%f, y=%f, z=%f, w=%f\n",
     //         quat.x, quat.z, quat.y, quat.w);

     fwrite(&keytime, sizeof(int), 1, f);
	 fwrite(&quat.x, sizeof(float), 1, f);
	 fwrite(&quat.z, sizeof(float), 1, f);
	 fwrite(&quat.y, sizeof(float), 1, f);
	 fwrite(&quat.w, sizeof(float), 1, f);
	 /*
	 fwrite(&f0, sizeof(float), 1, f);
	 fwrite(&f0, sizeof(float), 1, f);
	 fwrite(&f0, sizeof(float), 1, f);
	 fwrite(&f0, sizeof(float), 1, f);
	 fwrite(&f0, sizeof(float), 1, f);
	 */
	 fwrite(&kTCB.tens, sizeof(float), 1, f);
	 fwrite(&kTCB.bias, sizeof(float), 1, f);
	 fwrite(&kTCB.cont, sizeof(float), 1, f);
	 fwrite(&kTCB.easeIn, sizeof(float), 1, f);
	 fwrite(&kTCB.easeOut, sizeof(float), 1, f);
	 
	 // t1=t1+t;
  }
  fprintf(fTXT, "\n");
}