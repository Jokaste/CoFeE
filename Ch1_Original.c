#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <CommCtrl.h>

#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"

void PaintOriginal(void)
{
	register		LPIMAGEINFOS	_lpImageInfos ;
	DWORD			_dwNewWidth, _dwNewHeight ;
	HDC				_hDCMem ;
	HBITMAP			_hBitmap, _hBmpOld ;
	HWND			_hWnd ;
	RECT			_Rc ;
	PAINTSTRUCT		_Ps ;

	_hWnd = hTabsWindows[HWND_ORIGINAL] ;

	BeginPaint(_hWnd,&_Ps) ;
	if(hDCOriginal)
	{
		_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
		if(_lpImageInfos)
		{
			GetClientRect(_hWnd,&_Rc) ;

			_hDCMem = CreateCompatibleDC(hDCOriginal) ;
			if(_hDCMem)
			{
				_hBitmap = CreateCompatibleBitmap(hDCOriginal,_Rc.right,_Rc.bottom) ;
				if(_hBitmap)
				{
					_hBmpOld = SelectObject(_hDCMem,_hBitmap) ;
					if(_hBmpOld)
					{
						FillRect(_hDCMem,&_Rc,hBarbieBrush) ;

						SetStretchBltMode(_hDCMem,STRETCH_HALFTONE) ;

						ImageResize(_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_Rc.right,_Rc.bottom,&_dwNewWidth,&_dwNewHeight) ;
						StretchDIBits(_hDCMem,(_Rc.right - _dwNewWidth) / 2,(_Rc.bottom - _dwNewHeight) / 2,_dwNewWidth,_dwNewHeight,MAX_EDIT_COLUMNS,MAX_EDIT_ROWS,_lpImageInfos->BitmapInfo.bmiHeader.biWidth - (2 * MAX_EDIT_COLUMNS),_lpImageInfos->BitmapInfo.bmiHeader.biHeight - (2 * MAX_EDIT_ROWS),_lpImageInfos->lpOriginal,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS,SRCCOPY) ;

						BitBlt(hDCOriginal,0,0,_Rc.right,_Rc.bottom,_hDCMem,0,0,SRCCOPY) ;
						SelectObject(_hDCMem,_hBmpOld) ;
					}

					DeleteObject(_hBitmap) ;
				}

				DeleteDC(_hDCMem) ;
			}
		}

		EndPaint(_hWnd,&_Ps) ;
	}

	return ;
}

LRESULT CALLBACK Ch1_Original_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam)
{
	switch(__Message)
	{
		case	WM_CTLCOLORDLG :	return ((LRESULT) hBarbieBrush) ;

		case	WM_PAINT :			PaintOriginal() ;
									break ;

		case	WM_CREATE :			hTabsWindows[HWND_ORIGINAL] = __hWnd ;
									hDCOriginal = GetDC(hTabsWindows[HWND_ORIGINAL]) ;
									SetStretchBltMode(hDCOriginal,STRETCH_HALFTONE) ;
									break ;

		default :					return (DefWindowProc(__hWnd,__Message,__wParam,__lParam)) ;
	}

	return (FALSE) ;
}
