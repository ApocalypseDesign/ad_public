#ifndef _TEXTSCROLLER2D_H_
#define _TEXTSCROLLER2D_H_

#include <math.h>
#include "..\efx2D.h"

#define MAX_PHRASE  200
#define FONT_ADD 1
#define FONT_BLEND50 2
#define FONT_OVERWRITE 3
#define FONT_LEFT_ALIGNED 4
#define FONT_CENTER_ALIGNED 5
#define FONT_RIGHT_ALIGNED 6


typedef struct
{
   char phrase[56];
   int color;
   int x_alignement;
} PHRASE, *PHRASE_PTR;


class TextScroller2D : public efx2D
{
  private:
    int vertical_rows;  // indica l'altezza virtuale in pixel
	                    // di tutte le frasi

	int screen_y1, screen_y2, interline, interchar;
	int render_type, alignement;
	PHRASE_PTR frasi[MAX_PHRASE];
	int num_frasi;
	float stop;

	void put_phrase_line_overwrite (int y, int yy, PHRASE_PTR phrase);
	void put_phrase_line_add (int y, int yy, PHRASE_PTR phrase);
	void put_phrase_line_blend50 (int y, int yy, PHRASE_PTR phrase);
	
  public:

	TextScroller2D();
	void set_parameters(int y1, int y2, int interlinea, int intercarattere,
		                int rendertype, int allineamento, float _stop);
	void add_phrase (char *s, int color);
	void init();
	void do_efx2D(double pos);
};

#endif