#include <io.h>
#include "filebuf.h"

void fileWriter::fileOpen(const char *filename)
{
	myfile=fopen(filename,"wb");
}

void fileWriter::fileClose()
{
	fclose(myfile);
}

void fileWriter::writeBuffer(const void *buf, int size)
{
	if (size>0)
	fwrite(buf,size,1,myfile);
}

void fileWriter::writeChar(char chr)
{
	putc(chr,myfile);
}

void fileWriter::writeString(const char *str)
{
	fputs(str,myfile);
	putc(0,myfile);
}

void fileWriter::writeInt(int num)
{
	writeBuffer(&num,sizeof(int));
}

void fileWriter::writeFloat(float num)
{
	writeBuffer(&num,sizeof(float));
}

int fileWriter::fileSize()
{
	return filelength(fileno(myfile));
}

//fileReader

void fileReader::fileOpen(const char *filename)
{
	myfile=fopen(filename,"rb");
}

void fileReader::fileClose()
{
	fclose(myfile);
}

void fileReader::readBuffer(void *buf, int size)//buf deve essere già allocato
{
	if (size>0)
	fread(buf,size,1,myfile);
}

void fileReader::readChar(char *chr)
{
	*chr=getc(myfile);
}

void fileReader::readString(char *str)
{
	//char *mystr=new char[256];
	//fgets(*str,100,myfile);
	//fscanf(myfile,"%s",*str);
	//getc(myfile);
	int i=0;
	do {
		str[i]=getc(myfile);
		i++;
	} while (str[i-1]!=0);
}

void fileReader::readInt(int *num)
{
	readBuffer(num,sizeof(int));
}

void fileReader::readFloat(float *num)
{
	readBuffer(num,sizeof(float));
}

int fileReader::fileSize()
{
	return filelength(fileno(myfile));
}


