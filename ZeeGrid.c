#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <CommCtrl.h>

#include "zeegrid.h"
#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Ressources.h"

void SetEditorColors(void)
{
	SendMessage(hGrid,ZGM_SETGRIDBGCOLOR,lpCurrentTheme->Zgm_ColBgColor,0) ;
	SendMessage(hGrid,ZGM_SETGRIDLINECOLOR,lpCurrentTheme->Zgm_ColFgColor,0) ;

	SendMessage(hGrid,ZGM_SETCOLBCOLOR,0,lpCurrentTheme->Zgm_ColBgColor) ;
	SendMessage(hGrid,ZGM_SETCOLFCOLOR,0,lpCurrentTheme->Zgm_ColFgColor) ;

	SendMessage(hGrid,ZGM_SETCELLBCOLOR,0,lpCurrentTheme->Zgm_CellBgColor) ;
	SendMessage(hGrid,ZGM_SETCELLFCOLOR,0,lpCurrentTheme->Zgm_CellFgColor) ;

	SendMessage(hGrid,ZGM_SETROWBCOLOR,0,lpCurrentTheme->Zgm_RowBgcolor) ;
	SendMessage(hGrid,ZGM_SETROWFCOLOR,0,lpCurrentTheme->Zgm_RowFgColor) ;

	return ;
}

void CreateEditor(int __iNumberOfColonnes, int __iResetFilter)
{
	register	int		_i, _j ;
				int		_k, _iIndex, _iInteger, _iMatrix, _iMilieu ;
				char	_szTmp[256] ;

	lpCurrentFilter = &CurrentFilter ;

	if(__iResetFilter)
		RtlSecureZeroMemory(lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	lpCurrentFilter->TotalColsNumber = __iNumberOfColonnes * __iNumberOfColonnes ;

	lpCurrentFilter->ColsNumber = __iNumberOfColonnes ;
	lpCurrentFilter->Divisor = 1 ;
	lpCurrentFilter->Grey = GREY_NONE ;

	CheckDlgButton(hTabsWindows[HWND_WORKSPACE],RADIO_GREY_INPUT,BST_UNCHECKED) ;
	CheckDlgButton(hTabsWindows[HWND_WORKSPACE],RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
	CheckDlgButton(hTabsWindows[HWND_WORKSPACE],RADIO_GREY_NONE,BST_CHECKED) ;

	SendDlgItemMessage(hTabsWindows[HWND_WORKSPACE],COMBO_SIZES,CB_SETCURSEL,(__iNumberOfColonnes - 3) / 2,0) ;

	SendMessage(hGrid,ZGM_DIMGRID,lpCurrentFilter->ColsNumber,0) ;
	SendMessage(hGrid,ZGM_SHOWGRIDLINES,TRUE,0) ;
	SendMessage(hGrid,ZGM_EMPTYGRID,TRUE,0) ;

	SetEditorColors() ;

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		_iIndex = (int) SendMessage(hGrid,ZGM_APPENDROW,0,0) ;
		SendMessage(hGrid,ZGM_SETROWTYPE,_iIndex,3) ;

		sqlite3_snprintf(sizeof(_szTmp),_szTmp,"C%2.2d",_i + 1) ;
		SendMessage(hGrid,ZGM_SETCELLTEXT,_i + 1,(LPARAM) _szTmp) ;

		SendMessage(hGrid,ZGM_SETCOLEDIT,_i + 1,1) ;
		SendMessage(hGrid,ZGM_SETCOLFORMAT,_i + 1,0) ;
	}

	SendMessage(hGrid,ZGM_ENABLETOOLBARTOGGLE,FALSE,0) ;
	SendMessage(hGrid,ZGM_ENABLESORT,FALSE,0) ;
	SendMessage(hGrid,ZGM_ENABLECOLMOVE,FALSE,0) ;
	SendMessage(hGrid,ZGM_AUTOHSCROLL,TRUE,0) ;
	SendMessage(hGrid,ZGM_AUTOVSCROLL,TRUE,0) ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iInteger = 0 ;
	_iMatrix = 0 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		for(_j = 0 ; _j < lpCurrentFilter->ColsNumber ; _j++)
		{
			SendMessage(hGrid,ZGM_SETCELLEDIT,_k,1) ;			//	1 = Editable with an edit control
			SendMessage(hGrid,ZGM_SETCELLFORMAT,_k,0) ;			//	Numeric data that has a format of 0 will display its data in a normal numeric fashion.
			SendMessage(hGrid,ZGM_SETCELLINT,_k,(LPARAM) &lpCurrentFilter->Matrix[_iMatrix]) ;
			SendMessage(hGrid,ZGM_SETCELLTYPE,_k,3) ;			//	3 = Numeric
			SendMessage(hGrid,ZGM_SETCELLRESTRICTION,_k,1) ;	//	1 = Signed Integer

			if(_i == _iMilieu)
				SendMessage(hGrid,ZGM_SETCELLBCOLOR,_k,2) ;

			if(_j == _iMilieu)
				SendMessage(hGrid,ZGM_SETCELLBCOLOR,_k,2) ;

			if((_i == _iMilieu) && (_j == _iMilieu))
			{
				SendMessage(hGrid,ZGM_SETCELLBCOLOR,_k,4) ;
				SendMessage(hGrid,ZGM_SETCELLFCOLOR,_k,7) ;
			}

			_iMatrix ++ ;
			_k++ ;
		}
	}

	SendMessage(hGrid,ZGM_AUTOSIZE_ALL_COLUMNS,0,0) ;

	return ;
}

void LoadEditor(void)
{
	register	int		_i, _j ;
				int		_k, _iIndex, _iInteger, _iMatrix, _iMilieu ;
				char	_szTmp[256] ;

	hGrid = CreateWindow("ZeeGrid","Filter Editor",WS_CHILD|WS_VISIBLE|WS_BORDER,0,0,0,0,hTabsWindows[HWND_EDITOR],(HMENU) IDC_ZEEGRID,hInstance,NULL) ;

	SendMessage(hGrid,ZGM_DIMGRID,lpCurrentFilter->ColsNumber,0) ;
	SendMessage(hGrid,ZGM_SHOWROWNUMBERS,TRUE,0) ;
	SendMessage(hGrid,ZGM_SHOWGRIDLINES,TRUE,0) ;

	SetEditorColors() ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		_k = _i + 1 ;

		_iIndex = (int) SendMessage(hGrid,ZGM_APPENDROW,0,0) ;
		SendMessage(hGrid,ZGM_SETROWTYPE,_iIndex,3) ;

		sqlite3_snprintf(sizeof(_szTmp),_szTmp,"C%2.2d",_i + 1) ;
		SendMessage(hGrid,ZGM_SETCELLTEXT,_k,(LPARAM) _szTmp) ;

		SendMessage(hGrid,ZGM_SETCOLEDIT,_k,1) ;
		SendMessage(hGrid,ZGM_SETCOLFORMAT,_k,0) ;
	}

	SendMessage(hGrid,ZGM_AUTOSIZE_ALL_COLUMNS,0,0) ;
	SendMessage(hGrid,ZGM_ENABLETOOLBARTOGGLE,FALSE,0) ;
	SendMessage(hGrid,ZGM_ENABLESORT,FALSE,0) ;
	SendMessage(hGrid,ZGM_ENABLECOLMOVE,FALSE,0) ;
	SendMessage(hGrid,ZGM_AUTOHSCROLL,TRUE,0) ;
	SendMessage(hGrid,ZGM_AUTOVSCROLL,TRUE,0) ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iInteger = 0 ;
	_iMatrix = 0 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		for(_j = 0 ; _j < lpCurrentFilter->ColsNumber ; _j++)
		{
			SendMessage(hGrid,ZGM_SETCELLEDIT,_k,1) ;			//	1 = Editable with an edit control
			SendMessage(hGrid,ZGM_SETCELLFORMAT,_k,0) ;			//	Numeric data that has a format of 0 will display its data in a normal numeric fashion.
			SendMessage(hGrid,ZGM_SETCELLINT,_k,(LPARAM) &lpCurrentFilter->Matrix[_iMatrix]) ;
			SendMessage(hGrid,ZGM_SETCELLTYPE,_k,3) ;			//	3 = Numeric
			SendMessage(hGrid,ZGM_SETCELLRESTRICTION,_k,1) ;	//	1 = Signed Integer

			if(_i == _iMilieu)
				SendMessage(hGrid,ZGM_SETCELLBCOLOR,_k,2) ;

			if(_j == _iMilieu)
				SendMessage(hGrid,ZGM_SETCELLBCOLOR,_k,2) ;

			if((_i == _iMilieu) && (_j == _iMilieu))
			{
				SendMessage(hGrid,ZGM_SETCELLBCOLOR,_k,4) ;
				SendMessage(hGrid,ZGM_SETCELLFCOLOR,_k,7) ;
			}

			_iMatrix ++ ;
			_k++ ;
		}
	}

	return ;
}
