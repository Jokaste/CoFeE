#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <CommCtrl.h>
#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Languages.h"
#include "Ressources.h"

void PaintResultWindow(void)
{
	register		LPIMAGEINFOS	_lpImageInfos ;
	DWORD			_dwNewWidth, _dwNewHeight ;
	HDC				_hDCMem ;
	HWND			_hResults ;
	HBITMAP			_hBitmap, _hBmpOld ;
	RECT			_Rc ;

	_hResults = hTabsWindows[HWND_RESULTS] ;
	if(_hResults)
	{
		_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
		if(_lpImageInfos)
		{
			if(hDCResults)
			{
				GetClientRect(_hResults,&_Rc) ;

				_hDCMem = CreateCompatibleDC(hDCResults) ;
				if(_hDCMem)
				{
					_hBitmap = CreateCompatibleBitmap(hDCResults,_Rc.right,_Rc.bottom) ;
					if(_hBitmap)
					{
						_hBmpOld = SelectObject(_hDCMem,_hBitmap) ;
						if(_hBmpOld)
						{
							FillRect(_hDCMem,&_Rc,hBarbieBrush) ;

							SetStretchBltMode(_hDCMem,STRETCH_HALFTONE) ;

							ImageResize(_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_Rc.right,_Rc.bottom,&_dwNewWidth,&_dwNewHeight) ;
							StretchDIBits(_hDCMem,(_Rc.right - _dwNewWidth) / 2,(_Rc.bottom - _dwNewHeight) / 2,_dwNewWidth,_dwNewHeight,30,30,_lpImageInfos->BitmapInfo.bmiHeader.biWidth - 60,_lpImageInfos->BitmapInfo.bmiHeader.biHeight - 60,_lpImageInfos->lpResult,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS,SRCCOPY) ;

							BitBlt(hDCResults,0,0,_Rc.right,_Rc.bottom,_hDCMem,0,0,SRCCOPY) ;
							SelectObject(_hDCMem,_hBmpOld) ;
						}

						DeleteObject(_hBitmap) ;
					}

					DeleteDC(_hDCMem) ;
				}
			}
		}
	}

	return ;
}

void Child4_OnPaint(HWND __hWnd)
{
	register		LPIMAGEINFOS	_lpImageInfos ;
	DWORD			_dwNewWidth, _dwNewHeight ;
	HDC				_hDCMem ;
	HBITMAP			_hBitmap, _hBmpOld ;
	RECT			_Rc ;
	PAINTSTRUCT		_Ps ;

	BeginPaint(__hWnd,&_Ps) ;

	if(IsWindowVisible(__hWnd))
	{
		_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
		if(_lpImageInfos)
		{
			GetClientRect(__hWnd,&_Rc) ;

			_hDCMem = CreateCompatibleDC(hDCResults) ;
			if(_hDCMem)
			{
				_hBitmap = CreateCompatibleBitmap(hDCResults,_Rc.right,_Rc.bottom) ;
				if(_hBitmap)
				{
					_hBmpOld = SelectObject(_hDCMem,_hBitmap) ;
					if(_hBmpOld)
					{
						FillRect(_hDCMem,&_Rc,hBarbieBrush) ;

						SetStretchBltMode(_hDCMem,STRETCH_HALFTONE) ;

						ImageResize(_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_Rc.right,_Rc.bottom,&_dwNewWidth,&_dwNewHeight) ;
						StretchDIBits(_hDCMem,(_Rc.right - _dwNewWidth) / 2,(_Rc.bottom - _dwNewHeight) / 2,_dwNewWidth,_dwNewHeight,30,30,_lpImageInfos->BitmapInfo.bmiHeader.biWidth - 60,_lpImageInfos->BitmapInfo.bmiHeader.biHeight - 60,_lpImageInfos->lpResult,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS,SRCCOPY) ;

						BitBlt(hDCResults,0,0,_Rc.right,_Rc.bottom,_hDCMem,0,0,SRCCOPY) ;

						SelectObject(_hDCMem,_hBmpOld) ;
					}

					DeleteObject(_hBitmap) ;
				}

				DeleteDC(_hDCMem) ;
			}
		}
	}

	EndPaint(__hWnd,&_Ps) ;

	return ;
}

LRESULT CALLBACK Ch4_Result_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam)
{
	switch(__Message)
	{
		case	WM_PAINT :			Child4_OnPaint(__hWnd) ;
									break ;

		case	WM_CREATE :			hTabsWindows[HWND_RESULTS] = __hWnd ;
									hDCResults = GetDC(hTabsWindows[HWND_RESULTS]) ;
									SetStretchBltMode(hDCResults,STRETCH_HALFTONE) ;
									break ;

		default :					return (DefWindowProc(__hWnd,__Message,__wParam,__lParam)) ;
	}

	return (FALSE) ;
}
