#include <windows.h>
#include <stdio.h>
#include <d3d9.h>
#include "adbase.h"

void debug_error_lib(HWND hWnd, char *messaggio)
{
  FILE *fd;

  fd=fopen("debug_error.txt", "wt");
  fprintf(fd, "Debug_Error: %s\n", messaggio);
  fflush(fd);
  fclose(fd);
  MessageBox(hWnd, messaggio,"AD-Debug", NULL); 
  exit(0);
}


void memsetd_lib(void *dest, unsigned int val, int dwords) 
{
	_asm
	{
		    push edi
			push esi
			push ecx
			push eax

			mov edi, [dest]
			mov ecx, [dwords]

			shr ecx, 1
			mov eax, [val]

            lea esi, [edi+ecx*8]
			neg ecx

ciclo:
			mov [esi+ecx*8], eax
			mov [esi+ecx*8+4], eax

			inc ecx
			jnz ciclo

			pop eax
			pop ecx
			pop esi
			pop edi
		}
}


/*
void memsetd(void *dest,unsigned int val,int dwords) 
{
	_asm
	{

			mov edi, dest
			push ecx

			push eax
			mov eax,val

			mov ecx, dwords
            rep stosd

			pop eax
			pop ecx
		}
}
*/


void memcpyd_lib(void *dest, void *sorg, int dwords) 
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


void check_dx_lib(HWND hWnd, HRESULT hRet, char *stringa)
{ 
	char *err;
	char message[160];

	if (hRet != D3D_OK) 
    {
	   switch (hRet)
	   {
	      case D3DERR_WRONGTEXTUREFORMAT:
          err="D3DERR_WRONGTEXTUREFORMAT";
		  break;

          case D3DERR_UNSUPPORTEDCOLOROPERATION:
          err="D3DERR_UNSUPPORTEDCOLOROPERATION";
		  break;

          case D3DERR_UNSUPPORTEDCOLORARG:
          err="D3DERR_UNSUPPORTEDCOLORARG";
		  break;

          case D3DERR_UNSUPPORTEDALPHAOPERATION:
          err="D3DERR_UNSUPPORTEDALPHAOPERATION";
		  break;

          case D3DERR_UNSUPPORTEDALPHAARG:
          err="D3DERR_UNSUPPORTEDALPHAARG";
		  break;

          case D3DERR_TOOMANYOPERATIONS:
          err="D3DERR_TOOMANYOPERATIONS";
		  break;

          case D3DERR_CONFLICTINGTEXTUREFILTER:
          err="D3DERR_CONFLICTINGTEXTUREFILTER";
		  break;

          case D3DERR_UNSUPPORTEDFACTORVALUE:
          err="D3DERR_UNSUPPORTEDFACTORVALUE";
		  break;

          case D3DERR_CONFLICTINGRENDERSTATE:
          err="D3DERR_CONFLICTINGRENDERSTATE";
		  break;

          case D3DERR_UNSUPPORTEDTEXTUREFILTER:
          err="D3DERR_UNSUPPORTEDTEXTUREFILTER";
		  break;

          case D3DERR_CONFLICTINGTEXTUREPALETTE:
          err="D3DERR_CONFLICTINGTEXTUREPALETTE";
		  break;

          case D3DERR_DRIVERINTERNALERROR:
          err="D3DERR_DRIVERINTERNALERROR";
		  break;

          case D3DERR_NOTFOUND:
          err="D3DERR_NOTFOUND";
		  break;

          case D3DERR_MOREDATA:
          err="D3DERR_MOREDATA";
		  break;

          case D3DERR_DEVICELOST:
          err="D3DERR_DEVICELOST";
		  break;

          case D3DERR_DEVICENOTRESET:
          err="D3DERR_DEVICENOTRESET";
		  break;

          case D3DERR_NOTAVAILABLE:
          err="D3DERR_NOTAVAILABLE";
		  break;

          case D3DERR_OUTOFVIDEOMEMORY:
          err="D3DERR_OUTOFVIDEOMEMORY";
		  break;

          case D3DERR_INVALIDDEVICE:
          err="D3DERR_INVALIDDEVICE";
		  break;

          case D3DERR_INVALIDCALL:
          err="D3DERR_INVALIDCALL";
		  break;

          case D3DERR_DRIVERINVALIDCALL:
          err="D3DERR_DRIVERINVALIDCALL";
		  break;
			  
	      default: err="? UNKNOWN DX ERROR";
	   }
	   message[0]=0;
	   strcat(message,"AD-DX Error: ");
	   strcat(message,stringa);
	   strcat(message," - ");
	   strcat(message,err);
	   debug_error_lib(hWnd, message);
	}
}