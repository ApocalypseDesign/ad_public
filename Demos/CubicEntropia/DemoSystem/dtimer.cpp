// CTimer object v3.0 by HereBit

#include <time.h>
#include <windows.h>
#include <winbase.h>
#include "dtimer.h"


void CTimer::m_Init(double secondi)
{
  lengthsecondi=secondi;
  numsteps=0;
}

//--------------------------------------------------------------------

void CTimer::m_Start()
{
  totaluclocks=0.0;
  startuclocks=GetTickCount();//clock(); 
  startuclocks0=startuclocks;
}

//--------------------------------------------------------------------

void CTimer::m_Stop()
{
}

//--------------------------------------------------------------------

double CTimer::m_GetTimeFromStart()
{
  return double (totaluclocks/((long double)1000.0/*CLOCKS_PER_SEC*/));
}

//--------------------------------------------------------------------

double CTimer::m_GetStepsPerSecond()
{
  return double(numsteps)/m_GetTimeFromStart();
}