#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <CommCtrl.h>
#include <uxtheme.h>

#include "zeegrid.h"
#include "sqlite3.h"

#include "CoFeE.h"
#include "Languages.h"
#include "Datas.h"
#include "Ressources.h"

#pragma intrinsic(strcmp, memcmp, memcpy memset)

/*
	***


	Three cases:
		1-Inserting before the first column.
		2-Inserting before the last column.
		3-Inserting anywhere.

		1-Inserting before the first column.
		------------------------------------

		---------------------
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		---------------------

		The column #5 is lost
		Columns #1 to #4 are moved to colums #2 to #5
		Column #1 is set to 0

		---------------------
		| 0 | 1 | 2 | 3 | 4 |
		| 0 | 1 | 2 | 3 | 4 |
		| 0 | 1 | 2 | 3 | 4 |
		| 0 | 1 | 2 | 3 | 4 |
		| 0 | 1 | 2 | 3 | 4 |
		---------------------

		2-Inserting before the last column.
		-----------------------------------

		---------------------
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		---------------------

		The column #5 is lost and set to 0.

		---------------------
		| 1 | 2 | 3 | 4 | 0 |
		| 1 | 2 | 3 | 4 | 0 |
		| 1 | 2 | 3 | 4 | 0 |
		| 1 | 2 | 3 | 4 | 0 |
		| 1 | 2 | 3 | 4 | 0 |
		---------------------

		3-Inserting anywhere.
		----------------------

		---------------------
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		---------------------

		Here we insert a column before the column #3

		---------------------
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		---------------------

		Column #5 is lost
		Column #4 is moved to column #5
		Column #3 is moved to column #4
		Column #3 is set to 0

		Conclusion
		----------

		In all cases we lost the column #5.
		When inserting a column we push the colum on the right.
*/

void Editor_InsertColumLeft(void)
{
	register	int		_i, _j ;
				int		_k, _iInteger, _iMatrix, _iMilieu, _iColumnClicked ;

	_iColumnClicked = 0 ;

	_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETCURSORCOL,0,0) ;
	if(_iColumnClicked == -1)
		_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDCOL,0,0) ;

	if(_iColumnClicked < 1)
		return ;

//		*********************************
//		*** Inserting a column before ***
//		*********************************

	_j = lpCurrentFilter->ColsNumber - 1 ;
	_k = _iColumnClicked - 1 ;

	while(_iColumnClicked < lpCurrentFilter->TotalColsNumber)
	{
		_i = _j ;

		while( _i > _k)
		{
			lpCurrentFilter->Matrix[_i] = lpCurrentFilter->Matrix[_i - 1] ;
			_i-- ;
		}

		lpCurrentFilter->Matrix[_iColumnClicked - 1] = 0 ;

		_iColumnClicked += lpCurrentFilter->ColsNumber ;
		_j += lpCurrentFilter->ColsNumber ;
		_k += lpCurrentFilter->ColsNumber ;
	}

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

/*
	Inserting a column on the right (after) the selected one looks like
	Inserting a column on the left (before) the selected column + 1

	I insert after the column #3 looks like inserting before the column #4
	It is not possible to insert after the last column.

	Suppose the following:

		---------------------
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		| 1 | 2 | 3 | 4 | 5 |
		---------------------

		I insert a column after the column #2:

		The column #5 is lost
		The column #4 is moved to #5
		The column #3 si moved to #4
		The column #3 is set to 0.

		---------------------
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		---------------------

---------------------------------------------------------------------------------------
Looks like:
		Here we insert a column BEFORE the column #3

		---------------------
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		| 1 | 2 | 0 | 3 | 4 |
		---------------------

		Column #5 is lost
		Column #4 is moved to column #5
		Column #3 is moved to column #4
		Column #3 is set to 0
---------------------------------------------------------------------------------------
		If I want to insert a column after the column #4 I directly move 0 to column #5

		---------------------
		| 1 | 2 | 3 | 4 | 0 |
		| 1 | 2 | 3 | 4 | 0 |
		| 1 | 2 | 3 | 4 | 0 |
		| 1 | 2 | 3 | 4 | 0 |
		| 1 | 2 | 3 | 4 | 0 |
		---------------------

*/

void Editor_InsertColumRight(void)
{
	register	int		_i, _j ;
				int		_k, _iInteger, _iMatrix, _iMilieu, _iColumnClicked ;

	_iColumnClicked = 0 ;

	_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETCURSORCOL,0,0) ;
	if(_iColumnClicked == -1)
		_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDCOL,0,0) ;

	if(_iColumnClicked < 1)
		return ;

	if(_iColumnClicked == lpCurrentFilter->ColsNumber)
		return ;

	_j = 0 ;

	while(_j < lpCurrentFilter->TotalColsNumber)
	{
		_k = lpCurrentFilter->ColsNumber - 1 ;

		for(_i = _iColumnClicked ; _i < lpCurrentFilter->ColsNumber ; _i++, _k--)
			lpCurrentFilter->Matrix[_j + _k] = lpCurrentFilter->Matrix[_j + (_k - 1)] ;

		lpCurrentFilter->Matrix[_j + _iColumnClicked] = 0 ;

		_j += lpCurrentFilter->ColsNumber ;
	}

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_InsertRowAbove(void)
{
	register	int		_i, _j ;
				int		_k, _f, _iInteger, _iMatrix, _iMilieu, _iRowClicked ;

	_iRowClicked = 0 ;

	_iRowClicked = (int) SendMessage(hGrid,ZGM_GETCURSORROW,0,0) ;
	if(_iRowClicked == -1)
		_iRowClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDROW,0,0) ;

	if(_iRowClicked < 1)
		return ;

	_f = _iRowClicked * lpCurrentFilter->ColsNumber ;

	_k = lpCurrentFilter->ColsNumber * (lpCurrentFilter->ColsNumber - 1) ;
	_i = _k - lpCurrentFilter->ColsNumber ;
	_j = lpCurrentFilter->ColsNumber * sizeof(int) ;

	while(_k > _f)
	{
		memcpy(&lpCurrentFilter->Matrix[_k],&lpCurrentFilter->Matrix[_i],_j) ;

		_k -= lpCurrentFilter->ColsNumber ;
		_i -= lpCurrentFilter->ColsNumber ;
	}

	memcpy(&lpCurrentFilter->Matrix[_k],&lpCurrentFilter->Matrix[_i],_j) ;
	memset(&lpCurrentFilter->Matrix[_i],0,lpCurrentFilter->ColsNumber * sizeof(int)) ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_InsertRowBelow(void)
{
	register	int		_i, _j ;
				int		_k, _iInteger, _iMatrix, _iMilieu, _iRowClicked ;

	_iRowClicked = 0 ;

	_iRowClicked = (int) SendMessage(hGrid,ZGM_GETCURSORROW,0,0) ;
	if(_iRowClicked == -1)
		_iRowClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDROW,0,0) ;

	if(_iRowClicked < 1)
		return ;

	if(_iRowClicked == lpCurrentFilter->ColsNumber)
		return ;

	_k = lpCurrentFilter->TotalColsNumber - lpCurrentFilter->ColsNumber ;
	_j = _k - lpCurrentFilter->ColsNumber ;
	_i = _iRowClicked * lpCurrentFilter->ColsNumber ;

	while(_k >= _i)
	{
		memcpy(&lpCurrentFilter->Matrix[_k],&lpCurrentFilter->Matrix[_j],sizeof(int) * lpCurrentFilter->ColsNumber) ;

		_j -= lpCurrentFilter->ColsNumber ;
		_k -= lpCurrentFilter->ColsNumber ;
	}

	memset(&lpCurrentFilter->Matrix[_i],0,sizeof(int) * lpCurrentFilter->ColsNumber) ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_DeleteColum(void)
{
	register	int		_i, _j ;
				int		_k, _f, _iInteger, _iMatrix, _iMilieu, _iColumnClicked ;

	_iColumnClicked = 0 ;

	_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETCURSORCOL,0,0) ;
	if(_iColumnClicked == -1)
		_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDCOL,0,0) ;

	if(_iColumnClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

/*
	Si on est sur la dernière colonne on la met à zéro, c'est tout.
	Les colonnes de droite subissent un décallage à gauche pour remplacer
	la colonne qui est à supprimer.
*/

	_k = _iColumnClicked - 1 ;
	_f = 0 ;

	for(_j = 0 ; _j < lpCurrentFilter->ColsNumber ; _j ++)
	{
		for(_i = _k ; _i < lpCurrentFilter->ColsNumber - 1 ; _i++)
			lpCurrentFilter->Matrix[_f + _i] = lpCurrentFilter->Matrix[_f + _i + 1] ;

		lpCurrentFilter->Matrix[_f + lpCurrentFilter->ColsNumber - 1] = 0 ;

		_f += lpCurrentFilter->ColsNumber ;
	}

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_DeleteRow(void)
{
	register	int		_i, _j ;
				int		_k, _f, _iInteger, _iMatrix, _iMilieu, _iRowClicked ;

	_iRowClicked = 0 ;

	_iRowClicked = (int) SendMessage(hGrid,ZGM_GETCURSORROW,0,0) ;
	if(_iRowClicked == -1)
		_iRowClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDROW,0,0) ;

	if(_iRowClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_k = (_iRowClicked - 1) * lpCurrentFilter->ColsNumber ;
	_f = _iRowClicked * lpCurrentFilter->ColsNumber ;
	_j = (lpCurrentFilter->TotalColsNumber - lpCurrentFilter->ColsNumber) ;

	memcpy(&lpCurrentFilter->Matrix[_k],&lpCurrentFilter->Matrix[_f],(_iRowClicked - 1) * lpCurrentFilter->ColsNumber * sizeof(int)) ;
	memset(&lpCurrentFilter->Matrix[_j],0,lpCurrentFilter->ColsNumber * sizeof(int)) ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_MirroirHorizontal(void)
{
	register	int		_i, _j ;
				int		_k, _f, _iInteger, _iMatrix, _iMilieu ;

//	***********************
//	*** Not common part ***
//	***********************

	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;
	_j = 0 ;

	for(_f = 0 ; _f < lpCurrentFilter->ColsNumber ; _f++)
	{
		for(_i = 0 ; _i < _iMilieu ; _i++)
		{
			_k = lpCurrentFilter->Matrix[_j + _i + _iMilieu + 1] ;
			lpCurrentFilter->Matrix[_j + _i + _iMilieu + 1] = lpCurrentFilter->Matrix[_j + _i] ;
			lpCurrentFilter->Matrix[_j + _i] = _k ;
		}

		_j += lpCurrentFilter->ColsNumber ;
	}

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_MirroirVertical(void)
{
	register	int		_i, _j ;
				int		_k, _f, _m, _iInteger, _iMatrix, _iMilieu ;

//	***********************
//	*** Not common part ***
//	***********************

	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;
	_j = lpCurrentFilter->TotalColsNumber - lpCurrentFilter->ColsNumber ;
	_m = 0 ;

	for(_f = 0 ; _f < _iMilieu ; _f++)
	{
		for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
		{
			_k = lpCurrentFilter->Matrix[_j + _i] ;
			lpCurrentFilter->Matrix[_j + _i] = lpCurrentFilter->Matrix[_m + _i] ;
			lpCurrentFilter->Matrix[_m + _i] = _k ;
		}

		_j -= lpCurrentFilter->ColsNumber ;
		_m += lpCurrentFilter->ColsNumber ;
	}

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_CircularRotateTop(void)
{
	register	int		_i, _j ;
				int		_k, _f, _m, _iInteger, _iMatrix, _iMilieu, _iColumnClicked ;

	_iColumnClicked = 0 ;

	_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETCURSORCOL,0,0) ;
	if(_iColumnClicked == -1)
		_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDCOL,0,0) ;

	if(_iColumnClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_j = _iColumnClicked - 1 ;						// First Row
	_f = lpCurrentFilter->Matrix[_j] ;				// Save the first value
	_k = _j + lpCurrentFilter->ColsNumber ;			// Second Row
	_m = _j + ((lpCurrentFilter->ColsNumber - 1) * lpCurrentFilter->ColsNumber) ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		lpCurrentFilter->Matrix[_j] = lpCurrentFilter->Matrix[_k] ;

		_j += lpCurrentFilter->ColsNumber ;
		_k += lpCurrentFilter->ColsNumber ;
	}

	lpCurrentFilter->Matrix[_m] = _f ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_CircularRotateBottom(void)
{
	register	int		_i, _j ;
				int		_k, _f, _m, _iInteger, _iMatrix, _iMilieu, _iColumnClicked ;

	_iColumnClicked = 0 ;

	_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETCURSORCOL,0,0) ;
	if(_iColumnClicked == -1)
		_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDCOL,0,0) ;

	if(_iColumnClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_j = _iColumnClicked - 1 ;						// First Row
	_m = _j + ((lpCurrentFilter->ColsNumber - 1) * lpCurrentFilter->ColsNumber) ;
	_f = lpCurrentFilter->Matrix[_m] ;				// Save the last value
	_k = _m - lpCurrentFilter->ColsNumber ;			// Last Row - 1

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber - 1 ; _i++)
	{
		lpCurrentFilter->Matrix[_m] = lpCurrentFilter->Matrix[_k] ;

		_k -= lpCurrentFilter->ColsNumber ;
		_m -= lpCurrentFilter->ColsNumber ;
	}

	lpCurrentFilter->Matrix[_j] = _f ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_CircularRotateLeft(void)
{
	register	int		_i, _j ;
				int		_k, _f, _m, _iInteger, _iMatrix, _iMilieu, _iRowClicked ;

	_iRowClicked = 0 ;

	_iRowClicked = (int) SendMessage(hGrid,ZGM_GETCURSORROW,0,0) ;
	if(_iRowClicked == -1)
		_iRowClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDROW,0,0) ;

	if(_iRowClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_f = (_iRowClicked * lpCurrentFilter->ColsNumber) - 1 ;
	_j = (_iRowClicked - 1) * lpCurrentFilter->ColsNumber ;
	_k = _j + lpCurrentFilter->ColsNumber - 1 ;
	_f = lpCurrentFilter->Matrix[_f] ;
	_m = _j ;
	_j = _k - 1 ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		lpCurrentFilter->Matrix[_k] = lpCurrentFilter->Matrix[_j] ;
		_k-- ;
		_j-- ;
	}

	lpCurrentFilter->Matrix[_m] = _f ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_CircularRotateRight(void)
{
	register	int		_i, _j ;
				int		_k, _f, _iInteger, _iMatrix, _iMilieu, _iRowClicked ;

	_iRowClicked = 0 ;

	_iRowClicked = (int) SendMessage(hGrid,ZGM_GETCURSORROW,0,0) ;
	if(_iRowClicked == -1)
		_iRowClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDROW,0,0) ;

	if(_iRowClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_j = (_iRowClicked - 1) * lpCurrentFilter->ColsNumber ;
	_k = _j + 1 ;
	_f = lpCurrentFilter->Matrix[_j] ;
	_i = (lpCurrentFilter->ColsNumber - 1) * sizeof(int) ;

	memcpy(&lpCurrentFilter->Matrix[_j],&lpCurrentFilter->Matrix[_k],_i) ;
	lpCurrentFilter->Matrix[_j + lpCurrentFilter->ColsNumber - 1] = _f ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_ShiftToTop(void)
{
	register	int		_i, _j ;
				int		_k, _m, _iInteger, _iMatrix, _iMilieu, _iColumnClicked ;

	_iColumnClicked = 0 ;

	_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETCURSORCOL,0,0) ;
	if(_iColumnClicked == -1)
		_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDCOL,0,0) ;

	if(_iColumnClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_j = _iColumnClicked - 1 ;						// First Row
	_k = _j + lpCurrentFilter->ColsNumber ;			// Second Row
	_m = _j + ((lpCurrentFilter->ColsNumber - 1) * lpCurrentFilter->ColsNumber) ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		lpCurrentFilter->Matrix[_j] = lpCurrentFilter->Matrix[_k] ;

		_j += lpCurrentFilter->ColsNumber ;
		_k += lpCurrentFilter->ColsNumber ;
	}

	lpCurrentFilter->Matrix[_m] = 0 ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_ShiftToBottom(void)
{
	register	int		_i, _j ;
				int		_k, _m, _iInteger, _iMatrix, _iMilieu, _iColumnClicked ;

	_iColumnClicked = 0 ;

	_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETCURSORCOL,0,0) ;
	if(_iColumnClicked == -1)
		_iColumnClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDCOL,0,0) ;

	if(_iColumnClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_j = _iColumnClicked - 1 ;						// First Row
	_m = _j + ((lpCurrentFilter->ColsNumber - 1) * lpCurrentFilter->ColsNumber) ;
	_k = _m - lpCurrentFilter->ColsNumber ;			// Last Row - 1

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber - 1 ; _i++)
	{
		lpCurrentFilter->Matrix[_m] = lpCurrentFilter->Matrix[_k] ;

		_k -= lpCurrentFilter->ColsNumber ;
		_m -= lpCurrentFilter->ColsNumber ;
	}

	lpCurrentFilter->Matrix[_j] = 0 ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_ShiftToLeft(void)
{
	register	int		_i, _j ;
				int		_k, _iInteger, _iMatrix, _iMilieu, _iRowClicked ;

	_iRowClicked = 0 ;

	_iRowClicked = (int) SendMessage(hGrid,ZGM_GETCURSORROW,0,0) ;
	if(_iRowClicked == -1)
		_iRowClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDROW,0,0) ;

	if(_iRowClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_j = (_iRowClicked - 1) * lpCurrentFilter->ColsNumber ;
	_k = _j + 1 ;
	_i = (lpCurrentFilter->ColsNumber - 1) * sizeof(int) ;

	memcpy(&lpCurrentFilter->Matrix[_j],&lpCurrentFilter->Matrix[_k],_i) ;
	lpCurrentFilter->Matrix[_j + lpCurrentFilter->ColsNumber - 1] = 0 ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_ShiftToRight(void)
{
	register	int		_i, _j ;
				int		_k, _m, _iInteger, _iMatrix, _iMilieu, _iRowClicked ;

	_iRowClicked = 0 ;

	_iRowClicked = (int) SendMessage(hGrid,ZGM_GETCURSORROW,0,0) ;
	if(_iRowClicked == -1)
		_iRowClicked = (int) SendMessage(hGrid,ZGM_GETSELECTEDROW,0,0) ;

	if(_iRowClicked < 1)
		return ;

//	***********************
//	*** Not common part ***
//	***********************

	_j = (_iRowClicked - 1) * lpCurrentFilter->ColsNumber ;
	_k = _j + lpCurrentFilter->ColsNumber - 1 ;
	_m = _j ;
	_j = _k - 1 ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		lpCurrentFilter->Matrix[_k] = lpCurrentFilter->Matrix[_j] ;

		_k-- ;
		_j-- ;
	}

	lpCurrentFilter->Matrix[_m] = 0 ;

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_RotateLeft(void)
{
	register	int		_i, _j ;
				int		_i1, _i2 ,_i3, _i4 ;
				int		_k, _m, _f, _n ;
				int		_iInteger, _iMatrix, _iMilieu ;
				int		Q0[(MAX_EDIT_ROWS / 2) * (MAX_EDIT_COLUMNS / 2)] ;

//	***********************
//	*** Not common part ***
//	***********************

	_f = lpCurrentFilter->ColsNumber / 2 ;

//	+----+-+-----+		 0  1  2	 4 5  6
//	| Q1 | |  Q2 |		 7  8  9	11 12 13
//	+----|-|-----+		14 15 16	18 19 20
//	|    |X|     |
//	+----|-|-----+		28 29 30	32 33 34
//	| Q3 | |  Q4 |		35 36 37	39 40 41
//	+----+-+-----+		42 43 44	46 47 48

//	 0  1  2  3  4  5  6
//	 7  8  9 10 11 12 13
//	14 15 16 17 18 19 20
//	21 22 23 24 25 26 27
//	28 29 30 31 32 33 34
//	35 36 37 38 39 40 41
//	42 43 44 45 46 47 48

//	Q1 =  0  1  2  7  8  9 14 15 16
//	Q2 =  4  5  6 11 12 13 18 19 20
//	Q3 = 28 29 30 35 36 37 42 43 44
//	Q4 = 32 33 34 39 40 41 46 47 48

	_i1 = 0 ;										// 0
	_i2 = _f + 1 ;									// (7 / 2) + 1 = 3 + 1 = 4
	_i3 = lpCurrentFilter->ColsNumber * (_f + 1) ;	// 7 * ((7 / 2) + 1) = 7 * (3 + 1) = 7 * 4 = 28
	_i4 = _i3 + _f + 1 ;							// 28 + 4 = 32

	_j = 0 ;
	_k = 0 ;

	_m = _f * sizeof(int) ;

	for(_i = 0 ; _i < _f ; _i++)
	{
		memcpy(&Q0[_j],&lpCurrentFilter->Matrix[_k],_m) ;

		_j += _f ;
		_k += lpCurrentFilter->ColsNumber ;
	}

	_j = 0 ;
	_n = 0 ;

	for(_i = 0 ; _i < _f ; _i++)
	{
//		***************
//		*** Q1 = Q2 ***
//		***************

		memcpy(&lpCurrentFilter->Matrix[_i1],&lpCurrentFilter->Matrix[_i2],_m) ;

//		***************
//		*** Q2 = Q4 ***
//		***************

		memcpy(&lpCurrentFilter->Matrix[_i2],&lpCurrentFilter->Matrix[_i4],_m) ;

//		***************
//		*** Q4 = Q3 ***
//		***************

		memcpy(&lpCurrentFilter->Matrix[_i4],&lpCurrentFilter->Matrix[_i3],_m) ;

//		***************
//		*** Q3 = Q0 ***
//		***************

		memcpy(&lpCurrentFilter->Matrix[_i3],&Q0[_n],_m) ;

//		*****************
//		*** Next Loop ***
//		*****************

		_i1 += lpCurrentFilter->ColsNumber ;
		_i2 += lpCurrentFilter->ColsNumber ;
		_i3 += lpCurrentFilter->ColsNumber ;
		_i4 += lpCurrentFilter->ColsNumber ;

		_n += _f ;
	}

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	SetEditorColors() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_RotateRight(void)
{
	register	int		_i, _j ;
				int		_i1, _i2 ,_i3, _i4 ;
				int		_k, _m, _f, _n ;
				int		_iInteger, _iMatrix, _iMilieu ;
				int		Q0[(MAX_EDIT_ROWS / 2) * (MAX_EDIT_COLUMNS / 2)] ;

//	***********************
//	*** Not common part ***
//	***********************

	_f = lpCurrentFilter->ColsNumber / 2 ;

//	+----+-+-----+		 0  1  2	 4 5  6
//	| Q1 | |  Q2 |		 7  8  9	11 12 13
//	+----|-|-----+		14 15 16	18 19 20
//	|    |X|     |
//	+----|-|-----+		28 29 30	32 33 34
//	| Q3 | |  Q4 |		35 36 37	39 40 41
//	+----+-+-----+		42 43 44	46 47 48

//	 0  1  2  3  4  5  6
//	 7  8  9 10 11 12 13
//	14 15 16 17 18 19 20
//	21 22 23 24 25 26 27
//	28 29 30 31 32 33 34
//	35 36 37 38 39 40 41
//	42 43 44 45 46 47 48

//	Q1 =  0  1  2  7  8  9 14 15 16
//	Q2 =  4  5  6 11 12 13 18 19 20
//	Q3 = 28 29 30 35 36 37 42 43 44
//	Q4 = 32 33 34 39 40 41 46 47 48

	_i1 = 0 ;										// 0
	_i2 = _f + 1 ;									// (7 / 2) + 1 = 3 + 1 = 4
	_i3 = lpCurrentFilter->ColsNumber * (_f + 1) ;	// 7 * ((7 / 2) + 1) = 7 * (3 + 1) = 7 * 4 = 28
	_i4 = _i3 + _f + 1 ;							// 28 + 4 = 32

	_j = 0 ;
	_k = _i3 ;

	_m = _f * sizeof(int) ;

	for(_i = 0 ; _i < _f ; _i++)
	{
		memcpy(&Q0[_j],&lpCurrentFilter->Matrix[_k],_m) ;

		_j += _f ;
		_k += lpCurrentFilter->ColsNumber ;
	}

	_j = 0 ;
	_n = 0 ;

	for(_i = 0 ; _i < _f ; _i++)
	{
//		***************
//		*** Q3 = Q4 ***
//		***************

		memcpy(&lpCurrentFilter->Matrix[_i3],&lpCurrentFilter->Matrix[_i4],_m) ;

//		***************
//		*** Q4 = Q2 ***
//		***************

		memcpy(&lpCurrentFilter->Matrix[_i4],&lpCurrentFilter->Matrix[_i2],_m) ;

//		***************
//		*** Q2 = Q1 ***
//		***************

		memcpy(&lpCurrentFilter->Matrix[_i2],&lpCurrentFilter->Matrix[_i1],_m) ;

//		***************
//		*** Q1 = Q0 ***
//		***************

		memcpy(&lpCurrentFilter->Matrix[_i1],&Q0[_n],_m) ;

//		*****************
//		*** Next Loop ***
//		*****************

		_i1 += lpCurrentFilter->ColsNumber ;
		_i2 += lpCurrentFilter->ColsNumber ;
		_i3 += lpCurrentFilter->ColsNumber ;
		_i4 += lpCurrentFilter->ColsNumber ;

		_n += _f ;
	}

//	*****************************************
//	*** Replace the datas into the editor ***
//	*****************************************

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

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

void Editor_WmSize(DWORD __dwNewWidth,DWORD __dwNewHeight)
{
	MoveWindow(hEditorToolBar,0,0,__dwNewWidth,38,TRUE) ;
	MoveWindow(hGrid,0,40,__dwNewWidth,__dwNewHeight - 40,TRUE) ;

	return ;
}

LRESULT Editor_WmCommand(int __Id)
{
	switch (__Id)
	{
		case	BTN_INSERT_COL_LEFT :		Editor_InsertColumLeft() ;		break ;
		case	BTN_INSERT_COL_RIGHT :		Editor_InsertColumRight() ;		break ;
		case	BTN_INSERT_ROW_ABOVE :		Editor_InsertRowAbove() ;		break ;
		case	BTN_INSERT_ROW_BELOW :		Editor_InsertRowBelow() ;		break ;
		case	BTN_DELETE_COL :			Editor_DeleteColum() ;			break ;
		case	BTN_DELETE_ROW :			Editor_DeleteRow() ;			break ;
		case	BTN_MIRROR_HORIZONTAL :		Editor_MirroirHorizontal() ;	break ;
		case	BTN_MIRROR_VERTICAL :		Editor_MirroirVertical() ;		break ;
		case	BTN_CIRULAR_ROTATE_TOP :	Editor_CircularRotateTop() ;	break ;
		case	BTN_CIRULAR_ROTATE_BOTTOM :	Editor_CircularRotateBottom() ;	break ;
		case	BTN_CIRULAR_ROTATE_LEFT :	Editor_CircularRotateLeft() ;	break ;
		case	BTN_CIRULAR_ROTATE_RIGHT :	Editor_CircularRotateRight() ;	break ;
		case	BTN_SHIFT_TOP :				Editor_ShiftToTop() ;			break ;
		case	BTN_SHIFT_BOTTOM :			Editor_ShiftToBottom() ;		break ;
		case	BTN_SHIFT_LEFT :			Editor_ShiftToLeft() ;			break ;
		case	BTN_SHIFT_RIGHT :			Editor_ShiftToRight() ;			break ;
		case	BTN_ROTATE_LEFT :			Editor_RotateLeft() ;			break ;
		case	BTN_ROTATE_RIGHT :			Editor_RotateRight() ;			break ;
	}

	return (0) ;
}

LRESULT CALLBACK Ch3_Editor_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam)
{
	LPNMHDR				_lpNmHdr ;
	LPTOOLTIPTEXT		_lpToolTip ;

	switch(__Message)
	{
		case	WM_SIZE :			Editor_WmSize(LOWORD(__lParam),HIWORD(__lParam)) ;
									break ;

		case	WM_COMMAND :		return (Editor_WmCommand(LOWORD(__wParam))) ;

		case	WM_CREATE :			hTabsWindows[HWND_EDITOR] = __hWnd ;

									hEditorToolBar = CreateEditorToolBar(__hWnd) ;
									SetWindowTheme(hEditorToolBar,L"explorer",NULL) ;

									LoadEditor() ;

									break ;

		case	WM_CTLCOLORDLG :	return ((LRESULT) hBarbieBrush) ;

		case	WM_CTLCOLORLISTBOX :
 		case	WM_CTLCOLORSTATIC :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorStatic) ;
									SetBkColor((HDC) __wParam,lpCurrentTheme->CtlColorDlg) ;
									return ((LRESULT) hBarbieBrush) ;

		case	WM_CTLCOLORBTN :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorBtn);
									SetBkColor((HDC) __wParam,lpCurrentTheme->CtlColorDlg);
									return ((LRESULT) hBarbieBrush) ;

		case	WM_CTLCOLOREDIT :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorEdit);
									SetBkColor((HDC) __wParam,lpCurrentTheme->CtlColorDlg);
									return ((LRESULT) hBarbieBrush) ;

		case	WM_NOTIFY :			_lpNmHdr = (LPNMHDR) __lParam ;
									_lpToolTip = (LPTOOLTIPTEXT) __lParam ;

									switch(_lpNmHdr->code)
									{
										case	TTN_GETDISPINFO :	switch(_lpToolTip->hdr.idFrom)
																	{
																		case	BTN_INSERT_COL_LEFT :		_lpToolTip->lpszText = ToolTips(20) ;	break ;
																		case	BTN_INSERT_COL_RIGHT :		_lpToolTip->lpszText = ToolTips(21) ;	break ;
																		case	BTN_INSERT_ROW_ABOVE :		_lpToolTip->lpszText = ToolTips(22) ;	break ;
																		case	BTN_INSERT_ROW_BELOW :		_lpToolTip->lpszText = ToolTips(23) ;	break ;
																		case	BTN_DELETE_COL :			_lpToolTip->lpszText = ToolTips(24) ;	break ;
																		case	BTN_DELETE_ROW :			_lpToolTip->lpszText = ToolTips(25) ;	break ;
																		case	BTN_MIRROR_HORIZONTAL :		_lpToolTip->lpszText = ToolTips(26) ;	break ;
																		case	BTN_MIRROR_VERTICAL :		_lpToolTip->lpszText = ToolTips(27) ;	break ;
																		case	BTN_CIRULAR_ROTATE_TOP :	_lpToolTip->lpszText = ToolTips(28) ;	break ;
																		case	BTN_CIRULAR_ROTATE_BOTTOM :	_lpToolTip->lpszText = ToolTips(29) ;	break ;
																		case	BTN_CIRULAR_ROTATE_LEFT :	_lpToolTip->lpszText = ToolTips(30) ;	break ;
																		case	BTN_CIRULAR_ROTATE_RIGHT :	_lpToolTip->lpszText = ToolTips(31) ;	break ;
																		case	BTN_SHIFT_TOP :				_lpToolTip->lpszText = ToolTips(32) ;	break ;
																		case	BTN_SHIFT_BOTTOM :			_lpToolTip->lpszText = ToolTips(33) ;	break ;
																		case	BTN_SHIFT_LEFT :			_lpToolTip->lpszText = ToolTips(34) ;	break ;
																		case	BTN_SHIFT_RIGHT :			_lpToolTip->lpszText = ToolTips(35) ;	break ;
																		case	BTN_ROTATE_LEFT :			_lpToolTip->lpszText = ToolTips(36) ;	break ;
																		case	BTN_ROTATE_RIGHT :			_lpToolTip->lpszText = ToolTips(37) ;	break ;
																	}
									}

									return (TRUE) ;

		default :					return (DefWindowProc(__hWnd,__Message,__wParam,__lParam)) ;
	}

	return (FALSE) ;
}
