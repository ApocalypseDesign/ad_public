// variabili globali usate per il rendering
#include "..\ADrender.h"
#include "rasters.h"
#include "scanline.h"

// Constant slopes
int du, dv, envdu, envdv, dR, dG, dB;
float dz;


// Edge slopes
int left_dR, left_dG, left_dB;
int left_du, left_dv, left_envdu, left_envdv;
int left_dx, right_dx;
float left_dz;


// Valori correnti durante lo scorrere degli edge
int left_x, right_x;
int left_u, left_v, left_envu, left_envv;
int left_R, left_G, left_B;
int color_flat;
float left_z;


// Dati relativi alle texture, envmap e materiale corrente
// la variabile _umask_ necessita dei _ perche'
// il vc ha gia' una sua definizione di questa variabile
unsigned int *texmap32, *envmap32;
unsigned char *texmap8, *envmap8;
int vmask, vshift, _umask_, envvshift;


// Dati relativi al buffer video
unsigned int *uvideo32;
unsigned char *uvideo8;
int scrX, scrY, *muly;
int offset0;


// Dati relativi allo zbuffer e suo uso
float *zbuf;
float trick, trick_step;

// Valori correnti nel disegno di una scanline
float d;
int fi, fi2;
int l, offset, u, v, envu, envv;
int RR1, GG1, BB1;
int xx1, xx2;
int env_offset, tex_offset;


// Tabelle di mixing usate dalle scanline
unsigned char *ttable;  // tabella di tarsparenza corrente
unsigned char *atable;  // tabella di alphamixing corrente
unsigned char add_table[512], sub_table[512];


// Valori usati nel ciclo di disegno di un triangolo
ScanlineProc current_scanline_proc;
int current_flags;
int j, paint_second_part;
int top_y, bottom_y;
float scan, temp_scan;


void init_render (imagebuffer *videobuf, floatbuffer *zb)
{
  int t;

  zbuf=zb->where;
  trick=0;
  trick_step=0;  // da settare con la SW_Zbuffer_SetMinZ
  scrX=videobuf->width;
  scrY=videobuf->height;
  muly=videobuf->muly;
  uvideo32=videobuf->uint32ptr;
  uvideo8=videobuf->uchar8ptr;

  // inizializzazione delle tabelle di add e sub saturati
  for (t=0; t<512; t++)
  {
	if (t<256) add_table[t]=t;
	else add_table[t]=255;

	// per la sub table si deve mappare -255..255 in 0..512
	// infatti bisogna riferirsi ad essa con sub_table[a-b+255]
	if (t<255) sub_table[t]=0;
	else sub_table[t]=t-255;
  }
}


void set_material_procedures (material *m)
{
  int num_lights;

  m->myraster=*tria_raster_NULL;
  if (m->flags & PAINT_LIGHT) num_lights=1;
  else num_lights=0;

// ########################################################
// #############   in caso di nessuna luce   ##############
// ########################################################
   if ((num_lights<=0) || (m->lightmixtype==LIGHTMIXTYPE_NONE))
   {
	 if ((m->texture_ptr==(texture *)NULL) &&
         (m->envmap_ptr==(texture *)NULL))
	 {
        m->myraster=(*tria_raster_flat_slow);
		m->myscanline=(*scanline_flat);
		return;
	 }


	 if ((m->texture_ptr!=(texture *)NULL) &&
         (m->envmap_ptr!=(texture *)NULL))
	 // texture+env di base, no luci, chekko i vari metodi
	 {
		m->myraster=(*tria_raster_texenv_slow);

		if (m->trasparencytype!=OFF) 
		{ 
          if (m->trasparencytype==MIXTYPE_BLEND50) // caso di trasparenza mediata
		    switch (m->mapsmixtype)
 		    {
		      case MIXTYPE_ADD: m->myscanline=(*scanline_texenv_mADD_tBLEND50);
				                return;
						        break;

		      case MAPSMIXTYPE_TEXSUBENV: m->myscanline=(*scanline_texenv_mTEXSUBENV_tBLEND50);
				                          return;
						                  break;

		      case MAPSMIXTYPE_ENVSUBTEX: m->myscanline=(*scanline_texenv_mENVSUBTEX_tBLEND50);
				                          return;
						                  break;

		      case MIXTYPE_BLEND50: m->myscanline=(*scanline_texenv_mBLEND50_tBLEND50);
				                    return;
						            break;

		      case MIXTYPE_ALPHABLEND: m->myscanline=(*scanline_texenv_mBLENDalpha_tBLEND50);
				                       return;
					                   break;		  		  
		    }
          else  // caso di blend alpha per la trasparenza
            switch (m->mapsmixtype)
 		    {
		      case MIXTYPE_ADD: m->myscanline=(*scanline_texenv_mADD_tBLENDalpha);
				                return;
						        break;

		      case MAPSMIXTYPE_TEXSUBENV: m->myscanline=(*scanline_texenv_mTEXSUBENV_tBLENDalpha);
				                          return;
						                  break;

		      case MAPSMIXTYPE_ENVSUBTEX: m->myscanline=(*scanline_texenv_mENVSUBTEX_tBLENDalpha);
				                          return;
						                  break;

		      case MIXTYPE_BLEND50: m->myscanline=(*scanline_texenv_mBLEND50_tBLENDalpha);
				                    return;
						            break;

		      case MIXTYPE_ALPHABLEND: m->myscanline=(*scanline_texenv_mBLENDalpha_tBLENDalpha);
				                       return;
						               break;		  		  
		    }
		}
        else  // caso di non trasparenza e no luci
          switch (m->mapsmixtype)
		  {
		      case MIXTYPE_ADD: m->myscanline=(*scanline_texenv_mADD);
				                return;
						        break;

		      case MAPSMIXTYPE_TEXSUBENV: m->myscanline=(*scanline_texenv_mTEXSUBENV);
				                          return;
						                  break;

		      case MAPSMIXTYPE_ENVSUBTEX: m->myscanline=(*scanline_texenv_mENVSUBTEX);
				                          return;
						                  break;

		      case MIXTYPE_BLEND50: m->myscanline=(*scanline_texenv_mBLEND50);
                                    m->myraster=(*tria_raster_texenv_mBLEND50);
				                    return;
						            break;

		      case MIXTYPE_ALPHABLEND: m->myscanline=(*scanline_texenv_mBLENDalpha);
				                       return;
						               break;		  		  
		    }

       return;  // fine del caso tex+env no luci
	 }


     if ((m->texture_ptr!=(texture *)NULL))
	 // texture di base, no luci, chekko i vari metodi
     {
		m->myraster=(*tria_raster_texture);
        m->myscanline=(*scanline_texture);

	    if (m->trasparencytype!=OFF)
		{
		  // setto alphablend cosi' sto sicuro
          m->myscanline=(*scanline_texture_tBLENDalpha);
          m->myraster=(*tria_raster_texture_tBLENDalpha);
		  switch (m->trasparencytype)
		  {
		    case MIXTYPE_BLEND50:
			                      m->myscanline=(*scanline_texture_tBLEND50);
                                  m->myraster=(*tria_raster_texture_tBLEND50);
								  break;
			case MIXTYPE_ADD:
				                  m->myscanline=(*scanline_texture_tADD);
                                  m->myraster=(*tria_raster_texture_tADD);
								  break;
		  }
		}
		// fine del caso solo texture no luci
		return;
	 }


     if ((m->envmap_ptr!=(texture *)NULL))
	 // envmap di base, no luci, chekko i vari metodi
     {
		m->myraster=(*tria_raster_envmap);
        m->myscanline=(*scanline_envmap);

	    if (m->trasparencytype!=OFF)
		{
		  switch (m->trasparencytype)
		  {
            m->myscanline=(*scanline_texture_tBLENDalpha);
            m->myraster=(*tria_raster_envmap_tBLENDalpha);
		    case MIXTYPE_BLEND50:
			                      m->myscanline=(*scanline_envmap_tBLEND50);
                                  m->myraster=(*tria_raster_envmap_tBLEND50);
								  break;
			case MIXTYPE_ADD:
				                  m->myscanline=(*scanline_envmap_tADD);
                                  m->myraster=(*tria_raster_envmap_tADD);
								  break;
		  }
		}
	 // fine del caso envmap no luci
	 return;
	 }
   }


// ########################################################
// ###############  caso di una o piu' luci ###############
// ########################################################

   else
   {
	 if ((m->texture_ptr==(texture *)NULL) &&
         (m->envmap_ptr==(texture *)NULL))
	 {
        m->myraster=(*tria_raster_RGB);
		m->myscanline=(*scanline_RGB);
	    if (m->trasparencytype!=OFF)
		{
          m->myscanline=(*scanline_RGB_tBLENDalpha);
          m->myraster=(*tria_raster_RGB_slow);
		  switch (m->trasparencytype)
		  {
		    case MIXTYPE_BLEND50:
			                      m->myscanline=(*scanline_RGB_tBLEND50);
                                  m->myraster=(*tria_raster_RGB_slow);
								  break;
			case MIXTYPE_ADD:
				                  m->myscanline=(*scanline_RGB_tBLEND50);
                                  m->myraster=(*tria_raster_RGB_slow);
								  break;
		  }
		}
		return;
	 }


	 if ((m->texture_ptr!=(texture *)NULL) &&
         (m->envmap_ptr!=(texture *)NULL))
	 // texture+env di base, si luci, chekko i vari metodi
	 // NB: purtroppo non si puo' avere tex+env+luci+trasparenza
	 // quindi verra' usato tex+env+luci
	 {
		m->myraster=(*tria_raster_texenv_RGB_slow);

	    switch (m->mapsmixtype)
		{
          case MIXTYPE_ADD :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_texenv_mADD_lADD);
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_texenv_mADD_lSUB);
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_texenv_mADD_lBLEND50);
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_texenv_mADD_lBLEND3DS);
						                                    break;
							 }
                             return;
							 break;



          case MAPSMIXTYPE_TEXSUBENV :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_texenv_mTEXSUBENV_lADD);
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_texenv_mTEXSUBENV_lSUB);
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_texenv_mTEXSUBENV_lBLEND50);
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_texenv_mTEXSUBENV_lBLEND3DS);
						                                    break;
							 }
                             return;
							 break;
							 

          case MAPSMIXTYPE_ENVSUBTEX :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_texenv_mENVSUBTEX_lADD);
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_texenv_mENVSUBTEX_lSUB);
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_texenv_mENVSUBTEX_lBLEND50);
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_texenv_mENVSUBTEX_lBLEND3DS);
						                                    break;
							 }
                             return;
							 break;
							 

          case MIXTYPE_BLEND50 :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_texenv_mBLEND50_lADD);
                                                  m->myraster=(*tria_raster_texenv_mBLEND50_lADD);
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_texenv_mBLEND50_lSUB);
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_texenv_mBLEND50_lBLEND50);
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_texenv_mBLEND50_lBLEND3DS);
						                                    break;
							 }
                             return;
							 break;


          case MIXTYPE_ALPHABLEND :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_texenv_mBLENDalpha_lADD);
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_texenv_mBLENDalpha_lSUB);
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_texenv_mBLENDalpha_lBLEND50);
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_texenv_mBLENDalpha_lBLEND3DS);
						                                    break;
							 }
                             return;
							 break;

		}

		return; // fine caso tex+env+luci
     }


     if ((m->texture_ptr!=(texture *)NULL))
	 // texture di base, si luci, chekko i vari metodi
     {
		m->myraster=(*tria_raster_texRGB_slow);

		if (m->trasparencytype!=OFF)
		{
		  if (m->trasparencytype==MIXTYPE_BLEND50)
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_texture_tBLEND50_lADD);
								                  return;
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_texture_tBLEND50_lSUB);
								                       return;
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_texture_tBLEND50_lBLEND50);
								                      return;
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_texture_tBLEND50_lBLEND3DS);
								                            return;
						                                    break;
							 }
		  else // caso di trasparenza con blend di tipo alpha
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_texture_tBLENDalpha_lADD);
								                  return;
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_texture_tBLENDalpha_lSUB);
								                       return;
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_texture_tBLENDalpha_lBLEND50);
								                      return;
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_texture_tBLENDalpha_lBLEND3DS);
								                            return;
						                                    break;
							 }
          return;  // fine caso tex+luci+trasparenza
		}
        else  // caso tex+luci no trasparenza
		{
   		  switch (m->lightmixtype)
		  {
			case MIXTYPE_ADD : m->myscanline=(*scanline_texture_lADD);
                               m->myraster=(*tria_raster_texture_lADD);
			                   return;
						       break;

            case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_texture_lSUB);
								    return;
						            break;

            case MIXTYPE_BLEND50 : m->myscanline=(*scanline_texture_lBLEND50);
                                   m->myraster=(*tria_raster_texture_lBLEND50);
								   return;
						           break;

            case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_texture_lBLEND3DS);
								         return;
						                 break;
		  }
		}
	 }


     if ((m->envmap_ptr!=(texture *)NULL))
	 // envmap di base, si luci, chekko i vari metodi
     {
		m->myraster=(*tria_raster_envRGB_slow);

		if (m->trasparencytype!=OFF)
		{
		  if (m->trasparencytype==MIXTYPE_BLEND50)
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_envmap_tBLEND50_lADD);
								                  return;
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_envmap_tBLEND50_lSUB);
								                       return;
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_envmap_tBLEND50_lBLEND50);
								                      return;
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_envmap_tBLEND50_lBLEND3DS);
								                            return;
						                                    break;
							 }
		  else // caso di trasparenza con blend di tipo alpha
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=(*scanline_envmap_tBLENDalpha_lADD);
								                  return;
						                          break;

                               case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_envmap_tBLENDalpha_lSUB);
								                       return;
						                               break;

                               case MIXTYPE_BLEND50 : m->myscanline=(*scanline_envmap_tBLENDalpha_lBLEND50);
								                      return;
						                              break;

                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_envmap_tBLENDalpha_lBLEND3DS);
								                            return;
						                                    break;
							 }
          return; // fine caso env+luci+trasparenza
		}
        else  // caso env+luci no trasparenza
   		  switch (m->lightmixtype)
		  {
			case MIXTYPE_ADD : m->myscanline=(*scanline_envmap_lADD);
                               m->myraster=(*tria_raster_envmap_lADD);
			                   return;
						       break;

            case LIGHTMIXTYPE_SUB : m->myscanline=(*scanline_envmap_lSUB);
								    return;
						            break;

            case MIXTYPE_BLEND50 : m->myscanline=(*scanline_envmap_lBLEND50);
                                   m->myraster=(*tria_raster_envmap_lBLEND50);
								   return;
						           break;

            case LIGHTMIXTYPE_BLEND3DS : m->myscanline=(*scanline_envmap_lBLEND3DS);
								         return;
						                 break;
		  }
	 }
   }
}