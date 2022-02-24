#include <windows.h>
#include <CommCtrl.h>
#include <shlwapi.h>
#include <intrin.h>
#include <uxtheme.h>
#include <Shlobj.h>

//#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma warn(disable:2006 2145 2215 2228 2805)

#include "zeegrid.h"
#include "sqlite3.h"

#include "sph_keccak.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Languages.h"
#include "Ressources.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#pragma lib "comdlg32.lib"
#pragma lib "shlwapi.lib"
#pragma lib "libgfl.lib"
#pragma lib "libgfle.lib"
#pragma lib "sqlite3.lib"
#pragma lib "UxTheme.lib"

//	2436927.3125 = 24/12/1959 à 19:30:00

//	1.0.0.9416-2459603.57362

char	szSerialNumber[]	=	"2FCF8E083DDAD7E9F2105094231410240D9C6B31" ;

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPBYTE PhR_Alloc_Memory(DWORD __dwSize)
{
	return ((LPBYTE) malloc(__dwSize + 256)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int PhR_Free_Memory(LPVOID __lpcbBuffer)
{
	free(__lpcbBuffer) ;

	return (TRUE) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR GetAsteroidName(LPSTR __lpszName,DWORD __dwRecordNumber)
{
	HSTMT			_hFind ;
	char			_szTmp[1024] ;

	if(!__lpszName)
		return (NULL) ;

	Log("Entering GetAsteroidName") ;

	*__lpszName = '\0' ;

//	**********************************************************
//	*** Search the name corresponding to the record number ***
//	**********************************************************

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"SELECT [StarName] FROM Asteroids WHERE [StarRecordNumber] == %lu;",__dwRecordNumber) ;

	Log(_szTmp) ;

	if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,NULL) != SQLITE_OK)
	{
//		*************************
//		*** Compilation error ***
//		*************************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		Log("Error 52 in GetAsteroidName") ;

		Error(52) ;

		return (NULL) ;
	}

//	**************************************************
//	*** Request the datas for the name and surname ***
//	**************************************************

	if(sqlite3_step(_hFind) != SQLITE_ROW)
	{
//		*********************
//		*** No data found ***
//		*********************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		Log("No asteroids name found, Error 18.") ;

		*__lpszName = '\0' ;		// Indicate an empty string

		Error(18) ;

		return (NULL) ;
	}

//	*************************
//	*** The data is found ***
//	*************************

	Log("Asteroid name found") ;
	Log(__lpszName) ;

	lstrcpy(__lpszName,(LPSTR) sqlite3_column_text(_hFind,0)) ;

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

//	************************************
//	*** Return a pointer to the name ***
//	************************************

	return (__lpszName) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR GetAndUpdateAsteroidName(int __iFilterRecordNumber,LPSTR __lpszKeccack,LPSTR __lpszName,LPDWORD __lpdwRecordNumber)
{
	HSTMT			_hFind ;
	DWORD			_dwRecordNumber ;
	int				_iResult ;
	char			_szBuffer[1024] ;

	if(!__lpszKeccack)		return (NULL) ;
	if(!__lpszName)			return (NULL) ;
	if(!__lpdwRecordNumber)	return (NULL) ;

	Log("Entering GetAndUpdateAsteroidName") ;

	*__lpszName = '\0' ;
	*__lpdwRecordNumber = 0 ;

	if(sqlite3_prepare_v2(hCfeDb,"SELECT [StarRecordNumber],[StarName] FROM Asteroids WHERE [FreeStar] == 1;",-1,&_hFind,NULL) != SQLITE_OK)
	{
		Log("Error 52 in GetAndUpdateAsteroidName") ;

//		*************************
//		*** Compilation Error ***
//		*************************

		sqlite3_reset(_hFind) ;

		Error(52) ;

		return (NULL) ;
	}

	Log("SELECT [StarRecordNumber],[StarName] FROM Asteroids WHERE [FreeStar] == 1;") ;

	_iResult = sqlite3_step(_hFind) ;

	if(_iResult == SQLITE_DONE)
	{
		Log("All the asteroids names are used") ;

//		******************************
//		*** All the names are used ***
//		******************************

		Error(28) ;

		return(NULL) ;
	}

	if(_iResult != SQLITE_ROW)
	{
		Log("No asteroids name available. Error 18") ;

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

//		****************************************************
//		*** No name corresponding to record number found ***
//		****************************************************

		Error(18) ;

		return(NULL) ;
	}

	_dwRecordNumber = sqlite3_column_int(_hFind,0) ;
	lstrcpy(__lpszName,(LPSTR) sqlite3_column_text(_hFind,1)) ;

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

//	---> UPDATE OR IGNORE [Asteroids] SET Keccak=\"%w\", Freedom=0 WHERE [RecordNumber] = %lu;

	sqlite3_snprintf(sizeof(_szBuffer),_szBuffer,"UPDATE OR IGNORE [Asteroids] SET FilterRecordNumber=%d,Keccak_Star=\"%w\", FreeStar=0 WHERE [StarRecordNumber] = %lu;",__iFilterRecordNumber,__lpszKeccack,_dwRecordNumber) ;

	Log("Updating Asteroid table") ;
	Log(_szBuffer) ;

	if(sqlite3_exec(hCfeDb,_szBuffer,0,0,NULL) != SQLITE_OK)
	{
//		*******************************
//		*** Cannot update the table ***
//		*******************************

		*__lpdwRecordNumber = 0 ;
		*__lpszName = '\0' ;

		Log("Cannot update the Asteroids table. CFE displays the SQLite error message.") ;

		SQLite_Error() ;

		sqlite3_reset(_hFind) ;

		return (NULL) ;
	}

//	**********************************
//	*** The table has been updated ***
//	**********************************

	Log("The Asteroids table has been updated successfuly") ;

	*__lpdwRecordNumber = _dwRecordNumber ;

	return (__lpszName) ;
}

int StoreNamesAndSurnames(LPNOM_PRENOMS __lpNomPrenoms)
{
	register		DWORD	_i ;
	char			_szBuffer[1024] ;

	Log("Entering StoreNamesAndSurnames") ;

	sqlite3_snprintf(sizeof(_szBuffer),_szBuffer,"INSERT OR REPLACE INTO [Asteroids] VALUES(%lu,%lu,%lu,\"%w\",\"%w\");\n",
					__lpNomPrenoms->StarRecordNumber,
					__lpNomPrenoms->FilterRecordNumber,
					__lpNomPrenoms->Freedom,
					__lpNomPrenoms->Keccak,
					__lpNomPrenoms->StarName) ;

	Log(_szBuffer) ;

	while(sqlite3_exec(hCfeDb,"BEGIN TRANSACTION",0,0,NULL) == SQLITE_BUSY) ;

	_i = sqlite3_exec(hCfeDb,_szBuffer,0,0,NULL) ;
	if(_i != SQLITE_OK)
	{
		sqlite3_exec(hCfeDb,"END TRANSACTION",0,0,NULL) ;

		if(_i == SQLITE_CONSTRAINT)
		{
			Log("SQLITE_CONSTRAINT, CFE will retry") ;

			return (-1) ;
		}

		return (FALSE) ;
	}

	sqlite3_exec(hCfeDb,"END TRANSACTION",0,0,NULL) ;

	return (TRUE) ;
}

int OpenCfeDataBase(void)
{
	register	size_t	_i ;
	register	size_t	_k ;

	int			_iRandom_Nom, _iRandom_Prenom, _iError ;
	HSTMT		_hFind ;

	char		_szResult[1024] ;

	NOM_PRENOMS	_NamesAndSurnames ;

	Log("Entering OpenCfeDataBase") ;

	hCfeDb = NULL ;

	if(PathFileExists(szDatabaseFileName))
	{
		Log("The database exists") ;

//		********************************************
//		*** The file exists but cannot be opened ***
//		********************************************

		if(sqlite3_open_v2(szDatabaseFileName,&hCfeDb,SQLITE_OPEN_READWRITE,NULL) != SQLITE_OK)
		{
			Log("The database cannot be opened. Error 51") ;
			Log(szDatabaseFileName) ;

			hCfeDb = NULL ;
			return (Error(51)) ;
		}

//		********************************
//		*** The file has been opened ***
//		********************************

		SQLitePragmas() ;

		Log("Verifying the filters table") ;

//	********************************************
//	*** Verify if the table "Filters" exists ***
//	********************************************

		if(sqlite3_prepare_v2(hCfeDb,"SELECT [name] FROM sqlite_master WHERE [name] LIKE \"Filters\"",-1,&_hFind,NULL) != SQLITE_OK)
		{
			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;
			Log("Error 52") ;

			return (Error(52)) ;
		}

		if(sqlite3_step(_hFind) != SQLITE_ROW)
		{
			Log("The filters table does not exist. Error 53") ;

			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			return (Error(53)) ;
		}

//		***********************************
//		**** The table "Filters" exists ***
//		***********************************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		_hFind = NULL ;

		Log("The filters table exists") ;

//		**********************************
//		*** Is the table empty or full ***
//		**********************************

//		---> Full means at least one record

		if(sqlite3_prepare_v2(hCfeDb,"SELECT COUNT([FilterRecordNumber]) FROM [Filters];",-1,&_hFind,NULL) != SQLITE_OK)
		{
			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("Error 52 when verifying if the filters table is empty or not") ;

			return (Error(52)) ;
		}

		if(sqlite3_step(_hFind) != SQLITE_ROW)		// La table est vide
		{
//			************************************
//			*** The table "Filters" is empty ***
//			************************************

			Log("The filters table is empty. Error 55") ;

			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			return (Error(55)) ;
		}

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		Log("All is good with the filters table") ;

//	************************************************
//	*** Verify if the table "Convolution" exists ***
//	************************************************

		Log("Verifying the Convolution table") ;

		if(sqlite3_prepare_v2(hCfeDb,"SELECT [name] FROM sqlite_master WHERE [name] LIKE \"Convolution\"",-1,&_hFind,NULL) != SQLITE_OK)
		{
			Log("Error 52 when verifying if the Convolution table exists") ;

			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			return (Error(52)) ;
		}

		if(sqlite3_step(_hFind) != SQLITE_ROW)
		{
			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("The Convolution table does not exist. Error 54") ;

			return (Error(54)) ;
		}

//		***************************************
//		**** The table "Convolution" exists ***
//		***************************************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		_hFind = NULL ;

		Log("The Convolution table exists") ;

//		**********************************
//		*** Is the table empty or full ***
//		**********************************

//		---> Full means at least one record

		if(sqlite3_prepare_v2(hCfeDb,"SELECT COUNT([FilterRecordNumber]) FROM [Convolution];",-1,&_hFind,NULL) != SQLITE_OK)
		{
			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("Error 52 when verifying if the Convolution table is empty or not") ;

			return (Error(52)) ;
		}

		if(sqlite3_step(_hFind) != SQLITE_ROW)		// La table est vide
		{
//			****************************************
//			*** The table "Convolution" is empty ***
//			****************************************

			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("The Convolution table is empty. Error 56") ;

			return (Error(56)) ;
		}

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		Log("All is good with the Convolution table") ;

//	**********************************************
//	*** Verify if the table "Asteroids" exists ***
//	**********************************************

		Log("Verifying the Asteroid table") ;

		if(sqlite3_prepare_v2(hCfeDb,"SELECT [name] FROM sqlite_master WHERE [name] LIKE \"Asteroids\"",-1,&_hFind,NULL) != SQLITE_OK)
		{
			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("Error 52 when verifying if the Asteroid table exists or not") ;

			return (Error(52)) ;
		}

		if(sqlite3_step(_hFind) != SQLITE_ROW)
		{
			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("The Asteroid table does not exist. Error 58") ;

			return (Error(58)) ;
		}

//		*************************************
//		**** The table "Asteroids" exists ***
//		*************************************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		_hFind = NULL ;

		Log("The Asteroid table exists") ;

//		**********************************
//		*** Is the table empty or full ***
//		**********************************

//		---> Full means at least one record

		if(sqlite3_prepare_v2(hCfeDb,"SELECT COUNT([StarRecordNumber]) FROM [Asteroids];",-1,&_hFind,NULL) != SQLITE_OK)
		{
			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("Error 52 when verifying if the Asteroid table is empty or not") ;

			return (Error(52)) ;
		}

		if(sqlite3_step(_hFind) != SQLITE_ROW)		// La table est vide
		{
//			**************************************
//			*** The table "Asteroids" is empty ***
//			**************************************

			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("The Asteroid table is empty. Error 59") ;

			return (Error(59)) ;
		}

//	********************************************
//	*** If there are less that 5 000 records ***
//	*** Consider the table is empty          ***
//	********************************************

		if(sqlite3_column_int(_hFind,0) < (MAX_ASTEROIDS - 100))
		{
			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			sqlite3_close_v2(hCfeDb) ;

			Log("Not enough records in the Asteroid table. Error 59") ;

			return (Error(59)) ;
		}

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;
	}
	else
	{
//		*******************************
//		*** The file does not exist ***
//		*******************************

		Log("CoFeE will create the database") ;

		if(sqlite3_open_v2(szDatabaseFileName,&hCfeDb,SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE,NULL) != SQLITE_OK)
		{
//			***************************************
//			*** Cannot create the database file ***
//			***************************************

			hCfeDb = NULL ;

			Log("Cannot create the database. Error 57") ;
			Log(szDatabaseFileName) ;

			return (Error(57)) ;
		}

//		************************************
//		*** The database file is created ***
//		************************************

		SQLitePragmas() ;

		sqlite3_exec(hCfeDb,szNewCreateFilters,0,0,NULL) ;
		sqlite3_exec(hCfeDb,szNewCreateDatas,0,0,NULL) ;
		sqlite3_exec(hCfeDb,szCreateAsteroids,0,0,NULL) ;

		sqlite3_exec(hCfeDb,"CREATE UNIQUE INDEX [PremierIndex] ON [Asteroids]([StarName] COLLATE [BINARY] ASC);",0,0,NULL) ;

		SQLitePragmas() ;

//		*****************************************
//		*** Fill the table Names and Surnames ***
//		*****************************************

//		---> I suppose there will not be any error

		_i = 0 ;
		_k = 0 ;

		RtlSecureZeroMemory(&_NamesAndSurnames,sizeof(NOM_PRENOMS)) ;

		while(_i < (MAX_ASTEROIDS + 100))
		{
			_k++ ;

			_NamesAndSurnames.Freedom = 1 ;
			_NamesAndSurnames.StarRecordNumber = _k ;

			_iRandom_Nom = (int)((long long int) rand() % 1628) ;
			_iRandom_Prenom = (int)((long long int) rand() % 1783) ;

			sqlite3_snprintf(MAX_STAR_NAME_LENGTH,_NamesAndSurnames.StarName,"%w %w",Prenoms[_iRandom_Prenom].Name,NomsDeFamille[_iRandom_Nom].Name) ;

			Keccak((unsigned char *) _NamesAndSurnames.StarName,MAX_STAR_NAME_LENGTH,(unsigned char *) _szResult) ;
			Hex2Str((unsigned char *) _szResult,(unsigned char *) _NamesAndSurnames.Keccak,256 / 8) ;

			_iError = StoreNamesAndSurnames(&_NamesAndSurnames) ;
			if(_iError == -1)				// SQLITE_CONSTRAINT (19)
			{
//				************************
//				*** An error occured ***
//				************************

//					---> Generaly it is a contraint violation
//					---> The names and surnames have already been stored

				_i-- ;
				_k-- ;
			}
			else
			{
				if(_iError == FALSE)
				{
					sqlite3_close_v2(hCfeDb) ;
					hCfeDb = NULL ;

					Log("Cannot store new records into the datas into filters and Convolution tables. Error 19") ;

					return (Error(19)) ;
				}
			}

			_i++ ;
		}

		InitializeTheTables() ;

		Log("Vacuum started") ;
		sqlite3_exec(hCfeDb,"VACUUM",0,0,NULL) ;
		Log("Vacuum finished") ;
	}

//	*********************************
//	*** All is done and all is OK ***
//	*********************************

	Log("All is ok, the program can continue") ;

//	************************************************************
//	*** The database is not closed and he handle is not NULL ***
//	************************************************************

	return (TRUE) ;
}

int DisplayError(int __iErrorNumber)
{
	if(hCfeDb)
	{
		Log("Closing database") ;

		sqlite3_close(hCfeDb) ;
		hCfeDb = NULL ;
	}

	Log("Removing GFL SDK from memory") ;

	gflLibraryExit() ;			// Unload Gfl

	Log("Removing SQLite from memory") ;

	sqlite3_shutdown() ;		// Unload SQLite

	Log("Removing ZeeGrid from memory") ;

	FreeLibrary(hZeeGrid) ;		// Unload ZeeGrid

	Log("Unloading language file from memory") ;

	UnLoadLanguageFile() ;		// Unload Language

	return (Error(__iErrorNumber)) ;
}

void VerifyCfeIni(void)
{
	char	_szTmp[1024] ;
	int		_iValue ;
	HANDLE	_hFile ;

	Log("Entering VerifyCfeIni") ;
	Log("Flushing ini cache") ;

	WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;

	if(!PathFileExists(szProfileFileName))
	{
		Log("The CoFeE.ini file does not exist") ;

		_hFile = CreateFile(szProfileFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL) ;
		if(_hFile == INVALID_HANDLE_VALUE)
		{
			Log("Cannot create CoFeE.ini file") ;
			Log("CoFeE.ini could be opened in an other program") ;
			Log("CoFeE will force the system to record the values") ;

			WritePrivateProfileString(szProfile_01,"Language",szLanguage_01,szProfileFileName) ;

			WritePrivateProfileString(szProfile_09,szProfile_10,"0",szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_11,"0",szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_12,"0",szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_13,"0",szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_18,"0",szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_19,szEmptyString,szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_20,"0",szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_22,"0",szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_23,"0",szProfileFileName) ;
			WritePrivateProfileString(szProfile_09,szProfile_24,szEmptyString,szProfileFileName) ;

			WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
		}
		else
		{
			Log("CoFeE.ini has been created") ;

			Log("Writing into CoFeE.ini") ;
			if(!FileWrite(_hFile,szIniBuffer,lstrlen(szIniBuffer)))
			{
				Log("Cannot write into CoFeE.ini") ;
				Log("CoFeE.ini could be opened in an other program") ;
				Log("CoFeE will force the system to record the values") ;

				WritePrivateProfileString(szProfile_01,"Language",szLanguage_01,szProfileFileName) ;

				WritePrivateProfileString(szProfile_09,szProfile_10,"0",szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_11,"0",szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_12,"0",szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_13,"0",szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_18,"0",szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_19,szEmptyString,szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_20,"0",szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_22,"0",szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_23,"0",szProfileFileName) ;
				WritePrivateProfileString(szProfile_09,szProfile_24,szEmptyString,szProfileFileName) ;

				WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;
			}
			else
			{
				Log("Datas are written into CoFeE.ini") ;
			}

			Log("Flushing CoFeE.ini to the disk") ;
			FileFlush(_hFile) ;

			Log("Closing CoFeE.ini") ;
			CloseHandle(_hFile) ;
		}

		Log("CoFeE.ini created") ;
	}
	else
	{
		Log("CoFeE.ini already exists") ;
	}

	Log("Verifying CoFeE.ini") ;

	GetPrivateProfileString(szProfile_01,"Language",szLanguage_01,_szTmp,sizeof(_szTmp),szProfileFileName) ;
	WritePrivateProfileString(szProfile_01,"Language",_szTmp,szProfileFileName) ;

	_iValue = GetPrivateProfileInt(szProfile_09,szProfile_10,0,szProfileFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
	WritePrivateProfileString(szProfile_09,szProfile_10,_szTmp,szProfileFileName) ;

	_iValue = GetPrivateProfileInt(szProfile_09,szProfile_11,0,szProfileFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
	WritePrivateProfileString(szProfile_09,szProfile_11,_szTmp,szProfileFileName) ;

	_iValue = GetPrivateProfileInt(szProfile_09,szProfile_12,0,szProfileFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
	WritePrivateProfileString(szProfile_09,szProfile_12,_szTmp,szProfileFileName) ;

	_iValue = GetPrivateProfileInt(szProfile_09,szProfile_13,0,szProfileFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
	WritePrivateProfileString(szProfile_09,szProfile_13,_szTmp,szProfileFileName) ;

	_iValue = GetPrivateProfileInt(szProfile_09,szProfile_18,0,szProfileFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
	WritePrivateProfileString(szProfile_09,szProfile_18,_szTmp,szProfileFileName) ;

	GetPrivateProfileString(szProfile_09,szProfile_19,szEmptyString,_szTmp,sizeof(_szTmp),szProfileFileName) ;
	WritePrivateProfileString(szProfile_09,szProfile_19,_szTmp,szProfileFileName) ;

	_iValue = GetPrivateProfileInt(szProfile_09,szProfile_20,0,szProfileFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
	WritePrivateProfileString(szProfile_09,szProfile_20,_szTmp,szProfileFileName) ;

	_iValue = GetPrivateProfileInt(szProfile_09,szProfile_22,0,szProfileFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
	WritePrivateProfileString(szProfile_09,szProfile_22,_szTmp,szProfileFileName) ;

	_iValue = GetPrivateProfileInt(szProfile_09,szProfile_23,0,szProfileFileName) ;
	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%d",_iValue) ;
	WritePrivateProfileString(szProfile_09,szProfile_23,_szTmp,szProfileFileName) ;

	GetPrivateProfileString(szProfile_09,szProfile_24,szEmptyString,szExportDirectory,MAX_PATH,szProfileFileName) ;
	WritePrivateProfileString(szProfile_09,szProfile_24,szExportDirectory,szProfileFileName) ;

	WritePrivateProfileString(NULL,NULL,NULL,szProfileFileName) ;

	Log("Exiting VerifyCfeIni") ;

	return ;
}

int PASCAL WinMain(HINSTANCE __hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdLine,int nCmdShow)
{
	register		LPWNDCLASSEX			_lpWc ;
					LPMSG					_lpMsg ;
					HWND					_hWnd ;
	register		LPSTR					_lpszString ;
					int						_x, _y, _w, _h ;
					int						_iError ;
					SYSTEMTIME				_St ;
					char					_szTmp[4096] ;
					char					_szRelativePath[MAX_PATH] ;
					INITCOMMONCONTROLSEX	_Icc ;

//	*******************************
//	*** Unreferenced parameters ***
//	*******************************

	hPrevInstance = hPrevInstance ;
	lpszCmdLine = lpszCmdLine ;
	nCmdShow = nCmdShow ;

	Log_SetFileName(".\\CoFeE.log") ;
	Log_Init() ;
	Log(szSerialNumber) ;

	_Icc.dwSize = sizeof(INITCOMMONCONTROLSEX) ;
	_Icc.dwICC = ICC_TREEVIEW_CLASSES|ICC_STANDARD_CLASSES|ICC_PROGRESS_CLASS|ICC_BAR_CLASSES|ICC_TAB_CLASSES|ICC_WIN95_CLASSES ;

	InitCommonControlsEx(&_Icc) ;

//	*****************
//	*** Real code ***
//	*****************

	hInstance = __hInstance ;

	SetThreadErrorMode(SEM_FAILCRITICALERRORS,NULL) ;
	GetSystemInfo(&InfosSystem) ;

//	**************************************
//	*** Don't allow multiple instances ***
//	**************************************

	if(FindWindow("COFEE",NULL))
		return (Error(7)) ;

//	****************************************************
//	*** Initialize the seed for the random generator ***
//	****************************************************

	GetLocalTime(&_St) ;
	srand((unsigned int) _rdtsc() * _St.wMilliseconds) ;

//	*************************************
//	*** Initialize the libraries used ***
//	*************************************

	if(sqlite3_initialize() != SQLITE_OK)
		return(Error(8)) ;

	Log("SQLite correctly initialized") ;

	gflSetPluginsPathname(szProgramFolder) ;

	if(gflLibraryInit() != GFL_NO_ERROR)
		return(Error(9)) ;

	Log("GFL SDK correctly initialized") ;

	gflEnableLZW(GFL_TRUE) ;

//	**********************************************************************************
//	*** Get the program name and the folder name from which has been launched from ***
//	**********************************************************************************

	GetModuleFileName(hInstance,szProgramPathAndName,MAX_PATH) ;

	lstrcpy(szProgramName,PathFindFileName(lstrcpy(_szTmp,szProgramPathAndName))) ;
	*(PathFindFileName(_szTmp)) = '\0' ;

	lstrcpy(szProgramFolder,_szTmp) ;
	PathRemoveBackslash(szProgramFolder) ;

	GetModuleFileName(hInstance,szProgramPathAndName,MAX_PATH) ;

//	*******************************************
//	*** Create the name of the profile file ***
//	*******************************************

	sqlite3_snprintf(sizeof(szProfileFileName),szProfileFileName,"%w\\CoFeE.ini",szProgramFolder) ;
	VerifyCfeIni() ;

	if(szExportDirectory[0] == '\0')
		sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%w\\Backup & Export",szProgramFolder) ;

	GetPrivateProfileString(szProfile_09,szProfile_24,_szTmp,szExportDirectory,MAX_PATH,szProfileFileName) ;
	PathRelativePathTo(_szRelativePath,szProgramFolder,FILE_ATTRIBUTE_DIRECTORY,szExportDirectory,FILE_ATTRIBUTE_DIRECTORY) ;
	
	if(_szRelativePath[0] != '\0')
		lstrcpy(szExportDirectory,_szRelativePath) ;

	_lpszString = szExportDirectory ;

	while(*_lpszString++) ;

//		********************************************************************************
//		*** The length of a folder name is not equal to MAX_PATH but 248             ***
//		*** For the ANSI version of this function,                                   ***
//		*** there is a default string size limit for paths of 248 characters         ***
//		*** (MAX_PATH - enough room for a 8.3 filename).                             ***
//		*** To extend this limit to 32,767 wide characters,                          ***
//		*** call the Unicode version of the function and prepend "\\?\" to the path. ***
//		********************************************************************************

//		https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createdirectorya

/*
	La chaîne fait 54 caractères de long
	Le test renvoie 194 car c'est égal 248 - 54
	Si la valeur est < 0 cela signifie que la chaîne fait plus de 248 caractères.
	249 = 248 + 1 caractère à enlever de la longueur de la chaîne '\0' compté comme un caractère.
*/

	if((249 - ((int) _lpszString - (int) szExportDirectory)) < 0)
	{
		Log("The name of the Export Directory is too long.") ;

		Log("Removing GFL SDK from memory") ;

		gflLibraryExit() ;			// Unload Gfl

		Log("Removing SQLite from memory") ;

		sqlite3_shutdown() ;		// Unload SQLite

		Log_Exit() ;

		return (SpecialError("The name of the Export Directory is too long.")) ;
	}

	Log("Creating Export Directory") ;

	if(CreateDirectory(szExportDirectory,NULL) == 0)
	{
		_iError = GetLastError() ;
		if(_iError == ERROR_PATH_NOT_FOUND)
		{
//			****************************************************
//			*** The folder does not exist                    ***
//			*** The Export directory will be set to <AppDir> ***
//			*** The file CoFeE.ini will be updated            ***
//			****************************************************

			sqlite3_snprintf(sizeof(szExportDirectory),szExportDirectory,"%w\\Backup & Export",szProgramFolder) ;
			PathRelativePathTo(_szRelativePath,szProgramFolder,FILE_ATTRIBUTE_DIRECTORY,szExportDirectory,FILE_ATTRIBUTE_DIRECTORY) ;
			lstrcpy(szExportDirectory,_szRelativePath) ;

			if(CreateDirectory(szExportDirectory,NULL) == 0)
			{
				if(GetLastError() != ERROR_ALREADY_EXISTS)
				{
//					********************************************
//					*** Not possible to create the directory ***
//					*** Tell it to the user and exit         ***
//					********************************************

					Log("Removing GFL SDK from memory") ;

					gflLibraryExit() ;			// Unload Gfl

					Log("Removing SQLite from memory") ;

					sqlite3_shutdown() ;		// Unload SQLite

					Log("Cannot create the Export Directory.") ;

					return (SpecialError("Cannot create the Export Directory.")) ;
				}

				Log("Cannot create the export directory because it already exists. Not an error.") ;
			}

			Log("Writing the Export Directory into CoFeE.ini") ;

			WritePrivateProfileString(szProfile_09,szProfile_24,szExportDirectory,szProfileFileName) ;
		}
		else
		{
			if(GetLastError() == ERROR_ALREADY_EXISTS)
				Log("Cannot create the export directory because it already exists. Not an error.") ;
			else
			{
//				********************************************
//				*** Not possible to create the directory ***
//				*** Tell it to the user and exit         ***
//				********************************************

				Log("Removing GFL SDK from memory") ;

				gflLibraryExit() ;			// Unload Gfl

				Log("Removing SQLite from memory") ;

				sqlite3_shutdown() ;		// Unload SQLite

				Log("Cannot create the Export Directory.") ;

				return (SpecialError("Cannot create the Export Directory.")) ;
			}
		}
	}

//	****************************************
//	*** Search for language in CoFeE.ini ***
//	****************************************

//	*********************************************************************************
//	*** If the language file is not found the variable "hLanguage" is set to NULL ***
//	*********************************************************************************

	hLanguage = NULL ;

	if(GetPrivateProfileString(szProfile_01,"Language",szEmptyString,_szTmp,MAX_PATH,szProfileFileName))
	{
		Log("Language found in CoFeE.ini") ;

//		******************************************
//		*** Search if the language file exists ***
//		******************************************

//		CoFeE.ini must be in .\\CoFeE.ini

//		Where "." is the directory from which the program CoFeE.is launched

//		The file name musr begin by "Lng_"
//		The file name must be terminated by ".cfe"

		sqlite3_snprintf(sizeof(szLanguageFile),szLanguageFile,"%w\\Lng_%w.cfe",szProgramFolder,_szTmp) ;

		if(PathFileExists(szLanguageFile))
			Log("The language file exists") ;
		else
		{
			Log("The language file does not exist") ;

			Log("Removing GFL SDK from memory") ;

			gflLibraryExit() ;			// Unload Gfl

			Log("Removing SQLite from memory") ;

			sqlite3_shutdown() ;		// Unload SQLite

			return (SpecialError("Cannot load the language file, CFE stops running.")) ;
		}

		if(!LoadLanguageFile(szLanguageFile))
		{
			Log("Cannot load the language file, CFE stops running.") ;

			Log("Removing GFL SDK from memory") ;

			gflLibraryExit() ;			// Unload Gfl

			Log("Removing SQLite from memory") ;

			sqlite3_shutdown() ;		// Unload SQLite

			return (SpecialError("Cannot load the language file, CFE stops running.")) ;
		}

		Log("Language file loaded in memory") ;
	}
	else
	{
//		******************************
//		*** No Language File found ***
//		******************************

		Log("Removing GFL SDK from memory") ;

		gflLibraryExit() ;			// Unload Gfl

		Log("Removing SQLite from memory") ;

		sqlite3_shutdown() ;		// Unload SQLite

		Log("*** No Language File found ***") ;
		Log_Exit() ;

		return (SpecialError("No Language File found.")) ;
	}

//	**********************************************
//	*** Create the name of the ZeeGrid library ***
//	**********************************************

	sqlite3_snprintf(sizeof(szZeeGridFileName),szZeeGridFileName,"%w\\ZeeGrid.dll",szProgramFolder) ;

//	*****************************************
//	*** Try to load the library in memory ***
//	*****************************************

	hZeeGrid = LoadLibrary(szZeeGridFileName) ;
	if(!hZeeGrid)
	{
		Log("Cannot load ZeeGrid") ;
		Log_Exit() ;

		return (DisplayError(40)) ;
	}

	Log("ZeeGrid correctly initialized") ;

//	****************************
//	*** Stack Initialization ***
//	****************************

	RtlSecureZeroMemory(&FilterStack,sizeof(FilterStack)) ;
	lpTopStack = &FilterStack[0] ;
	lpFilterStack = &FilterStack[MAX_FILTER_STACK - 1] ;
	iStackPointer = -1;

	Log("Undo Stack correctly initialized") ;

//	***********************************************************
//	*** Once the image is loaded, the filter is not applied ***
//	***********************************************************

	bFirstDrop = TRUE ;

//	****************************
//	*** Initialize the theme ***
//	****************************

	lpCurrentTheme = &Theme_Dark ;

	hButtons = LoadBitmap(hInstance,MAKEINTRESOURCE(BMP_BUTTONS)) ;
	hIcons = LoadBitmap(hInstance,MAKEINTRESOURCE(BMP_TOOLBAR)) ;

//	*******************************
//	*** Create the first filter ***
//	*******************************

	CreateFilter() ;

//	*****************************
//	*** Where is the database ***
//	*****************************

//		This file must be in the subfolder "Datas"
//		The file name must be terminated by ".db3"

//	********************************************
//	*** Create the name of the database file ***
//	********************************************

	sqlite3_snprintf(sizeof(szDatabaseFileName),szDatabaseFileName,"%w\\CoFeE.db3",szProgramFolder) ;

//	**********************************************
//	*** Copy the database in "Backup & Export" ***
//	**********************************************

	if(PathFileExists(szDatabaseFileName))
	{
		if(GetPrivateProfileInt(szProfile_09,szProfile_23,0,szProfileFileName) == 1)
		{
			sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%w\\CoFeE.db3",szExportDirectory) ;

			if(CopyFile(szDatabaseFileName,_szTmp,0) == 0)
			{
				Log("Cannot copy the database to the backup folder") ;

				return (DisplayError(60)) ;
			}

			Log("Database backup finished successfuly") ;
		}
	}

	if(!OpenCfeDataBase())
	{
		Log("Cannot open the database") ;
		Log_Exit() ;

		return (DisplayError(11)) ;
	}

	Log("The database is now opened") ;

//	***************************************************
//	*** Get and Set some informations about windows ***
//	***************************************************

	SystemParametersInfo(SPI_GETWORKAREA,0,&RcWorkArea,0) ;
	SystemParametersInfo(SPI_SETGRADIENTCAPTIONS,0,(LPVOID) TRUE,0) ;
	SystemParametersInfo(SPI_SETCURSORSHADOW,0,(LPVOID) TRUE,0) ;
	SystemParametersInfo(SPI_SETGRADIENTCAPTIONS,0,(LPVOID) TRUE,0) ;
	SystemParametersInfo(SPI_SETMENUANIMATION,0,(LPVOID) TRUE,0) ;
	SystemParametersInfo(SPI_SETMENUFADE,0,(LPVOID) TRUE,0) ;
	SystemParametersInfo(SPI_SETFLATMENU,0,(LPVOID) TRUE,0) ;
	SystemParametersInfo(SPI_SETSELECTIONFADE,0,(LPVOID) TRUE,0) ;

//	*******************
//	*** Prepare CFE ***
//	*******************

	lstrcpy(szDlgStatic_01,"Fun with Convolution Filters Editor") ;
	lstrcpy(szDlgStatic_02,"By Philippe RIO") ;

//	********************************
//	*** Empty the TEMP directory ***
//	********************************

	Log("CoFeE will delete the temporary files") ;

	GetTempPath(MAX_PATH,_szTmp) ;
	DeleteTempFiles(_szTmp) ;

	Log("CoFeE has successfully deleted the temporary files") ;

//	*********************************
//	*** Loading Lock/Unlock icons ***
//	*********************************

	hIcon1 = LoadIcon(hInstance,MAKEINTRESOURCE(IDR_ICO_LOCKED)) ;
	hIcon2 = LoadIcon(hInstance,MAKEINTRESOURCE(IDR_ICO_UNLOCKED)) ;

//	****************************
//	*** Register the classes ***
//	****************************

	Log("Registering the windows classes") ;

	RtlSecureZeroMemory(_szTmp,sizeof(_szTmp)) ;

	_lpWc = (LPWNDCLASSEX) _szTmp ;

//	*************************************
//	*** Register ToolBox window class ***
//	*************************************

	_lpWc->cbSize			= sizeof(WNDCLASSEX) ;
	_lpWc->lpszClassName	= "COFEE" ;
	_lpWc->lpfnWndProc		= ToolBoxWndProc ;
	_lpWc->style			= CS_OWNDC|CS_DBLCLKS|CS_BYTEALIGNWINDOW|CS_BYTEALIGNCLIENT ;
	_lpWc->hInstance		= hInstance ;
	_lpWc->hIcon			= LoadIcon(hInstance,MAKEINTRESOURCE(IDR_ICO_01_MAIN)) ;
	_lpWc->hCursor			= LoadCursor(NULL,IDC_ARROW) ;
	_lpWc->hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH) ;
	_lpWc->cbWndExtra		= 16 ;

	if(!RegisterClassEx(_lpWc))
	{
		Log("Cannot register ToolBox") ;
		Log_Exit() ;

		return (DisplayError(37)) ;
	}

	Log("ToolBox registered") ;

//	********************************
//	*** Register window class #1 ***
//	********************************

	_lpWc->hIcon			= NULL ;
	_lpWc->lpszClassName	= "CHILD_1" ;
	_lpWc->lpfnWndProc		= Ch1_Original_Proc ;
	_lpWc->hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH) ;

	if(!RegisterClassEx(_lpWc))
	{
		Log("Cannot register CHILD_1") ;
		Log_Exit() ;

		return (DisplayError(42)) ;
	}

	Log("Child 1 registered") ;

//	********************************
//	*** Register window class #2 ***
//	********************************

	_lpWc->lpszClassName	= "CHILD_2" ;
	_lpWc->lpfnWndProc		= Ch2_Workspace_Proc ;

	if(!RegisterClassEx(_lpWc))
	{
		Log("Cannot register CHILD_2") ;
		Log_Exit() ;

		return (DisplayError(44)) ;
	}

	Log("Child 2 registered") ;

//	********************************
//	*** Register window class #3 ***
//	********************************

	_lpWc->lpszClassName	= "CHILD_3" ;
	_lpWc->lpfnWndProc		= Ch3_Editor_Proc ;
	_lpWc->hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH) ;

	if(!RegisterClassEx(_lpWc))
	{
		Log("Cannot register CHILD_3") ;
		Log_Exit() ;

		return (DisplayError(45)) ;
	}

	Log("Child 3 registered") ;

//	********************************
//	*** Register window class #4 ***
//	********************************

	_lpWc->lpszClassName	= "CHILD_4" ;
	_lpWc->lpfnWndProc		= Ch4_Result_Proc ;
	_lpWc->hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH) ;

	if(!RegisterClassEx(_lpWc))
	{
		Log("Cannot register CHILD_4") ;
		Log_Exit() ;

		return (DisplayError(80)) ;
	}

	Log("Child 4 registered") ;

//	********************************
//	*** Register window class #5 ***
//	********************************

	_lpWc->lpszClassName	= "CHILD_5" ;
	_lpWc->lpfnWndProc		= Ch5_Diaporama_Proc ;

	if(!RegisterClassEx(_lpWc))
	{
		Log("Cannot register CHILD_5") ;
		Log_Exit() ;

		return (DisplayError(16)) ;
	}

	Log("Child 5 registered") ;

//	********************************
//	*** Register window class #6 ***
//	********************************

	_lpWc->lpszClassName	= "CHILD_6" ;
	_lpWc->lpfnWndProc		= Ch6_Options_Proc ;

	if(!RegisterClassEx(_lpWc))
	{
		Log("Cannot register CHILD_6") ;
		Log_Exit() ;

		return (DisplayError(43)) ;
	}

	Log("Child 6 registered") ;

	SetThemeAppProperties(STAP_ALLOW_NONCLIENT|STAP_ALLOW_CONTROLS) ;

	_x = GetPrivateProfileInt(szProfile_09,szProfile_10,0,szProfileFileName) ;
	_y = GetPrivateProfileInt(szProfile_09,szProfile_11,0,szProfileFileName) ;
	_w = GetPrivateProfileInt(szProfile_09,szProfile_12,MIN_WINDOW_WIDTH,szProfileFileName) ;
	_h = GetPrivateProfileInt(szProfile_09,szProfile_13,MIN_WINDOW_HEIGHT,szProfileFileName) ;

	if(_w == 0)	_w = MIN_WINDOW_WIDTH ;
	if(_h == 0)	_h = MIN_WINDOW_HEIGHT ;
	if(_w < 0)	_w = MIN_WINDOW_WIDTH ;
	if(_h < 0)	_h = MIN_WINDOW_HEIGHT ;

	if(_y == 0)	_y = (RcWorkArea.bottom - _h) / 2 ;
	if(_x == 0)	_x = (RcWorkArea.right - _w) / 2 ;

	_hWnd = CreateWindowEx(WS_EX_ACCEPTFILES,"COFEE",Titles(3),
				WS_OVERLAPPEDWINDOW|WS_VISIBLE,_x,_y,_w,_h,NULL,NULL,hInstance,NULL) ;

	if(!_hWnd)
	{
		Log("Cannot create the main window") ;
		Log_Exit() ;

		return (DisplayError(47)) ;
	}

	Log("Main window created") ;

	ShowWindow(_hWnd,SW_SHOWNORMAL) ;
	InvalidateRect(_hWnd,NULL,TRUE) ;
	UpdateWindow(_hWnd) ;

	SendMessage(_hWnd,WM_THEMECHANGED,0,0) ;

//			********************************
//			*** Messages loop start here ***
//			********************************

	_lpMsg = (LPMSG) _szTmp ;

	while(GetMessage(_lpMsg,NULL,0,0))
	{
		if((!IsWindow(hModless)) || (!IsDialogMessage(hModless,_lpMsg)))
		{
			if((!IsWindow(hWaiting)) || (!IsDialogMessage(hWaiting,_lpMsg)))
			{
				if(!TranslateAccelerator(hToolBox,hAccel,_lpMsg))
				{
					TranslateMessage(_lpMsg) ;
					DispatchMessage(_lpMsg) ;
				}
			}
		}
	}

	DestroyIcon(hIcon1) ;
	DestroyIcon(hIcon2) ;

	Log("The user has exited CFE and now CFE will export the filters") ;

	if(GetPrivateProfileInt(szProfile_09,szProfile_22,0,szProfileFileName) == 1)
	{
		lstrcpy(szDlgStatic_01,Strings(15)) ;
		lstrcpy(szDlgStatic_02,Strings(16)) ;

		hModless = (HWND) CreateDialog(hInstance,MAKEINTRESOURCE(DLG_MODLESS),NULL,(DLGPROC) DlgModlessProc) ;
		if(hModless)
		{
			ShowWindow(hModless,SW_SHOW) ;

			SendMessage(hModless,WM_COMMAND,IDM_SET_TEXT_01,(LPARAM) NULL) ;
			SendMessage(hModless,WM_COMMAND,IDM_SET_TEXT_02,(LPARAM) NULL) ;
		}

		Log("Exporting filters") ;

		ExportFilters() ;

		Log("Filters exported successfuly") ;

		if(hModless)
			DestroyWindow(hModless) ;
	}
	else
	{
		Log("The user does not want to export the filters") ;
	}

	UnLoadLanguageFile() ;

	Log("Language file unloaded") ;

	sqlite3_close_v2(hCfeDb) ;
	hCfeDb = NULL ;

	Log("Database closed") ;

	sqlite3_shutdown() ;

	Log("SQLite removed from memory") ;

	gflLibraryExit() ;

	Log("GFL SDK removed from memory"); 

	FreeLibrary(hZeeGrid) ;

	Log("ZeeGrid removed from memory") ;

	Log_Exit() ;

	return (_lpMsg->wParam) ;
}
