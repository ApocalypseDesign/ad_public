#include <stdio.h>
#include "AtlBase.h"
#include "DShow.h"
#include "QEdit.h"
#include "VideoReader.h"

extern FILE *fdebug;
void DumpDebugPoint (FILE *f, char8 *msg, HRESULT hr);

CComPtr<IGraphBuilder> pGraphBuilder;
CComPtr<IBaseFilter> pGrabberBaseFilter;
CComPtr<ISampleGrabber> pSampleGrabber;
CComPtr<IMediaControl> pMediaControl;
CComPtr<IMediaEvent> pMediaEventEx;

//--------------------------------------------------------------------

CVideoReader::CVideoReader()
{
  p_VideoIsOpened=0;
  p_VideoInfoAlreadyRead=0;
}

//--------------------------------------------------------------------

int32 CVideoReader::m_OpenVideo(char8 *filename)
{
  AM_MEDIA_TYPE mt;
  WCHAR wFile[150];
  FILE *f;

  if (!filename) return(0);
  f=fopen(filename, "rb");
  if (!f) return(0);
  else fclose(f);

  if (p_VideoIsOpened) m_CloseVideo();

  // creo il graph builder
  HRESULT hr = ::CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
							      IID_IGraphBuilder, (void**)&pGraphBuilder);
  if (FAILED(hr)) return(0);

  // creao il "Grabber filter"
  hr = ::CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, 
					      IID_IBaseFilter, (LPVOID *)&pGrabberBaseFilter);
  if (FAILED(hr)) return(0);
  pGrabberBaseFilter->QueryInterface(IID_ISampleGrabber, (void**)&pSampleGrabber);
  if (pSampleGrabber == NULL) return(0);
  // aggiungo il filtro di grabbing
  hr = pGraphBuilder->AddFilter(pGrabberBaseFilter, NULL);
  if (FAILED(hr)) return(0);

  // specifico il tipo di dato che voglio leggere
  ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
  mt.majortype = MEDIATYPE_Video;
  mt.subtype = MEDIASUBTYPE_RGB32;
  mt.formattype = FORMAT_VideoInfo; 
  if (FAILED(hr = pSampleGrabber->SetMediaType(&mt))) return(0);
  // apro il file e creo il grafo
  MultiByteToWideChar(CP_ACP, 0, filename, -1, wFile, 150);
  hr = pGraphBuilder->RenderFile(wFile, NULL);
  if (FAILED(hr)) return(0);
		
  // QueryInterface for some basic interfaces
  pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&pMediaControl);
  pGraphBuilder->QueryInterface(IID_IMediaEvent, (void **)&pMediaEventEx);
  if (pMediaControl==NULL || pMediaEventEx==NULL) return(0);

  // Set up one-shot mode.
  if (FAILED(hr = pSampleGrabber->SetBufferSamples(TRUE))) return(0);
  if (FAILED(hr = pSampleGrabber->SetOneShot(TRUE))) return(0);

  p_VideoIsOpened=1;
  return(1);
}

//--------------------------------------------------------------------

int32 CVideoReader::m_GetVideoInfo (int32 *width, int32 *height,
									int32 *bpp, float4 *num_seconds)
{
  LONGLONG Duration;
  AM_MEDIA_TYPE MediaType;
  HRESULT hr;
  char8 msg[150];
  long evCode;

  // non volgio che il filmato venga visualizzato in una
  // finestra automatica
  CComQIPtr<IVideoWindow> pVideoWindow = pGraphBuilder;
  if (FAILED(hr = pVideoWindow->put_AutoShow(OAFALSE))) return(0);
  DumpDebugPoint(fdebug, "m_GetVideoInfo: put_AutoShow done", 0);

  // mi server solo per settare l'unita' di misura in frames
  CComQIPtr<IMediaSeeking> pSeek = pMediaControl;
  if (pSeek == NULL) return(0);
  DumpDebugPoint(fdebug, "m_GetVideoInfo: pSeek done", 0);

  // voglio la durata in frames
  //hr=pSeek->SetTimeFormat(&TIME_FORMAT_FRAME);
  if (FAILED(hr = pSeek->GetDuration(&Duration))) return(0);
  *num_seconds=(float4)(((float4)Duration)/10000000.0f);
  DumpDebugPoint(fdebug, "m_GetVideoInfo: GetDuration done", 0);

  // run il filmato
  if (FAILED(hr = pSampleGrabber->SetOneShot(TRUE))) return(0);
  if (FAILED(hr = pMediaControl->Run())) return(0);
  if (FAILED(hr = pMediaEventEx->WaitForCompletion(INFINITE, &evCode)))
     return(0);
  
  ZeroMemory(&MediaType, sizeof(MediaType));
  hr = pSampleGrabber->GetConnectedMediaType(&MediaType);
  if (hr==E_POINTER) DumpDebugPoint(fdebug, "m_GetVideoInfo: hr==E_POINTER", 0);
  else
  if (hr==VFW_E_NOT_CONNECTED) DumpDebugPoint(fdebug, "m_GetVideoInfo: hr==VFW_E_NOT_CONNECTED", 0);
  else
  if (hr==S_OK) DumpDebugPoint(fdebug, "m_GetVideoInfo: hr==S_OK", 0);

  if (FAILED(hr)) return(0);
  DumpDebugPoint(fdebug, "m_GetVideoInfo: GetConnectedMediaType done", 0);

  VIDEOINFOHEADER *pVideoHeader = (VIDEOINFOHEADER*)MediaType.pbFormat;
  if (pVideoHeader == NULL) return(0);
  DumpDebugPoint(fdebug, "m_GetVideoInfo: pVideoHeader done", 0);

  sprintf(msg, "m_GetVideoInfo: width=%d, height=%d\n", 
	      pVideoHeader->bmiHeader.biWidth,
		  pVideoHeader->bmiHeader.biHeight);
  DumpDebugPoint(fdebug, msg, 0);

  *bpp=32;
  *width=(int32)pVideoHeader->bmiHeader.biWidth;
  *height=(int32)pVideoHeader->bmiHeader.biHeight;
  p_VideoIsOpened=1;
  return(1);
}

//--------------------------------------------------------------------

int32 CVideoReader::m_GetFrame (float4 secondNumber, void *buffer)
{
  LONGLONG rtStart, rtStop;
  long evCode, size;
  HRESULT hr;

  CComQIPtr<IMediaSeeking> pSeek = pMediaControl;
  if (pSeek == NULL) return(0);

  //hr=pSeek->SetTimeFormat(&TIME_FORMAT_FRAME);
  if (FAILED(hr = pSampleGrabber->SetOneShot(TRUE))) return(0);

  rtStart=LONGLONG(secondNumber*10000000.0f);
  rtStop=rtStart;
  hr = pSeek->SetPositions(&rtStart, AM_SEEKING_AbsolutePositioning, 
                           &rtStop, AM_SEEKING_AbsolutePositioning);
  if (FAILED(hr)) return(0);
  if (FAILED(hr = pMediaControl->Run())) return(0);
  if (FAILED(hr = pMediaEventEx->WaitForCompletion(INFINITE, &evCode)))
     return(0);
  hr = pSampleGrabber->GetCurrentBuffer(&size, (long *)buffer);
  pMediaControl->Stop();
  return(1);
}

//--------------------------------------------------------------------

int32 CVideoReader::m_CloseVideo(void)
{
  HRESULT hr;

  hr=pMediaControl->Stop();
  pSampleGrabber.Release();
  pMediaControl.Release();
  pMediaEventEx.Release();
  pGrabberBaseFilter.Release();
  pGraphBuilder.Release();
  p_VideoIsOpened=0;
  return(1);
}