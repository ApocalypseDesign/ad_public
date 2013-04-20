#ifndef _PTRLIST_H_
#define _PTRLIST_H_

#ifdef ADBASE_EXPORTS
#define ADBASE_API __declspec(dllexport)
#else
#define ADBASE_API __declspec(dllimport)
#endif

class ptrItem
{
public:
	void *ptr;
	char *name;
	char *description;
	ptrItem *next;
	ptrItem();
};

class ADBASE_API ptrList
{
private: 
	ptrItem *first;
	ptrItem *last;

public:
	int count;

	ptrList();
	int addItem(void *cPtr,char *name,char *description);
	void setItem(int num,void *cPtr,char *name,char *description);
	void setItem(int num,void *cPtr);
	void setItem(char *name,void *cPtr);
	int getItemIndexByName(char *name);
	bool ptrList::ItemNameExists(char *name);
	void *getPtrByName(char *name);
	void *getPtrByIndex(int index);
	char *getName(int index);
	char *getDescription(int index);
	void deleteItem(int index);
	void moveUpItem(int index);
	void moveDownItem(int index);
};

#endif