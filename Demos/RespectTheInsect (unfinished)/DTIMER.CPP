// dtimer object v3.0 by HereBit

#include <time.h>
#include <windows.h>
#include <winbase.h>
#include "dtimer.h"


void dtimer::init(double secondi)
{
  lengthsecondi=secondi;
  numsteps=0;
}

void dtimer::start()
{
  totaluclocks=0.0;
  startuclocks=GetTickCount();//clock(); 
}

void dtimer::stop()
{
}

double dtimer::get_timeFromStart()
{
  return double (totaluclocks/((long double)1000.0/*CLOCKS_PER_SEC*/));
}

double dtimer::get_stepsPerSecond()
{
  return double(numsteps)/get_timeFromStart();
}
