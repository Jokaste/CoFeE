#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <commdlg.h>
#include <wingdi.h>
#include <math.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <intrin.h>
#include <time.h>
#include <uxtheme.h>

#pragma warn(disable:2006 2145 2215 2228 2805)

#include "zeegrid.h"
#include "sqlite3.h"

#include "libgfl.h"
#include "libgfle.h"

#include "sph_keccak.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Languages.h"
#include "Ressources.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static void Radio_01(void)
{
	HWND	_hWorkSpace ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	switch(lpCurrentFilter->Grey)
	{
		case	GREY_NONE :					lpCurrentFilter->Grey = GREY_INPUT ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_UNCHECKED) ;
											break ;

		case	GREY_INPUT :				lpCurrentFilter->Grey = GREY_NONE ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											break ;

		case	GREY_OUTPUT :				lpCurrentFilter->Grey = GREY_INPUT|GREY_OUTPUT ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_CHECKED) ;
											break ;

		case	GREY_INPUT|GREY_OUTPUT :	lpCurrentFilter->Grey = GREY_OUTPUT ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											break ;
	}

	ApplyFilter(FALSE) ;

	return ;
}

static void Radio_02(void)
{
	HWND	_hWorkSpace ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	switch(lpCurrentFilter->Grey)
	{
		case	GREY_NONE :					lpCurrentFilter->Grey = GREY_OUTPUT ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_UNCHECKED) ;
											break ;

		case	GREY_INPUT :				lpCurrentFilter->Grey = GREY_OUTPUT|GREY_INPUT ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_CHECKED) ;
											break ;

		case	GREY_OUTPUT :				lpCurrentFilter->Grey = GREY_NONE ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											break ;

		case	GREY_INPUT|GREY_OUTPUT :	lpCurrentFilter->Grey = GREY_INPUT ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
											break ;
	}

	ApplyFilter(FALSE) ;

	return ;
}

static void Radio_03(void)
{
	HWND	_hWorkSpace ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	switch(lpCurrentFilter->Grey)
	{
		case	GREY_NONE :					CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											break ;

		case	GREY_INPUT :				lpCurrentFilter->Grey = GREY_NONE ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											break ;

		case	GREY_OUTPUT :				lpCurrentFilter->Grey = GREY_NONE ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											break ;

		case	GREY_INPUT|GREY_OUTPUT :	lpCurrentFilter->Grey = GREY_NONE ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											break ;
	}

	ApplyFilter(FALSE) ;

	return ;
}

//	-----------------
//	--- ***** = 1 ---
//	--- ****  = 2 ---
//	--- ***   = 3 ---
//	--- **    = 4 ---
//	--- *     = 5 ---
//	--- 0     = 0 ---
//	-----------------

void GetRating(HWND __hWnd)
{
	HWND	_hWnd ;

	_hWnd = GetDlgItem(__hWnd,COMBO_FILTER_RATING) ;

	lpCurrentFilter->Rate = (int) SendMessage(_hWnd,CB_GETCURSEL,0,0) ;

	SetFocus(GetDlgItem(__hWnd,LISTBOX_MODELS)) ;

	return ;
}

void SetSimilarity(double __dSimilarity)
{
	char	_szTmp[1024] ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%6.2f%%",__dSimilarity) ;
	SetDlgItemText(hTabsWindows[HWND_WORKSPACE],IDC_SIMILARITY,_szTmp) ;

	return ;
}

int Child2_OnCreate(HWND __hWnd)
{
	register		size_t			_i ;
	register		int				_iIndex ;
					HWND			_hWnd, _hWorkSpace ;
					LPSTR			_lpszTmp ;
					LPIMAGEINFOS	_lpImageInfos ;
					COMBOBOXEXITEM	_Cbbi ;
					char			_szFileName[1024], _szTmp[1024] ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;
	hDCWorkSpace = GetDC(hTabsWindows[HWND_WORKSPACE]) ;
	SetStretchBltMode(hDCWorkSpace,STRETCH_HALFTONE) ;

//	*************
//	*** Sizes ***
//	*************

	_hWnd = CreateWindowEx(0,"ComboBox",szEmptyString,
			WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWNLIST|CBS_SORT|CBS_HASSTRINGS|WS_VSCROLL,
			4,4,120,130,__hWnd,(HMENU) COMBO_SIZES,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static","Sizes",WS_CHILD|WS_VISIBLE|SS_CENTER,
			124,4,120,16,__hWnd,(HMENU) IDC_STATIC_01,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	SetDlgItemText(__hWnd,IDC_STATIC_01,Statics(29)) ;	// Sizes

//	---> Now fill the combobox <---

	_hWnd = GetDlgItem(__hWnd,COMBO_SIZES) ;

	_i = 0 ;

	while(Combos[_i].Index)
	{
		_iIndex = (int) SendMessage(_hWnd,CB_ADDSTRING,0,(LPARAM) Statics(_i + 10)) ;
		SendMessage(_hWnd,CB_SETITEMDATA,_iIndex,Combos[_i].Index) ;

		_i++ ;
	}

//	---> Select the first size

	SendDlgItemMessage(__hWnd,COMBO_SIZES,CB_SETCURSEL,0,0) ;

//	**************
//	*** Select ***
//	**************

	_hWnd = CreateWindowEx(0,"Button",Buttons(1),
			WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER,
			4,30,120,24,__hWnd,(HMENU) BTN_SELECT,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button",Buttons(2),
			WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER,
			130,30,120,24,__hWnd,(HMENU) BTN_UNSELECT,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(31),
			WS_CHILD|WS_VISIBLE|SS_CENTER,
			4,60,250,40,__hWnd,(HMENU) IDC_STATIC_03,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

//	**************
//	*** Models ***
//	**************

	_hWnd = CreateWindowEx(0,"Static",Statics(30),WS_CHILD|WS_VISIBLE|SS_CENTER,
			4,100,250,16,__hWnd,(HMENU) IDC_STATIC_02,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"LISTBOX",szEmptyString,
			WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_HASSTRINGS|LBS_NOTIFY|LBS_SORT|WS_VSCROLL,
			4,120,250,430,__hWnd,(HMENU) LISTBOX_MODELS,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	SetFiltersToListBox() ;

//	*************************
//	*** Rating the filter ***
//	*************************

	_hWnd = CreateWindowEx(0,"Static",Statics(43),WS_CHILD|WS_VISIBLE|SS_CENTER,
			380,350,190,24,__hWnd,(HMENU) IDC_STATIC_15,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,WC_COMBOBOXEX,szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_BITMAP|CBS_DROPDOWNLIST,
			260,350,110,200,__hWnd,(HMENU) COMBO_FILTER_RATING,hInstance,NULL) ;

	_Cbbi.pszText = NULL ;
	_Cbbi.cchTextMax = 0 ;
	_Cbbi.iOverlay = 0 ;
	_Cbbi.iIndent = 0 ;

	_Cbbi.mask = CBEIF_DI_SETITEM|CBEIF_IMAGE|CBEIF_LPARAM|CBEIF_SELECTEDIMAGE ;

	_Cbbi.iItem = 0 ;	_Cbbi.iImage = 6 ;	_Cbbi.iSelectedImage = 0 ;	_Cbbi.lParam = 0 ;

	SendMessage(_hWnd,CBEM_INSERTITEM,0,(LPARAM) &_Cbbi) ;

	_Cbbi.iItem = 1 ;	_Cbbi.iImage = 11 ;	_Cbbi.iSelectedImage = 5 ;	_Cbbi.lParam = 5 ;

	SendMessage(_hWnd,CBEM_INSERTITEM,0,(LPARAM) &_Cbbi) ;

	_Cbbi.iItem = 2 ;	_Cbbi.iImage = 10 ;	_Cbbi.iSelectedImage = 4 ;	_Cbbi.lParam = 4 ;

	SendMessage(_hWnd,CBEM_INSERTITEM,0,(LPARAM) &_Cbbi) ;

	_Cbbi.iItem = 3 ;	_Cbbi.iImage = 9 ;	_Cbbi.iSelectedImage = 3 ;	_Cbbi.lParam = 3 ;

	SendMessage(_hWnd,CBEM_INSERTITEM,0,(LPARAM) &_Cbbi) ;

	_Cbbi.iItem = 4 ;	_Cbbi.iImage = 8 ;	_Cbbi.iSelectedImage = 2 ;	_Cbbi.lParam = 2 ;

	SendMessage(_hWnd,CBEM_INSERTITEM,0,(LPARAM) &_Cbbi) ;

	_Cbbi.iItem = 5 ;	_Cbbi.iImage = 7 ;	_Cbbi.iSelectedImage = 1 ;	_Cbbi.lParam = 1 ;

	_iIndex = (int) SendMessage(_hWnd,CBEM_INSERTITEM,0,(LPARAM) &_Cbbi) ;
	SendMessage(_hWnd,CB_SETCURSEL,_iIndex,0) ;

	hImageListRating = ImageList_LoadImageA(hInstance,MAKEINTRESOURCE(BMP_RATING),80,0,CLR_NONE,IMAGE_BITMAP,LR_CREATEDIBSECTION) ;

	SendMessage(_hWnd,CBEM_SETIMAGELIST,0,(LPARAM) hImageListRating) ;
	SetWindowPos(_hWnd,NULL,260,350,110,200,SWP_NOACTIVATE) ;

//	*************************
//	*** Query filters (1) ***
//	*************************

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|WS_GROUP|BS_AUTORADIOBUTTON|BS_LEFT|BS_VCENTER,
			260,170,12,15,__hWnd,(HMENU) RADIO_4_RATED_UNRATED,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Buttons(8),
			WS_CHILD|WS_VISIBLE|SS_LEFT,
			275,170,150,15,__hWnd,(HMENU) IDC_STATIC_19,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_LEFT|BS_VCENTER,
			260,185,12,15,__hWnd,(HMENU) RADIO_5_RATED,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Buttons(9),
			WS_CHILD|WS_VISIBLE|SS_LEFT,
			275,185,150,15,__hWnd,(HMENU) IDC_STATIC_20,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_LEFT|BS_VCENTER,
			260,200,12,15,__hWnd,(HMENU) RADIO_6_UNRATED,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Buttons(10),
			WS_CHILD|WS_VISIBLE|SS_LEFT,
			275,200,150,15,__hWnd,(HMENU) IDC_STATIC_21,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	CheckRadioButton(_hWorkSpace,RADIO_4_RATED_UNRATED,RADIO_6_UNRATED,RADIO_4_RATED_UNRATED) ;

//	*************************
//	*** Query filters (2) ***
//	*************************

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|WS_GROUP|BS_AUTORADIOBUTTON|BS_LEFT|BS_VCENTER,
			260,220,12,15,__hWnd,(HMENU) RADIO_1_BLACK_WHITE,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Buttons(5),
			WS_CHILD|WS_VISIBLE|SS_LEFT,
			275,220,150,15,__hWnd,(HMENU) IDC_STATIC_16,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_LEFT|BS_VCENTER,
			260,235,12,15,__hWnd,(HMENU) RADIO_2_BLACK,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Buttons(6),
			WS_CHILD|WS_VISIBLE|SS_LEFT,
			275,235,150,15,__hWnd,(HMENU) IDC_STATIC_17,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_LEFT|BS_VCENTER,
			260,250,12,15,__hWnd,(HMENU) RADIO_3_WHITE,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Buttons(7),
			WS_CHILD|WS_VISIBLE|SS_LEFT,
			275,250,150,15,__hWnd,(HMENU) IDC_STATIC_18,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	CheckRadioButton(_hWorkSpace,RADIO_1_BLACK_WHITE,RADIO_3_WHITE,RADIO_1_BLACK_WHITE) ;

//	***************
//	*** Request ***
//	***************

	_hWnd = CreateWindowEx(0,"Button",Buttons(11),
			WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER,
			260,270,150,24,__hWnd,(HMENU) BTN_REQUEST,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

//	***********************
//	*** Write a comment ***
//	***********************

	_hWnd = CreateWindowEx(0,"Edit",Strings(1),WS_CHILD|WS_VISIBLE|ES_CENTER|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL|WS_BORDER,
			260,380,300,150,__hWnd,(HMENU) EDIT_COMMENT,hInstance,NULL) ;
	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(41),
			WS_CHILD|WS_VISIBLE|SS_CENTER,260,530,300,40,__hWnd,(HMENU) IDC_STATIC_13,hInstance,NULL) ;
	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	SendDlgItemMessage(__hWnd,EDIT_COMMENT,EM_LIMITTEXT,MAX_COMMENT_LENGTH,0) ;

//	**********************
//	*** Filling Editor ***
//	**********************

	_hWnd = CreateWindowEx(0,"Button","-1",
			WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER,
			720,8,18,16,__hWnd,(HMENU) BTN_MOINS_UN,hInstance,NULL) ;
	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button","0",
			WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER,
			740,8,18,16,__hWnd,(HMENU) BTN_CLEAR,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button","1",
			WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER,
			760,8,18,16,__hWnd,(HMENU) BTN_SET,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button","X"
			,WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER,
			780,8,18,16,__hWnd,(HMENU) BTN_FILL,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Edit","-1234",
			WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL|ES_UPPERCASE|WS_BORDER,
			810,8,48,16,__hWnd,(HMENU) EDIT_VALUE,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(40),
			WS_CHILD|WS_VISIBLE|SS_CENTER,
			720,30,150,40,__hWnd,(HMENU) IDC_STATIC_12,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	SendDlgItemMessage(__hWnd,EDIT_VALUE,EM_LIMITTEXT,5,0) ;
	SetDlgItemInt(__hWnd,EDIT_VALUE,0,TRUE) ;

	EnforceSignedIntegerEdit(GetDlgItem(__hWnd,EDIT_VALUE)) ;

//	******************
//	*** Similarity ***
//	******************

	_hWnd = CreateWindowEx(0,"Static",Strings(18),
			WS_CHILD|WS_VISIBLE|SS_RIGHT,
			650,85,100,24,__hWnd,(HMENU) IDC_STATIC_14,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"EDIT","0.00%",
			WS_CHILD|WS_VISIBLE|ES_LEFT|ES_READONLY,
			750,85,100,24,_hWorkSpace,(HMENU) IDC_SIMILARITY,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

//	**********************
//	*** Set RNG Limits ***
//	**********************

	_hWnd = CreateWindowEx(0,"Static",Strings(10),
			WS_CHILD|WS_VISIBLE|SS_CENTER,
			550,8,150,16,__hWnd,(HMENU) IDC_STATIC_13,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"LISTBOX",szEmptyString,
			WS_CHILD|WS_VISIBLE|LBS_HASSTRINGS|LBS_NOTIFY|LBS_SORT|WS_VSCROLL,
			550,26,150,320,_hWorkSpace,(HMENU) LISTBOX_RNG_LIMITS,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_i = 0 ;

	while(lpszRanDomLimits[_i])
	{
		_lpszTmp = lpszRanDomLimits[_i] ;

		_iIndex = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_RNG_LIMITS,LB_ADDSTRING,0,(LPARAM) _lpszTmp) ;
		SendDlgItemMessage(_hWorkSpace,LISTBOX_RNG_LIMITS,LB_SETITEMDATA,_iIndex,(LPARAM) _i) ;

		_i++ ;
	}

	if(GetPrivateProfileInt(szProfile_09,szProfile_18,0,szProfileFileName) == 1)
		SendDlgItemMessage(__hWnd,LISTBOX_RNG_LIMITS,LB_SETCURSEL,0,0) ;

//	**********************************
//	*** Grey Input / Output / None ***
//	**********************************

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_LEFT|BS_VCENTER,
			700,160,10,14,__hWnd,(HMENU) RADIO_GREY_INPUT,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(32),
			WS_CHILD|WS_VISIBLE,
			715,160,140,14,__hWnd,(HMENU) IDC_STATIC_04,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_LEFT|BS_VCENTER,
			700,178,10,14,__hWnd,(HMENU) RADIO_GREY_OUTPUT,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(33),
			WS_CHILD|WS_VISIBLE,
			715,178,140,14,__hWnd,(HMENU) IDC_STATIC_05,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_LEFT|BS_VCENTER,
			700,196,10,14,__hWnd,(HMENU) RADIO_GREY_NONE,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(34),
			WS_CHILD|WS_VISIBLE,
			715,196,170,14,__hWnd,(HMENU) IDC_STATIC_06,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

//	**********************
//	*** Divisor & Bias ***
//	**********************

	_hWnd = CreateWindowEx(0,"Static",Statics(38),
			WS_CHILD|WS_VISIBLE|SS_CENTER,
			720,240,60,12,__hWnd,(HMENU) IDC_STATIC_10,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Edit","0",
			WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL|ES_UPPERCASE|WS_BORDER,
			790,240,40,14,__hWnd,(HMENU) EDIT_DIVISOR,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(39),
			WS_CHILD|WS_VISIBLE|SS_CENTER,
			720,260,60,12,__hWnd,(HMENU) IDC_STATIC_11,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Edit","0",
			WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL|ES_UPPERCASE|WS_BORDER,
			790,260,40,14,__hWnd,(HMENU) EDIT_BIAS,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	SendDlgItemMessage(__hWnd,EDIT_DIVISOR,EM_LIMITTEXT,5,0) ;
	SendDlgItemMessage(__hWnd,EDIT_BIAS,EM_LIMITTEXT,5,0) ;

	SetDlgItemInt(__hWnd,EDIT_DIVISOR,1,TRUE) ;
	SetDlgItemInt(__hWnd,EDIT_BIAS,0,TRUE) ;

	EnforceSignedIntegerEdit(GetDlgItem(__hWnd,EDIT_DIVISOR)) ;
	EnforceSignedIntegerEdit(GetDlgItem(__hWnd,EDIT_BIAS)) ;

//	*************
//	*** N & P ***
//	*************

	_hWnd = CreateWindowEx(0,"Static",Statics(35),
			WS_CHILD|WS_VISIBLE,
			720,300,32,12,__hWnd,(HMENU) IDC_STATIC_07,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(36),
			WS_CHILD|WS_VISIBLE,
			720,320,32,12,__hWnd,(HMENU) IDC_STATIC_08,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Static",Statics(37),
			WS_CHILD|WS_VISIBLE,
			720,340,32,12,__hWnd,(HMENU) IDC_STATIC_09,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Edit","0",
			WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL|ES_READONLY,
			770,300,48,12,__hWnd,(HMENU) EDIT_NEGATIF,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Edit","0",
			WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL|ES_READONLY,
			770,320,48,12,__hWnd,(HMENU) EDIT_POSITIF,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

	_hWnd = CreateWindowEx(0,"Edit","0",
			WS_CHILD|WS_VISIBLE|ES_CENTER|ES_AUTOHSCROLL|ES_READONLY,
			770,340,48,12,__hWnd,(HMENU) EDIT_NEGPOS,hInstance,NULL) ;

	SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

//	***************************
//	*** Images Informations ***
//	***************************

	_hWnd = CreateWindowEx(0,"Button",szEmptyString,
			WS_CHILD|WS_VISIBLE|BS_GROUPBOX,
			570,370,300,165,__hWnd,(HMENU) STATIC_GROUPBOX,hInstance,NULL) ;
	SetWindowTheme(_hWnd,L"explorer",NULL) ;

//	-----------------------------------------------------------------------------------------------------
	lpCurrentFilter = &CurrentFilter ;

	RtlSecureZeroMemory(lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	lpCurrentFilter->TotalColsNumber = 3 * 3 ;
	lpCurrentFilter->ColsNumber = 3 ;
	lpCurrentFilter->Divisor = 1 ;

	SetDlgItemInt(__hWnd,EDIT_DIVISOR,lpCurrentFilter->Divisor,TRUE) ;
	SetDlgItemInt(__hWnd,EDIT_BIAS,lpCurrentFilter->Bias,TRUE) ;

	CreateEditor(3,TRUE) ;
	ChangeFilterSize(3,TRUE) ;
//	----------------------------------------------------------------------------------------------------
	if(GetPrivateProfileInt(szProfile_09,szProfile_20,0,szProfileFileName) == 1)
	{
		if(GetPrivateProfileString(szProfile_09,szProfile_19,szEmptyString,_szFileName,sizeof(_szFileName),szProfileFileName))
		{
			_lpImageInfos = LoadImageFromFile(_szFileName,MAX_EDIT_COLUMNS,MAX_EDIT_ROWS) ;
			if(!_lpImageInfos)
				return (Error(6)) ;

			_lpImageInfos->StructureSize = sizeof(IMAGEINFOS) ;

			SetWindowLongPtr(hToolBox,GWLP_USERDATA,(long long int) _lpImageInfos) ;

			lstrcpy(_szFileName,_lpImageInfos->Image.szImageFile) ;
			PathStripPath(_szFileName) ;
			sqlite3_snprintf(sizeof(_szTmp),_szTmp,"Convolution Filters Editor: [%s]",_szFileName) ;
			SetWindowText(hToolBox,_szTmp) ;

			sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d x %d",_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight) ;
			SendMessage(hStatus,(UINT) SB_SETTEXT,(WPARAM) 2,(LPARAM) (LPSTR) _szTmp) ;

			PaintResultWindow() ;

			bFirstDrop = TRUE ;
		}
	}
//	----------------------------------------------------------------------------------------------------

	return (TRUE) ;
}

void Child2_SetRedOrGreen(void)
{
	HDC			_hDCMem1, _hDCMem2 ;
	HBITMAP		_hBitmap, _hBmpOld1 ;
	HSTMT		_hFind ;
	HWND		_hWorkSpace ;
	int			_iIndex, _iValue, _iModel ;
	char		_szTmp[1024] ;

	RECT		_rcClient ;

	if(hDCWorkSpace)
	{
		SetRect(&_rcClient,0,0,48,48) ;

		_hDCMem1 = CreateCompatibleDC(hDCWorkSpace) ;
		if(_hDCMem1)
		{
			_hDCMem2 = CreateCompatibleDC(hDCWorkSpace) ;
			if(_hDCMem2)
			{
				_hBitmap = CreateCompatibleBitmap(hDCWorkSpace,_rcClient.right,_rcClient.bottom) ;
				if(_hBitmap)
				{
					_hBmpOld1 = SelectObject(_hDCMem1,_hBitmap) ;
					if(_hBmpOld1)
					{
						FillRect(_hDCMem1,&_rcClient,hBarbieBrush) ;

						_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

						if(SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0))
						{
							_iIndex = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCURSEL,0,0) ;
							if(_iIndex != CB_ERR)
							{
								_iValue = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETITEMDATA,_iIndex,0) ;
								if(_iValue != CB_ERR)
								{
									sqlite3_snprintf(sizeof(_szTmp),_szTmp,"SELECT [Model] FROM Filters WHERE [FilterRecordNumber] == %lu",_iValue) ;
									Log(_szTmp) ;

									if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,NULL) != SQLITE_OK)
									{
										sqlite3_reset(_hFind) ;
										sqlite3_finalize(_hFind) ;

										Error(52) ;		// SQLite compile error
									}
									else
									{
										if(sqlite3_step(_hFind) == SQLITE_ROW)
										{
											_iModel = sqlite3_column_int(_hFind,0) ;

											if(_iModel == 1)
												DrawIcon(_hDCMem1,0,0,hIcon1) ;
											else
												DrawIcon(_hDCMem1,0,0,hIcon2) ;
										}

										sqlite3_reset(_hFind) ;
										sqlite3_finalize(_hFind) ;
									}
								}
							}
						}

						BitBlt(hDCWorkSpace,260,120,_rcClient.right,_rcClient.bottom,_hDCMem1,0,0,SRCCOPY) ;
						SelectObject(_hDCMem1,_hBmpOld1) ;
					}

					DeleteObject(_hBitmap) ;
				}

				DeleteDC(_hDCMem2) ;
			}

			DeleteDC(_hDCMem1) ;
		}
	}

	return ;
}

void Child2_PaintResultWindow(void)
{
	register		LPIMAGEINFOS	_lpImageInfos ;
					DWORD			_dwNewWidth, _dwNewHeight ;
					HWND			_hWnd ;
					HDC				_hDCMem_1, _hDCMem_2 ;
					HBITMAP			_hBmpCrop, _hBmpFinal ;
					HBITMAP			_hBitmap_1, _hBmpOld_1, _hBmpOld_2 ;
					int				_w ;
					RECT			_Rc ;
					BITMAP			_BmpInfo ;

	_hWnd = hTabsWindows[HWND_WORKSPACE] ;
	if(_hWnd)
	{
		_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
		if(_lpImageInfos)
		{
			_hBmpCrop = ImageToBitmap(_lpImageInfos,_lpImageInfos->lpResult) ;
			if(_hBmpCrop)
			{
				_hBmpFinal = CropImage(_hBmpCrop,MAX_EDIT_COLUMNS,MAX_EDIT_ROWS) ;
				if(_hBmpFinal)
				{
					DeleteObject(_hBmpCrop) ;

					GetClientRect(_hWnd,&_Rc) ;

					if(hDCWorkSpace)
					{
						_hDCMem_1 = CreateCompatibleDC(hDCWorkSpace) ;
						if(_hDCMem_1)
						{
							_hDCMem_2 = CreateCompatibleDC(hDCWorkSpace) ;
							if(_hDCMem_2)
							{
								GetObject(_hBmpFinal,sizeof(BITMAP),&_BmpInfo) ;

								_w = _Rc.right - 900 ;

								ImageResize(_BmpInfo.bmWidth,_BmpInfo.bmHeight,_w,_Rc.bottom,&_dwNewWidth,&_dwNewHeight) ;

								_hBmpOld_2 = SelectObject(_hDCMem_2,_hBmpFinal) ;
								if(_hBmpOld_2)
								{
									_hBitmap_1 = CreateCompatibleBitmap(hDCWorkSpace,_w,_Rc.bottom) ;
									if(_hBitmap_1)
									{
										_hBmpOld_1 = SelectObject(_hDCMem_1,_hBitmap_1) ;
										if(_hBmpOld_1)
										{
											SetStretchBltMode(_hDCMem_1,HALFTONE) ;

											StretchBlt(_hDCMem_1,(_w - _dwNewWidth) / 2,(_Rc.bottom - _dwNewHeight) / 2,_dwNewWidth,_dwNewHeight,_hDCMem_2,0,0,_BmpInfo.bmWidth,_BmpInfo.bmHeight,SRCCOPY) ;
											BitBlt(hDCWorkSpace,900,0,_w,_Rc.bottom,_hDCMem_1,0,0,SRCCOPY) ;

											SelectObject(_hDCMem_1,_hBmpOld_1) ;
										}

										DeleteObject(_hBitmap_1) ;
									}

									SelectObject(_hDCMem_2,_hBmpOld_2) ;
								}

								DeleteDC(_hDCMem_2) ;
							}

							DeleteDC(_hDCMem_1) ;
						}
					}

					DeleteObject(_hBmpFinal) ;
				}
			}
		}
	}

	return ;
}

void Child2_OnPaint(HWND __hWnd)
{
	HDC				_hDCMem1 ;
	HBITMAP			_hBitmap, _hBmpOld1 ;
	HWND			_hWorkSpace ;
	HFONT			_hFontOld ;
	HSTMT			_hFind ;
	LPIMAGEINFOS	_lpImageInfos ;
	int				_iIndex, _iValue, _iModel, _iLen, _iY ;
	char			_szTmp[1024], _szNumeric[1024] ;
	RECT			_rcClient, _Rc ;
	PAINTSTRUCT		_Ps ;
	SIZE			_Size ;

	BeginPaint(__hWnd,&_Ps) ;
	if(hDCWorkSpace)
	{
		_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

		GetClientRect(__hWnd,&_rcClient) ;

		_hDCMem1 = CreateCompatibleDC(hDCWorkSpace) ;
		if(_hDCMem1)
		{
			_hBitmap = CreateCompatibleBitmap(hDCWorkSpace,_rcClient.right,_rcClient.bottom) ;
			if(_hBitmap)
			{
				_hBmpOld1 = SelectObject(_hDCMem1,_hBitmap) ;
				if(_hBmpOld1)
				{
					FillRect(_hDCMem1,&_rcClient,hBarbieBrush) ;

					if(SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0))
					{
						_iIndex = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCURSEL,0,0) ;
						_iValue = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETITEMDATA,_iIndex,0) ;

						sqlite3_snprintf(sizeof(_szTmp),_szTmp,"SELECT [Model] FROM Filters WHERE [FilterRecordNumber] == %lu",_iValue) ;
						if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,NULL) != SQLITE_OK)
						{
							sqlite3_reset(_hFind) ;
							sqlite3_finalize(_hFind) ;

							Error(52) ;		// SQLite compile error
						}
						else
						{
							if(sqlite3_step(_hFind) == SQLITE_ROW)
							{
								_iModel = sqlite3_column_int(_hFind,0) ;
								if(_iModel == 1)
									DrawIcon(_hDCMem1,260,120,hIcon1) ;
								else
									DrawIcon(_hDCMem1,260,120,hIcon2) ;
							}

							sqlite3_reset(_hFind) ;
							sqlite3_finalize(_hFind) ;
						}

//	***************************************************
//	*** File Size, Image Size, Width, Height, Ratio ***
//	***************************************************

						_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
						if(_lpImageInfos)
						{
							_hFontOld = SelectObject(_hDCMem1,hDialogFont) ;
							SetBkMode(_hDCMem1,TRANSPARENT) ;
							SetTextColor(_hDCMem1,lpCurrentTheme->CtlColorStatic) ;

							_iY = 385 ;

							sqlite3_snprintf(sizeof(_szNumeric),_szTmp,"%lu",_lpImageInfos->BitmapInfo.bmiHeader.biWidth) ;
							GetNumberFormat(LOCALE_USER_DEFAULT,0,_szTmp,&NumFmt,_szNumeric,sizeof(_szNumeric)) ;
							sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%s%s %s",Statics(48),_szNumeric,Statics(53)) ;
							_iLen = lstrlen(_szTmp) ;
							GetTextExtentPoint32A(_hDCMem1,_szTmp,_iLen,&_Size) ;
							SetRect(&_Rc,575,_iY,575 + _Size.cx,_iY + _Size.cy) ;
							ExtTextOutA(_hDCMem1,575,_iY,ETO_CLIPPED|ETO_NUMERICSLOCAL|TA_LEFT|TA_NOUPDATECP,&_Rc,_szTmp,_iLen,NULL) ;
							_iY += _Size.cy ;

							sqlite3_snprintf(sizeof(_szNumeric),_szTmp,"%lu",_lpImageInfos->BitmapInfo.bmiHeader.biHeight) ;
							GetNumberFormat(LOCALE_USER_DEFAULT,0,_szTmp,&NumFmt,_szNumeric,sizeof(_szNumeric)) ;
							sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%s%s %s",Statics(49),_szNumeric,Statics(53)) ;
							_iLen = lstrlen(_szTmp) ;
							GetTextExtentPoint32A(_hDCMem1,_szTmp,_iLen,&_Size) ;
							SetRect(&_Rc,575,_iY,575 + _Size.cx,_iY + _Size.cy) ;
							ExtTextOutA(_hDCMem1,575,_iY,ETO_CLIPPED|ETO_NUMERICSLOCAL|TA_LEFT|TA_NOUPDATECP,&_Rc,_szTmp,_iLen,NULL) ;
							_iY += _Size.cy ;

							StrFormatByteSize(_lpImageInfos->BitmapInfo.bmiHeader.biSizeImage,_szNumeric,sizeof(_szNumeric)) ;
							sqlite3_snprintf(sizeof(_szNumeric),_szTmp,"%lu",_lpImageInfos->BitmapInfo.bmiHeader.biSizeImage) ;
							GetNumberFormat(LOCALE_USER_DEFAULT,0,_szTmp,&NumFmt,_szNumeric,sizeof(_szNumeric)) ;
							sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%s%s %s",Statics(50),_szNumeric,Statics(54)) ;
							_iLen = lstrlen(_szTmp) ;
							GetTextExtentPoint32A(_hDCMem1,_szTmp,_iLen,&_Size) ;
							SetRect(&_Rc,575,_iY,575 + _Size.cx,_iY + _Size.cy) ;
							ExtTextOutA(_hDCMem1,575,_iY,ETO_CLIPPED|ETO_NUMERICSLOCAL|TA_LEFT|TA_NOUPDATECP,&_Rc,_szTmp,_iLen,NULL) ;
							_iY += _Size.cy ;

							sqlite3_snprintf(sizeof(_szNumeric),_szTmp,"%lu",_lpImageInfos->BitmapInfo.bmiHeader.biPlanes) ;
							GetNumberFormat(LOCALE_USER_DEFAULT,0,_szTmp,&NumFmt,_szNumeric,sizeof(_szNumeric)) ;
							sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%s%s",Statics(51),_szNumeric) ;
							_iLen = lstrlen(_szTmp) ;
							GetTextExtentPoint32A(_hDCMem1,_szTmp,_iLen,&_Size) ;
							SetRect(&_Rc,575,_iY,575 + _Size.cx,_iY + _Size.cy) ;
							ExtTextOutA(_hDCMem1,575,_iY,ETO_CLIPPED|ETO_NUMERICSLOCAL|TA_LEFT|TA_NOUPDATECP,&_Rc,_szTmp,_iLen,NULL) ;
							_iY += _Size.cy ;

							sqlite3_snprintf(sizeof(_szNumeric),_szTmp,"%lu",_lpImageInfos->BitmapInfo.bmiHeader.biBitCount) ;
							GetNumberFormat(LOCALE_USER_DEFAULT,0,_szTmp,&NumFmt,_szNumeric,sizeof(_szNumeric)) ;
							sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%s%s %s",Statics(52),_szNumeric,Statics(55)) ;
							_iLen = lstrlen(_szTmp) ;
							GetTextExtentPoint32A(_hDCMem1,_szTmp,_iLen,&_Size) ;
							SetRect(&_Rc,575,_iY,575 + _Size.cx,_iY + _Size.cy) ;
							ExtTextOutA(_hDCMem1,575,_iY,ETO_CLIPPED|ETO_NUMERICSLOCAL|TA_LEFT|TA_NOUPDATECP,&_Rc,_szTmp,_iLen,NULL) ;

							SelectObject(_hDCMem1,_hFontOld) ;
						}

//	***********************
//	*** Draw the result ***
//	***********************

						BitBlt(hDCWorkSpace,0,0,_rcClient.right,_rcClient.bottom,_hDCMem1,0,0,SRCCOPY) ;
					}

					SelectObject(_hDCMem1,_hBmpOld1) ;
				}

				DeleteObject(_hBitmap) ;
			}

			DeleteDC(_hDCMem1) ;
		}

		EndPaint(__hWnd,&_Ps) ;

		if((_rcClient.right >= 900) && (_rcClient.bottom >= 500))
			Child2_PaintResultWindow() ;
	}

	return ;
}

void Child2_OnCommand(HWND __hWnd,WPARAM __wParam,LPARAM __lParam)
{
	register	int		_iIndex ;
	register	int		_iValue ;

				HWND	_hWorkSpace ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	if(HIWORD(__wParam) == BN_CLICKED)
	{
		bFirstDrop = FALSE ;

		if((LOWORD(__wParam) == RADIO_GREY_INPUT) || (LOWORD(__wParam) == RADIO_GREY_OUTPUT) || (LOWORD(__wParam) == RADIO_GREY_NONE))
		{
			lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
			lstrcpy(lpCurrentFilter->Keccak_Filter,"0000000000000000000000000000000000000000000000000000000000000000") ;
			lstrcpy(lpCurrentFilter->StarName,Strings(1)) ;

			lpCurrentFilter->Model = 0 ;
			lpCurrentFilter->Rate = 0 ;
			lpCurrentFilter->BlackAndWhite = 0 ;

			SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;
			SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;
		}

		switch(LOWORD(__wParam))
		{
			case	RADIO_GREY_INPUT :		Radio_01() ;								break ;
			case	RADIO_GREY_OUTPUT :		Radio_02() ;								break ;
			case	RADIO_GREY_NONE :		Radio_03() ;								break ;
			case	BTN_REQUEST :			RequestFilter() ;							break ;
			case	BTN_SET :				SetAllFields(1) ;							break ;
			case	BTN_CLEAR :				SetAllFields(0) ;							break ;
			case	BTN_MOINS_UN :			SetAllFields(-1) ;							break ;

			case	BTN_FILL :				_iValue = GetDlgItemInt(__hWnd,EDIT_VALUE,NULL,TRUE) ;
											SetAllFields(_iValue) ;
											break ;

			case	BTN_SELECT :			SetFiltersToListBox() ;						break ;
			case	BTN_UNSELECT :			SetAllTheFilters() ;						break ;

			case	TLB_BTN_ABOUT :			DialogBox(hInstance,MAKEINTRESOURCE(DLG_ABOUT),hToolBox,(DLGPROC) AboutDlgProc) ;
											break ;
		}

		return ;
	}

	switch(LOWORD(__wParam))
	{
		case	LISTBOX_MODELS :		if(HIWORD(__wParam) == LBN_SELCHANGE)
										{
											bFirstDrop = FALSE ;

											SendMessage(hStatus,(UINT) SB_SETTEXT,(WPARAM)(INT) 4,(LPARAM) (LPSTR) szEmptyString) ;
											ChangeFilter(lpCurrentFilter) ;
											Child2_SetRedOrGreen() ;
											SetFocus(GetDlgItem(__hWnd,LISTBOX_MODELS)) ;
											PushFilter() ;

											break ;
										}

										break ;

		case	COMBO_SIZES :			if(HIWORD(__wParam) == CBN_SELCHANGE)
										{
											bFirstDrop = FALSE ;

											_iIndex = (int) SendMessage((HWND) __lParam,CB_GETCURSEL,0,0) ;
											_iValue = (int) SendMessage((HWND) __lParam,CB_GETITEMDATA,_iIndex,0) ;

											ChangeFilterSize(_iValue,TRUE) ;
											SetFocus(GetDlgItem(__hWnd,LISTBOX_MODELS)) ;
											SetFiltersToListBox() ;
											PushFilter() ;

											break ;
										}

										break ;

		case	COMBO_FILTER_RATING :	if(HIWORD(__wParam) == CBN_SELCHANGE)
										{
											bFirstDrop = FALSE ;
											GetRating(__hWnd) ;
										}

										break ;

	}

	return ;
}

LRESULT CALLBACK Ch2_Workspace_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam)
{
	switch(__Message)
	{
		case	WM_PAINT :			Child2_OnPaint(__hWnd) ;
									break ;

		case	WM_CTLCOLORLISTBOX :
 		case	WM_CTLCOLORSTATIC :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorStatic) ;
									SetBkMode((HDC) __wParam,TRANSPARENT) ;
		case	WM_CTLCOLORDLG :	return ((LRESULT) hBarbieBrush) ;

		case	WM_CTLCOLORBTN :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorBtn) ;
									SetBkMode((HDC) __wParam,TRANSPARENT) ;
									return ((LRESULT) hBarbieBrush) ;

		case	WM_CTLCOLOREDIT :	SetBkMode((HDC) __wParam,TRANSPARENT) ;
		
									if(GetDlgCtrlID((HWND) __lParam) == EDIT_DIVISOR)
										SetTextColor((HDC) __wParam,0x0000FF) ;
									else
										SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorEdit) ;
									
									return ((LRESULT) hBarbieBrush) ;

		case	WM_COMMAND :		Child2_OnCommand(__hWnd,__wParam,__lParam) ;
									break ;

		case	WM_CREATE :			hTabsWindows[HWND_WORKSPACE] = __hWnd ;
									Child2_OnCreate(__hWnd) ;
									break ;

		default :					return (DefWindowProc(__hWnd,__Message,__wParam,__lParam)) ;
	}

	return (FALSE) ;
}
