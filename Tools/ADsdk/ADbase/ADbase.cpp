// ADbase.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ADbase.h"

HANDLE hmodulo;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	hmodulo=hModule;
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

/*
// This is an example of an exported variable
ADBASE_API int nADbase=0;

// This is an example of an exported function.
ADBASE_API int fnADbase(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see ADbase.h for the class definition
CADbase::CADbase()
{ 
	return; 
}
*/
