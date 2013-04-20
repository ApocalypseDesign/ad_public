#include "ADbasic.h"
#include <stdlib.h>

ADBASE_API void debug_warning(HWND hWnd,char *messaggio)
{
	MessageBox(hWnd,messaggio,"AD-Debug Warning",NULL); 
}

ADBASE_API void debug_error(HWND hWnd,char *messaggio)
{
	MessageBox(hWnd,messaggio,"AD-Debug Error",NULL); 
	exit(0);
}

ADBASE_API void memsetd(void *dest,unsigned int val,int dwords) 
{
	_asm{
			push edi
			push ecx
			push eax
			mov edi,dest
			mov eax,val
			mov ecx,dwords
			rep stosd
			pop eax
			pop ecx
			pop edi
		}
}

ADBASE_API void memcpyd(void *dest,void *sorg,int dwords) 
{
	_asm{
			push edi
			push ecx
			push esi
			mov edi,dest
			mov esi,sorg
			mov ecx,dwords
			rep movsd
			pop esi
			pop ecx
			pop edi
		}
}

