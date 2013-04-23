#define STRICT
#include <stdio.h>
#include "..\ADRender.h"

extern LPDIRECT3D9  g_pD3D;   // in ADrender.cpp
D3DDEVICEINFO g_Devices[3];

char D3DFormat2Bpp (D3DFORMAT format)
{
   switch (format)
   {
	  case D3DFMT_R5G6B5:
      case D3DFMT_X1R5G5B5: return(16);
		                    break;

      case D3DFMT_R8G8B8: return(24);
		                  break;

      case D3DFMT_A8R8G8B8:
      case D3DFMT_X8R8G8B8: return(32);
		                    break;
   }
}


int SortModesCallback (const VOID* arg1, const VOID* arg2)
{
    D3DDISPLAYMODE* p1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* p2 = (D3DDISPLAYMODE*)arg2;

    if( p1->Format > p2->Format )   return -1;
    if( p1->Format < p2->Format )   return +1;
    if( p1->Width  < p2->Width )    return -1;
    if( p1->Width  > p2->Width )    return +1;
    if( p1->Height < p2->Height )   return -1;
    if( p1->Height > p2->Height )   return +1;
    return 0;
}


int EnumerateDriver(D3DDEVICEINFO **tabdev)
{
   typedef char devDesc[4];
   unsigned int i, j, k;
   const DWORD dwNumDeviceTypes = 1;
   devDesc strDeviceDescs[2] = { "HAL", "REF" };
   int deviceIndex = 0;
   const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

   g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
   D3DDISPLAYMODE modes[100];
   D3DFORMAT      formats[20];
   DWORD dwNumFormats      = 0;
   DWORD dwNumModes        = 0;
   DWORD dwNumAdapterModes = g_pD3D->GetAdapterModeCount(0, D3DFMT_X8R8G8B8);
   DWORD dwNumDevices=0;

   for(i=0; i < dwNumAdapterModes; i++)
   {
      // Get the display mode attributes
      D3DDISPLAYMODE DisplayMode;
      g_pD3D->EnumAdapterModes(0, D3DFMT_X8R8G8B8, i, &DisplayMode);

      // Check if the mode already exists (to filter out refresh rates)
      for(j=0; j<dwNumModes; j++)
      {
         if(( modes[j].Width  == DisplayMode.Width  ) &&
            ( modes[j].Height == DisplayMode.Height ) &&
            ( modes[j].Format == DisplayMode.Format ))
                    break;
	  }
      // If we found a new mode, add it to the list of modes
      if (j == dwNumModes)
      {
         modes[dwNumModes].Width       = DisplayMode.Width;
         modes[dwNumModes].Height      = DisplayMode.Height;
         modes[dwNumModes].Format      = DisplayMode.Format;
         modes[dwNumModes].RefreshRate = 0;
         dwNumModes++;

         // Check if the mode's format already exists
         for(k=0; k<dwNumFormats; k++)
         {
            if (DisplayMode.Format == formats[k]) break;
		 }
         // If the format is new, add it to the list
         if(k==dwNumFormats) formats[dwNumFormats++] = DisplayMode.Format;
	  }
   }
   // Sort the list of display modes (by format, then width, then height)
   qsort(modes, dwNumModes, sizeof(D3DDISPLAYMODE), SortModesCallback);

   // Fill in device info
   D3DDEVICEINFO *pDevice;
   pDevice  = &g_Devices[0];
   pDevice->DeviceType = DeviceTypes[deviceIndex];
   strcpy(pDevice->strDesc, strDeviceDescs[deviceIndex]);

   // se non ci sono devices REF ritorna errore
   /*if (g_pD3D->GetDeviceCaps(0, D3DDEVTYPE_REF, &pDevice->d3dCaps)!=D3D_OK)
	    return(-1);*/


   // Add all enumerated display modes with confirmed formats to the
   // device's list of valid modes
   for(DWORD m=0L; m<dwNumModes; m++)
   {
         for( DWORD f=0; f<dwNumFormats; f++)
         {
             if (modes[m].Format == formats[f])
             {
                   // Add this mode to the device's list of valid modes
                   pDevice->SupportedModes[pDevice->dwNumModes].Width      = modes[m].Width;
                   pDevice->SupportedModes[pDevice->dwNumModes].Height     = modes[m].Height;
                   pDevice->SupportedModes[pDevice->dwNumModes].Format     = modes[m].Format;
                   pDevice->SupportedModes[pDevice->dwNumModes].Bpp        = D3DFormat2Bpp(modes[m].Format);
                   pDevice->SupportedModes[pDevice->dwNumModes].dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
                   //pDevice->SupportedModes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
                   pDevice->dwNumModes++;
              }
         }
   }
   pDevice->bCanDoWindowed = TRUE;
   if (pDevice->dwNumModes > 0) dwNumDevices++;

   if(0 == dwNumDevices)
   {
	  *tabdev=NULL;
      return -1;
   }
   else
   {
	 *tabdev=g_Devices;
	 return(dwNumDevices);
   }
}