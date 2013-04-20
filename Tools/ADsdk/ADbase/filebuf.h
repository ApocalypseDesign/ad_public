#ifndef _FILEBUF_H_
#define _FILEBUF_H_

#ifdef ADBASE_EXPORTS
#define ADBASE_API __declspec(dllexport)
#else
#define ADBASE_API __declspec(dllimport)
#endif

#include <stdio.h>

class ADBASE_API fileWriter
{
public:
	void fileOpen(const char *filename);
	void writeBuffer(const void *buf, int size);
	void writeChar(char chr);
	void writeString(const char *str);
	void writeInt(int num);
	void writeFloat(float num);
	int  fileSize();
	void fileClose();
private:
	FILE *myfile;
};

class ADBASE_API fileReader
{
public:
	void fileOpen(const char *filename);
	void readBuffer(void *buf, int size);
	void readChar(char *chr);
	void readString(char *str);
	void readInt(int *num);
	void readFloat(float *num);
	int  fileSize();
	void fileClose();
private:
	FILE *myfile;
};

#endif