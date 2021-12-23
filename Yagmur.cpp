/**
 * Yagmur ~ Rain sound background app for rainy moods.
 * Copyright (c) 2003 Erdinc AKAN (erakan@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **/

#include "stdafx.h"
#include "mmsystem.h"
#include "shellapi.h"		  
#pragma comment(lib, "winmm")
#include "resource.h"

// Global Variables:
HINSTANCE hInst;								// current instance

UINT GlobalTimer;
bool KillIt = false, SesKisik = false;
HICON hMaIcon, hSimsekIcon;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK TimerTiki(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

void systray_add(HWND hwnd)
{
  NOTIFYICONDATA tnid;
  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd = hwnd;
  tnid.uID = 3131;
  tnid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
  tnid.uCallbackMessage = WM_USER+3131;
  tnid.hIcon = hMaIcon;
  strcpy(tnid.szTip,"Yagmur v0.1 © Erdinc AKAN");
  Shell_NotifyIcon(NIM_ADD, &tnid);
}

void systray_notify(HWND hwnd, int ne)
{
  NOTIFYICONDATA tnid;
  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd = hwnd;
  tnid.uID = 3131;
  tnid.uFlags = NIF_ICON;
  tnid.hIcon = ne?hSimsekIcon:hMaIcon;
  Shell_NotifyIcon(NIM_MODIFY, &tnid);
}

void systray_del(HWND hwnd)
{
  NOTIFYICONDATA tnid;
  tnid.cbSize = sizeof(NOTIFYICONDATA);
  tnid.hWnd = hwnd;
  tnid.uID = 3131;
  Shell_NotifyIcon(NIM_DELETE, &tnid);
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	hMaIcon = LoadIcon(hInstance, (LPCTSTR)IDI_YAGMUR);
	hSimsekIcon = LoadIcon(hInstance, (LPCTSTR)IDI_SIMSEK);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_YAGMUR);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= hMaIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	=  (LPCSTR)IDC_YAGMUR;
	wcex.lpszClassName	= "Erdinc AKAN YAGMUR";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; 

   hWnd = CreateWindow("Erdinc AKAN YAGMUR", "Yagmur", WS_OVERLAPPED| WS_VISIBLE | WS_SYSMENU,
      CW_USEDEFAULT, 0, 202, 69, NULL, NULL, hInstance, NULL);
   SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW /*| GetWindowLong(hWnd, GWL_EXSTYLE)*/);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
		case WM_USER+3131:
			{
				UINT msg = LOWORD(lParam);
				//if (LOWORD(lParam) == WM_LBUTTONDOWN && !IsWindowVisible(hwndDlg))
				if(msg == WM_LBUTTONUP || msg == WM_LBUTTONDBLCLK)
				{
					SetForegroundWindow(hWnd);
					if(!IsWindowVisible(hWnd))
					{
						RECT rfrom, rto;
						GetWindowRect(hWnd, &rfrom);
						SystemParametersInfo(SPI_GETWORKAREA,0,&rto,0);
						rto.left=rto.right-150;
						rto.top=rto.bottom-30;
						DrawAnimatedRects(hWnd, IDANI_CAPTION, &rto, &rfrom);
						ShowWindow(hWnd, SW_SHOW);
					}
				}
				if(msg == WM_RBUTTONUP)
				{
					HMENU hMenu = LoadMenu(hInst, (LPCTSTR)IDC_YAGMUR);
					POINT p;
					GetCursorPos(&p);
					hMenu = GetSubMenu(hMenu, 0);
					TrackPopupMenu(hMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, p.x, p.y, 0,
								   hWnd, NULL);
				}
					
			}
			return 0;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				case IDM_PAUSE:
					if(SesKisik)
						PlaySound((LPCTSTR) IDR_YAGMURDISARDA, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
					else
						PlaySound(NULL, NULL, NULL);
					SesKisik = !SesKisik;
					break;
				case IDM_GIZLE:
					if(!IsWindowVisible(hWnd))
					{
						RECT rfrom, rto;
						GetWindowRect(hWnd, &rfrom);
						SystemParametersInfo(SPI_GETWORKAREA,0,&rto,0);
						rto.left=rto.right-150;
						rto.top=rto.bottom-30;
						DrawAnimatedRects(hWnd, IDANI_CAPTION, &rto, &rfrom);
						ShowWindow(hWnd, SW_SHOW);
					}
					else
					{
						RECT rfrom, rto;
						GetWindowRect(hWnd, &rfrom);
						SystemParametersInfo(SPI_GETWORKAREA,0,&rto,0);
						rto.left=rto.right-150;
						rto.top=rto.bottom-30;
						DrawAnimatedRects(hWnd, IDANI_CAPTION, &rfrom, &rto);
						ShowWindow(hWnd, SW_HIDE);
					}
					break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			return 0;
		case WM_NCHITTEST:
				LRESULT test;
				test = DefWindowProc(hWnd, message, wParam, lParam);
				if( test != HTCLOSE && test != HTMINBUTTON && test != HTMENU && 
					test != HTSYSMENU)
					return HTCAPTION;
				else
					return test;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			RECT rt;
			GetClientRect(hWnd, &rt);
			SetBkMode(hdc, TRANSPARENT);
			DrawText(hdc, "Erdinç AKAN - Yağmur", 20, &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			EndPaint(hWnd, &ps);
			return 0;
		case WM_DESTROY:
			systray_del(hWnd);
			PlaySound(NULL, NULL, NULL);
			PostQuitMessage(0);
			return 0;
		case WM_CREATE:
			PlaySound((LPCTSTR) IDR_YAGMURDISARDA, NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
			GlobalTimer = SetTimer(hWnd, 1, 4000, *TimerTiki);
		case WM_INITDIALOG:
			systray_add(hWnd);
	}
   
   return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

VOID CALLBACK TimerTiki(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	int RandSayi;
	if(!SesKisik)
	{
		if(idEvent == GlobalTimer)
		{
			if(KillIt)
			{
				KillTimer(hWnd, idEvent);
				KillIt = false;
				PlaySound(NULL, NULL, NULL);
				PlaySound((LPCTSTR) IDR_YAGMURDISARDA, NULL, SND_RESOURCE | SND_ASYNC | SND_PURGE | SND_LOOP);
				systray_notify(hWnd, 0);
				GlobalTimer = SetTimer(hWnd, 1, 4000, *TimerTiki);
				return;
			}
			srand((unsigned int) dwTime);
			RandSayi = rand() % 5000;
			if(RandSayi > 3784 && RandSayi < 4221)
			{
				KillTimer(hWnd, idEvent);
				PlaySound(NULL, NULL, NULL);
				PlaySound((LPCTSTR) IDR_GOKGURLE1, NULL, SND_RESOURCE | SND_PURGE | SND_ASYNC  | SND_LOOP);
				KillIt = true;
				systray_notify(hWnd, 1);
				GlobalTimer = SetTimer(hWnd, 1, 9000, *TimerTiki);
			}
		}
	}
}
 