// Definizione delle dipendenze esterne per le variabili
// globali usate dal render; tutte queste variabili si trovano
// nel file GLOBREND.CPP

#ifndef _EXTREND_H_
#define _EXTREND_H_

#include "render.h"

extern TEX1RGBVERTEX *HW_texrgb;
extern TEX1RGBVERTEX *HW_texrgb_arrays[2];

extern TEX2RGBVERTEX    *HW_texenv;
extern TEX2RGBVERTEX    *HW_texenv_arrays[2];

extern int HW_numvert;

#endif