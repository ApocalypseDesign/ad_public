#include <string.h>
#include "efx2D.h"
#include "Resource.h"

extern HANDLE hmodulo;

void efx2D::addChannel(tChannel *defaultChannel,char *name, char *desc)
{
	channels.addItem(defaultChannel,name,desc);
	channelimages.addItem(defaultChannel->chImage,name,desc);
}

void efx2D::addImage(char *name, char *desc)
{
	images.addItem(new image(4,4),name,desc);
}

image *efx2D::getChannelImage(char *channelName)
{
	return (image *)channelimages.getPtrByName(channelName);
}

image *efx2D::getImage(char *name)
{
	return (image *)images.getPtrByName(name);
}

void efx2D::addProperty(void *defaultCt,char *name,char *desc)
{
	ct.addItem(defaultCt,name,desc);
}
	
void *efx2D::getProperty(char *name)
{
	return ct.getPtrByName(name);
}

efx2D::efx2D(void) 
{ 
	int activeval=1;
	ctBoolConst *defaultActive=new ctBoolConst;
	ct.addItem(defaultActive,"active","determina dove è attivo l'effetto"); 
	strcpy(className,"efx2D");
	initialized=false;
} 

void *efx2D::create() 
{ return new efx2D; }

void efx2D::defineControllers(tChannel *defaultChannel)
{ } 

void efx2D::updateControllers() 
{ }

void efx2D::loadFromMemory(void *data,int size)
{ }
  
void efx2D::saveToMemory(void **data,int *size)
{
	*data=NULL; *size=0;
}

void efx2D::freeSavedMemory()
{ }

int FAR PASCAL
DialogEditEfx(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 
// char *strbuffer;
 
 switch (message)
	{
		case WM_INITDIALOG:
		/*	strbuffer=new char[20];
			strbuffer=gcvt((double)(*(float *)(CT_data)),3,strbuffer);
			

            SetDlgItemText(hWnd,IDC_EDIT1,strbuffer);
			SetDlgItemInt(hWnd,IDC_STATICN,CT_keynum,false);
			SetDlgItemInt(hWnd,IDC_STATICM,CT_keypos,false);
			delete [] strbuffer;

			SetFocus(GetDlgItem(hWnd,IDC_EDIT1));
			PostMessage(GetDlgItem(hWnd,IDC_EDIT1),EM_SETSEL,0,10);
*/
			return FALSE;

		case WM_COMMAND:
		/*	if (LOWORD(wParam) == IDOK) 
			{
				strbuffer=new char[20];
				GetDlgItemText(hWnd,IDC_EDIT1,strbuffer,20);
				float *sux=(float *)CT_data;
				*sux=(float)atof(strbuffer);
				delete [] strbuffer;
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			if (LOWORD(wParam) == IDCANCEL) 
			{*/
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			//}
			break;
	}
    return FALSE;
}


void efx2D::edit(HWND mainwin)
{
	int result;

//	CT_keynum=keynum;
//	CT_keypos=keys->keyPos(keynum);
//	CT_data=keys->keyGetData(keynum);
	result=DialogBox((HINSTANCE)hmodulo,MAKEINTRESOURCE(IDD_EDITEFXDIALOG),mainwin,&DialogEditEfx);

	//if (LOWORD(result) == IDOK)
	//{
//		keys->keySetData(keynum,CT_data);
//	}

	//delete [] CT_data;
}
								

void efx2D::init() 
{ initialized=true; }

void efx2D::free() 
{ initialized=false; }

void efx2D::onPaint(double pos) 
{ 
	myActive=(ctBool *)ct.getPtrByIndex(0); 
	// byIndex è + veloce di byName
	// la proprietà active deve avere per forza index = 0
	if (myActive->getValue(float(pos))) paint(pos);
} 

void efx2D::paint(double pos) 
{ } // da overloadare

efx2D::~efx2D(void) 
{ } // destructor