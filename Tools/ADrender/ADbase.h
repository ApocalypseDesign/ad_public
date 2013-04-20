// Funzioni di base condivise

#ifndef _ADBASE_H_
#define _ADBASE_H_
#include <windows.h>

void debug_error_lib(HWND hWnd,char *messaggio);
void memsetd_lib(void *dest,unsigned int val,int dwords);
void memcpyd_lib(void *dest,void *sorg,int dwords);
void check_dx_lib(HWND hWnd, HRESULT hRet, char *stringa);

#endif