#ifndef _VIDEO_H_
#define _VIDEO_H_

#include "Types.h"

class CVideoReader
{
private:
   int32 p_VideoIsOpened;
   int32 p_VideoInfoAlreadyRead;

public:
   CVideoReader();
   int32 m_OpenVideo(char8 *filename);
   int32 m_GetVideoInfo (int32 *width, int32 *height, int32 *bpp, float4 *num_seconds);
   int32 m_GetFrame (float4 secondNumber, void *buffer);
   int32 m_CloseVideo(void);
};

#endif