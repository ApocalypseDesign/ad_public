
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ADBASE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ADBASE_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef _ADBASE_H_
#define _ADBASE_H_

#ifdef ADBASE_EXPORTS
#define ADBASE_API __declspec(dllexport)
#else
#define ADBASE_API __declspec(dllimport)
#endif



#include "ADbasic.h"
#include "ptrlist.h"
#include "filebuf.h"
#include "image.h"
#include "efx2D.h"
#include "ADcontrol.h"

extern HANDLE hmodulo;

#endif