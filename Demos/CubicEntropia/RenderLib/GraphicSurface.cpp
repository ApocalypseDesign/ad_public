#include "RenderLib.h"
#include "VideoReader.h"

extern FILE *fdebug;

CGraphicSurface::CGraphicSurface()
{
   p_HWSurfaces=(IDirect3DTexture8 **)NULL;
   p_OptWidth=0;
   p_OptHeight=0;
   p_Pixels=(void *)NULL;
   p_Width=-1;
   p_Height=-1;
   p_MipMapLevels=1;
   p_ChannelSize=-1;
   p_ChannelSize2x=-1;
   p_XShift=-1;
   p_YShift=-1;
   p_MulY=(int32 *)NULL;
   strcpy(p_Name, "");
   p_Type=RGBINTERLEAVED;
   p_Palette=(char8 *)NULL;
   p_ScanLines=(void *)NULL;
   p_NumTextures=0;
   p_CurrentTextureIndex=0;
   p_StartFrame=0;
   p_PlayBackRate=1.0f;
   p_PlayDirection=1;
   p_EndCondition=ENDCONDITION_LOOP;
}

//--------------------------------------------------------------------

CGraphicSurface::CGraphicSurface(char8 *fileName, int32 *err)
{
  if (!fileName)
  {
	 if (err) *err=0;
     return;
  }
  if (err)
    *err=m_Load(fileName, RGBINTERLEAVED);
  else m_Load(fileName, RGBINTERLEAVED);
}

//--------------------------------------------------------------------

CGraphicSurface::CGraphicSurface(int32 Width, int32 Height, int32 Bpp)
{
   if (Width<=0) return;
   if (Height<=0) return;
   if ((Bpp<=0) || (Bpp>32)) return;

   uchar8 *l_pixels;

   p_HWSurfaces=(IDirect3DTexture8 **)NULL;
   p_OptWidth=0;
   p_OptHeight=0;
   p_Width=Width;
   p_Height=Height;
   p_ChannelSize=Width*Height;
   p_ChannelSize2x=p_ChannelSize*2;
   l_pixels=new uchar8[p_ChannelSize*(Bpp/8)];
   p_Pixels=(void *)l_pixels;
   p_XShift=m_Int2Bits(Width);
   p_YShift=m_Int2Bits(Height);
   //p_MulY=;
   strcpy(p_Name, "Costruttore2");
   if (Bpp>8)
      p_Type=RGBINTERLEAVED;
   else
      p_Type=GREYSCALE;
   p_Palette=(char8 *)NULL;
   p_ScanLines=(void *)NULL;
   p_NumTextures=1;
   p_HWSurfaces=new LPDIRECT3DTEXTURE8;
   p_HWSurfaces[0]=(IDirect3DTexture8 *)NULL;
   p_CurrentTextureIndex=0;
   p_StartFrame=0;
   p_PlayBackRate=1.0f;
   p_EndCondition=ENDCONDITION_LOOP;
   p_MipMapLevels=1;
}

//--------------------------------------------------------------------

void CGraphicSurface::m_Clear(int32 A, int32 R, int32 G, int32 B)
{
   int32 i, ARGB, *pixels;

   if (p_Pixels)
   {
      A&=255;
      R&=255;
      G&=255;
      B&=255;
      ARGB=(A<<24) | (R<<16) | (G<<8) | B;
	  pixels=(int32 *)p_Pixels;
	  for (i=0; i<p_ChannelSize; i++)
		 pixels[i]=ARGB;
   }
}

//--------------------------------------------------------------------

void CGraphicSurface::m_Clear(int32 ARGB)
{
   int32 i, *pixels;

   if (p_Pixels)
   {
	  pixels=(int32 *)p_Pixels;
	  for (i=0; i<p_ChannelSize; i++)
		 pixels[i]=ARGB;
   }
}

//--------------------------------------------------------------------

int32 CGraphicSurface::m_LoadJPG (char8 *namefile, int32 loadmethod)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *infile;
  JSAMPARRAY buffer;
  int32 row_stride;
  uint32 offset, j;
  uchar8 *map;
  float4 fR, fG, fB;

  if ((infile = fopen(namefile, "rb")) == NULL)
	 return(FILENOTFOUND);
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  strcpy(p_Name, namefile);

  if ((cinfo.num_components==1) &&
	  (cinfo.out_color_space==JCS_GRAYSCALE))
  {
    p_Type=GREYSCALE;
	map=new uchar8[cinfo.output_width*cinfo.output_height];
  }
  else
  if (loadmethod==RGBINTERLEAVED)
  {
    p_Type=RGBINTERLEAVED;
	map=new uchar8[cinfo.output_width*cinfo.output_height*4];
  }
  else if (loadmethod==RGBPLANAR)
  {
    p_Type=RGBPLANAR;
	map=new uchar8[cinfo.output_width*cinfo.output_height*4];
  }

  p_Pixels=(void *)map;
  p_NumTextures=1;

  p_Width=cinfo.output_width;
  p_XShift=m_Int2Bits(p_Width);
  p_Height=cinfo.output_height;
  p_YShift=m_Int2Bits(p_Height);
  p_ChannelSize=p_Width*p_Height;
  p_ChannelSize2x=p_ChannelSize*2;

  p_MulY=new int32[p_Height];
  for (j=0; j<p_Height; j++) p_MulY[j]=j*p_Width;

  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
	       ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  if (p_Type & GREYSCALE)
  {
    offset=0;
    while (cinfo.output_scanline < cinfo.output_height)
	{
      jpeg_read_scanlines(&cinfo, buffer, 1);
	  for (j=0; j<cinfo.output_width; j++)
         map[offset++]=buffer[0][j];
	}
  }
  else
  if (p_Type & RGBINTERLEAVED)
  {
    offset=0;
    while (cinfo.output_scanline < cinfo.output_height)
	{
      jpeg_read_scanlines(&cinfo, buffer, 1);
	  for (j=0; j<cinfo.output_width; j++)
	  {
        map[offset++]=buffer[0][j*3+2];
		fR=(float4)buffer[0][j*3+2];
        map[offset++]=buffer[0][j*3+1];
		fG=(float4)buffer[0][j*3+1];
        map[offset++]=buffer[0][j*3+0];
		fB=(float4)buffer[0][j*3+0];
		map[offset++]=(uchar8)(fR*0.299+fG*0.587+fB*0.114);
	  }
	}
  }
  else if (p_Type & RGBPLANAR)
  {
    offset=0;
    while (cinfo.output_scanline < cinfo.output_height)
	{
      jpeg_read_scanlines(&cinfo, buffer, 1);
	  for (j=0; j<cinfo.output_width; j++)
	  {
        map[offset]=buffer[0][j*3+2];
		fR=(float4)buffer[0][j*3+2];
        map[offset+p_ChannelSize]=buffer[0][j*3+1];
		fG=(float4)buffer[0][j*3+1];
        map[offset+p_ChannelSize2x]=buffer[0][j*3+0];
		fB=(float4)buffer[0][j*3+0];
        map[offset+p_ChannelSize*3]=(uchar8)(fR*0.299+fG*0.587+fB*0.114);
	    offset++;
	  }
	}
  }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  return(1);
}

//--------------------------------------------------------------------

int32 CGraphicSurface::m_LoadPNG (char8 *namefile, int32 loadmethod)
{
   png_structp png_ptr;
   png_infop info_ptr;
   png_uint_32 width, height, pass, y, offset;
   int32 bit_depth, color_p_Type, interlace_p_Type;
   uint32 number_passes, j;
   uchar8 *map;
   float4 fR, fG, fB;

   FILE *fp = fopen(namefile, "rb");
   if (!fp) return(FILENOTFOUND);

   // create read struct
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL)
   {
      fclose(fp);
	  return (FILENOTFOUND);
   }

   // create info struct
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL)
   {
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      fclose(fp);
	  //debug_error_lib(hWindow, "PNG Texture reader: png_create_info_struct failed!");
	  return(0);
   }

   if (setjmp(png_jmpbuf(png_ptr)))
   {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
     fclose(fp);
	 // debug_error_lib(hWindow, "PNG Texture reader: setjmp() failed!");
	 return(0);
   }

   strcpy(p_Name, namefile);
   png_init_io(png_ptr, fp);
   png_read_info(png_ptr, info_ptr);
   png_get_IHDR(png_ptr, info_ptr, &width, &height,
                &bit_depth, &color_p_Type, &interlace_p_Type,
                NULL, NULL);

   if (bit_depth != 8)
   {
	  // debug_error_lib(hWindow, "PNG Texture reader: olny 8 bit depth are valid (grayscale or RGB)");
	  return(0);
   }

   if (color_p_Type==PNG_COLOR_TYPE_GRAY)
   {
     p_Type=GREYSCALE;
     map=new uchar8[width*height];
   }
   else
   // alloco 4 canali: R, G, B, A
   if (loadmethod==RGBINTERLEAVED)
   {
      p_Type=RGBINTERLEAVED;
      map=new uchar8[width*height*4];
   }
   else
   if (loadmethod==RGBPLANAR)
   {
      p_Type=RGBPLANAR;
      map=new uchar8[width*height*4];
   }

   p_Pixels=(void *)map;
   p_NumTextures=1;

   // settaggio dei campi provati della texture
   p_Width=width;
   p_XShift=m_Int2Bits(p_Width);
   p_Height=height;
   p_YShift=m_Int2Bits(p_Height);
   p_ChannelSize=p_Width*p_Height;
   p_ChannelSize2x=p_ChannelSize*2;

   // settaggio imagebuffer
   p_MulY=new int32[p_Height];
   for (j=0; j<p_Height; j++) p_MulY[j]=j*p_Width;

   number_passes = png_set_interlace_handling(png_ptr);
   png_bytep row_pointer;
   row_pointer=(png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));

   if (p_Type & GREYSCALE)
   {
      offset=0;
      for (pass=0; pass<number_passes; pass++)
	  {
        for (y=0; y<height; y++)
		{
           png_read_rows(png_ptr, &row_pointer, NULL, 1);
	       for (j=0; j<width; j++)
              map[offset++]=row_pointer[j];
		}
	  }
   }
   else
   if (p_Type & RGBINTERLEAVED)
   {
      offset=0;
      for (pass=0; pass<number_passes; pass++)
	  {
        for (y=0; y<height; y++)
		{
           png_read_rows(png_ptr, &row_pointer, NULL, 1);
		   if (color_p_Type & PNG_COLOR_MASK_ALPHA)
	         for (j=0; j<width; j++)
			 {
				map[offset++]=row_pointer[j*4+2];
                map[offset++]=row_pointer[j*4+1];
                map[offset++]=row_pointer[j*4+0];
                map[offset++]=row_pointer[j*4+3];
			 }
		   else
	         for (j=0; j<width; j++)
			 {
                map[offset++]=row_pointer[j*3+2];
				fR=(float4)row_pointer[j*3+2];
                map[offset++]=row_pointer[j*3+1];
				fG=(float4)row_pointer[j*3+1];
                map[offset++]=row_pointer[j*3+0];
				fB=(float4)row_pointer[j*3+0];
				map[offset++]=(uchar8)(fR*0.299+fG*0.587+fB*0.114);
			 }
		}
	  }
   }
   else if (p_Type & RGBPLANAR)
   {
      offset=0;
      for (pass=0; pass<number_passes; pass++)
	  {
        for (y=0; y<height; y++)
		{
           png_read_rows(png_ptr, &row_pointer, NULL, 1);
		   if (color_p_Type & PNG_COLOR_MASK_ALPHA)
	         for (j=0; j<width; j++)
			 {
                map[offset]=row_pointer[j*4+2];
                map[offset+p_ChannelSize]=row_pointer[j*4+1];
                map[offset+p_ChannelSize2x]=row_pointer[j*4+0];
				map[offset+p_ChannelSize*3]=row_pointer[j*4+3];
	            offset++;
			 }
			 else
	           for (j=0; j<width; j++)
			   {
                  map[offset]=row_pointer[j*3+2];
				  fR=(float4)row_pointer[j*3+2];
                  map[offset+p_ChannelSize]=row_pointer[j*3+1];
				  fG=(float4)row_pointer[j*3+1];
                  map[offset+p_ChannelSize2x]=row_pointer[j*3+0];
				  fB=(float4)row_pointer[j*3+0];
				  map[offset+p_ChannelSize*3]=(uchar8)(fR*0.299+fG*0.587+fB*0.114);
	              offset++;
			   }
		}
	  }
   }
   // fine lettura
   png_read_end(png_ptr, info_ptr);
   png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
   fclose(fp);
   if (color_p_Type & PNG_COLOR_MASK_ALPHA)
	 p_Type |= TEXTURE_HAS_ALPHA;
   return(1);
}

//--------------------------------------------------------------------

int32 CGraphicSurface::m_LoadVideo (char8 *namefile, int32 loadmethod)
{
  CVideoReader reader;
  uchar8 *map, lum;
  uint32 *map32, pixel, *r1, *r2;
  int32 bpp, j, frame_size, i, k, res;
  float4 num_seconds, fR, fG, fB;
  char8 msg[150];

  if (!namefile) return(FILENOTFOUND);
  strcpy(p_Name, namefile);

  if (!reader.m_OpenVideo(namefile)) return(FILENOTFOUND);
  sprintf(msg, "p_Height: %d\n", p_Height);
  DumpDebugPoint(fdebug, msg, 0);

  res=reader.m_GetVideoInfo(&p_Width, &p_Height, &bpp, &num_seconds);
  if (!res) return(FILENOTFOUND);
  sprintf(msg, "p_Height: %d\n", p_Height);
  DumpDebugPoint(fdebug, msg, 0);
  
  p_NumTextures=(int32)(30.0f*num_seconds);
  sprintf(msg, "p_Height: %d\n", p_Height);
  DumpDebugPoint(fdebug, msg, 0);

  if (loadmethod==RGBINTERLEAVED) p_Type=RGBINTERLEAVED;
  else
  if (loadmethod==RGBPLANAR) p_Type=RGBPLANAR;
  sprintf(msg, "p_Height: %d\n", p_Height);
  DumpDebugPoint(fdebug, msg, 0);

  p_XShift=m_Int2Bits(p_Width);
  p_YShift=m_Int2Bits(p_Height);
  sprintf(msg, "p_Height: %d\n", p_Height);
  DumpDebugPoint(fdebug, msg, 0);

  p_ChannelSize=p_Width*p_Height;
  p_ChannelSize2x=p_ChannelSize*2;
  DumpDebugPoint(fdebug, "p_LoadVideo allocation done", 0);
  
  sprintf(msg, "p_Height: %d\n", p_Height);
  DumpDebugPoint(fdebug, msg, 0);

  p_MulY=new int32[p_Height];
  DumpDebugPoint(fdebug, "p_LoadVideo allocation done1", 0);

  for (j=0; j<p_Height; j++) p_MulY[j]=j*p_Width;

  DumpDebugPoint(fdebug, "p_LoadVideo allocation done2", 0);

  map=new uchar8[p_Width*p_Height*4*p_NumTextures];
  p_Pixels=(void *)map;
  DumpDebugPoint(fdebug, "p_LoadVideo allocation done3", 0);

  frame_size=p_Width*p_Height*(bpp/8);
  for (j=0; j<p_NumTextures; j++)
  {
    reader.m_GetFrame(((float4)j)/30.0f, (void *)&map[frame_size*j]);
	sprintf(msg, "p_LoadVideo frame %d read done\n", j);
	DumpDebugPoint(fdebug, msg, 0);
	map32=(uint32 *)&map[frame_size*j];
	for (i=0; i<p_Height/2; i++)
	{
      r1=(uint32 *)&map32[i*p_Width];
      r2=(uint32 *)&map32[p_Width*p_Height-(i+1)*p_Width];
	  for (k=0; k<p_Width; k++)
	  {
		 fR=(float4)((r1[k] >> 16) & 255);
		 fG=(float4)((r1[k] >> 8) & 255);
		 fB=(float4)(r1[k] & 255);
         lum=(uchar8)(fR*0.299+fG*0.587+fB*0.114);
		 r1[k]=(r1[k] & 0x00FFFFFF) | ((uint32)lum << 24);

		 fR=(float4)((r2[k] >> 16) & 255);
		 fG=(float4)((r2[k] >> 8) & 255);
		 fB=(float4)(r2[k] & 255);
         lum=(uchar8)(fR*0.299+fG*0.587+fB*0.114);
		 r2[k]=(r2[k] & 0x00FFFFFF) | ((uint32)lum << 24);

		 pixel=r1[k];
		 r1[k]=r2[k];
		 r2[k]=pixel;
	  }
	}
  }

  reader.m_CloseVideo();
  DumpDebugPoint(fdebug, "p_LoadVideo closing done", 0);
  return(1);
}

//--------------------------------------------------------------------

int32 CGraphicSurface::m_Int2Bits (int32 val)
{
  int32 i;

  i=0;
  while (((val & (1<<i))==0) && (i<32)) i++;
  return(i);
}

//--------------------------------------------------------------------
   
int32 CGraphicSurface::m_Load (char8 *namefile, int32 loadmethod)
{
  int32 i, j, l, res;
  char8 ext[4], msg[150]="";

  if (namefile == (char8 *)NULL)
	return(FILENOTFOUND);

  j=-1;
  l=strlen(namefile);
  for (i=0; i<l; i++)
    if (namefile[i]=='.')
	{
       j=i;
       break;
	}

  if (j<0)
  {
    strcat(msg, "texture::load error: ");
	strcat(msg, namefile);
    strcat(msg, " file has no extention! (only .PNG, .JPG, .AVI, .MPEG, .MPG are valid)");
	//m_Error(msg);
  }

  if (j+1 < l) ext[0]=toupper(namefile[j+1]);
  else ext[0]='\0';
  if (j+2 < l) ext[1]=toupper(namefile[j+2]);
  else ext[1]='\0';
  if (j+3 < l) ext[2]=toupper(namefile[j+3]);
  else ext[2]='\0';
  ext[3]='\0';

  if (strcmp(ext, "PNG")==0) res=m_LoadPNG(namefile, loadmethod);
  else
  if (strcmp(ext, "JPG")==0) res=m_LoadJPG(namefile, loadmethod);
  else
  if (strcmp(ext, "AVI")==0) res=m_LoadVideo(namefile, loadmethod);
  else
  if (strcmp(ext, "MPG")==0) res=m_LoadVideo(namefile, loadmethod);
  else
  if (strcmp(ext, "MPEG")==0) res=m_LoadVideo(namefile, loadmethod);
  else
  {
    strcat(msg, "texture::load error: extension ");
	strcat(msg, ext);
    strcat(msg, " is not supported! (only .PNG, .JPG, .AVI, .MPEG, .MPG are allowed)");
	//debug_error_lib(hWindow, msg);
  }

  if (res != FILENOTFOUND)
  {
    p_HWSurfaces=new LPDIRECT3DTEXTURE8[p_NumTextures];
    for (j=0; j<p_NumTextures; j++)
      p_HWSurfaces[j]=(IDirect3DTexture8 *)NULL;
    p_CurrentTextureIndex=0;
  }
  return(res);
}

//--------------------------------------------------------------------

void CGraphicSurface::m_Update(float4 frame)
{
  if ((frame<=p_StartFrame) || (p_NumTextures==1))
  {
     p_CurrentTextureIndex=0;
	 return;
  }

  p_CurrentTextureIndex=(int32)(frame*p_PlayBackRate);

  if (p_EndCondition != ENDCONDITION_PINGPONG)
  {
    if (p_CurrentTextureIndex >= p_NumTextures)
	{
	  switch (p_EndCondition)
	  {
        case ENDCONDITION_LOOP:
          p_CurrentTextureIndex=p_CurrentTextureIndex % p_NumTextures;
          break;
        case ENDCONDITION_HOLD:
          p_CurrentTextureIndex=p_NumTextures-1;
          break;

	  }
	}
  }
  // caso di PINGPONG
  else
  {
	 // mi ricavo la direzione di moto
	 p_PlayDirection=(p_CurrentTextureIndex / p_NumTextures) & 1;
	 // 0 signifa che sto andando avanti, 1 indietro
	 if (p_PlayDirection==0)
	 {
        p_CurrentTextureIndex=p_CurrentTextureIndex % p_NumTextures;
	 }
	 else
	 {
        p_CurrentTextureIndex=p_NumTextures-(p_CurrentTextureIndex % p_NumTextures)-1;
	 }
  }
}

//--------------------------------------------------------------------

void *CGraphicSurface::m_Lock(void)
{
   return(p_Pixels);
}

//--------------------------------------------------------------------

void CGraphicSurface::m_Unlock(void)
{
}