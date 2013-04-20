#ifndef _ADEFX_H_
#define _ADEFX_H_

//****Includes**************
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//**************************

//****Gestione import/export dalla DLL****
#ifdef ADEFX_EXPORTS
#define ADEFX_API __declspec(dllexport)
#else
#define ADEFX_API __declspec(dllimport)
#endif
//****************************************


ADEFX_API void __stdcall ADEFX_GetInfo(char **name,char **desc);
ADEFX_API void * __stdcall ADEFX_Create();


#endif

