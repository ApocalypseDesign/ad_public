// Utility per gli script dei materiali (.ADM), luci (.ADL),
// oggetti e patch (.ADO), dummy (.ADD)
// Made by [AD]Turbo
// Questa e' la versione per l'engine raytracing!


#ifndef _RAYTUTILSSCRIPT_H_
#define _RAYTUTILSSCRIPT_H_

#include "rayt_scene.h"


// ----------------------------------------------
// procedure proprie per gli script dei materiali
// ----------------------------------------------
int rayt_load_ADM (char *nfile, AD_RaytScene *s);


// ----------------------------------------------
// procedure proprie per gli script delle luci
// ----------------------------------------------
int rayt_load_ADL (char *nfile, AD_RaytScene *s);


#endif
