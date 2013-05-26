#include "SceneExport.h"
#include "A3Dfile.h"

int	SkeletonExporter::GetTexID(char *name)
{
  for (int k=0; k<ntex; k++)
	if (strcmp(textures[k].name, name)==0) return(textures[k].ID);
  return(-1);
}

//--------------------------------------------------------------------

int	SkeletonExporter::GetTexID(Texmap *texmap)
{
  char *name;

  if (texmap->ClassID() != Class_ID(BMTEX_CLASS_ID, 0)) return(-1);
  BitmapTex *bmptex=(BitmapTex *)texmap;
  name=bmptex->GetMapName();

  for (int k=0; k<ntex; k++)
	if (strcmp(textures[k].name, name)==0) return(textures[k].ID);
  return(-1);
}

//--------------------------------------------------------------------

void SkeletonExporter::add_bitmap(BitmapTex *bmp)
{
  int flag, k;
  char *name;
  
  name=bmp->GetMapName();
  flag=0;
  for (k=0; k<ntex; k++)
    if (strcmp(name, textures[k].name)==0) flag=1;
  // texture non ancora inserita
  if (flag==0)
  {
	textures[ntex].ID=ntex;
	strcpy(textures[ntex].name, name);
	textures[ntex].bmptex=bmp;
	ntex++;
  }
}

//--------------------------------------------------------------------

void SkeletonExporter::collect_bitmaps(Mtl *m)
{
  int j, i, k;

  if (m->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
  {
     for (j=0; j<m->NumSubMtls(); j++)
	 {
	    Mtl *subMtl = m->GetSubMtl(j);
	    if (subMtl) collect_bitmaps(subMtl);
	 }
  }
  else if (m->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
  {
    StdMat *stdm=(StdMat *)m;
	for (i=0; i<m->NumSubTexmaps(); i++)
	{
      Texmap *subTex = m->GetSubTexmap(i);
	  // aggiungo la texture solo se essa è enabled
      if ((subTex) && (stdm->MapEnabled(i)))
	  {
		 // se è una bitmap la aggiungo...
         if (subTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
			 add_bitmap((BitmapTex *)subTex);
		 else
         // altrimenti se è una texture composita esporto le
		 // sottotexture (se sono enabled e delle bitmap)
         if (subTex->ClassID() == Class_ID(COMPOSITE_CLASS_ID, 0))
	        for (k=0; k<subTex->NumSubTexmaps(); k++)
			{
               Texmap *sub2 = subTex->GetSubTexmap(k);
			   if ((sub2) && (subTex->SubTexmapOn(k)))
                 if (sub2->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
			        add_bitmap((BitmapTex *)sub2);
			}
	  }
	}
  }
}

//--------------------------------------------------------------------

void SkeletonExporter::dump_bitmap(Tex *tex)
{
   char ext[4], msg[100];
   int i, j, k,tstart;
   float rate;

   if (!makeADM) return;

   k=strlen(tex->name);
   j=-1;
   for (i=0; i<k; i++)
	 if (tex->name[i]=='.') j=i;

   if (j>0)
   {
	  ext[0]=toupper(tex->name[j+1]);
	  ext[1]=toupper(tex->name[j+2]);
	  ext[2]=toupper(tex->name[j+3]);
	  ext[3]='\0';
   }
   else strcpy(ext, "");

   fprintf(fADM, "  bitmap %c%d%c \n  {\n", '"', tex->ID, '"');
   fprintf(fADM, "    id=%c%d%c;\n", '"', tex->ID, '"');
   fprintf(fADM, "    filename=%c%s%c;\n", '"', tex->name, '"');
   fprintf(fADM, "    mipmaplevels=%c1%c;\n", '"', '"');
   if (strcmp(ext, "AVI")==0)
   {
      tstart=tex->bmptex->GetStartTime()/GetTicksPerFrame();
      rate=tex->bmptex->GetPlaybackRate();
      fprintf(fADM, "    start_time=%c%d%c;\n", '"', tstart, '"');
	  my_ftoa(rate, msg);
      fprintf(fADM, "    playback_rate=%c%s%c;\n", '"', msg, '"');
	  switch (tex->bmptex->GetEndCondition())
	  {
         case END_LOOP: fprintf(fADM, "    end_condition=%cLOOP%c;\n", '"', '"');
                        break;
         case END_PINGPONG: fprintf(fADM, "    end_condition=%cPINGPONG%c;\n", '"', '"');
						    break;
         case END_HOLD: fprintf(fADM, "    end_condition=%cHOLD%c;\n", '"', '"');
                        break;
	  }
   }
   fprintf(fADM, "  }\n\n");
   fflush(fADM);
}

//--------------------------------------------------------------------

void SkeletonExporter::dump_map(Texmap *texmap, int MAPx, int ID)
{
   BitmapTex *bmptex;
   StdUVGen *uvgen;
   char MAPch[3];

   if (!texmap) return;
   if (!makeADM) return;
   if (texmap->ClassID() != Class_ID(BMTEX_CLASS_ID, 0)) return;
   bmptex=(BitmapTex *)texmap;

   // nel caso ci fosse una mappa di trasparenza
   if (ID == ID_OP)
     fprintf(fADM, "    trasparency_type=%c%d%c;\n", '"',
	         GetTexID(bmptex->GetMapName()), '"');
   // mappa di bump
   if (ID == ID_BU)
   {
     fprintf(fADM, "    bumpmap=%cDOT3, %d%c;\n", '"',
	         GetTexID(bmptex->GetMapName()), '"');
     fprintf(fADM, "    bumpscale=%c1.0, 1.0%c;\n", '"', '"');
   }
   
   itoa(MAPx, MAPch, 10);
   if (ID == ID_RL)
	 fprintf(fADM, "    map%schannel=%cENV%c;\n", MAPch, '"', '"');
   else
   if (ID == ID_DI)
   {
      int UVchan=bmptex->GetMapChannel();
	  fprintf(fADM, "    map%schannel=%cTEX%d%c;\n", MAPch, '"', UVchan, '"');
   }

   uvgen=bmptex->GetUVGen();
   if ((uvgen) && (ID!=ID_RL))
   {
     char sval1[50], sval2[50], sval3[50];
     if ((uvgen->GetUOffs(0)!=0) || (uvgen->GetVOffs(0)!=0))
	 {
       my_ftoa(uvgen->GetUOffs(0), sval1);
       my_ftoa(uvgen->GetVOffs(0), sval2);
       fprintf(fADM, "    map%soffset=%c%s, %s%c;\n", MAPch, '"', sval1, sval2, '"');
	 }
     if ((uvgen->GetUAng(0)!=0) || (uvgen->GetVAng(0)!=0) ||
         (uvgen->GetWAng(0)!=0))
	 {
       my_ftoa(uvgen->GetUAng(0), sval1);
       my_ftoa(uvgen->GetVAng(0), sval2);
       my_ftoa(uvgen->GetWAng(0), sval3);
       fprintf(fADM, "    map%sangle=%c%s, %s, %s%c;\n", MAPch, '"', sval1, sval2, sval3, '"');
	 }
     if ((uvgen->GetUScl(0)!=1) || (uvgen->GetVScl(0)!=1))
	 {
       my_ftoa(uvgen->GetUScl(0), sval1);
       my_ftoa(uvgen->GetVScl(0), sval2);
       fprintf(fADM, "    map%stile=%c%s, %s%c;\n", MAPch, '"', sval1, sval2, '"');
	 }
   }
}

//--------------------------------------------------------------------

Control *SkeletonExporter::get_control_from_animatable_name(Animatable *anim, char *name)
{
   int w;
   Control *c1, *c2;

   if (!anim) return(NULL);
   for (w=0; w<anim->NumSubs(); w++)
   {
	  if (strcmp(anim->SubAnimName(w), name)==0)
	  {
         c1=GetControlInterface(anim->SubAnim(w));
		 return(c1);
	  }
	  c2=get_control_from_animatable_name(anim->SubAnim(w), name);
	  if (c2) return(c2);
   }
   return(NULL);
}

//--------------------------------------------------------------------

void SkeletonExporter::dump_map_tracks(Texmap *map, char indexMap, char *material_name)
{
  Control *c;
  int size_key;

  if (!map) return;
  if (map->ClassID() != Class_ID(BMTEX_CLASS_ID, 0)) return;

  // export_animatable(tex, 0);
  c=get_control_from_animatable_name(map, "U Offset");
  if ((c) && (c->NumKeys() > 0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "U Offset track present.\n");
     write_chunk_header(fA3D, U_OFFSET_TRACK_ID,
	                    material_name, 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&indexMap, sizeof(char), 1, fA3D);
  }

  c=get_control_from_animatable_name(map, "V Offset");
  if ((c) && (c->NumKeys() > 0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "V Offset track present.\n");
     write_chunk_header(fA3D, V_OFFSET_TRACK_ID,
	                    material_name, 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&indexMap, sizeof(char), 1, fA3D);
  }

  c=get_control_from_animatable_name(map, "U Tiling");
  if ((c) && (c->NumKeys() > 0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "U Tiling track present.\n");
     write_chunk_header(fA3D, U_TILE_TRACK_ID,
	                    material_name, 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&indexMap, sizeof(char), 1, fA3D);
  }

  c=get_control_from_animatable_name(map, "V Tiling");
  if ((c) && (c->NumKeys() > 0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "V Tiling track present.\n");
     write_chunk_header(fA3D, V_TILE_TRACK_ID,
	                    material_name, 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);
	 fwrite(&indexMap, sizeof(char), 1, fA3D);
  }

  c=get_control_from_animatable_name(map, "U Angle");
  if ((c) && (c->NumKeys() > 0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "U Angle track present.\n");
     write_chunk_header(fA3D, U_ANGLE_TRACK_ID,
	                    material_name, 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);  // radianti
	 fwrite(&indexMap, sizeof(char), 1, fA3D);
  }

  c=get_control_from_animatable_name(map, "V Angle");
  if ((c) && (c->NumKeys() > 0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "V Angle track present.\n");
     write_chunk_header(fA3D, V_ANGLE_TRACK_ID,
	                    material_name, 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);  // radianti
	 fwrite(&indexMap, sizeof(char), 1, fA3D);
  }

  c=get_control_from_animatable_name(map, "W Angle");
  if ((c) && (c->NumKeys() > 0))
  {
	 if (IsTCBControl(c)) size_key=28;
	 else
	 if (IsBezierControl(c)) size_key=16;
	 else size_key=8;
	 fprintf(fTXT, "W Angle track present.\n");
     write_chunk_header(fA3D, W_ANGLE_TRACK_ID,
	                    material_name, 1+2+4+c->NumKeys()*size_key+1);
     export_float_track(c, 1, fA3D);  // radianti
	 fwrite(&indexMap, sizeof(char), 1, fA3D);
  }
}

//--------------------------------------------------------------------