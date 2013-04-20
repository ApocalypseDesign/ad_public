#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include "..\utilsscript.h"
#include "rayt_utilsscript.h"
#include "..\..\utility.h"
#include "..\..\Math\fastmath.h"

extern HWND miawin;         // finestra principale; usata qui per le debug_error


// ----------------------------------------------
// procedure proprie per gli script dei materiali
// ----------------------------------------------

int rayt_load_ADM (char *nfile, AD_RaytScene *s)
// carica un file .ADM
{
  int pos, ss, nmat, ntex, i, j, tval;
  char *stri;
  char *adm=alloc_script(nfile);
  float fv;


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
  s->materials=new rayt_material[nmat];
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
      if (strcmp(get_key_type(adm,ss),"diffuse_r") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].diffuse_color.x=(float)tval;
	  }
      if (strcmp(get_key_type(adm,ss),"diffuse_g") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].diffuse_color.y=(float)tval;
	  }
      if (strcmp(get_key_type(adm,ss),"diffuse_b") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].diffuse_color.z=(float)tval;
	  }
      if (strcmp(get_key_type(adm,ss),"diffuse_kd") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        fv=my_atof(stri);
	    s->materials[s->num_materials].kdr=fv;
	  }


      if (strcmp(get_key_type(adm,ss),"specular_r") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].specular_color.x=(float)tval;
	  }
      if (strcmp(get_key_type(adm,ss),"specular_g") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].specular_color.y=(float)tval;
	  }
      if (strcmp(get_key_type(adm,ss),"specular_b") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].specular_color.z=(float)tval;
	  }
      if (strcmp(get_key_type(adm,ss),"specular_ks") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        fv=my_atof(stri);
	    s->materials[s->num_materials].ksr=fv;
	  }

      if (strcmp(get_key_type(adm,ss),"glossiness") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        fv=my_atof(stri);
	    s->materials[s->num_materials].glossiness=fv;
	  }

      if (strcmp(get_key_type(adm,ss),"refraction_index") == 0)
	  {
        stri=strupr(get_key_value(adm,ss));
        fv=my_atof(stri);
	    s->materials[s->num_materials].refraction_index=fv;
	  }

      if (strcmp(get_key_type(adm,ss),"trasparency") == 0)
	  // leggo il fattore di trasparenza
	  {
        stri=strupr(get_key_value(adm,ss));
        tval=atoi(stri);
	    if (tval<0) tval=0; if (tval>255) tval=255;
	    s->materials[s->num_materials].transparency=(float)tval;
        s->materials[s->num_materials].flags |= IS_TRASPARENT;
	  }

      if (strcmp(get_key_type(adm,ss),"texture") == 0)
      {
        // sono in una texture
        stri=(get_key_value(adm,ss));
        i=0; j=-1;
        while (i<s->num_textures) { if (strcmp(s->textures[i].name,stri)==0) j=i; i++; }
        if (j>=0) {
                    s->materials[s->num_materials].texture_ptr=&s->textures[j];
		}                      
	    else {
               if (strcmp(get_file_ext(stri),"PCX")==0)
                 if (s->textures[s->num_textures].load_pcx(stri, RGBINTERLEAVED)==FILENOTFOUND)
				   debug_error(miawin, "PCX loader error: file not found!");
                 else AddUpdate_Texture(&s->textures[s->num_textures]);

               if (strcmp(get_file_ext(stri),"JPG")==0)
                 if(s->textures[s->num_textures].load_jpg(stri, RGBINTERLEAVED)==FILENOTFOUND)
			         debug_error(miawin, "JPG loader error: file not found!");
                 else AddUpdate_Texture(&s->textures[s->num_textures]);

               s->materials[s->num_materials].texture_ptr=&s->textures[s->num_textures];
               s->num_textures++;
		}
		s->materials[s->num_materials].flags |= PAINT_TEXTURE;
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
int rayt_load_ADL (char *nfile, AD_RaytScene *s)
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

            if (strcmp(get_file_ext(stri),"JPG")==0)
			{
		      s->omni[i].flare=new texture;
              if (s->omni[i].flare->load_jpg(stri, RGBINTERLEAVED)==FILENOTFOUND)
				debug_error(miawin, "JPG loader error: file not found!");
			  else AddUpdate_Texture(s->omni[i].flare);
			}

			if (strcmp(get_file_ext(stri),"PCX")==0)
			{
		      s->omni[i].flare=new texture;
              if (s->omni[i].flare->load_pcx(stri, RGBINTERLEAVED)==FILENOTFOUND)
				debug_error(miawin, "PCX loader error: file not found!");
			  else AddUpdate_Texture(s->omni[i].flare);
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
