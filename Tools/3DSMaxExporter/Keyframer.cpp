//
// Esportazioni delle tracce/animazioni in generale
//
#include "SceneExport.h"
#include "A3Dfile.h"
#include "ExtraSDK.h"

BOOL SkeletonExporter::IsKnownController(Control* cont)
{
	ulong partA, partB;

	if (!cont)
		return FALSE;

	partA = cont->ClassID().PartA();
	partB = cont->ClassID().PartB();

	if (partB != 0x00)
		return FALSE;

	switch (partA)
	{
	  case TCBINTERP_POSITION_CLASS_ID:
	  case TCBINTERP_ROTATION_CLASS_ID:
	  case TCBINTERP_SCALE_CLASS_ID:
	  case HYBRIDINTERP_POSITION_CLASS_ID:
	  case HYBRIDINTERP_ROTATION_CLASS_ID:
	  case HYBRIDINTERP_SCALE_CLASS_ID:
	  case LININTERP_POSITION_CLASS_ID:
	  case LININTERP_ROTATION_CLASS_ID:
	  case LININTERP_SCALE_CLASS_ID: return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------

int SkeletonExporter::MaxORT2MyORT (int maxORT)
{
  switch (maxORT)
  {
    case ORT_CONSTANT: return(OOR_HOLD);
                      break;
    case ORT_LOOP: return(OOR_LOOP);
                   break;
    case ORT_CYCLE: return(OOR_CYCLE);
                    break;
    case ORT_OSCILLATE: return(OOR_PINGPONG);
                        break;
    case ORT_LINEAR: return(OOR_LINEAR);
                     break;
    case ORT_RELATIVE_REPEAT: return(OOR_RELATIVE_REPEAT);
                              break;
    default: return(OOR_HOLD);
             break;
  }
}


// ###########################################################
// ##################   TRACCE DI hiding   ###################
// ###########################################################

void SkeletonExporter::export_hide_track(Control *c, float scale, FILE *f)
{
  int numkeys;
  int keytime, i;
  char ORT_before, ORT_after;
  BKey bkey;
  unsigned char type=0;
  
  // ci devono essere almeno 1 key
  numkeys = c->NumKeys();
  if (numkeys==NOT_KEYFRAMEABLE || numkeys==0) return;
  
  if (c->ClassID()!=Class_ID(0x984b8d27,0x938f3e43)) return;

  BoolControl *bc = (BoolControl *)c;

  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));

  type=TCB_CONTROLLER;
  fwrite(&type, sizeof(unsigned char), 1, f);
  fwrite(&ORT_before, sizeof(char), 1, f);
  fwrite(&ORT_after, sizeof(char), 1, f);
  fwrite(&numkeys, sizeof(int), 1, f);

  for (i=0; i<numkeys; i++) 
  {
     bkey=bc->keys[i];
	 keytime = bkey.time/GetTicksPerFrame();
     fprintf(fTXT, "timepos : %d\n", keytime);
	 // timeline position
	 fwrite(&keytime, sizeof(int), 1, f);
  }
}


// ###########################################################
// ##################   TRACCE DI Point3   ###################
// ###########################################################

void SkeletonExporter::export_1key_point3_track(Control *c, float scale, FILE *f)
{
  int keytime=0;
  int numkeys=1;
  char ORT_before, ORT_after;
  Point3 pval;
  unsigned char type;
  Interval valid;
  float s=0;
  

  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));

  // Interpolazione TCB
  type=TCB_CONTROLLER;
  fwrite(&type, sizeof(unsigned char), 1, f);
  fwrite(&ORT_before, sizeof(char), 1, f);
  fwrite(&ORT_after, sizeof(char), 1, f);
  fwrite(&numkeys, sizeof(int), 1, f);
  fprintf(fTXT, " Type is TCB\n");
  fprintf(fTXT, "Number of keys : %d\n", numkeys);

  c->GetValue(0, &pval, valid);
  pval.x*=scale;
  pval.y*=scale;
  pval.z*=scale;
  //fprintf(fTXT, "key settings : t=%f, c=%f, b=%f, et=%f, ef=%f\n",
	//      s, s, s, s, s);
  fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
	      keytime, pval.x, pval.y, pval.z);

  // key values
  fwrite(&keytime, sizeof(int), 1, f);  // timepos=0
  write_point3(&pval, f);

  // key settings
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
}


void SkeletonExporter::export_point3_track(Control *c, float scale, FILE *f)
{
  int numkeys;
  int keytime;
  ITCBPoint3Key kTCB;
  IBezPoint3Key kBEZ, kBEZ2;
  ILinPoint3Key kLIN;
  Point3 T, l, p;
  char ORT_before, ORT_after;
  unsigned char type;
  float f0=0, dt;
  
//  if (!c) return;
//  if (!IsKnownController(c)) return;

  // ci devono essere almeno 1 key
  numkeys = c->NumKeys();
  if (numkeys==NOT_KEYFRAMEABLE || numkeys==0) return;
  
  // Get the keyframe interface
  IKeyControl *ikeys = GetKeyControlInterface(c);

  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));

  //  --------------------------------------------------
  // |  Differenziamo secondo il tipo di interpolazione |
  //  --------------------------------------------------


  // Interpolazione TCB
  if (IsTCBControl(c) && ikeys)
  {
	type=TCB_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);

    fprintf(fTXT, " Type is TCB\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
	
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kTCB);
	  kTCB.val.x*=scale;
	  kTCB.val.y*=scale;
	  kTCB.val.z*=scale;

	  //fprintf(fTXT, "key settings : t=%f, c=%f, b=%f, et=%f, ef=%f\n",
		//      kTCB.tens, kTCB.cont, kTCB.bias, kTCB.easeIn, kTCB.easeOut);

	  keytime = kTCB.time/GetTicksPerFrame();
      fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
		      keytime, kTCB.val.x, kTCB.val.y, kTCB.val.z);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);
	  // key values
      write_point3(&kTCB.val, f);
	  // key settings
	  fwrite(&kTCB.tens, sizeof(float), 1, f);
	  fwrite(&kTCB.cont, sizeof(float), 1, f);
	  fwrite(&kTCB.bias, sizeof(float), 1, f);
	  fwrite(&kTCB.easeIn, sizeof(float), 1, f);
	  fwrite(&kTCB.easeOut, sizeof(float), 1, f);
	}
  }
  else
  // Interpolazione BEZIER
  if (IsBezierControl(c) && ikeys)
  {
    type=BEZIER_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
	fprintf(fTXT, " Type is Bezier\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);

    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kBEZ);
      kBEZ.val.x*=scale;
      kBEZ.val.y*=scale;
      kBEZ.val.z*=scale;
	  keytime = kBEZ.time/GetTicksPerFrame();
      fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n", keytime, kBEZ.val.x, kBEZ.val.y, kBEZ.val.z);
	  fwrite(&keytime, sizeof(int), 1, f);
      write_point3(&kBEZ.val, f);

	  // incoming tangent
	  if (i==0)
	  {
         fwrite(&f0, sizeof(float), 3, f);
		 fprintf(fTXT, "inTan: %f, %f, %f\n", 0, 0, 0);
	  }
	  else
	  {
	     p=kBEZ.intan; l=kBEZ.inLength;
		 ikeys->GetKey(i-1, &kBEZ2);
         dt=(kBEZ2.time-kBEZ.time);
	     T.x=p.x*dt*scale;
	     T.y=p.y*dt*scale;
	     T.z=p.z*dt*scale;
	     fprintf(fTXT, "inTan: %f, %f, %f, %f\n", T.x, T.y, T.z, dt);
         write_point3(&T, f);
	  }
	  // outcoming tangent
	  if (i==numkeys-1)
	  {
         fwrite(&f0, sizeof(float), 3, f);
		 fprintf(fTXT, "outTan: %f, %f, %f\n", 0, 0, 0);
	  }
	  else
	  {
	     p=kBEZ.outtan; l=kBEZ.outLength;
		 ikeys->GetKey(i+1, &kBEZ2);
         dt=(kBEZ2.time-kBEZ.time);
	     T.x=p.x*dt*scale;
	     T.y=p.y*dt*scale;
	     T.z=p.z*dt*scale;
	     fprintf(fTXT, "outTan: %f, %f, %f %f\n", T.x, T.y, T.z, dt);
         write_point3(&T, f);
	  }
	}
  }
  else // Interpolazione LINEARE
  if (IsLinearControl(c) && ikeys)
  {
	type=LINEAR_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
    fprintf(fTXT, " Type is Linear\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kLIN);
      kLIN.val.x*=scale;
      kLIN.val.y*=scale;
      kLIN.val.z*=scale;
	  keytime = kLIN.time/GetTicksPerFrame();
	  fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
              keytime, kLIN.val.x, kLIN.val.y, kLIN.val.z);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);

	  // key values
      write_point3(&kLIN.val, f);
	}
  }
}



// ###########################################################
// ###################   TRACCE DI Float   ###################
// ###########################################################

void SkeletonExporter::export_1key_float_track(Control *c, float scale, FILE *f)
{
  int keytime=0;
  int numkeys=1;
  float fval;
  unsigned char type;
  char ORT_before, ORT_after;
  Interval valid;
  float s=0;

  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));
  
  // Interpolazione TCB
  type=TCB_CONTROLLER;
  fwrite(&type, sizeof(unsigned char), 1, f);
  fwrite(&ORT_before, sizeof(char), 1, f);
  fwrite(&ORT_after, sizeof(char), 1, f);
  fwrite(&numkeys, sizeof(int), 1, f);
  fprintf(fTXT, " Type is TCB\n");
  fprintf(fTXT, "Number of keys : %d\n", numkeys);

  c->GetValue(0, &fval, valid);
  fval*=scale;
  //fprintf(fTXT, "key settings : t=%f, c=%f, b=%f, et=%f, ef=%f\n",
	//	      s, s, s, s, s);
  fprintf(fTXT, "timepos : %d,  value %f\n",
	      keytime, fval);

  // key values
  fwrite(&keytime, sizeof(int), 1, f);  // timepos=0
  fwrite(&fval, sizeof(float), 1, f);

  // key settings
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
}


void SkeletonExporter::export_float_track(Control *c, float scale, FILE *f)
{
  int numkeys;
  int keytime;
  ITCBFloatKey kTCB;
  IBezFloatKey kBEZ, kBEZ2;
  ILinFloatKey kLIN;
  unsigned char type;
  char ORT_before, ORT_after;
  float fval, l , p, T, f0=0, dt;
  
//  if (!c) return;
//  if (!IsKnownController(c)) return;

  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));

  // ci devono essere almeno 1 key
  numkeys = c->NumKeys();
  if (numkeys==NOT_KEYFRAMEABLE || numkeys==0) return;

  // Get the keyframe interface
  IKeyControl *ikeys = GetKeyControlInterface(c);

  //  --------------------------------------------------
  // |  Differenziamo secondo il tipo di interpolazione |
  //  --------------------------------------------------


  // Interpolazione TCB  
  if (IsTCBControl(c) && ikeys)
  {
	type=TCB_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
    fprintf(fTXT, " Type is TCB\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kTCB);
      fval=kTCB.val*scale;
	  
	  //fprintf(fTXT, "key settings : t=%f, c=%f, b=%f, et=%f, ef=%f\n",
		//      kTCB.tens, kTCB.cont, kTCB.bias, kTCB.easeIn, kTCB.easeOut);

	  keytime = kTCB.time/GetTicksPerFrame();
      fprintf(fTXT, "timepos : %d,  value %f\n",
		      keytime, fval);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);

	  // key values
	  fwrite(&fval, sizeof(float), 1, f);

	  // key settings
	  fwrite(&kTCB.tens, sizeof(float), 1, f);
	  fwrite(&kTCB.cont, sizeof(float), 1, f);
	  fwrite(&kTCB.bias, sizeof(float), 1, f);
	  fwrite(&kTCB.easeIn, sizeof(float), 1, f);
	  fwrite(&kTCB.easeOut, sizeof(float), 1, f);
	}
  }
  else

  // Interpolazione BEZIER
  if (IsBezierControl(c) && ikeys)
  {
    type=BEZIER_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
	fprintf(fTXT, " Type is Bezier\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kBEZ);
      fval=kBEZ.val*scale;

	  keytime = kBEZ.time/GetTicksPerFrame();
      fprintf(fTXT, "timepos : %d,  value %f\n", keytime, fval);
	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);
	  // key values
	  fwrite(&fval, sizeof(float), 1, f);

	  // incoming tangent
	  if (i==0)
	  {
         fwrite(&f0, sizeof(float), 1, f);
		 fprintf(fTXT, "inTan: %f\n", 0);
	  }
	  else
	  {
	     p=scale*kBEZ.intan; l=kBEZ.inLength;
		 ikeys->GetKey(i-1, &kBEZ2);
         dt=(kBEZ2.time-kBEZ.time);
	     T=p*dt;
	     fprintf(fTXT, "inTan: %f\n", T);
	     fwrite(&T, sizeof(float), 1, f);
	  }
	  // outcoming tangent
	  if (i==numkeys-1)
	  {
         fwrite(&f0, sizeof(float), 1, f);
		 fprintf(fTXT, "outTan: %f\n", 0);
	  }
	  else
	  {
	     p=scale*kBEZ.outtan; l=kBEZ.outLength;
		 ikeys->GetKey(i+1, &kBEZ2);
         dt=(kBEZ2.time-kBEZ.time);
	     T=p*dt;
	     fprintf(fTXT, "outTan: %f\n", T);
	     fwrite(&T, sizeof(float), 1, f);
	  }
	}
  }
  else
  // Interpolazione LINEARE
  if (IsLinearControl(c) && ikeys)
  {
	type=LINEAR_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
    fprintf(fTXT, " Type is Linear\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kLIN);
      fval=kLIN.val*scale;

	  keytime = kLIN.time/GetTicksPerFrame();
	  fprintf(fTXT, "timepos : %d,  value %f\n",
              keytime, fval);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);

	  // key values
	  fwrite(&fval, sizeof(float), 1, f);
	}
  }
  else fprintf(fTXT, "position controller type not supported!");
}


// ###########################################################
// ################   TRACCE DI Rotazione   ##################
// ###########################################################

void SkeletonExporter::export_1key_rot_track(Control *c, FILE *f)
{
  int keytime=0;
  int numkeys=1;
  Quat qval;
  unsigned char type;
  char ORT_before, ORT_after;
  Interval valid;
  float s=0, angle;
  Point3 axis;
  
  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));

  // Interpolazione TCB
  type=TCB_CONTROLLER;
  fwrite(&type, sizeof(unsigned char), 1, f);
  fwrite(&ORT_before, sizeof(char), 1, f);
  fwrite(&ORT_after, sizeof(char), 1, f);
  fwrite(&numkeys, sizeof(int), 1, f);
  fprintf(fTXT, " Type is TCB\n");
  fprintf(fTXT, "Number of keys : %d\n", numkeys);

  c->GetValue(0, &qval, valid);
  AngAxisFromQ(qval, &angle, axis);

  //fprintf(fTXT, "key settings : t=%f, c=%f, b=%f, et=%f, ef=%f\n",
	//	      s, s, s, s, s);
  fprintf(fTXT, "timepos : %d,  value %f, %f, %f,  %f\n",
	      keytime, axis.x, axis.y, axis.z, angle);

  // key values
  fwrite(&keytime, sizeof(int), 1, f);  // timepos=0
  write_point3(&axis, f);
  fwrite(&angle, sizeof(float), 1, f);

  // key settings
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
}


void SkeletonExporter::export_rot_track(Control *c, FILE *f)
{
  int numkeys;
  int keytime;
  ITCBRotKey kTCB;
  IBezQuatKey kBEZ;
  ILinRotKey kLIN;
  unsigned char type;
  char ORT_before, ORT_after;
  float angle;
  Point3 axis;
  
//  if (!c) return;
//  if (!IsKnownController(c)) return;

  // ci devono essere almeno 1 key
  numkeys = c->NumKeys();
  if (numkeys==NOT_KEYFRAMEABLE || numkeys==0) return;

  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));

  // Get the keyframe interface
  IKeyControl *ikeys = GetKeyControlInterface(c);

  //  --------------------------------------------------
  // |  Differenziamo secondo il tipo di interpolazione |
  //  --------------------------------------------------


  // Interpolazione TCB
  if (IsTCBControl(c) && ikeys)
  {
	type=TCB_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
    fprintf(fTXT, " Type is TCB\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
	
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kTCB);

	  //fprintf(fTXT, "key settings : t=%f, c=%f, b=%f, et=%f, ef=%f\n",
		//      kTCB.tens, kTCB.cont, kTCB.bias, kTCB.easeIn, kTCB.easeOut);

	  keytime = kTCB.time/GetTicksPerFrame();
      fprintf(fTXT, "timepos : %d,  value %f, %f, %f, %f\n",
		      keytime, kTCB.val.axis.x, kTCB.val.axis.y, kTCB.val.axis.z, kTCB.val.angle);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);

	  // key values
      write_point3(&kTCB.val.axis, f);
	  fwrite(&kTCB.val.angle, sizeof(float), 1, f);

	  // key settings
	  fwrite(&kTCB.tens, sizeof(float), 1, f);
	  fwrite(&kTCB.cont, sizeof(float), 1, f);
	  fwrite(&kTCB.bias, sizeof(float), 1, f);
	  fwrite(&kTCB.easeIn, sizeof(float), 1, f);
	  fwrite(&kTCB.easeOut, sizeof(float), 1, f);
	}
  }
  else
  // Interpolazione BEZIER
  if (IsBezierControl(c) && ikeys)
  {
    type=BEZIER_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
	fprintf(fTXT, " Type is Bezier\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kBEZ);
	  keytime = kBEZ.time/GetTicksPerFrame();

	  AngAxisFromQ(kBEZ.val, &angle, axis);
      fprintf(fTXT, "timepos : %d,  value %f, %f, %f, %f\n",
		      keytime, axis.x, axis.y, axis.z, angle);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);

	  // key values
      write_point3(&axis, f);
	  fwrite(&angle, sizeof(float), 1, f);
	}
  }
  else
  // Interpolazione LINEARE
  if (IsLinearControl(c) && ikeys)
  {
	type=LINEAR_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
    fprintf(fTXT, " Type is Linear\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kLIN);
	  keytime = kLIN.time/GetTicksPerFrame();
      AngAxisFromQ(kLIN.val, &angle, axis);

	  fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
              keytime, axis.x, axis.y, axis.z, angle);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);

	  // key values
      write_point3(&axis, f);
	  fwrite(&angle, sizeof(float), 1, f);
	}
  }
  else fprintf(fTXT, "rotation controller type not supported!");
}



// ###########################################################
// ##################   TRACCE DI SCALING  ###################
// ###########################################################

void SkeletonExporter::export_1key_scale_track(Control *c, FILE *f)
{
  int keytime=0;
  int numkeys=1;
  ScaleValue sval;
  unsigned char type;
  char ORT_before, ORT_after;
  Interval valid;
  float s=0;
  
  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));

  // Interpolazione TCB
  type=TCB_CONTROLLER;
  fwrite(&type, sizeof(unsigned char), 1, f);
  fwrite(&ORT_before, sizeof(char), 1, f);
  fwrite(&ORT_after, sizeof(char), 1, f);
  fwrite(&numkeys, sizeof(int), 1, f);
  fprintf(fTXT, " Type is TCB\n");
  fprintf(fTXT, "Number of keys : %d\n", numkeys);

  c->GetValue(0, &sval, valid);
  //fprintf(fTXT, "key settings : t=%f, c=%f, b=%f, et=%f, ef=%f\n",
	//	      s, s, s, s, s);
  fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
	      keytime, sval.s.x, sval.s.y, sval.s.z);

  // key values
  fwrite(&keytime, sizeof(int), 1, f);  // timepos=0
  write_point3(&sval.s, f);

  // key settings
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
  fwrite(&s, sizeof(float), 1, f);
}


void SkeletonExporter::export_scale_track(Control *c, FILE *f)
{
  int numkeys;
  int keytime;
  ITCBScaleKey  kTCB;
  IBezScaleKey  kBEZ, kBEZ2;
  ILinScaleKey  kLIN;
  unsigned char type;
  char ORT_before, ORT_after;
  float dt, f0=0;
  Point3 l, p, T;
  
  if (!c) return;
//  if (!IsKnownController(c)) return;

  // ci devono essere almeno 1 key
  numkeys = c->NumKeys();
  if (numkeys==NOT_KEYFRAMEABLE || numkeys==0) return;

  ORT_before=(char)MaxORT2MyORT(c->GetORT(ORT_BEFORE));
  ORT_after=(char)MaxORT2MyORT(c->GetORT(ORT_AFTER));

  // Get the keyframe interface
  IKeyControl *ikeys = GetKeyControlInterface(c);

  //  --------------------------------------------------
  // |  Differenziamo secondo il tipo di interpolazione |
  //  --------------------------------------------------


  // Interpolazione TCB
  if (IsTCBControl(c) && ikeys)
  {
	type=TCB_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
    fprintf(fTXT, " Type is TCB\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
	
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kTCB);

	  //fprintf(fTXT, "key settings : t=%f, c=%f, b=%f, et=%f, ef=%f\n",
		//      kTCB.tens, kTCB.cont, kTCB.bias, kTCB.easeIn, kTCB.easeOut);

	  keytime = kTCB.time/GetTicksPerFrame();
      fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
		      keytime, kTCB.val.s.x, kTCB.val.s.y, kTCB.val.s.z);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);

	  // key values
      write_point3(&kTCB.val.s, f);

	  // key settings
	  fwrite(&kTCB.tens, sizeof(float), 1, f);
	  fwrite(&kTCB.cont, sizeof(float), 1, f);
	  fwrite(&kTCB.bias, sizeof(float), 1, f);
	  fwrite(&kTCB.easeIn, sizeof(float), 1, f);
	  fwrite(&kTCB.easeOut, sizeof(float), 1, f);
	}
  }
  else

  // Interpolazione BEZIER
  if (IsBezierControl(c) && ikeys)
  {
    type=BEZIER_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
	fprintf(fTXT, " Type is Bezier\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kBEZ);
	  keytime = kBEZ.time/GetTicksPerFrame();
      fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
		      keytime, kBEZ.val.s.x, kBEZ.val.s.y, kBEZ.val.s.z);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);
	  // key values
      write_point3(&kBEZ.val.s, f);

	  // incoming tangent
	  if (i==0)
	  {
         fwrite(&f0, sizeof(float), 3, f);
		 fprintf(fTXT, "inTan: %f, %f, %f\n", 0, 0, 0);
	  }
	  else
	  {
	     p=kBEZ.intan; l=kBEZ.inLength;
		 ikeys->GetKey(i-1, &kBEZ2);
         dt=(kBEZ2.time-kBEZ.time);
	     T.x=p.x*dt;
	     T.y=p.y*dt;
	     T.z=p.z*dt;
	     fprintf(fTXT, "inTan: %f, %f, %f, %f\n", T.x, T.y, T.z, dt);
         write_point3(&T, f);
	  }
	  // outcoming tangent
	  if (i==numkeys-1)
	  {
         fwrite(&f0, sizeof(float), 3, f);
		 fprintf(fTXT, "outTan: %f, %f, %f\n", 0, 0, 0);
	  }
	  else
	  {
	     p=kBEZ.outtan; l=kBEZ.outLength;
		 ikeys->GetKey(i+1, &kBEZ2);
         dt=(kBEZ2.time-kBEZ.time);
	     T.x=p.x*dt;
	     T.y=p.y*dt;
	     T.z=p.z*dt;
	     fprintf(fTXT, "outTan: %f, %f, %f %f\n", T.x, T.y, T.z, dt);
         write_point3(&T, f);
	  }
	}
  }
  else
  // Interpolazione LINEARE
  if (IsLinearControl(c) && ikeys)
  {
	type=LINEAR_CONTROLLER;
	fwrite(&type, sizeof(unsigned char), 1, f);
    fwrite(&ORT_before, sizeof(char), 1, f);
    fwrite(&ORT_after, sizeof(char), 1, f);
	fwrite(&numkeys, sizeof(int), 1, f);
    fprintf(fTXT, " Type is Linear\n");
    fprintf(fTXT, "Number of keys : %d\n", numkeys);
    for (int i=0; i<numkeys; i++) 
	{
	  ikeys->GetKey(i, &kLIN);
	  keytime = kLIN.time/GetTicksPerFrame();
	  fprintf(fTXT, "timepos : %d,  value %f, %f, %f\n",
              keytime, kLIN.val.s.x, kLIN.val.s.y, kLIN.val.s.z);

	  // timeline position
	  fwrite(&keytime, sizeof(int), 1, f);

	  // key values
      write_point3(&kLIN.val.s, f);
	}
  }
  else fprintf(fTXT, "scaling controller type not supported!");
}