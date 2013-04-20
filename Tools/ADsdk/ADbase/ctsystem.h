#ifndef _CTSYSTEM_H_
#define _CTSYSTEM_H_

#ifdef ADBASE_EXPORTS
#define ADBASE_API __declspec(dllexport)
#else
#define ADBASE_API __declspec(dllimport)
#endif

#include "ptrlist.h"
#include "image.h"


class ADBASE_API tChannel
{
public:
	ptrList level;
	image *chImage;
};

class ADBASE_API tSystem
{
public:
	int width,height;
	ptrList clevel,ccontrol,channel,images;
};

#endif