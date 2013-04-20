// inJpeg.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include "ijl.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


void __stdcall IMAGE_GetExtensions(char **ext1,char **ext2,char **ext3,char **ext4,char **ext5)
{
	*ext1=*new (char *)("JPG");
	*ext2=*new (char *)("JPEG");
	*ext3=*new (char *)("JFIF");
	*ext4=*new (char *)("JPE");
	*ext5=*new (char *)("JFI");
}

void __stdcall IMAGE_GetPluginInfo(char **name, char **desc)
{
	*name=*new (char *)("AD JPEG input plugin");
	*desc=*new (char *)("JPEG image");
}

/*
void IMAGE_LoadFromFile(const char *filename, void **data_out, void *size_out)
{
}
*/


void __stdcall IMAGE_LoadFromMemory(void *data_in, int size_in, void **data_out, int *size_out, int *width, int *height)
{
//----------------------------------------------------------
// An example using the Intel(R) JPEG Library:
// -- Decode image from a JFIF buffer.
//----------------------------------------------------------
int i;
unsigned char tp,*cp; 
BYTE* lpJpgBuffer=(BYTE *)data_in;
DWORD dwJpgBufferSize=size_in;
//DWORD* numberOfChannels;

BOOL bres;
IJLERR jerr;
DWORD dwWholeImageSize;
BYTE* lpTemp = NULL;
// Allocate the IJL JPEG_CORE_PROPERTIES structure.
JPEG_CORE_PROPERTIES jcprops;
bres = TRUE;
__try
{
// Initialize the Intel(R) JPEG Library.
jerr = ijlInit(&jcprops);
if(IJL_OK != jerr)
{
bres = FALSE;
__leave;
}
// Get information on the JPEG image
// (i.e., width, height, and channels).
jcprops.JPGFile = NULL;
jcprops.JPGBytes = lpJpgBuffer;
jcprops.JPGSizeBytes = dwJpgBufferSize;
jerr = ijlRead(&jcprops, IJL_JBUFF_READPARAMS);
if(IJL_OK != jerr)
{
bres = FALSE;
__leave;
}
// Set the JPG color space ... this will always be
// somewhat of an educated guess at best because JPEG
// is "color blind" (i.e., nothing in the bit stream
// tells you what color space the data was encoded from).
// However, in this example we assume that we are
// reading JFIF files which means that 3 channel images
// are in the YCbCr color space and 1 channel images are
// in the Y color space.
switch(jcprops.JPGChannels)
{
case 1:
{
jcprops.JPGColor = IJL_G;
jcprops.DIBColor = IJL_RGBA_FPX;
jcprops.DIBChannels = 4;
break;
}
case 3:
{
jcprops.JPGColor = IJL_YCBCR;
jcprops.DIBColor = IJL_RGBA_FPX;
jcprops.DIBChannels = 4;
break;
}
default:
{
// This catches everything else, but no
// color twist will be performed by the IJL.
jcprops.JPGColor = IJL_OTHER;
jcprops.DIBColor = IJL_OTHER;
jcprops.DIBChannels = jcprops.JPGChannels;
break;
}
}
// Compute size of desired pixel buffer.
*size_out = dwWholeImageSize = jcprops.JPGWidth * jcprops.JPGHeight * 
jcprops.DIBChannels;
// Allocate memory to hold the decompressed image data.
lpTemp = new BYTE [dwWholeImageSize];
if(NULL == lpTemp)
{
bres = FALSE;
__leave;
}
// Set up the info on the desired DIB properties.
jcprops.DIBWidth = jcprops.JPGWidth;
jcprops.DIBHeight = jcprops.JPGHeight;
jcprops.DIBPadBytes = 0;
jcprops.DIBBytes = lpTemp;
// Now get the actual JPEG image data into the pixel buffer.
jerr = ijlRead(&jcprops, IJL_JBUFF_READWHOLEIMAGE);
if(IJL_OK != jerr)
{
bres = FALSE;
__leave;
}
} // __try
__finally
{
//if(FALSE == bres)
//{
//if(NULL != lpTemp)
//{
//delete [] lpTemp;
//lpTemp = NULL;
//}
}
// Clean up the Intel(R) JPEG Library.
ijlFree(&jcprops);
*width = jcprops.DIBWidth;
*height = jcprops.DIBHeight;
//*numberOfChannels = jcprops.DIBChannels;
cp=(unsigned char *)lpTemp;

//ora converto da RGBA -> a -> BGRA
for (i=0; i<*width * *height *4 ; i+=4)
{
	tp=cp[i];
	cp[i]=cp[i+2];
	cp[i+2]=tp;
	//cp[i+3]=0; //setto l'alpha a zero
}
*data_out = lpTemp;
//} // __finally

 
}

