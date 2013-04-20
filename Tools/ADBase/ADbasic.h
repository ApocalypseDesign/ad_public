// Funzioni di base condivise

#ifndef _ADBASE_H_
#define _ADBASE_H_

#ifdef ADBASE_EXPORTS
#define ADBASE_API __declspec(dllexport)
#else
#define ADBASE_API __declspec(dllimport)
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

ADBASE_API void debug_warning(HWND hWnd,char *messaggio);
ADBASE_API void debug_error(HWND hWnd,char *messaggio);
ADBASE_API void memsetd(void *dest,unsigned int val,int dwords);
ADBASE_API void memcpyd(void *dest,void *sorg,int dwords);

#endif