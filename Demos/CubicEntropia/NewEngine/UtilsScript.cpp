// Utility per gli script dei materiali (.ADM), luci (.ADL),
// oggetti (.ADO), particle system (.ADP)
// Made by [AD]Turbo

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include "Utilsscript.h"
#include "A3DFile.h"
#include "ParticleSystem.h"
#include "Utility.h"

extern HWND miawin;         // finestra principale; usata qui per le debug_error


// -------------------------------------------------
// procedure di utilita' generale a tutti gli script
// -------------------------------------------------

void get_file_ext(char8 *nomefile, char8 *out)
// si ottiene l'estensione da un nomefile (es: "pippo.pcx" -> "PCX")
{
  int32 i, j;

  j=-1;
  for (i=0; i<(int32)strlen(nomefile); i++)
     if (nomefile[i]=='.') { j=i; break; }

  if (j<0) {
             strcpy(out, "");
             return;
           }
  out[0]=toupper(nomefile[j+1]);
  out[1]=toupper(nomefile[j+2]);
  out[2]=toupper(nomefile[j+3]);
  out[3]='\0';
}

//--------------------------------------------------------------------

int32 find_first_section(char8 *memfile, int32 pos)
{
  int32 i=pos;

  while (memfile[i]==' ') i++;
  if (memfile[i]==0) return -1;
  return i;
}

//--------------------------------------------------------------------

int32 find_next_section(char8 *memfile, int32 pos)
{
  int32 i=pos;

  while((memfile[i]!='}') && (memfile[i]!=0)) i++;
  if (memfile[i]==0) return -1;
  i++;
  while((memfile[i]==' ') && (memfile[i]!=0)) i++;
  if (memfile[i]==0) return -1;
  return i;
}

//--------------------------------------------------------------------

void get_section_type(char8 *memfile, int32 pos, char8 *out)
{
  int32 i=pos, n=0;

  while((memfile[i]!=' ') && (memfile[i]!='"'))
	  out[n++]=memfile[i++];

  out[n]='\0';
}

//--------------------------------------------------------------------

void get_section_value(char8 *memfile, int32 pos, char8 *out)
{
  int32 i=pos, n=0;

  while(memfile[i]!='"') i++;
  i++;
  while(memfile[i]!='"') out[n++]=memfile[i++];
  out[n]='\0';
}

//--------------------------------------------------------------------

int32 get_subsection(char8 *memfile, int32 pos)
{
  int32 i=pos;
  
  while(memfile[i]!='{') i++;
  i++;
 
  return i;
}

//--------------------------------------------------------------------

int32 get_first_key(char8 *memfile, int32 pos)
{
  int32 i=pos;
  
  while(memfile[i]==' ') i++;
 
  if (memfile[i]=='}') return -1;
  return i;
}

//--------------------------------------------------------------------

int32 get_next_key(char* memfile, int32 pos)
{
  int32 i=pos;
  
  while(memfile[i]!=';') i++;
  i++; 
  while(memfile[i]==' ') i++;

  if (memfile[i]=='}') return -1;
  return i;
}

//--------------------------------------------------------------------

void get_key_type(char8 *memfile, int32 pos, char8 *out)
{
  int32 i, n;

  i=pos;
  n=0;
  while((memfile[i]!=' ') && (memfile[i]!='='))
	 out[n++]=memfile[i++];
  out[n]='\0';
}

//--------------------------------------------------------------------

int32 get_key_value(char8 *memfile, int32 pos, int32 quale, char8 *out)
{
  int32 i=pos, n=0, j, flag;

  if (quale<0) return(0);
  while(memfile[i]!='"') i++;
  i++;
  if (quale==0) goto estrai;

  // devo trovare la quale-esima virgola, con 'quale' che va da 0 a
  // un numero indefinito positivo qualsiasi
  j=0;
  flag=0;
  while ((flag==0) && (memfile[i]!='"'))
  {
	if (memfile[i]==',') j++;
	if (j==quale) flag=1;
	i++;
  }
  // virgola non trovata (si puo' segnalare errore di sintassi)
  if (!flag) return(0);

estrai:
;
  while ((memfile[i] != '"') && (memfile[i] != ','))
  {
	// la condizione su quale mi serve per evitare che la
	// prima key venga estratta con gli spazi eliminati (ad esempio
	// per gli ADO i materiali sono anche del tipo "1 - Default")
	if ((memfile[i]!=' ') || (quale==0)) out[n++]=memfile[i];
	i++;
  }
  out[n]='\0';
  return(1);
}

//--------------------------------------------------------------------

char8 *alloc_script(char8 *filename)
// alloca in memoria il file togliendo commenti, spazi inutili e andate a capo
{
  FILE *bfile;
  char8 *memfile;
  char8 tmp;
  int32 i=0;

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

//--------------------------------------------------------------------



// ----------------------------------------------
// procedure proprie per gli script dei materiali
// ----------------------------------------------
typedef struct
{
  int32 ID;
  char8 name[100]; 
} myBITMAP;


CGraphicSurface *get_bitmap_ADM(int32 id, myBITMAP *bmps,
								int32 num_bmp, CScene3D *scn)
{
   int32 i;

   i=0;
   while (i<num_bmp)
   {
       if (bmps[i].ID==id)
	   {
		  return(scn->get_texture_pointer(bmps[i].name));
	   }
	   i++;
   }
   return(NULL);
}


int32 load_ADM (char8 *nfile, CScene3D *s)
// carica un file .ADM
{
  int32 pos, ss, nmat, ntex, i, tval, color;
  char8 stri[100], stri2[100];
  char8 *adm=alloc_script(nfile);
  float4 fval;
  myBITMAP bitmaps[50];
  char8 msg[150];

  // inizio contando il numero max di materiali e textures che allochero'
  nmat=0;
  ntex=0;
  pos=find_first_section(adm, 0);
  while (pos != -1)
  {
	get_section_type(adm, pos, stri);
    if (strcmp(stri, "material") == 0) nmat++;
    if (strcmp(stri, "bitmap") == 0) ntex++;
    pos=find_next_section(adm, pos);
  }

  // alloco e carico le texture/bitmap
  s->p_Textures=new CGraphicSurface[ntex];
  s->p_NumTextures=0;
  pos=find_first_section(adm, 0);
  while (pos != -1)
  {
	get_section_type(adm, pos, stri);
    if (strcmp(stri, "bitmap")==0)
    {
      // entro in un materiale
      ss=get_subsection(adm, pos);
      ss=get_first_key(adm, ss);
      while (ss != -1)
	  {
		get_key_type(adm, ss, stri);
	    if (strcmp(stri, "id") == 0)
		{
            get_key_value(adm, ss, 0, stri2);
			bitmaps[s->p_NumTextures].ID=atoi(stri2);
		}
	    if (strcmp(stri, "filename") == 0)
		{
            get_key_value(adm, ss, 0, stri2);
			strcpy(bitmaps[s->p_NumTextures].name, stri2);
			if (s->p_Textures[s->p_NumTextures].m_Load(stri2, RGBINTERLEAVED)==FILENOTFOUND)
			{
               strcpy(msg, "ADM script loader error: ");
			   strcat(msg, stri2);
			   strcat(msg, " file not found!");
		       debug_error(miawin, msg);
			}
		}
	    if (strcmp(stri, "mipmaplevels") == 0)
		{
            get_key_value(adm, ss, 0, stri2);
			i=atoi(stri2);
            s->p_Textures[s->p_NumTextures].p_MipMapLevels=i;
		}
	    if (strcmp(stri, "start_time") == 0)
		{
            get_key_value(adm, ss, 0, stri2);
			fval=my_atof(stri2);
            s->p_Textures[s->p_NumTextures].p_StartFrame=fval;
		}
	    if (strcmp(stri, "playback_rate") == 0)
		{
            get_key_value(adm, ss, 0, stri2);
			fval=my_atof(stri2);
            s->p_Textures[s->p_NumTextures].p_PlayBackRate=fval;
		}
	    if (strcmp(stri, "end_condition") == 0)
		{
            get_key_value(adm, ss, 0, stri2);
            strupr(stri2);
			if (strcmp(stri2, "HOLD")==0)
              s->p_Textures[s->p_NumTextures].p_EndCondition=ENDCONDITION_HOLD;
			else
			if (strcmp(stri2, "PINGPONG")==0)
              s->p_Textures[s->p_NumTextures].p_EndCondition=ENDCONDITION_PINGPONG;
			else s->p_Textures[s->p_NumTextures].p_EndCondition=ENDCONDITION_LOOP;
		}
        ss=get_next_key(adm, ss);
	  }
      s->p_NumTextures++;
	}
    pos=find_next_section(adm, pos);
  }
  
  // alloco il numero esatto di materiali
  s->p_Materials=new CMaterial[nmat];

  // carico i materiali
  pos=find_first_section(adm, 0);
  s->p_NumMaterials=0;
  while (pos != -1)
  {
	get_section_type(adm, pos, stri);
    if (strcmp(stri, "material")==0)
    {
      // entro in un materiale
      get_section_value(adm, pos, stri);
      strcpy(s->p_Materials[s->p_NumMaterials].p_Name, stri);
      ss=get_subsection(adm, pos);
      ss=get_first_key(adm, ss);
      while(ss != -1)
	  {
		get_key_type(adm, ss, stri);
        // valori di tiling delle UV
	    if (strcmp(stri, "map1tile") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_U1Tile=fval;
          get_key_value(adm, ss, 1, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_V1Tile=fval;
		}
	    if (strcmp(stri, "map2tile") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_U2Tile=fval;
          get_key_value(adm, ss, 1, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_V2Tile=fval;
		}
        // valori di rotazione delle UV
	    if (strcmp(stri, "map1angle") == 0)
		{
          get_key_value(adm, ss, 2, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_W1Angle=fval;
		}
	    if (strcmp(stri, "map2angle") == 0)
		{
          get_key_value(adm, ss, 2, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_W2Angle=fval;
		}
        // valori di offset/traslazioni delle UV
	    if (strcmp(stri, "map1offset") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_U1Offset=fval;
          get_key_value(adm, ss, 1, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_V1Offset=fval;
		}
	    if (strcmp(stri, "map2offset") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_U2Offset=fval;
          get_key_value(adm, ss, 1, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_V2Offset=fval;
		}
        // specularita' della luce
	    if (strcmp(stri, "specular") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
	      if ((strcmp(stri2, "ON")==0) || (strcmp(stri2, "on")==0))
            s->p_Materials[s->p_NumMaterials].p_Specular=ON;
          else s->p_Materials[s->p_NumMaterials].p_Specular=OFF;
		}

        // rendering wireframe
	    if (strcmp(stri, "wireframe") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
	      if ((strcmp(stri2, "ON")==0) || (strcmp(stri2, "on")==0))
            s->p_Materials[s->p_NumMaterials].p_WireFrame=ON;
          else s->p_Materials[s->p_NumMaterials].p_WireFrame=OFF;

		  int32 col=0;
		  if (get_key_value(adm, ss, 1, stri2))
		  {
			 tval=atoi(stri2);
			 if (tval<0) tval=0;
			 else if (tval>255) tval=255;
			 col=col | (tval << 16);
		     if (get_key_value(adm, ss, 2, stri2))
			 {
			   tval=atoi(stri2);
			   if (tval<0) tval=0;
			   else if (tval>255) tval=255;
			   col=col | (tval << 8);
 		       if (get_key_value(adm, ss, 3, stri2))
			   {
			     tval=atoi(stri2);
			     if (tval<0) tval=0;
			     else if (tval>255) tval=255;
			     col=col | (tval);
			   }
			 }
             s->p_Materials[s->p_NumMaterials].p_WireFrameColor=col;
		  }
		}

	    if (strcmp(stri, "next_material") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
		  if ((strcmp(stri2, "NONE")==0) || (strcmp(stri2, "none")==0))
             s->p_Materials[s->p_NumMaterials].p_NextMaterial=NULL;
		  else strcpy(s->p_Materials[s->p_NumMaterials].p_ChildrenName, stri2);
		}

        if (strcmp(stri, "backfaceculling") == 0)
	    // leggo l'opzione di double facing
		{
          get_key_value(adm, ss, 0, stri2);
		  strupr(stri2);
	      if (strcmp(stri2, "FRONTFACE")==0) 
		    s->p_Materials[s->p_NumMaterials].p_BackFaceCulling=FRONTFACE_CULLING;
	      else
	      if (strcmp(stri2, "BACKFACE")==0) 
		    s->p_Materials[s->p_NumMaterials].p_BackFaceCulling=BACKFACE_CULLING;
		  else
          if (strcmp(stri2, "NONE")==0) 
            s->p_Materials[s->p_NumMaterials].p_BackFaceCulling=OFF;
		  else s->p_Materials[s->p_NumMaterials].p_BackFaceCulling=OFF;
		}

        if (strcmp(stri, "transparency_type") == 0)
	    // leggo il fattore di trasparenza
		{
          get_key_value(adm, ss, 0, stri2);
		  strupr(stri2);
		  if (strcmp(stri2, "NONE")==0)
            s->p_Materials[s->p_NumMaterials].p_TransparencyType=OFF;
		  else
		  if (strcmp(stri2, "ADD")==0) s->p_Materials[s->p_NumMaterials].p_TransparencyType=MIXTYPE_ADD;
		  else
		  if (strcmp(stri2, "ALPHA")==0) s->p_Materials[s->p_NumMaterials].p_TransparencyType=MIXTYPE_ALPHABLEND;
          else
		  if (strcmp(stri2, "MAP1")==0) s->p_Materials[s->p_NumMaterials].p_TransparencyType=ALPHA_MAP1;
          else
		  {
		     i=atoi(stri2);
             s->p_Materials[s->p_NumMaterials].p_AlphaMap=
			   get_bitmap_ADM(i, bitmaps, s->p_NumTextures, s);
             if (!s->p_Materials[s->p_NumMaterials].p_AlphaMap)
			 {
			   strcpy(msg, "ADM script loader error: ");
			   strcat(msg, s->p_Materials[s->p_NumMaterials].p_Name);
			   strcat(msg, " Alphamap(id) not found!");
	           debug_error(miawin, msg);
			 }
			 else
               s->p_Materials[s->p_NumMaterials].p_TransparencyType=ALPHA_MAP;
		  }
		}

        if (strcmp(stri, "transparency_amount") == 0)
	    // leggo la quantià di trasparenza
		{
          get_key_value(adm, ss, 0, stri2);
          tval=atoi(stri2);
	      if (tval<0) tval=0; if (tval>255) tval=255;
	      s->p_Materials[s->p_NumMaterials].p_TransparencyAmount=(float4)(tval/255.0);
		}

        if (strcmp(stri, "mapsmixtype") == 0)
	    // leggo come devono essere mixate texture+envmap
		{
          get_key_value(adm, ss, 0, stri2);
          strupr(stri2);
          if (strcmp(stri2, "ADD")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_ADD;
		  else
          if (strcmp(stri2, "SUB")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_SUB;
		  else
	      if (strcmp(stri2, "MODULATE")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_MODULATE;
		  else
	      if (strcmp(stri2, "MODULATE2X")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_MODULATE2X;
		  else
	      if (strcmp(stri2, "MODULATE4X")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_MODULATE4X;
		  else
	      if (strcmp(stri2, "NONE")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_NONE;
		  else
	      if (strcmp(stri2, "ADDSIGNED")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_ADDSIGNED;
		  else
	      if (strcmp(stri2, "ADDSIGNED2X")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_ADDSIGNED2X;
		  else
	      if (strcmp(stri2, "ADDSMOOTH")==0)
		      s->p_Materials[s->p_NumMaterials].p_MapsMixType=MIXTYPE_ADDSMOOTH;
		}

        if (strcmp(stri, "lightmixtype") == 0)
	    // leggo come la luce va mixata al materiale
		{
          get_key_value(adm, ss, 0, stri2);
          strupr(stri2);
          if (strcmp(stri2, "ADD")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_ADD;
		  else
          if (strcmp(stri2, "SUB")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_SUB;
		  else
	      if (strcmp(stri2, "MODULATE")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_MODULATE;
		  else
	      if (strcmp(stri2, "MODULATE2X")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_MODULATE2X;
		  else
	      if (strcmp(stri2, "MODULATE4X")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_MODULATE4X;
		  else
	      if (strcmp(stri2, "NONE")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_NONE;
		  else
	      if (strcmp(stri2, "ADDSIGNED")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_ADDSIGNED;
		  else
	      if (strcmp(stri2, "ADDSIGNED2X")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_ADDSIGNED2X;
		  else
	      if (strcmp(stri2, "ADDSMOOTH")==0)
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_ADDSMOOTH;
		  else
	      if (strcmp(stri2, "TOONSHADE")==0)
		  {
		      s->p_Materials[s->p_NumMaterials].p_LightMixType=MIXTYPE_TOONSHADE;
			  // leggo gli eventuali parametri
              if (get_key_value(adm, ss, 1, stri2))
			  {
                fval=my_atof(stri2);
                s->p_Materials[s->p_NumMaterials].p_ToonScale=fval;
                if (get_key_value(adm, ss, 2, stri2))
				{
				   if (strcmp(stri2, "MODULATE")== 0)
                      s->p_Materials[s->p_NumMaterials].p_ToonLightsMixType=MIXTYPE_MODULATE;
				   else
				   if (strcmp(stri2, "ADDSMOOTH")== 0)
                      s->p_Materials[s->p_NumMaterials].p_ToonLightsMixType=MIXTYPE_ADDSMOOTH;
				   else s->p_Materials[s->p_NumMaterials].p_ToonLightsMixType=MIXTYPE_ADD;
				   // mix finale della mappa
                   if (get_key_value(adm, ss, 3, stri2))
				   {
				     if (strcmp(stri2, "ADD")== 0)
                        s->p_Materials[s->p_NumMaterials].p_ToonMapsMixType=MIXTYPE_ADD;
					 else
				     if (strcmp(stri2, "ADDSMOOTH")== 0)
                        s->p_Materials[s->p_NumMaterials].p_ToonMapsMixType=MIXTYPE_ADDSMOOTH;
					 else
				     if (strcmp(stri2, "MODULATE2X")== 0)
                        s->p_Materials[s->p_NumMaterials].p_ToonMapsMixType=MIXTYPE_MODULATE2X;
				     else
						s->p_Materials[s->p_NumMaterials].p_ToonMapsMixType=MIXTYPE_MODULATE;
                     if (get_key_value(adm, ss, 4, stri2))
					 {
                        // se non è gia' associato una mappa di gradient ne alloco
                        // una nuova e la carico
						if (!s->p_Materials[s->p_NumMaterials].p_ToonGradient)
						{
						   s->p_Materials[s->p_NumMaterials].p_ToonGradient=new CGraphicSurface;
						   if (s->p_Materials[s->p_NumMaterials].p_ToonGradient->m_Load(stri2, RGBINTERLEAVED)==FILENOTFOUND)
						   {
			                  strcpy(msg, "ADM script loader error: ");
			                  strcat(msg, s->p_Materials[s->p_NumMaterials].p_Name);
			                  strcat(msg, " Toon Map not found!");
	                          debug_error(miawin, msg);
						   }
						}
					 }
				   }
				}
			  }
		  }
		}

        if (strcmp(stri, "map1") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
		  i=atoi(stri2);
          s->p_Materials[s->p_NumMaterials].p_Map1=
			  get_bitmap_ADM(i, bitmaps, s->p_NumTextures, s);
          if (!s->p_Materials[s->p_NumMaterials].p_Map1)
		  {
			 strcpy(msg, "ADM script loader error: ");
			 strcat(msg, s->p_Materials[s->p_NumMaterials].p_Name);
			 strcat(msg, " Map1(id) not found!");
	         debug_error(miawin, msg);
		  }
		}

        if (strcmp(stri, "map2") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
		  i=atoi(stri2);
          s->p_Materials[s->p_NumMaterials].p_Map2=
			  get_bitmap_ADM(i, bitmaps, s->p_NumTextures, s);
          if (!s->p_Materials[s->p_NumMaterials].p_Map2)
		  {
			 strcpy(msg, "ADM script loader error: ");
			 strcat(msg, s->p_Materials[s->p_NumMaterials].p_Name);
			 strcat(msg, " Map2(id) not found!");
	         debug_error(miawin, msg);
		  }
		}

        if (strcmp(stri, "map1channel") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
		  if (strcmp(stri2, "ENV")==0)
			  s->p_Materials[s->p_NumMaterials].p_Map1Channel=ENVMAPPING;
		  else
		  if (strcmp(stri2, "TEX1")==0)
			  s->p_Materials[s->p_NumMaterials].p_Map1Channel=TEXMAPPING1;
		  else
		  if (strcmp(stri2, "TEX2")==0)
			  s->p_Materials[s->p_NumMaterials].p_Map1Channel=TEXMAPPING2;
		  else
		  if (strcmp(stri2, "SPEC")==0)
			  s->p_Materials[s->p_NumMaterials].p_Map1Channel=SPECULARMAPPING;
		}

        if (strcmp(stri, "map2channel") == 0)
		{
          get_key_value(adm, ss, 0, stri2);
		  if (strcmp(stri2, "ENV")==0)
			  s->p_Materials[s->p_NumMaterials].p_Map2Channel=ENVMAPPING;
		  else
		  if (strcmp(stri2, "TEX1")==0)
			  s->p_Materials[s->p_NumMaterials].p_Map2Channel=TEXMAPPING1;
		  else
		  if (strcmp(stri2, "TEX2")==0)
			  s->p_Materials[s->p_NumMaterials].p_Map2Channel=TEXMAPPING2;
		  else
		  if (strcmp(stri2, "SPEC")==0)
			  s->p_Materials[s->p_NumMaterials].p_Map2Channel=SPECULARMAPPING;
		}

        if (strcmp(stri, "bumpmap") == 0)
		{
          // tipo di bump (DOT3 o EMBM)
          get_key_value(adm, ss, 0, stri2);
		  if (strcmp(stri2, "DOT3")==0)
             s->p_Materials[s->p_NumMaterials].p_BumpType=BUMP_DOT3;
		  else
		  if (strcmp(stri2, "EMBM")==0)
             s->p_Materials[s->p_NumMaterials].p_BumpType=BUMP_EMBM;
		  else
             s->p_Materials[s->p_NumMaterials].p_BumpType=0;
          // mappa delle altezze
          get_key_value(adm, ss, 1, stri2);
		  if (strcmp(stri2, "AUTOMATIC")==0)
             s->p_Materials[s->p_NumMaterials].p_BumpType|=AUTOMATIC_BUMPMAP;
		  else
		  {
		    i=atoi(stri2);
            s->p_Materials[s->p_NumMaterials].p_BumpMap=
			    get_bitmap_ADM(i, bitmaps, s->p_NumTextures, s);
            if (!s->p_Materials[s->p_NumMaterials].p_BumpMap)
			{
			   strcpy(msg, "ADM script loader error: ");
			   strcat(msg, s->p_Materials[s->p_NumMaterials].p_Name);
			   strcat(msg, " Bumpmap(id) not found!");
	           debug_error(miawin, msg);
			}
		  }
		}

        if (strcmp(stri, "bumpscale") == 0)
		{
          // scale per il bump EMBM
          get_key_value(adm, ss, 0, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_BumpUScale=fval;
          get_key_value(adm, ss, 1, stri2);
	      fval=my_atof(stri2);
          s->p_Materials[s->p_NumMaterials].p_BumpVScale=fval;
		}

		color=0xFF000000;
        if (strcmp(stri, "fog") == 0)
		{
          // scale per il bump EMBM
          get_key_value(adm, ss, 0, stri2);
          if (strcmp(stri2, "LINEAR")==0)
			  s->p_Materials[s->p_NumMaterials].p_FogType=LINEARFOG;
		  else
          if (strcmp(stri2, "EXP")==0)
			  s->p_Materials[s->p_NumMaterials].p_FogType=EXPFOG;
		  else
          if (strcmp(stri2, "EXP2")==0)
			  s->p_Materials[s->p_NumMaterials].p_FogType=EXP2FOG;
		  if (get_key_value(adm, ss, 1, stri2))
		  {
	         tval=atoi(stri2) & 255;
			 color=color | (tval<<16); // R
		     if (get_key_value(adm, ss, 2, stri2))
			 {
	           tval=atoi(stri2) & 255;
			   color=color | (tval<<8); // G
		       if (get_key_value(adm, ss, 3, stri2))
			   {
	              tval=atoi(stri2) & 255;
				  color=color | tval; // B
		          if (get_key_value(adm, ss, 4, stri2))
				  {
	                fval=my_atof(stri2);  // Fog Start
			        s->p_Materials[s->p_NumMaterials].p_FogStart=fval;
		            if (get_key_value(adm, ss, 5, stri2))
					{
	                  fval=my_atof(stri2);  // Fog End
			          s->p_Materials[s->p_NumMaterials].p_FogEnd=fval;
		              if (get_key_value(adm, ss, 6, stri2))
					  {
	                    fval=my_atof(stri2);  // Fog Density
			            s->p_Materials[s->p_NumMaterials].p_FogDensity=fval;
					  }
					}
				  }
			   }
			 }
		  }
		  s->p_Materials[s->p_NumMaterials].p_FogColor=color;
		}


        ss=get_next_key(adm, ss);
      }
      s->p_NumMaterials++;
    }
    pos=find_next_section(adm, pos);
  }

  delete [] adm;
  return 0;
}





// ----------------------------------------------
// procedure proprie per gli script delle luci
// ----------------------------------------------
int32 load_ADL (char8 *nfile, CScene3D *s)
{
  int32 pos, ss, i;
  char8 stri[100], stri2[100];
  char8 *adl=alloc_script(nfile);
  float4 fval;

  pos=find_first_section(adl, 0);
  while (pos != -1)
  {
	get_section_type(adl, pos, stri);
    if (strcmp(stri, "light") == 0)
    {
      // entro in una luce
      get_section_value(adl, pos, stri);
      i=s->index_of_omni(stri);
	  // ho trovato la luce specificata dallo script
	  if ((i>=0) && (i<s->p_NumOmniLights))
	  {
        ss=get_subsection(adl, pos);
        ss=get_first_key(adl, ss);
        while(ss!=-1)
		{
          get_key_type(adl, ss, stri);

          if (strcmp(stri, "scale_x") == 0)
		  {
            get_key_value(adl, ss, 0, stri2);
	        fval=my_atof(stri2);
            s->p_OmniLights[i].p_FlareScaleX=fval;
		  }

          if (strcmp(stri, "scale_y") == 0)
		  {
            get_key_value(adl, ss, 0, stri2);
	        fval=my_atof(stri2);
            s->p_OmniLights[i].p_FlareScaleY=fval;
		  }

          if (strcmp(stri, "attenuation0") == 0)
		  {
            get_key_value(adl, ss, 0, stri2);
	        fval=my_atof(stri2);
            s->p_OmniLights[i].p_Attenuation0=fval;
		  }

          if (strcmp(stri, "attenuation1") == 0)
		  {
            get_key_value(adl, ss, 0, stri2);
	        fval=my_atof(stri2);
            s->p_OmniLights[i].p_Attenuation1=fval;
		  }

          if (strcmp(stri, "attenuation2") == 0)
		  {
            get_key_value(adl, ss, 0, stri2);
	        fval=my_atof(stri2);
            s->p_OmniLights[i].p_Attenuation2=fval;
		  }

          if (strcmp(stri, "max_range") == 0)
		  {
            get_key_value(adl, ss, 0, stri2);
	        fval=my_atof(stri2);
            s->p_OmniLights[i].p_MaxRange=fval;
		  }

          if (strcmp(stri, "material") == 0)
	      // leggo la texture di flare
		  {
            get_key_value(adl, ss, 0, stri2);
			// se il parametro è NONE non devo leggere nessuna texture
			if (strcmp(stri2, "NONE") != 0)
			{
			   s->p_OmniLights[i].p_BaseMaterial=s->get_material_pointer(stri2);
	           if (!s->p_OmniLights[i].p_BaseMaterial)
				   debug_error(miawin, "ADL script loader error: material not found!");
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

/*
// ----------------------------------------------
// procedure proprie per gli script dei dummy
// ----------------------------------------------
int32 load_ADD (char8 *nfile, CScene3D *s)
{
  int32 pos, ss, i, j, flag;
  char8 *stri;
  char8 *add=alloc_script(nfile);
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
	  if ((i>=0) && (i<s->p_NumObjects))
	  {
        ss=get_subsection(add, pos);
        ss=get_first_key(add, ss);
        while(ss!=-1)
		{
          if (strcmp(get_key_type(add, ss),"scale_x") == 0)
		  {
            stri=(get_key_value(add, ss));
	        fval=my_atof(stri);
            s->p_Objects[i]->flare_scale_x=fval;
		  }

          if (strcmp(get_key_type(add, ss),"scale_y") == 0)
		  {
            stri=(get_key_value(add, ss));
	        fval=my_atof(stri);
            s->p_Objects[i]->flare_scale_y=fval;
		  }


          if (strcmp(get_key_type(add, ss),"texture") == 0)
	      // leggo la texture di flare
		  {
			stri=(get_key_value(add, ss));
			if (strcmp(stri, "NONE") != 0)
			{
			  // controllo che la texture non esista gia' (per altri dummy)
			  flag=-1;
			  for (j=0; j<s->p_NumObjects; j++)
			  {
			    if ((s->objects[j]->flare!=(texture *)NULL) &&
				    (s->objects[j]->type==DUMMY)            &&
				    (strcmp(s->objects[j]->flare->name, stri)==0))
			    flag=j;
			  }
			  if (flag>=0) s->p_Objects[i]->flare=s->objects[flag]->flare;
			  else
              if (flag==-1)
			  {
				// creo una nuova texture per il flare e la carico
			    s->p_Objects[i]->flare=new CGraphicSurface;
                if (s->p_Objects[i]->flare->load(stri, RGBINTERLEAVED)==FILENOTFOUND)
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

*/
// --------------------------------------------------------
// procedure proprie per gli script delle mesh triangolari
// e BezierPatch
// --------------------------------------------------------
int32 load_ADO (char8 *nfile, CScene3D *s)
{
  int32 pos, ss, i, ival, j, flag;
  char8 stri[100], stri2[100];
  char8 *ado=alloc_script(nfile);
  float4 fval, fval2;
  CMaterial *material;
  CMesh *mesh;
  //AD_PatchObject *patch_obj;

  pos=find_first_section(ado, 0);
  while (pos != -1)
  {
    get_section_type(ado, pos, stri);
    if (strcmp(stri, "mesh") == 0)
	{
      get_section_value(ado, pos, stri); // leggo il nome della mesh
      i=s->index_of_mesh(stri);
	  if (i>=0)
	  {
        ss=get_subsection(ado, pos);
        ss=get_first_key(ado, ss);
        while(ss != -1)
		{
          get_key_type(ado, ss, stri);
		  // profondita' degli alberi di volume (sfere, AABB, OBB)
          if (strcmp(stri, "boundvolumes_depths") == 0)
		  {
            get_key_value(ado, ss, 0, stri2);
	        ival=atoi(stri2);
			s->p_Meshes[i]->p_SPHEREVolumeTreeDepth=ival;
            get_key_value(ado, ss, 1, stri2);
	        ival=atoi(stri2);
			s->p_Meshes[i]->p_AABBVolumeTreeDepth=ival;
            get_key_value(ado, ss, 2, stri2);
	        ival=atoi(stri2);
			s->p_Meshes[i]->p_OBBVolumeTreeDepth=ival;
		  }
		  // normalizzazione a raggio unitario
          if (strcmp(stri, "normalize") == 0)
		  {
            get_key_value(ado, ss, 0, stri2);
			if (strcmp(stri2, "ON")==0)
               s->p_Meshes[i]->m_NormalizeObjspaceVertex();
		  }
		  ss=get_next_key(ado, ss);
		}
	  }
	}
    
	if (strcmp(stri, "object") == 0)
	{
      get_section_value(ado, pos, stri); // leggo il nome dell'oggetto
      i=s->index_of_object(stri);
	  if (i>=0)
	  {
        ss=get_subsection(ado, pos);
        ss=get_first_key(ado, ss);
        while(ss != -1)
		{
          get_key_type(ado, ss, stri);
		  // leggo ilt ipo di oggetto geometrico
          if (strcmp(stri, "type") == 0)
		  {
            get_key_value(ado, ss, 0, stri2);
			if (strcmp(stri2, "SOLID")==0)
				s->p_Objects[i]->p_Type=SOLID_OBJECT;
            else
			if (strcmp(stri2, "DUMMY")==0)
				s->p_Objects[i]->p_Type=DUMMY_HELPER;
			else
			if (strcmp(stri2, "POINT")==0)
				s->p_Objects[i]->p_Type=POINT_HELPER;
			else
			if (strcmp(stri2, "BOXGIZMO")==0)
				s->p_Objects[i]->p_Type=BOXGIZMO_HELPER;
			else
			if (strcmp(stri2, "SPHEREGIZMO")==0)
				s->p_Objects[i]->p_Type=SPHEREGIZMO_HELPER;
			else
			if (strcmp(stri2, "BONE")==0)
				s->p_Objects[i]->p_Type=BONE_OBJECT;
			else
				s->p_Objects[i]->p_Type=UNKNOWN_GEOMETRIC_OBJECT;
		  }

		  // leggo il padre
          if (strcmp(stri, "father") == 0)
		  {
             get_key_value(ado, ss, 0, stri2);
			 strcpy(s->p_Objects[i]->p_FatherName, stri2);
		  }

		  // leggo il materiale base
          if (strcmp(stri, "material") == 0)
		  {
            get_key_value(ado, ss, 0, stri2);
			if (strcmp(stri2, "NONE")==0)
              s->p_Objects[i]->p_BaseMaterial=(CMaterial *)NULL;

            material=s->get_material_pointer(stri2);
			if (material)
              s->p_Objects[i]->p_BaseMaterial=material;
		  }

		  // leggo il materiale base
          if (strcmp(stri, "static_vertex") == 0)
		  {
             get_key_value(ado, ss, 0, stri2);
             if (strcmp(stri2, "NO")==0)
                s->p_Objects[i]->p_StaticVertex=0;
			 else s->p_Objects[i]->p_StaticVertex=1;
		  }

		  // leggo un lod
          if (strcmp(stri, "lod") == 0)
		  {
            // nome della mesh
			get_key_value(ado, ss, 0, stri2);
            mesh=s->get_mesh_pointer(stri2);
			// distanza minima
            get_key_value(ado, ss, 1, stri2);
	        fval=my_atof(stri2);
            get_key_value(ado, ss, 2, stri2);
	        fval2=my_atof(stri2);
			if (mesh)
			{
               // controllo che non ci sia gia' un lod con questa
			   // mesh e con le stesse distanze
               flag=-1;
		       for (j=0; j<s->p_Objects[i]->p_NumLods; j++)
		         if (s->p_Objects[i]->p_Lods[j].Mesh==mesh)
				 {
					 /*
					if ((s->p_Objects[i]->p_Lods[j].DistMin==fval) &&
                        (s->p_Objects[i]->p_Lods[j].DistMax==fval2))
					*/
						flag=j;
				 }
               // aggiungo il lod
	           if (flag==-1)
			   {
				  flag=s->p_Objects[i]->p_NumLods;
                  s->p_Objects[i]->p_Lods[flag].Mesh=mesh;
				  s->p_Objects[i]->p_NumLods++;
			   }
			   // sovrascrivo le distanze
               s->p_Objects[i]->p_Lods[flag].DistMin=fval;
               s->p_Objects[i]->p_Lods[flag].DistMax=fval2;
			}
		  }

		  ss=get_next_key(ado, ss);
		}
	  }
	}

	/*
    if (strcmp(stri ,"patch") == 0)
    {
      // entro in un patchobject
      get_section_value(adl, pos, stri); // leggo il nome della patch
      i=s->index_of_object(stri);
	  // ho trovato l'oggetto specificato dallo script
	  if ((i>=0) && (i<s->p_NumObjects))
	  {
        ss=get_subsection(ado, pos);
        ss=get_first_key(ado, ss);
        while(ss!=-1)
		{
          if (strcmp(get_key_type(ado, ss),"u_steps") == 0)
		  {
            get_key_value(ado, ss, 0, stri2);
	        fval=my_atof(stri2);
            patch_obj=(AD_PatchObject *)s->p_Objects[i];
            patch_obj->u_evaluations=fval;
		  }

          if (strcmp(get_key_type(ado, ss),"v_steps") == 0)
		  {
            stri=(get_key_value(ado, ss));
	        fval=my_atof(stri);
            patch_obj=(AD_PatchObject *)s->p_Objects[i];
            patch_obj->v_evaluations=fval;
		  }

          if (strcmp(get_key_type(ado, ss),"swap_normals") == 0)
		  {
            stri=(get_key_value(ado, ss));
            patch_obj=(AD_PatchObject *)s->p_Objects[i];
            if (strcmp(stri, "ON")==0)
				patch_obj->swap_normals=TRUE;
			else patch_obj->swap_normals=FALSE;
		  }

		  ss=get_next_key(ado, ss);
		}
	  }
    }
	*/
    pos=find_next_section(ado, pos);
  }

  delete [] ado;
  return 0;
}


// -----------------------------------------------------------
// procedure proprie per gli script dei particle systems e WSM
// -----------------------------------------------------------
int32 load_ADP (char8 *nfile, CScene3D *s)
{
  int32 pos, ss, i, ival;
  char8 stri[100], stri2[100];
  char8 *adp=alloc_script(nfile);
  float4 fval;
  CParticleSystem *psys;
  CWindModifier *wmod;
  CGravityModifier *gmod;


  pos=find_first_section(adp, 0);
  while (pos != -1)
  {
    get_section_type(adp, pos, stri);
    if (strcmp(stri, "particle")== 0)
    {
       // entro in un particle system
       get_section_value(adp, pos, stri);
       i=s->index_of_object(stri);
	   if ((i>=0) && (i<s->p_NumObjects))
	   {
         ss=get_subsection(adp, pos);
         ss=get_first_key(adp, ss);
         psys=(CParticleSystem *)s->p_Objects[i];

         while(ss != -1)
		 {
           get_key_type(adp, ss, stri);

           if (strcmp(stri, "emitter_width")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         fval=my_atof(stri2);
             psys->p_EmitterWidth=fval;
		   }

           if (strcmp(stri, "emitter_height")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         fval=my_atof(stri2);
             psys->p_EmitterHeight=fval;
		   }

           if (strcmp(stri, "faded_particles")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         fval=my_atof(stri2);
			 if ((strcmp(stri2, "OFF")==0) || (fval<=0.00001f))
                psys->p_FadedParticles=0;
			 else psys->p_FadedParticles=fval;
		   }

           if (strcmp(stri, "max_particles")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         ival=atoi(stri2);
             psys->p_MaxParticles=ival;
		   }

           if (strcmp(stri, "start_time")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         fval=my_atof(stri2);
             psys->p_StartTime=fval;
		   }

           if (strcmp(stri, "end_time")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         fval=my_atof(stri2);
             psys->p_EndTime=fval;
		   }

           if (strcmp(stri, "life")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         fval=my_atof(stri2);
             psys->p_Life=fval;
		   }

           if (strcmp(stri, "speed")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         fval=my_atof(stri2);
             psys->p_EmitterSpeed=fval;
		   }

           if (strcmp(stri, "variation")==0)
		   {
             get_key_value(adp, ss, 0, stri2);
	         fval=my_atof(stri2);
             psys->p_EmitterVariation=fval;
		   }

           if (strcmp(stri, "size_attenuation")==0)
		   {
             if (get_key_value(adp, ss, 0, stri2))
			 {
	           fval=my_atof(stri2);
               psys->p_Attenuation0=fval;
               if (get_key_value(adp, ss, 1, stri2))
			   {
	             fval=my_atof(stri2);
                 psys->p_Attenuation1=fval;
                 if (get_key_value(adp, ss, 2, stri2))
				 {
	               fval=my_atof(stri2);
                   psys->p_Attenuation2=fval;
                   if (get_key_value(adp, ss, 3, stri2))
				   {
	                 fval=my_atof(stri2);
                     psys->p_Attenuation3=fval;
				   }
				 }
			   }
			 }
		   }

           if (strcmp(stri, "texture")==0)
	       // leggo la texture di flare
		   {
		 	 get_key_value(adp, ss, 0, stri2);
			 if (strcmp(stri2, "NONE") != 0)
			 {
			    psys->p_Texture=new CGraphicSurface;
				if (psys->p_Texture->m_Load(stri2, RGBINTERLEAVED)==FILENOTFOUND)
		            debug_error(miawin, "ADP script loader error: texture(particle) file not found!");
			 }
		  }
		  ss=get_next_key(adp, ss);
		}
	  }
    }
    else
    
	// entro in un Wind Modifier
	if (strcmp(stri, "windWSM") == 0)
    {
      get_section_value(adp, pos, stri);
      i=s->index_of_object(stri);
	  if ((i>=0) && (i<s->p_NumObjects))
	  {
        ss=get_subsection(adp, pos);
        ss=get_first_key(adp, ss);
        wmod=(CWindModifier *)s->p_Objects[i];
        while (ss != -1)
		{
          get_key_type(adp, ss, stri);

          if (strcmp(stri, "type") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
            if (strcmp(stri2, "SPHERICAL")==0)
              wmod->p_Mapping=FORCE_SPHERICAL;
			else wmod->p_Mapping=FORCE_PLANAR;
		  }

          if (strcmp(stri, "strength") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
	        fval=my_atof(stri2);
            wmod->p_Strength=fval;
		  }
          if (strcmp(stri, "decay") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
	        fval=my_atof(stri2);
            wmod->p_Decay=fval;
		  }
          if (strcmp(stri, "turbolence") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
	        fval=my_atof(stri2);
            wmod->p_Turbolence=fval;
		  }
          if (strcmp(stri, "frequency") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
	        fval=my_atof(stri2);
            wmod->p_Frequency=fval;
		  }
          if (strcmp(stri, "scale") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
	        fval=my_atof(stri2);
            wmod->p_Scale=fval;
		  }
		  ss=get_next_key(adp, ss);
		}
	  }
	}
	else

    // entro in un Gravity Modifier
	if (strcmp(stri, "gravityWSM") == 0)
    {
      get_section_value(adp, pos, stri);
      i=s->index_of_object(stri);
	  if ((i>=0) && (i<s->p_NumObjects))
	  {
        ss=get_subsection(adp, pos);
        ss=get_first_key(adp, ss);
        gmod=(CGravityModifier *)s->p_Objects[i];
        while (ss != -1)
		{
          get_key_type(adp, ss, stri);

          if (strcmp(stri, "type") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
            if (strcmp(stri2, "SPHERICAL")==0)
              gmod->p_Mapping=FORCE_SPHERICAL;
			else gmod->p_Mapping=FORCE_PLANAR;
		  }

          if (strcmp(stri, "strength") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
	        fval=my_atof(stri2);
            gmod->p_Strength=fval;
		  }
          if (strcmp(stri, "decay") == 0)
		  {
            get_key_value(adp, ss, 0, stri2);
	        fval=my_atof(stri2);
            gmod->p_Decay=fval;
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