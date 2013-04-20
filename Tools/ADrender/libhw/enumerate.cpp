#define STRICT
#include <stdio.h>
#include "..\ADRender.h"

extern LPDIRECT3D8  g_pD3D;   // in ADrender.cpp
D3DDEVICEINFO g_Devices[5];

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


BOOL FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType,
    D3DFORMAT TargetFormat, D3DFORMAT *pDepthStencilFormat)
{
	// prima lo cerco a 32bit, poi a 24 e infine a 16...e se non
	// c'è vaffanculo alla tua scheda grafica.

    if (SUCCEEDED(g_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
                  TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)))
    {
       if (SUCCEEDED(g_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32)))
       {
           *pDepthStencilFormat = D3DFMT_D32;
           return TRUE;
       }
    }

    
	if (SUCCEEDED(g_pD3D->CheckDeviceFormat( iAdapter, DeviceType,
                  TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8)))
    {
       if (SUCCEEDED(g_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
                     TargetFormat, TargetFormat, D3DFMT_D24X8 )))
       {
           *pDepthStencilFormat = D3DFMT_D24X8;
           return TRUE;
       }
    }

    
	if(SUCCEEDED(g_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
                 TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
    {
       if(SUCCEEDED(g_pD3D->CheckDepthStencilMatch(iAdapter, DeviceType,
                    TargetFormat, TargetFormat, D3DFMT_D24S8)))
       {
           *pDepthStencilFormat = D3DFMT_D24S8;
           return TRUE;
       }
    }
    
	
	if (SUCCEEDED(g_pD3D->CheckDeviceFormat(iAdapter, DeviceType,
                  TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
    {
       if (SUCCEEDED(g_pD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                     TargetFormat, TargetFormat, D3DFMT_D16)))
       {
           *pDepthStencilFormat = D3DFMT_D16;
            return TRUE;
       }
    }
    return FALSE;
}


int EnumerateDriver(D3DDEVICEINFO **tabdev)
{
   typedef char devDesc[4];
   unsigned int i, j, k;
   const DWORD dwNumDeviceTypes = 1;
   devDesc strDeviceDescs[2] = { "HAL", "REF" };
   const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };
   BOOL bHALExists = FALSE;
   BOOL bHALIsWindowedCompatible = FALSE;
   BOOL bHALIsDesktopCompatible = FALSE;
   BOOL bHALIsSampleCompatible = FALSE;

   g_pD3D = Direct3DCreate8(D3D_SDK_VERSION);
   D3DDISPLAYMODE modes[100];
   D3DFORMAT      formats[20];
   DWORD dwNumFormats      = 0;
   DWORD dwNumModes        = 0;
   DWORD dwNumAdapterModes = g_pD3D->GetAdapterModeCount(0);
   DWORD dwNumDevices;

   for(i=0; i < dwNumAdapterModes; i++)
   {
      // Get the display mode attributes
      D3DDISPLAYMODE DisplayMode;
      g_pD3D->EnumAdapterModes(0, i, &DisplayMode);

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

   // Add devices to adapter
   dwNumDevices=0;
   for(i=0; i<dwNumDeviceTypes; i++)
   {
     // Fill in device info
     D3DDEVICEINFO *pDevice;
     pDevice  = &g_Devices[dwNumDevices];
	 pDevice->DeviceType = DeviceTypes[i];
	 // se non ci sono devices HAL o T&L ritorna errore
	 if (g_pD3D->GetDeviceCaps(0, D3DDEVTYPE_HAL, &pDevice->d3dCaps)!=D3D_OK)
	    return(-1);

     strcpy(pDevice->strDesc, strDeviceDescs[i]);
     pDevice->dwNumModes     = 0;
     pDevice->bCanDoWindowed = FALSE;
     pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

     // Examine each format supported by the adapter to see if it will
     // work with this device and meets the needs of the application.
     BOOL  bFormatConfirmed[20];
     DWORD dwBehavior[20];
     D3DFORMAT fmtDepthStencil[20];

     for(DWORD f=0; f<dwNumFormats; f++)
     {
        bFormatConfirmed[f] = FALSE;
        fmtDepthStencil[f] = D3DFMT_UNKNOWN;
        // Skip formats that cannot be used as render targets on this device
        if( FAILED(g_pD3D->CheckDeviceType(0, pDevice->DeviceType,
                                            formats[f], formats[f], FALSE )))
        continue;

        if(pDevice->DeviceType == D3DDEVTYPE_HAL )
        {
           // This system has a HAL device
           bHALExists = TRUE;
           if(pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED)
           {
               // HAL can run in a window for some mode
               bHALIsWindowedCompatible = TRUE;
			   /*
               if(f == 0)
               {
                   // HAL can run in a window for the current desktop mode
                   bHALIsDesktopCompatible = TRUE;
               }
			   */
           }
		}

       // Confirm the device/format for HW vertex processing
       if (pDevice->d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
       {
          if (pDevice->d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
          {
              dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
                              D3DCREATE_PUREDEVICE;
              bFormatConfirmed[f] = TRUE;
          }
          if (FALSE == bFormatConfirmed[f])
          {
             dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;
             bFormatConfirmed[f] = TRUE;
          }
          if (FALSE == bFormatConfirmed[f])
          {
             dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;
             bFormatConfirmed[f] = TRUE;
          }
	   }
       // Confirm the device/format for SW vertex processing
       if(bFormatConfirmed[f]==FALSE)
       {
         dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
         bFormatConfirmed[f] = TRUE;
       }

       // Find a suitable depth/stencil buffer format for this device/format
       if(bFormatConfirmed[f])
       {
           if(!FindDepthStencilFormat(0, pDevice->DeviceType,
                         formats[f], &fmtDepthStencil[f]))
           {
              bFormatConfirmed[f] = FALSE;
		   }
       }
	 }

     // Add all enumerated display modes with confirmed formats to the
     // device's list of valid modes
     for(DWORD m=0L; m<dwNumModes; m++)
     {
         for( DWORD f=0; f<dwNumFormats; f++)
         {
             if (modes[m].Format == formats[f])
             {
                 if(bFormatConfirmed[f] == TRUE)
                 {
                   // Add this mode to the device's list of valid modes
                   pDevice->SupportedModes[pDevice->dwNumModes].Width      = modes[m].Width;
                   pDevice->SupportedModes[pDevice->dwNumModes].Height     = modes[m].Height;
                   pDevice->SupportedModes[pDevice->dwNumModes].Format     = modes[m].Format;
                   pDevice->SupportedModes[pDevice->dwNumModes].Bpp        = D3DFormat2Bpp(modes[m].Format);
                   pDevice->SupportedModes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
                   pDevice->SupportedModes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];
                   pDevice->dwNumModes++;

                   if(pDevice->DeviceType == D3DDEVTYPE_HAL)
                                bHALIsSampleCompatible = TRUE;
                 }
              }
         }
	 }
     if (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) pDevice->bCanDoWindowed = TRUE;
     if (pDevice->dwNumModes > 0) dwNumDevices++;
   }

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