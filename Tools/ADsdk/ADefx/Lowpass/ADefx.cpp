#include "ADefx.h"
#include "myefx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

ADEFX_API void __stdcall ADEFX_GetInfo(char **name, char **desc)
{
	*name=new char [strlen(MYEFX_NAME)+1];
	*desc=new char [strlen(MYEFX_DESC)+1];
	strcpy(*name,MYEFX_NAME);
	strcpy(*desc,MYEFX_DESC);
}


ADEFX_API void * __stdcall ADEFX_Create() 
{
   return (void *)new myefx;
}
