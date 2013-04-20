#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_FMOD
#include <fmod.h>
#include <fmod_errors.h>
#endif

#include "ADsystem.h"
#include "ADbase.h"

__declspec(dllimport) void debug_warning(HWND hWnd,char *messaggio);
__declspec(dllimport) void debug_error(HWND hWnd,char *messaggio);



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{ 
	    case DLL_PROCESS_ATTACH:

#ifdef USE_FMOD

			if (FSOUND_GetVersion() < FMOD_VERSION)
				debug_error(0,"Error : You are using the wrong FMOD DLL version!");

			if (!FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND))
				debug_error(0,FMOD_ErrorString(FSOUND_GetError()));
			
			if (!FSOUND_SetDriver(0))
				debug_error(0,FMOD_ErrorString(FSOUND_GetError()));

			if (!FSOUND_Init(44100, 32, FSOUND_INIT_GLOBALFOCUS))
				debug_error(0,FMOD_ErrorString(FSOUND_GetError()));
#endif

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

ptrList ADChunks,ADinPlugs;
#ifdef USE_FMOD
	FSOUND_STREAM *stream;
#endif
tSystem ADsystem;
image *backbuffer;

typedef void (CALLBACK* LP_ADEFX_GETINFO)(char **name,char **desc);
LP_ADEFX_GETINFO ADEFX_GetInfo;
typedef void *(CALLBACK* LP_ADEFX_CREATE)(void);
LP_ADEFX_CREATE ADEFX_Create;


char *GetFileExt(char *filename)
{
	int i=0;
	int j=0;
	char *ext=new char[6];

	while ((filename[i]!='.') && (filename[i]!=0)) i++;
	if (filename[i]==0) return NULL;
	i++;
	while (filename[i+j]!=0) 
	{
		ext[j]=filename[i+j];
		j++;
	}
	ext[j]=0;
	ext=strupr(ext);
	return ext;
}

char *NoPath(char *filename)
{
	int i=strlen(filename)-1;

	while ((filename[i]!='\\') && (filename[i]!='/') && (i>=0)) i--;
    return filename+i+1;
}

int GetImageNum()
{
	char *buffer;
	int bufsize;

	if (ADSYSTEM_ChunkRead("IMAGE_NUM",(void **)&buffer,(int *)&bufsize)==-1)
	{
		buffer=(char *)new int(0);
		bufsize=sizeof(int);
	}

	return *(int *)buffer;
}

void SetImageNum(int num)
{
	char *buffer;
	int bufsize;
		
	buffer=(char *)new int(num);
	bufsize=sizeof(int);

	ADSYSTEM_ChunkWrite("IMAGE_NUM",buffer,bufsize);
}

void UncompressImageChunk(int imgnum)
{
	void *buffer;
	char *tmpbuf,*chunkID1,*chunkID2,*ext,*filename;
	int bufsize;
    ADinPlug *mioplug;
	void *outbuf;
	image *miaimg;
	int i,plugfound,outsize,outwidth,outheight;

	chunkID1=new char[12];
	chunkID2=new char[12];
	strcpy(chunkID1,"IMAGE");
	chunkID1[5]=0;
	strcpy(chunkID2,"");
	itoa(imgnum,chunkID2,10);
	strcat(chunkID1,chunkID2);

	ADSYSTEM_ChunkRead(chunkID1,&buffer,&bufsize);
	filename=(char *)buffer;
	tmpbuf=((char *)buffer)+strlen(filename)+1;
	bufsize-=strlen(filename)+1;
	ext=GetFileExt(filename);
	
//	debug_warning(0,chunkID1);

	//ora estraggo l'immagine usando i plugin di input

	plugfound=-1;
	for(i=0; i<ADinPlugs.count; i++)
	{
		mioplug=(ADinPlug *)ADinPlugs.getPtrByIndex(i);
	    if ( (strcmp(mioplug->ext1,ext)==0) ||
			 (strcmp(mioplug->ext2,ext)==0) ||
			 (strcmp(mioplug->ext3,ext)==0) ||
			 (strcmp(mioplug->ext4,ext)==0) ||
			 (strcmp(mioplug->ext5,ext)==0) )
			 plugfound=i;
	}
	if (plugfound==-1) 
	{
		debug_warning(0,"Unknown image file extension");
		return;
	}

	mioplug=(ADinPlug *)ADinPlugs.getPtrByIndex(plugfound);

    mioplug->IMAGE_LoadFromMemory(tmpbuf,bufsize,&outbuf,&outsize,&outwidth,&outheight);

	miaimg=new image(outwidth,outheight,outbuf);
	
	if (ADsystem.images.count<GetImageNum())
		ADsystem.images.addItem(miaimg,filename,chunkID1);
	else
		ADsystem.images.setItem(imgnum,miaimg,filename,chunkID1);

	//delete [] buffer;
	//il buffer lo potrei cancellare solo se non lo devo poi salvare
	delete [] chunkID1;
	delete [] chunkID2;
	delete [] ext;
}


ADSYSTEM_API void ADSYSTEM_Init(int width,int height) 
{
	tChannel *mychannel;
	bool in_a_subdir=false;
	

	ADsystem.width=width;
	ADsystem.height=height;
	ADsystem.channel.count=0;
	ADSYSTEM_Channel_Create();
	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(0);
	backbuffer=mychannel->chImage;

	// definisco i controller di base
	ADsystem.ccontrol.count=0;
	ADsystem.ccontrol.addItem(new ctBoolConst,"ctBoolConst","Valore booleano costante");
	ADsystem.ccontrol.addItem(new ctIntConst,"ctIntConst","Valore intero costante");
	ADsystem.ccontrol.addItem(new ctFloatConst,"ctFloatConst","Valore reale costante");
	ADsystem.ccontrol.addItem(new ctVector2DConst,"ctVector2DConst","Vettore 2D costante");
	ADsystem.ccontrol.addItem(new ctIntLinear,"ctIntLinear","Valore intero lineare");
	ADsystem.ccontrol.addItem(new ctFloatLinear,"ctFloatLinear","Valore reale lineare");
	ADsystem.ccontrol.addItem(new ctVector2DLinear,"ctVector2DLinear","Vettore 2D lineare");
//	ADsystem.ccontrol.addItem(new ctTextConst,"ctTextConst","Valore testuale costante");
//	ADsystem.ccontrol.addItem(new ctBoolRange,"ctBoolRange","Valore booleano definito per intervalli");
//  ADsystem.ccontrol.addItem(new ctFloatTCB,"ctFloatTCB","Valore reale interpolato");

	//efx2D *ppmyefx;
	//ppmyefx=new myefx;
	//ppmyefx->defineControllers((tChannel *)ADsystem.channel.getPtrByIndex(0));
    //ADsystem.clevel.addItem(ppmyefx,"Flash","Effettua un flash");

	
	ADsystem.clevel.count=0;

	//loading efx esterni
	HINSTANCE ADLib;
	char *efxname;
	char *efxdesc;
	efx2D *pmyefx;
	struct _finddata_t lib_file;
    long hFile;
	ADinPlug *plugstruct;

	
if ( !chdir("plugins") )
{
	// trova tutti i file in*.dll 
    if(!((hFile = _findfirst( "in*.dll", &lib_file )) == -1L ))
    {
		do
        {
			ADLib=LoadLibrary(lib_file.name);
			if (ADLib!=NULL)
			{
				plugstruct=new ADinPlug;
				plugstruct->IMAGE_GetExtensions=(LP_IMAGE_GETEXTENSIONS)GetProcAddress(ADLib,"IMAGE_GetExtensions");
				if (plugstruct->IMAGE_GetExtensions==0) debug_error(0,"Errore nel plugin di input (IMAGE_GetExtensions)");
				plugstruct->IMAGE_GetPluginInfo=(LP_IMAGE_GETPLUGININFO)GetProcAddress(ADLib,"IMAGE_GetPluginInfo");
				if (plugstruct->IMAGE_GetPluginInfo==0) debug_error(0,"Errore nel plugin di input (IMAGE_GetPluginInfo)");
				plugstruct->IMAGE_LoadFromMemory=(LP_IMAGE_LOADFROMMEMORY)GetProcAddress(ADLib,"IMAGE_LoadFromMemory");
				if (plugstruct->IMAGE_LoadFromMemory==0) debug_error(0,"Errore nel plugin di input (IMAGE_LoadFromMemory)");

				plugstruct->IMAGE_GetExtensions(&plugstruct->ext1,&plugstruct->ext2,
					                            &plugstruct->ext3,&plugstruct->ext4,
												&plugstruct->ext5);
				plugstruct->IMAGE_GetPluginInfo(&plugstruct->name,&plugstruct->desc);
				ADinPlugs.addItem(plugstruct,plugstruct->name,plugstruct->desc);
			}
			else debug_error(0,"Errore nel plugin di input (LoadLibrary)");
        }
		while( _findnext( hFile, &lib_file ) == 0 );
       _findclose( hFile );
    }

  chdir("..");
}

in_a_subdir=!chdir("efx");

    // trova tutti i files .efx
    if(!((hFile = _findfirst( "*.efx", &lib_file )) == -1L ))
    {
		do
        {
			ADLib=LoadLibrary(lib_file.name);
			if (ADLib!=NULL)
			{
				ADEFX_GetInfo=(LP_ADEFX_GETINFO)GetProcAddress(ADLib,"ADEFX_GetInfo");
				ADEFX_Create=(LP_ADEFX_CREATE)GetProcAddress(ADLib,"ADEFX_Create");

				ADEFX_GetInfo(&efxname,&efxdesc);
				pmyefx=(efx2D *)ADEFX_Create();
				pmyefx->defineControllers((tChannel *)ADsystem.channel.getPtrByIndex(0));
				pmyefx->updateControllers();
				ADsystem.clevel.addItem(pmyefx,efxname,efxdesc);
			}
        }
		while( _findnext( hFile, &lib_file ) == 0 );
       _findclose( hFile );
    }

	if (in_a_subdir) { chdir(".."); in_a_subdir=false; }

}

ADSYSTEM_API void ADSYSTEM_Update() 
{
	tChannel *mychannel;
	efx2D *myefx;
	int i,j;
	
	for (i=0;i<ADsystem.channel.count;i++)
	{
		mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(i);
		mychannel->chImage->clear();
		for (j=0; j<mychannel->level.count; j++) 
		{
			myefx=(efx2D *)mychannel->level.getPtrByIndex(j);
			if (myefx->initialized) myefx->free();
			myefx->updateControllers();
			myefx->init();
			
		}
	}

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(0);
	backbuffer=mychannel->chImage;
}

ADSYSTEM_API void *ADSYSTEM_Paint(float curpos) // loop principale
{
	tChannel *mychannel;
	efx2D *myefx;
	int i,j;
	
	for (i=0;i<ADsystem.channel.count;i++)
	{
		mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(i);
		//mychannel->chImage->clear(); //temporaneo!!!
		for (j=0; j<mychannel->level.count; j++) 
		{
			myefx=(efx2D *)mychannel->level.getPtrByIndex(j);
			myefx->onPaint(curpos);
		}
	}

	return backbuffer->voidptr;
}

ADSYSTEM_API void ADSYSTEM_Close() // codice finale
{
	ADSYSTEM_Reset();
#ifdef USE_FMOD
	FSOUND_Close();
#endif
  //video.close();
  /*
  char *str=new char[100];
  int dec,sign;
  str=fcvt(demotimer.get_stepsPerSecond(),4,&dec,&sign);
  str[dec+1]=str[dec];
  str[dec]='.';
  str[dec+2]=0;
  strcat(str," fps");
  debug_error(miawin,str);
  */
}

ADSYSTEM_API void  ADSYSTEM_Reset()
{
	int i;

	for(i=ADSYSTEM_Channel_Count()-1; i>=0; i--)
	{
		ADSYSTEM_Channel_SetCurrent(i);
		ADSYSTEM_Channel_Delete();
	}
	for(i=ADSYSTEM_Image_Count()-1; i>=0; i--)
	{
		ADSYSTEM_Image_SetCurrent(i);
		ADSYSTEM_Image_Delete();
	}
    SetImageNum(0);
	//todo: eliminare le immagini dai chunks e da images
}

ADSYSTEM_API void  ADSYSTEM_LoadFromFile(char *filename)
{
	fileReader myfile;
	controller *mycontrol;
	tChannel *mychannel;
	efx2D *myefx;
	int i,j,k,num,num2,num3,leveltype,size;
	char st1[256];
	char st2[256];
	char *mybuffer;

	ADSYSTEM_Reset();

	myfile.fileOpen(filename);
    
	//FileType tag
	myfile.readString(st1); //AD file format 01

	//FileInfo tag
	myfile.readInt(&i); //dimensione blocco
    mybuffer=new char[i];
	myfile.readBuffer(mybuffer,i); //contenuto blocco
	delete mybuffer;

	// chunks
	myfile.readInt(&i); //numero chunks

	for(j=0; j<i; j++)
	{
		myfile.readString(st1);	// chunk name
		myfile.readInt(&k); //chunk size
		mybuffer=new char[k];
		myfile.readBuffer(mybuffer,k); //chunk data
		ADSYSTEM_ChunkWrite(st1,mybuffer,k);
		delete mybuffer;
	}


//	//FileSettings tag (per usi futuri)
//	myfile.readInt(&i); //dimensione blocco
//    st1=new char[i];
//	myfile.readBuffer(st1,i); //contenuto blocco
//	delete st1;

	//images
	for(i=0; i<GetImageNum(); i++) UncompressImageChunk(i+1);


	//channels
	myfile.readInt(&num);
	for(i=0; i<num; i++)
	{
		j=ADSYSTEM_Channel_Create();
		ADSYSTEM_Channel_SetCurrent(j);
		mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(j);
		myfile.readString(st1);
		myfile.readString(st2);
		ADSYSTEM_Channel_SetInfo(st1,st2);
		//levels
		myfile.readInt(&num2);
		for (j=0; j<num2; j++)
		{
			myfile.readString(st1); 
            leveltype=ADsystem.clevel.getItemIndexByName(st1);
			if (leveltype<0) debug_error(0,"tipo di effetto non disponibile");
			ADSYSTEM_CLevel_SetCurrent(leveltype);
			k=ADSYSTEM_Level_Create();
			ADSYSTEM_Level_SetCurrent(k);
			myfile.readString(st1);
			myfile.readString(st2);
			ADSYSTEM_Level_SetInfo(st1,st2);
			
			myefx=(efx2D *)mychannel->level.getPtrByIndex(k);

			//levelcontrols
			myfile.readInt(&num3);
			for(k=0; k<num3; k++)
			{
				myfile.readString(st1);
				ADSYSTEM_CControl_SetCurrent(ADsystem.ccontrol.getItemIndexByName(st1));
				ADSYSTEM_LevelControl_SetCurrent(k);
				ADSYSTEM_LevelControl_Create();
				myfile.readString(st1);
				myfile.readString(st2);
			//	ADSYSTEM_LevelControl_SetInfo(st1,st2);
				mycontrol=(controller *)myefx->ct.getPtrByIndex(k);
				myfile.readInt(&size);
				mybuffer=new char[size];
				myfile.readBuffer(mybuffer,size);
				mycontrol->loadFromMemory(mybuffer,size);
				delete mybuffer;
			}

			//levelchannels
			myfile.readInt(&num3);
			for (k=0; k<num3; k++)
			{
				myfile.readString(st1);
				myfile.readString(st2);
				ADSYSTEM_LevelChannel_SetCurrent(k);
				myefx->channels.setItem(k,NULL,myefx->channels.getName(k),st2);
			}
			//levelimages
			myfile.readInt(&num3);
			for (k=0; k<num3; k++)
			{
				myfile.readString(st1);
				myfile.readString(st2);
				ADSYSTEM_LevelImage_SetCurrent(k);
				ADSYSTEM_LevelImage_SetImage(ADsystem.images.getItemIndexByName(st2));
			}
			//efxdata
			myfile.readInt(&size);
			mybuffer=new char[size];
			myfile.readBuffer(mybuffer,size);
			myefx->loadFromMemory(mybuffer,size);
			delete mybuffer;
		}
	}

	for (i=0; i<ADSYSTEM_Channel_Count(); i++)
	{
		mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(i);
		ADSYSTEM_Channel_SetCurrent(i);
		for(j=0; j<ADSYSTEM_Level_Count(); j++)
		{
		    myefx=(efx2D *)mychannel->level.getPtrByIndex(j);
			ADSYSTEM_Level_SetCurrent(j);
			ADSYSTEM_CLevel_SetCurrent(ADSYSTEM_Level_GetClass());
			for (k=0; k<ADSYSTEM_CLevelChannel_Count(); k++)
			{
				ADSYSTEM_LevelChannel_SetCurrent(k);
				//debug_warning(0,myefx->channels.getDescription(k));
				ADSYSTEM_LevelChannel_SetChannel(ADsystem.channel.getItemIndexByName(myefx->channels.getDescription(k)));
			}
		}
	}

	if (ADsystem.channel.count==0) ADSYSTEM_Channel_Create(); // x evitare crash

	myfile.fileClose();
	ADSYSTEM_Update();
}

ADSYSTEM_API void  ADSYSTEM_SaveToFile(char *filename)
{
	fileWriter myfile;
	controller *mycontrol;
	tChannel *mychannel;
	efx2D *myefx;
	void *data;
	int size,i,j,k;

	myfile.fileOpen(filename);
    
	//FileType tag
	myfile.writeString("ADf-01"); //AD file format 01

	//FileInfo tag
	const char *info="Apocalypse Design - Visual ADESSO";
	myfile.writeInt(strlen(info)+1); //dimensione blocco
	myfile.writeString(info); //contenuto blocco

	//FileSettings tag (per usi futuri)
	//myfile.writeInt(0); //dimensione blocco

	//chunks

	myfile.writeInt(ADChunks.count);

	for(i=0; i<ADChunks.count; i++)
	{
		//debug_warning(0,ADChunks.getName(i));
		myfile.writeString(ADChunks.getName(i));	// chunk name
		myfile.writeInt(atoi(ADChunks.getDescription(i))); //chunk size
		myfile.writeBuffer(ADChunks.getPtrByIndex(i),atoi(ADChunks.getDescription(i))); //chunk data
	}

	//channels
	myfile.writeInt(ADsystem.channel.count);
	for(i=0; i<ADsystem.channel.count; i++)
	{
		mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(i);
		myfile.writeString(ADsystem.channel.getName(i));
		myfile.writeString(ADsystem.channel.getDescription(i));

		//levels
		myfile.writeInt(mychannel->level.count);
		for (j=0; j<mychannel->level.count; j++)
		{
			myefx=(efx2D *)mychannel->level.getPtrByIndex(j);
			myfile.writeString(myefx->className);
			myfile.writeString(mychannel->level.getName(j));
			myfile.writeString(mychannel->level.getDescription(j));
			//levelcontrols
			myfile.writeInt(myefx->ct.count);			
			for(k=0; k<myefx->ct.count; k++)
			{
				mycontrol=(controller *)myefx->ct.getPtrByIndex(k);
				myfile.writeString(mycontrol->className);
				myfile.writeString(myefx->ct.getName(k));
				myfile.writeString(myefx->ct.getDescription(k));
				mycontrol->saveToMemory(&data,&size);
				myfile.writeInt(size);
				myfile.writeBuffer(data,size);
				mycontrol->freeSavedMemory();
			}

			//levelchannels
			myfile.writeInt(myefx->channels.count);
			for (k=0; k<myefx->channels.count; k++)
			{
				myfile.writeString(myefx->channels.getName(k));
				myfile.writeString(myefx->channels.getDescription(k));
			}
			//levelimages
			myfile.writeInt(myefx->images.count);
			for (k=0; k<myefx->images.count; k++)
			{
				myfile.writeString(myefx->images.getName(k));
				myfile.writeString(myefx->images.getDescription(k));
			}
			//efxdata
			myefx->saveToMemory(&data,&size);
			myfile.writeInt(size);
			myfile.writeBuffer(data,size);
			myefx->freeSavedMemory();
		}
	}

			
	myfile.fileClose();
}

//*** chunk ***

ADSYSTEM_API int   ADSYSTEM_ChunkWrite(char *ChunkName, void *data, int size)
{
	char *buffer=new char[size];
	char *stSize=new char[32];
	char *Cname=new char[strlen(ChunkName)+1];
	strcpy(Cname,ChunkName);

	memcpy(buffer,data,size);
	itoa(size,stSize,10);

	if (ADChunks.ItemNameExists(Cname))
	{
	   // ADChunks.deleteItem(ADChunks.getItemIndexByName(Cname));
		ADChunks.setItem(ADChunks.getItemIndexByName(Cname),buffer,Cname,stSize);
	} else ADChunks.addItem(buffer,Cname,stSize);

	return ADChunks.count;
}

ADSYSTEM_API int   ADSYSTEM_ChunkRead(char *ChunkName,void **data, int *size)
{
	if (!ADChunks.ItemNameExists(ChunkName)) return -1;

	*data=ADChunks.getPtrByName(ChunkName);
	*size=atoi(ADChunks.getDescription(ADChunks.getItemIndexByName(ChunkName)));

	return ADChunks.count;
}

ADSYSTEM_API void ADSYSTEM_SoundLoad()
{
	
#ifdef USE_FMOD

	void *filename;
	int filenamesize;
	char tmpstr[100];

	if (ADSYSTEM_ChunkRead("SOUNDTRACK",&filename,&filenamesize)!=-1)
	{
		stream = FSOUND_Stream_Open((char *)filename, FSOUND_NORMAL | FSOUND_LOOP_NORMAL, 0, 0);
	    if (!stream) 
		{
			strcpy(tmpstr,"Music File Error: ");
			strcat(tmpstr,FMOD_ErrorString(FSOUND_GetError()));
			debug_warning(0,tmpstr);
		}
	}

#endif
}

ADSYSTEM_API void ADSYSTEM_SoundPlay(int pos)
{
#ifdef USE_FMOD
	if (stream) 
	{
		FSOUND_Stream_Play(FSOUND_FREE, stream);
		if (pos!=0) FSOUND_Stream_SetTime(stream,pos);
	}
#endif
}

ADSYSTEM_API void ADSYSTEM_SoundStop()
{
#ifdef USE_FMOD
	FSOUND_Stream_Close(stream);
#endif
}

ADSYSTEM_API void  ADSYSTEM_Image_Import(char *filename)
{
	
	fileReader myfile;
	char *buffer,*tmpbuf,*chunkID1,*chunkID2,*ext;
	unsigned int bufsize,imgnum;

	chunkID1=new char[12];
	chunkID2=new char[12];

	ext=GetFileExt(filename);

	//incremento il contatore delle immagini
	imgnum=GetImageNum()+1;
	SetImageNum(imgnum);

	//inserisco l'immagine
	myfile.fileOpen(filename);
	filename=NoPath(filename);
	bufsize=myfile.fileSize();
	buffer=new char[bufsize+strlen(filename)+1];
	strcpy(buffer,filename);
	tmpbuf=buffer+strlen(filename)+1;
	myfile.readBuffer(tmpbuf,bufsize);
	myfile.fileClose();
	
	strcpy(chunkID1,"IMAGE");
	chunkID1[5]=0;
	strcpy(chunkID2,"");
	itoa(imgnum,chunkID2,10);
	strcat(chunkID1,chunkID2);
//	debug_warning(0,chunkID1);
	ADSYSTEM_ChunkWrite(chunkID1,buffer,bufsize+strlen(filename)+1);

	UncompressImageChunk(imgnum);

	delete [] buffer;
	delete [] chunkID1;
	delete [] chunkID2;
	delete [] ext;
}

ADSYSTEM_API int   ADSYSTEM_Image_Count()
{
	return ADsystem.images.count;
}

ADSYSTEM_API void  ADSYSTEM_Image_SetCurrent(int imageIndex)
{
	curImage=imageIndex;
}

ADSYSTEM_API void  ADSYSTEM_Image_Delete()
{
	image *mypic=(image *)ADsystem.images.getPtrByIndex(curImage);
	delete mypic;
	ADsystem.images.deleteItem(curImage);
	//TODO: cancellare il chunk!!!
}

ADSYSTEM_API void  ADSYSTEM_Image_GetData(void **buffer, int *width, int *height)
{
	image *mypic=(image *)ADsystem.images.getPtrByIndex(curImage);
	*buffer=mypic->voidptr;
	*width=mypic->width;
	*height=mypic->height;
}

ADSYSTEM_API void ADSYSTEM_Image_GetInfo(char **name,char **desc)
{
	*name=ADsystem.images.getName(curImage);
    *desc=ADsystem.images.getDescription(curImage);
}

//******Channel******

ADSYSTEM_API int  ADSYSTEM_Channel_Count()
{
	return ADsystem.channel.count;
}

ADSYSTEM_API int  ADSYSTEM_Channel_Create() 
{
	tChannel *mychannel;
	char newname1[30];
	char newname2[30];
	int num;

    mychannel=new tChannel;
    mychannel->chImage=new image(ADsystem.width,ADsystem.height);
	num=1;
	do
	{
		itoa(num,newname2,10);
		strcpy(newname1,"Canale");
		if (num<10) strcat(newname1,"0");
		strcat(newname1,newname2);
		num++;
	} while (ADsystem.channel.ItemNameExists(newname1));
		
	return ADsystem.channel.addItem(mychannel,newname1,"Nessuna descrizione");
}

ADSYSTEM_API void ADSYSTEM_Channel_SetCurrent(int channelIndex)
{
	curChannel=channelIndex;
}

ADSYSTEM_API int  ADSYSTEM_Channel_GetCurrent()
{
	return curChannel;
}

ADSYSTEM_API void ADSYSTEM_Channel_GetInfo(char **name,char **desc)
{
	*name=ADsystem.channel.getName(curChannel);
    *desc=ADsystem.channel.getDescription(curChannel);
}

ADSYSTEM_API void ADSYSTEM_Channel_SetInfo(char *name,char *desc)
{
	ADsystem.channel.setItem(curChannel,ADsystem.channel.getPtrByIndex(curChannel),name,desc);
}

ADSYSTEM_API void ADSYSTEM_Channel_Delete()
{
	tChannel *mychannel;

	while(ADSYSTEM_Level_Count()>0)
	{
		ADSYSTEM_Level_SetCurrent(0);
		ADSYSTEM_Level_Delete();
	}

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	delete mychannel->chImage;
	delete mychannel;
	ADsystem.channel.deleteItem(curChannel);
	if (curChannel>ADsystem.channel.count-1) curChannel=ADsystem.channel.count-1;
}

ADSYSTEM_API void ADSYSTEM_Channel_MoveUp()
{
	ADsystem.channel.moveUpItem(curChannel);
}

ADSYSTEM_API void ADSYSTEM_Channel_MoveDown()
{
	ADsystem.channel.moveDownItem(curChannel);
}


//******CLevel******

ADSYSTEM_API int  ADSYSTEM_CLevel_Count()
{
	return ADsystem.clevel.count;
}


ADSYSTEM_API void ADSYSTEM_CLevel_SetCurrent(int clevelIndex)
{
	curCLevel=clevelIndex;
}

ADSYSTEM_API void ADSYSTEM_CLevel_GetInfo(char **name,char **desc)
{
	*name=ADsystem.clevel.getName(curCLevel);
    *desc=ADsystem.clevel.getDescription(curCLevel);
}

//******CLevelControl******

ADSYSTEM_API int  ADSYSTEM_CLevelControl_Count()
{
	efx2D *myefx;

    myefx=(efx2D *)ADsystem.clevel.getPtrByIndex(curCLevel);

	return myefx->ct.count;
}

ADSYSTEM_API void ADSYSTEM_CLevelControl_SetCurrent(int cLevelControlIndex)
{
	curCLevelControl=cLevelControlIndex;
}

ADSYSTEM_API void ADSYSTEM_CLevelControl_GetInfo(char **name,char **desc)
{
	efx2D *myefx;

    myefx=(efx2D *)ADsystem.clevel.getPtrByIndex(curCLevel);
	*name=myefx->ct.getName(curCLevelControl);
	*desc=myefx->ct.getDescription(curCLevelControl);
}

ADSYSTEM_API void ADSYSTEM_CLevelControl_GetBase(char **name)
{
	efx2D *myefx;
	controller *myct;

    myefx=(efx2D *)ADsystem.clevel.getPtrByIndex(curCLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curCLevelControl);
	*name=myct->baseClass;
}

//******CLevelChannel******

ADSYSTEM_API int  ADSYSTEM_CLevelChannel_Count()
{
	efx2D *myefx;

    myefx=(efx2D *)ADsystem.clevel.getPtrByIndex(curCLevel);

	return myefx->channels.count;
}

ADSYSTEM_API void ADSYSTEM_CLevelChannel_SetCurrent(int cLevelChannelIndex)
{
	curCLevelChannel=cLevelChannelIndex;
}

ADSYSTEM_API void ADSYSTEM_CLevelChannel_GetInfo(char **name,char **desc)
{
	efx2D *myefx;

    myefx=(efx2D *)ADsystem.clevel.getPtrByIndex(curCLevel);
	*name=myefx->channels.getName(curCLevelChannel);
	*desc=myefx->channels.getDescription(curCLevelChannel);
}


//******CLevelImage******

ADSYSTEM_API int  ADSYSTEM_CLevelImage_Count()
{
	efx2D *myefx;

    myefx=(efx2D *)ADsystem.clevel.getPtrByIndex(curCLevel);

	return myefx->images.count;
}

ADSYSTEM_API void ADSYSTEM_CLevelImage_SetCurrent(int cLevelImageIndex)
{
	curCLevelImage=cLevelImageIndex;
}

ADSYSTEM_API void ADSYSTEM_CLevelImage_GetInfo(char **name,char **desc)
{
	efx2D *myefx;

    myefx=(efx2D *)ADsystem.clevel.getPtrByIndex(curCLevel);
	*name=myefx->images.getName(curCLevelImage);
	*desc=myefx->images.getDescription(curCLevelImage);
}


//******Level******

ADSYSTEM_API int  ADSYSTEM_Level_Count()
{
	tChannel *mychannel;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	return mychannel->level.count;
}

ADSYSTEM_API int  ADSYSTEM_Level_Create() 
{
	tChannel *mychannel;
	efx2D *myefx;
	char newname1[30];
	char newname2[30];
	int num,i;

    mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	num=1;
	do
	{
		itoa(num,newname2,10);
		strcpy(newname1,ADsystem.clevel.getName(curCLevel));
		if (num<10) strcat(newname1,"0");
		strcat(newname1,newname2);
		num++;
	} while (mychannel->level.ItemNameExists(newname1));

	myefx=(efx2D *)ADsystem.clevel.getPtrByIndex(curCLevel);
	myefx=(efx2D *)myefx->create();
	myefx->defineControllers(mychannel);
	for (i=0;i<myefx->ct.count;i++) //imposto a default i controller
	   myefx->ct.setItem(i,myefx->ct.getPtrByIndex(i),myefx->ct.getName(i),"default");
	for (i=0;i<myefx->channels.count;i++) //imposto a default i channels
	   myefx->channels.setItem(i,myefx->channels.getPtrByIndex(i),myefx->channels.getName(i),ADsystem.channel.getName(curChannel));
	for (i=0;i<myefx->images.count;i++) //imposto a default le images
	   myefx->images.setItem(i,myefx->images.getPtrByIndex(i),myefx->images.getName(i),"none"/*ADsystem.images.getName(0)*/);
	
	return mychannel->level.addItem(myefx,newname1,"Nessuna descrizione");
}

ADSYSTEM_API void ADSYSTEM_Level_SetCurrent(int levelIndex)
{
	curLevel=levelIndex;
}

ADSYSTEM_API void ADSYSTEM_Level_GetInfo(char **name,char **desc)
{
	tChannel *mychannel;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	*name=mychannel->level.getName(curLevel);
    *desc=mychannel->level.getDescription(curLevel);
}

ADSYSTEM_API void ADSYSTEM_Level_SetInfo(char *name,char *desc)
{
	tChannel *mychannel;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	mychannel->level.setItem(curLevel,mychannel->level.getPtrByIndex(curLevel),name,desc);
}

ADSYSTEM_API void ADSYSTEM_Level_Delete()
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	delete myefx;
	mychannel->level.deleteItem(curLevel);
	if (curLevel>mychannel->level.count-1) curLevel=mychannel->level.count-1;
}

ADSYSTEM_API int  ADSYSTEM_Level_GetClass()
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);

	return ADsystem.clevel.getItemIndexByName(myefx->className);
}

ADSYSTEM_API void ADSYSTEM_Level_MoveUp()
{
	tChannel *mychannel;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    mychannel->level.moveUpItem(curLevel);
}

ADSYSTEM_API void ADSYSTEM_Level_MoveDown()
{
	tChannel *mychannel;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    mychannel->level.moveDownItem(curLevel);
}

ADSYSTEM_API void ADSYSTEM_Level_LoadFromMemory(void *data,int size)
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myefx->loadFromMemory(data,size);
}

ADSYSTEM_API void ADSYSTEM_Level_SaveToMemory(void **data,int *size)
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myefx->saveToMemory(data,size);
}

ADSYSTEM_API void ADSYSTEM_Level_FreeSavedMemory()
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myefx->freeSavedMemory();
}

ADSYSTEM_API void ADSYSTEM_Level_Edit(HWND mainWin)
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myefx->edit(mainWin);
}

//******LevelControl******

ADSYSTEM_API int ADSYSTEM_LevelControl_Count()
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);

	return myefx->ct.count;
}

ADSYSTEM_API void ADSYSTEM_LevelControl_Create()
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	//elimino il vecchio controller:
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);
//	if (myct != NULL) delete myct;
	//creo il nuovo controller:
	myct=(controller *)ADsystem.ccontrol.getPtrByIndex(curCControl);
	myct=(controller *)myct->create();
	myefx->ct.setItem(curLevelControl,myct);
}

ADSYSTEM_API void ADSYSTEM_LevelControl_SetCurrent(int levelControlIndex)
{
	curLevelControl=levelControlIndex;
}

ADSYSTEM_API void ADSYSTEM_LevelControl_GetInfo(char **name,char **desc)
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);

	*name=myefx->ct.getName(curLevelControl);
    *desc=myefx->ct.getDescription(curLevelControl);
}

ADSYSTEM_API int ADSYSTEM_LevelControl_GetClass()
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	return ADsystem.ccontrol.getItemIndexByName(myct->className);
}

ADSYSTEM_API void ADSYSTEM_LevelControl_GetBase(char **name)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	*name=myct->baseClass;
}

ADSYSTEM_API void ADSYSTEM_LevelControl_LoadFromMemory(void *data,int size)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	myct->loadFromMemory(data,size);
}

ADSYSTEM_API void ADSYSTEM_LevelControl_SaveToMemory(void **data,int *size)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	myct->saveToMemory(data,size);
}

ADSYSTEM_API void ADSYSTEM_LevelControl_FreeSavedMemory()
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	myct->freeSavedMemory();
}

ADSYSTEM_API void ADSYSTEM_LevelControl_GetValue(float pos, void **data, int *size)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	*size=4;

	if (strcmp(myct->baseClass,"ctBool")==0)
	{
		ctBool *myctBool=(ctBool *)myct;
		bool *temp=new bool;
		*temp=myctBool->getValue(pos);
		*data=temp;
		return;
	}

	if (strcmp(myct->baseClass,"ctFloat")==0)
	{
		ctFloat *myctFloat=(ctFloat *)myct;
		float *temp=new float;
		*temp=myctFloat->getValue(pos);
		*data=temp;
		return;
	}

	//TODO: tutti gli altri tipi di controller di base
}

ADSYSTEM_API int ADSYSTEM_LevelControlKey_Count()
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);
	
	return myct->keys->keyCount();
}

ADSYSTEM_API int  ADSYSTEM_LevelControlKey_GetPos(int keynum)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	return myct->keys->keyPos(keynum);
}

ADSYSTEM_API void ADSYSTEM_LevelControlKey_Add(int pos)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	myct->keys->keyAdd(pos,myct->createDefaultKey());
}

ADSYSTEM_API void ADSYSTEM_LevelControlKey_Move(int num,int newpos)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	myct->keys->keyMove(num,newpos);
}

ADSYSTEM_API void ADSYSTEM_LevelControlKey_Delete(int keynum)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	myct->keys->keyDelete(keynum);
}

ADSYSTEM_API void ADSYSTEM_LevelControlKey_Edit(int pos, HWND mainWin)
{
	tChannel *mychannel;
	efx2D *myefx;
	controller *myct;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
    myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	myct=(controller *)myefx->ct.getPtrByIndex(curLevelControl);

	myct->editKey(pos,mainWin);
}


//******LevelChannel******


ADSYSTEM_API void ADSYSTEM_LevelChannel_SetCurrent(int levelChannelIndex)
{
	curLevelChannel=levelChannelIndex;
}

ADSYSTEM_API int  ADSYSTEM_LevelChannel_GetChannel()
{
	tChannel *mychannel;
	efx2D *myefx;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);

	mychannel=(tChannel *)(myefx->channels.getPtrByIndex(curLevelChannel));
	return ADsystem.channel.getItemIndexByName(myefx->channels.getDescription(curLevelChannel));
}

ADSYSTEM_API void ADSYSTEM_LevelChannel_SetChannel(int channelIndex)
{
	tChannel *mychannel;
	efx2D *myefx;
	tChannel *mylchan;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);

	myefx->channels.setItem(curLevelChannel,ADsystem.channel.getPtrByIndex(channelIndex),
		myefx->channels.getName(curLevelChannel),ADsystem.channel.getName(channelIndex));

	mylchan=(tChannel *)ADsystem.channel.getPtrByIndex(channelIndex);
	myefx->channelimages.setItem(curLevelChannel,mylchan->chImage);
}

//******LevelImage******


ADSYSTEM_API void ADSYSTEM_LevelImage_SetCurrent(int levelImageIndex)
{
	curLevelImage=levelImageIndex;
}

ADSYSTEM_API int  ADSYSTEM_LevelImage_GetImage()
{
	tChannel *mychannel;
	efx2D *myefx;
	image *myimage;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);

	myimage=(image *)(myefx->images.getPtrByIndex(curLevelImage));
	return ADsystem.images.getItemIndexByName(myefx->images.getDescription(curLevelImage));
}

ADSYSTEM_API void ADSYSTEM_LevelImage_SetImage(int imageIndex)
{
	tChannel *mychannel;
	efx2D *myefx;
	image *mylimg;

	mychannel=(tChannel *)ADsystem.channel.getPtrByIndex(curChannel);
	if (mychannel==NULL) return;
	myefx=(efx2D *)mychannel->level.getPtrByIndex(curLevel);
	if (myefx==NULL) return;

	//if (myefx->images.count<=imageIndex) return;

	myefx->images.setItem(curLevelImage,ADsystem.images.getPtrByIndex(imageIndex),
		myefx->images.getName(curLevelImage),ADsystem.images.getName(imageIndex));

	mylimg=(image *)ADsystem.images.getPtrByIndex(imageIndex);
	myefx->images.setItem(curLevelImage,mylimg);
}

//******CControl******

ADSYSTEM_API int  ADSYSTEM_CControl_Count()
{
	return ADsystem.ccontrol.count;
}

ADSYSTEM_API void ADSYSTEM_CControl_SetCurrent(int cControlIndex)
{
	curCControl=cControlIndex;
}

ADSYSTEM_API void ADSYSTEM_CControl_GetInfo(char **name,char **desc)
{
	*name=ADsystem.ccontrol.getName(curCControl);
    *desc=ADsystem.ccontrol.getDescription(curCControl);
}

ADSYSTEM_API void ADSYSTEM_CControl_GetBase(char **name)
{
	controller *myct;
	myct=(controller *)ADsystem.ccontrol.getPtrByIndex(curCControl);
	*name=myct->baseClass;
}
