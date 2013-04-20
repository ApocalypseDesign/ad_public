// variabili globali usate per il rendering

#define WIN32_LEAN_AND_MEAN	
#define D3D_OVERLOADS // deve stare prima di #include <d3d.h>
#include "..\ADrender.h"
#include "rasters.h"
#include "setmat.h"
#include "render.h"

TEX1RGBVERTEX    *HW_texrgb;
TEX1RGBVERTEX    *HW_texrgb_arrays[2];
TEX2RGBVERTEX    *HW_texenv;
TEX2RGBVERTEX    *HW_texenv_arrays[2];
int              HW_numvert;

void set_material_procedures (material *m)
{
  int num_lights;

  // cosi' mi salvo sempre il culo almeno
  m->myraster=*tria_raster_NULL;
  m->myscanline=*setNULL;

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
        m->myraster=*tria_raster_NULL;
		m->myscanline=*setNULL;
		return;
	 }


	 if ((m->texture_ptr!=(texture *)NULL) &&
         (m->envmap_ptr!=(texture *)NULL))
	 // texture + env di base, no luci, chekko i vari metodi
	 {
		m->myraster=(*tria_raster_texenv);
		m->myscanline=*setTexEnv_mBLEND3DS;

		if (m->trasparencytype!=OFF) 
		{ 
          if (m->trasparencytype==MIXTYPE_BLEND50) // caso di trasparenza mediata
		    switch (m->mapsmixtype)
 		    {
		      case MIXTYPE_ADD: m->myscanline=*setTexEnv_mADD_tBLENDalpha;
				                m->trasparency=128;
				                return; break;
		      case MAPSMIXTYPE_TEXSUBENV: return; break;
		      case MAPSMIXTYPE_ENVSUBTEX: return; break;
		      case MIXTYPE_BLEND50: m->myscanline=*setTexEnv_mBLEND3DS_tBLENDalpha;
				                    m->trasparency=128;
				                    return; break;
		      case MIXTYPE_ALPHABLEND: return; break;		  		  
		    }
          else  // caso di blend alpha per la trasparenza
          if (m->trasparencytype==MIXTYPE_ALPHABLEND)
            switch (m->mapsmixtype)
 		    {
		      case MIXTYPE_ADD: m->myscanline=*setTexEnv_mADD_tBLENDalpha;
				                return; break;
		      case MAPSMIXTYPE_TEXSUBENV: return; break;
		      case MAPSMIXTYPE_ENVSUBTEX: return; break;
		      case MIXTYPE_BLEND50: m->myscanline=*setTexEnv_mBLEND3DS_tBLENDalpha;
				                    return; break;
		      case MIXTYPE_ALPHABLEND: return; break;		  		  
		    }
          else  // caso di blend additivo per la trasparenza
          if (m->trasparencytype==MIXTYPE_ADD)
		    switch (m->mapsmixtype)
 		    {
		      case MIXTYPE_ADD: m->myscanline=*setTexEnv_mADD_tADD;
				                return; break;
		      case MAPSMIXTYPE_TEXSUBENV: return; break;
		      case MAPSMIXTYPE_ENVSUBTEX: return; break;
		      case MIXTYPE_BLEND50: m->myscanline=*setTexEnv_mBLEND3DS_tADD;
				                    return; break;
		      case MIXTYPE_ALPHABLEND: return; break;		  		  
		    }
          else  // caso di mappa ALPHA
          if (m->trasparencytype==ALPHA_MAP)
            switch (m->mapsmixtype)
 		    {
		      case MIXTYPE_ADD: m->myscanline=*setTexEnv_mADD_tMAPalpha;
				                return; break;
		      case MAPSMIXTYPE_TEXSUBENV: return; break;
		      case MAPSMIXTYPE_ENVSUBTEX: return; break;
		      case MIXTYPE_BLEND50: m->myscanline=*setTexEnv_mBLEND3DS_tMAPalpha;
				                    return; break;
		      case MIXTYPE_ALPHABLEND: return; break;		  		  
		    }
		}
        else  // caso di non trasparenza e no luci (solo tex+env)
          switch (m->mapsmixtype)
		  {
		      case MIXTYPE_ADD: m->myscanline=*setTexEnv_mADD;
				                return; break;
		      case MAPSMIXTYPE_TEXSUBENV: m->myscanline=*setTexEnv_mTEXSUBENV;
				                          return; break;
		      case MAPSMIXTYPE_ENVSUBTEX: m->myscanline=*setTexEnv_mENVSUBTEX;
				                          return; break;
		      case MIXTYPE_BLEND50: m->myscanline=*setTexEnv_mBLEND3DS;
				                    return; break;
		      case MIXTYPE_ALPHABLEND: m->myscanline=*setTexEnv_mBLEND3DS;
				                       return; break;
		    }
       return;  // fine del caso tex + env no luci
	 }


     if ((m->texture_ptr!=(texture *)NULL))
	 // texture di base, no luci, chekko i vari metodi
     {
		m->myraster=(*tria_raster_texture);
		m->myscanline=*setTex;        

	    if (m->trasparencytype!=OFF)
		{
		  if (m->trasparencytype==MIXTYPE_BLEND50)
		  {
		     m->trasparency=127;
		     m->myscanline=*setTex_tBLENDalpha;
		  }
		  else
		  if (m->trasparencytype==MIXTYPE_ADD)
		  {
		     m->myscanline=*setTex_tADD;
		  }
		  else
		  if (m->trasparencytype==MIXTYPE_ALPHABLEND)
		      m->myscanline=*setTex_tBLENDalpha;
          else
		  if (m->trasparencytype==ALPHA_MAP)
		      m->myscanline=*setTex_tMAPalpha;
		}
	    return; // fine del caso solo texture no luci
	 }


     if ((m->envmap_ptr!=(texture *)NULL))
	 // envmap di base, no luci, chekko i vari metodi
     {
		m->myraster=(*tria_raster_envmap);
        m->myscanline=*setEnv;

	    if (m->trasparencytype!=OFF)
		{
		  if (m->trasparencytype==MIXTYPE_BLEND50)
		  {
		    m->trasparency=127;
		    m->myscanline=*setEnv_tBLENDalpha;
		  }
		  else
		  if (m->trasparencytype==MIXTYPE_ADD)
		  {
		    m->myscanline=*setEnv_tADD;
		  }
		  else
		  if (m->trasparencytype==MIXTYPE_ALPHABLEND)
		      m->myscanline=*setEnv_tBLENDalpha;
		  else
		  if (m->trasparencytype==ALPHA_MAP)
		      m->myscanline=*setEnv_tMAPalpha;
		}
	 return; // fine del caso envmap no luci
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
		m->myscanline=*setRGB;
		
	    if (m->trasparencytype!=OFF)
		{
		  switch (m->trasparencytype)
		  {
		    case MIXTYPE_BLEND50: break;
			case MIXTYPE_ALPHABLEND: break;
			case ALPHA_MAP: break;
			case MIXTYPE_ADD: 
		                      m->myscanline=*setRGB_tADD;
							  break;
		  }
		}
		else if (m->lightmixtype==LIGHTMIXTYPE_BLEND3DS)
			 m->myscanline=*setRGB_lBLEND3DS;
		return;
	 }


	 if ((m->texture_ptr!=(texture *)NULL) &&
         (m->envmap_ptr!=(texture *)NULL))
	 // texture+env di base, si luci, chekko i vari metodi
	 {
		m->myraster=(*tria_raster_texenv_RGB);
        m->myscanline=*setTexEnv_mBLEND3DS;

	    switch (m->mapsmixtype)
		{
          case MIXTYPE_ADD :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=*setTexEnv_mADD_lADD;
						                          break;
                               case LIGHTMIXTYPE_SUB : m->myscanline=*setTexEnv_mADD_lSUB;
						                               break;
                               case MIXTYPE_BLEND50 : m->myscanline=*setTexEnv_mADD_lBLEND3DS;
								                      break;
                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setTexEnv_mADD_lBLEND3DS;
						                                    break;
							 }
                             return;
							 break;

          case MAPSMIXTYPE_TEXSUBENV :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : break;
                               case LIGHTMIXTYPE_SUB : break;
                               case MIXTYPE_BLEND50 : break;
                               case LIGHTMIXTYPE_BLEND3DS : break;
							 }
                             return;
							 break;

          case MAPSMIXTYPE_ENVSUBTEX :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : break;
                               case LIGHTMIXTYPE_SUB : break;
                               case MIXTYPE_BLEND50 :  break;
                               case LIGHTMIXTYPE_BLEND3DS : break;
							 }
                             return;
							 break;

          case MIXTYPE_BLEND50 :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=*setTexEnv_mBLEND3DS_lADD; break;
                               case LIGHTMIXTYPE_SUB : m->myscanline=*setTexEnv_mBLEND3DS_lSUB; break;
                               case MIXTYPE_BLEND50 :  m->myscanline=*setTexEnv_mBLEND3DS_lBLEND3DS; break;
                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setTexEnv_mBLEND3DS_lBLEND3DS; break;
							 }
                             return;
							 break;

          case MIXTYPE_ALPHABLEND :
		                     switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : break;
                               case LIGHTMIXTYPE_SUB : break;
                               case MIXTYPE_BLEND50 : break;
                               case LIGHTMIXTYPE_BLEND3DS : break;
							 }
                             return;
							 break;
		}
		return; // fine caso tex+env+luci
     }


     if ((m->texture_ptr!=(texture *)NULL))
	 // texture di base, si luci, chekko i vari metodi
     {
		m->myraster=(*tria_raster_texRGB);
        m->myscanline=*setTex_lBLEND3DS;

		if (m->trasparencytype!=OFF)
		{
		  if (m->trasparencytype==MIXTYPE_BLEND50)
          // caso di trasparenza con blend di tipo BLEND50
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : 
                                                  m->trasparency=128;
												  m->myscanline=*setTex_lADD_tBLENDalpha;
								                  return; break;
                               case LIGHTMIXTYPE_SUB : m->trasparency=128;
												       m->myscanline=*setTex_lSUB_tBLENDalpha;
								                       return; break;
                               case MIXTYPE_BLEND50 : m->trasparency=128;
													  m->myscanline=*setTex_lBLEND3DS_tBLENDalpha;
								                      return;
						                              break;
                               case LIGHTMIXTYPE_BLEND3DS : m->trasparency=128;
															m->myscanline=*setTex_lBLEND3DS_tBLENDalpha;
								                            return;	break;
							 }
		  else
		  if (m->trasparencytype==MIXTYPE_ADD)
          // caso di trasparenza con blend di tipo ADD
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=*setTex_lADD_tADD;
								                  return; break;
                               case LIGHTMIXTYPE_SUB : m->myscanline=*setTex_lSUB_tADD;
								                       return; break;
                               case MIXTYPE_BLEND50 : m->myscanline=*setTex_lBLEND3DS_tADD;
								                      return; break;
                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setTex_lBLEND3DS_tADD;
								                            return; break;
							 }
		  else
		  if (m->trasparencytype==ALPHA_MAP)
		  			  // caso di trasparenza con blend con mappa alpha
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=*setTex_lADD_tMAPalpha;
								                  return; break;
                               case LIGHTMIXTYPE_SUB : m->myscanline=*setTex_lSUB_tMAPalpha;
								                       return; break;
                               case MIXTYPE_BLEND50 : m->myscanline=*setTex_lBLEND3DS_tMAPalpha;
								                      return; break;
                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setTex_lBLEND3DS_tMAPalpha;
								                            return; break;
							 }
		  else
		  if (m->trasparencytype==MIXTYPE_ALPHABLEND)
			  // caso di trasparenza con blend di tipo alpha
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=*setTex_lADD_tBLENDalpha;
								                  return; break;
                               case LIGHTMIXTYPE_SUB : m->myscanline=*setTex_lSUB_tBLENDalpha;
								                       return; break;
                               case MIXTYPE_BLEND50 : m->myscanline=*setTex_lBLEND3DS_tBLENDalpha;
								                      return; break;
                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setTex_lBLEND3DS_tBLENDalpha;
								                            return; break;
							 }
          return;  // fine caso tex+luci+trasparenza
		}
        else  // caso tex+luci no trasparenza
		{
   		  switch (m->lightmixtype)
		  {
			case MIXTYPE_ADD : m->myscanline=*setTex_lADD;
			                   return; break;
            case LIGHTMIXTYPE_SUB : m->myscanline=*setTex_lSUB;
				                    return; break;
            case MIXTYPE_BLEND50 : m->myscanline=*setTex_lBLEND3DS;
								   return; break;
            case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setTex_lBLEND3DS;
								         return; break;
		  }
		}
	 }


     if ((m->envmap_ptr!=(texture *)NULL))
	 // envmap di base, si luci, chekko i vari metodi
     {
		m->myraster=(*tria_raster_envRGB);
        m->myscanline=*setEnv_lBLEND3DS;

		if (m->trasparencytype!=OFF)
		{
		  if (m->trasparencytype==MIXTYPE_BLEND50)
          // caso di trasparenza con blend di tipo BLEND50
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->trasparency=128;
								                  m->myscanline=*setEnv_lADD_tBLENDalpha;
								                  return; break;
                               case LIGHTMIXTYPE_SUB : m->trasparency=128;
								                       m->myscanline=*setEnv_lSUB_tBLENDalpha;
								                       return; break;
                               case MIXTYPE_BLEND50 : m->trasparency=128;
								                      m->myscanline=*setEnv_lBLEND3DS_tBLENDalpha;
								                      return; break;
                               case LIGHTMIXTYPE_BLEND3DS : m->trasparency=128;
								                            m->myscanline=*setEnv_lBLEND3DS_tBLENDalpha;
								                            return; break;
							 }
		  else
		  if (m->trasparency==MIXTYPE_ADD)
          // caso di trasparenza con blend di tipo ADD
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=*setEnv_lADD_tADD;
								                  return; break;
                               case LIGHTMIXTYPE_SUB : m->myscanline=*setEnv_lSUB_tADD;
								                       return; break;
                               case MIXTYPE_BLEND50 : m->myscanline=*setEnv_lBLEND3DS_tADD;
								                      return; break;
                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setEnv_lBLEND3DS_tADD;
								                            return; break;
							 }
          else
		  if (m->trasparencytype==ALPHA_MAP)
	      // caso di trasparenza con blend con mappa alpha
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=*setEnv_lADD_tMAPalpha;
								                  return; break;
                               case LIGHTMIXTYPE_SUB :m->myscanline=*setEnv_lSUB_tMAPalpha;
								                      return; break;
                               case MIXTYPE_BLEND50 : m->myscanline=*setEnv_lBLEND3DS_tMAPalpha;
								                      return; break;
                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setEnv_lBLEND3DS_tMAPalpha;
								                            return; break;
							 }
		  else
		  if (m->trasparencytype==MIXTYPE_ALPHABLEND)
	      // caso di trasparenza con blend di tipo alpha
                             switch (m->lightmixtype)
							 {
							   case MIXTYPE_ADD : m->myscanline=*setEnv_lADD_tBLENDalpha;
								                  return; break;
                               case LIGHTMIXTYPE_SUB :m->myscanline=*setEnv_lSUB_tBLENDalpha;
								                      return; break;
                               case MIXTYPE_BLEND50 : m->myscanline=*setEnv_lBLEND3DS_tBLENDalpha;
								                      return; break;
                               case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setEnv_lBLEND3DS_tBLENDalpha;
								                            return; break;
							 }
          return; // fine caso env+luci+trasparenza
		}
        else  // caso env+luci no trasparenza
   		  switch (m->lightmixtype)
		  {
			case MIXTYPE_ADD : m->myscanline=*setEnv_lADD;
			                   return; break;
            case LIGHTMIXTYPE_SUB : m->myscanline=*setEnv_lSUB;
								    return; break;
            case MIXTYPE_BLEND50 : m->myscanline=*setEnv_lBLEND3DS;
								   return; break;
            case LIGHTMIXTYPE_BLEND3DS : m->myscanline=*setEnv_lBLEND3DS;
								         return; break;
		  }
	 }
   }
}