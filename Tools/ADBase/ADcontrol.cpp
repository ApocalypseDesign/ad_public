#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>


#include "ADcontrol.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ADbasic.h"
#include "ADbase.h"
#include "Resource.h"

#define PI 3.14159265358979f
extern HANDLE hmodulo;

// per uso interno delle editbox:
int CT_keynum,CT_keypos;
void *CT_data;


// keyframer


key::key()
{
	pos=0;
	data=NULL;
	next=NULL;
}

keyList::keyList()
{
	key_size=0;
	key_count=0;
	firstkey=NULL;
}

keyList::keyList(int keysize)
{
	key_size=keysize;
	key_count=0;
	firstkey=NULL;
}

int keyList::keyCount()
{
	return key_count;
}

int keyList::keySize()
{
	return key_size;
}

void keyList::keyClear()
{
	key *tmpkey;
	key *tmpnext;

	tmpkey=firstkey;
	while (tmpkey!=NULL)
	{
		tmpnext=tmpkey->next;
		delete [] tmpkey->data;
		delete [] tmpkey;
		tmpkey=tmpnext;
	}
	key_count=0;
}

	
int keyList::keyPos(int num)
{
	int i;
	key *tmpkey;

	if ((num>=0) && (num<key_count))
	{
		tmpkey=firstkey;
		for (i=0;i<num;i++)
			tmpkey=tmpkey->next;
		return tmpkey->pos;
	}
	return 0;
}

int keyList::keyNum(int pos)
{
	int i;
	key *tmpkey;

	if (key_count==0) return -1;
	tmpkey=firstkey;
	for (i=1;i<key_count;i++)
	{
		if (tmpkey->pos==pos) return i-1;
		tmpkey=tmpkey->next;
	}
	return -1;
}

int keyList::keyBefore(float pos)
{
	int i;
	key *tmpkey;

	if (key_count==0) return -1;
	tmpkey=firstkey;

	for (i=0;i<key_count;i++)
	{
		if (tmpkey->pos>pos) return i-1;
		tmpkey=tmpkey->next;
	}
	return key_count-1;
}

int keyList::keyAfter(float pos)
{
	int i;
	key *tmpkey;

	if (key_count==0) return -1;
	tmpkey=firstkey;
	for (i=0;i<key_count;i++)
	{
		if (pos<tmpkey->pos) return i;
		tmpkey=tmpkey->next;
	}
	return -1;
}

void keyList::keyAdd(int pos, void *data)
{
	key *tmpkey=new key;

	tmpkey->pos=pos;
	tmpkey->next=firstkey;
	tmpkey->data=data;
    firstkey=tmpkey;
	key_count++;
	keySort();
}

void keyList::keyMove(int num,int newpos)
{
	int i;
	key *tmpkey;

	if ((num<0)||(num>=key_count)) return;

	tmpkey=firstkey;
	for (i=0;i<num;i++) tmpkey=tmpkey->next;
	tmpkey->pos=newpos;
    keySort();
}

void keyList::keyDelete(int num)
{
	int i;
	key *tmpkey;
	key *tmp2;

	if (num==0)
	{
		tmp2=firstkey;
		firstkey=firstkey->next;
		delete [] tmp2->data;
		delete [] tmp2;
	}
    else
	{
		tmpkey=firstkey;
		for (i=0;i<num-1;i++) tmpkey=tmpkey->next;
		tmp2=tmpkey->next;
		tmpkey->next=tmpkey->next->next;
		delete [] tmp2->data;
		delete [] tmp2;
	}
	key_count--;
    keySort();
}
	
void *keyList::keyGetData(int num)
{
	int i;
	key *tmpkey;
	char *tmpdata=new char[key_size];

	tmpkey=firstkey;
	for (i=1;i<=num;i++) tmpkey=tmpkey->next;
    memcpy(tmpdata,tmpkey->data,key_size);
	return tmpdata;
}

void keyList::keySetData(int num,void *newdata)
{
	int i;
	key *tmpkey;

	tmpkey=firstkey;
	for (i=1;i<=num;i++) tmpkey=tmpkey->next;
	memcpy(tmpkey->data,newdata,key_size);
}

void keyList::keySort() // bubblesort molto suxante, ottimizzare!!!
{
	key *tmp1;
	key *tmp2;
	key tmp;
	bool scambio=true;

   if (key_count<=1) return;

   while (scambio)	
   {
	tmp1=firstkey;
	tmp2=tmp1->next;
	scambio=false;
	while (tmp2!=NULL)
	{
		if (tmp1->pos>tmp2->pos)
		{
			tmp.pos=tmp1->pos;
			tmp.data=tmp1->data;
			tmp1->pos=tmp2->pos;
			tmp1->data=tmp2->data;
			tmp2->pos=tmp.pos;
			tmp2->data=tmp.data;
			scambio=true;
		}
		tmp1=tmp2;
		tmp2=tmp2->next;
	}
   }
}


// controller

controller::controller() 
{ 
	baseClass="controller"; 
	className="controller"; 
	keys=new keyList(0);
}

void *controller::create() 
{ return new controller; }

void controller::editKey(int keynum, HWND mainWin)
{ }

void *controller::createDefaultKey()
{
	return NULL;
}

void controller::loadFromMemory(void *data, int size)
{
	int keysize=keys->keySize(); 
	int num,i;
	char *dataoffs=(char *)data;

	if (size%(keysize+4)!=0) debug_warning(0,"Controller can't load data (data size error)");
	num=size/(keysize+4);

	keys->keyClear();
	for (i=0;i<num;i++)
	{
		char *tmpdata=new char[keysize];
		memcpy(tmpdata,dataoffs+4,keysize);
		keys->keyAdd(*(int *)(dataoffs),tmpdata);
		dataoffs+=keysize+4;
	}
}

void controller::saveToMemory(void **data, int *size)
{
	int keysize=keys->keySize(); // anche la pos va salvata
	int num=keys->keyCount();

	saveddata=new char[num*(keysize+4)];

	char *dataoffs=saveddata;
	char *datasrc;
	int i,j;

	for (i=0;i<num;i++)
	{
		*(int *)(dataoffs)=keys->keyPos(i);
		dataoffs+=4;
		datasrc=(char *)keys->keyGetData(i);
		for (j=0;j<keysize;j++) dataoffs[j]=datasrc[j];
		dataoffs+=keysize;
	}

	*data=saveddata;
	*size=num*(keysize+4);
}

void controller::freeSavedMemory()
{
	delete [] saveddata;
}

// ctFloat

ctFloat::ctFloat() 
{ className="ctFloat"; baseClass="ctFloat"; }

void *ctFloat::create() 
{ return new ctFloat; }

int FAR PASCAL
DialogFloat(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 
 char *strbuffer;
 
 switch (message)
	{
		case WM_INITDIALOG:
			strbuffer=new char[20];
			strbuffer=gcvt((double)(*(float *)(CT_data)),3,strbuffer);
			

            SetDlgItemText(hWnd,IDC_EDIT1,strbuffer);
			SetDlgItemInt(hWnd,IDC_STATICN,CT_keynum,false);
			SetDlgItemInt(hWnd,IDC_STATICM,CT_keypos,false);
			delete [] strbuffer;

			SetFocus(GetDlgItem(hWnd,IDC_EDIT1));
			PostMessage(GetDlgItem(hWnd,IDC_EDIT1),EM_SETSEL,0,10);

			return FALSE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK) 
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
			{
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


void ctFloat::editKey(int keynum, HWND mainWin)
{
	int result;

	CT_keynum=keynum;
	CT_keypos=keys->keyPos(keynum);
	CT_data=keys->keyGetData(keynum);
	result=DialogBox((HINSTANCE)hmodulo,MAKEINTRESOURCE(IDD_DIALOGFLOAT),mainWin,&DialogFloat);

	if (LOWORD(result) == IDOK)
	{
		keys->keySetData(keynum,CT_data);
	}

	delete [] CT_data;
}

float ctFloat::getValue(float pos) 
{ return 0.0f; }

// ctInt

ctInt::ctInt() 
{ className="ctInt"; baseClass="ctInt"; }

void *ctInt::create() 
{ return new ctInt; }

int FAR PASCAL
DialogInt(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
	{
		case WM_INITDIALOG:
	

            SetDlgItemInt(hWnd,IDC_EDIT1,*(int *)(CT_data),false);
			SetDlgItemInt(hWnd,IDC_STATICN,CT_keynum,false);
			SetDlgItemInt(hWnd,IDC_STATICM,CT_keypos,false);

			SetFocus(GetDlgItem(hWnd,IDC_EDIT1));
			PostMessage(GetDlgItem(hWnd,IDC_EDIT1),EM_SETSEL,0,10);

			return FALSE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK) 
			{
				int transl;

				*(int *)(CT_data)=GetDlgItemInt(hWnd,IDC_EDIT1,&transl,true);
			
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


void ctInt::editKey(int keynum, HWND mainWin)
{
	int result;

	CT_keynum=keynum;
	CT_keypos=keys->keyPos(keynum);
	CT_data=keys->keyGetData(keynum);
	result=DialogBox((HINSTANCE)hmodulo,MAKEINTRESOURCE(IDD_DIALOGINT),mainWin,&DialogInt);

	if (LOWORD(result) == IDOK)
	{
		keys->keySetData(keynum,CT_data);
	}

	delete [] CT_data;
}

int ctInt::getValue(float pos) 
{ return 0; };

// ctBool

ctBool::ctBool() 
{ className="ctBool"; baseClass="ctBool"; }

void *ctBool::create()
{ return new ctBool; }

int FAR PASCAL
DialogBool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const char *strTrue="True";
	const char *strFalse="False";
 switch (message)
	{
		case WM_INITDIALOG:

			SendDlgItemMessage(hWnd,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)strTrue);
			SendDlgItemMessage(hWnd,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)strFalse);

			if (*(bool *)(CT_data)==true)
			   SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, 0, (LPARAM)0);
			else SendDlgItemMessage(hWnd, IDC_COMBO1, CB_SETCURSEL, 1, (LPARAM)0);

			SetDlgItemInt(hWnd,IDC_STATICN,CT_keynum,false);
			SetDlgItemInt(hWnd,IDC_STATICM,CT_keypos,false);

			SetFocus(GetDlgItem(hWnd,IDC_COMBO1));

			return FALSE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK) 
			{

				if (SendDlgItemMessage(hWnd, IDC_COMBO1, CB_GETCURSEL, 0, (LPARAM)0)==0)
					*(bool *)(CT_data)=true;
				else *(bool *)(CT_data)=false;
			
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


void ctBool::editKey(int keynum, HWND mainWin)
{
	int result;

	CT_keynum=keynum;
	CT_keypos=keys->keyPos(keynum);
	CT_data=keys->keyGetData(keynum);
	result=DialogBox((HINSTANCE)hmodulo,MAKEINTRESOURCE(IDD_DIALOGBOOL),mainWin,&DialogBool);

	if (LOWORD(result) == IDOK)
	{
		keys->keySetData(keynum,CT_data);
	}

	delete [] CT_data;
}

bool ctBool::getValue(float pos)
{ return 0; }

// ctText

/*
ctText::ctText() 
{ className="ctText"; baseClass="ctText"; }

void *ctText::create() 
{ return new ctText; }

char *ctText::getValue(float pos) 
{ return ""; }
*/

// ctVector2D

ctVector2D::ctVector2D() 
{ className="ctVector2D"; baseClass="ctVector2D"; }

void *ctVector2D::create() 
{ return new ctVector2D; }

int FAR PASCAL
DialogVector2D(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 
 char *strbuffer;
 Vector2D *vtemp;
 
 switch (message)
	{
		case WM_INITDIALOG:
			strbuffer=new char[20];
			vtemp=(Vector2D *)(CT_data);
			strbuffer=gcvt((double)(vtemp->x),3,strbuffer);
            SetDlgItemText(hWnd,IDC_EDIT1,strbuffer);
			strbuffer=gcvt((double)(vtemp->y),3,strbuffer);
            SetDlgItemText(hWnd,IDC_EDIT2,strbuffer);

			SetDlgItemInt(hWnd,IDC_STATICN,CT_keynum,false);
			SetDlgItemInt(hWnd,IDC_STATICM,CT_keypos,false);
			delete [] strbuffer;

			SetFocus(GetDlgItem(hWnd,IDC_EDIT1));
			PostMessage(GetDlgItem(hWnd,IDC_EDIT1),EM_SETSEL,0,10);

			return FALSE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK) 
			{
				strbuffer=new char[20];
				GetDlgItemText(hWnd,IDC_EDIT1,strbuffer,20);
				vtemp=(Vector2D *)CT_data;
				vtemp->x=(float)atof(strbuffer);
				GetDlgItemText(hWnd,IDC_EDIT2,strbuffer,20);
				vtemp->y=(float)atof(strbuffer);
				delete [] strbuffer;
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


void ctVector2D::editKey(int keynum, HWND mainWin)
{
	int result;

	CT_keynum=keynum;
	CT_keypos=keys->keyPos(keynum);
	CT_data=keys->keyGetData(keynum);
	result=DialogBox((HINSTANCE)hmodulo,MAKEINTRESOURCE(IDD_DIALOGVECTOR2D),mainWin,&DialogVector2D);

	if (LOWORD(result) == IDOK)
	{
		keys->keySetData(keynum,CT_data);
	}

	delete [] CT_data;
}


Vector2D ctVector2D::getValue(float pos) 
{
	Vector2D vtemp;

	vtemp.x=0.0f;
	vtemp.y=0.0f;
	return vtemp; 
} 

// ctFloatConst

ctFloatConst::ctFloatConst()
{
	className="ctFloatConst";
	delete [] keys;
	keys=new keyList(sizeof(float));
	//float *pippo=new float;
	//*pippo=1.0f;
	//keys->keyAdd(0,pippo);
}

void *ctFloatConst::create() 
{ return new ctFloatConst; }

void *ctFloatConst::createDefaultKey()
{
	float *temp=new float;
	*temp=0.0f;//(float)(rand())/(float)(RAND_MAX);
	return temp;
}

float ctFloatConst::getValue(float pos)
{
	int num=keys->keyCount();
	int after,before;

	if (num==0) return 0.0f;
	else if (num==1) return *(float *)(keys->keyGetData(0));

	before=keys->keyBefore(pos*KEYNUM);
	after=keys->keyAfter(pos*KEYNUM);
	if (before!=-1) return *(float *)(keys->keyGetData(before));
	if (after!=-1) return *(float *)(keys->keyGetData(after));
	return 0.0f;
}

// ctIntConst


ctIntConst::ctIntConst()
{
	className="ctIntConst";
	delete [] keys;
	keys=new keyList(sizeof(int));
}

void *ctIntConst::create() 
{ return new ctIntConst; }

void *ctIntConst::createDefaultKey()
{
	int *temp=new int;
	*temp=0;
	return temp;
}

int ctIntConst::getValue(float pos)
{
	int num=keys->keyCount();
	int after,before;

	if (num==0) return 0;
	if (num==1) return *(int *)(keys->keyGetData(0));

	before=keys->keyBefore(pos*KEYNUM);
	after=keys->keyAfter(pos*KEYNUM);
	if (before!=-1) return *(int *)(keys->keyGetData(before));
	if (after!=-1) return *(int *)(keys->keyGetData(after));
	return 0;
}

// ctBoolConst

ctBoolConst::ctBoolConst()
{
	className="ctBoolConst";
    delete [] keys;
	keys=new keyList(sizeof(bool));
}

void *ctBoolConst::create() 
{ return new ctBoolConst; }

void *ctBoolConst::createDefaultKey()
{
	bool *temp=new bool;
	*temp=true;
	return temp;
}

bool ctBoolConst::getValue(float pos)
{
	int num=keys->keyCount();
	int after,before;

	if (num==0) return true;
	if (num==1) return *(bool *)(keys->keyGetData(0));

	before=keys->keyBefore(pos*KEYNUM);
	after=keys->keyAfter(pos*KEYNUM);
	if (before!=-1) return *(bool *)(keys->keyGetData(before));
	if (after!=-1) return *(bool *)(keys->keyGetData(after));
	return true;
}

// ctTextConst

/*
ctTextConst::ctTextConst()
{
	className="ctTextConst";
	delete [] keys;
	keys=new keyList(sizeof(char *));
}

void *ctTextConst::create() 
{ return new ctTextConst; }

void *ctTextConst::createDefaultKey()
{
	//todo
	return NULL;
}

char *ctTextConst::getValue(float pos)
{
	int num=keys->keyCount();
	int after,before;
	static char deftext[16]="Default text";

	if (num==0) return deftext;
	if (num==1) return (char *)(keys->keyGetData(0));

	before=keys->keyBefore(pos*KEYNUM);
	after=keys->keyAfter(pos*KEYNUM);
	if (before!=-1) return (char *)(keys->keyGetData(before));
	if (after!=-1) return (char *)(keys->keyGetData(after));
	return deftext;
}
*/

// ctVector2DConst

ctVector2DConst::ctVector2DConst()
{
	className="ctVector2DConst";
	delete [] keys;
	keys=new keyList(sizeof(Vector2D));
}

void *ctVector2DConst::create() 
{ return new ctVector2DConst; }

void *ctVector2DConst::createDefaultKey()
{
	Vector2D *temp=new Vector2D;
	temp->x=0.0f;
	temp->y=0.0f;
	return temp;
}

Vector2D ctVector2DConst::getValue(float pos)
{
	int num=keys->keyCount();
	int after,before;
	static Vector2D defvect;
	defvect.x=0.0f;
	defvect.y=0.0f;

	if (num==0) return defvect;
	if (num==1) return *(Vector2D *)(keys->keyGetData(0));

	before=keys->keyBefore(pos*KEYNUM);
	after=keys->keyAfter(pos*KEYNUM);
	if (before!=-1) return *(Vector2D *)(keys->keyGetData(before));
	if (after!=-1) return *(Vector2D *)(keys->keyGetData(after));
	return defvect;
}

//LINEAR

ctFloatLinear::ctFloatLinear()
{
	className="ctFloatLinear";
	delete [] keys;
	keys=new keyList(sizeof(float));
}

void *ctFloatLinear::create() 
{ return new ctFloatLinear; }

float ctFloatLinear::getValue(float pos)
{
	int num=keys->keyCount();
	int after,before;

	if (num==0) return 0.0f;
	else if (num==1) return *(float *)(keys->keyGetData(0));

	before=keys->keyBefore(pos*KEYNUM);
	after=keys->keyAfter(pos*KEYNUM);

	if ((before!=-1) && (after!=-1)) 
	{
		float divid=float(keys->keyPos(after)-keys->keyPos(before));
		if (divid==0) divid=0.00001f;
		float perc=(pos*1000.0f-keys->keyPos(before)) /divid;
		return (*(float *)(keys->keyGetData(before)))*(1.0f-perc)+
			   (*(float *)(keys->keyGetData(after)))*perc;
	}
	
	if (before!=-1) return *(float *)(keys->keyGetData(before));
	if (after!=-1) return *(float *)(keys->keyGetData(after));
	return 0.0f;
}


ctIntLinear::ctIntLinear()
{
	className="ctIntLinear";
	delete [] keys;
	keys=new keyList(sizeof(int));
}

void *ctIntLinear::create() 
{ return new ctIntLinear; }

int ctIntLinear::getValue(float pos)
{
	int num=keys->keyCount();
	int after,before;

	if (num==0) return 0;
	else if (num==1) return *(int *)(keys->keyGetData(0));

	before=keys->keyBefore(pos*KEYNUM);
	after=keys->keyAfter(pos*KEYNUM);

	if ((before!=-1) && (after!=-1)) 
	{
		float divid=float(keys->keyPos(after)-keys->keyPos(before));
		if (divid==0) divid=0.00001f;
		float perc=(pos*1000.0f-keys->keyPos(before)) /divid;
		return (int)((float)(*(int *)(keys->keyGetData(before)))*(1.0f-perc)+
			         (float)(*(int *)(keys->keyGetData(after)))*perc+0.5f);
	}
	
	if (before!=-1) return *(int *)(keys->keyGetData(before));
	if (after!=-1) return *(int *)(keys->keyGetData(after));
	return 0;
}


ctVector2DLinear::ctVector2DLinear()
{
	className="ctVector2DLinear";
	delete [] keys;
	keys=new keyList(sizeof(Vector2D));
}

void *ctVector2DLinear::create() 
{ return new ctVector2DLinear; }

Vector2D ctVector2DLinear::getValue(float pos)
{
	int num=keys->keyCount();
	int after,before;
	Vector2D vtemp,*v1,*v2;

	vtemp.x=0.0f;
	vtemp.y=0.0f;

	if (num==0) return vtemp;
	else if (num==1) return *(Vector2D *)(keys->keyGetData(0));

	before=keys->keyBefore(pos*KEYNUM);
	after=keys->keyAfter(pos*KEYNUM);

	if ((before!=-1) && (after!=-1)) 
	{
		float divid=float(keys->keyPos(after)-keys->keyPos(before));
		if (divid==0) divid=0.00001f;
		float perc=(pos*1000.0f-keys->keyPos(before)) /divid;
		v1=(Vector2D *)(keys->keyGetData(before));
		v2=(Vector2D *)(keys->keyGetData(after));
		vtemp.x= v1->x*(1.0f-perc) + v2->x*perc;
		vtemp.y= v1->y*(1.0f-perc) + v2->y*perc;
		return vtemp;
	}
	
	if (before!=-1) return *(Vector2D *)(keys->keyGetData(before));
	if (after!=-1) return *(Vector2D *)(keys->keyGetData(after));
	return vtemp;
}

/*
//**** Controller complessi ****


//ctFloatTCB

ctFloatTCB::ctFloatTCB()
{ 
    className="ctFloatTCB";
	baseClass="ctFloat";
	numkeys=0;
    mykeys=new FloatTCB_Key_ptr[MAXKEY];
	for (int h=0; h<MAXKEY; h++) mykeys[h]=(FloatTCB_Key_ptr)NULL;

	add_key(  0,0.0f ,1.0f,1.0f,0.0f);
	add_key(100,0.2f ,1.0f,1.0f,0.0f);
	add_key(200,0.0f ,1.0f,1.0f,0.0f);
	add_key(300,0.4f ,1.0f,1.0f,0.0f);
	add_key(400,0.0f ,1.0f,1.0f,0.0f);
	add_key(500,0.6f ,1.0f,1.0f,0.0f);
	add_key(600,0.0f ,1.0f,1.0f,0.0f);
	add_key(700,0.8f ,1.0f,1.0f,0.0f);
	add_key(800,0.0f ,1.0f,1.0f,0.0f);
	add_key(1000,1.0f ,1.0f,1.0f,0.0f);
}

void *ctFloatTCB::create() 
{ return new ctFloatTCB; }

float ctFloatTCB::spline_ease (float t, float easefrom, float easeto)
{
  float k, s;

  s=easeto+easefrom;
  if (s==0) return(t);
  if (s>1.0)
  {
    easeto=easeto/s;
    easefrom=easefrom/s;
  }
  k=1.0f/(2.0f-easeto-easefrom);
  if (t<easefrom) return((k/easefrom)*t*t);
  else
  {
    if (t<(1.0-easeto)) return(k*(2.0f*t-easefrom));
    else
    {
      t=1.0f-t;
      return(1.0f-(k/easeto)*t*t);
    }
  }
}


void ctFloatTCB::sort_keys(void)
{
  FloatTCB_Key_ptr p;
  int i, j;

  for (i=0; i<numkeys-1; i++)
  {
	for (j=i+1; j<numkeys; j++)
	{
	  if (mykeys[i]->posintrack > mykeys[j]->posintrack)
	  {
	    p=mykeys[i];
        mykeys[i]=mykeys[j];
        mykeys[j]=p;
	  }
	}
  }
}


void ctFloatTCB::precalc_derivates(void)
{
  int j, i2, i1, i0;
  float c1, c2, csi, cso, k, o;

  if (numkeys<=1) return;
  if (numkeys==2)
  {
    c1=1.0f-mykeys[0]->tension;
    mykeys[0]->outcomtg=c1*(mykeys[1]->data-mykeys[0]->data);

    c2=1.0f-mykeys[1]->tension;
    mykeys[1]->incomtg=c1*(mykeys[1]->data-mykeys[0]->data);
  }
  else
  // caso di 3 o piu' key
  for (j=0; j<numkeys; j++)
  {
    // caso della prima key
    if (j==0)
    {
      i2=j+2;
      i1=j+1;
      i0=j;
	  csi=(float)mykeys[i2]->posintrack-mykeys[i0]->posintrack;
	  cso=(float)mykeys[i1]->posintrack-mykeys[i0]->posintrack;

	  k=mykeys[i2]->data - mykeys[i0]->data;
      o=mykeys[i1]->data - mykeys[i0]->data;
	  k*=-cso/(2.0f*csi);
	  o*=3.0f/2.0f;
	  mykeys[i0]->outcomtg=o+k;
	  mykeys[i0]->outcomtg*=(1.0f-mykeys[i0]->tension);
    }
    else
    {
	  // caso dell'ultima key
	  if (j==numkeys-1)
	  {
	    i2=j-2;
	    i1=j-1;
	    i0=j;
		csi=(float)mykeys[i0]->posintrack-mykeys[i2]->posintrack;
	    cso=(float)mykeys[i0]->posintrack-mykeys[i1]->posintrack;

	    k=mykeys[i0]->data - mykeys[i2]->data;
        o=mykeys[i0]->data - mykeys[i1]->data;
	    k*=-cso/(2.0f*csi);
	    o*=3.0f/2.0f;
	    mykeys[i0]->incomtg=o+k;
	    mykeys[i0]->incomtg*=(1.0f-mykeys[i0]->tension);
	  }
	  else
	  // caso di una key intermezza
	  {
	    i2=j+1;
	    i1=j;
	    i0=j-1;
	    csi=2.0f*(mykeys[j+1]->posintrack-mykeys[j]->posintrack)/(mykeys[j+1]->posintrack-mykeys[j-1]->posintrack);
	    cso=2.0f*(mykeys[j]->posintrack-mykeys[j-1]->posintrack)/(mykeys[j+1]->posintrack-mykeys[j-1]->posintrack);

        // CALCOLO TANGENTE OUTCOMING
        c1=cso*(1.0f-mykeys[j]->tension)*
               (1.0f-mykeys[j]->continuity)*
               (1.0f-mykeys[j]->bias)/2.0f;

        c2=cso*(1.0f-mykeys[j]->tension)*
               (1.0f+mykeys[j]->continuity)*
               (1.0f+mykeys[j]->bias)/2.0f;

        mykeys[j]->outcomtg=c1*(mykeys[i2]->data-mykeys[i1]->data) +
			                c2*(mykeys[i1]->data-mykeys[i0]->data);


        // CALCOLO TANGENTE INCOMING
        c1=csi*(1.0f-mykeys[j]->tension)*
               (1.0f+mykeys[j]->continuity)*
               (1.0f-mykeys[j]->bias)/2.0f;

        c2=csi*(1.0f-mykeys[j]->tension)*
               (1.0f-mykeys[j]->continuity)*
               (1.0f+mykeys[j]->bias)/2.0f;

        mykeys[j]->incomtg=c1*(mykeys[i2]->data-mykeys[i1]->data) +
			               c2*(mykeys[i1]->data-mykeys[i0]->data);
	  }
	}
  }
}

void ctFloatTCB::loadFromMemory(void *data, int size)
{
	int i;
	FloatTCB_Key_ptr import=(FloatTCB_Key_ptr)data;

	for (i=0;i<numkeys;i++) delete mykeys[i];

	numkeys=size/sizeof(FloatTCB_Key);
	for (i=0;i<numkeys;i++)
	{
		mykeys[i]=(FloatTCB_Key_ptr)new FloatTCB_Key;
		*mykeys[i]=import[i];
	}

	sort_keys();
	precalc_derivates();
}

void ctFloatTCB::saveToMemory(void **data, int *size)
{
	int i;
	export=new FloatTCB_Key[numkeys];

	for (i=0;i<numkeys;i++) export[i]=*mykeys[i];
    
    *data=export;
	*size=sizeof(FloatTCB_Key)*numkeys;
}


void ctFloatTCB::add_key (int pos, float v, float t, float c, float b, float et, float ef)
{
  if (numkeys>=MAXKEY) return;
  mykeys[numkeys] = (FloatTCB_Key_ptr)new FloatTCB_Key;

  mykeys[numkeys]->posintrack=pos;
  mykeys[numkeys]->data=v;
  mykeys[numkeys]->tension=t;
  mykeys[numkeys]->continuity=c;
  mykeys[numkeys]->bias=b;
  mykeys[numkeys]->easefrom=ef;
  mykeys[numkeys]->easeto=et;

  numkeys++;
  sort_keys();
  precalc_derivates();
}


void ctFloatTCB::add_key (int pos, float v, float t, float c, float b)
{
  if (numkeys>=MAXKEY) return;
  mykeys[numkeys] = (FloatTCB_Key_ptr)new FloatTCB_Key;

  mykeys[numkeys]->posintrack=pos;
  mykeys[numkeys]->data=v;
  mykeys[numkeys]->tension=t;
  mykeys[numkeys]->continuity=c;
  mykeys[numkeys]->bias=b;
  mykeys[numkeys]->easefrom=0;
  mykeys[numkeys]->easeto=0;

  numkeys++;
  sort_keys();
  precalc_derivates();
}


void ctFloatTCB::add_key (int pos, float v)
{
  if (numkeys>=MAXKEY) return;
  mykeys[numkeys] = (FloatTCB_Key_ptr)new FloatTCB_Key;

  mykeys[numkeys]->posintrack=pos;
  mykeys[numkeys]->data=v;
  mykeys[numkeys]->tension=0;
  mykeys[numkeys]->continuity=0;
  mykeys[numkeys]->bias=0;
  mykeys[numkeys]->easefrom=0;
  mykeys[numkeys]->easeto=0;

  numkeys++;
  sort_keys();
  precalc_derivates();
}


void ctFloatTCB::update_key (int witch, int newpos, float newv, float newt, float newc,
	              float newb, float newet, float newef)
{
  if (witch>=MAXKEY) return;
  if (witch>=numkeys) return;
  if (witch<0) return;

  mykeys[witch]->posintrack=newpos;
  mykeys[witch]->data=newv;
  mykeys[witch]->tension=newt;
  mykeys[witch]->continuity=newc;
  mykeys[witch]->bias=newb;
  mykeys[witch]->easefrom=newef;
  mykeys[witch]->easeto=newet;
  precalc_derivates();
}


void ctFloatTCB::update_key (int witch, int newpos, float newv, float newt, float newc, float newb)
{
  if (witch>=MAXKEY) return;
  if (witch>=numkeys) return;
  if (witch<0) return;

  mykeys[witch]->posintrack=newpos;
  mykeys[witch]->data=newv;
  mykeys[witch]->tension=newt;
  mykeys[witch]->continuity=newc;
  mykeys[witch]->bias=newb;
  precalc_derivates();
}


void ctFloatTCB::update_key (int witch, int newpos, float newv)
{
  if (witch>=MAXKEY) return;
  if (witch>=numkeys) return;
  if (witch<0) return;

  mykeys[witch]->posintrack=newpos;
  mykeys[witch]->data=newv;
  precalc_derivates();
}


void ctFloatTCB::delete_key (int witch)
{
  int k;

  if (witch>=MAXKEY) return;
  if (witch>=numkeys) return;
  if (witch<0) return;

  delete mykeys[witch];
  for (k=witch; k<numkeys-1; k++)
  {
	mykeys[k]=mykeys[k+1];
  }

  mykeys[numkeys]=(FloatTCB_Key_ptr)NULL;
  numkeys--;
  precalc_derivates();
}


float ctFloatTCB::getValue(float pos)
{
  float h1, h2, h3, h4, t, t2, t3;
  float trackpos=pos*1000.0f;
  int intpos, i;

  if (trackpos<0.0) return(0);
  if (mykeys==(FloatTCB_Key_ptr *)NULL) return(0);
  if (numkeys==1) return(mykeys[0]->data);
  if (numkeys<2) return(0);

  intpos=(int)(ceil(trackpos));

  if (trackpos<=mykeys[0]->posintrack) return mykeys[0]->data;

  i=0;
  while ((i<numkeys) && (mykeys[i]->posintrack<intpos))
  {
    i++;
  }
  if (i>=numkeys) return(mykeys[numkeys-1]->data);
  

  if (i==0) i=1;  // caso della key in posizione zero
  t=(trackpos-mykeys[i-1]->posintrack)/(1.0f*mykeys[i]->posintrack-mykeys[i-1]->posintrack);
  t=spline_ease(t, mykeys[i-1]->easefrom, mykeys[i]->easeto);
  t2=t*t;
  t3=t2*t;

  // caloclo delle funzioni base
  h1=2.0f*t3 - 3.0f*t2 + 1.0f;
  h2=-2.0f*t3 + 3.0f*t2;
  h3=t3 - 2.0f*t2 + t;
  h4=t3 - t2;

  return(h1*mykeys[i-1]->data + h2*mykeys[i]->data +
         h3*mykeys[i-1]->outcomtg + h4*mykeys[i]->data);
}


*/