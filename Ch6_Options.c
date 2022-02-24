#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <CommCtrl.h>
#include <shlwapi.h>
#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Languages.h"
#include "Ressources.h"

int SetYesNoCombo(HWND __hCombo,int __iValue)
{
	register	int			_iIndex ;

				COMBO_YESNO	_YesAndNo[5] ;

	_YesAndNo[0].Index = 0 ;	_YesAndNo[0].lpszName = Strings(7) ;	// "No"
	_YesAndNo[1].Index = 1 ;	_YesAndNo[1].lpszName = Strings(6) ;	// "Yes"

	_iIndex = (int) SendMessage(__hCombo,CB_ADDSTRING,0,(LPARAM) _YesAndNo[0].lpszName) ;
	SendMessage(__hCombo,CB_SETITEMDATA,_iIndex,(LPARAM) _YesAndNo[0].Index) ;

	_iIndex = (int) SendMessage(__hCombo,CB_ADDSTRING,0,(LPARAM) _YesAndNo[1].lpszName) ;
	SendMessage(__hCombo,CB_SETITEMDATA,_iIndex,(LPARAM) _YesAndNo[1].Index) ;

	if(__iValue == _YesAndNo[0].Index)
		_iIndex = 0 ;
	else
		_iIndex = 1 ;

	return ((int) SendMessage(__hCombo,CB_SETCURSEL,_iIndex,0)) ;
}

LRESULT CALLBACK Ch6_Options_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam)
{
	register	int			_iIndex ;
	register	size_t		_i ;
				int			_iValue ;
				HWND		_hWnd ;
				LPSTR		_lpszTmp ;
				char		_szRelativePath[MAX_PATH] ;
				char		_szLanguage[256] ;
				char		_szTmp[1024] ;

	switch(__Message)
	{
		case	WM_CTLCOLORLISTBOX :
 		case	WM_CTLCOLORSTATIC :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorStatic) ;
									SetBkColor((HDC) __wParam,lpCurrentTheme->CtlColorDlg) ;
		case	WM_CTLCOLORDLG :	return ((LRESULT) hBarbieBrush) ;

		case	WM_CTLCOLORBTN :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorBtn);
									SetBkColor((HDC) __wParam,lpCurrentTheme->CtlColorDlg);
									return ((LRESULT) hBarbieBrush) ;

		case	WM_CTLCOLOREDIT :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorEdit);
									SetBkColor((HDC) __wParam,lpCurrentTheme->CtlColorDlg);
									return ((LRESULT) hBarbieBrush) ;

		case	WM_CREATE :			hTabsWindows[HWND_OPTIONS] = __hWnd ;

									_hWnd = CreateWindowEx(0,"Static",Statics(45),
									WS_CHILD|WS_VISIBLE|SS_CENTER,
									4,12,150,16,__hWnd,(HMENU) STATIC_DIAPORAMA,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"Static",Statics(46),
									WS_CHILD|WS_VISIBLE|SS_CENTER,
									4,56,150,40,__hWnd,(HMENU) STATIC_SHOW_FILTERS,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"ComboBox",szEmptyString,
									WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWNLIST|CBS_SORT|CBS_HASSTRINGS|WS_VSCROLL,
									4,30,150,116,__hWnd,(HMENU) COMBO_DIAPORAMA,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;

//									***************************
//									*** Preselect RNG Value ***
//									***************************

									_hWnd = CreateWindowEx(0,"Static",Strings(10),
									WS_CHILD|WS_VISIBLE|SS_CENTER,
									200,4,150,24,__hWnd,(HMENU) STATIC_PRESELECT_RNG,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"ComboBox",szEmptyString,
									WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWNLIST|CBS_SORT|CBS_HASSTRINGS|WS_VSCROLL,
									200,30,150,116,__hWnd,(HMENU) COMBO_PRESELECT_RNG,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;

									_iValue = GetPrivateProfileInt(szProfile_09,szProfile_18,0,szProfileFileName) ;
									SetYesNoCombo(_hWnd,_iValue) ;

//									***********************************
//									*** Reload Last File at Startup ***
//									***********************************

									_hWnd = CreateWindowEx(0,"Static",Strings(8),
									WS_CHILD|WS_VISIBLE|SS_CENTER,
									370,4,150,24,__hWnd,(HMENU) STATIC_RELOAD_LAST_FILE,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"ComboBox",szEmptyString,
									WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWNLIST|CBS_SORT|CBS_HASSTRINGS|WS_VSCROLL,
									370,30,150,116,__hWnd,(HMENU) COMBO_RELOADLASTFILE,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;

									_iValue = GetPrivateProfileInt(szProfile_09,szProfile_20,0,szProfileFileName) ;
									SetYesNoCombo(_hWnd,_iValue) ;

//									********************************
//									**** Reorganize the database ***
//									********************************

									_hWnd = CreateWindowEx(0,"Static",Strings(9),
									WS_CHILD|WS_VISIBLE|SS_CENTER,
									540,4,150,24,__hWnd,(HMENU) STATIC_VACUUM,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"ComboBox",szEmptyString,
									WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWNLIST|CBS_SORT|CBS_HASSTRINGS|WS_VSCROLL,
									540,30,150,116,__hWnd,(HMENU) COMBO_VACUUM,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;

									_iValue = GetPrivateProfileInt(szProfile_09,"Vacuum",0,szProfileFileName) ;
									SetYesNoCombo(_hWnd,_iValue) ;

//									***********************
//									*** Backup Database ***
//									***********************

									_hWnd = CreateWindowEx(0,"Static",Strings(3),
									WS_CHILD|WS_VISIBLE|SS_CENTER,
									700,4,150,24,__hWnd,(HMENU) STATIC_EXPORT_DATABASE,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"ComboBox",szEmptyString,
									WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWNLIST|CBS_SORT|CBS_HASSTRINGS|WS_VSCROLL,
									700,30,150,116,__hWnd,(HMENU) COMBO_BACKUP,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;

									_iValue = GetPrivateProfileInt(szProfile_09,szProfile_23,0,szProfileFileName) ;
									SetYesNoCombo(_hWnd,_iValue) ;

//									************************
//									*** Export Directory ***
//									************************

									_hWnd = CreateWindowEx(0,"Static",Strings(11),
									WS_CHILD|WS_VISIBLE|SS_CENTER,
									200,60,200,12,__hWnd,(HMENU) STATIC_EXPORT_DIRECTORY,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"EDIT",szExportDirectory,
									WS_CHILD|WS_VISIBLE|WS_BORDER|ES_AUTOHSCROLL|ES_LEFT,
									200,76,400,24,__hWnd,(HMENU) EDIT_EXPORT_DIRECTORY,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"BUTTON","...",
									WS_CHILD|WS_VISIBLE|BS_CENTER|BS_NOTIFY|BS_PUSHBUTTON|BS_TEXT|BS_VCENTER,
									605,76,24,24,__hWnd,(HMENU) BTN_EXPORT_DIRECTORY,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;

//									*********************************
//									*** Export Database when Exit ***
//									*********************************

									_hWnd = CreateWindowEx(0,"Static",Strings(17),
									WS_CHILD|WS_VISIBLE|SS_CENTER,
									700,50,150,48,__hWnd,(HMENU) STATIC_EXPORT_DATABASE,hInstance,NULL) ;

									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"ComboBox",szEmptyString,
									WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWNLIST|CBS_SORT|CBS_HASSTRINGS|WS_VSCROLL,
									700,76,150,116,__hWnd,(HMENU) COMBO_EXPORT_DB,hInstance,NULL) ;

									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hMainFont,TRUE) ;

									_iValue = GetPrivateProfileInt(szProfile_09,szProfile_22,0,szProfileFileName) ;
									SetYesNoCombo(_hWnd,_iValue) ;

//									*****************
//									*** Diaporama ***
//									*****************

									_hWnd = GetDlgItem(__hWnd,COMBO_DIAPORAMA) ;

									ComboDiaporama[0].lpszName = Statics(1) ;
									ComboDiaporama[1].lpszName = Statics(2) ;
									ComboDiaporama[2].lpszName = Statics(3) ;
									ComboDiaporama[3].lpszName = Statics(4) ;
									ComboDiaporama[4].lpszName = Statics(5) ;
									ComboDiaporama[5].lpszName = Statics(6) ;
									ComboDiaporama[6].lpszName = Statics(7) ;
									ComboDiaporama[7].lpszName = Statics(8) ;
									ComboDiaporama[8].lpszName = Statics(9) ;

									_iValue = GetPrivateProfileInt(szProfile_09,"Diaporama",0,szProfileFileName) ;
									_i = 0 ;

									while(ComboDiaporama[_i].Index)
									{
										
										_iIndex = (int) SendMessage(_hWnd,CB_ADDSTRING,0,(LPARAM) ComboDiaporama[_i].lpszName) ;
										SendMessage(_hWnd,CB_SETITEMDATA,_iIndex,ComboDiaporama[_i].Index) ;

										if(_iValue == ComboDiaporama[_i].Index)
											SendMessage(_hWnd,CB_SETCURSEL,_i,0) ;

										_i++ ;
									}

									if(SendMessage(_hWnd,CB_GETCURSEL,0,0) == CB_ERR)
										SendMessage(_hWnd,CB_SETCURSEL,0,0) ;

									GetPrivateProfileString(szProfile_01,"Language",szLanguage_01,_szLanguage,MAX_PATH,szProfileFileName) ;

									_hWnd = CreateWindowEx(0,"Static",Statics(47),WS_CHILD|WS_VISIBLE|SS_CENTER,4,128,150,24,__hWnd,(HMENU) IDC_STATIC_03,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									_hWnd = CreateWindowEx(0,"ComboBox",szEmptyString,WS_CHILD|WS_VISIBLE|WS_BORDER|CBS_DROPDOWNLIST|CBS_HASSTRINGS|WS_VSCROLL,4,144,150,116,__hWnd,(HMENU) COMBO_LANGUAGES,hInstance,NULL) ;
									SendMessage(_hWnd,WM_SETFONT,(WPARAM) hDialogFont,TRUE) ;

									for(_i = 0 ; _i < 5 ;_i++)
									{
										_iIndex = (int) SendMessage(_hWnd,CB_ADDSTRING,0,(LPARAM) lpszLanguages[_i]) ;
										SendMessage(_hWnd,CB_SETITEMDATA,_iIndex,_i) ;
									}

									_iIndex = 0 ;

									     if(lstrcmp(_szLanguage,lpszLanguages[0]) == 0)	_iIndex = 0 ;
									else if(lstrcmp(_szLanguage,lpszLanguages[1]) == 0)	_iIndex = 1 ;
									else if(lstrcmp(_szLanguage,lpszLanguages[2]) == 0)	_iIndex = 2 ;
									else if(lstrcmp(_szLanguage,lpszLanguages[3]) == 0)	_iIndex = 3 ;
									else if(lstrcmp(_szLanguage,lpszLanguages[4]) == 0)	_iIndex = 4 ;

									SendMessage(_hWnd,CB_SETCURSEL,_iIndex,0) ;

									break ;

		case	WM_COMMAND :		switch(LOWORD(__wParam))
									{
										case	COMBO_LANGUAGES :
										{
											if(HIWORD(__wParam) == CBN_SELCHANGE)
											{
												_hWnd = GetDlgItem(__hWnd,COMBO_LANGUAGES) ;

												_iIndex = (int) SendMessage(_hWnd,CB_GETCURSEL,0,0) ;
												_iIndex = (int) SendMessage(_hWnd,CB_GETITEMDATA,_iIndex,0) ;

												WritePrivateProfileString(szProfile_01,"Language",lpszLanguages[_iIndex],szProfileFileName) ;
												WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
											}
										}
										
										break ;
										
										case	COMBO_DIAPORAMA :
										{
											if(HIWORD(__wParam) == CBN_SELCHANGE)
											{
												_hWnd = GetDlgItem(__hWnd,COMBO_DIAPORAMA) ;

												_iIndex = (int) SendMessage(_hWnd,CB_GETCURSEL,0,0) ;
												_iValue = (int) SendMessage(_hWnd,CB_GETITEMDATA,_iIndex,0) ;

												sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
												WritePrivateProfileString(szProfile_09,"Diaporama",_szTmp,szProfileFileName) ;
												WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
											}
											
											break ;
										}
										
										case	COMBO_PRESELECT_RNG :
										{
											if(HIWORD(__wParam) == CBN_SELCHANGE)
											{
												_hWnd = GetDlgItem(__hWnd,COMBO_PRESELECT_RNG) ;

												_iIndex = (int) SendMessage(_hWnd,CB_GETCURSEL,0,0) ;
												_iValue = (int) SendMessage(_hWnd,CB_GETITEMDATA,_iIndex,0) ;

												sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_iValue) ;

												WritePrivateProfileString(szProfile_09,szProfile_18,_szTmp,szProfileFileName) ;
												WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
											}
											
											break ;
										}

										case	COMBO_RELOADLASTFILE :
										{
											if(HIWORD(__wParam) == CBN_SELCHANGE)
											{
												_hWnd = GetDlgItem(__hWnd,COMBO_RELOADLASTFILE) ;

												_iIndex = (int) SendMessage(_hWnd,CB_GETCURSEL,0,0) ;
												_iValue = (int) SendMessage(_hWnd,CB_GETITEMDATA,_iIndex,0) ;

												sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_iValue) ;

												WritePrivateProfileString(szProfile_09,szProfile_20,_szTmp,szProfileFileName) ;

												if(_iValue == 0)
													WritePrivateProfileString(szProfile_09,szProfile_19,szEmptyString,szProfileFileName) ;

												WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
											}
											
											break ;
										}

										case	COMBO_VACUUM :
										{
											if(HIWORD(__wParam) == CBN_SELCHANGE)
											{
												_hWnd = GetDlgItem(__hWnd,COMBO_VACUUM) ;

												_iIndex = (int) SendMessage(_hWnd,CB_GETCURSEL,0,0) ;
												_iValue = (int) SendMessage(_hWnd,CB_GETITEMDATA,_iIndex,0) ;

												sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_iValue) ;

												WritePrivateProfileString(szProfile_09,"Vacuum",_szTmp,szProfileFileName) ;
												WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
											}
											
											break ;
										}

										case	COMBO_BACKUP :
										{
											if(HIWORD(__wParam) == CBN_SELCHANGE)
											{
												_hWnd = GetDlgItem(__hWnd,COMBO_BACKUP) ;

												_iIndex = (int) SendMessage(_hWnd,CB_GETCURSEL,0,0) ;
												_iValue = (int) SendMessage(_hWnd,CB_GETITEMDATA,_iIndex,0) ;

												sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_iValue) ;

												WritePrivateProfileString(szProfile_09,szProfile_23,_szTmp,szProfileFileName) ;
												WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
											}
											
											break ;
										}

										case	COMBO_EXPORT_DB :
										{
											if(HIWORD(__wParam) == CBN_SELCHANGE)
											{
												_hWnd = GetDlgItem(__hWnd,COMBO_EXPORT_DB) ;

												_iIndex = (int) SendMessage(_hWnd,CB_GETCURSEL,0,0) ;
												_iValue = (int) SendMessage(_hWnd,CB_GETITEMDATA,_iIndex,0) ;

												sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu",_iValue) ;

												WritePrivateProfileString(szProfile_09,szProfile_22,_szTmp,szProfileFileName) ;
												WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
											}
											
											break ;
										}

										case	BTN_EXPORT_DIRECTORY :
										{
												if(HIWORD(__wParam) == BN_CLICKED)
												{
													if(BrowseForFolder(__hWnd,_szTmp,Strings(11)))
													{
														_lpszTmp = _szTmp ;

														while(*_lpszTmp == ' ')
														{
															if(*_lpszTmp == '\0')
																break ;

															_lpszTmp++ ;
														}

														if(lstrlen(_lpszTmp))
														{
															PathRelativePathTo(_szRelativePath,szProgramFolder,FILE_ATTRIBUTE_DIRECTORY,_szTmp,FILE_ATTRIBUTE_DIRECTORY) ;

															if(_szRelativePath[0] == '\0')
																SetDlgItemText(__hWnd,EDIT_EXPORT_DIRECTORY,_lpszTmp) ;
															else
																SetDlgItemText(__hWnd,EDIT_EXPORT_DIRECTORY,_szRelativePath) ;

															GetDlgItemText(__hWnd,EDIT_EXPORT_DIRECTORY,szExportDirectory,MAX_PATH) ;

															WritePrivateProfileString(szProfile_09,szProfile_24,szExportDirectory,szProfileFileName) ;
															WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
														}
														else
														{
															Error(48) ;
														}
													}
												}

												break ;
										}
									}

									break ;


		default :					return (DefWindowProc(__hWnd,__Message,__wParam,__lParam)) ;
	}

	return (FALSE) ;
}
