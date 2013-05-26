// NON TOCCARE QUESTO FILE, IL CODICE DELLA DEMO VA MODIFICATO
// SOLTANTO IN "ADmain.h" !!!

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <direct.h>
#include <commdlg.h>
#include <stdio.h>
#include <stdarg.h>
#include "ADrender.h"
#include "resource.h"

//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                "And now design"
#define TITLE               "And now design"

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
bool g_bActive = false;   // applicazione attiva?
bool demoRunning= false;  // la demo sta "girando"
HWND miawin;			  // finestra principale
HINSTANCE mioHINST;
HICON miocursor;
double curpos=0.0;
int demo_state=0;
int fpsdiv;              // velocita' di generazione frame

BOOL RunFullscreen;          // finestra/schermo intero
D3DDEVICEINFO driver_to_use;
int mouseop;

#include "utility.h"
#include "ADmain.h"

//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
    char                        szBuff[128];
    va_list                     vl;

    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
    Close_Screen();
    MessageBox(hWnd, szBuff, TITLE, MB_OK);
    DestroyWindow(hWnd);
    va_end(vl);
    return hRet;
}


int search_res(D3DDEVICEINFO *tab_dev, int dev,
			   int rx, int ry, int bpp)
{
  unsigned int ww;

  for (ww=0; ww<tab_dev[dev].dwNumModes; ww++)
  {
	 if ((tab_dev[dev].SupportedModes[ww].Width==(unsigned int)rx) &&
	     (tab_dev[dev].SupportedModes[ww].Height==(unsigned int)ry) &&
		 (tab_dev[dev].SupportedModes[ww].Bpp==bpp)
		 )
	 return((int)ww);
  }
  return(-1);
}


int CALLBACK MyDialogFunc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int modi, driver, current_sel_driver;
  int i;
  unsigned int ww;
  static D3DDEVICEINFO *tabella_dev;
  char modo_str[30];
  BOOL err;


  switch (message) {
    case WM_INITDIALOG:

     // enumerazione dei driver directx 8
     driver=Enumerate_DrawDriver(&tabella_dev);
	 if (driver<=0) debug_error(hdwnd, "No DirectX 8 drivers present");
	 for (ww=0; ww<(unsigned int)driver; ww++)
	   SendDlgItemMessage(hdwnd, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)tabella_dev[ww].strDesc);

	 SendDlgItemMessage(hdwnd, IDC_COMBO2, CB_SETCURSEL, 0, (LPARAM)0);
	 current_sel_driver=0;
	 for (ww=0; ww<tabella_dev[current_sel_driver].dwNumModes; ww++)
	 {
        sprintf(modo_str, "%dx%dx%d", tabella_dev[0].SupportedModes[ww].Width,
	                                  tabella_dev[0].SupportedModes[ww].Height,
                                      tabella_dev[0].SupportedModes[ww].Bpp);
        SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)modo_str);
	 }
	 // cerco una res 640x480 (prima 32bpp poi 16bpp)
	 i=search_res(tabella_dev, current_sel_driver, 320, 240, 32);
     if (i==-1)
	 {
	   i=search_res(tabella_dev, current_sel_driver, 320, 240, 16);
	   if (i==-1) debug_error(miawin, "This intro need a 320x240 resolution support!");
	   //if (i==-1) SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_SETCURSEL, 0, (LPARAM)0);
	   else SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_SETCURSEL, i, (LPARAM)0);
	 }
	 else SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_SETCURSEL, i, (LPARAM)0);

     // setto fullscreen di default
     SendDlgItemMessage(hdwnd, IDC_RADIO1, BM_SETCHECK, 0, 0);
     SendDlgItemMessage(hdwnd, IDC_RADIO2, BM_SETCHECK, 1, 0);
     SendDlgItemMessage(hdwnd, IDC_CHECK1, BM_SETCHECK, 0, 0);
	 break;


    case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDC_COMBO2:
			   if (HIWORD(wParam)==CBN_CLOSEUP)
			   {
			     i=SendDlgItemMessage(hdwnd, IDC_COMBO2, CB_GETCURSEL, 0, 0L);
				 if (i!=current_sel_driver)
				 {
				   current_sel_driver=i;
                   SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_RESETCONTENT, 0, (LPARAM)0);
	               // enumerazione modi video del nuovo driver
	               for (ww=0; ww<tabella_dev[i].dwNumModes; ww++)
				   { 
	                   sprintf(modo_str, "%dx%dx%d", tabella_dev[i].SupportedModes[ww].Width,
	                                                 tabella_dev[i].SupportedModes[ww].Height,
                                                     tabella_dev[i].SupportedModes[ww].Bpp);
	                   SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)modo_str);
				   }
	               // cerco una res 320x240 (prima 32bpp poi 16bpp)
	               i=search_res(tabella_dev, current_sel_driver, 320, 240, 32);
                   if (i==-1)
				   {
	                 i=search_res(tabella_dev, current_sel_driver, 320, 240, 16);
	                 if (i==-1) debug_error(miawin, "This intro need a 320x240 resolution support!");
	                 //if (i==-1) SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_SETCURSEL, 0, (LPARAM)0);
	                 else SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_SETCURSEL, i, (LPARAM)0);
				   }
	               else SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_SETCURSEL, i, (LPARAM)0);
                   // setto fullscreen di default
                   SendDlgItemMessage(hdwnd, IDC_RADIO1, BM_SETCHECK, 0, 0);
                   SendDlgItemMessage(hdwnd, IDC_RADIO2, BM_SETCHECK, 1, 0);
				 }
			   }
			   break;

		  case IDC_RADIO1:
  			 i=SendDlgItemMessage(hdwnd, IDC_COMBO2, CB_GETCURSEL, 0, 0L);
	         if (!tabella_dev[i].bCanDoWindowed)
			 {
               SendDlgItemMessage(hdwnd, IDC_RADIO1, BM_SETCHECK, 0, 0);
	           SendDlgItemMessage(hdwnd, IDC_RADIO2, BM_SETCHECK, 1, 0);
			 }
			 break;

		  case IDCANCEL:
			   EndDialog(hdwnd, 0);
			   return 1;

		  case IDOK:
			   fpsdiv=30;
			   fpsdiv=GetDlgItemInt(hdwnd, IDC_EDIT2, &err, FALSE);
			   if (fpsdiv==0) fpsdiv=30;
			   RunFullscreen=(BOOL)SendDlgItemMessage(hdwnd, IDC_RADIO2, BM_GETCHECK, 0, 0);
			   NoSound=(BOOL)SendDlgItemMessage(hdwnd, IDC_CHECK1, BM_GETCHECK, 0, 0);
			   i=SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_GETCURSEL, 0, 0L);
			   driver_to_use=tabella_dev[current_sel_driver];
			   driver_to_use.WantedMode=tabella_dev[current_sel_driver].SupportedModes[i];
			   if ((driver_to_use.WantedMode.Width == 320) ||
                   (driver_to_use.WantedMode.Height == 240))
			   {
			      driver_to_use.bFullscreen=RunFullscreen;
			      EndDialog(hdwnd, 1);
			      return 1;
			   }
			   else MessageBox(hdwnd, "You MUST select a 320x240 resolution", "AD-Debug", NULL); 
		}
  }
  return(0);
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//-----------------------------------------------------------------------------
long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	char str2[40];
//	char *ss;
//	int dec,sign,xpos,pp;

    switch (message)
    {
        case WM_ACTIVATEAPP:
            // Pause if minimized or not the top window
            g_bActive = (wParam == WA_ACTIVE) || (wParam == WA_CLICKACTIVE);
            return 0L;
			break;

        case WM_DESTROY:
            // Clean up and close the app
            ADmainExit();
            PostQuitMessage(0);
            return 0L;
			break;
		
		case WM_PAINT:
//			if (video.enabled) ADmainLoop();
			break;

		case WM_SIZE:
			if (!RunFullscreen)
			{
				//wnWidth = LOWORD(lParam);
				//wnHeight = HIWORD(lParam); 
//			    if (video.enabled) ADmainLoop();
			}
            break;

		case WM_LBUTTONDOWN:
			if (demoRunning)
			{
			  PostMessage(hWnd, WM_CLOSE, 0, 0);
              return 0L;
			}
			break;
/*
        case WM_MOUSEMOVE:
		    if (demoRunning)
			{
               xpos = LOWORD(lParam);
			   if (xpos>mouseop)
			   {
				  curpos+=0.001;
				  if (curpos>1) curpos=1;
			   }
			   else
			   {
				  curpos-=0.001;
				  if (curpos<0) curpos=0;
			   }
			   mouseop=xpos;
			}
*/
        case WM_KEYDOWN:
            // Handle any non-accelerated key commands
			if (demoRunning)
            switch (wParam)
            {
/*
				case VK_F2:
					     curpos-=0.02f;
						 if (curpos<0) curpos=0;
						 break;
                case VK_F3:
                         curpos+=0.02f;
						 if (curpos>1) curpos=1;
						 break;
                case VK_F4:
                         curpos=1.0;
						 break;
				case VK_F5:
					// ss=str1;
					ss=ecvt(curpos, 8, &dec, &sign);
					strcpy(str2,"pos: 0.");
					dec=abs(dec);
					for (pp=0; pp<dec; pp++)
					  strcat(str2, "0");

					strcat(str2,ss);
					MessageBox(miawin, str2, "AD-Tuning System", NULL);
                    break;
*/
				case VK_ESCAPE:
                case VK_F12:
                    // PostMessage(hWnd, WM_CLOSE, 0, 0);
					demo_state=1;
                    return 0L;
					break;
            }
            break;

        case WM_SETCURSOR:
            // Turn off the cursor if this is a full-screen app
            if (RunFullscreen) SetCursor(NULL);
            else SetCursor(miocursor);
            return TRUE;
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: InitApp()
//-----------------------------------------------------------------------------
static HRESULT
InitApp(HINSTANCE hInstance, int nCmdShow)
{
    HWND                        hWnd;
    WNDCLASS                    wc;
	int dlgres;

    // Set up and register window class
    wc.style = /*CS_HREDRAW | CS_VREDRAW | */CS_BYTEALIGNCLIENT;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ADCURSOR)/*IDC_ARROW*/);
    wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
			           //GetSysColorBrush(COLOR_APPWORKSPACE);

    wc.lpszMenuName = NAME;
    wc.lpszClassName = NAME;
    RegisterClass(&wc);

	dlgres=DialogBox(mioHINST, MAKEINTRESOURCE(IDD_DIALOG1), NULL, MyDialogFunc);
	if (dlgres==0) exit(0);

    // Create a window
	if (RunFullscreen)
    hWnd = CreateWindowEx(WS_EX_TOPMOST,  
                          NAME,
                          TITLE,
                          WS_POPUP,
                          0,
                          0,
                          GetSystemMetrics(SM_CXSCREEN),
                          GetSystemMetrics(SM_CYSCREEN),
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
	else
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                          NAME,
                          TITLE,
                          WS_POPUP | WS_CAPTION | WS_SYSMENU |
						  WS_THICKFRAME | WS_MAXIMIZEBOX| WS_MINIMIZEBOX,
                          (GetSystemMetrics(SM_CXSCREEN)-driver_to_use.WantedMode.Width)/2,
                          (GetSystemMetrics(SM_CYSCREEN)-driver_to_use.WantedMode.Height)/2,
                          driver_to_use.WantedMode.Width+GetSystemMetrics(SM_CXSIZEFRAME)*2,
						  driver_to_use.WantedMode.Height+GetSystemMetrics(SM_CYSIZEFRAME)*2
						  +GetSystemMetrics(SM_CYCAPTION),
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

	miawin=hWnd;
	miocursor=wc.hCursor;
    if (!hWnd) return FALSE;
    ShowWindow(miawin, nCmdShow);
    UpdateWindow(miawin);
    SetFocus(miawin);
    return DD_OK;
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//-----------------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    MSG msg;
	int miorv;

	mioHINST=hInstance;
    if (InitApp(hInstance, nCmdShow) != DD_OK)
       return FALSE;

	{
	  ADmainEnter();
	  demoRunning=true;
      while (TRUE)
      {

        if (PeekMessage(&msg, NULL,  0, 0, PM_NOREMOVE)!=0)
		{
			miorv=GetMessage(&msg, NULL, 0, 0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
        } 
		else 
		{
			if (demo_state==0)
			{
			   ADmainLoop();
			   if (curpos>=1.0)
			   {
	             curpos=0;
                 demotimer.init(90);
                 demotimer.start();
			     //if (curpos>=1.0) PostMessage(miawin, WM_CLOSE, 0, 0);
			   }
			}
			else
			{
			   AD_PreExit();
			}
		}

        if (miorv==0) break;
	  }
	}
    
    return msg.wParam;
}