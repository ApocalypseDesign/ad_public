// classe di base per gli effetti 2D v2001.1
// by HereBit


#ifndef _EFX2D_H_
#define _EFX2D_H_

#ifdef ADBASE_EXPORTS
#define ADBASE_API __declspec(dllexport)
#else
#define ADBASE_API __declspec(dllimport)
#endif

#include "image.h"
#include "ADcontrol.h"
#include "ptrlist.h"
#include "ctsystem.h"

class ADBASE_API efx2D
{  
public:

	char className[32];
	bool initialized;
	ptrList ct,channels,channelimages,images;

   efx2D(void);
   virtual void *create();										// da overloadare
   virtual void defineControllers(tChannel *defaultChannel);	// da overloadare
   virtual void updateControllers();							// da overloadare
   virtual void loadFromMemory(void *data,int size);			// da overloadare
   virtual void saveToMemory(void **data,int *size);			// da overloadare
   virtual void freeSavedMemory();								// da overloadare
   virtual void edit(HWND mainwin);								// da overloadare
   virtual void init();											// da overloadare
   virtual void free();											// da overloadare
   void onPaint(double pos);
   virtual void paint(double pos);								// da overloadare
   virtual ~efx2D(void);

private:
	ctBool *myActive;

protected:
	void addChannel(tChannel *defaultChannel,char *name, char *desc);
	void addImage(char *name,char *desc);
	void addProperty(void *defaultCt,char *name,char *desc);
	image *getChannelImage(char *name);
	image *getImage(char *name);
	void  *getProperty(char *name);
};

#endif