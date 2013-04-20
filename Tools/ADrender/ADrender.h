#ifndef _ADRENDER_H_
#define _ADRENDER_H_

#include <stdio.h>
#include <ddraw.h>
#define WIN32_LEAN_AND_MEAN	
#define D3D_OVERLOADS // deve stare prima di #include <d3d.h>
#include <d3d8.h>

typedef struct
{
    DWORD      Width;      // Screen width in this mode
    DWORD      Height;     // Screen height in this mode
	char       Bpp;        // bits per pixel; calcolato in base a Format
    D3DFORMAT  Format;     // Pixel format in this mode
    DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
    D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
}D3DModeInfo;


typedef struct
{
    // D3D Device info
    CHAR           strDesc[40];

    // DDraw Mode Info
    D3DModeInfo    WantedMode;
    BOOL           bFullscreen;

    // For internal use (apps should not need these)
    D3DModeInfo    SupportedModes[100];
    DWORD          dwNumModes;
    BOOL           bCanDoWindowed;
    D3DMULTISAMPLE_TYPE MultiSampleType;  // livelli di FSAA

    D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
    D3DCAPS8     d3dCaps;         // Capabilities of this device

}D3DDEVICEINFO;


typedef struct
{
  float xs, ys, z, dist;
  float u, v, envu, envv, R, G, B;
} AD_Vertex2D;


typedef void (__cdecl *DrawTriangleProc)(AD_Vertex2D *, AD_Vertex2D *, AD_Vertex2D *);
typedef void (__cdecl *ScanlineProc)(void);


// ***********************************************************
// ********************  CLASSI ESPORTATE  *******************
// ***********************************************************


// *******************************************************
// **********************  BUFFERS  **********************
// *******************************************************
class imagebuffer
{
private:
   unsigned char *real_where;
   int real_height, real_channelsize;
protected:
public:
   
   void *voidptr;
   unsigned char *uchar8ptr;
   unsigned int *uint32ptr;
   int channelsize, width, height;
   int *muly;

   imagebuffer() //Constructor
   { real_channelsize=real_height=channelsize=width=height=0; }
   imagebuffer(int sizex, int sizey); //Constructor

   void clear (unsigned char value);
   void clear (unsigned char r, unsigned char g, unsigned char b);
   void set_cinemascope(int top, int bottom);
};


class floatbuffer
{
private:
protected:
public:
   
   float *where;
   int size, width, height;
   int *muly;

   floatbuffer(void) //Constructor
   { size=width=height=0; }
   floatbuffer(int sizex, int sizey); //Constructor

   void clear (float value);
};


// *******************************************************
// **********************  TEXTURE  **********************
// *******************************************************
#define GREYSCALE 0        // texture a 256 colori con palatte
#define RGBPLANAR 1        // texture RGB in modo planare (RRRR GGGG BBBB)
#define RGBINTERLEAVED 2   // texture RBG in modo interlacciato (RGBA RGBA)
#define TEXTURE_HAS_ALPHA 10   // la texture letta ha anche il canale alpha
#define FILENOTFOUND -1

typedef struct
{
	  char manufacturer;              // INDICA SE IL FILE E' UN PCX
	  char version;                   // VERSIONE
	  char encoding;                  // TIPO DI COMPRESSIONE
	  char bitperpixel;
	  unsigned short int xmin,ymin;         // ANGOLO SUPERIORE SINISTRO
	  unsigned short int xmax,ymax;         // ANGOLO INFERIORE DESTRO
	  unsigned short int horres;
	  unsigned short int verres;
	  unsigned char egapalette[48];   // PALETTE EGA
	  char reserved;                  // RISERVATO
	  char colourplanes;              // NUMERO DI PIANI COLORE
	  unsigned short int bytesperline;      // NUMERO DI BYTES PER LINEA
	  unsigned short int palettetype;       // TIPO DI PALETTE
	  unsigned char filler[58];       // INUTILIZZATO
} PCXHEADER;


class texture
{
 private:
    // dice quanti la potenza di due per ottenere il valore val
    // es. se val=256 ritorna 8 (2^8=256); serve per settare i campi xbits
    // e ybits
    int dim_to_bits (int val);
    void read_pcxline (FILE *f, int bytes, unsigned char *buffer);

 public:
   int HW_ADRender_Pos;
   int dimx, dimy, xbits, ybits, channelsize, channelsize2x;
   float fdimx_opt, fdimy_opt;
   char name[70];
   unsigned char *map;
   unsigned int *map32;
   unsigned char *palette;  // eventuale palette
   unsigned char type;      // tipo di texture; vedi le costanti sopra
   imagebuffer img;
   
   // costruttori
   texture ();
   texture (int width, int height, int num_channels, int bit_per_channel);
   imagebuffer *get_my_image_buffer(void);
   
   // restituiscono -1 (FILENOTFOUND) oppure 0 (tutto ok)
   int load_pcx (char *nfile, int loadmethod);
   int load_jpg (char *nfile, int loadmethod);
   int load_png (char *nfile, int loadmethod);
   int load (char *nfile, int loadmethod);
};


// *******************************************************
// *********************  MATERIALI  *********************
// *******************************************************
#define ON 1
#define OFF 0
// utilizzate maps-mixtype
#define MAPSMIXTYPE_TEXSUBENV     256
#define MAPSMIXTYPE_ENVSUBTEX     257

// utilizzata sia per maps/light/trasparency-mixtype
#define MIXTYPE_ADD               258
#define MIXTYPE_BLEND50           259
#define MIXTYPE_ALPHABLEND        260
#define ALPHA_MAP                 264
#define LIGHTMIXTYPE_SUB          261
#define LIGHTMIXTYPE_NONE         262
#define LIGHTMIXTYPE_BLEND3DS     263

// utilizzata per bumptype
#define BUMP_EMBM  270
#define BUMP_DOT3  271

// questi campi serviranno nella paint degli oggetti per
// scegliere i vari calcoli da fare (env, luci, ...)
#define PAINT_FLAT 1
#define PAINT_LIGHT 2
#define PAINT_TEXTURE 4
#define PAINT_ENVMAP 8
#define IS_TRASPARENT 16
#define PAINT_BUMPMAP 32

class material
{
 public:
 char *name;
 int trasparency,
     trasparencytype,
	 mapsmixtype,
	 alphamixfactor,
	 lightmixtype,
	 bumptype;
 int double_facing;
 int flags;
 int zsorting;
 void **my_tria_list, **my_tria_list_copy;
 int my_num_tria;
 float tile_u, tile_v;

// di 256 entry, contiene la solita formula di blending lineare
// a*t + b*(1-t)
 unsigned char *trasparency_table;

// di 256 entry, contiene la solita formula di blending lineare
// a*t + b*(1-t) 
 unsigned char *alphablend_table;

 DrawTriangleProc myraster;
 ScanlineProc myscanline; // a seconda dei campi sopra
                           // contiene una diversa routine
                           // di scanline


 // posizioni nell'array di oggetti "texture" :
 texture *texture_ptr;
 texture *envmap_ptr;
 texture *bump_ptr;
 texture *alphamap_ptr;

 material() {
			  envmap_ptr=texture_ptr=bump_ptr=alphamap_ptr=(texture *)NULL;
			  bumptype=trasparencytype=OFF;
			  trasparency=OFF;  // default niente trasparenza
	          mapsmixtype=MIXTYPE_BLEND50;  // default media aritmetica
	          alphamixfactor=0;
	          lightmixtype=MIXTYPE_BLEND50; // default media aritmetica
			  alphablend_table=(unsigned char *)NULL;
			  trasparency_table=(unsigned char *)NULL;
			  name=(char *)NULL;
              my_num_tria=0;
			  my_tria_list=(void**)NULL;
			  double_facing=OFF;
			  zsorting=OFF;
			  tile_u=tile_v=1.0f;
			  flags=0;
            }
 private:
 protected:
};


// #############################
// #   METODI DELLA ADRENDER   #
// #############################
int Enumerate_DrawDriver (D3DDEVICEINFO **tab_dev);
imagebuffer *Open_Screen (HWND hWnd, D3DDEVICEINFO *devinf);

int AddUpdate_Material (material *m);
int AddUpdate_Texture (texture *t);
int AddUpdate_AlphaTexture (texture *t);
int AddUpdate_BumpTexture (texture *t);
DrawTriangleProc Set_Material (material *m);

void Begin_Scene();
void End_Scene();
void Begin_ExclusiveTransparency(void);
void End_ExclusiveTransparency(void);
void Update_Screen(void);
void Close_Screen();
void Clear_DepthBuffer(float zclear);
void Set_ZRanges(float znear, float zfar);

//------------------------------------------
// metodi specifici per la versione software
//------------------------------------------
floatbuffer *SW_Get_WBuffer();


extern float TEXTURE_PRECISION;
extern float ENVMAP_PRECISION;
extern float RGB_PRECISION;
extern float RGB_MAXVALUE;

// PROCEDURE DI SUPPORTO INTERNE
void init_render (imagebuffer *videobuf, floatbuffer *zb);
void set_material_procedures (material *m);
int EnumerateDriver(D3DDEVICEINFO **tabdev);

// tipicamente lo schema di utilizzo e' il seguente:
//
// - a load time si fanno le varie load delle texture
//   seguite dalle AddUpdate_Texture
// - sempre a load time si caricano i materiali seguiti
//   dai AddUpdate_Material
//
// A runtime:
// - <selezione telecamera>
// - Clear_DepthBuffer(camera.znear)
// - Begin_Scene()
// -   Set_Material(materiale_1)
// -   <disegno dei triangoli>
// -   ......
// -   ......
// -   Set_Material(materiale_n)
// -   <disegno dei triangoli>
// - End_Scene()

#endif