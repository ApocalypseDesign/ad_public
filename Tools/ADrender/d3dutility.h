#ifndef _D3DUTILITY_H_
#define _D3DUTILITY_H_

#include "Adrender.h"

BOOL is_texture_pow2 (texture *tex);
int CopyRGBTetxureToSurface (texture *t, LPDIRECT3DTEXTURE8 s);
int CopyARGBTetxureToSurface (texture *t, LPDIRECT3DTEXTURE8 s);

#endif