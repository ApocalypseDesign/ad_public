// Metodi della classe texture
// By [Turbo]

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "ADrender.h"
#include "ADbase.h"
#include "jpeglib.h"
#include "png.h"

extern HWND hWindow;

texture::texture(void)
{
  dimx=dimy=xbits=ybits=channelsize=channelsize2x=0;
  strcpy(name, "");
  map=(unsigned char *)NULL;
  map32=(unsigned int *)NULL;
  palette=(unsigned char *)NULL;
  type=RGBINTERLEAVED;

  HW_ADRender_Pos=-1;
  fdimx_opt=fdimx_opt=0;

  img.width=img.height=img.channelsize=0;
  img.voidptr=(void *)NULL;
  img.uchar8ptr=(unsigned char *)NULL;
  img.uint32ptr=(unsigned int *)NULL;
  img.muly=(int *)NULL;
}


texture::texture (int width, int height, int num_channels, int bit_per_channel)
{
  int j;

  dimx=width;
  dimy=height;
  xbits=dim_to_bits(dimx);
  ybits=dim_to_bits(dimy);
  channelsize=dimx*dimy;
  channelsize2x=2*channelsize;
  strcpy(name, "Costructor2");
  map=new unsigned char[channelsize*num_channels];
  map32=(unsigned int *)map;
  palette=(unsigned char *)NULL;
  type=RGBINTERLEAVED;

  HW_ADRender_Pos=-1;
  fdimx_opt=fdimx_opt=0;

  img.width=width;
  img.height=height;
  img.channelsize=channelsize;
  img.voidptr=(void *)map;
  img.uchar8ptr=(unsigned char *)map;
  img.uint32ptr=(unsigned int *)map;
  img.muly=(int *)malloc(dimy*sizeof(int));
  for (j=0; j<dimy; j++) img.muly[j]=j*dimx;
}


imagebuffer *texture::get_my_image_buffer(void)
{
  if ((img.width>0) && (img.height>0) &&
	  (img.channelsize>0) && (img.voidptr!=(void *)NULL) &&
	  (img.uchar8ptr!=(unsigned char *)NULL) &&
      (img.uint32ptr!=(unsigned int *)NULL) &&
      (img.muly!=(int *)NULL))
  {
	return(&img);
  }
  else return ((imagebuffer *)NULL);
}


int texture::dim_to_bits (int val)
{
  int i;

  i=0;
  while (((val & (1<<i))==0) && (i<16))
     i++;
  return(i);
}


void texture::read_pcxline(FILE *f, int bytes, unsigned char *buffer)
// legge e decompatta uan linea salvata secondo un algoritmo RLE (Run Lenght
// Encoding) specifico del formato PCX
{
  int count, written;  // written = numero di byte decompressi
  unsigned char code;  // valore letto dal file

  written=0;
  do
  {
     fread(&code, 1, 1, f);    // legge un byte
     if ((code & 0xc0)==0xc0)
       { // di deve leggere il prossimo dato
	  count=(code & 0x3f);
	  fread (&code, 1, 1, f); // che rappresenta il colore
	  memset(&buffer[written], code, count);
	  written+=count;
       }
      else   // gli ultimi 2 bit non sono settati quindi e' un colore
	{
	   buffer[written]=code;
	   written++;
	}
  } while (written<bytes);
}


int texture::load_pcx(char *nfile, int loadmethod)
{
   PCXHEADER pcx;
   FILE *pcxfile;
   int width, depth, Y, X, P, pianointerl, j;
   unsigned char buffer_line[1024];
   fpos_t pos;

    if ((pcxfile=fopen(nfile,"rb"))==NULL) return(FILENOTFOUND);
    strcpy(name, nfile);

    // legge l'header per ricavare le informazioni utili
    fread (&pcx, 1, sizeof(PCXHEADER), pcxfile);

    width=pcx.xmax-pcx.xmin+1;  // NUMERO DI PIXEL IN ORIZZONTALE
    depth=pcx.ymax-pcx.ymin+1;  // NUMERO DI PIXEL IN VERTICALE

    // settaggio dei campi provati della texture
    dimx=width; xbits=dim_to_bits(width);
    dimy=depth; ybits=dim_to_bits(depth);
    channelsize=width*depth;
    channelsize2x=width*depth*2;
    
	// settaggio imagebuffer
	img.width=dimx;
	img.height=depth;
	img.channelsize=channelsize;
    img.muly=(int *)malloc(dimy*sizeof(int));
    for (j=0; j<dimy; j++) img.muly[j]=j*dimx;


    // ######  in questo caso e' un PCX a 256 colori ######
    if (pcx.colourplanes==1)
    {
      type=GREYSCALE;       // settaggio del tipo di PCX caricata
      palette=(unsigned char*)malloc(768);  // alloca la palette

      // memorizza la posizione corrente del file
      fgetpos(pcxfile, &pos);

      // si posiziona per leggere la palette
      fseek(pcxfile, -768L, SEEK_END);

      // legge la palette e la sistema per averla in formato rbg a 6 bit
      fread(palette, 768, 1, pcxfile);
      for (Y=0; Y<768; Y++) palette[Y]=(palette[Y] >> 2);

      // riposizionamento, pronti per leggere l'immagine vera e propria
      fsetpos(pcxfile, &pos);
      // alloca spazio per l'immagine decompressa
      map=(unsigned char *)malloc(channelsize);
	  map32=(unsigned int *)map;

      img.voidptr=(void *)map;
      img.uchar8ptr=(unsigned char *)map;
      img.uint32ptr=(unsigned int *)map32;
    }

    // ######  in questo caso e' un PCX a 24 bit ######
    if (pcx.colourplanes==3)
    {
       if (loadmethod==RGBPLANAR)
       {
	     type=RGBPLANAR;
	     map=(unsigned char *)malloc(channelsize*3);
		 map32=(unsigned int *)map;
         img.voidptr=(void *)map;
         img.uchar8ptr=(unsigned char *)map;
         img.uint32ptr=(unsigned int *)map32;
       }
       else
       {
	     type=RGBINTERLEAVED;
	     // si alloca anche il canale alpha per avere allineamento ai 4 byte
	     map=(unsigned char *)malloc(channelsize*4);
		 map32=(unsigned int *)map;
         img.voidptr=(void *)map;
         img.uchar8ptr=(unsigned char *)map;
         img.uint32ptr=(unsigned int *)map32;
       }
    }

    // ciclo di decompressione vero e proprio
    for (Y=0; Y<=(depth-1); Y++)
    {
      for (P=0; P<pcx.colourplanes; P++)
      {
	    // legge una linea di pixel di un canale
	    read_pcxline(pcxfile, width, buffer_line);

	    if (loadmethod==RGBINTERLEAVED)
		{
	      if (P==0) pianointerl=2;   // rosso
	      if (P==1) pianointerl=1;   // verde
	      if (P==2) pianointerl=0;   // blu

	      for (X=0; X<width; X++)
	        map[Y*width*4+X*4+pianointerl]=buffer_line[X];
		}
	    // caso di immagini a 256 colori o RGB planari
	    else memcpy((map+(P*channelsize+Y*width)), buffer_line, width);
	  }
	}

    fclose(pcxfile);
	return(0);
}


// ###########################################################
// #####################  JPEG LOADER  #######################
// ###########################################################
int texture::load_jpg (char *nfile, int loadmethod)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *infile;
  JSAMPARRAY buffer;
  int row_stride;
  unsigned int j, offset;
  int jj;

  if ((infile = fopen(nfile, "rb")) == NULL) return(FILENOTFOUND);
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  strcpy(name, nfile);

  if ((cinfo.num_components==1) &&
	  (cinfo.out_color_space==JCS_GRAYSCALE))
  {
    type=GREYSCALE;
    map=(unsigned char *)malloc(cinfo.output_width*cinfo.output_height);
  }
  else
  if (loadmethod==RGBINTERLEAVED)
  {
    type=RGBINTERLEAVED;
    map=(unsigned char *)malloc(cinfo.output_width*cinfo.output_height*4);
  }
  else if (loadmethod==RGBPLANAR)
  {
    type=RGBPLANAR;
    map=(unsigned char *)malloc(cinfo.output_width*cinfo.output_height*4);
  }
  map32=(unsigned int *)map;
  img.voidptr=(void *)map;
  img.uchar8ptr=(unsigned char *)map;
  img.uint32ptr=(unsigned int *)map32;


  // settaggio dei campi provati della texture
  dimx=cinfo.output_width;
  xbits=dim_to_bits(dimx);
  dimy=cinfo.output_height;
  ybits=dim_to_bits(dimy);
  channelsize=dimx*dimy;
  channelsize2x=channelsize*2;

  // settaggio imagebuffer
  img.width=dimx;
  img.height=dimy;
  img.channelsize=channelsize;
  img.muly=(int *)malloc(dimy*sizeof(int));
  for (jj=0; jj<dimy; jj++) img.muly[jj]=jj*dimx;

  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
	       ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  if (type==GREYSCALE)
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
  if (type==RGBINTERLEAVED)
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
  else if (type==RGBPLANAR)
  {
    offset=0;
    while (cinfo.output_scanline < cinfo.output_height)
	{
      jpeg_read_scanlines(&cinfo, buffer, 1);
	  for (j=0; j<cinfo.output_width; j++)
	  {
        map[offset]=buffer[0][j*3+2];
        map[offset+channelsize]=buffer[0][j*3+1];
        map[offset+channelsize2x]=buffer[0][j*3+0];
        map[offset+channelsize*3]=0;
	    offset++;
	  }
	}
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  return(0);
}


int texture::load_png (char *nfile, int loadmethod)
{
   png_structp png_ptr;
   png_infop info_ptr;
   png_uint_32 width, height, pass, y, offset;
   int bit_depth, color_type, interlace_type, jj;
   unsigned int number_passes, j;

   FILE *fp = fopen(nfile, "rb");
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
	  debug_error_lib(hWindow, "PNG Texture reader: png_create_info_struct failed!");
   }

   if (setjmp(png_jmpbuf(png_ptr)))
   {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
     fclose(fp);
	 debug_error_lib(hWindow, "PNG Texture reader: setjmp() failed!");
   }

   png_init_io(png_ptr, fp);

   png_read_info(png_ptr, info_ptr);
   png_get_IHDR(png_ptr, info_ptr, &width, &height,
                &bit_depth, &color_type, &interlace_type,
                NULL, NULL);

   if (bit_depth != 8)
	  debug_error_lib(hWindow, "PNG Texture reader: olny 8 bit depth are valid (grayscale or RGB)");


   if (color_type==PNG_COLOR_TYPE_GRAY)
   {
     type=GREYSCALE;
     map=(unsigned char *)malloc(width*height);
   }
   else
   // alloco 4 canali: R, G, B, A
   if (loadmethod==RGBINTERLEAVED)
   {
      type=RGBINTERLEAVED;
      map=(unsigned char *)malloc(width*height*4);
   }
   else
   if (loadmethod==RGBPLANAR)
   {
      type=RGBPLANAR;
      map=(unsigned char *)malloc(width*height*4);
   }

   map32=(unsigned int *)map;
   img.voidptr=(void *)map;
   img.uchar8ptr=(unsigned char *)map;
   img.uint32ptr=(unsigned int *)map32;

   // settaggio dei campi provati della texture
   dimx=width;
   xbits=dim_to_bits(dimx);
   dimy=height;
   ybits=dim_to_bits(dimy);
   channelsize=dimx*dimy;
   channelsize2x=channelsize*2;

   // settaggio imagebuffer
   img.width=dimx;
   img.height=dimy;
   img.channelsize=channelsize;
   img.muly=(int *)malloc(dimy*sizeof(int));
   for (jj=0; jj<dimy; jj++) img.muly[jj]=jj*dimx;

   number_passes = png_set_interlace_handling(png_ptr);
   png_bytep row_pointer;

   row_pointer=(png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
   if (type==GREYSCALE)
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
   if (type==RGBINTERLEAVED)
   {
      offset=0;
      for (pass=0; pass<number_passes; pass++)
	  {
        for (y=0; y<height; y++)
		{
           png_read_rows(png_ptr, &row_pointer, NULL, 1);
		   if (color_type & PNG_COLOR_MASK_ALPHA)
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
   else if (type==RGBPLANAR)
   {
      offset=0;
      for (pass=0; pass<number_passes; pass++)
	  {
        for (y=0; y<height; y++)
		{
           png_read_rows(png_ptr, &row_pointer, NULL, 1);
		   if (color_type & PNG_COLOR_MASK_ALPHA)
	         for (j=0; j<width; j++)
			 {
                map[offset]=row_pointer[j*4+2];
                map[offset+channelsize]=row_pointer[j*4+1];
                map[offset+channelsize2x]=row_pointer[j*4+0];
				map[offset+channelsize*3]=row_pointer[j*4+3];
	            offset++;
			 }
			 else
	           for (j=0; j<width; j++)
			   {
                  map[offset]=row_pointer[j*3+2];
                  map[offset+channelsize]=row_pointer[j*3+1];
                  map[offset+channelsize2x]=row_pointer[j*3+0];
				  map[offset+channelsize*3]=0;
	              offset++;
			   }
		}
	  }
   }

   // fine lettura
   png_read_end(png_ptr, info_ptr);
   png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
   fclose(fp);
   if (color_type & PNG_COLOR_MASK_ALPHA)
   return(TEXTURE_HAS_ALPHA);
   else return(0);
}


int texture::load (char *nfile, int loadmethod)
{
  int i, j, l;
  char ext[4], msg[150]="";

  if (nfile == (char *)NULL) return (FILENOTFOUND);

  j=-1;
  l=strlen(nfile);

  for (i=0; i<l; i++)
     if (nfile[i]=='.') { j=i; break; }

  if (j<0)
  {
    strcat(msg, "texture::load error: ");
	strcat(msg, nfile);
    strcat(msg, " file has no extention! (only .PNG, .JPG, .PCX are valid)");
	debug_error_lib(hWindow, msg);
  }

  if (j+1 < l) ext[0]=toupper(nfile[j+1]);
  else ext[0]='\0';
  if (j+2 < l) ext[1]=toupper(nfile[j+2]);
  else ext[1]='\0';
  if (j+3 < l) ext[2]=toupper(nfile[j+3]);
  else ext[2]='\0';
  ext[3]='\0';

  if (strcmp(ext, "PNG")==0) return(load_png(nfile, loadmethod));
  else
  if (strcmp(ext, "JPG")==0) return(load_jpg(nfile, loadmethod));
  else
  if (strcmp(ext, "PCX")==0) return(load_pcx(nfile, loadmethod));
  else
  {
    strcat(msg, "texture::load error: extension ");
	strcat(msg, ext);
    strcat(msg, " is not supported! (only .PNG, .JPG, .PCX are allowed)");
	debug_error_lib(hWindow, msg);
  }
  return(0);
}
