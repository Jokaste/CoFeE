#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <CommCtrl.h>
#include <shellapi.h>

#pragma warn(disable:2006 2215)

#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Languages.h"
#include "Ressources.h"

void Child5_Diapo_OnCreate(HWND __hWnd)
{
	hTabsWindows[HWND_DIAPORAMA] = __hWnd ;
	hDCDiaporama = GetDC(hTabsWindows[HWND_DIAPORAMA]) ;
	SetStretchBltMode(hDCDiaporama,STRETCH_HALFTONE) ;

	bStopDiaporama = bDiaporama = TRUE ;		//	The diaporama is not lauched

	return ;
}

void Child5_Diapo_Paint(void)
{
	register		LPIMAGEINFOS	_lpImageInfos ;
	DWORD			_dwNewWidth, _dwNewHeight ;
	HDC				_hDCMem ;
	HBITMAP			_hBitmap, _hBmpOld ;
	RECT			_Rc ;

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(_lpImageInfos)
	{
		if(hDCDiaporama)
		{
			GetClientRect(hTabsWindows[HWND_DIAPORAMA],&_Rc) ;

			_hDCMem = CreateCompatibleDC(hDCDiaporama) ;
			if(_hDCMem)
			{
				_hBitmap = CreateCompatibleBitmap(hDCDiaporama,_Rc.right,_Rc.bottom) ;
				if(_hBitmap)
				{
					_hBmpOld = SelectObject(_hDCMem,_hBitmap) ;
					if(_hBmpOld)
					{
						FillRect(_hDCMem,&_Rc,hBarbieBrush) ;

						SetStretchBltMode(_hDCMem,STRETCH_HALFTONE) ;

						ImageResize(_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_Rc.right,_Rc.bottom,&_dwNewWidth,&_dwNewHeight) ;
						StretchDIBits(_hDCMem,(_Rc.right - _dwNewWidth) / 2,(_Rc.bottom - _dwNewHeight) / 2,_dwNewWidth,_dwNewHeight,30,30,_lpImageInfos->BitmapInfo.bmiHeader.biWidth - 60,_lpImageInfos->BitmapInfo.bmiHeader.biHeight - 60,_lpImageInfos->lpResult,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS,SRCCOPY) ;

						BitBlt(hDCDiaporama,0,0,_Rc.right,_Rc.bottom,_hDCMem,0,0,SRCCOPY) ;

						SelectObject(_hDCMem,_hBmpOld) ;
					}

					DeleteObject(_hBitmap) ;
				}

				DeleteDC(_hDCMem) ;
			}
		}
	}

	return ;
}

void Child5_Diapo_OnPaint(void)
{
	PAINTSTRUCT		_Ps ;

	BeginPaint(hTabsWindows[HWND_DIAPORAMA],&_Ps) ;

	if(IsWindowVisible(hTabsWindows[HWND_DIAPORAMA]))
		Child5_Diapo_Paint() ;

	EndPaint(hTabsWindows[HWND_DIAPORAMA],&_Ps) ;

	return ;
}

void Child5_Diaporama_ApplyFilter(void)
{
	register		LPIMAGEINFOS	_lpImageInfos ;
	register		LPBYTE			_lpResult ;

	DWORD			_dwSize, _iIndex ;

	if(lpCurrentFilter->TotalColsNumber == 0)
		return ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	_iIndex = (int) SendDlgItemMessage(hTabsWindows[HWND_WORKSPACE],LISTBOX_MODELS,LB_GETCURSEL,0,0) ;

	lpCurrentFilter = ReadFilter((int) SendDlgItemMessage(hTabsWindows[HWND_WORKSPACE],LISTBOX_MODELS,LB_GETITEMDATA,_iIndex,0),&CurrentFilter) ;
	if(!lpCurrentFilter)
		return ;

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(_lpImageInfos)
	{
		_lpResult = GetBitsFromBitmap(_lpImageInfos->hImgBitmap,&_dwSize,_lpImageInfos->lpWork) ;
		if(_lpResult)
		{
			_lpImageInfos->lpWork = _lpImageInfos->lpResult ;
			_lpImageInfos->lpResult = _lpResult ;

			if(lpCurrentFilter->Divisor == 0)
				lpCurrentFilter->Divisor = 1 ;

			Convolve(_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpResult,lpCurrentFilter) ;

			Child5_Diapo_Paint() ;
		}
	}

	SetCursor(LoadCursor(NULL,IDC_ARROW)) ;

	return ;
}

void Diaporama(void)
{
	int				_iSecondes ;
	HWND			_hWnd ;

	_hWnd = hTabsWindows[HWND_DIAPORAMA] ;

	if(SendDlgItemMessage(hTabsWindows[HWND_WORKSPACE],LISTBOX_MODELS,LB_GETCOUNT,0,0) == 0)
		return ;

	if(bStopDiaporama == TRUE)
	{
		bStopDiaporama = FALSE ;
		bDiaporama = FALSE ;

		MoveWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),0,0,34,34,TRUE) ;
		MoveWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),0,0,34,34,TRUE) ;

		ShowWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),SW_SHOW) ;
		ShowWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),SW_HIDE) ;

		_iSecondes = GetPrivateProfileInt(szProfile_09,"Diaporama",1000,szProfileFileName) ;

		SetTimer(_hWnd,ID_DIAPORAMA,(UINT) _iSecondes,(TIMERPROC) NULL) ;

		return ;
	}

	bStopDiaporama = TRUE ;
	bDiaporama = TRUE ;

	MoveWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),0,0,34,34,TRUE) ;
	MoveWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),0,0,34,34,TRUE) ;

	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),SW_SHOW) ;

	KillTimer(_hWnd,ID_DIAPORAMA) ;

	return ;
}

LRESULT CALLBACK Ch5_Diaporama_Proc(HWND __hWnd,UINT __Msg,WPARAM __wParam,LPARAM __lParam)
{
	int				_iIndex, _iCount ;
	TBBUTTONINFOA	_Tb_Info ;

	switch (__Msg)
	{
		case	WM_TIMER :			if(__wParam == ID_DIAPORAMA)
									{
										if(bStopDiaporama)
										{
											bDiaporama = TRUE ;
											KillTimer(__hWnd,ID_DIAPORAMA) ;
											break ;
										}

										if(bDiaporama)
											break ;

										bDiaporama = TRUE ;

										_iIndex = (int) SendDlgItemMessage(hTabsWindows[HWND_WORKSPACE],LISTBOX_MODELS,LB_GETCURSEL,0,0) ;
										if(_iIndex == LB_ERR)
										{
											_Tb_Info.cbSize = sizeof(TBBUTTONINFOA) ;
											_Tb_Info.dwMask = TBIF_IMAGE ;
											_Tb_Info.iImage = 23 ;

											SendMessage(hToolBar,TB_SETBUTTONINFO,TLB_BTN_DIAPORAMA_OFF,(LPARAM) &_Tb_Info) ;

											bStopDiaporama = TRUE ;
											break ;
										}

										_iCount = (int) SendDlgItemMessage(hTabsWindows[HWND_WORKSPACE],LISTBOX_MODELS,LB_GETCOUNT,0,0) ;
										if((_iIndex + 1) >= _iCount)
										{
											bStopDiaporama = TRUE ;

											_Tb_Info.cbSize = sizeof(TBBUTTONINFOA) ;
											_Tb_Info.dwMask = TBIF_IMAGE ;
											_Tb_Info.iImage = 23 ;

											ShowWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),SW_SHOW) ;
											ShowWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),SW_HIDE) ;

											break ;
										}

										if(IsWindowVisible(__hWnd))
											Child5_Diaporama_ApplyFilter() ;

										SendDlgItemMessage(hTabsWindows[HWND_WORKSPACE],LISTBOX_MODELS,LB_SETCURSEL,_iIndex + 1,0) ;

										bDiaporama = FALSE ;
									}

									break ;

		case	WM_CREATE :			Child5_Diapo_OnCreate(__hWnd) ;		break ;
		case	WM_PAINT :			Child5_Diapo_OnPaint() ;			break ;

		default :					return(DefWindowProc(__hWnd,__Msg,__wParam,__lParam)) ;
	}

	return (0) ;
}
