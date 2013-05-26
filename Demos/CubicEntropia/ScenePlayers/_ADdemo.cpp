//-----------------------------------------------------------------------------
// ADdemo system 99 - for Win32 - HereBit - v1.0
//-----------------------------------------------------------------------------
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
#include "RenderLib.h"
#include "resource.h"

//-----------------------------------------------------------------------------
// Local definitions
//-----------------------------------------------------------------------------
#define NAME                "ADdemo"
#define TITLE               "Apocalypse Design Demo System"

//-----------------------------------------------------------------------------
// Global data
//-----------------------------------------------------------------------------
CRenderLib RenderLib;
BOOL g_bActive = false; // applicazione attiva?
BOOL demoRunning= false;
HWND miawin;			// finestra principale
HINSTANCE mioHINST;
HICON miocursor;
double curpos=0.0;
int32 mouseX=0, mouseY=0;

char8 FileToPlay[100]="";  // nome del file di scena da playare
char8 FileToPlay2[100]="";  // nome del file di scena da playare
char8 SoundFile[100]="";   // eventuale nome del file sonoro
int32 fpsdiv;              // velocita' di generazione frame

BOOL RunFullscreen;          // finestra/schermo intero
static D3DModeInfo *ScrMode;
#include "ADmain.h"

//-----------------------------------------------------------------------------
// Name: InitFail()
// Desc: This function is called if an initialization function fails
//-----------------------------------------------------------------------------
HRESULT
InitFail(HWND hWnd, HRESULT hRet, LPCTSTR szError,...)
{
    char8                       szBuff[128];
    va_list                     vl;

    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
    RenderLib.m_Close_Screen();
    MessageBox(hWnd, szBuff, TITLE, MB_OK);
    DestroyWindow(hWnd);
    va_end(vl);
    return hRet;
}


int32 Mode2Index (D3DModeInfo *m, int32 n)
{
   D3DModeInfo *mm;

   for (int32 i=0; i<n; i++)
   {
	  mm=RenderLib.m_GetScreenMode(i);
	  if (mm)
	  {
		 if (mm==m) return(i);
	  }
   }
   return(-1);
}


BOOL OpenFileDialog(HWND hWnd, LPTSTR pszFileName,
					int32 *nFileName, LPDWORD lpdwFlags )
{
    BOOL            fReturn;
    OPENFILENAME    ofn;

    pszFileName[0]          = 0;
    ofn.lStructSize         = sizeof(ofn);
    ofn.hwndOwner           = hWnd;//GetParent(hWnd);
    ofn.hInstance           = mioHINST;
//    ofn.lpstrFilter         = "3DS Files\0*.3DS\0A3D Files\0*.A3D\0\0";
    ofn.lpstrFilter         = "A3D Files\0*.A3D\0\0";
    ofn.lpstrCustomFilter   = NULL;
    ofn.nMaxCustFilter      = 0;
    ofn.nFilterIndex        = 1;
    ofn.lpstrFile           = pszFileName;
    ofn.nMaxFile            = 200;
    ofn.lpstrFileTitle      = NULL;
    ofn.nMaxFileTitle       = 0;
    ofn.lpstrInitialDir     = "";
    ofn.lpstrTitle          = "Open Scene";
    ofn.Flags               = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY;
    ofn.nFileOffset         = 0;
    ofn.nFileExtension      = 0;
    ofn.lpstrDefExt         = "A3D";
    ofn.lCustData           = (LONG)lpdwFlags;
    fReturn = GetOpenFileName(&ofn);
    return fReturn;
}


int32 CALLBACK MyDialogFunc(HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  unsigned long fff;
  char8 nome_sel[100];
  int32 i, j, l_NumScreenModes;
  char8 modo_str[30];
  char8 DeviceDesc[30]="HAL Device";
  BOOL err;

  switch (message) {
  case WM_INITDIALOG:
     // enumerazione dei driver DirectX 8.1
	 if (!RenderLib.m_CreateDevice(HARDWARE_DEVICE))
	 //if (!RenderLib.m_CreateDevice(SOFTWARE_DEVICE))
        debug_error(hdwnd, "No DirectX 8.1 drivers present!");
	 SendDlgItemMessage(hdwnd, IDC_SLIDER1, TBM_SETRANGEMAX, (WPARAM)(1), (LPARAM)255);
	 SendDlgItemMessage(hdwnd, IDC_SLIDER1, TBM_SETRANGEMIN, (WPARAM)(1), (LPARAM)0);
	 SendDlgItemMessage(hdwnd, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)DeviceDesc);
	 SendDlgItemMessage(hdwnd, IDC_COMBO2, CB_SETCURSEL, 0, (LPARAM)0);
	 // enumerazione modi video
	 l_NumScreenModes=RenderLib.m_EnumerateScreenModes();
	 if (l_NumScreenModes<=0) debug_error(hdwnd, "No Screen Modes available!");
	 for (i=0; i<l_NumScreenModes; i++)
	 {
        ScrMode=RenderLib.m_GetScreenMode(i);
        sprintf(modo_str, "%dx%dx%d", ScrMode->Width, ScrMode->Height, ScrMode->Bpp);
        SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)modo_str);
	 }
	 // cerco una res 640x480 (prima 32bpp poi 16bpp)
	 ScrMode=RenderLib.m_GetScreenMode(512, 384, 32);
     if (!ScrMode)
	 {
	   ScrMode=RenderLib.m_GetScreenMode(512, 384, 16);
	   if (!ScrMode) debug_error(miawin, "This player needs a 640x480 resolution support!");
	 }
	 SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_SETCURSEL, Mode2Index(ScrMode, l_NumScreenModes), (LPARAM)0);
     // setto nessun blur di default
     SendDlgItemMessage(hdwnd, NOBLUR_RADIO, BM_SETCHECK, 1, 0);
	 // No sound = FALSE
     SendDlgItemMessage(hdwnd, IDC_CHECK1, BM_SETCHECK, 0, 0);
	 // Fulscreen = FALSE
	 SendDlgItemMessage(hdwnd, FULLSCREEN_CHECK, BM_SETCHECK, 0, 0);
	 break;


    case WM_COMMAND:
		switch(LOWORD(wParam)) {
		  case IDC_BUTTON1:
			   err=OpenFileDialog(GetParent(hdwnd), nome_sel, &i, &fff);
			   if (err)
			   {
				 strcpy(FileToPlay, nome_sel);
                 SendDlgItemMessage(hdwnd, IDC_EDIT1, EM_SETSEL,  (WPARAM)0, (LPARAM)-1);
                 SendDlgItemMessage(hdwnd, IDC_EDIT1, EM_REPLACESEL,  (WPARAM)FALSE, (LPARAM)nome_sel);
			   }
			   break;

		  case IDC_BUTTON2:
			   err=OpenFileDialog(GetParent(hdwnd), nome_sel, &i, &fff);
			   if (err)
			   {
				 strcpy(FileToPlay2, nome_sel);
                 SendDlgItemMessage(hdwnd, IDC_EDIT3, EM_SETSEL,  (WPARAM)0, (LPARAM)-1);
                 SendDlgItemMessage(hdwnd, IDC_EDIT3, EM_REPLACESEL,  (WPARAM)FALSE, (LPARAM)nome_sel);
			   }
			   break;

		  case IDCANCEL:
			   EndDialog(hdwnd, 0);
			   return 1;

		  case IDOK:
			   fpsdiv=30;
			   fpsdiv=GetDlgItemInt(hdwnd, IDC_EDIT2, &err, FALSE);
			   if (fpsdiv==0) fpsdiv=25;

			   GetDlgItemText(hdwnd, IDC_EDIT1, FileToPlay, 100);
			   RunFullscreen=(BOOL)SendDlgItemMessage(hdwnd, FULLSCREEN_CHECK, BM_GETCHECK, 0, 0);
			   NoSound=(BOOL)SendDlgItemMessage(hdwnd, IDC_CHECK1, BM_GETCHECK, 0, 0);
			   i=SendDlgItemMessage(hdwnd, IDC_COMBO1, CB_GETCURSEL, 0, 0L);

			   ScrMode=RenderLib.m_GetScreenMode(i);
			   if (strcmp(FileToPlay, "") ==0)
                  MessageBox(hdwnd, "You have to browse at least one scene!", "AD-Debug", NULL);
			   else
			   {
			     // il loader non vuole l'estensione
			     for (i=0; i<(int32)strlen(FileToPlay); i++)
                     if (FileToPlay[i]=='.') { j=i; break; }
			     j=i;
			     FileToPlay[j]='\0';
			     if (strcmp(FileToPlay2, "")!=0)
				 {
			       // il loader non vuole l'estensione
			       for (i=0; i<(int32)strlen(FileToPlay2); i++)
                       if (FileToPlay2[i]=='.') { j=i; break; }
			       j=i;
			       FileToPlay2[j]='\0';
				 }
			     if (!ScrMode) MessageBox(hdwnd, "ScrMode is NULL!", "AD-Debug", NULL);
				 // blur level
				 l_BlurLevel=0;
	             i=SendDlgItemMessage(hdwnd, IDC_SLIDER1, TBM_GETPOS, (WPARAM)0, (LPARAM)0);
                 l_BlurLevel=((float4)i)/255.0f;
				 // blur type
				 l_BlurType=NOBLUR;
				 i=(BOOL)SendDlgItemMessage(hdwnd, MOTIONBLUR_RADIO, BM_GETCHECK, 0, 0);
				 if (i) l_BlurType=MOTIONBLUR;
				 else
				 {
				   i=(BOOL)SendDlgItemMessage(hdwnd, RADIALBLUR_RADIO, BM_GETCHECK, 0, 0);
				   if (i) l_BlurType=RADIALBLUR;
				 }
			     EndDialog(hdwnd, 1);
			   }
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
	// char8 str2[40];
	// char8 *ss;
	// int32 dec, sign, pp;
	int32 xpos, ypos, deb, b;
	const float4 camRot = 0.1f;

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

		case WM_RBUTTONDOWN:
			if (demoRunning)
			{
			  PostMessage(hWnd, WM_CLOSE, 0, 0);
              return 0L;
			}
			break;

        case WM_MOUSEMOVE:
		    if (demoRunning)
			{
               xpos = LOWORD(lParam);
               ypos = HIWORD(lParam);
			   /*if (wParam & MK_LBUTTON)
			   {
			      if (ypos>mouseY)
					 scena.m_TranslateCamera(0, 0, 10);
				  else
			      if (ypos<mouseY)
					 scena.m_TranslateCamera(0, 0, -10);
			   }
			   else*/
			   if (wParam & MK_LBUTTON)
			   {
			     if (xpos>mouseX)
				 {
				    //scena.m_RotateCamera(camRot, 0, 0);
					scena.m_RotateCamera(0, camRot, 0);
				    //curpos+=0.0001;
			        //if (curpos>1) curpos=1;
				 }
			     else
			     if (xpos<mouseX)
				 {
				   //scena.m_RotateCamera(-camRot, 0, 0);
				   scena.m_RotateCamera(0, -camRot, 0);
				   //curpos-=0.0001;
				   //if (curpos<0) curpos=0;
				 }
				 else
			     if (ypos>mouseY)
				   //scena.m_RotateCamera(0, camRot, 0);
				   scena.m_RotateCamera(camRot, 0, 0);
			     else
			     if (ypos<mouseY)
				    //scena.m_RotateCamera(0, -camRot, 0);
				    scena.m_RotateCamera(-camRot, 0, 0);
			   }
			   mouseX=xpos;
			   mouseY=ypos;
			}

        case WM_KEYDOWN:
            // Handle any non-accelerated key commands
			if (demoRunning)
            switch (wParam)
            {
		        case VK_F1:
			        deb=0;
			      break;
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
                    PostMessage(hWnd, WM_CLOSE, 0, 0);
                    return 0L;
					break;

				case VK_UP:
					 //scena.m_TranslateCamera(0, -50, 0);
                     scena.m_TranslateCamera(0, 0, 10);
					 break;
				case VK_DOWN:
					 //scena.m_TranslateCamera(0, 50, 0);
                     scena.m_TranslateCamera(0, 0, -10);
					 break;
				// cambiamo il tipo di blur
				case VK_LEFT:
					 b=RenderLib.p_BlurType;
					 if (b & MOTIONBLUR)
					 {
						b=(b & (!MOTIONBLUR)) | RADIALBLUR;
					    RenderLib.m_SetBlur(b, l_BlurLevel);
					 }
					 else
					 if (b & RADIALBLUR)
					 {
						b=(b & (!RADIALBLUR)) | MOTIONBLUR;
					    RenderLib.m_SetBlur(b, l_BlurLevel);
					 }
					 break;
				case VK_RIGHT:
					 //scena.m_TranslateCamera(-50, 0, 0);
					 b=RenderLib.p_BlurType;
					 if (b & BLUR_ADD)
					 {
						b=(b & (!BLUR_ADD)) | BLUR_ALPHA;
					    RenderLib.m_SetBlur(b, l_BlurLevel);
					 }
					 else
					 if (b & BLUR_ALPHA)
					 {
						b=(b & (!BLUR_ALPHA)) | BLUR_ADD;
					    RenderLib.m_SetBlur(b, l_BlurLevel);
					 }
					 else
					 {
					   b=RenderLib.p_BlurType | BLUR_ADD;
					   RenderLib.m_SetBlur(b, l_BlurLevel);
					 }
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
// Desc: Do work required for every instance of the application:
//          Create the window, initialize data
//-----------------------------------------------------------------------------
static HRESULT
InitApp(HINSTANCE hInstance, int nCmdShow)
{
    HWND                        hWnd;
    WNDCLASS                    wc;
	int dlgres;

    // Set up	and register window class
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
                          (GetSystemMetrics(SM_CXSCREEN)-ScrMode->Width)/2,
                          (GetSystemMetrics(SM_CYSCREEN)-ScrMode->Height)/2,
                          ScrMode->Width+GetSystemMetrics(SM_CXSIZEFRAME)*2,
						  ScrMode->Height+GetSystemMetrics(SM_CYSIZEFRAME)*2
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
    return D3D_OK;
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
    if (InitApp(hInstance, nCmdShow) != D3D_OK) return FALSE;
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
			ADmainLoop();
			if (curpos>=1.0)
			{
	           curpos=0;
			   demotimer.m_Stop();
               if (scena.p_NumFrames > scena2.p_NumFrames) demotimer.m_Init(scena.p_NumFrames/(float4)fpsdiv);
               else demotimer.m_Init(scena2.p_NumFrames/(float4)fpsdiv);
               demotimer.m_Start();
			   //PostMessage(miawin, WM_CLOSE, 0, 0);
			}
		}
        if (miorv==0) break;
	  }
	}
    return msg.wParam;
}