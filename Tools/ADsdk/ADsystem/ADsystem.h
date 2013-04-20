#ifndef _ADSYSTEM_H_
#define _ADSYSTEM_H_



//*** Includes *************
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "ADbase.h"
//**************************

//*** Gestione import/export dalla DLL ***
#ifdef ADSYSTEM_EXPORTS
#define ADSYSTEM_API //__declspec(dllexport)
#else
#define ADSYSTEM_API //__declspec(dllimport)
#endif


typedef void (CALLBACK* LP_IMAGE_LOADFROMMEMORY)(void *data_in, int size_in, void **data_out, int *size_out, int *width, int *height);
typedef void (CALLBACK* LP_IMAGE_GETEXTENSIONS)(char **ext1,char **ext2,char **ext3,char **ext4,char **ext5);
typedef void (CALLBACK* LP_IMAGE_GETPLUGININFO)(char **name, char **desc);

typedef struct
{
	int num;
	char *ext1,*ext2,*ext3,*ext4,*ext5;
	char *name,*desc;

    LP_IMAGE_GETEXTENSIONS IMAGE_GetExtensions;
	LP_IMAGE_GETPLUGININFO IMAGE_GetPluginInfo;
	LP_IMAGE_LOADFROMMEMORY IMAGE_LoadFromMemory;
 
} ADinPlug;


//*** FUNZIONI GENERALI *** (da usare in ADplayer e in Visual Adesso)

ADSYSTEM_API void  ADSYSTEM_Init(int width,int height);
//da richiamare solo all'inizio (specificare dimensione dei channel)
ADSYSTEM_API void  ADSYSTEM_Update();
//da richiamare prima del play
ADSYSTEM_API void  ADSYSTEM_LoadFromFile(char *filename);
//carica da un buffer
ADSYSTEM_API void  ADSYSTEM_SaveToFile(char *filename);
//salva in un buffer
//ADSYSTEM_API void  ADSYSTEM_LoadFromMemory(char *mem,int size);
//carica da un buffer
//ADSYSTEM_API void  ADSYSTEM_SaveToMemory(char **mem,int *size);
//salva in un buffer
ADSYSTEM_API void *ADSYSTEM_Paint(float curpos); 
//curpos va da 0.0 a 1.0, ritorna il puntatore al buffer da blittare
ADSYSTEM_API void  ADSYSTEM_Close();
//da richiamare solo alla fine
ADSYSTEM_API void  ADSYSTEM_Reset();
//dealloca tutta l'animazione e resetta
ADSYSTEM_API int   ADSYSTEM_ChunkWrite(char *ChunkName, void *data, int size);
//inserisce un chunk
ADSYSTEM_API int   ADSYSTEM_ChunkRead (char *ChunkName,void **data, int *size);
//legge un chunk

ADSYSTEM_API void ADSYSTEM_SoundLoad();
ADSYSTEM_API void ADSYSTEM_SoundPlay(int pos);
ADSYSTEM_API void ADSYSTEM_SoundStop();




//*** FUNZIONI DI EDITING *** (da usare in Visual Adesso)

//todo: selezione del canale da blittare e interfaccia con l'active
int curImage,curChannel,curCLevel,curCLevelControl,curCLevelChannel,curCLevelImage,
    curLevel,curLevelControl,curLevelChannel,curLevelImage,curCControl,curControl;

//*Immagini
ADSYSTEM_API void  ADSYSTEM_Image_Import(char *filename);
ADSYSTEM_API int   ADSYSTEM_Image_Count();
ADSYSTEM_API void  ADSYSTEM_Image_SetCurrent(int imageIndex);
ADSYSTEM_API void  ADSYSTEM_Image_Delete();
ADSYSTEM_API void  ADSYSTEM_Image_GetData(void **buffer, int *width, int *height);
ADSYSTEM_API void  ADSYSTEM_Image_GetInfo(char **name,char **desc);
//importa un'immagine

//*Canali
ADSYSTEM_API int  ADSYSTEM_Channel_Count(); //ritorna il numero di canali esistenti
ADSYSTEM_API int  ADSYSTEM_Channel_Create(); //crea un canale e ne ritorna l'index
ADSYSTEM_API void ADSYSTEM_Channel_SetCurrent(int channelIndex); //setta il canale su cui si lavora
ADSYSTEM_API int  ADSYSTEM_Channel_GetCurrent(); //ritorna il canale su cui si lavora
ADSYSTEM_API void ADSYSTEM_Channel_GetInfo(char **name,char **desc);
ADSYSTEM_API void ADSYSTEM_Channel_SetInfo(char *name,char *desc);
ADSYSTEM_API void ADSYSTEM_Channel_Delete(); //elimina il canale corrente
ADSYSTEM_API void ADSYSTEM_Level_MoveUp();
ADSYSTEM_API void ADSYSTEM_Level_MoveDown();

//*Classi di Livelli (efx)
ADSYSTEM_API int  ADSYSTEM_CLevel_Count(); //ritorna il numero delle classi di efx
ADSYSTEM_API void ADSYSTEM_CLevel_SetCurrent(int cLevelIndex);
ADSYSTEM_API void ADSYSTEM_CLevel_GetInfo(char **name,char **desc);
//proprietà associate
ADSYSTEM_API int  ADSYSTEM_CLevelControl_Count(); 
ADSYSTEM_API void ADSYSTEM_CLevelControl_SetCurrent(int cLevelContolIndex);
ADSYSTEM_API void ADSYSTEM_CLevelControl_GetInfo(char **name,char **desc);
ADSYSTEM_API void ADSYSTEM_CLevelControl_GetBase(char **name);
//canali in input
ADSYSTEM_API int  ADSYSTEM_CLevelChannel_Count(); 
ADSYSTEM_API void ADSYSTEM_CLevelChannel_SetCurrent(int cLevelChannelIndex);
ADSYSTEM_API void ADSYSTEM_CLevelChannel_GetInfo(char **name,char **desc);

//*Istanze di Livelli (efx)
ADSYSTEM_API int  ADSYSTEM_Level_Count(); //ritorna il numero di livelli esistenti (nel canale corrente)
ADSYSTEM_API int  ADSYSTEM_Level_Create(); //crea un nuovo livello (nel canale corrente) e ritorna l'index
ADSYSTEM_API void ADSYSTEM_Level_SetCurrent(int levelIndex);
ADSYSTEM_API void ADSYSTEM_Level_GetInfo(char **name,char **desc);
ADSYSTEM_API void ADSYSTEM_Level_SetInfo(char *name,char *desc);
ADSYSTEM_API void ADSYSTEM_Level_Delete();
ADSYSTEM_API int  ADSYSTEM_Level_GetClass(); //ritorna la classe corrispondente all'istanza
ADSYSTEM_API void ADSYSTEM_Level_MoveUp();
ADSYSTEM_API void ADSYSTEM_Level_MoveDown();
ADSYSTEM_API void ADSYSTEM_Level_LoadFromMemory(void *data,int size);
ADSYSTEM_API void ADSYSTEM_Level_SaveToMemory(void **data,int *size);
ADSYSTEM_API void ADSYSTEM_Level_FreeSavedMemory();
ADSYSTEM_API void ADSYSTEM_Level_Edit(HWND mainWin);

//proprietà associate
ADSYSTEM_API int  ADSYSTEM_LevelControl_Count();
ADSYSTEM_API void ADSYSTEM_LevelControl_Create(); 
ADSYSTEM_API void ADSYSTEM_LevelControl_SetCurrent(int levelControlIndex);
ADSYSTEM_API void ADSYSTEM_LevelControl_GetInfo(char **name,char **desc);
ADSYSTEM_API int  ADSYSTEM_LevelControl_GetClass();
ADSYSTEM_API void ADSYSTEM_LevelControl_GetBase(char **name);
ADSYSTEM_API void ADSYSTEM_LevelControl_LoadFromMemory(void *data,int size);
ADSYSTEM_API void ADSYSTEM_LevelControl_SaveToMemory(void **data,int *size);
ADSYSTEM_API void ADSYSTEM_LevelControl_FreeSavedMemory();
ADSYSTEM_API void ADSYSTEM_LevelControl_GetValue(float pos, void **data, int *size);

ADSYSTEM_API int  ADSYSTEM_LevelControlKey_Count();
ADSYSTEM_API int  ADSYSTEM_LevelControlKey_GetPos(int keynum);
ADSYSTEM_API void ADSYSTEM_LevelControlKey_Add(int pos);
ADSYSTEM_API void ADSYSTEM_LevelControlKey_Move(int num,int newpos);
ADSYSTEM_API void ADSYSTEM_LevelControlKey_Delete(int keynum);
ADSYSTEM_API void ADSYSTEM_LevelControlKey_Edit(int pos, HWND mainWin);

//canali in input / output
ADSYSTEM_API void ADSYSTEM_LevelChannel_SetCurrent(int levelChannelIndex);
ADSYSTEM_API int  ADSYSTEM_LevelChannel_GetChannel();
ADSYSTEM_API void ADSYSTEM_LevelChannel_SetChannel(int channelIndex);

//immagini in input
ADSYSTEM_API void ADSYSTEM_LevelImage_SetCurrent(int levelImageIndex);
ADSYSTEM_API int  ADSYSTEM_LevelImage_GetImage();
ADSYSTEM_API void ADSYSTEM_LevelImage_SetImage(int imageIndex);

//Classi di Controller
ADSYSTEM_API int  ADSYSTEM_CControl_Count(); //ritorna il numero delle classi controller esistenti
ADSYSTEM_API void ADSYSTEM_CControl_SetCurrent(int controllerIndex);
ADSYSTEM_API void ADSYSTEM_CControl_GetInfo(char **name,char **desc);
ADSYSTEM_API void ADSYSTEM_CControl_GetBase(char **name);

#endif

