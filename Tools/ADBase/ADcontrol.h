//******************************************
// AD - Controllers & Keyframer 2000 alpha2
// by HereBit 
//******************************************

#ifndef _ADCONTROL_H_
#define _ADCONTROL_H_

#include <windows.h>

#ifdef ADBASE_EXPORTS
#define ADBASE_API __declspec(dllexport)
#else
#define ADBASE_API __declspec(dllimport)
#endif

#define MAXKEY 50
#define KEYNUM 1000

// ---- Keyframer centralizzato ----

class ADBASE_API key
{
public:
	key();
	int pos;
	void *data;
	key *next;
};

class ADBASE_API keyList
{
public:
	keyList();
	keyList(int keysize);

	int  keyCount();
	int  keySize();

	void keyClear();
	
	int  keyPos(int num);
	int  keyNum(int pos);

	int  keyBefore(float pos);
	int  keyAfter(float pos);

	void keyAdd(int pos,void *data);
	void keyMove(int num,int newpos);
	void keyDelete(int num);
	
	void *keyGetData(int num);
	void keySetData(int num,void *newdata);

protected:
	int key_size,key_count;
	key *firstkey;
	void keySort();
};


// ---- Definizione e gestione dei controller di base ----

class ADBASE_API controller
{
public:
	char *className;
	char *baseClass;
	keyList *keys;

	controller();
    virtual void *create();
	virtual void *createDefaultKey();
	virtual void editKey(int keynum, HWND mainWin);
	void loadFromMemory(void *data, int size);
	void saveToMemory(void **data, int *size);
	void freeSavedMemory();
private:
	char *saveddata;
};


// ---- 1° Livello della gerarchia ----

class ADBASE_API ctFloat : public controller
{
public:
	ctFloat();
	virtual void *create();
	virtual void editKey(int keynum, HWND mainWin);
	virtual float getValue(float pos);
};

class ADBASE_API ctInt : public controller
{
public:
	ctInt();
	virtual void *create();
    virtual void editKey(int keynum, HWND mainWin);
	virtual int getValue(float pos);
};

class ADBASE_API ctBool : public controller
{
public:
	ctBool();
	virtual void *create();
	virtual void editKey(int keynum, HWND mainWin);
	virtual bool getValue(float pos);
};

/*
class ADBASE_API ctText : public controller
{
public:
	ctText();
	virtual void *create();
	//virtual void editKey(int keynum, HWND mainWin);
	virtual char *getValue(float pos);
};
*/

typedef struct { float x,y; } Vector2D;

class ADBASE_API ctVector2D : public controller
{
public:
	ctVector2D();
	virtual void *create();
	virtual void editKey(int keynum, HWND mainWin);
	virtual Vector2D getValue(float pos);
};


// ---- 2° Livello della gerarchia ----

class ADBASE_API ctFloatConst : public ctFloat
{
public:
	ctFloatConst();
	virtual void *create();
	virtual void *createDefaultKey();
	float getValue(float pos);
};

class ADBASE_API ctIntConst : public ctInt
{
public:
	ctIntConst();
	virtual void *create();
	virtual void *createDefaultKey();
	int getValue(float pos);
};

class ADBASE_API ctBoolConst : public ctBool
{
public:
	ctBoolConst();
	virtual void *create();
	virtual void *createDefaultKey();
	bool getValue(float pos);
};

/*
class ADBASE_API ctTextConst : public ctText
{
public:
	ctTextConst();
	virtual void *create();
	virtual void *createDefaultKey();
	char *getValue(float pos);
};
*/


class ADBASE_API ctVector2DConst : public ctVector2D
{
public:
	ctVector2DConst();
	virtual void *create();
	virtual void *createDefaultKey();
	Vector2D getValue(float pos);
};

// LINEAR

class ADBASE_API ctFloatLinear : public ctFloatConst
{
public:
	ctFloatLinear();
	virtual void *create();
	float getValue(float pos);
};

class ADBASE_API ctIntLinear : public ctIntConst
{
public:
	ctIntLinear();
	virtual void *create();
	int getValue(float pos);
};

class ADBASE_API ctVector2DLinear : public ctVector2DConst
{
public:
	ctVector2DLinear();
	virtual void *create();
	Vector2D getValue(float pos);
};

/*

//**** controller complessi ****

//ctBoolRange

typedef struct
{
	int keyType; 
	float pos; // da 0.0 a 1.0
}	BoolRange_Key;

class ADBASE_API ctBoolRange : public ctBool
{
public:
	virtual void *create();
	ctBoolRange();
    ctBoolRange(bool value);
	void loadFromMemory(void *data, int size);
	void saveToMemory(void **data, int *size);
	void freeSavedMemory();
	bool getValue(float pos);
private:
	int numkeys;
	BoolRange_Key keys[MAXKEY];
	BoolRange_Key *tempKeys;
	void sortKeys();
};


//ctFloatTCB

typedef struct	
{
	float tension, bias, continuity;
	float easeto, easefrom;
	float incomtg;
	float outcomtg;
	int   posintrack;
	float data;
} FloatTCB_Key, *FloatTCB_Key_ptr;


class ADBASE_API ctFloatTCB : public ctFloat
{
public:
	ctFloatTCB();
	void *create();
	float getValue(float pos);

	void loadFromMemory(void *data, int size);
	void saveToMemory(void **data, int *size);
	void freeSavedMemory() { delete [] export; }

	void add_key (int pos, float v, float t, float c, float b, float et, float ef);
	void add_key (int pos, float v, float t, float c, float b);
	void add_key (int pos, float v);
	void update_key (int witch, int newpos, float newv, float newt, float newc,
	                 float newb, float newet, float newef);
	void update_key (int witch, int newpos, float newv, float newt, float newc, float newb);
	void update_key (int witch, int newpos, float newv);
	void delete_key (int witch);

private:
	int numkeys;
	FloatTCB_Key **mykeys;
	FloatTCB_Key_ptr export;

	void sort_keys(void);
	void precalc_derivates(void);
	float spline_ease (float t, float easefrom, float easeto);
};


*/
#endif