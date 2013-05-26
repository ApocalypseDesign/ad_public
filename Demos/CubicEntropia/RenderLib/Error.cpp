#include "RenderLib.h"

void CRenderLib::m_Error(char8 *msg)
{
  FILE *fd;

  fd=fopen("debug_error.txt", "wt");
  fprintf(fd, "Debug_Error: %s\n", msg);
  fflush(fd);
  fclose(fd);
  MessageBox(hWindow, msg, "AD-Debug", NULL); 
  exit(0);
}

//--------------------------------------------------------------------

void CRenderLib::m_D3DError(HRESULT hRet, char8 *msg)
{
	char8 *err;
	char8 message[200];

	if (hRet != D3D_OK) 
    {
	   switch (hRet)
	   {
	      case D3DERR_WRONGTEXTUREFORMAT:
            err="D3DERR_WRONGTEXTUREFORMAT"; break;
          case D3DERR_UNSUPPORTEDCOLOROPERATION:
            err="D3DERR_UNSUPPORTEDCOLOROPERATION"; break;
          case D3DERR_UNSUPPORTEDCOLORARG:
            err="D3DERR_UNSUPPORTEDCOLORARG"; break;
          case D3DERR_UNSUPPORTEDALPHAOPERATION:
            err="D3DERR_UNSUPPORTEDALPHAOPERATION"; break;
          case D3DERR_UNSUPPORTEDALPHAARG:
            err="D3DERR_UNSUPPORTEDALPHAARG"; break;
          case D3DERR_TOOMANYOPERATIONS:
            err="D3DERR_TOOMANYOPERATIONS"; break;
          case D3DERR_CONFLICTINGTEXTUREFILTER:
            err="D3DERR_CONFLICTINGTEXTUREFILTER"; break;
          case D3DERR_UNSUPPORTEDFACTORVALUE:
            err="D3DERR_UNSUPPORTEDFACTORVALUE"; break;
          case D3DERR_CONFLICTINGRENDERSTATE:
            err="D3DERR_CONFLICTINGRENDERSTATE"; break;
          case D3DERR_UNSUPPORTEDTEXTUREFILTER:
            err="D3DERR_UNSUPPORTEDTEXTUREFILTER"; break;
          case D3DERR_CONFLICTINGTEXTUREPALETTE:
            err="D3DERR_CONFLICTINGTEXTUREPALETTE"; break;
          case D3DERR_DRIVERINTERNALERROR:
            err="D3DERR_DRIVERINTERNALERROR"; break;
          case D3DERR_NOTFOUND:
            err="D3DERR_NOTFOUND"; break;
          case D3DERR_MOREDATA:
            err="D3DERR_MOREDATA"; break;
          case D3DERR_DEVICELOST:
            err="D3DERR_DEVICELOST"; break;
          case D3DERR_DEVICENOTRESET:
            err="D3DERR_DEVICENOTRESET"; break;
          case D3DERR_NOTAVAILABLE:
            err="D3DERR_NOTAVAILABLE"; break;
          case D3DERR_OUTOFVIDEOMEMORY:
            err="D3DERR_OUTOFVIDEOMEMORY"; break;
          case D3DERR_INVALIDDEVICE:
            err="D3DERR_INVALIDDEVICE"; break;
          case D3DERR_INVALIDCALL:
            err="D3DERR_INVALIDCALL"; break;
          case D3DERR_DRIVERINVALIDCALL:
            err="D3DERR_DRIVERINVALIDCALL"; break;
	      default: err="? UNKNOWN DX ERROR";
	   }
	   message[0]='\0';
	   strcat(message, "AD-RenderLib Error: ");
	   strcat(message, msg);
	   strcat(message, " - ");
	   strcat(message, err);
	   m_Error(message);
	}
}