// dtimer object v3.0

#ifndef _DTIMER_H_
#define _DTIMER_H_

#include <time.h>
#include <windows.h>
#include <winbase.h>

class dtimer
{
private:
  double lengthsecondi;
  long double startuclocks,totaluclocks;
  int numsteps;

public:
  void init(double secondi);

  void start();

  // da usare dopo start():

  inline void step()
  {
    totaluclocks=(long double) (GetTickCount()-startuclocks);
    numsteps++;
  }

  void stop();

  inline double get_pos()
  {
    return double ( totaluclocks / ((long double)1000.0/*CLOCKS_PER_SEC*/)
                   /(long double) (lengthsecondi) );
  }

  double get_timeFromStart();

  // da usare dopo stop():

  double get_stepsPerSecond();

};

#endif
