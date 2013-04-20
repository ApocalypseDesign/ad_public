#include <string.h>
#include "ptrlist.h"

ptrItem::ptrItem()
{
	ptr=NULL;
	name=description=NULL;
	next=NULL;
}

ptrList::ptrList()
{  count=0; first=NULL; last=NULL; }

int ptrList::addItem(void *cPtr,char *name,char *description)
{
	ptrItem *temp;

	temp=new ptrItem;
	if (count==0)
	{
		first=last=temp;
		temp->next=NULL;
	} 
	else
	{
		last->next=temp;
		temp->next=NULL;
		last=temp;
	}
	temp->ptr=cPtr;
	temp->name=new char[strlen(name)+1];
	temp->description=new char[strlen(description)+1];
	strcpy(temp->name,name);
	strcpy(temp->description,description);
	count++;
	return count-1;
}

void ptrList::setItem(int num,void *cPtr,char *name,char *description)
{
	ptrItem *temp;
	char *nname=new char[strlen(name)+1];
	char *ndesc=new char[strlen(description)+1];
	int i;

	strcpy(nname,name);strcpy(ndesc,description);

	temp=first;
	if ((num<count) && (num>=0))
	{
		for (i=0; i<num; i++) temp=temp->next;
		temp->ptr=cPtr;
		temp->name=nname;
		temp->description=ndesc;
	}
}

void ptrList::setItem(int num,void *cPtr)
{
	ptrItem *temp;
	int i;

	temp=first;
	if ((num<count) && (num>=0))
	{
		for (i=0; i<num; i++) temp=temp->next;
		temp->ptr=cPtr;
	}
}
	
void ptrList::setItem(char *name,void *cPtr)
{
	ptrItem *temp;

	temp=first;
	while (temp!=NULL)
	{
		if (strcmp(temp->name,name)==0) temp->ptr=cPtr;
		temp=temp->next;
	}
}
	
int ptrList::getItemIndexByName(char *name)
{
	ptrItem *temp;
	int i=0;

	temp=first;
	while (temp!=NULL)
	{
		if (strcmp(temp->name,name)==0) return i;
		temp=temp->next;
		i++;
	}
	return -1;
}

bool ptrList::ItemNameExists(char *name)
{
	ptrItem *temp;
	int i=0;

	temp=first;
	while (temp!=NULL)
	{
		if (strcmp(temp->name,name)==0) return true;
		temp=temp->next;
		i++;
	}
	return false;
}
	
void *ptrList::getPtrByName(char *name)
{
	ptrItem *temp;

	temp=first;
	while (temp!=NULL)
	{
		if (strcmp(temp->name,name)==0) return temp->ptr;
		temp=temp->next;
	}
	return NULL;
}
	
void *ptrList::getPtrByIndex(int index)
{
	ptrItem *temp;
	int i;

	temp=first;
	if ((index<count) && (index>=0))
	{
		for (i=0; i<index; i++) temp=temp->next;
		return temp->ptr;
	}
	else return NULL;
}

char *ptrList::getName(int index)
{
	ptrItem *temp;
	int i;

	temp=first;
	if ((index<count) && (index>=0))
	{
		for (i=0; i<index; i++) temp=temp->next;
		return temp->name;
	}
	else return NULL;
}

char *ptrList::getDescription(int index)
{
	ptrItem *temp;
	int i;

	temp=first;
	if ((index<count) && (index>=0))
	{
		for (i=0; i<index; i++) temp=temp->next;
		return temp->description;
	}
	else return NULL;
}

void ptrList::deleteItem(int index)
{
	ptrItem *temp,*temp2;
	int i;

	if ((count>0) && (index<count))
	if ((count==1) && (index==0))
	{
		delete first;
        first=last=NULL;
		count=0;
	}
	else
	if (index==0)
	{
		temp=first;
		first=first->next;
		delete temp;
		count--;
	}
	else
	if (index==count-1)
	{
		temp=first;
		for (i=0; i<count-1; i++) temp=temp->next;
		delete temp->next;
		temp->next=NULL;
		last=temp;
		count--;
	}
	else
	{
		temp=first;
		for (i=0; i<index-1; i++) temp=temp->next;
		temp2=temp->next;
		temp->next=temp2->next;
		delete temp2;
		count--;
	}
}

void ptrList::moveUpItem(int index)
{
	ptrItem pippo;

	if ((index<=0) || (index>=count)) return;

	pippo.ptr=getPtrByIndex(index-1);
	pippo.name=getName(index-1);
	pippo.description=getDescription(index-1);

	setItem(index-1,getPtrByIndex(index),getName(index),getDescription(index));
	setItem(index,pippo.ptr,pippo.name,pippo.description);
}

void ptrList::moveDownItem(int index)
{
	ptrItem pippo;

	if ((index>=count-1) || (index<0)) return;

	pippo.ptr=getPtrByIndex(index+1);
	pippo.name=getName(index+1);
	pippo.description=getDescription(index+1);

	setItem(index+1,getPtrByIndex(index),getName(index),getDescription(index));
	setItem(index,pippo.ptr,pippo.name,pippo.description);
}
