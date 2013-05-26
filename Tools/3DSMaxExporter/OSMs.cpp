#include "SceneExport.h"
#include "A3Dfile.h"
#include "OSMs.h"
#include "extrasdk.h"

int SkeletonExporter::KnownModifiers(INode *node)
// vale come segue:
// 1 = non ha nessun modificatore
// 2 = ha solo il physic
// 3 = non ha il physic ma in cascata ha solo modificatori di
//     tipo TWISTER, RIPPLE, BEND, MELT, STRETCH, TAPER, FFD
//     (e quelli che supportero' nell'engine)
// 4 = ha modificatori implementati ma anche non implementati
// 5 = ha solo lo skin
{
  int state, ModStackIndex;
  Object* ObjectPtr;

  if (!node) return(1);
  ObjectPtr = node->GetObjectRef();
  if (!ObjectPtr) return(1);

  state=1;

  while ((ObjectPtr) && (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID))
  {
    IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);
    ModStackIndex = 0;
    while (ModStackIndex < DerivedObjectPtr->NumModifiers())
	{
       Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);
       if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
	   // il physic vince su tutti e per risultati corretti
	   // deve essere l'unico modificatore presente; se la mesh
	   // su cui applicare il physic la si vuole modificata per
	   // esigenze di modello con altri modificatori, prima di
	   // usare il physic bisogna convertire in editable mesh
	   // proprio prima di usare il physic
	   {
		  state=2;
          return(state);
	   }
	   else
       Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);
       if (ModifierPtr->ClassID() == SKIN_CLASSID)
	   // lo skin vince su tutti e per risultati corretti
	   // deve essere l'unico modificatore presente; se la mesh
	   // su cui applicare lo skin la si vuole modificata per
	   // esigenze di modello con altri modificatori, prima di
	   // usare lo skin bisogna convertire in editable mesh
	   // proprio prima di usare lo skin
	   {
		  state=5;
          return(state);
	   }
	   else
       if ( // Twist OSM
		    (ModifierPtr->ClassID() == Class_ID(TWISTOSM_CLASS_ID, 1))      ||
		    (ModifierPtr->ClassID() == Class_ID(TWISTOSM_CLASS_ID, 0))      ||
			// Ripple OSM
		    (ModifierPtr->ClassID() == Class_ID(SINEWAVE_OMOD_CLASS_ID, 0)) ||
			// Bend OSM
			(ModifierPtr->ClassID() == Class_ID(BENDOSM_CLASS_ID, 0))       ||
			// Melt OSM
			(ModifierPtr->ClassID() == Class_ID(MELT_ID1, MELT_ID2))        ||
			// Stretch OSM
			(ModifierPtr->ClassID() == Class_ID(STRETCHOSM_CLASS_ID1, STRETCHOSM_CLASS_ID2)) ||
			// Taper OSM
			(ModifierPtr->ClassID() == Class_ID(TAPEROSM_CLASS_ID, 0)) ||
			// FFD4x4x4, FFD3x3x3, FFD2x2x2
			(ModifierPtr->ClassID() == FFD44_CLASS_ID) ||
            (ModifierPtr->ClassID() == FFD33_CLASS_ID) ||
			(ModifierPtr->ClassID() == FFD22_CLASS_ID) ||
			// Noise OSM
			(ModifierPtr->ClassID() == Class_ID(NOISEOSM_CLASS_ID, 0))
		  )
	   {
		  if (state==1) state=3;
	   }
	   else
	   {
		 fprintf(fTXT, "mod cid A=%X, B=%X\n", ModifierPtr->ClassID().PartA(), ModifierPtr->ClassID().PartB());
		 // modifier neutri che non cambiano lo stato
		 if (ModifierPtr->ClassID() != Class_ID(UVWMAPOSM_CLASS_ID, 0))
           state=4;
	   }

       ModStackIndex++;
	}
    ObjectPtr = DerivedObjectPtr->GetObjRef();
  }
  
  return(state);
}

//--------------------------------------------------------------------

void SkeletonExporter::export_OSM_gizmo_controllers(SimpleMod *sm, INode *node, char ordinale, FILE *f)
{
  Control *c;
  int size_key;

  // --------------------  POSITION CONTROLLER  ----------------------
  c=sm->tmControl->GetPositionController();
  fprintf(fTXT, "tmControl->GetPositionController: %p, %d\n", c, c->NumKeys());
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Osm position track present.");
     write_chunk_header(f, OSM_POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_point3_track(c, 1, f);
	 fwrite(&ordinale, sizeof(char), 1, f);
  } // esportiamo comunque una key di posizione TCB
  else
  {
	fprintf(fTXT, "Osm position track present. (1 key case)");
	size_key=36;
    write_chunk_header(f, OSM_POSITION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key+1);
    export_1key_point3_track(c, 1, f);
	fwrite(&ordinale, sizeof(char), 1, f);
  }

  // --------------------  ROTATION CONTROLLER  ----------------------
  c=sm->tmControl->GetRotationController();
  fprintf(fTXT, "tmControl->GetRotationController: %p, %d\n", c, c->NumKeys());
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=40;
	 else size_key=20;
	 fprintf(fTXT, "Osm rotation track present.");
     write_chunk_header(f, OSM_ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_rot_track(c, f);
	 fwrite(&ordinale, sizeof(char), 1, f);
  } // esportiamo comunque una key di rotazione TCB
  else
  {
	fprintf(fTXT, "Osm rotation track present. (1 key case)");
	size_key=40;
    write_chunk_header(f, OSM_ROTATION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key+1);
    export_1key_rot_track(c, f);
	fwrite(&ordinale, sizeof(char), 1, f);
  }
  
  // ---------------------  SCALE CONTROLLER  ------------------------
  c=sm->tmControl->GetScaleController();
  fprintf(fTXT, "tmControl->GetScaleController: %p, %d\n", c, c->NumKeys());
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Osm scaling track present.");
     write_chunk_header(f, OSM_SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_scale_track(c, f);
	 fwrite(&ordinale, sizeof(char), 1, f);
  } // esportiamo comunque una key di scaling TCB
  else
  {
	fprintf(fTXT, "Osm scaling track present. (1 key case)");
	size_key=36;
    write_chunk_header(f, OSM_SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key+1);
    export_1key_scale_track(c, f);
	fwrite(&ordinale, sizeof(char), 1, f);
  }
}


//********************************************************************
//************************      TWIST      ***************************
//********************************************************************

void SkeletonExporter::export_OSM_twist(Modifier *twm, char ordinale, INode *node)
{
   float angle, bias, uplim, lowlim;
   int dolim, axis, size_key;
   Point3 cpos;  // center pos
   Interval valid;
   Control *c;
   
   SimpleMod *sm;
   sm=(SimpleMod *)twm;


   sm->pblock->GetValue(PB_TWIST_ANGLE, 0, angle, FOREVER);	
   sm->pblock->GetValue(PB_TWIST_AXIS, 0, axis, FOREVER);
   sm->pblock->GetValue(PB_TWIST_FROM, 0, lowlim, FOREVER);
   sm->pblock->GetValue(PB_TWIST_TO, 0, uplim, FOREVER);
   sm->pblock->GetValue(PB_TWIST_BIAS, 0, bias, FOREVER);
   sm->pblock->GetValue(PB_TWIST_DOREGION, 0, dolim, FOREVER);
   sm->posControl->GetValue(0, &cpos, valid);

   write_chunk_header(fA3D, TWIST_MODIFIER_ID,
		              node->GetName(), 37);
   
   fwrite(&angle, sizeof(float), 1, fA3D);
   fwrite(&bias, sizeof(float), 1, fA3D);
   fwrite(&axis, sizeof(int), 1, fA3D);
   fwrite(&uplim, sizeof(float), 1, fA3D);
   fwrite(&lowlim, sizeof(float), 1, fA3D);
   fwrite(&dolim, sizeof(int), 1, fA3D);
   fwrite(&cpos.x, sizeof(float), 1, fA3D);
   fwrite(&cpos.y, sizeof(float), 1, fA3D);
   fwrite(&cpos.z, sizeof(float), 1, fA3D);
   fwrite(&ordinale, sizeof(char), 1, fA3D);

   fprintf(fTXT, "angle: %f \n", angle);
   fprintf(fTXT, "bias: %f \n", bias);
   fprintf(fTXT, "axis: %d \n", axis);
   fprintf(fTXT, "upper limit: %f \n", uplim);
   fprintf(fTXT, "lower limit: %f \n", lowlim);
   fprintf(fTXT, "use limits: %d \n", dolim);
   fprintf(fTXT, "center pos: %f, %f, %f \n", cpos.x, cpos.y, cpos.z);

   // ****** KEYFRAME: ANGLE ******
   c=sm->pblock->GetController(PB_TWIST_ANGLE);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Twist angle track present.\n");
     write_chunk_header(fA3D, TWIST_ANGLE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, (float)(3.141592654/180), fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: ANGLE ******
   c=sm->pblock->GetController(PB_TWIST_BIAS);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Twist angle track present.\n");
     write_chunk_header(fA3D, TWIST_BIAS_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: LOW LIMIT ******
   c=sm->pblock->GetController(PB_TWIST_FROM);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Twist lowlimit track present.\n");
     write_chunk_header(fA3D, TWIST_LOWLIMIT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: UPPER LIMIT ******
   c=sm->pblock->GetController(PB_TWIST_TO);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Twist uplimit track present.\n");
     write_chunk_header(fA3D, TWIST_UPLIMIT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }


   export_OSM_gizmo_controllers(sm, node, ordinale, fA3D);
}


//********************************************************************
//************************      RIPPLE      **************************
//********************************************************************

void SkeletonExporter::export_OSM_ripple(Modifier *twm, char ordinale, INode *node)
{
   float amp1, amp2, wlen, phase, decay;
   Point3 cpos;  // center pos
   int size_key;
   Interval valid;
   Control *c;
   
   SimpleMod *sm;
   sm=(SimpleMod *)twm;

   sm->pblock->GetValue(PB_RIPPLE_AMPLITUDE, 0, amp1, FOREVER);
   sm->pblock->GetValue(PB_RIPPLE_AMPLITUDE2, 0, amp2, FOREVER);
   sm->pblock->GetValue(PB_RIPPLE_WAVELEN, 0, wlen, FOREVER);
   sm->pblock->GetValue(PB_RIPPLE_PHASE, 0, phase, FOREVER);
   sm->pblock->GetValue(PB_RIPPLE_DECAY, 0, decay, FOREVER);
   sm->posControl->GetValue(0, &cpos, valid);

   write_chunk_header(fA3D, RIPPLE_MODIFIER_ID,
		              node->GetName(), 33);
   
   fwrite(&amp1, sizeof(float), 1, fA3D);
   fwrite(&amp2, sizeof(float), 1, fA3D);
   fwrite(&wlen, sizeof(int), 1, fA3D);
   fwrite(&phase, sizeof(float), 1, fA3D);
   fwrite(&decay, sizeof(float), 1, fA3D);
   fwrite(&cpos.x, sizeof(float), 1, fA3D);
   fwrite(&cpos.y, sizeof(float), 1, fA3D);
   fwrite(&cpos.z, sizeof(float), 1, fA3D);
   fwrite(&ordinale, sizeof(char), 1, fA3D);

   fprintf(fTXT, "amplitude1: %f \n", amp1);
   fprintf(fTXT, "amplitude2: %f \n", amp2);
   fprintf(fTXT, "wave lenght: %f \n", wlen);
   fprintf(fTXT, "phase: %f \n", phase);
   fprintf(fTXT, "decay: %f \n", decay);
   fprintf(fTXT, "center pos: %f, %f, %f \n", cpos.x, cpos.y, cpos.z);

   // ****** KEYFRAME: AMPLITUDE1 ******
   c=sm->pblock->GetController(PB_RIPPLE_AMPLITUDE);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Ripple amplitude1 track present.\n");
     write_chunk_header(fA3D, RIPPLE_AMP1_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: AMPLITUDE2 ******
   c=sm->pblock->GetController(PB_RIPPLE_AMPLITUDE2);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Ripple amplitude2 track present.\n");
     write_chunk_header(fA3D, RIPPLE_AMP2_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: WAVELENGHT ******
   c=sm->pblock->GetController(PB_RIPPLE_WAVELEN);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Ripple wavelenght track present.\n");
     write_chunk_header(fA3D, RIPPLE_WAVELEN_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: PHASE ******
   c=sm->pblock->GetController(PB_RIPPLE_PHASE);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Ripple phase track present.\n");
     write_chunk_header(fA3D, RIPPLE_PHASE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: DECAY ******
   c=sm->pblock->GetController(PB_RIPPLE_DECAY);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Ripple decay track present.\n");
     write_chunk_header(fA3D, RIPPLE_DECAY_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   export_OSM_gizmo_controllers(sm, node, ordinale, fA3D);
}


//********************************************************************
//*************************      BEND      ***************************
//********************************************************************

void SkeletonExporter::export_OSM_bend(Modifier *twm, char ordinale, INode *node)
{
   float angle, dir, uplim, lowlim;
   int dolim, axis, size_key;
   Point3 cpos;  // center pos
   Interval valid;
   Control *c;
   
   SimpleMod2 *sm;
   sm=(SimpleMod2 *)twm;


   sm->pblock2->GetValue(PB_BEND_ANGLE, 0, angle, FOREVER);	
   sm->pblock2->GetValue(PB_BEND_AXIS, 0, axis, FOREVER);
   sm->pblock2->GetValue(PB_BEND_FROM, 0, lowlim, FOREVER);
   sm->pblock2->GetValue(PB_BEND_TO, 0, uplim, FOREVER);
   sm->pblock2->GetValue(PB_BEND_DIR, 0, dir, FOREVER);
   sm->pblock2->GetValue(PB_BEND_DOREGION, 0, dolim, FOREVER);
   sm->posControl->GetValue(0, &cpos, valid);

   write_chunk_header(fA3D, BEND_MODIFIER_ID,
		              node->GetName(), 37);
   
   fwrite(&angle, sizeof(float), 1, fA3D);
   fwrite(&dir, sizeof(float), 1, fA3D);
   fwrite(&axis, sizeof(int), 1, fA3D);
   fwrite(&uplim, sizeof(float), 1, fA3D);
   fwrite(&lowlim, sizeof(float), 1, fA3D);
   fwrite(&dolim, sizeof(int), 1, fA3D);
   fwrite(&cpos.x, sizeof(float), 1, fA3D);
   fwrite(&cpos.y, sizeof(float), 1, fA3D);
   fwrite(&cpos.z, sizeof(float), 1, fA3D);
   fwrite(&ordinale, sizeof(char), 1, fA3D);

   fprintf(fTXT, "angle: %f \n", angle);
   fprintf(fTXT, "direction: %f \n", dir);
   fprintf(fTXT, "axis: %d \n", axis);
   fprintf(fTXT, "upper limit: %f \n", uplim);
   fprintf(fTXT, "lower limit: %f \n", lowlim);
   fprintf(fTXT, "use limits: %d \n", dolim);
   fprintf(fTXT, "center pos: %f, %f, %f \n", cpos.x, cpos.y, cpos.z);

   // ****** KEYFRAME: ANGLE ******
   c=sm->pblock2->GetController(PB_BEND_ANGLE);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Bend angle track present.\n");
     write_chunk_header(fA3D, BEND_ANGLE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, (float)(3.141592654/180), fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }


   // ****** KEYFRAME: DIRECTION ******
   c=sm->pblock2->GetController(PB_BEND_DIR);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Bend direction track present.\n");
     write_chunk_header(fA3D, BEND_DIR_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, (float)(3.141592654/180), fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }


   // ****** KEYFRAME: UPPER LIMIT ******
   c=sm->pblock2->GetController(PB_BEND_TO);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Bend uplimit track present.\n");
     write_chunk_header(fA3D, BEND_UPLIMIT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }


   // ****** KEYFRAME: LOWER LIMIT ******
   c=sm->pblock2->GetController(PB_BEND_FROM);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Bend lowlimit track present.\n");
     write_chunk_header(fA3D, BEND_LOWLIMIT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   export_OSM_gizmo_controllers(sm, node, ordinale, fA3D);
}


//********************************************************************
//*************************      MELT      ***************************
//********************************************************************

void SkeletonExporter::export_OSM_melt(Modifier *twm, char ordinale, INode *node)
{
   float amount, spread, solidity, zbr;
   int axis, negaxis, size_key, viscosity_type, confine;
   Point3 cpos;  // center pos
   Interval valid;
   Control *c;
   
   SimpleMod *sm;
   sm=(SimpleMod *)twm;

   sm->pblock->GetValue(PB_MELT_AMT, 0, amount, FOREVER);	
   sm->pblock->GetValue(PB_MELT_YB, 0, spread, FOREVER);
   sm->pblock->GetValue(PB_MELT_VISTYPE, 0, viscosity_type, FOREVER);
   switch (viscosity_type)
   {
      case 0: solidity=2.0f;
              break;
      case 1: solidity=12.0f;
              break;
      case 2: solidity=0.4f;
              break;
      case 3: solidity=0.7f;
              break;
      case 4: sm->pblock->GetValue(PB_MELT_VISVAL, 0, solidity, FOREVER);
              break;
   }
   sm->pblock->GetValue(PB_MELT_AXIS, 0, axis, FOREVER);
   sm->pblock->GetValue(PB_MELT_NEGAXIS, 0, negaxis, FOREVER);
   sm->pblock->GetValue(PB_MELT_CONFINE, 0, confine, FOREVER);
   sm->pblock->GetValue(PB_MELT_ZB, 0, zbr, FOREVER);
   sm->posControl->GetValue(0, &cpos, valid);

   write_chunk_header(fA3D, MELT_MODIFIER_ID,
		              node->GetName(), 41);
   
   fwrite(&amount, sizeof(float), 1, fA3D);
   fwrite(&spread, sizeof(float), 1, fA3D);
   fwrite(&solidity, sizeof(float), 1, fA3D);
   fwrite(&axis, sizeof(int), 1, fA3D);
   fwrite(&negaxis, sizeof(int), 1, fA3D);
   fwrite(&confine, sizeof(int), 1, fA3D);
   fwrite(&zbr, sizeof(float), 1, fA3D);
   fwrite(&cpos.x, sizeof(float), 1, fA3D);
   fwrite(&cpos.y, sizeof(float), 1, fA3D);
   fwrite(&cpos.z, sizeof(float), 1, fA3D);
   fwrite(&ordinale, sizeof(char), 1, fA3D);

   fprintf(fTXT, "amount: %f \n", amount);
   fprintf(fTXT, "spread: %f \n", spread);
   fprintf(fTXT, "solidity: %f \n", solidity);
   fprintf(fTXT, "axis: %d \n", axis);
   fprintf(fTXT, "negaxis: %d \n", negaxis);
   fprintf(fTXT, "confine: %d \n", confine);
   fprintf(fTXT, "zbr: %f \n", zbr);
   fprintf(fTXT, "center pos: %f, %f, %f \n", cpos.x, cpos.y, cpos.z);

   // ****** KEYFRAME: AMOUNT ******
   c=sm->pblock->GetController(PB_MELT_AMT);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Melt amount track present.\n");
     write_chunk_header(fA3D, MELT_AMOUNT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: SPREAD ******
   c=sm->pblock->GetController(PB_MELT_YB);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Melt spread track present.\n");
     write_chunk_header(fA3D, MELT_SPREAD_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   export_OSM_gizmo_controllers(sm, node, ordinale, fA3D);
}


//********************************************************************
//************************      TAPER      ***************************
//********************************************************************

void SkeletonExporter::export_OSM_taper(Modifier *twm, char ordinale, INode *node)
{
   float amount, curve, uplim, lowlim;
   int axis, effectaxis, dolim, symmetry, size_key;
   Point3 cpos;  // center pos
   Interval valid;
   Control *c;
   
   SimpleMod *sm;
   sm=(SimpleMod *)twm;

   sm->pblock->GetValue(PB_TAPER_AMT, 0, amount, FOREVER);	
   sm->pblock->GetValue(PB_TAPER_CRV, 0, curve, FOREVER);
   sm->pblock->GetValue(PB_TAPER_AXIS, 0, axis, FOREVER);
   sm->pblock->GetValue(PB_TAPER_EFFECTAXIS, 0, effectaxis, FOREVER);
   sm->pblock->GetValue(PB_TAPER_SYMMETRY, 0, symmetry, FOREVER);
   sm->pblock->GetValue(PB_TAPER_FROM, 0, lowlim, FOREVER);
   sm->pblock->GetValue(PB_TAPER_TO, 0, uplim, FOREVER);
   sm->pblock->GetValue(PB_TAPER_DOREGION, 0, dolim, FOREVER);
   sm->posControl->GetValue(0, &cpos, valid);

   write_chunk_header(fA3D, TAPER_MODIFIER_ID,
		              node->GetName(), 45);
   
   fwrite(&amount, sizeof(float), 1, fA3D);
   fwrite(&curve, sizeof(float), 1, fA3D);
   fwrite(&axis, sizeof(int), 1, fA3D);
   fwrite(&effectaxis, sizeof(int), 1, fA3D);
   fwrite(&symmetry, sizeof(int), 1, fA3D);
   fwrite(&uplim, sizeof(float), 1, fA3D);
   fwrite(&lowlim, sizeof(float), 1, fA3D);
   fwrite(&dolim, sizeof(int), 1, fA3D);
   fwrite(&cpos.x, sizeof(float), 1, fA3D);
   fwrite(&cpos.y, sizeof(float), 1, fA3D);
   fwrite(&cpos.z, sizeof(float), 1, fA3D);
   fwrite(&ordinale, sizeof(char), 1, fA3D);

   fprintf(fTXT, "amount: %f \n", amount);
   fprintf(fTXT, "curve: %f \n", curve);
   fprintf(fTXT, "axis: %d \n", axis);
   fprintf(fTXT, "effectaxis: %d \n", effectaxis);
   fprintf(fTXT, "symmetry: %d \n", symmetry);
   fprintf(fTXT, "upper limit: %f \n", uplim);
   fprintf(fTXT, "lower limit: %f \n", lowlim);
   fprintf(fTXT, "use limits: %d \n", dolim);
   fprintf(fTXT, "center pos: %f, %f, %f \n", cpos.x, cpos.y, cpos.z);

   // ****** KEYFRAME: ANGLE ******
   c=sm->pblock->GetController(PB_TAPER_AMT);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Taper amount track present.\n");
     write_chunk_header(fA3D, TAPER_AMOUNT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }


   // ****** KEYFRAME: CURVE ******
   c=sm->pblock->GetController(PB_TAPER_CRV);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Taper curve track present.\n");
     write_chunk_header(fA3D, TAPER_CURVE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }


   // ****** KEYFRAME: UPPER LIMIT ******
   c=sm->pblock->GetController(PB_TAPER_TO);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Taper uplimit track present.\n");
     write_chunk_header(fA3D, TAPER_UPLIMIT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }


   // ****** KEYFRAME: LOWER LIMIT ******
   c=sm->pblock->GetController(PB_TAPER_FROM);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Taper lowlimit track present.\n");
     write_chunk_header(fA3D, TAPER_LOWLIMIT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   export_OSM_gizmo_controllers(sm, node, ordinale, fA3D);
}


//********************************************************************
//************************      NOISE      ***************************
//********************************************************************

void SkeletonExporter::export_OSM_noise(Modifier *twm, char ordinale, INode *node)
{
   float scale, rough, iterations, freq;
   TimeValue phase;
   int seed, fractal, animate, size_key;
   Point3 cpos, strength;  // center pos
   Interval valid;
   Control *c;
   
   SimpleMod *sm;
   sm=(SimpleMod *)twm;
   
   sm->pblock->GetValue(PB_NOISE_SEED, 0, seed, FOREVER);
   sm->pblock->GetValue(PB_NOISE_SCALE, 0, scale, FOREVER);
   sm->pblock->GetValue(PB_NOISE_FRACTAL, 0, fractal, FOREVER);
   sm->pblock->GetValue(PB_NOISE_ROUGH, 0, rough, FOREVER);
   sm->pblock->GetValue(PB_NOISE_ITERATIONS, 0, iterations, FOREVER);
   sm->pblock->GetValue(PB_NOISE_ANIMATE, 0, animate, FOREVER);
   sm->pblock->GetValue(PB_NOISE_FREQ, 0, freq, FOREVER);
   sm->pblock->GetValue(PB_NOISE_PHASE, 0, phase, FOREVER);
   sm->pblock->GetValue(PB_NOISE_STRENGTH, 0, strength, FOREVER);
   sm->posControl->GetValue(0, &cpos, valid);

   write_chunk_header(fA3D, NOISE_MODIFIER_ID,
		              node->GetName(), 45);

   fprintf(fTXT, "Seed: %d \n", seed);
   fprintf(fTXT, "Scale: %f \n", scale);
   fprintf(fTXT, "Fractal: %d \n", fractal);
   fprintf(fTXT, "Rough: %f \n", rough);
   fprintf(fTXT, "Iterations: %f \n", iterations);
   fprintf(fTXT, "Animate noise: %d \n", animate);
   fprintf(fTXT, "Frequency: %f \n", freq);
   fprintf(fTXT, "Phase: %d \n", phase);
   fprintf(fTXT, "Strength0: %f, %f, %f\n", strength.x, strength.y, strength.z);

   fwrite(&seed, sizeof(int), 1, fA3D);
   fwrite(&scale, sizeof(float), 1, fA3D);
   fwrite(&fractal, sizeof(int), 1, fA3D);
   fwrite(&rough, sizeof(float), 1, fA3D);
   fwrite(&iterations, sizeof(float), 1, fA3D);
   fwrite(&animate, sizeof(int), 1, fA3D);
   fwrite(&freq, sizeof(float), 1, fA3D);
   fwrite(&phase, sizeof(int), 1, fA3D);
   fwrite(&strength.x, sizeof(float), 1, fA3D);
   fwrite(&strength.y, sizeof(float), 1, fA3D);
   fwrite(&strength.z, sizeof(float), 1, fA3D);
   fwrite(&ordinale, sizeof(char), 1, fA3D);

   // ****** KEYFRAME: SCALE ******
   c=sm->pblock->GetController(PB_NOISE_SCALE);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Noise scale track present.\n");
     write_chunk_header(fA3D, NOISE_SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: FREQUENCY ******
   c=sm->pblock->GetController(PB_NOISE_FREQ);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Noise frequency track present.\n");
     write_chunk_header(fA3D, NOISE_FREQ_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: PHASE ******
   c=sm->pblock->GetController(PB_NOISE_PHASE);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Noise phase track present.\n");
     write_chunk_header(fA3D, NOISE_PHASE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: STRENGTH ******
   c=sm->pblock->GetController(PB_NOISE_STRENGTH);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Noise strength track present.\n");
     write_chunk_header(fA3D, NOISE_STRENGTH_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_point3_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   export_OSM_gizmo_controllers(sm, node, ordinale, fA3D);
}


//********************************************************************
//************************     STRETCH     ***************************
//********************************************************************

void SkeletonExporter::export_OSM_stretch(Modifier *twm, char ordinale, INode *node)
{
   float stretch, amplify, uplim, lowlim;
   int axis, dolim, size_key;
   Point3 cpos;  // center pos
   Interval valid;
   Control *c;
   
   SimpleMod *sm;
   sm=(SimpleMod *)twm;

   sm->pblock->GetValue(PB_STRETCH_STRETCH, 0, stretch, FOREVER);	
   sm->pblock->GetValue(PB_STRETCH_AMPLIFY, 0, amplify, FOREVER);
   sm->pblock->GetValue(PB_STRETCH_AXIS, 0, axis, FOREVER);
   sm->pblock->GetValue(PB_STRETCH_FROM, 0, lowlim, FOREVER);
   sm->pblock->GetValue(PB_STRETCH_TO, 0, uplim, FOREVER);
   sm->pblock->GetValue(PB_STRETCH_DOREGION, 0, dolim, FOREVER);
   sm->posControl->GetValue(0, &cpos, valid);

   write_chunk_header(fA3D, STRETCH_MODIFIER_ID,
		              node->GetName(), 37);
   
   fwrite(&stretch, sizeof(float), 1, fA3D);
   fwrite(&amplify, sizeof(float), 1, fA3D);
   fwrite(&axis, sizeof(int), 1, fA3D);
   fwrite(&uplim, sizeof(float), 1, fA3D);
   fwrite(&lowlim, sizeof(float), 1, fA3D);
   fwrite(&dolim, sizeof(int), 1, fA3D);
   fwrite(&cpos.x, sizeof(float), 1, fA3D);
   fwrite(&cpos.y, sizeof(float), 1, fA3D);
   fwrite(&cpos.z, sizeof(float), 1, fA3D);
   fwrite(&ordinale, sizeof(char), 1, fA3D);

   fprintf(fTXT, "stretch: %f \n", stretch);
   fprintf(fTXT, "amplify: %f \n", amplify);
   fprintf(fTXT, "axis: %d \n", axis);
   fprintf(fTXT, "upper limit: %f \n", uplim);
   fprintf(fTXT, "lower limit: %f \n", lowlim);
   fprintf(fTXT, "use limits: %d \n", dolim);
   fprintf(fTXT, "center pos: %f, %f, %f \n", cpos.x, cpos.y, cpos.z);

   // ****** KEYFRAME: STRETCH ******
   c=sm->pblock->GetController(PB_STRETCH_STRETCH);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Stretch stretch track present.\n");
     write_chunk_header(fA3D, STRETCH_STRETCH_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: AMPLIFY ******
   c=sm->pblock->GetController(PB_STRETCH_AMPLIFY);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Stretch amplify track present.\n");
     write_chunk_header(fA3D, STRETCH_AMPLIFY_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   // ****** KEYFRAME: UPPER LIMIT ******
   c=sm->pblock->GetController(PB_STRETCH_TO);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Stretch uplimit track present.\n");
     write_chunk_header(fA3D, STRETCH_UPLIMIT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }


   // ****** KEYFRAME: LOWER LIMIT ******
   c=sm->pblock->GetController(PB_STRETCH_FROM);
   if ((c) && (c->NumKeys()>0))
   {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Stretch lowlimit track present.\n");
     write_chunk_header(fA3D, STRETCH_LOWLIMIT_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
   }

   export_OSM_gizmo_controllers(sm, node, ordinale, fA3D);
}


//********************************************************************
//*************************       FFD      ***************************
//********************************************************************

enum { ffd_params };
enum { ffd_disp_lattice, ffd_disp_source, ffd_deform, ffd_inpoints, ffd_outpoints, ffd_offset};

void SkeletonExporter::export_OSM_ffd(Modifier *twm, char ordinale, INode *node)
{
  IFFDMod<Modifier> *ffd;
  FFDMod *ffdm;
  Control *c, *pos, *rot, *scale;
  Point3 p;
  int i, np, npc, dim123, invol, size_key;
  Interval valid;

  if (twm->ClassID() == FFD44_CLASS_ID)
  {
	  fprintf(fTXT, "Type: 4x4x4\n");
	  dim123=4;
  }
  else
  if (twm->ClassID() == FFD33_CLASS_ID)
  {
	  fprintf(fTXT, "Type: 3x3x3\n");
	  dim123=3;
  }
  else
  if (twm->ClassID() == FFD22_CLASS_ID)
  {
	  fprintf(fTXT, "Type: 2x2x2\n");
  	  dim123=2;
  }

  ffd=(IFFDMod<Modifier>*)twm;
  ffdm=(FFDMod *)twm;
  np=ffd->NumPts();
  invol = !ffdm->pblock->GetInt(ffd_deform, 0);	
  write_chunk_header(fA3D, FFD_MODIFIER_ID,
                     node->GetName(), 4+4+4+4+4+
					                  np*3*4+
									  3*4+1);
  
  fwrite(&dim123, sizeof(int), 1, fA3D);  // dim1
  fwrite(&dim123, sizeof(int), 1, fA3D);  // dim2
  fwrite(&dim123, sizeof(int), 1, fA3D);  // dim3
  fwrite(&invol,  sizeof(int), 1, fA3D);  // Deformation type
  fwrite(&np,     sizeof(int), 1, fA3D);  // n.ro control point

  fprintf(fTXT, "Numero di punti: %d\n", np);
  for (i=0; i<np; i++)
  {
    p=ffd->GetPt(i);
	c=ffd->GetPtCont(i);
	// NB: non so perchè, ma se un punto di controllo è keyframato
	// la funzione GetPt ritorna la posizione finale, non quella
	// iniziale
	if (!c)
	{
	  fprintf(fTXT, "Control point %d: %f, %f, %f\n", i, p.x, p.y, p.z);
	  fwrite(&p.x, sizeof(float), 1, fA3D);
	  fwrite(&p.y, sizeof(float), 1, fA3D);
	  fwrite(&p.z, sizeof(float), 1, fA3D);
	}
	else
	{
	  c->GetValue(0, &p, valid);
	  fprintf(fTXT, "Control point %d: %f, %f, %f\n", i, p.x, p.y, p.z);
	  fwrite(&p.x, sizeof(float), 1, fA3D);
	  fwrite(&p.y, sizeof(float), 1, fA3D);
	  fwrite(&p.z, sizeof(float), 1, fA3D);
	}
  }

  // centro del lattice
  ffdm->tmControl->GetPositionController()->GetValue(0, &p, valid);
  fprintf(fTXT, "Center point: %f, %f, %f\n", p.x, p.y, p.z);
  fwrite(&p.x, sizeof(float), 1, fA3D);
  fwrite(&p.y, sizeof(float), 1, fA3D);
  fwrite(&p.z, sizeof(float), 1, fA3D);
  // ordinale, fine chunk
  fwrite(&ordinale, sizeof(char), 1, fA3D);

  // Lattice: varie informazioni
  c=ffdm->tmControl;
  fprintf(fTXT, "Lattice tmControl: %p\n", c);
  pos=c->GetPositionController();
  rot=c->GetRotationController();
  scale=c->GetScaleController();
  fprintf(fTXT, "tmControl->PosControl: %p\n", pos);
  fprintf(fTXT, "tmControl->RotControl: %p\n", rot);
  fprintf(fTXT, "tmControl->ScaleControl: %p\n", scale);

// ***********************************************************
// ***************   KEYFRAME DEL LATTICE   ******************
// ***********************************************************
  // POSITION CONTROLLER
  if ((pos) && (pos->NumKeys()>0))
  {
	 if (IsTCBControl(pos)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Lattice position track present.\n");
     write_chunk_header(fA3D, OSM_POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+pos->NumKeys()*size_key+1);
     export_point3_track(pos, 1, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
  }
  // esportiamo comunque una key di posizione TCB
  else
  {
	fprintf(fTXT, "Lattice position track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fA3D, OSM_POSITION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key+1);
    export_1key_point3_track(pos, 1, fA3D);
	fwrite(&ordinale, sizeof(char), 1, fA3D);
  }


  // ROTATION CONTROLLER
  if ((rot) && (rot->NumKeys()>0))
  {
	 if (IsTCBControl(rot)) size_key=40;
	 else size_key=20;
	 fprintf(fTXT, "Lattice rotation track present.\n");
     write_chunk_header(fA3D, OSM_ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+rot->NumKeys()*size_key+1);
     export_rot_track(rot, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
  }
  // esportiamo comunque una key di rotazione TCB
  else
  {
	fprintf(fTXT, "Lattice rotation track present. (1 key case)\n");
	size_key=40;
    write_chunk_header(fA3D, OSM_ROTATION_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key+1);
    export_1key_rot_track(rot, fA3D);
	fwrite(&ordinale, sizeof(char), 1, fA3D);
  }


   // SCALE CONTROLLER
  if ((scale) && (scale->NumKeys()>0))
  {
	 if (IsTCBControl(scale)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Lattice scaling track present.\n");
     write_chunk_header(fA3D, OSM_SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+scale->NumKeys()*size_key+1);
     export_scale_track(scale, fA3D);
	 fwrite(&ordinale, sizeof(char), 1, fA3D);
  }
  // esportiamo comunque una key di scaling TCB
  else
  {
	fprintf(fTXT, "Lattice scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fA3D, OSM_SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key+1);
    export_1key_scale_track(scale, fA3D);
	fwrite(&ordinale, sizeof(char), 1, fA3D);
  }

// ***********************************************************
// ************   KEYFRAME DEI CONTROL POINTS   **************
// ***********************************************************
  npc=ffd->NumPtConts();
  fprintf(fTXT, "Numero di punto-controlli: %d\n", npc);
  for (i=0; i<npc; i++)
  {
	c=ffd->GetPtCont(i);
    fprintf(fTXT, "Controller %d: %p\n", i, c);
	if ((c) && (c->NumKeys()>0))
	{
	   if (IsTCBControl(c)) size_key=36;
	   else
	   if (IsBezierControl(c)) size_key=40;
	   else size_key=16;
	   fprintf(fTXT, "Lattice control point track present.\n");
       write_chunk_header(fA3D, FFD_CONTROLPOINT_TRACK_ID,
	                      node->GetName(), 1+2+4+c->NumKeys()*size_key+1+4);
       export_point3_track(c, 1, fA3D);
	   fwrite(&ordinale, sizeof(char), 1, fA3D);
	   fwrite(&i, sizeof(int), 1, fA3D);
	}
  }
}