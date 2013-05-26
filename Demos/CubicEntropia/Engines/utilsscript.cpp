// Utility per gli script dei materiali (.ADM), luci (.ADL), oggetti (.ADO),
// dummy (.ADD)
// Made by [AD]Turbo

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include "utilsscript.h"
#include "..\utility.h"

extern HWND miawin;         // finestra principale; usata qui per le debug_error


// -------------------------------------------------
// procedure di utilita' generale a tutti gli script
// -------------------------------------------------

char *get_file_ext(char *nomefile)
// si ottiene l'estensione da un nomefile (es: "pippo.pcx" -> "PCX")
{
  int i,j;
  char *ext=new char[4];

  j=-1;
  for (i=0; i<(int)strlen(nomefile); i++)
     if (nomefile[i]=='.') { j=i; break; }

  if (j<0) {
             strcpy(ext, "");
             return ext;
           }
  ext[0]=toupper(nomefile[j+1]);
  ext[1]=toupper(nomefile[j+2]);
  ext[2]=toupper(nomefile[j+3]);
  ext[3]=0;
  strcat(ext,"");

  return ext;
}



int find_first_section(char* memfile, int pos)
{
  int i=pos;

  while(memfile[i]==' ') i++;
  if (memfile[i]==0) return -1;
  return i;
}


int find_next_section(char* memfile, int pos)
{
  int i=pos;

  while((memfile[i]!='}')&&(memfile[i]!=0)) i++;
  if (memfile[i]==0) return -1;
  i++;
  while((memfile[i]==' ')&&(memfile[i]!=0)) i++;
  if (memfile[i]==0) return -1;
  return i;
}


char *get_section_type(char* memfile, int pos)
{
  int i=pos,n=0;
  char *secname=new char[30];

  while((memfile[i]!=' ')&&(memfile[i]!='"')) secname[n++]=memfile[i++];
  secname[n]=0;
  return secname;
}


char *get_section_value(char* memfile, int pos)
{
  int i=pos,n=0;
  char *secname=new char[30];

  while(memfile[i]!='"') i++;
  i++;
  while(memfile[i]!='"') secname[n++]=memfile[i++];
  secname[n]=0;
  return secname;
}


int get_subsection(char* memfile, int pos)
{
  int i=pos;
  
  while(memfile[i]!='{') i++;
  i++;
 
  return i;
}


int get_first_key(char* memfile, int pos)
{
  int i=pos;
  
  while(memfile[i]==' ') i++;
 
  if (memfile[i]=='}') return -1;
  return i;
}


int get_next_key(char* memfile, int pos)
{
  int i=pos;
  
  while(memfile[i]!=';') i++;
  i++; 
  while(memfile[i]==' ') i++;

  if (memfile[i]=='}') return -1;
  return i;
}


char *get_key_type(char* memfile, int pos)
{
  int i=pos,n=0;
  char *keyname=new char[30];

  while((memfile[i]!=' ')&&(memfile[i]!='=')) keyname[n++]=memfile[i++];
  keyname[n]=0;
  return keyname;
}


char *get_key_value(char* memfile, int pos)
{
  int i=pos,n=0;
  char *keyname=new char[30];

  while(memfile[i]!='"') i++;
  i++;
  while(memfile[i]!='"') keyname[n++]=memfile[i++];
  keyname[n]=0;
  return keyname;
}



char *alloc_script(char *filename)
// alloca in memoria il file togliendo commenti, spazi inutili e andate a capo
{
  FILE *bfile;
  char *memfile;
  char tmp;
  int i=0;

  if ((bfile=fopen(filename, "rb"))==NULL) debug_error(miawin, "allocADM : File ADM non trovato");
  memfile=new char[filelength(fileno(bfile))];
  strcpy(memfile,"");
 
  while (!feof(bfile))
  {   
    fread(&tmp,1,1,bfile);
    if (tmp=='"') { do { memfile[i++]=tmp; fread(&tmp,1,1,bfile);  } while (tmp!='"'); }
    if (tmp=='/') {
                   fread(&tmp,1,1,bfile);
                   if (tmp=='/') do fread(&tmp,1,1,bfile); while ((tmp!=10)&&(!feof(bfile)));
                      else if (tmp=='*') { do fread(&tmp,1,1,bfile); 
                                           while ((tmp!='/')&&(!feof(bfile)));
                                           fread(&tmp,1,1,bfile); }
                           else memfile[i++]='/'; 
                  }
    while ((tmp==' ')&&(memfile[i-1]==' ')) fread(&tmp,1,1,bfile);
    if ((tmp!=10)&&(tmp!=13)&&(tmp!=0)) memfile[i++]=tmp; 
  }
  memfile[i++]=0;
  memfile[i]=0;
  fclose(bfile);
  return memfile;
}





// ----------------------------------------------
// procedure proprie per gli script dei materiali
// ----------------------------------------------

int load_ADM (char *nfile, AD_Scene3D *s)
// carica un file .ADM
{
  int pos, ss, nmat, ntex, i, j, tval;
  char *stri;
  char *adm=alloc_script(nfile);
  float fval;

  // inizio contando il numero max di materiali e textures che allochero'
  nmat=0;
  ntex=0;
  pos=find_first_section(adm,0);
  while (pos!=-1)
  {
   if (strcmp(get_section_type(adm,pos),"material") == 0)
   {
    nmat++;
    ss=get_subsection(adm,pos);
    ss=get_first_key(adm,ss);
    while(ss!=-1)
    {
     if (strcmp(get_key_type(adm,ss),"texture") == 0) ntex++;
     if (strcmp(get_key_type(adm,ss),"envmap") == 0) ntex++;
     ss=get_next_key(adm,ss);
    }
   }
   pos=find_next_section(adm,pos);
  }

  
  // alloco il numero esatto di materiali e textures
  s->materials=new material[nmat];
  s->material_tracks=new material_parameters_tracks[nmat];
  for (i=0; i<nmat; i++)
     s->material_tracks[i].trasparency_track=(rolltrack *)NULL;

  s->textures=new texture[ntex];
  s->num_textures=0;
  s->num_materials=0;

  // riparto e carico sul serio
  pos=find_first_section(adm,0);
  while (pos!=-1)
  {
   if (strcmp(get_section_type(adm,pos),"material") == 0)
   {
    // entro in un materiale
    stri=get_section_value(adm,pos);
    s->materials[s->num_materials].name=stri;
    ss=get_subsection(adm,pos);
    ss=get_first_key(adm,ss);
    while(ss!=-1)
    {
      if (strcmp(get_key_type(adm,ss),"zsorting") == 0)
	  // leggo l'opzione di zsorting per l'intera scena
	  {
        stri=strupr(get_key_value(adm,ss));
	    if (strcmp(stri, "ON")==0)
		  s->materials[s->num_materials].zsorting=ON;
	    else s->materials[s->num_materials].zsorting=OFF;
	  }

      // pzioni di tiling delle UV
	  if (strcmp(get_key_type(adm, ss),"u_tile") == 0)
	  {
         stri=(get_key_value(adm, ss));
	     fval=my_atof(stri);
         s->materials[s->num_materials].tile_u=fval;
	  }

	  if (strcmp(get_key_type(adm, ss),"v_tile") == 0)
	  {
         stri=(get_key_value(adm, ss));
	     fval=my_atof(stri);
         s->materials[s->num_materials].tile_v=fval;
	  }

      if (strcmp(get_key_type(adm,ss),"doubleface") == 0)
	  // leggo l'opzione di double facing
	  {
        stri=strupr(get_key_value(adm,ss));
	    if (strcmp(stri, "ON")==0) 
		  s->materials[s->num_materials].double_facing=ON;
	    else s->materials[s->num_materials].double_facing=OFF;
	  }

      if (strcmp(get_key_type(adm,ss),"trasparency_type") == 0)
	  // leggo il fattore di trasparenza
	  {
        stri=strupr(get_key_value(adm,ss));
		if (strcmp(stri, "NONE")==0)
		{
           s->materials[s->num_materials].trasparencytype=OFF;
		   goto dopoNOT;
		}
		if (strcmp(stri, "BLEND50")==0) s->materials[s->num_materials].trasparencytype=MIXTYPE_BLEND50;
		else
		if (strcmp(stri, "ADD")==0) s->materials[s->num_materials].trasparencytype=MIXTYPE_ADD;
		else
		if (strcmp(stri, "ALPHA")==0) s->materials[s->num_materials].trasparencytype=MIXTYPE_ALPHABLEND;
        else
		{
			// carico da file la mappa alpha
            s->materials[s->num_materials].alphamap_ptr=new texture;
			if (s->materials[s->num_materials].alphamap_ptr->load(stri, RGBINTERLEAVED)==FILENOTFOUND)
			  debug_error(miawin, "ADM script loader error: Alpha map file not found!");
            s->materials[s->num_materials].trasparencytype=ALPHA_MAP;
		}
        s->materials[s->num_materials].flags |= IS_TRASPARENT;
dopoNOT:
;
	  }

      if (strcmp(get_key_type(adm,ss),"trasparency_amount") == 0)
	  // leggo la quantià di trasparenza
	  {
        stri=strupr(get_key_value(adm,ss));
        tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].trasparency=tval;
	  }

      
	  if (strcmp(get_key_type(adm,ss),"alphamixfactor") == 0)
	  // leggo il fattore di alphamixing di texture+envmap
	  {
        stri=strupr(get_key_value(adm,ss));
	    tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].alphamixfactor=tval;
	  }

      if (strcmp(get_key_type(adm,ss),"mapsmixtype") == 0)
	  // leggo come devono essere mixate texture+envmap
	  {
        stri=strupr(get_key_value(adm,ss));
        if (strcmp(stri, "ADD")==0) 
 		    s->materials[s->num_materials].mapsmixtype=MIXTYPE_ADD;
        if (strcmp(stri, "TEXSUBENV")==0)  
		    s->materials[s->num_materials].mapsmixtype=MAPSMIXTYPE_TEXSUBENV;
        if (strcmp(stri, "ENVSUBTEX")==0) 
	 	    s->materials[s->num_materials].mapsmixtype=MAPSMIXTYPE_ENVSUBTEX;
	    if (strcmp(stri, "BLEND50")==0) 
		    s->materials[s->num_materials].mapsmixtype=MIXTYPE_BLEND50;
	    if (strcmp(stri, "ALPHABLEND")==0) 
		    s->materials[s->num_materials].mapsmixtype=MIXTYPE_ALPHABLEND;
	  }

      if (strcmp(get_key_type(adm,ss),"lightmixtype") == 0)
	  // leggo come la luce va mixata al materiale
	  {
        stri=strupr(get_key_value(adm,ss));
        if (strcmp(stri, "ADD")==0)
		    s->materials[s->num_materials].lightmixtype=MIXTYPE_ADD;
        if (strcmp(stri, "SUB")==0)
		    s->materials[s->num_materials].lightmixtype=LIGHTMIXTYPE_SUB;
	    if (strcmp(stri, "BLEND50")==0)
		    s->materials[s->num_materials].lightmixtype=MIXTYPE_BLEND50;
	    if (strcmp(stri, "NONE")==0)
		    s->materials[s->num_materials].lightmixtype=LIGHTMIXTYPE_NONE;
	    if (strcmp(stri, "BLEND3DS")==0)
		    s->materials[s->num_materials].lightmixtype=LIGHTMIXTYPE_BLEND3DS;
	  }

      if (strcmp(get_key_type(adm,ss),"texture") == 0)
      {
        // sono in una texture
        stri=(get_key_value(adm,ss));
        i=0;
		j=-1;
		/*
        while (i<s->num_textures) { 
			                        if (strcmp(s->textures[i].name, stri)==0)
									j=i;
									i++;
		}*/
        if (j>=0) s->materials[s->num_materials].texture_ptr=&s->textures[j];
	    else {
                if (s->textures[s->num_textures].load(stri, RGBINTERLEAVED)==FILENOTFOUND)
				   debug_error(miawin, "ADM script loader error: Texture file not found!");
                s->materials[s->num_materials].texture_ptr=&s->textures[s->num_textures];
                s->num_textures++;
		}
		s->materials[s->num_materials].flags |= PAINT_TEXTURE;
	  }

      if (strcmp(get_key_type(adm,ss),"envmap") == 0)
      {
        // sono in una envmap
        stri=(get_key_value(adm,ss));
        i=0;
		j=-1;
		/*
        while (i<s->num_textures) { 
			                        if (strcmp(s->textures[i].name, stri)==0)
									j=i;
									i++;
		} */
        if (j>=0) {
                    s->materials[s->num_materials].envmap_ptr=&s->textures[j];
                    s->textures[j].fdimx_opt=ENVMAP_PRECISION*s->textures[j].dimx;
                    s->textures[j].fdimy_opt=ENVMAP_PRECISION*s->textures[j].dimy;
		}
        else {
                if (s->textures[s->num_textures].load(stri, RGBINTERLEAVED)==FILENOTFOUND)
				   debug_error(miawin, "ADM script loader error: Envmap file not found!");
                s->materials[s->num_materials].envmap_ptr=&s->textures[s->num_textures];
                s->textures[s->num_textures].fdimx_opt=ENVMAP_PRECISION*s->textures[s->num_textures].dimx;
                s->textures[s->num_textures].fdimy_opt=ENVMAP_PRECISION*s->textures[s->num_textures].dimy;
                s->num_textures++;
		}
		s->materials[s->num_materials].flags |= PAINT_ENVMAP;
	  }
      ss=get_next_key(adm,ss);
    }
    s->num_materials++;
  }
  pos=find_next_section(adm,pos);
 }

 
 delete [] adm;
 return 0;
}





// ----------------------------------------------
// procedure proprie per gli script delle luci
// ----------------------------------------------
int load_ADL (char *nfile, AD_Scene3D *s)
{
  int pos, ss, i;
  char *stri;
  char *adl=alloc_script(nfile);
  float fval;

  pos=find_first_section(adl, 0);
  while (pos!=-1)
  {
    if (strcmp(get_section_type(adl, pos),"light") == 0)
    {
      // entro in una luce
      stri=get_section_value(adl, pos);
      i=s->index_of_omni(stri);
	  // ho trovato la luce specificata dallo script
	  if ((i>=0) && (i<s->num_omni))
	  {
        ss=get_subsection(adl, pos);
        ss=get_first_key(adl, ss);
        while(ss!=-1)
		{
          if (strcmp(get_key_type(adl, ss),"scale_x") == 0)
		  {
            stri=(get_key_value(adl, ss));
	        fval=my_atof(stri);
            s->omni[i].flare_scale_x=fval;
		  }

          if (strcmp(get_key_type(adl, ss),"scale_y") == 0)
		  {
            stri=(get_key_value(adl, ss));
	        fval=my_atof(stri);
            s->omni[i].flare_scale_y=fval;
		  }

          if (strcmp(get_key_type(adl, ss),"near_radius") == 0)
		  {
            stri=(get_key_value(adl, ss));
	        fval=my_atof(stri);
            s->omni[i].near_radius=fval;
		  }

          if (strcmp(get_key_type(adl, ss),"far_radius") == 0)
		  {
            stri=(get_key_value(adl, ss));
	        fval=my_atof(stri);
            s->omni[i].far_radius=fval;
		  }

          if (strcmp(get_key_type(adl, ss),"texture") == 0)
	      // leggo la texture di flare
		  {
            stri=(get_key_value(adl, ss));
			// se il paranetro è NONE non devo leggere nessuna texture
			if (strcmp(stri, "NONE") != 0)
			{
		       s->omni[i].flare=new texture;
               if (s->omni[i].flare->load(stri, RGBINTERLEAVED)==FILENOTFOUND)
		         debug_error(miawin, "ADL script loader error: texture(flare) file not found!");
			}
		  }

		  ss=get_next_key(adl, ss);
		}
	  }
    }
    pos=find_next_section(adl, pos);
  }

  delete [] adl;
  return 0;
}


// ----------------------------------------------
// procedure proprie per gli script dei dummy
// ----------------------------------------------
int load_ADD (char *nfile, AD_Scene3D *s)
{
  int pos, ss, i, j, flag;
  char *stri;
  char *add=alloc_script(nfile);
  float fval;

  pos=find_first_section(add, 0);
  while (pos!=-1)
  {
    if (strcmp(get_section_type(add, pos),"dummy") == 0)
    {
      // entro in un dummy
      stri=get_section_value(add, pos);
      i=s->index_of_object(stri);
	  // ho trovato il dummy specificato dallo script
	  if ((i>=0) && (i<s->num_objects))
	  {
        ss=get_subsection(add, pos);
        ss=get_first_key(add, ss);
        while(ss!=-1)
		{
          if (strcmp(get_key_type(add, ss),"scale_x") == 0)
		  {
            stri=(get_key_value(add, ss));
	        fval=my_atof(stri);
            s->objects[i]->flare_scale_x=fval;
		  }

          if (strcmp(get_key_type(add, ss),"scale_y") == 0)
		  {
            stri=(get_key_value(add, ss));
	        fval=my_atof(stri);
            s->objects[i]->flare_scale_y=fval;
		  }


          if (strcmp(get_key_type(add, ss),"texture") == 0)
	      // leggo la texture di flare
		  {
			stri=(get_key_value(add, ss));
			if (strcmp(stri, "NONE") != 0)
			{
			  // controllo che la texture non esista gia' (per altri dummy)
			  flag=-1;
			  for (j=0; j<s->num_objects; j++)
			  {
			    if ((s->objects[j]->flare!=(texture *)NULL) &&
				    (s->objects[j]->type==DUMMY)            &&
				    (strcmp(s->objects[j]->flare->name, stri)==0))
			    flag=j;
			  }
			  if (flag>=0) s->objects[i]->flare=s->objects[flag]->flare;
			  else
              if (flag==-1)
			  {
				// creo una nuova texture per il flare e la carico
			    s->objects[i]->flare=new texture;
                if (s->objects[i]->flare->load(stri, RGBINTERLEAVED)==FILENOTFOUND)
		            debug_error(miawin, "ADD script loader error: texture(flare) file not found!");
			  }
			}
		  }
		  ss=get_next_key(add, ss);
		}
	  }
    }
    pos=find_next_section(add, pos);
  }

  delete [] add;
  return 0;
}


// --------------------------------------------------------
// procedure proprie per gli script delle mesh triangolari
// e BezierPatch
// --------------------------------------------------------
int load_ADO (char *nfile, AD_Scene3D *s)
{
  int pos, ss, i;
  char *stri;
  char *ado=alloc_script(nfile);
  float fval;
  AD_PatchObject *patch_obj;

  pos=find_first_section(ado, 0);
  while (pos!=-1)
  {
    if (strcmp(get_section_type(ado, pos),"patch") == 0)
    {
      // entro in un patchobject
      stri=get_section_value(ado, pos);
      i=s->index_of_object(stri);
	  // ho trovato l'oggetto specificato dallo script
	  if ((i>=0) && (i<s->num_objects))
	  {
        ss=get_subsection(ado, pos);
        ss=get_first_key(ado, ss);
        while(ss!=-1)
		{
          if (strcmp(get_key_type(ado, ss),"u_steps") == 0)
		  {
            stri=(get_key_value(ado, ss));
	        fval=my_atof(stri);
            patch_obj=(AD_PatchObject *)s->objects[i];
            patch_obj->u_evaluations=fval;
		  }

          if (strcmp(get_key_type(ado, ss),"v_steps") == 0)
		  {
            stri=(get_key_value(ado, ss));
	        fval=my_atof(stri);
            patch_obj=(AD_PatchObject *)s->objects[i];
            patch_obj->v_evaluations=fval;
		  }

          if (strcmp(get_key_type(ado, ss),"swap_normals") == 0)
		  {
            stri=(get_key_value(ado, ss));
            patch_obj=(AD_PatchObject *)s->objects[i];
            if (strcmp(stri, "ON")==0)
				patch_obj->swap_normals=TRUE;
			else patch_obj->swap_normals=FALSE;
		  }

		  ss=get_next_key(ado, ss);
		}
	  }
    }
    pos=find_next_section(ado, pos);
  }

  delete [] ado;
  return 0;
}


// -----------------------------------------------------------
// procedure proprie per gli script dei particle systems e WSM
// -----------------------------------------------------------
int load_ADP (char *nfile, AD_Scene3D *s)
{
  int pos, ss, i, j, flag, ival;
  char *stri;
  char *adp=alloc_script(nfile);
  float fval;
  AD_ParticleSystem *psys;
  AD_WindModifier *wmod;
  AD_GravityModifier *gmod;


  pos=find_first_section(adp, 0);
  while (pos!=-1)
  {
    if (strcmp(get_section_type(adp, pos), "particle") == 0)
    {
      // entro in un particle system
      stri=get_section_value(adp, pos);
      i=s->index_of_object(stri);
	  if ((i>=0) && (i<s->num_objects))
	  {
        ss=get_subsection(adp, pos);
        ss=get_first_key(adp, ss);
        while(ss!=-1)
		{
          if (strcmp(get_key_type(adp, ss),"max_particles") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        ival=atoi(stri);
            psys=(AD_ParticleSystem *)s->objects[i];
            psys->max_particles=ival;
		  }

          if (strcmp(get_key_type(adp, ss),"start_time") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            psys=(AD_ParticleSystem *)s->objects[i];
            psys->start_time=fval;
		  }

          if (strcmp(get_key_type(adp, ss),"end_time") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            psys=(AD_ParticleSystem *)s->objects[i];
            psys->end_time=fval;
		  }

          if (strcmp(get_key_type(adp, ss),"life") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            psys=(AD_ParticleSystem *)s->objects[i];
            psys->life=fval;
		  }

          if (strcmp(get_key_type(adp, ss),"speed") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            psys=(AD_ParticleSystem *)s->objects[i];
            psys->emitter_speed=fval;
		  }

          if (strcmp(get_key_type(adp, ss),"variation") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            psys=(AD_ParticleSystem *)s->objects[i];
            psys->emitter_variation=fval;
		  }

          if (strcmp(get_key_type(adp, ss),"scale_x") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            s->objects[i]->flare_scale_x=fval;
		  }

          if (strcmp(get_key_type(adp, ss),"scale_y") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            s->objects[i]->flare_scale_y=fval;
		  }


          if (strcmp(get_key_type(adp, ss),"texture") == 0)
	      // leggo la texture di flare
		  {
			stri=(get_key_value(adp, ss));
			if (strcmp(stri, "NONE") != 0)
			{
			  // controllo che la texture non esista gia' per altri dummy
			  flag=-1;
			  for (j=0; j<s->num_objects; j++)
			  {
			    if ((s->objects[j]->flare!=(texture *)NULL) &&
				    (s->objects[j]->type==DUMMY)            &&
				    (strcmp(s->objects[j]->flare->name, stri)==0))
			    flag=j;
			  }
			  if (flag>=0) s->objects[i]->flare=s->objects[flag]->flare;
			  else
              if (flag==-1)
			  {
			     s->objects[i]->flare=new texture;
                 if (s->objects[i]->flare->load_jpg(stri, RGBINTERLEAVED)==FILENOTFOUND)
		            debug_error(miawin, "ADP script loader error: texture(particle) file not found!");
			  }
			}
		  }
		  ss=get_next_key(adp, ss);
		}
	  }
    }

    
	// entro in un Wind Modifier
	if (strcmp(get_section_type(adp, pos), "windWSM") == 0)
    {
      stri=get_section_value(adp, pos);
      i=s->index_of_object(stri);
	  if ((i>=0) && (i<s->num_objects))
	  {
        ss=get_subsection(adp, pos);
        ss=get_first_key(adp, ss);
        while(ss!=-1)
		{
          if (strcmp(get_key_type(adp, ss),"strength") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            wmod=(AD_WindModifier *)s->objects[i];
            wmod->strenght=fval;
		  }
          if (strcmp(get_key_type(adp, ss),"decay") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            wmod=(AD_WindModifier *)s->objects[i];
            wmod->decay=fval;
		  }
          if (strcmp(get_key_type(adp, ss),"turbolence") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            wmod=(AD_WindModifier *)s->objects[i];
            wmod->turbolence=fval;
		  }
          if (strcmp(get_key_type(adp, ss),"frequency") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            wmod=(AD_WindModifier *)s->objects[i];
            wmod->frequency=fval;
		  }
          if (strcmp(get_key_type(adp, ss),"scale") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            wmod=(AD_WindModifier *)s->objects[i];
            wmod->scale=fval;
		  }
		  ss=get_next_key(adp, ss);
		}
	  }
	}


    // entro in un Gravity Modifier
	if (strcmp(get_section_type(adp, pos), "gravityWSM") == 0)
    {
      stri=get_section_value(adp, pos);
      i=s->index_of_object(stri);
	  if ((i>=0) && (i<s->num_objects))
	  {
        ss=get_subsection(adp, pos);
        ss=get_first_key(adp, ss);
        while(ss!=-1)
		{
          if (strcmp(get_key_type(adp, ss),"strength") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            gmod=(AD_GravityModifier *)s->objects[i];
            gmod->strenght=fval;
		  }
          if (strcmp(get_key_type(adp, ss),"decay") == 0)
		  {
            stri=(get_key_value(adp, ss));
	        fval=my_atof(stri);
            gmod=(AD_GravityModifier *)s->objects[i];
            gmod->decay=fval;
		  }
		  ss=get_next_key(adp, ss);
		}
	  }
	}

    pos=find_next_section(adp, pos);
  }

  delete [] adp;
  return 0;
}