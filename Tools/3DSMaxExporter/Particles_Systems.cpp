#include "SceneExport.h"
#include "A3Dfile.h"
#include "extrasdk.h"


void SkeletonExporter::export_particle_spray(INode *node)
{
  char sval[50];
  Interval range = ip->GetAnimRange();
  ObjectState os = node->EvalWorldState(0);
  if (!os.obj) return;

  SimpleParticle *partsys;
  Object *p; IDerivedObject *q; Modifier *m;
  partsys=(SimpleParticle *)os.obj;
  Point3 row;
  Matrix3 mat;
  Mtl *materiale;
  INode *padre=node->GetParentNode();
  int mod_names=0, k, mod_count=0;
  char modNames[25][50], refname[50];
  int sf, sm, mf, n;

  if ((padre) && (strcmp(padre->GetName(), "Scene Root")!=0))
    sf=strlen(padre->GetName())+1;
  else sf=0;
  
  materiale=node->GetMtl();
  if (materiale) sm=strlen(materiale->GetName())+1;
  else sm=0;

  mf=0;

  // trovo i modificatori Wind e Gravity con il loro
  // nome
  p=node->GetObjOrWSMRef();
  if ((p->SuperClassID()==GEN_DERIVOB_CLASS_ID) &&
	 (p->ClassID()==Class_ID(WSM_DERIVOB_CLASS_ID, 0)))
  {
	q=(IDerivedObject *)p;
	n=q->NumModifiers();
	mod_names=mod_count=0;
	for (k=0; k<n; k++)
	{
      m=q->GetModifier(k);
      Class_ID cidd = m->ClassID();
	  if ((cidd==Class_ID(WINDMOD_CLASS_ID, 0)) ||
		  (cidd==Class_ID(GRAVITYMOD_CLASS_ID, 0)) ||
		  (cidd==Class_ID(BOMB_OBJECT_CLASS_ID, 0)))
	  {
		 SimpleWSMMod *wm=(SimpleWSMMod *)m;
		 strcpy(refname, wm->nodeRef->GetName());
		 strcpy(modNames[mod_count], refname);
		 mod_names+=(strlen(refname)+1);
         mod_count++;
	  }
	}
  }

  write_chunk_header(fA3D, SPRAY_PARTICLE_SYSTEM_ID, node->GetName(),
	                 4+sf+4+sm+  // padre, materiale
					 12+48+  //pivot, matrice world(t=0)
					 4+mf+   // nome mesh/oggetto
					 4+4+4+4+ // emitter: width, height, speed , variation
					 4+4+4+4+ // life, startTime, stopTime, max_particles
					 4+mod_names // num WSM, nomi WSM
					 );

  fprintf(fTXT, "Spray particle system found\n");
  fprintf(fTXT, "Name : %s\n", node->GetName());
  if (padre) fprintf(fTXT, "Parent name : %s\n", node->GetParentNode()->GetName());
  if (materiale) fprintf(fTXT, "Material name : %s\n", materiale->GetName());
  if (makeADP)
    fprintf(fADP, "  particle %c%s%c\n  {\n", '"', node->GetName(), '"');

  // -----------    scrivo il padre (flag, nome)   ------------------
  fwrite(&sf, sizeof(int), 1, fA3D);
  if (sf>0)
  {
	 write_string0(fA3D, padre->GetName());
	 if (makeADP) fprintf(fADP, "     father=%c%s%c;\n", '"', padre->GetName(), '"');
  }
  else
	 if (makeADP) fprintf(fADP, "     father=%cNONE%c;\n", '"', '"');


  // ---------   scrivo il materiale di base (flag, nome)   ----------
  fwrite(&sm, sizeof(int), 1, fA3D);
  if (sm>0)
  {
     write_string0(fA3D, materiale->GetName());
	 //if (makeADP) fprintf(fADP, "     material=%c%s%c;\n", '"', materiale->GetName(), '"');
  }
  else
	 //if (makeADP) fprintf(fADP, "     material=%cNONE%c;\n", '"', '"');


  // ---------------   scrittura del punto di pivot   ----------------
  GetPivotOffset(node, &row);
  fprintf(fTXT, "Pivot point : %f, %f, %f\n", row.x, row.y, row.z);
  write_point3(&row, fA3D);

  // -------------------   scrittura matrice   -----------------------
  mat = node->GetNodeTM(0);
  write_matrix(&mat, fA3D);

  // ---------   scrittura dell'eventuale nome della mesh   ----------
  fwrite(&mf, sizeof(int), 1, fA3D);
  if (mf>0)
  {
    write_string0(fA3D, "TO_DO");
    if (makeADP) fprintf(fADP, "    lod=%cTO_DO, 0, 99999.99%c;\n", '"', '"');
  }


  // *************  ESPORAZIONE DATI DEL PARTCLE SYSTEM  *************
  float initVel, var, width, height;
  int count, life, start_time, stop_time;

  partsys->pblock->GetValue(PB_EMITTERWIDTH, 0, width, FOREVER);
  partsys->pblock->GetValue(PB_EMITTERHEIGHT, 0, height, FOREVER);
  partsys->pblock->GetValue(PB_SPEED, 0, initVel, FOREVER);
  partsys->pblock->GetValue(PB_VARIATION, 0, var, FOREVER);
  partsys->pblock->GetValue(PB_LIFETIME, 0, life, FOREVER);
  life=life/ GetTicksPerFrame();
  partsys->pblock->GetValue(PB_STARTTIME, 0, start_time, FOREVER);
  start_time=start_time / GetTicksPerFrame();
  stop_time=range.End() / GetTicksPerFrame();
  partsys->pblock->GetValue(PB_RNDPARTICLES, 0, count, FOREVER);

  fprintf(fTXT, "Emitter width: %f\n", width);
  fprintf(fTXT, "Emitter height: %f\n", height);
  fprintf(fTXT, "Emitter speed (init vel): %f\n", initVel);
  fprintf(fTXT, "Emitter variation: %f\n", var);
  fprintf(fTXT, "Life: %d\n", life);
  fprintf(fTXT, "Start time: %d\n", start_time);
  fprintf(fTXT, "Stop time: %d\n", stop_time);
  fprintf(fTXT, "Max particles: %d\n", count);

  // informazioni in formato binario (.A3D)
  fwrite(&width, sizeof(float), 1, fA3D);
  fwrite(&height, sizeof(float), 1, fA3D);
  fwrite(&initVel, sizeof(float), 1, fA3D);
  fwrite(&var, sizeof(float), 1, fA3D);
  fwrite(&life, sizeof(int), 1, fA3D);
  fwrite(&start_time, sizeof(int), 1, fA3D);
  fwrite(&stop_time, sizeof(int), 1, fA3D);
  fwrite(&count, sizeof(int), 1, fA3D);

  fwrite(&mod_count, sizeof(int), 1, fA3D);
  fprintf(fTXT, "Modificatori (wind, gravity, bomb) collegati: %d\n", mod_count);
  for (k=0; k<mod_count; k++)
  {
     fprintf(fTXT, "Modificatore %d: %s\n", k, modNames[k]);
	 write_string0(fA3D, modNames[k]);
	 //if (makeADP)
       //fprintf(fADP, "     modifier=%c%s, ON%c;\n", '"', modNames[k], '"');
  }


  //------------------------
  // ESPORTAZIONE KEYFRAMER
  //------------------------
  Control *c;
  int size_key;

  // NB: per gli oggetti mesh e quant'altre tipologie di
  // oggetti che possono essere linkati (ovvero dove e'
  // possibile implmenetare gerarchie), esporto SEMPRE una key 
  // di posizione, una di rotazione ed una di scaling
  // POSITION CONTROLLER
  c=node->GetTMController()->GetPositionController();
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=36;
	 else
	 if (IsBezierControl(c)) size_key=40;
	 else size_key=16;
	 fprintf(fTXT, "Particle position track present.\n");
     write_chunk_header(fA3D, POSITION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_point3_track(c, 1, fA3D);
  }
  else
  {
	fprintf(fTXT, "Particle position track present. (1 key case)\n");
	if (!c) 	fprintf(fTXT, "c nullo !\n");
    fflush(fTXT);
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
	 fprintf(fTXT, "Particle rotation track present.\n");
     write_chunk_header(fA3D, ROTATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_rot_track(c, fA3D);
  }
  else
  {
	fprintf(fTXT, "Particle rotation track present. (1 key case)\n");
    fflush(fTXT);
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
	 fprintf(fTXT, "Particle scaling track present.\n");
     write_chunk_header(fA3D, SCALE_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_scale_track(c, fA3D);
  }
  else
  {
	fprintf(fTXT, "Particle scaling track present. (1 key case)\n");
	size_key=36;
    write_chunk_header(fA3D, SCALE_TRACK_ID,
	                   node->GetName(), 1+2+4+1*size_key);
    export_1key_scale_track(c, fA3D);
  }

  // keyframer emitter width
  c=partsys->pblock->GetController(PB_EMITTERWIDTH);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Particle emitter width track present.\n");
     write_chunk_header(fA3D, PARTICLE_EMITTER_WIDTH_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  // keyframer emitter length
  c=partsys->pblock->GetController(PB_EMITTERHEIGHT);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Particle emitter length track present.\n");
     write_chunk_header(fA3D, PARTICLE_EMITTER_LENGTH_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  // keyframer particles speed
  c=partsys->pblock->GetController(PB_SPEED);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Particle emitter speed track present.\n");
     write_chunk_header(fA3D, PARTICLE_EMITTER_SPEED_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }

  // keyframer particles variations
  c=partsys->pblock->GetController(PB_VARIATION);
  if ((c) && (c->NumKeys()>0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "Particle emitter variation track present.\n");
     write_chunk_header(fA3D, PARTICLE_EMITTER_VARIATION_TRACK_ID,
	                    node->GetName(), 1+2+4+c->NumKeys()*size_key);
     export_float_track(c, 1, fA3D);
  }


  if (makeADP)
  {
    fprintf(fADP, "     texture=%cNONE%c;\n", '"', '"');
	my_ftoa(width, sval);
	fprintf(fADP, "     emitter_width=%c%s%c;\n", '"', sval, '"');
	my_ftoa(height, sval);
    fprintf(fADP, "     emitter_height=%c%s%c;\n", '"', sval, '"');
    fprintf(fADP, "     faded_particles=%cOFF%c;\n", '"', '"');
    fprintf(fADP, "     max_particles=%c%d%c;\n", '"', count, '"');
    fprintf(fADP, "     start_time=%c%d%c;\n", '"', start_time, '"');
    fprintf(fADP, "     end_time=%c%d%c;\n", '"', stop_time, '"');
    fprintf(fADP, "     life=%c%d%c;\n", '"', life, '"');
	my_ftoa(initVel, sval);
    fprintf(fADP, "     speed=%c%s%c;\n", '"', sval, '"');
	my_ftoa(var, sval);
    fprintf(fADP, "     variation=%c%s%c;\n", '"', sval, '"');
    fprintf(fADP, "     size_attenuation=%c0.4, 1.0, 0.8, 0.1%c;\n", '"', '"');
	fprintf(fADP, "  }\n\n");
  }
  fprintf(fTXT, "\n\n");
}