#include "RenderLib.h"

int32 CRenderLib::m_CreateDevice(int32 type)
{
   HRESULT hr;

   p_D3D = Direct3DCreate8(D3D_SDK_VERSION);
   if (type==HARDWARE_DEVICE) p_DeviceType=D3DDEVTYPE_HAL;
   else
   if (type==SOFTWARE_DEVICE) p_DeviceType=D3DDEVTYPE_REF;

   hr=p_D3D->GetDeviceCaps(D3DADAPTER_DEFAULT, p_DeviceType, &p_DeviceInfo.Caps);
   if (hr != D3D_OK) return(0);

   // ----------   il device puo' disegnare in finestra   ------------
   if (p_DeviceInfo.Caps.Caps2 & D3DCAPS2_CANRENDERWINDOWED)
      p_DeviceInfo.CanDoWindowed=TRUE;
   else p_DeviceInfo.CanDoWindowed=FALSE;

   if (type==SOFTWARE_DEVICE)
   {
      p_DeviceInfo.Behavior=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
      p_VSHardwareSupport=0;
   }
   else
   {
     // -----------  T&L e capacità di processo dei vertici  -----------
     if (p_DeviceInfo.Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	 {
        if (p_DeviceInfo.Caps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
		{
           // if (p_DeviceInfo.Caps.DevCaps & D3DDEVCAPS_PUREDEVICE)
           p_DeviceInfo.Behavior=D3DCREATE_HARDWARE_VERTEXPROCESSING;
			                   //| D3DCREATE_PUREDEVICE;
           p_VSHardwareSupport=(1<<16) | (1);
		}
        else
		{
	       p_DeviceInfo.Behavior=D3DCREATE_MIXED_VERTEXPROCESSING;
           p_VSHardwareSupport=0;
		}
	 }
     else
	 {
	    p_DeviceInfo.Behavior=D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		p_VSHardwareSupport=0;
	 }

   }
   p_IsDeviceCreated=1;
   return(1);
}

//--------------------------------------------------------------------

BOOL CRenderLib::FindDepthStencilFormat(UINT iAdapter,
                                        D3DDEVTYPE DeviceType,
                                        D3DFORMAT TargetFormat,
                                        D3DFORMAT *pDepthStencilFormat)
{
    if (!p_IsDeviceCreated)
       return FALSE;

	// prima lo cerco a 32bit, poi a 24 e infine a 16...e se non
	// c'è vaffanculo alla tua scheda grafica.
    if (SUCCEEDED(p_D3D->CheckDeviceFormat(iAdapter, DeviceType,
                  TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)))
    {
       if (SUCCEEDED(p_D3D->CheckDepthStencilMatch(iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32)))
       {
           *pDepthStencilFormat = D3DFMT_D32;
           return TRUE;
       }
    }

    
	if (SUCCEEDED(p_D3D->CheckDeviceFormat( iAdapter, DeviceType,
                  TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8)))
    {
       if (SUCCEEDED(p_D3D->CheckDepthStencilMatch(iAdapter, DeviceType,
                     TargetFormat, TargetFormat, D3DFMT_D24X8 )))
       {
           *pDepthStencilFormat = D3DFMT_D24X8;
           return TRUE;
       }
    }

    
	if(SUCCEEDED(p_D3D->CheckDeviceFormat(iAdapter, DeviceType,
                 TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8)))
    {
       if(SUCCEEDED(p_D3D->CheckDepthStencilMatch(iAdapter, DeviceType,
                    TargetFormat, TargetFormat, D3DFMT_D24S8)))
       {
           *pDepthStencilFormat = D3DFMT_D24S8;
           return TRUE;
       }
    }
    
	
	if (SUCCEEDED(p_D3D->CheckDeviceFormat(iAdapter, DeviceType,
                  TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
    {
       if (SUCCEEDED(p_D3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                     TargetFormat, TargetFormat, D3DFMT_D16)))
       {
           *pDepthStencilFormat = D3DFMT_D16;
            return TRUE;
       }
    }
    return FALSE;
}

//--------------------------------------------------------------------

int SortModesCallback (const VOID *arg1, const VOID *arg2)
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


int32 CRenderLib::m_EnumerateScreenModes (void)
{
  uint32 i, j, k;
  DWORD NumAdapterModes;
  D3DDISPLAYMODE DisplayMode;
  D3DDISPLAYMODE Modes[100];
  DWORD NumModes;
  D3DFORMAT Formats[20];
  D3DFORMAT DepthStencilFormats[20];
  BOOL FormatConfirmed[20];
  DWORD NumFormats;
	 
  if (!p_IsDeviceCreated) return(0);

  NumAdapterModes=p_D3D->GetAdapterModeCount(D3DADAPTER_DEFAULT);
  NumModes=0;
  NumFormats=0;
  for(i=0; i<NumAdapterModes; i++)
  {
     p_D3D->EnumAdapterModes(D3DADAPTER_DEFAULT, i, &DisplayMode);
     for(j=0; j<NumModes; j++)
	 {
       if ((Modes[j].Width==DisplayMode.Width) &&
           (Modes[j].Height==DisplayMode.Height) &&
           (Modes[j].Format==DisplayMode.Format))
          break;
	 }
     if (j==NumModes)
     {
        Modes[NumModes].Width=DisplayMode.Width;
        Modes[NumModes].Height=DisplayMode.Height;
        Modes[NumModes].Format=DisplayMode.Format;
        Modes[NumModes].RefreshRate = 0;
        NumModes++;

        // Check if the mode's format already exists
        for(k=0; k<NumFormats; k++)
        {
           if (DisplayMode.Format==Formats[k]) break;
		}
        // If the format is new, add it to the list
        if(k==NumFormats)
		  Formats[NumFormats++]=DisplayMode.Format;
	 }
  }
  // Sort the list of display modes (by format, then width, then height)
  qsort(Modes, NumModes, sizeof(D3DDISPLAYMODE), SortModesCallback);

  // uso i soli formati che possono usati come target di un rendering
  for(i=0; i<NumFormats; i++)
  {
     FormatConfirmed[i]=FALSE;
     // Skip formats that cannot be used as render targets on this device
     if (FAILED(p_D3D->CheckDeviceType(D3DADAPTER_DEFAULT, p_DeviceType,
                                       Formats[i], Formats[i], FALSE)))
     continue;

     // Find a suitable depth/stencil buffer format for this device/format
     if (!FindDepthStencilFormat(D3DADAPTER_DEFAULT, p_DeviceType,
                                 Formats[i], &DepthStencilFormats[i]))
       FormatConfirmed[i] = FALSE;
	 else FormatConfirmed[i]=TRUE;
  }

  // espongo i modi video trovati, mettendo solo quelli con un format
  // video confermato dalla verifica appena compiuta
  p_DeviceInfo.NumSupportedModes=0;
  for(i=0; i<NumModes; i++)
    for(j=0; j<NumFormats; j++)
    {
      if ((Modes[i].Format==Formats[j]) && (FormatConfirmed[j]))
	  {
        // Add this mode to the device's list of valid modes
        p_DeviceInfo.SupportedModes[p_DeviceInfo.NumSupportedModes].Width=Modes[i].Width;
        p_DeviceInfo.SupportedModes[p_DeviceInfo.NumSupportedModes].Height=Modes[i].Height;
        p_DeviceInfo.SupportedModes[p_DeviceInfo.NumSupportedModes].Format=Modes[i].Format;
        p_DeviceInfo.SupportedModes[p_DeviceInfo.NumSupportedModes].Bpp=D3DFormat2Bpp(Modes[i].Format);
        p_DeviceInfo.SupportedModes[p_DeviceInfo.NumSupportedModes].Behavior=p_DeviceInfo.Behavior;
        p_DeviceInfo.SupportedModes[p_DeviceInfo.NumSupportedModes].DepthStencilFormat = DepthStencilFormats[j];
        p_DeviceInfo.NumSupportedModes++;
	  }
    }

  p_IsEnumerationDone=1;
  return(NumModes);
}

//--------------------------------------------------------------------

D3DModeInfo *CRenderLib::m_GetScreenMode(int32 index)
{
   if (index<0)
     return (D3DModeInfo *)NULL;

   if (!p_IsEnumerationDone)
     return (D3DModeInfo *)NULL;

   if ((p_DeviceInfo.NumSupportedModes<=0) ||
       (p_DeviceInfo.NumSupportedModes<=index))
     return (D3DModeInfo *)NULL;

   return (&p_DeviceInfo.SupportedModes[index]);
}

//--------------------------------------------------------------------

D3DModeInfo *CRenderLib::m_GetScreenMode(int32 width, int32 height, int32 bpp)
{
   int32 i;

   if (!p_IsEnumerationDone)
     return (D3DModeInfo *)NULL;

   if (p_DeviceInfo.NumSupportedModes<=0)
     return (D3DModeInfo *)NULL;

   for (i=0; i<p_DeviceInfo.NumSupportedModes; i++)
   {
      if ((p_DeviceInfo.SupportedModes[i].Width==width) &&
          (p_DeviceInfo.SupportedModes[i].Height==height) &&
          (p_DeviceInfo.SupportedModes[i].Bpp==bpp))
          return (&p_DeviceInfo.SupportedModes[i]);
   }
   return (D3DModeInfo *)NULL;
}