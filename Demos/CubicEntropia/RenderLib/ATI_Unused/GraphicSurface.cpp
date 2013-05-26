#include "Renderlib.h"


CGraphicSurface::CGraphicSurface()
{
   //p_HWRenderPos=-1;
   p_HWSurface=(IDirect3DTexture8 *)NULL;
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
   p_Type=-1;
   p_Palette=(char8 *)NULL;
   p_ScanLines=(void *)NULL;
}

//--------------------------------------------------------------------

CGraphicSurface::CGraphicSurface(int32 Width, int32 Height, int32 Bpp)
{
   if (Width<=0) return;
   if (Height<=0) return;
   if ((Bpp<=0) || (Bpp>32)) return;

   uchar8 *l_pixels;

   p_HWSurface=(IDirect3DTexture8 *)NULL;
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
   //p_Type=-1;
   p_Palette=(char8 *)NULL;
   p_ScanLines=(void *)NULL;
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
    map=(uchar8 *)malloc(cinfo.output_width*cinfo.output_height);
  }
  else
  if (loadmethod==RGBINTERLEAVED)
  {
    p_Type=RGBINTERLEAVED;
    map=(uchar8 *)malloc(cinfo.output_width*cinfo.output_height*4);
  }
  else if (loadmethod==RGBPLANAR)
  {
    p_Type=RGBPLANAR;
    map=(uchar8 *)malloc(cinfo.output_width*cinfo.output_height*4);
  }

  p_Pixels=(void *)map;

  p_Width=cinfo.output_width;
  p_XShift=m_Int2Bits(p_Width);
  p_Height=cinfo.output_height;
  p_YShift=m_Int2Bits(p_Height);
  p_ChannelSize=p_Width*p_Height;
  p_ChannelSize2x=p_ChannelSize*2;

  p_MulY=new int32[p_Height];
  for (j=0; j<p_Height; j++) 
	p_MulY[j]=j*p_Width;

  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
	       ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  if (p_Type==GREYSCALE)
  {
    offset=0;
    while (cinfo.output_scanline < cinfo.output_height)
	{
      jpeg_read_scanlines(&cinfo, buffer, 1);
	  for (j=0; j<cinfo.output_width; j++)
	  {
         map[offset]=buffer[0][j];
		 offset++;
	  }
	}
  }
  else
  if (p_Type==RGBINTERLEAVED)
  {
    offset=0;
    while (cinfo.output_scanline < cinfo.output_height)
	{
      jpeg_read_scanlines(&cinfo, buffer, 1);
	  for (j=0; j<cinfo.output_width; j++)
	  {
        map[offset]=buffer[0][j*3+2]; offset++;
        map[offset]=buffer[0][j*3+1]; offset++;
        map[offset]=buffer[0][j*3+0]; offset++;
		map[offset]=0; offset++;
	  }
	}
  }
  else if (p_Type==RGBPLANAR)
  {
    offset=0;
    while (cinfo.output_scanline < cinfo.output_height)
	{
      jpeg_read_scanlines(&cinfo, buffer, 1);
	  for (j=0; j<cinfo.output_width; j++)
	  {
        map[offset]=buffer[0][j*3+2];
        map[offset+p_ChannelSize]=buffer[0][j*3+1];
        map[offset+p_ChannelSize2x]=buffer[0][j*3+0];
        map[offset+p_ChannelSize*3]=0;
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
     map=(uchar8 *)malloc(width*height);
   }
   else
   // alloco 4 canali: R, G, B, A
   if (loadmethod==RGBINTERLEAVED)
   {
      p_Type=RGBINTERLEAVED;
      map=(uchar8 *)malloc(width*height*4);
   }
   else
   if (loadmethod==RGBPLANAR)
   {
      p_Type=RGBPLANAR;
      map=(uchar8 *)malloc(width*height*4);
   }

   p_Pixels=(void *)map;

   // settaggio dei campi provati della texture
   p_Width=width;
   p_XShift=m_Int2Bits(p_Width);
   p_Height=height;
   p_YShift=m_Int2Bits(p_Height);
   p_ChannelSize=p_Width*p_Height;
   p_ChannelSize2x=p_ChannelSize*2;

   // settaggio imagebuffer
   p_MulY=new int32[p_Height];
   for (j=0; j<p_Height; j++)
	  p_MulY[j]=j*p_Width;

   number_passes = png_set_interlace_handling(png_ptr);
   png_bytep row_pointer;

   row_pointer=(png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
   if (p_Type==GREYSCALE)
   {
      offset=0;
      for (pass=0; pass<number_passes; pass++)
	  {
        for (y=0; y<height; y++)
		{
           png_read_rows(png_ptr, &row_pointer, NULL, 1);
	       for (j=0; j<width; j++)
		   {
              map[offset]=row_pointer[j];
		      offset++;
		   }
		}
	  }
   }
   else
   if (p_Type==RGBINTERLEAVED)
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
				map[offset]=row_pointer[j*4+2]; offset++;
                map[offset]=row_pointer[j*4+1]; offset++;
                map[offset]=row_pointer[j*4+0]; offset++;
                map[offset]=row_pointer[j*4+3]; offset++;
			 }
		   else
	         for (j=0; j<width; j++)
			 {
                map[offset]=row_pointer[j*3+2]; offset++;
                map[offset]=row_pointer[j*3+1]; offset++;
                map[offset]=row_pointer[j*3+0]; offset++;
				map[offset]=0;                  offset++;
			 }
		}
	  }
   }
   else if (p_Type==RGBPLANAR)
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
                  map[offset+p_ChannelSize]=row_pointer[j*3+1];
                  map[offset+p_ChannelSize2x]=row_pointer[j*3+0];
				  map[offset+p_ChannelSize*3]=0;
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
   return(TEXTURE_HAS_ALPHA);
   else return(1);
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
  int32 i, j, l;
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
    strcat(msg, " file has no extention! (only .PNG, .JPG, .PCX are valid)");
	//debug_error_lib(hWindow, msg);
  }

  if (j+1 < l) ext[0]=toupper(namefile[j+1]);
  else ext[0]='\0';
  if (j+2 < l) ext[1]=toupper(namefile[j+2]);
  else ext[1]='\0';
  if (j+3 < l) ext[2]=toupper(namefile[j+3]);
  else ext[2]='\0';
  ext[3]='\0';

  if (strcmp(ext, "PNG")==0) return(m_LoadPNG(namefile, loadmethod));
  else
  if (strcmp(ext, "JPG")==0) return(m_LoadJPG(namefile, loadmethod));
  else
  {
    strcat(msg, "texture::load error: extension ");
	strcat(msg, ext);
    strcat(msg, " is not supported! (only .PNG, .JPG, .PCX are allowed)");
	//debug_error_lib(hWindow, msg);
  }
  return(1);
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