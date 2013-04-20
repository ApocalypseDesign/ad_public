// classe di base per i buffer v1.0

#ifndef _IMAGE_H_
#define _IMAGE_H_

#ifdef ADBASE_EXPORTS
#define ADBASE_API __declspec(dllexport)
#else
#define ADBASE_API __declspec(dllimport)
#endif

class ADBASE_API image
{
public:
   
   void *voidptr;
   unsigned char *uchar8ptr;
   unsigned int *uint32ptr;
   int size, width, height;
   int *muly;

   image(void);
   image(int sizex, int sizey);
   image(int sizex, int sizey, void *buffer);

   void clear (void);
   void clear (unsigned char value);
   void clear (unsigned char r, unsigned char g, unsigned char b);
};

#endif