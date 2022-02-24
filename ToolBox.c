#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <ShlWApi.h>
#include <uxtheme.h>
#include <time.h>

#pragma warn(disable:2006 2145 2215 2228 2805)

#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Languages.h"
#include "Ressources.h"

int ToolBox_OnClose(void)
{
	RECT	_Rc ;
	char	_szTmp[1024] ;

	LPIMAGEINFOS	_lpImageInfos ;

	if(Questions(3,szQuestion_03) == IDNO)
		return (FALSE) ;

	DragAcceptFiles(hToolBox,FALSE) ;

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(_lpImageInfos)
	{
		if(GetPrivateProfileInt(szProfile_09,szProfile_20,0,szProfileFileName) == 0)
			WritePrivateProfileString(szProfile_09,szProfile_19,szEmptyString,szProfileFileName) ;
		else
		{
			if(!PathFileExists(_lpImageInfos->Image.szImageFile))
				WritePrivateProfileString(szProfile_09,szProfile_19,szEmptyString,szProfileFileName) ;
			else
				WritePrivateProfileString(szProfile_09,szProfile_19,_lpImageInfos->Image.szImageFile,szProfileFileName) ;
		}

		ImageFree(_lpImageInfos) ;
		_lpImageInfos = NULL ;

		SetWindowLongPtr(hToolBox,GWLP_USERDATA,(long long int) NULL) ;
	}

	if(GetPrivateProfileInt(szProfile_09,szProfile_22,0,szProfileFileName) == 0)
		if(GetPrivateProfileInt(szProfile_09,szProfile_23,0,szProfileFileName) == 0)
			WritePrivateProfileString(szProfile_09,szProfile_24,szEmptyString,szProfileFileName) ;

	GetWindowRect(hToolBox,&_Rc) ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_Rc.left) ;
	WritePrivateProfileString(szProfile_09,szProfile_10,_szTmp,szProfileFileName) ;	// X

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_Rc.top) ;
	WritePrivateProfileString(szProfile_09,szProfile_11,_szTmp,szProfileFileName) ;	// Y

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_Rc.right - _Rc.left) ;
	WritePrivateProfileString(szProfile_09,szProfile_12,_szTmp,szProfileFileName) ;	// W

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_Rc.bottom - _Rc.top) ;
	WritePrivateProfileString(szProfile_09,szProfile_13,_szTmp,szProfileFileName) ;	// H

	WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;

	ReleaseDC(hTabsWindows[HWND_WORKSPACE],hDCWorkSpace) ;
	ReleaseDC(hTabsWindows[HWND_ORIGINAL],hDCOriginal) ;
	ReleaseDC(hTabsWindows[HWND_RESULTS],hDCResults) ;
	ReleaseDC(hTabsWindows[HWND_DIAPORAMA],hDCDiaporama) ;

	DestroyWindow(hTabsWindows[HWND_ORIGINAL]) ;
	DestroyWindow(hTabsWindows[HWND_WORKSPACE]) ;
	DestroyWindow(hTabsWindows[HWND_EDITOR]) ;
	DestroyWindow(hTabsWindows[HWND_RESULTS]) ;
	DestroyWindow(hTabsWindows[HWND_DIAPORAMA]) ;
	DestroyWindow(hTabsWindows[HWND_OPTIONS]) ;

	hTabsWindows[HWND_ORIGINAL]  = hTabsWindows[HWND_WORKSPACE] = hTabsWindows[HWND_EDITOR]  = NULL ;
	hTabsWindows[HWND_DIAPORAMA] = hTabsWindows[HWND_RESULTS]   = hTabsWindows[HWND_OPTIONS] = NULL ;

	lpCurrentFilter = NULL ;

	DeleteObject(hBarbieBrush) ;
	hBarbieBrush = NULL ;

	if(GetPrivateProfileInt(szProfile_09,"Vacuum",0,szProfileFileName) == 1)
	{
		hModless = (HWND) CreateDialog(hInstance,MAKEINTRESOURCE(DLG_MODLESS),hToolBox,(DLGPROC) DlgModlessProc) ;
		if(hModless)
		{
			ShowWindow(hModless,SW_SHOW) ;

			SendMessage(hModless,WM_COMMAND,IDM_SET_TEXT_01,(LPARAM) NULL) ;
			SendMessage(hModless,WM_COMMAND,IDM_SET_TEXT_02,(LPARAM) NULL) ;
		}

		sqlite3_exec(hCfeDb,"VACUUM",0,0,NULL) ;
		DestroyWindow(hModless) ;
	}

	DestroyWindow(hToolBox) ;

	return (TRUE) ;
}

LRESULT ToolBox_OnCommand(HWND __hWnd,WPARAM __wParam,LPARAM __lParam)
{
	HANDLE			_hThread ;
	DWORD			_dwTmp ;
	char			_szTmp[512] ;
	char			_szExec[512] ;

	if(HIWORD(__wParam) == BN_CLICKED)
	{
		bFirstDrop = FALSE ;

		switch(LOWORD(__wParam))
		{
			case	BTN_CLEAR :				ClearAllFields() ;						break ;

			case	BTN_DIVISOR :			ComputeTheDivisor() ;					break ;

			case	TLB_BTN_OPEN :			OpenThisFile() ;						break ;

			case	TLB_BTN_NEW :			NewFilter() ;							break ;

			case	TLB_BTN_SAVE :			SaveFilter(TRUE) ;						break ;

			case	TLB_BTN_SAVE_ALL :		_hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE) SaveAllTheFilters,__hWnd,0,NULL) ;
											CloseHandle(_hThread) ;
											break ;

			case	TLB_BTN_SAVE_IMAGE :	SaveFilter(FALSE) ;						break ;

			case	TLB_BTN_UPDATE :		UpdateFilter() ;						break ;

			case	TLB_BTN_APPLY :			ApplyFilter(FALSE) ;					break ;

			case	TLB_BTN_COPY_TO_CLIPBOARD :	ApplyFilter(TRUE) ;					break ;

			case	TLB_BTN_PASTE :			PasteFromClipboard() ;					break ;

			case	TLB_BTN_LOCK :			Filter_SetLocked(lpCurrentFilter->FilterRecordNumber) ;		break ;

			case	TLB_BTN_UNLOCK :		Filter_SetUnLocked(lpCurrentFilter->FilterRecordNumber) ;	break ;

			case	TLB_BTN_DELETE :		DeleteFilter() ;						break ;

			case	TLB_BTN_PREVIOUS :		SetPrevious() ;							break ;

			case	TLB_BTN_RANDOM :		Randomize() ;							break ;

			case	TLB_BTN_INVERSE :		InverseValues() ;						break ;

			case	TLB_BTN_REVERSE :		ReverseValues() ;						break ;

			case	TLB_BTN_COMPUTE :		ComputeTheDivisor() ;					break ;

			case	TLB_BTN_RESET :			ResetFilter() ;							break ;

			case	TLB_BTN_EXPORT :		if(ExportFilters() == TRUE)
												Informations(2,szInformation_02) ;
											else
												Informations(3,szInformation_03) ;

											break ;

			case	TLB_BTN_IMPORT :		Filters_Import() ;					break ;

			case	TLB_BTN_HELP :			_dwTmp = sizeof(_szTmp) ;
											AssocQueryString(0,ASSOCSTR_EXECUTABLE,".chm","open",_szTmp,&_dwTmp) ;
											wsprintf(_szExec,"\"%s\" \".\\CoFeE.chm\"",_szTmp) ;
											WinExec(_szExec,SW_SHOW) ;
											break ;

			case	TLB_BTN_ABOUT :			DialogBox(hInstance,MAKEINTRESOURCE(DLG_ABOUT),__hWnd,(DLGPROC) AboutDlgProc) ;
											break ;

			case	TLB_BTN_QUIT :			ToolBox_OnClose() ;					break ;

			case	TLB_BTN_DIAPORAMA_ON :
			case	TLB_BTN_DIAPORAMA_OFF :	Diaporama() ;						break ;

			default :						return (DefWindowProc(__hWnd,WM_COMMAND,__wParam,__lParam)) ;
		}
	}

	return (FALSE) ;
}

LRESULT ToolBox_OnSize(DWORD __dwNewWidth,DWORD  __dwNewHeight)
{
				RECT	_RcStatus, _RcTabs ;

	register	int		_Y ;
	register	int		_ClientHeight ;
				int		_StatusHeight ;
				int		_TabsHeight ;
				int		_ToolBarHeight ;

	_ToolBarHeight = 34 ;

	MoveWindow(hStatus,0,0,__dwNewWidth,24,TRUE) ;

	GetWindowRect(hStatus,&_RcStatus) ;

	MoveWindow(hTabs,0,_ToolBarHeight,__dwNewWidth,32,TRUE) ;
	GetWindowRect(hTabs,&_RcTabs) ;

	_StatusHeight = _RcStatus.bottom - _RcStatus.top ;
	_TabsHeight = _RcTabs.bottom - _RcTabs.top ;

	_Y = _ToolBarHeight + _TabsHeight ;

	_ClientHeight = __dwNewHeight - _StatusHeight - _TabsHeight - _ToolBarHeight ;

	MoveWindow(hTabsWindows[HWND_ORIGINAL],	0,_Y,__dwNewWidth,_ClientHeight,TRUE) ;
	MoveWindow(hTabsWindows[HWND_WORKSPACE],0,_Y,__dwNewWidth,_ClientHeight,TRUE) ;
	MoveWindow(hTabsWindows[HWND_EDITOR],	0,_Y,__dwNewWidth,_ClientHeight,TRUE) ;
	MoveWindow(hTabsWindows[HWND_RESULTS],	0,_Y,__dwNewWidth,_ClientHeight,TRUE) ;
	MoveWindow(hTabsWindows[HWND_DIAPORAMA],0,_Y,__dwNewWidth,_ClientHeight,TRUE) ;
	MoveWindow(hTabsWindows[HWND_OPTIONS],	0,_Y,__dwNewWidth,_ClientHeight,TRUE) ;

	return (0) ;
}

void ToolBox_OnCreate(HWND __hWnd)
{
	TCITEM			_Tci ;

	HDC		_hDC ;
	int		_nHeight ;

	hToolBox = __hWnd ;

	SetTimer(__hWnd,TIMER_01,5000,NULL) ;

//	****************************************
//	*** Create the modeless dialog boxes ***
//	****************************************

	hModless = (HWND) CreateDialog(hInstance,MAKEINTRESOURCE(DLG_MODLESS),__hWnd,(DLGPROC) DlgModlessProc) ;

//	*********************************
//	*** The messages are sent and ***
//	*** the timer is set only if  ***
//	*** the modeless bow exist    ***
//	*********************************

	if(hModless)
	{
		ShowWindow(hModless,SW_SHOW) ;

		SendMessage(hModless,WM_COMMAND,IDM_SET_TEXT_01,(LPARAM) NULL) ;
		SendMessage(hModless,WM_COMMAND,IDM_SET_TEXT_02,(LPARAM) NULL) ;
	}

	hBarbieBrush = CreateSolidBrush(lpCurrentTheme->CtlColorDlg) ;

//	******************************
//	*** Accept the Drag'n Drop ***
//	******************************

	DragAcceptFiles(__hWnd,TRUE) ;

//	************************
//	*** Create the fonts ***
//	************************

//	*******************************
//	*** Compute the fonts sizes ***
//	*******************************

	_hDC = GetDC(__hWnd) ;
	_nHeight = -MulDiv(9,GetDeviceCaps(_hDC,LOGPIXELSY),72) ;
	hMainFont = CreateFont(_nHeight,0,0,0,FW_EXTRALIGHT,0,0,0,ANSI_CHARSET,OUT_TT_ONLY_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,FF_SWISS,szTahoma) ;

	_nHeight = -MulDiv(8,GetDeviceCaps(_hDC,LOGPIXELSY),72) ;
	hDialogFont = CreateFont(_nHeight,0,0,0,FW_EXTRALIGHT,0,0,0,ANSI_CHARSET,OUT_TT_ONLY_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,FF_SWISS,szTahoma) ;

	ReleaseDC(__hWnd,_hDC) ;

	hStatus = CreateWindow(STATUSCLASSNAME,NULL,WS_CHILD|WS_VISIBLE,0,0,0,0,__hWnd,(HMENU) IDC_STATUSBAR,hInstance,NULL) ;
	SetWindowTheme(__hWnd,L"explorer",NULL) ;

	SendMessage(hStatus,SB_SETPARTS,(WPARAM) 6,(LPARAM) StatWidths) ;
	SendMessage(hStatus,(UINT) SB_SETTEXT,(WPARAM)(INT) 0,(LPARAM) (LPSTR) "Welcome") ;

	CreateNewToolbar(__hWnd) ;

	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_02_NEW),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_04_SAVE_ALL),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_05_SAVE_IMAGE),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_06_UPDATE),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_07_APPLY),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_08_COPY_TO_CLIPBOARD),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_09_PASTE),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_10_LOCK),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_11_UNLOCK),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_12_DELETE),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_13_PREVIOUS),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_14_RANDOM),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_15_INVERSE),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_16_REVERSE),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_17_COMPUTE),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_19_EXPORT),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_20_IMPORT),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),SW_HIDE) ;

	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_01_OPEN),SW_SHOW) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),SW_HIDE) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),SW_SHOW) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),SW_SHOW) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),SW_SHOW) ;
	ShowWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),SW_SHOW) ;

	MoveWindow(GetDlgItem(hToolBox,TLB_BTN_01_OPEN),0,0,34,34,TRUE) ;
	MoveWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),34,0,34,34,TRUE) ;
	MoveWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),68,0,34,34,TRUE) ;
	MoveWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),102,0,34,34,TRUE) ;
	MoveWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),136,0,34,34,TRUE) ;

//	*******************************
//	*** Create the first filter ***
//	*******************************

	CreateFilter() ;

//	***********************************
//	*** Create the children windows ***
//	***********************************

	hTabsWindows[HWND_WORKSPACE]	= CreateWindowEx(0,"CHILD_2",szEmptyString,WS_CHILD|WS_VISIBLE,0,0,0,0,__hWnd,(HMENU) 0x1960,hInstance,NULL) ;
	hTabsWindows[HWND_EDITOR]		= CreateWindowEx(0,"CHILD_3",szEmptyString,WS_CHILD|WS_VISIBLE,0,0,0,0,__hWnd,(HMENU) 0x2014,hInstance,NULL) ;
	hTabsWindows[HWND_DIAPORAMA]	= CreateWindowEx(0,"CHILD_5",szEmptyString,WS_CHILD|WS_VISIBLE,0,0,0,0,__hWnd,(HMENU) 0x1995,hInstance,NULL) ;
	hTabsWindows[HWND_RESULTS]		= CreateWindowEx(0,"CHILD_4",szEmptyString,WS_CHILD|WS_VISIBLE,0,0,0,0,__hWnd,(HMENU) 0x1987,hInstance,NULL) ;
	hTabsWindows[HWND_ORIGINAL]		= CreateWindowEx(0,"CHILD_1",szEmptyString,WS_CHILD|WS_VISIBLE,0,0,0,0,__hWnd,(HMENU) 0x1959,hInstance,NULL) ;
	hTabsWindows[HWND_OPTIONS]		= CreateWindowEx(0,"CHILD_6",szEmptyString,WS_CHILD|WS_VISIBLE,0,0,0,0,__hWnd,(HMENU) 0x1989,hInstance,NULL) ;

	hTabs = CreateWindow(WC_TABCONTROL,szEmptyString,WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|TCS_FLATBUTTONS|TCS_SINGLELINE|TCS_TABS,0,0,0,0,__hWnd,(HMENU) TAB_ONGLET,hInstance,NULL) ;
	SetWindowTheme(__hWnd,L"explorer",NULL) ;

	SendMessage(hTabs,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;
	SendMessage(hStatus,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

//	*****************************
//	*** Initalize tab Control ***
//	*****************************

	_Tci.mask = TCIF_TEXT|TCIF_PARAM ;
	_Tci.pszText = Statics(26) ;
	_Tci.lParam = (LPARAM) hTabsWindows[HWND_ORIGINAL] ;

	TabCtrl_InsertItem(hTabs,0,&_Tci) ;

	_Tci.pszText = Statics(24) ;
	_Tci.lParam = (LPARAM) hTabsWindows[HWND_WORKSPACE] ;

	TabCtrl_InsertItem(hTabs,1,&_Tci) ;

	_Tci.pszText = Statics(25) ;
	_Tci.lParam = (LPARAM) hTabsWindows[HWND_EDITOR] ;

	TabCtrl_InsertItem(hTabs,2,&_Tci) ;

	_Tci.pszText = Statics(28) ;
	_Tci.lParam = (LPARAM) hTabsWindows[HWND_RESULTS] ;

	TabCtrl_InsertItem(hTabs,3,&_Tci) ;

	_Tci.pszText = Statics(45) ;
	_Tci.lParam = (LPARAM) hTabsWindows[HWND_DIAPORAMA] ;

	TabCtrl_InsertItem(hTabs,4,&_Tci) ;

	_Tci.pszText = Statics(27) ;
	_Tci.lParam = (LPARAM) hTabsWindows[HWND_OPTIONS] ;

	TabCtrl_InsertItem(hTabs,5,&_Tci) ;

	TabCtrl_SetCurSel(hTabs,0) ;

//	***************************************************
//	*** Hide the diaglog boxes except the first one ***
//	***************************************************

	ShowWindow(hTabsWindows[HWND_OPTIONS],SW_HIDE) ;
	ShowWindow(hTabsWindows[HWND_DIAPORAMA],SW_HIDE) ;
	ShowWindow(hTabsWindows[HWND_RESULTS],SW_HIDE) ;
	ShowWindow(hTabsWindows[HWND_EDITOR],SW_HIDE) ;
	ShowWindow(hTabsWindows[HWND_WORKSPACE],SW_HIDE) ;
	ShowWindow(hTabsWindows[HWND_ORIGINAL],SW_SHOW) ;

	SendMessage(hStatus,(UINT) SB_SETTEXT,(WPARAM)(INT) 3,(LPARAM) (LPSTR) "Version 2.2022.0.1263") ;

	return ;
}

void ToolBox_OnDrop(HDROP __hDrop)
{
	LPIMAGEINFOS	_lpImageInfos ;
	char			_szFileName[1024], _szTmp[1024] ;

	if(!DragQueryFile(__hDrop,0,_szFileName,sizeof(_szFileName)))
		return ;

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(_lpImageInfos)
	{
		ImageFree(_lpImageInfos) ;
		SetWindowLongPtr(hToolBox,GWLP_USERDATA,(long long int) NULL) ;
	}

	_lpImageInfos = LoadImageFromFile(_szFileName,MAX_EDIT_COLUMNS,MAX_EDIT_ROWS) ;
	if(!_lpImageInfos)
	{
		DragFinish(__hDrop) ;
		Error(6) ;

		return ;
	}

	_lpImageInfos->StructureSize = sizeof(IMAGEINFOS) ;

	SetWindowLongPtr(hToolBox,GWLP_USERDATA,(long long int) _lpImageInfos) ;

	lstrcpy(_szFileName,_lpImageInfos->Image.szImageFile) ;
	PathStripPath(_szFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"Convolution Filters Editor: [%s]",_szFileName) ;
	SetWindowText(hToolBox,_szTmp) ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d x %d",_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight) ;
	SendMessage(hStatus,(UINT) SB_SETTEXT,(WPARAM)(INT) 2,(LPARAM) (LPSTR) _szTmp) ;

	DragFinish(__hDrop) ;

	InvalidateRect(hTabsWindows[HWND_ORIGINAL],NULL,TRUE) ;
	InvalidateRect(hTabsWindows[HWND_RESULTS],NULL,TRUE) ;
	InvalidateRect(hTabsWindows[HWND_WORKSPACE],NULL,TRUE) ;
	InvalidateRect(hTabsWindows[HWND_DIAPORAMA],NULL,TRUE) ;

	bFirstDrop = TRUE ;

	return ;
}

void ToolBox_OnDestroy(void)
{
	if(hModless)
	{
		KillTimer(hToolBox,TIMER_01) ;
		DestroyWindow(hModless) ;
		hModless = NULL ;
	}

	KillTimer(hToolBox,ID_DIAPORAMA) ;

	DeleteObject(hMainFont) ;
	DeleteObject(hDialogFont) ;

	DeleteObject(hButtons) ;
	DeleteObject(hIcons) ;

	ImageList_Destroy(hImageListRating) ;
	ImageList_Destroy(hImageListEditor) ;

	PostQuitMessage(0) ;

	return ;
}

LRESULT ToolBox_OnMinMax(LPMINMAXINFO __lpMinMaxInfos)
{
	__lpMinMaxInfos->ptMinTrackSize.x = MIN_WINDOW_WIDTH ;
	__lpMinMaxInfos->ptMinTrackSize.y = MIN_WINDOW_HEIGHT ;

	__lpMinMaxInfos->ptMaxTrackSize.x = GetSystemMetrics(SM_CXMAXTRACK) ;
	__lpMinMaxInfos->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) ;

	__lpMinMaxInfos->ptMaxPosition.x = 0 ;
	__lpMinMaxInfos->ptMaxPosition.y = 0 ;

	return (0) ;
}

int ToolBox_OnNotify(HWND __hWnd,LPNMHDR __lpNmHdr)
{
	HWND			_hTabs ;

	_hTabs = GetDlgItem(__hWnd,TAB_ONGLET) ;

	switch(__lpNmHdr->code)
	{
		case	TCN_SELCHANGING :	ShowWindow(hTabsWindows[TabCtrl_GetCurSel(_hTabs)],SW_HIDE) ;

									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_01_OPEN),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_02_NEW),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_04_SAVE_ALL),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_05_SAVE_IMAGE),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_06_UPDATE),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_07_APPLY),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_08_COPY_TO_CLIPBOARD),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_09_PASTE),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_10_LOCK),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_11_UNLOCK),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_12_DELETE),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_13_PREVIOUS),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_14_RANDOM),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_15_INVERSE),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_16_REVERSE),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_17_COMPUTE),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_19_EXPORT),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_20_IMPORT),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),SW_HIDE) ;
									ShowWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),SW_HIDE) ;

									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_01_OPEN),0,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_02_NEW),34,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),68,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_04_SAVE_ALL),102,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_05_SAVE_IMAGE),136,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_06_UPDATE),170,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_07_APPLY),204,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_08_COPY_TO_CLIPBOARD),238,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_09_PASTE),272,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_10_LOCK),306,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_11_UNLOCK),340,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_12_DELETE),374,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_13_PREVIOUS),408,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_14_RANDOM),442,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_15_INVERSE),476,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_16_REVERSE),510,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_17_COMPUTE),544,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),576,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_19_EXPORT),610,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_20_IMPORT),644,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),678,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),678,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),712,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),746,0,34,34,TRUE) ;
									MoveWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),780,0,34,34,TRUE) ;

									return (FALSE) ;

		case	TCN_SELCHANGE :		ShowWindow(hTabsWindows[TabCtrl_GetCurSel(_hTabs)],SW_SHOW) ;

									switch(TabCtrl_GetCurSel(_hTabs))
									{
										case	0 :	
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_01_OPEN),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),SW_HIDE) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),SW_SHOW) ;

													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_01_OPEN),0,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),34,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),68,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),102,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),136,0,34,34,TRUE) ;

													break ;

										case	1 :	
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_01_OPEN),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_02_NEW),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_04_SAVE_ALL),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_05_SAVE_IMAGE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_06_UPDATE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_07_APPLY),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_08_COPY_TO_CLIPBOARD),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_09_PASTE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_10_LOCK),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_11_UNLOCK),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_12_DELETE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_13_PREVIOUS),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_14_RANDOM),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_15_INVERSE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_16_REVERSE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_17_COMPUTE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_19_EXPORT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_20_IMPORT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),SW_SHOW) ;

													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),678,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),712,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),746,0,34,34,TRUE) ;
													break ;

										case	2 :	
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_04_SAVE_ALL),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_05_SAVE_IMAGE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_06_UPDATE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_07_APPLY),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_08_COPY_TO_CLIPBOARD),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_10_LOCK),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_11_UNLOCK),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_12_DELETE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_13_PREVIOUS),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_14_RANDOM),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_15_INVERSE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_16_REVERSE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_17_COMPUTE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),SW_SHOW) ;

													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),0,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_04_SAVE_ALL),34,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_05_SAVE_IMAGE),68,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_06_UPDATE),102,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_07_APPLY),136,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_08_COPY_TO_CLIPBOARD),170,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_10_LOCK),204,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_11_UNLOCK),238,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_12_DELETE),272,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_13_PREVIOUS),306,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_14_RANDOM),340,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_15_INVERSE),374,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_16_REVERSE),408,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_17_COMPUTE),442,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),476,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),508,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),542,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),576,0,34,34,TRUE) ;

													break ;

										case	3 :	
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_04_SAVE_ALL),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_05_SAVE_IMAGE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_06_UPDATE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_07_APPLY),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_08_COPY_TO_CLIPBOARD),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_10_LOCK),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_11_UNLOCK),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_12_DELETE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_13_PREVIOUS),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_14_RANDOM),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_15_INVERSE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_16_REVERSE),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),SW_SHOW) ;

													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_03_SAVE),0,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_04_SAVE_ALL),34,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_05_SAVE_IMAGE),68,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_06_UPDATE),102,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_07_APPLY),136,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_08_COPY_TO_CLIPBOARD),170,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_10_LOCK),204,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_11_UNLOCK),238,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_12_DELETE),272,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_13_PREVIOUS),306,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_14_RANDOM),340,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_15_INVERSE),374,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_16_REVERSE),408,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_18_RESET),442,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),476,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),508,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),542,0,34,34,TRUE) ;

													break ;

										case	4 :	
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),SW_SHOW) ;

													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_25_DIAPORAMA_ON),0,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_21_DIAPORAMA_OFF),0,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),34,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),68,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),102,0,34,34,TRUE) ;

													break ;

										case	5 :	
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_19_EXPORT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_20_IMPORT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),SW_SHOW) ;
													ShowWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),SW_SHOW) ;

													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_19_EXPORT),0,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_20_IMPORT),34,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_22_HELP),68,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_23_ABOUT),102,0,34,34,TRUE) ;
													MoveWindow(GetDlgItem(hToolBox,TLB_BTN_24_QUIT),136,0,34,34,TRUE) ;

													break ;
									}

									return (FALSE) ;
	}

	return (TRUE) ;
}

void WmDrawItem(int __iCtrlId,LPDRAWITEMSTRUCT __lpDis)
{
	HDC			_hDCMem_1, _hDCMem_2 ;
	HBITMAP		_hBmpMem_1, hBmpButtonsOld, _hBmpOld ;
	int			_iControl ;

	_hDCMem_1 = CreateCompatibleDC(__lpDis->hDC) ;
	if(_hDCMem_1)
	{
		_hBmpMem_1 = CreateCompatibleBitmap(__lpDis->hDC,34,34) ;
		if(_hBmpMem_1)
		{
			_hBmpOld = SelectObject(_hDCMem_1,_hBmpMem_1) ;
			if(_hBmpOld)
			{
				_hDCMem_2 = CreateCompatibleDC(__lpDis->hDC) ;
				if(_hDCMem_2)
				{
					hBmpButtonsOld = SelectObject(_hDCMem_2,hButtons) ;
					if(hBmpButtonsOld)
					{
						if(__lpDis->itemState & ODS_SELECTED)
							BitBlt(_hDCMem_1,0,0,34,34,_hDCMem_2,34,0,SRCCOPY) ;
						else
							BitBlt(_hDCMem_1,0,0,34,34,_hDCMem_2,0,0,SRCCOPY) ;

						_iControl = (__iCtrlId - TLB_BTN_01_OPEN) * 32 ;

						SelectObject(_hDCMem_2,hBmpButtonsOld) ;

						hBmpButtonsOld = SelectObject(_hDCMem_2,hIcons) ;

						if(__lpDis->itemState & ODS_SELECTED)
							BitBlt(_hDCMem_1,1,2,32,32,_hDCMem_2,_iControl,0,SRCCOPY) ;
						else
							BitBlt(_hDCMem_1,0,1,32,32,_hDCMem_2,_iControl,0,SRCCOPY) ;

						SelectObject(_hDCMem_2,hBmpButtonsOld) ;
					}

					DeleteDC(_hDCMem_2) ;
				}

				BitBlt(__lpDis->hDC,0,0,32,32,_hDCMem_1,0,0,SRCCOPY) ;

				SelectObject(_hDCMem_1,_hBmpOld) ;
			}

			DeleteObject(_hBmpMem_1) ;
		}

		DeleteDC(_hDCMem_1) ;
	}

	return ;
}

LRESULT CALLBACK ToolBoxWndProc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam)
{
	LPMEASUREITEMSTRUCT		_lpMis ;

	switch(__Message)
	{
		case	WM_DRAWITEM :		WmDrawItem(__wParam,(LPDRAWITEMSTRUCT) __lParam) ;
									return (TRUE) ;

		case	WM_MEASUREITEM :	_lpMis = (LPMEASUREITEMSTRUCT) __lParam ;
									_lpMis->itemWidth = _lpMis->itemHeight = 34 ;
									return (TRUE) ;

		case	WM_NOTIFY :			return (ToolBox_OnNotify(__hWnd,(LPNMHDR) __lParam)) ;

		case	WM_COMMAND :		return (ToolBox_OnCommand(__hWnd,__wParam,__lParam)) ;

		case	WM_DROPFILES :		ToolBox_OnDrop((HDROP) __wParam) ;		break ;

		case	WM_CTLCOLORDLG :	return ((LRESULT) GetStockObject(DKGRAY_BRUSH)) ;

		case	WM_TIMER :			KillTimer(__hWnd,TIMER_01) ;
									DestroyWindow(hModless) ;
									hModless = NULL ;
									break ;

		case	WM_GETMINMAXINFO :	return (ToolBox_OnMinMax((LPMINMAXINFO) __lParam)) ;
		case	WM_SIZE :			return (ToolBox_OnSize(LOWORD(__lParam),HIWORD(__lParam))) ;
		case	WM_DESTROY :		ToolBox_OnDestroy() ;		break ;
		case	WM_CLOSE :			ToolBox_OnClose() ;			break ;
		case	WM_CREATE :			ToolBox_OnCreate(__hWnd) ;	break ;

		default :					return (DefWindowProc(__hWnd,__Message,__wParam,__lParam)) ;
	}

	return (FALSE) ;
}
