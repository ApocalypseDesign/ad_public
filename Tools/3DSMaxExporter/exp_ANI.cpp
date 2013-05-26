// Esportazione di animazioni .ANI (tesi)
#include "SceneExport.h"
#include "A3Dfile.h"

void SkeletonExporter::export_ANI_postrack(INode *node, Control *c, FILE *f)
{
  int numkeys;
  int keytime;
  ITCBPoint3Key kTCB;
  Point3 T, T0;
  
  // ci devono essere almeno 1 key
  numkeys = c->NumKeys();
  if (numkeys==NOT_KEYFRAMEABLE || numkeys==0) return;
  if (numkeys>29) numkeys=29;

  if (!IsTCBControl(c))
  {
	 fprintf(fTXT, "ANI traccia di posizione bone non TCB!!!\n");
	 return;
  }
  
  // Get the keyframe interface
  IKeyControl *ikeys = GetKeyControlInterface(c);
  fwrite(&numkeys, sizeof(int), 1, f);
  fprintf(fTXT, "Number of keys : %d\n", numkeys);
  c->GetValue(0, &T0, FOREVER, CTRL_ABSOLUTE);
	
  for (int i=0; i<numkeys; i++) 
  {
     ikeys->GetKey(i, &kTCB);
     keytime = kTCB.time/GetTicksPerFrame();
	 T.x = kTCB.val.x - T0.x;
	 T.y = kTCB.val.y - T0.y;
	 T.z = kTCB.val.z - T0.z;
     fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
		      keytime, T.x, T.z, T.y);

     // timeline position
     fwrite(&keytime, sizeof(int), 1, f);
     fwrite(&T.x, sizeof(float), 1, f);
     fwrite(&T.z, sizeof(float), 1, f);
     fwrite(&T.y, sizeof(float), 1, f);
     fwrite(&kTCB.tens, sizeof(float), 1, f);
     fwrite(&kTCB.cont, sizeof(float), 1, f);
     fwrite(&kTCB.bias, sizeof(float), 1, f);
     fwrite(&kTCB.easeIn, sizeof(float), 1, f);
     fwrite(&kTCB.easeOut, sizeof(float), 1, f);
  }
}


void SkeletonExporter::export_ANI_rottrack(INode *node, Control *c, FILE *f)
{
  int numkeys, i, keytime;
  ITCBRotKey kTCB, key0, keys[30];
  Quat quat0Inv, quat, iquat, old_qkey, qkeys[30];
  Matrix3 fTM;
  INode *father;

  
  if (!IsTCBControl(c))
  { fprintf(fTXT, "ANI traccia di rotazione bone non TCB!!!\n"); return; }

  numkeys = c->NumKeys();
  if (numkeys>29) numkeys=29;
  IKeyControl *ikeys = GetKeyControlInterface(c);
  fwrite(&numkeys, sizeof(int), 1, f);
  fprintf(fTXT, "ANI rot number of keys : %d\n", numkeys);
  father=node->GetParentNode();

  // estraiamo tutte le keys
  for (i=0; i<numkeys; i++)
  {
	 ikeys->GetKey(i, &kTCB);
	 keys[i]=kTCB;
	 qkeys[i]=QFromAngAxis(kTCB.val.angle, kTCB.val.axis);
  }
  old_qkey=qkeys[0];
  quat0Inv=Inverse(qkeys[0]);

  // rendo le key assolute
  for (i=0; i<numkeys; i++)
  {
	 if (i>0)
	 {
        qkeys[i]=old_qkey*qkeys[i];
		old_qkey=qkeys[i];
	 }
	 else qkeys[i]=old_qkey;
  }
  for (i=0; i<numkeys; i++)
    qkeys[i]=quat0Inv*qkeys[i];

  // rendo gli assi assoluti
  for (i=0; i<numkeys; i++)
  {
	 if (!father->IsRootNode())
	    fTM=father->GetNodeTM(keys[i].time);
	 else fTM.IdentityMatrix();
	 fTM.NoScale();
	 fTM.NoTrans();
     quat=TransformQuat(fTM, qkeys[i]);

	 keytime = keys[i].time/GetTicksPerFrame();
     fwrite(&keytime, sizeof(int), 1, f);
	 fwrite(&quat.x, sizeof(float), 1, f);
	 fwrite(&quat.z, sizeof(float), 1, f);
	 fwrite(&quat.y, sizeof(float), 1, f);
	 fwrite(&quat.w, sizeof(float), 1, f);
	 fwrite(&kTCB.tens, sizeof(float), 1, f);
	 fwrite(&kTCB.bias, sizeof(float), 1, f);
	 fwrite(&kTCB.cont, sizeof(float), 1, f);
	 fwrite(&kTCB.easeIn, sizeof(float), 1, f);
	 fwrite(&kTCB.easeOut, sizeof(float), 1, f);
  }

/*  
  // estraggo la prima key di rotazione
  quat0Inv=QFromAngAxis(-keys[0].val.angle, keys[0].val.axis);
  fprintf(fTXT, "Quat0Inv: %f, %f, %f, %f\n", quat0Inv.x, quat0Inv.z, quat0Inv.y, quat0Inv.w);
  
  for (i=0; i<numkeys; i++)
  {
     fprintf(fTXT, "kTCB %d: %f, %f, %f, %f\n", i, keys[i].val.axis.x, keys[i].val.axis.z, keys[i].val.axis.y, keys[i].val.angle);

     // trasformo la key in un quaternione unitario
     quat=QFromAngAxis(keys[i].val.angle, keys[i].val.axis);
     fprintf(fTXT, "Quat %d: %f, %f, %f, %f\n", i, quat.x, quat.z, quat.y, quat.w);

     // applico la ortazione inversa iniziale
     iquat=quat0Inv*quat;
     fprintf(fTXT, "Quat0Inv*Quat %d: %f, %f, %f, %f\n", i, iquat.x, iquat.z, iquat.y, iquat.w);
	 qkeys[i]=iquat;
  }

  // rendo le keys assolute (non relative alla key precedente)  
  old_qkey=quat0Inv*QFromAngAxis(keys[0].val.angle, keys[0].val.axis);
  for (i=0; i<numkeys; i++) 
  {
	  iquat=qkeys[i];
	  if (i > 0)
	  {
	    quat=old_qkey*iquat;
        old_qkey=quat;
	  }
	  else quat=iquat;

	  keytime = keys[i].time/GetTicksPerFrame();
      fwrite(&keytime, sizeof(int), 1, f);
	  fwrite(&quat.x, sizeof(float), 1, f);
	  fwrite(&quat.z, sizeof(float), 1, f);
	  fwrite(&quat.y, sizeof(float), 1, f);
	  fwrite(&quat.w, sizeof(float), 1, f);
	  fwrite(&kTCB.tens, sizeof(float), 1, f);
	  fwrite(&kTCB.bias, sizeof(float), 1, f);
	  fwrite(&kTCB.cont, sizeof(float), 1, f);
	  fwrite(&kTCB.easeIn, sizeof(float), 1, f);
	  fwrite(&kTCB.easeOut, sizeof(float), 1, f);
  }
*/
  fprintf(fTXT, "\n");
}


// *****************************************************************
//                  Conteggio di Joint(s) e IKChain(s)
// *****************************************************************
void SkeletonExporter::export_ANI_count (INode *node, int *cJ, int *cIKC)
{
  ObjectState os;
  Class_ID cid;
  SClass_ID scid;

  if (!node) return;
  os = node->EvalWorldState(0);
  if (os.obj)
  {
    scid=os.obj->SuperClassID();
    cid=os.obj->ClassID();

    if ((scid==GEOMOBJECT_CLASS_ID) && (cid==BONE_OBJ_CLASSID))
	   (*cJ)++;
    else
    if ((scid==HELPER_CLASS_ID) && (cid==Class_ID(0x1C706482, 0x3E82994)))
	   (*cIKC)++;
    for (int c=0; c<node->NumberOfChildren(); c++)
       export_ANI_count(node->GetChildNode(c), cJ, cIKC);
  }
}


// esporta un singolo joint
void SkeletonExporter::export_ANI_Joint (INode *node)
{
  Matrix3 mat;
  Point3 row;
  float zero=0.0f, uno=1.0f;
  char char0=0, char1=1;
  int i, int0=0, int1=1;
  Control *c;
  INode *padre=node->GetParentNode();

  write_string0(fANI, node->GetName());
  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
    write_string0(fANI, padre->GetName());
  else write_string0(fANI, "");

  mat = node->GetNodeTM(0);
  // orginal world pos
  row = mat.GetRow(3);
  fwrite(&row.x, sizeof(float), 1, fANI);
  fwrite(&row.z, sizeof(float), 1, fANI);
  fwrite(&row.y, sizeof(float), 1, fANI);
  // original oX, oY, oZ
  row = mat.GetRow(0);
  fwrite(&row.x, sizeof(float), 1, fANI);
  fwrite(&row.z, sizeof(float), 1, fANI);
  fwrite(&row.y, sizeof(float), 1, fANI);
  row = mat.GetRow(2);
  fwrite(&row.x, sizeof(float), 1, fANI);
  fwrite(&row.z, sizeof(float), 1, fANI);
  fwrite(&row.y, sizeof(float), 1, fANI);
  row = mat.GetRow(1);
  fwrite(&row.x, sizeof(float), 1, fANI);
  fwrite(&row.z, sizeof(float), 1, fANI);
  fwrite(&row.y, sizeof(float), 1, fANI);

  // scrivo i DOFs di posizione (non attivi)
  for (i=0; i<3; i++)
  {
     fwrite(&char0, sizeof(char), 1, fANI);  // active
     fwrite(&char0, sizeof(char), 1, fANI);  // limited
     fwrite(&zero, sizeof(float), 1, fANI);  // upper limit
     fwrite(&zero, sizeof(float), 1, fANI);  // lower limit
  }
  // scrivo i DOFs di rotazione (attivi e nonlimitati)
  for (i=0; i<3; i++)
  {
     fwrite(&char1, sizeof(char), 1, fANI);  // active
     fwrite(&char0, sizeof(char), 1, fANI);  // limited
     fwrite(&zero, sizeof(float), 1, fANI);  // upper limit
     fwrite(&zero, sizeof(float), 1, fANI);  // lower limit
  }

  // se non ha padre scrivo la key di traslazione (rispetto alla
  // posizione iniziale) nulla
  if ((padre) && (padre->IsRootNode()))
  {
     c=node->GetTMController()->GetPositionController();
	 if ((c) && (c->NumKeys()>0)) 
		export_ANI_postrack(node, c, fANI);
	 else
	 {
        // numero di keys di posizione
        fwrite(&int1, sizeof(int), 1, fANI);
	    // pos in track al tempo 0
        fwrite(&int0, sizeof(int), 1, fANI);   // posintrack
	    // traslazione nulla
        fwrite(&zero, sizeof(float), 1, fANI); // x
        fwrite(&zero, sizeof(float), 1, fANI); // y
        fwrite(&zero, sizeof(float), 1, fANI); // z
        fwrite(&zero, sizeof(float), 1, fANI); // T
        fwrite(&zero, sizeof(float), 1, fANI); // C
        fwrite(&zero, sizeof(float), 1, fANI); // B
        fwrite(&zero, sizeof(float), 1, fANI); // ef
        fwrite(&zero, sizeof(float), 1, fANI); // et
	 }
  }
  // numero 0 di chiavi di traslazione
  else fwrite(&int0, sizeof(int), 1, fANI);

  c=node->GetTMController()->GetRotationController();
  if ((c) && (c->NumKeys()>0))
  //if (c)
  {
	 // export_ANI_rottrack(node, c, fANI);
     // export_ANI_rottrack2(node, c, fANI);
      export_ANI_rottrack3(node, c, fANI);
  }
  else
  {
	 // scrizo una sola chiave di rotazione (rispetto alla rotazione
	 // iniziale) nulla
     fwrite(&int1, sizeof(int), 1, fANI);
	 // pos in track al tempo 0
     fwrite(&int0, sizeof(int), 1, fANI);
	 // asse
	 fwrite(&zero, 1, sizeof(float), fANI);
	 fwrite(&zero, 1, sizeof(float), fANI);
	 fwrite(&zero, 1, sizeof(float), fANI);
	 // angolo
	 fwrite(&zero, 1, sizeof(float), fANI);
	 // T, C, B, ef, et
	 fwrite(&zero, 1, sizeof(float), fANI);
	 fwrite(&zero, 1, sizeof(float), fANI);
	 fwrite(&zero, 1, sizeof(float), fANI);
	 fwrite(&zero, 1, sizeof(float), fANI);
	 fwrite(&zero, 1, sizeof(float), fANI);
  }
}


// esporta una singola catena cinematica
void SkeletonExporter::export_ANI_IKChain (INode *node)
{
  Control *cont;
  Matrix3 ptm;
  Point3 row;
  INode *padre=node->GetParentNode();
  ObjectState os = node->EvalWorldState(0);
  INode *goal, *J;
  int num_joints, int0=0;
  unsigned char uno=1;


  fprintf(fTXT, "IK chain presente\n");

  cont = node->GetTMController();
  fprintf(fTXT, "TMController Chain: cid=(%X, %X)\n", cont->ClassID().PartA(), cont->ClassID().PartB());
  IIKChainControl* pIKgoal = (IIKChainControl*)cont->GetInterface(I_IKCHAINCONTROL);
  if (pIKgoal)
  {
	 write_string0(fANI, node->GetName());
	 // conto il numero di joint
     num_joints=0;
     J=pIKgoal->EndJoint();
     while (J != pIKgoal->StartJoint())
	 {
        num_joints++;
        J=J->GetParentNode();
	 }
	 if (num_joints>0) num_joints++;
	 fprintf(fTXT, "num_joints: %d\n", num_joints);
	 fwrite(&num_joints, sizeof(int), 1, fANI);
	 // nome degli start e end joint
	 write_string0(fANI, pIKgoal->StartJoint()->GetName());
	 write_string0(fANI, pIKgoal->EndJoint()->GetName());

	 // normal plane e EE axis
	 Point3 initEE = pIKgoal->InitEEAxis(0);
	 Point3 initNormalPlane = pIKgoal->InitPlane(0);
	 fwrite(&initNormalPlane.x, sizeof(float), 1, fANI);
	 fwrite(&initNormalPlane.z, sizeof(float), 1, fANI);
	 fwrite(&initNormalPlane.y, sizeof(float), 1, fANI);
	 fwrite(&initEE.x, sizeof(float), 1, fANI);
	 fwrite(&initEE.z, sizeof(float), 1, fANI);
	 fwrite(&initEE.y, sizeof(float), 1, fANI);

	 // current EE world position
	 goal=pIKgoal->GetNode();
     ptm=goal->GetNodeTM(0);
	 row=ptm.GetRow(3);
	 fwrite(&row.x, sizeof(float), 1, fANI);
	 fwrite(&row.z, sizeof(float), 1, fANI);
	 fwrite(&row.y, sizeof(float), 1, fANI);
     fwrite(&uno, sizeof(unsigned char), 1, fANI);


     // scrivo 0 keys per l'effettore finale, ma andrebbero estratte
	 // le key reali
	 fwrite(&int0, sizeof(int), 1, fANI);
  }
}


// *****************************************************************
//                Esportazione di Joint(s) e IKChain(s)
// *****************************************************************
// esporta tutti i joint(s)
void SkeletonExporter::export_ANI_allJoints (INode *node)
{
  ObjectState os;
  Class_ID cid;
  SClass_ID scid;

  if (!node) return;
  os = node->EvalWorldState(0);
  if (os.obj)
  {
    scid=os.obj->SuperClassID();
    cid=os.obj->ClassID();
    if ((scid==GEOMOBJECT_CLASS_ID) && (cid==BONE_OBJ_CLASSID))
        export_ANI_Joint(node);
    for (int c=0; c<node->NumberOfChildren(); c++)
      export_ANI_allJoints(node->GetChildNode(c));
  }
}


// esporta tutte le catene cinematiche
void SkeletonExporter::export_ANI_allIKChains (INode *node)
{
  ObjectState os;
  Class_ID cid;
  SClass_ID scid;

  if (!node) return;
  os = node->EvalWorldState(0);
  if (os.obj)
  {
    scid=os.obj->SuperClassID();
    cid=os.obj->ClassID();
    if ((scid==HELPER_CLASS_ID) && (cid==Class_ID(0x1C706482, 0x3E82994)))
         export_ANI_IKChain(node);
    for (int c=0; c<node->NumberOfChildren(); c++)
       export_ANI_allIKChains(node->GetChildNode(c));
  }
}


// *****************************************************************
//                     Esportazione del file .ANI
// *****************************************************************
// crea il .ANI della scena
void SkeletonExporter::export_ANI (INode *root)
{
  int num_Joints=0, num_IKChains=0;
  int c;

  for (c=0; c<root->NumberOfChildren(); c++)
    export_ANI_count(root->GetChildNode(c), &num_Joints, &num_IKChains);

  fwrite(&num_Joints, sizeof(int), 1, fANI);
  fprintf(fTXT, "Number of joints: %d\n", num_Joints);
  for (c=0; c<root->NumberOfChildren(); c++)
     export_ANI_allJoints(root->GetChildNode(c));

  fwrite(&num_IKChains, sizeof(int), 1, fANI);
  fprintf(fTXT, "Number of IKChains: %d\n", num_IKChains);
  for (c=0; c<root->NumberOfChildren(); c++)
    export_ANI_allIKChains(root->GetChildNode(c));
}