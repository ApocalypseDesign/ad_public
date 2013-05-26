#include "SceneExport.h"
#include "A3Dfile.h"


void SkeletonExporter::dump_joint_parameters(JointParams* joint)
{
  float scale = joint->scale;

  if (joint->Type() == JNT_POS)
     fprintf(fTXT, "ID_IKTYPE: Positional joint\n");
  else
     fprintf(fTXT, "ID_IKTYPE: Rotational joint\n");

  fprintf(fTXT, "ID_IKDOF: %d\n", joint->dofs);

  fprintf(fTXT, "ID_IKXACTIVE: %d\n", joint->flags & JNT_XACTIVE  ? 1 : 0);
  fprintf(fTXT, "ID_IKYACTIVE: %d\n", joint->flags & JNT_YACTIVE  ? 1 : 0);
  fprintf(fTXT, "ID_IKZACTIVE: %d\n", joint->flags & JNT_ZACTIVE  ? 1 : 0);

  fprintf(fTXT, "ID_IKXLIMITED: %d\n", joint->flags & JNT_XLIMITED ? 1 : 0);
  fprintf(fTXT, "ID_IKYLIMITED: %d\n", joint->flags & JNT_YLIMITED ? 1 : 0);
  fprintf(fTXT, "ID_IKZLIMITED: %d\n", joint->flags & JNT_ZLIMITED ? 1 : 0);

  fprintf(fTXT, "ID_IKXEASE: %d\n", joint->flags & JNT_XEASE ? 1 : 0);
  fprintf(fTXT, "ID_IKYEASE: %d\n", joint->flags & JNT_YEASE ? 1 : 0);
  fprintf(fTXT, "ID_IKZEASE: %d\n", joint->flags & JNT_ZEASE ? 1 : 0);

  fprintf(fTXT, "ID_IKLIMITEXACT: %d\n", joint->flags & JNT_LIMITEXACT ? 1 : 0);

  for (int i=0; i<joint->dofs; i++)
  {
     fprintf(fTXT, "ID_IKJOINTINFO %d %f %f %f\n", i, joint->min[i], joint->max[i], joint->damping[i]);
  }
}


int SkeletonExporter::get_IIKControl_size(IIKControl *c)
{
   // ritorna la size (in byte) che occupano i parametri esportati
   int s=0;

   if (!c) return(0);
   // posizioni
   if (c->DofActive(IKSys::TransX))
   {
      s+=2;  // il flag di active + flag di limited
	  if ((c->DofLowerLimited(IKSys::TransX)) ||
		  (c->DofUpperLimited(IKSys::TransX)))
          s+=(2*sizeof(float));
   } else s+=1;  // solo il flag di active
   if (c->DofActive(IKSys::TransY))
   {
      s+=2;  // il flag di active + flag di limited
	  if ((c->DofLowerLimited(IKSys::TransY)) ||
		  (c->DofUpperLimited(IKSys::TransY)))
          s+=(2*sizeof(float));
   } else s+=1;  // solo il flag di active
   if (c->DofActive(IKSys::TransZ))
   {
      s+=2;  // il flag di active + flag di limited
	  if ((c->DofLowerLimited(IKSys::TransZ)) ||
          (c->DofUpperLimited(IKSys::TransZ)))
          s+=(2*sizeof(float));
   } else s+=1;  // solo il flag di active

   // rotazioni
   if (c->DofActive(IKSys::RotX))
   {
      s+=2;  // il flag di active + flag di limited
	  if ((c->DofLowerLimited(IKSys::RotX)) ||
	      (c->DofUpperLimited(IKSys::RotX)))
           s+=(2*sizeof(float));
   } else s+=1;  // solo il flag di active
   if (c->DofActive(IKSys::RotY))
   {
      s+=2;  // il flag di active + flag di limited
	  if ((c->DofLowerLimited(IKSys::RotY)) ||
	      (c->DofUpperLimited(IKSys::RotY)))
           s+=(2*sizeof(float));
   } else s+=1;  // solo il flag di active
   if (c->DofActive(IKSys::RotZ))
   {
      s+=2;  // il flag di active + flag di limited
	  if ((c->DofLowerLimited(IKSys::RotZ)) ||
	      (c->DofUpperLimited(IKSys::RotZ)))
           s+=(2*sizeof(float));
   } else s+=1;  // solo il flag di active
   return(s);
}


void SkeletonExporter::export_IIKControl(IIKControl *c, FILE *f)
{
   Point2 l;
   int i, j;
   char f0=0, f1=1;
   INode *node;

   if (!c) return;

   // ----------------------------------------------------------------
   // *************************  Posizioni  **************************
   // ----------------------------------------------------------------
   if (c->DofActive(IKSys::TransX))
   {
	  fprintf(fTXT, "TransX active\n");
	  fwrite(&f1, sizeof(char), 1, f);  // scrittura flag active=1
	  if ((c->DofLowerLimited(IKSys::TransX)) ||
		  (c->DofUpperLimited(IKSys::TransX)))
	  {
         l=c->DofLimits(IKSys::TransX);
	     fprintf(fTXT, "TransX is lower limited at: %f\n", l.x);
	     fprintf(fTXT, "TransX is upper limited at: %f\n", l.y);
         fwrite(&f1, sizeof(char), 1, f);   // scrittura flag limited=1
		 fwrite(&l.x, sizeof(float), 1, f); // lower limit
		 fwrite(&l.y, sizeof(float), 1, f); // upper limit
	  } else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag limited=0
   }
   else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag active=0

   if (c->DofActive(IKSys::TransY))
   {
	  fprintf(fTXT, "TransY active\n");
	  fwrite(&f1, sizeof(char), 1, f);  // scrittura flag active=1
	  if ((c->DofLowerLimited(IKSys::TransY)) ||
		  (c->DofUpperLimited(IKSys::TransY)))
	  {
         l=c->DofLimits(IKSys::TransY);
	     fprintf(fTXT, "TransY is lower limited at: %f\n", l.x);
	     fprintf(fTXT, "TransY is upper limited at: %f\n", l.y);
         fwrite(&f1, sizeof(char), 1, f);   // scrittura flag limited=1
		 fwrite(&l.x, sizeof(float), 1, f); // lower limit
		 fwrite(&l.y, sizeof(float), 1, f); // upper limit
	  } else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag limited=0
   }
   else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag active=0

   if (c->DofActive(IKSys::TransZ))
   {
	  fprintf(fTXT, "TransZ active\n");
	  fwrite(&f1, sizeof(char), 1, f);  // scrittura flag active=1
	  if ((c->DofLowerLimited(IKSys::TransZ)) ||
		  (c->DofUpperLimited(IKSys::TransZ)))
	  {
         l=c->DofLimits(IKSys::TransZ);
	     fprintf(fTXT, "TransZ is lower limited at: %f\n", l.x);
	     fprintf(fTXT, "TransZ is upper limited at: %f\n", l.y);
         fwrite(&f1, sizeof(char), 1, f);   // scrittura flag limited=1
		 fwrite(&l.x, sizeof(float), 1, f); // lower limit
		 fwrite(&l.y, sizeof(float), 1, f); // upper limit
	  } else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag limited=0
   }
   else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag active=0



   // ----------------------------------------------------------------
   // *************************  Rotazioni  **************************
   // ----------------------------------------------------------------
   if (c->DofActive(IKSys::RotX))
   {
	  fprintf(fTXT, "RotX active\n");
	  fwrite(&f1, sizeof(char), 1, f);  // scrittura flag active=1
	  if ((c->DofLowerLimited(IKSys::RotX)) ||
		  (c->DofUpperLimited(IKSys::RotX)))
	  {
         l=c->DofLimits(IKSys::RotX);
	     fprintf(fTXT, "RotX is lower limited at: %f\n", l.x);
	     fprintf(fTXT, "RotX is upper limited at: %f\n", l.y);
         fwrite(&f1, sizeof(char), 1, f);   // scrittura flag limited=1
		 fwrite(&l.x, sizeof(float), 1, f); // lower limit
		 fwrite(&l.y, sizeof(float), 1, f); // upper limit
	  } else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag limited=0
   }
   else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag active=0

   if (c->DofActive(IKSys::RotY))
   {
	  fprintf(fTXT, "RotY active\n");
	  fwrite(&f1, sizeof(char), 1, f);  // scrittura flag active=1
	  if ((c->DofLowerLimited(IKSys::RotY)) ||
		  (c->DofUpperLimited(IKSys::RotY)))
	  {
         l=c->DofLimits(IKSys::RotY);
	     fprintf(fTXT, "RotY is lower limited at: %f\n", l.x);
	     fprintf(fTXT, "RotY is upper limited at: %f\n", l.y);
         fwrite(&f1, sizeof(char), 1, f);   // scrittura flag limited=1
		 fwrite(&l.x, sizeof(float), 1, f); // lower limit
		 fwrite(&l.y, sizeof(float), 1, f); // upper limit
	  } else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag limited=0
   }
   else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag active=0

   if (c->DofActive(IKSys::RotZ))
   {
	  fprintf(fTXT, "RotZ active\n");
	  fwrite(&f1, sizeof(char), 1, f);  // scrittura flag active=1
	  if ((c->DofLowerLimited(IKSys::RotZ)) ||
          (c->DofUpperLimited(IKSys::RotZ)))
	  {
         l=c->DofLimits(IKSys::RotZ);
	     fprintf(fTXT, "RotZ is lower limited at: %f\n", l.x);
	     fprintf(fTXT, "RotZ is upper limited at: %f\n", l.y);
         fwrite(&f1, sizeof(char), 1, f);   // scrittura flag limited=1
		 fwrite(&l.x, sizeof(float), 1, f); // lower limit
		 fwrite(&l.y, sizeof(float), 1, f); // upper limit
	  } else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag limited=0
   }
   else fwrite(&f0, sizeof(char), 1, f);  // scrittura flag active=0



   INodeTab stab = c->IKChains(IKSys::SlidingJoint);
   INodeTab rtab = c->IKChains(IKSys::RotationalJoint);
   i=stab.Count();
   for (j=0; j<i; j++)
   {
	  node=stab[j];
	  fprintf(fTXT, "SlidingJoin tab, node %d: %s\n", j, node->GetName());
   }
   i=rtab.Count();
   for (j=0; j<i; j++)
   {
	  node=rtab[j];
	  fprintf(fTXT, "RotationalJoin tab, node %d: %s\n", j, node->GetName());
   }
}



void SkeletonExporter::export_MAXbone(INode *node)
{
  // Control *c, *cont;
  // int is_IK;
  Matrix3 ptm;
  Point3 row;
  INode *padre=node->GetParentNode();
  ObjectState os = node->EvalWorldState(0);


  fprintf(fTXT, "Bone presente\n");

  export_geometric_object(node, BONE_OBJECT, NULL);
  if (makeADO) fprintf(fADO, "  }\n\n");

/*
  cont = node->GetTMController();
  fprintf(fTXT, "TMController Bone: cid=(%X, %X)\n", cont->ClassID().PartA(), cont->ClassID().PartB());
  IIKControl *pIKbone=(IIKControl*)cont->GetInterface(I_IKCONTROL);
  if (pIKbone) is_IK=1;
  else is_IK=0;

  // flag padre + nome padre + flag_is_IK + size dei parametri
  // di IK +  world position + pivot
  write_chunk_header(fA3D, BONE_OBJECT_ID, node->GetName(),
	                 4+sf+4+get_IIKControl_size(pIKbone)+12*4+12);

  // flag che mi dice se ho i parametri di IK (joint active & limits)
  fwrite(&is_IK, sizeof(int), 1, fA3D);
  if (is_IK != 0)
  {
	 // esporto i parametri sul joint
     fprintf(fTXT, "IIKControl interface OK!\n");
     export_IIKControl(pIKbone, fA3D);
  }
*/
  fprintf(fTXT, "\n\n");
}



void SkeletonExporter::export_IKChain(INode *node)
{
  Control *cont;
  int sj, ej;
  unsigned char uno=1;
  Matrix3 ptm;
  Point3 row;
  INode *padre=node->GetParentNode();
  ObjectState os = node->EvalWorldState(0);
  INode *goal;

  fprintf(fTXT, "IK chain presente\n");

  cont = node->GetTMController();
  fprintf(fTXT, "TMController Chain: cid=(%X, %X)\n", cont->ClassID().PartA(), cont->ClassID().PartB());
  IIKChainControl* pIKgoal = (IIKChainControl*)cont->GetInterface(I_IKCHAINCONTROL);
  if (pIKgoal)
  {
     fprintf(fTXT, "IIKChainControl interface OK!\n");
	 fprintf(fTXT, "Start joint: %s\n", pIKgoal->StartJoint()->GetName());
	 fprintf(fTXT, "End joint: %s\n", pIKgoal->EndJoint()->GetName());
	 Point3 initEE = pIKgoal->InitEEAxis(0);
	 fprintf(fTXT, "Initial (time=0) End Effector Axis: %f, %f, %f\n",
		     initEE.x, initEE.y, initEE.z);

	 Point3 initNormalPlane = pIKgoal->InitPlane(0);
	 fprintf(fTXT, "Initial (time=0) Normal Plane: %f, %f, %f\n",
		     initNormalPlane.x, initNormalPlane.y, initNormalPlane.z);

	 goal=pIKgoal->GetNode();
	 fprintf(fTXT, "End Effector name: %s\n", goal->GetName());
     ptm=goal->GetNodeTM(0);
	 row=ptm.GetRow(3);
	 fprintf(fTXT, "End Effector world position (t=0): %f, %f, %f\n",
		     row.x, row.y, row.z);

     sj=strlen(pIKgoal->StartJoint()->GetName())+1;
     ej=strlen(pIKgoal->EndJoint()->GetName())+1;

     // nome padre joint base (start) + nome joint finale (effettore)
	 // + end effector axis + initial normal plane + end effector
	 // world position + IK constrait every frame
     write_chunk_header(fA3D, IKCHAIN_OBJECT_ID, node->GetName(),
	                    sj+ej+12+12+12+1);
     write_string0(fA3D, pIKgoal->StartJoint()->GetName());
     write_string0(fA3D, pIKgoal->EndJoint()->GetName());
	 fwrite(&initEE.x, sizeof(float), 1, fA3D);
	 fwrite(&initEE.y, sizeof(float), 1, fA3D);
	 fwrite(&initEE.z, sizeof(float), 1, fA3D);
	 fwrite(&initNormalPlane.x, sizeof(float), 1, fA3D);
	 fwrite(&initNormalPlane.y, sizeof(float), 1, fA3D);
	 fwrite(&initNormalPlane.z, sizeof(float), 1, fA3D);
	 fwrite(&row.x, sizeof(float), 1, fA3D);
	 fwrite(&row.y, sizeof(float), 1, fA3D);
	 fwrite(&row.z, sizeof(float), 1, fA3D);
	 fwrite(&uno, sizeof(unsigned char), 1, fA3D);
  }

  fprintf(fTXT, "\n\n");
}