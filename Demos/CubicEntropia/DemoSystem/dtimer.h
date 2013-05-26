// dtimer object v3.0

#ifndef _DTIMER_H_
#define _DTIMER_H_

#include <time.h>
#include <windows.h>
#include <winbase.h>
#include "Types.h"

class CTimer
{
private:
  double lengthsecondi;
  long double startuclocks0, startuclocks, totaluclocks;
  int32 numsteps;

public:
  void m_Init(double secondi);
  void m_Start();

  // da usare dopo start():
  inline void m_Step()
  {
    totaluclocks=(long double) (GetTickCount()-startuclocks);
    numsteps++;
  }

  void m_Stop();

  inline double m_GetPos()
  {
    return double ( totaluclocks / ((long double)1000.0/*CLOCKS_PER_SEC*/)
                   /(long double) (lengthsecondi) );
  }

  void m_SetPos(double relpos01)
  {
     //totaluclocks=relpos01*1000.0*lengthsecondi;
	 startuclocks=startuclocks0-relpos01*1000.0*lengthsecondi;
  }

  double m_GetTimeFromStart();

  // da usare dopo stop():
  double m_GetStepsPerSecond();
};

#endif