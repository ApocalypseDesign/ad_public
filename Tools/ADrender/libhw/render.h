#ifndef _RENDER_H_
#define _RENDER_H_

#include <d3d8.h>

typedef struct
{
  float x, y, z, w;
  D3DCOLOR ARGB;
  D3DCOLOR specular;
  float u, v;
} TEX1RGBVERTEX;

#define TEX1RGBVERTEX_FORMAT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)

/*
typedef struct
{
  float x, y, z, w;
  float u, v;
  float envu, envv;
} TEX2VERTEX;
#define TEX2VERTEX_FORMAT (D3DFVF_XYZRHW | D3DFVF_TEX2)
*/

typedef struct
{
  float x, y, z, w;
  D3DCOLOR ARGB;
  float u, v;
  float envu, envv;
} TEX2RGBVERTEX;

#define TEX2RGBVERTEX_FORMAT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2)


#endif