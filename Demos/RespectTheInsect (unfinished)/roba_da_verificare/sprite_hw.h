#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "ADrender.h"
#include "efx2D.h"

#define MAXKEY 10
#define NOSCALING 1


// costanti da mettere in OR per definire il metodo di
// rendering
#define RENDER_SCALING      1
#define RENDER_NOSCALING    2
#define RENDER_CLIP         4
#define RENDER_NOCLIP       8
#define RENDER_ADD         16
#define RENDER_BLEND50     32
#define RENDER_OVERWRITE   64


typedef struct
{
  float relpos;
  float x, y, scale_x, scale_y;
} AD_Sprite_Key;


typedef struct
{
  float relpos;
  float tlevel;
} AD_Sprite_tKey;


class AD_Sprite : public texture, public efx2D
{
private:
   AD_Vertex2D vert[4];
   material mat;
   int use_tmask;

   AD_Sprite_Key  keys[MAXKEY];
   AD_Sprite_tKey tkeys[MAXKEY];
   int num_keys, num_tkeys;
   int type;

public:

   AD_Sprite();
   void load_transparency_map(char *nomefileJPEG, BOOL needinvert);
   void add_key(float relpos, float x, float y, float scale_x, float scale_y);
   void add_tkey(float relpos, float tlevel);
   void set_render_type(int render_type);

   // ########################################################
   // ##############   VERSIONI SENZA SCALING   ##############
   // ########################################################

   void put(int x, int y, imagebuffer *imagebuffer);
   void put(float pos, imagebuffer *imagebuf);

   void put_blend50(int x, int y, imagebuffer *imagebuf);
   void put_blend50(float pos, imagebuffer *imagebuf);

   void put_add(int x, int y, imagebuffer *imagebuf);
   void put_add(float pos, imagebuffer *imagebuf);
   

   // ########################################################
   // ##############   VERSIONI CON  SCALING   ###############
   // ########################################################
   
   void put_scaled(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled(float pos, imagebuffer *imagebuf);

   void put_scaled_add(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled_add(float pos, imagebuffer *imagebuf);

   void put_scaled_blend50(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled_blend50(float pos, imagebuffer *imagebuf);

   void init();
   void do_efx2D(double pos);
};

#endif