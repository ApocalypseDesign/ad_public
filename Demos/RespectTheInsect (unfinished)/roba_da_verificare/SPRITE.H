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


class AD_Sprite : public texture, public efx2D
{
private:

   AD_Sprite_Key keys[MAXKEY];
   int num_keys;
   int type;

public:

   AD_Sprite();
   void add_key(float relpos, float x, float y, float scale_x, float scale_y);
   void set_render_type(int render_type);

   // ########################################################
   // ##############   VERSIONI SENZA SCALING   ##############
   // ########################################################

   void put(int x, int y, imagebuffer *imagebuffer);
   void put(float pos, imagebuffer *imagebuf);
   void put_clip(int x, int y, imagebuffer *imagebuf);
   void put_clip(float pos, imagebuffer *imagebuf);

   void put_blend50(int x, int y, imagebuffer *imagebuf);
   void put_blend50(float pos, imagebuffer *imagebuf);
   void put_blend50_clip(int x, int y, imagebuffer *imagebuf);
   void put_blend50_clip(float pos, imagebuffer *imagebuf);

   void put_add(int x, int y, imagebuffer *imagebuf);
   void put_add(float pos, imagebuffer *imagebuf);
   void put_add_clip(int x, int y, imagebuffer *imagebuf);
   void put_add_clip(float pos, imagebuffer *imagebuf);
   


   // ########################################################
   // ##############   VERSIONI CON  SCALING   ###############
   // ########################################################
   
   void put_scaled(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled(float pos, imagebuffer *imagebuf);
   void put_scaled_clip(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled_clip(float pos, imagebuffer *imagebuf);

   void put_scaled_add(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled_add(float pos, imagebuffer *imagebuf);
   void put_scaled_add_clip(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled_add_clip(float pos, imagebuffer *imagebuf);

   void put_scaled_blend50(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled_blend50(float pos, imagebuffer *imagebuf);
   void put_scaled_blend50_clip(int x1, int y1, int x2, int y2, imagebuffer *imagebuf);
   void put_scaled_blend50_clip(float pos, imagebuffer *imagebuf);


   void init();
   void do_efx2D(double pos);
};

#endif