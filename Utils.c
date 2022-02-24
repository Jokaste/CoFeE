#include <windows.h>
#include <CommCtrl.h>
#include <commdlg.h>
#include <wingdi.h>
#include <Shlobj.h>
#include <uxtheme.h>
#include <shlwapi.h>

#pragma warn(disable:2006 2145 2215 2228 2805)

#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Ressources.h"

#include "sph_keccak.h"

#include "sqlite3.h"

#include "Languages.h"

HANDLE	hLogFile ;
BOOL	bLog ;

char	szSQLiteErrorTitle[]	=	"SQLite Error" ;
char	szLogFileName[MAX_PATH] ;

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR Get_SQLite_Error(LPSTR __lpszErrorText)
{
	LPSTR		_lpszSQLiteError ;
	int			_iError ;

	_lpszSQLiteError = (LPSTR) sqlite3_errstr(sqlite3_errcode(hCfeDb)) ;
	_iError = sqlite3_errcode(hCfeDb) ;

	sqlite3_snprintf(1024,__lpszErrorText,"%d - %s",_iError,_lpszSQLiteError) ;

	return (__lpszErrorText) ;
}


//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int SQLite_Error(void)
{
	LPSTR		_lpszSQLiteError ;
	char		_szResult[256] ;
	int			_iError ;
	FLASHWINFO	_Flash ;

	_lpszSQLiteError = (LPSTR) sqlite3_errstr(sqlite3_errcode(hCfeDb)) ;
	_iError = sqlite3_errcode(hCfeDb) ;

	_Flash.cbSize = sizeof(FLASHWINFO) ;
	_Flash.hwnd = hToolBox ;
	_Flash.dwFlags = FLASHW_ALL ;
	_Flash.uCount = 10 ;
	_Flash.dwTimeout = 0 ;

	sqlite3_snprintf(sizeof(_szResult),_szResult,"%d - %s",_iError,_lpszSQLiteError) ;

	FlashWindowEx(&_Flash) ;
	MessageBeep(MB_ICONERROR) ;

	Log(_szResult) ;

	MessageBoxA(hToolBox,_szResult,szSQLiteErrorTitle,MB_OK|MB_ICONERROR|MB_APPLMODAL|MB_TOPMOST) ;

	return (FALSE) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

void Keccak(register LPBYTE __lpcbData,DWORD __dwSize,register LPBYTE __lpcbResult)
{
	sph_keccak256_context	_Keccak ;

	sph_keccak256_init(&_Keccak) ;
	sph_keccak256(&_Keccak,__lpcbData,__dwSize) ;
	sph_keccak256_close(&_Keccak,__lpcbResult) ;

	return ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPBYTE Hex2Str(LPBYTE __lpBuffer,LPBYTE __lpszResult,size_t __iSize)
{
	register	char	_c ;
	register	char	_d ;

				size_t	_i ;
				size_t	_j ;

				LPBYTE	_lpszTmp ;

	_lpszTmp = __lpszResult ;

	_j = 0 ;

	for(_i = 0 ; _i < __iSize ; _i++)
	{
		_c = *(__lpBuffer + _i) ;
		_d = (_c >> 4 ) & 0x0f ;
		_c &= 0x0f ;

		if((_c >= 0) && (_c <= 9))	_c += '0' ;
		else						_c = (_c - 0x0a) + 'A' ;

		*(_lpszTmp + (_j + 1)) = _c ;

		if((_d >= 0) && (_d <= 9))	_d += '0' ;
		else						_d = (_d - 0x0a) + 'A' ;

		*(_lpszTmp + _j) = _d ;

		_j += 2 ;
	}

	*(__lpszResult + (2 * __iSize)) = '\0' ;

	return (__lpszResult) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

void SQLitePragmas(void)
{
	sqlite3_exec(hCfeDb,szPragma_01,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_02,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_03,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_04,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_05,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_06,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_07,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_08,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_09,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_10,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_11,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_12,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_13,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_14,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_15,0,0,NULL) ;
	sqlite3_exec(hCfeDb,szPragma_16,0,0,NULL) ;

	return ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int IsAtStartOfEditControl(HWND __hWnd)
{
	return (LOWORD(SendMessage(__hWnd,EM_GETSEL,0,0)) == 0) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LRESULT CALLBACK SignedIntegerSubclassProc(HWND __hWnd,UINT __uMsg,WPARAM __wParam,LPARAM __lParam,UINT_PTR __uIdSubclass,DWORD_PTR __dwRefData)
{
	char	_ch ;

	__dwRefData = __dwRefData ;

	switch(__uMsg)
	{
		case	WM_NCDESTROY :	RemoveWindowSubclass(__hWnd,SignedIntegerSubclassProc,__uIdSubclass) ;
								break ;

		case	WM_CHAR :		_ch = (char) __wParam ;

								if(_ch < ' ')						break ;		// let control character through
								else if((_ch == '-' || _ch == '.') &&			// hyphen-minus or Unicode minus sign
									IsAtStartOfEditControl(__hWnd))	break ;		// at start of edit control is okay
								else if(isdigit(_ch))				break ;		// let digit through
									MessageBeep(0) ;							// otherwise invalid

								return (0) ;
	}

	return (DefSubclassProc(__hWnd,__uMsg,__wParam,__lParam)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

BOOL EnforceSignedIntegerEdit(HWND __hWnd)
{
	return (SetWindowSubclass(__hWnd,SignedIntegerSubclassProc,0,0)) ;
}

void DeleteTempFiles(LPSTR __lpszDirectory)
{
	register	LPWIN32_FIND_DATA	_lpWf ;
	register	LPSTR				_lpszTmp ;
				HANDLE				_hFindFile ;
				WIN32_FIND_DATA		_Wf ;
				char				_szTmp[1024] ;

	_lpWf = &_Wf ;
	_lpszTmp = _szTmp ;

	wsprintf(_lpszTmp,"%s\\CLP*.tmp",__lpszDirectory) ;

	_hFindFile = FindFirstFile(_lpszTmp,_lpWf) ;
	if(_hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(_lpWf->cFileName[0] != '.')
			{
				wsprintf(_lpszTmp,"%s\\%s",__lpszDirectory,_lpWf->cFileName) ;
				DeleteFile(_lpszTmp) ;
				Log(_lpszTmp) ;
			}

		} while(FindNextFile(_hFindFile,_lpWf)) ;

		FindClose(_hFindFile) ;
	}

	return ;
}
/*
int CALLBACK MyBrowseCallbackProc(HWND __hWnd,UINT __uMsg,WPARAM __wParam,LPARAM __lParam)
{
	switch (__uMsg)
	{
		case	BFFM_INITIALIZED :	SendMessage(__hWnd,BFFM_SETSELECTION,TRUE,(LPARAM) szUserDirectory) ;
									break;
	}

	return (0) ;
}

LPSTR BrowseForFolder(HWND __hWnd,LPSTR __lpszPath,LPSTR __lpszBrowseForFolderTitle)
{
	BROWSEINFO		_Bi;
	CHAR			_szDisplayName[MAX_PATH];
	LPITEMIDLIST	_Pidl;

	memset(&_Bi,0,sizeof(BROWSEINFO)) ;
	memset(&_szDisplayName,0,MAX_PATH) ;

	_Bi.hwndOwner = __hWnd;
	_Bi.ulFlags = BIF_USENEWUI|BIF_DONTGOBELOWDOMAIN|BIF_EDITBOX|BIF_NEWDIALOGSTYLE ;
	_Bi.pszDisplayName = _szDisplayName;
	_Bi.lpszTitle = __lpszBrowseForFolderTitle ;

	_Bi.lpfn = (BFFCALLBACK) MyBrowseCallbackProc ;

	_Pidl = SHBrowseForFolder(&_Bi) ;
	if (NULL != _Pidl)
	{
		SHGetPathFromIDList(_Pidl,__lpszPath) ;
		return (__lpszPath) ;
	}

	return (NULL) ;
}
*/


//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPBYTE GetBitsFromBitmap(HBITMAP __hBitmap,LPDWORD __lpdwSize,LPBYTE __lpResult)
{
	register		LPIMAGEINFOS	_lpImageInfos ;
	register		HDC				_hDC ;
					LPBYTE			_lpResult ;

	BITMAP			_Bitmap ;
	IMAGEINFOS		_ImageInfos ;

	_lpImageInfos = &_ImageInfos ;

	_lpResult = NULL ;

	GetObject(__hBitmap,sizeof(BITMAP),&_Bitmap) ;

	_hDC = GetDC(NULL) ;
	if(_hDC)
	{
		_lpImageInfos->BitmapInfo.bmiHeader.biBitCount = _Bitmap.bmBitsPixel ;
		_lpImageInfos->BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ;
		_lpImageInfos->BitmapInfo.bmiHeader.biWidth = _Bitmap.bmWidth ;
		_lpImageInfos->BitmapInfo.bmiHeader.biHeight = _Bitmap.bmHeight ;
		_lpImageInfos->BitmapInfo.bmiHeader.biPlanes = _Bitmap.bmPlanes ;
		_lpImageInfos->BitmapInfo.bmiHeader.biCompression = BI_RGB ;

		if(GetDIBits(_hDC,__hBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,NULL,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
		{
			_lpImageInfos->Image.iPixelSize = _Bitmap.bmBitsPixel / 8 ;
			_lpImageInfos->Image.iScanlineSize = (_lpImageInfos->Image.iPixelSize * _Bitmap.bmWidth + 3) & ~3 ;
			_lpImageInfos->Image.iBitmapSize = _Bitmap.bmHeight * _lpImageInfos->Image.iScanlineSize ;

			_lpImageInfos->BitmapInfo.bmiHeader.biSizeImage = (unsigned long int) _lpImageInfos->Image.iBitmapSize ;

			if(__lpResult == NULL)
				_lpResult = (LPBYTE) PhR_Alloc_Memory((DWORD) _lpImageInfos->BitmapInfo.bmiHeader.biSizeImage) ;
			else
				_lpResult = __lpResult ;

			if(_lpResult)
			{
				if(!GetDIBits(_hDC,__hBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpResult,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
				{
					PhR_Free_Memory(_lpResult) ;
					_lpResult = NULL ;
				}
			}
		}

		ReleaseDC(NULL,_hDC) ;
	}

	if(__lpdwSize)
		*__lpdwSize = _lpImageInfos->BitmapInfo.bmiHeader.biSizeImage ;

	return (_lpResult) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

void ImageResize(DWORD __dwOriginalWidth,DWORD __dwOriginalHeight,DWORD __dwExpectedWidth,DWORD __dwExpectedHeight,DWORD *__lpdwNewWidth,DWORD *__lpdwNewHeight)
{
	register		DWORD	_dwWidth ;
	register		DWORD	_dwHeight ;

	double	dRatio ;

	if(__dwOriginalWidth > __dwOriginalHeight)
	{
		dRatio = (double) __dwOriginalHeight / (double) __dwOriginalWidth ;

		_dwWidth = __dwExpectedWidth ;
		_dwHeight = (DWORD) ((double) _dwWidth * dRatio) ;

		if(_dwHeight > __dwExpectedHeight)
		{
			_dwHeight = __dwExpectedHeight ;
			_dwWidth = (DWORD) (((double) _dwHeight) / dRatio) ;
		}

		*__lpdwNewWidth = _dwWidth ;
		*__lpdwNewHeight = _dwHeight ;

		return ;
	}

	dRatio = (double) __dwOriginalWidth / (double) __dwOriginalHeight ;

	_dwHeight = __dwExpectedHeight ;
	_dwWidth = (DWORD) (((double) _dwHeight) * dRatio) ;

	if(_dwWidth > __dwExpectedWidth)
	{
		_dwWidth = __dwExpectedWidth ;
		_dwHeight = (DWORD) (((double) _dwWidth) / dRatio) ;
	}

	*__lpdwNewWidth = _dwWidth ;
	*__lpdwNewHeight = _dwHeight ;

	return ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPIMAGEINFOS LoadImageFromFile(register LPSTR __lpszImageFile,DWORD __dwBorderWidth,DWORD __dwBorderHeight)
{
	register		LPIMAGEINFOS			_lpImageInfos ;

	GFL_LOAD_PARAMS			_LoadParams ;
	GFL_FILE_INFORMATION	_GflFileInformation ;
	WIN32_FIND_DATA			_FileFind ;
	GFL_BITMAP 				*_GflBitmap ;
	GFL_COLOR				_GflColor ;
	BITMAP					_Bitmap ;
	HBITMAP					_hImgBitmap ;
	HDC						_hDC ;
	HANDLE					_hFind ;

	if(!__lpszImageFile)	return (NULL) ;

	_hFind = FindFirstFile(__lpszImageFile,&_FileFind) ;
	if(_hFind == INVALID_HANDLE_VALUE)
		return (NULL) ;	// Fichier invalide

	FindClose(_hFind) ;

	if(_FileFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)			return (NULL) ;	// Il s'agit dun dossier
	if(_FileFind.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)				return (NULL) ;
	if(_FileFind.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)			return (NULL) ;
	if(_FileFind.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)				return (NULL) ;
	if(_FileFind.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)			return (NULL) ;
	if(_FileFind.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)			return (NULL) ;
	if(_FileFind.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)		return (NULL) ;
	if(_FileFind.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)			return (NULL) ;
	if(_FileFind.nFileSizeHigh != 0)									return (NULL) ;	// Image trop Grosse

	gflGetDefaultLoadParams(&_LoadParams) ;

	_LoadParams.Flags = GFL_LOAD_SKIP_ALPHA ;
	_LoadParams.FormatIndex = -1 ;
	_LoadParams.Origin = GFL_TOP_LEFT ;
	_LoadParams.ColorModel = GFL_ARGB ;

	_lpImageInfos = (LPIMAGEINFOS) PhR_Alloc_Memory(sizeof(IMAGEINFOS)) ;
	if(_lpImageInfos)
	{
		_lpImageInfos->StructureSize = sizeof(IMAGEINFOS) ;

		if(gflLoadBitmap(__lpszImageFile,&_GflBitmap,&_LoadParams,&_GflFileInformation) == GFL_NO_ERROR)
		{
			RtlSecureZeroMemory(&_GflColor,sizeof(GFL_COLOR)) ;

			if(gflResizeCanvas(_GflBitmap,NULL,_GflBitmap->Width + (2 * __dwBorderWidth),_GflBitmap->Height + ( 2 * __dwBorderHeight),GFL_CANVASRESIZE_CENTER,&_GflColor) == GFL_NO_ERROR)
			{
				if(gflConvertBitmapIntoDDB(_GflBitmap,&_hImgBitmap) == GFL_NO_ERROR)
				{
					_lpImageInfos->Image.dwFormatIndex = _GflFileInformation.FormatIndex ;

					gflFreeFileInformation(&_GflFileInformation) ;

					if(_hImgBitmap)
					{
						_hDC = GetDC(NULL) ;
						if(_hDC)
						{
							if(GetObject(_hImgBitmap,sizeof(BITMAP),&_Bitmap))
							{
								_lpImageInfos->Image.iPixelSize = _Bitmap.bmBitsPixel >> 3 ;
								_lpImageInfos->Image.iScanlineSize = (_lpImageInfos->Image.iPixelSize * _Bitmap.bmWidth + 3) & ~3 ;
								_lpImageInfos->Image.iBitmapSize = _Bitmap.bmHeight * _lpImageInfos->Image.iScanlineSize ;

								_lpImageInfos->BitmapInfo.bmiHeader.biBitCount = 0 ;
								_lpImageInfos->BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ;
								_lpImageInfos->BitmapInfo.bmiHeader.biWidth = _Bitmap.bmWidth ;
								_lpImageInfos->BitmapInfo.bmiHeader.biHeight = _Bitmap.bmHeight ;
								_lpImageInfos->BitmapInfo.bmiHeader.biPlanes = _Bitmap.bmPlanes ;

								if(_lpImageInfos->BitmapInfo.bmiHeader.biSizeImage <= 0)
									_lpImageInfos->BitmapInfo.bmiHeader.biSizeImage = (unsigned long int) _lpImageInfos->Image.iBitmapSize ;

								if(GetDIBits(_hDC,_hImgBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,NULL,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
								{
									_lpImageInfos->dwMemBlockSize = (unsigned long int) _lpImageInfos->Image.iBitmapSize ;
									_lpImageInfos->dwMemBlockSize /= 512 ;
									_lpImageInfos->dwMemBlockSize *= 512 ;

									_lpImageInfos->lpOriginal = (LPBYTE) PhR_Alloc_Memory((DWORD) _lpImageInfos->Image.iBitmapSize) ;
									if(_lpImageInfos->lpOriginal)
									{
										_lpImageInfos->lpResult = (LPBYTE) PhR_Alloc_Memory((DWORD) _lpImageInfos->BitmapInfo.bmiHeader.biSizeImage) ;
										if(_lpImageInfos->lpResult)
										{
											_lpImageInfos->lpWork = (LPBYTE) PhR_Alloc_Memory((DWORD) _lpImageInfos->BitmapInfo.bmiHeader.biSizeImage) ;
											if(_lpImageInfos->lpWork)
											{
												if(GetDIBits(_hDC,_hImgBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpOriginal,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
												{
													if(GetDIBits(_hDC,_hImgBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpResult,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
													{
														if(_Bitmap.bmWidth > _Bitmap.bmHeight)
															_lpImageInfos->Image.dwSensImage = IMAGE_SENS_HORIZONTAL ;
														else
															if(_Bitmap.bmWidth == _Bitmap.bmHeight)
																_lpImageInfos->Image.dwSensImage = IMAGE_SENS_CARRE ;
															else
																_lpImageInfos->Image.dwSensImage = IMAGE_SENS_VERTICAL ;

														_lpImageInfos->Image.dRatio = _Bitmap.bmHeight / _Bitmap.bmWidth ;
														_lpImageInfos->Image.dwFileSize = _FileFind.nFileSizeLow ;
														_lpImageInfos->hImgBitmap = _hImgBitmap ;
														_lpImageInfos->Image.lpszImageFile = lstrcpy(_lpImageInfos->Image.szImageFile,__lpszImageFile) ;

														ReleaseDC(NULL,_hDC) ;

														gflFreeBitmapData(_GflBitmap) ;
														gflFreeBitmap(_GflBitmap) ;

														return (_lpImageInfos) ;
													}
												}

												PhR_Free_Memory(_lpImageInfos->lpWork) ;
											}

											PhR_Free_Memory(_lpImageInfos->lpResult) ;
										}

										PhR_Free_Memory(_lpImageInfos->lpOriginal) ;
									}
								}
							}

							ReleaseDC(NULL,_hDC) ;
						}

						DeleteObject(_hImgBitmap) ;
					}
				}
			}

			gflFreeBitmapData(_GflBitmap) ;
			gflFreeBitmap(_GflBitmap) ;
		}

		PhR_Free_Memory(_lpImageInfos) ;
	}

	return (NULL) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

void ImageFree(LPIMAGEINFOS __lpImageInfos)
{
	DeleteObject(__lpImageInfos->hImgBitmap) ;

	PhR_Free_Memory(__lpImageInfos->lpOriginal) ;
	PhR_Free_Memory(__lpImageInfos->lpResult) ;
	PhR_Free_Memory(__lpImageInfos->lpWork) ;
	PhR_Free_Memory(__lpImageInfos) ;

	return ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

HBITMAP SetBitsToBitmap(int __iWidth,int __iHeight,LPBYTE __lpBits)
{
	HDC			_hDC ;
	HBITMAP		_hBitmap ;
	BITMAPINFO	_Bmi ;
	BITMAP		_Bitmap ;
	int			_iPixelSize, _iScanlineSize, _iBitmapSize ;

	_hDC = GetDC(NULL) ;
	if(_hDC)
	{
		_hBitmap = CreateCompatibleBitmap(_hDC,__iWidth,__iHeight) ;
		if(_hBitmap)
		{
			GetObject(_hBitmap,sizeof(BITMAP),&_Bitmap) ;

			_iPixelSize = _Bitmap.bmBitsPixel / 8 ;
			_iScanlineSize = (_iPixelSize * _Bitmap.bmWidth + 3) & ~3 ;
			_iBitmapSize = _Bitmap.bmHeight * _iScanlineSize;

			_Bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ;
			_Bmi.bmiHeader.biWidth = __iWidth ;
			_Bmi.bmiHeader.biHeight = __iHeight ;
			_Bmi.bmiHeader.biPlanes = 1 ;
			_Bmi.bmiHeader.biBitCount = 32 ;
			_Bmi.bmiHeader.biCompression = BI_RGB ;
			_Bmi.bmiHeader.biXPelsPerMeter = 0 ;
			_Bmi.bmiHeader.biYPelsPerMeter = 0 ;
			_Bmi.bmiHeader.biClrUsed = 0 ;
			_Bmi.bmiHeader.biClrImportant = 0 ;
			_Bmi.bmiHeader.biSizeImage = _iBitmapSize ;

			if(SetDIBits(_hDC,_hBitmap,0,_Bmi.bmiHeader.biHeight,__lpBits,&_Bmi,DIB_RGB_COLORS))
			{
				ReleaseDC(NULL,_hDC) ;
				return (_hBitmap) ;
			}

			DeleteObject(_hBitmap) ;
		}

		ReleaseDC(NULL,_hDC) ;
	}

	return (NULL) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

HBITMAP CropImage(HBITMAP __hBitmap,DWORD __dwBorderWidth,DWORD __dwBorderHeight)
{
	HBITMAP		_hBitmapNew, _hBmpOldNew, _hBmpOldld ;
	HDC			_hDC, _hDCMemNew, _hDCMemOld ;
	BITMAP		_Bitmap ;
	int			_iWidth, _iHeight ;

	if(__hBitmap == NULL)		return (NULL) ;

	_hDC = GetDC(NULL) ;
	if(_hDC == NULL)			return (NULL) ;

	_hDCMemNew = CreateCompatibleDC(NULL) ;
	if(_hDCMemNew == NULL)
	{
		ReleaseDC(NULL,_hDC) ;

		return (NULL) ;
	}

	_hDCMemOld = CreateCompatibleDC(NULL) ;
	if(_hDCMemOld == NULL)
	{
		DeleteDC(_hDCMemNew) ;
		ReleaseDC(NULL,_hDC) ;

		return (NULL) ;
	}

	GetObject(__hBitmap,sizeof(BITMAP),&_Bitmap) ;

	_iWidth = _Bitmap.bmWidth - (2 * __dwBorderWidth) ;
	_iHeight = _Bitmap.bmHeight - (2 * __dwBorderHeight) ;

	_hBmpOldld = SelectObject(_hDCMemOld,__hBitmap) ;
	if(_hBmpOldld == NULL)
	{
		ReleaseDC(NULL,_hDC) ;
		DeleteDC(_hDCMemNew) ;
		DeleteDC(_hDCMemOld) ;

		return (NULL) ;
	}

	_hBitmapNew = CreateCompatibleBitmap(_hDC,_iWidth,_iHeight) ;
	if(_hBitmapNew == NULL)
	{
		SelectObject(_hDCMemOld,_hBmpOldld) ;

		ReleaseDC(NULL,_hDC) ;
		DeleteDC(_hDCMemNew) ;
		DeleteDC(_hDCMemOld) ;

		return (NULL) ;
	}

	_hBmpOldNew = SelectObject(_hDCMemNew,_hBitmapNew) ;
	if(_hBmpOldNew == NULL)
	{
		SelectObject(_hDCMemOld,_hBmpOldld) ;
		DeleteObject(_hBitmapNew) ;

		ReleaseDC(NULL,_hDC) ;
		DeleteDC(_hDCMemNew) ;
		DeleteDC(_hDCMemOld) ;

		return (NULL) ;
	}

	BitBlt(_hDCMemNew,0,0,_iWidth,_iHeight,_hDCMemOld,0,__dwBorderHeight,SRCCOPY) ;

	SelectObject(_hDCMemNew,_hBmpOldNew) ;
	SelectObject(_hDCMemOld,_hBmpOldld) ;

	DeleteDC(_hDCMemNew) ;
	DeleteDC(_hDCMemOld) ;

	ReleaseDC(NULL,_hDC) ;

	return (_hBitmapNew) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPIMAGEINFOS LoadImageFromClipboard(DWORD __dwBorderWidth,DWORD __dwBorderHeight)
{
	register		LPIMAGEINFOS			_lpImageInfos ;

	GFL_BITMAP 				*_GflBitmap ;
	GFL_COLOR				_GflColor ;
	BITMAP					_Bitmap ;
	HBITMAP					_hImgBitmap ;
	HDC						_hDC ;

	_lpImageInfos = (LPIMAGEINFOS) PhR_Alloc_Memory(sizeof(IMAGEINFOS)) ;
	if(_lpImageInfos)
	{
		_lpImageInfos->StructureSize = sizeof(IMAGEINFOS) ;

		if(gflImportFromClipboard(&_GflBitmap) == GFL_NO_ERROR)
		{
			RtlSecureZeroMemory(&_GflColor,sizeof(GFL_COLOR)) ;

			if(gflResizeCanvas(_GflBitmap,NULL,_GflBitmap->Width + __dwBorderWidth,_GflBitmap->Height + __dwBorderHeight,GFL_CANVASRESIZE_CENTER,&_GflColor) == GFL_NO_ERROR)
			{
				if(gflConvertBitmapIntoDDB(_GflBitmap,&_hImgBitmap) == GFL_NO_ERROR)
				{
					_lpImageInfos->Image.dwFormatIndex = 0 ;

					if(_hImgBitmap)
					{
						_hDC = GetDC(NULL) ;
						if(_hDC)
						{
							if(GetObject(_hImgBitmap,sizeof(BITMAP),&_Bitmap))
							{
								_lpImageInfos->Image.iPixelSize = _Bitmap.bmBitsPixel >> 3 ;
								_lpImageInfos->Image.iScanlineSize = (_lpImageInfos->Image.iPixelSize * _Bitmap.bmWidth + 3) & ~3 ;
								_lpImageInfos->Image.iBitmapSize = _Bitmap.bmHeight * _lpImageInfos->Image.iScanlineSize;

								_lpImageInfos->BitmapInfo.bmiHeader.biBitCount = 0 ;
								_lpImageInfos->BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ;
								_lpImageInfos->BitmapInfo.bmiHeader.biWidth = _Bitmap.bmWidth ;
								_lpImageInfos->BitmapInfo.bmiHeader.biHeight = _Bitmap.bmHeight ;
								_lpImageInfos->BitmapInfo.bmiHeader.biPlanes = _Bitmap.bmPlanes ;

								if(_lpImageInfos->BitmapInfo.bmiHeader.biSizeImage <= 0)
									_lpImageInfos->BitmapInfo.bmiHeader.biSizeImage = (unsigned long int) _lpImageInfos->Image.iBitmapSize ;

								if(GetDIBits(_hDC,_hImgBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,NULL,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
								{
									_lpImageInfos->dwMemBlockSize = (unsigned long int) _lpImageInfos->Image.iBitmapSize ;
									_lpImageInfos->dwMemBlockSize /= 512 ;
									_lpImageInfos->dwMemBlockSize *= 512 ;

									_lpImageInfos->lpOriginal = (LPBYTE) PhR_Alloc_Memory((DWORD) _lpImageInfos->Image.iBitmapSize) ;
									if(_lpImageInfos->lpOriginal)
									{
										_lpImageInfos->lpResult = (LPBYTE) PhR_Alloc_Memory((DWORD) _lpImageInfos->BitmapInfo.bmiHeader.biSizeImage) ;
										if(_lpImageInfos->lpResult)
										{
											_lpImageInfos->lpWork = (LPBYTE) PhR_Alloc_Memory((DWORD) _lpImageInfos->BitmapInfo.bmiHeader.biSizeImage) ;
											if(_lpImageInfos->lpWork)
											{
												if(GetDIBits(_hDC,_hImgBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpOriginal,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
												{
													if(GetDIBits(_hDC,_hImgBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpResult,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
													{
														if(GetDIBits(_hDC,_hImgBitmap,0,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpWork,(LPBITMAPINFO) &_lpImageInfos->BitmapInfo,DIB_RGB_COLORS))
														{
															if(_Bitmap.bmWidth > _Bitmap.bmHeight)
																_lpImageInfos->Image.dwSensImage = IMAGE_SENS_HORIZONTAL ;
															else
																if(_Bitmap.bmWidth == _Bitmap.bmHeight)
																	_lpImageInfos->Image.dwSensImage = IMAGE_SENS_CARRE ;
																else
																	_lpImageInfos->Image.dwSensImage = IMAGE_SENS_VERTICAL ;

															_lpImageInfos->Image.dRatio = _Bitmap.bmHeight / _Bitmap.bmWidth ;
															_lpImageInfos->Image.dwFileSize = 0 ;
															_lpImageInfos->hImgBitmap = _hImgBitmap ;
															_lpImageInfos->Image.lpszImageFile = lstrcpy(_lpImageInfos->Image.szImageFile,"CLP") ;

															ReleaseDC(NULL,_hDC) ;

															gflFreeBitmapData(_GflBitmap) ;
															gflFreeBitmap(_GflBitmap) ;

															return (_lpImageInfos) ;
														}
													}
												}

												PhR_Free_Memory(_lpImageInfos->lpWork) ;
											}

											PhR_Free_Memory(_lpImageInfos->lpResult) ;
										}

										PhR_Free_Memory(_lpImageInfos->lpOriginal) ;
									}
								}
							}

							ReleaseDC(NULL,_hDC) ;
						}

						DeleteObject(_hImgBitmap) ;
					}
				}
			}

			gflFreeBitmapData(_GflBitmap) ;
			gflFreeBitmap(_GflBitmap) ;
		}

		PhR_Free_Memory(_lpImageInfos) ;
	}

	return (NULL) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

HBITMAP ImageToBitmap(LPIMAGEINFOS __lpImage,LPBYTE __lpImageBits)
{
	register		HDC				_hDCMem1 ;
	register		HBITMAP			_hBmp1 ;
					HDC				_hDC ;
					HBITMAP			_hBmp1Old ;

	_hBmp1 = NULL ;

	if(!__lpImage)	return (NULL) ;

	_hDC = GetDC(NULL) ;
	if(_hDC)
	{
		_hDCMem1 = CreateCompatibleDC(_hDC) ;
		if(_hDCMem1)
		{
			SetStretchBltMode(_hDCMem1,COLORONCOLOR) ;

			_hBmp1 = CreateCompatibleBitmap(_hDC,__lpImage->BitmapInfo.bmiHeader.biWidth,__lpImage->BitmapInfo.bmiHeader.biHeight) ;
			if(_hBmp1)
			{
				_hBmp1Old = SelectObject(_hDCMem1,_hBmp1) ;
				if(_hBmp1Old)
				{
					if(StretchDIBits(_hDCMem1,0,0,__lpImage->BitmapInfo.bmiHeader.biWidth,__lpImage->BitmapInfo.bmiHeader.biHeight,0,0,__lpImage->BitmapInfo.bmiHeader.biWidth,__lpImage->BitmapInfo.bmiHeader.biHeight,__lpImageBits,(LPBITMAPINFO) &__lpImage->BitmapInfo,DIB_RGB_COLORS,SRCCOPY))
					{
						SelectObject(_hDCMem1,_hBmp1Old) ;
						DeleteDC(_hDCMem1) ;
						ReleaseDC(NULL,_hDC) ;
						return (_hBmp1) ;
					}

					SelectObject(_hDCMem1,_hBmp1Old) ;
				}

				DeleteObject(_hBmp1) ;
			}

			DeleteDC(_hDCMem1) ;
		}

		ReleaseDC(NULL,_hDC) ;
	}

	return (NULL) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPBYTE SuperCropImage(LPIMAGEINFOS __lpImage,LPBYTE __lpImageBits,LPDWORD __lpdwImageSize,DWORD __dwBorderWidth,DWORD __dwBorderHeight)
{
	HBITMAP		_hBitmap_1, _hBitmap_2 ;
	LPBYTE		_lpTmp ;

	_lpTmp = NULL ;

	if(__lpImage)
	{
		if(__lpImageBits)
		{
			_hBitmap_1 = ImageToBitmap(__lpImage,__lpImageBits) ;
			if(_hBitmap_1)
			{
				_hBitmap_2 = CropImage(_hBitmap_1,__dwBorderWidth,__dwBorderHeight) ;
				if(_hBitmap_2)
				{
					_lpTmp = GetBitsFromBitmap(_hBitmap_2,__lpdwImageSize,NULL) ;
				
					DeleteObject(_hBitmap_2) ;
				}

				DeleteObject(_hBitmap_1) ;
			}
		}
	}

	return (_lpTmp) ;
}

void Log_SetFileName(LPSTR __lpszLogFileName)
{
	lstrcpy(szLogFileName,__lpszLogFileName) ;

	return ;
}

LPSTR Log_GetFileName(void)
{
	return (szLogFileName) ;
}

void Log_Stop(void)
{
	Log("*** Log Stoped ***") ;

	bLog = FALSE ;

	return ;
}

void Log_Start(void)
{
	bLog = TRUE ;
	Log("*** Log Started ***") ;

	return ;
}

void Log(LPSTR __lpszTxt)
{
	char		_szTmp[4096] ;

	char		_szTime[256] ;
	char		_szDate[256] ;

	SYSTEMTIME	_St ;

	DWORD		_dwTmp ;
	int			_iLen ;

	if(!hLogFile)	return ;
	if(!bLog)		return ;

	GetLocalTime(&_St) ;
	GetTimeFormat(LOCALE_USER_DEFAULT,TIME_FORCE24HOURFORMAT,&_St,NULL,_szTime,sizeof(_szTime)) ;
	GetDateFormat(LOCALE_USER_DEFAULT,DATE_LONGDATE,&_St,NULL,_szDate,sizeof(_szDate)) ;

	_iLen = wsprintf(_szTmp,"[%s %s] : %s\n",_szDate,_szTime,__lpszTxt) ;
	WriteFile(hLogFile,_szTmp,_iLen,&_dwTmp,NULL) ;

	return ;
}

void Log_Init(void)
{
	hLogFile = CreateFile(szLogFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL) ;
	if(hLogFile == INVALID_HANDLE_VALUE)
	{
		hLogFile = NULL ;
		return ;
	}

	if(hLogFile)	Log_Start() ;
	if(hLogFile)	Log("Openning Log") ;
	if(hLogFile)	Log("Log Ready") ;

	return ;
}

void Log_Exit(void)
{
	if(!hLogFile)	return ;

	if(!bLog)	Log_Start() ;
	Log("Closing Log file") ;
	Log_Stop() ;
	FlushFileBuffers(hLogFile) ;
	CloseHandle(hLogFile) ;
	hLogFile = NULL ;

	return ;
}

HWND CreateEditorToolBar(HWND __hWnd)
{
	register	HWND 	_hWndToolbar ;
	register	int		_i ;

	TBBUTTON	_Tb[MAX_EDITOR_BUTTONS] ;

	memset(&_Tb,0,sizeof(_Tb)) ;

	for(_i = 0 ; _i < MAX_EDITOR_BUTTONS ; _i++)
	{
		_Tb[_i].fsState = TBSTATE_ENABLED ;
		_Tb[_i].fsStyle = BTNS_AUTOSIZE ;
		_Tb[_i].dwData = 1000 + _i ;
	}

	_Tb[0].iBitmap = 1 ;
	_Tb[0].idCommand = BTN_INSERT_COL_LEFT ;

	_Tb[1].iBitmap = 0 ;
	_Tb[1].idCommand = BTN_INSERT_COL_RIGHT ;

	_Tb[2].fsStyle = TBSTYLE_SEP ;
	_Tb[2].dwData = 0 ;

	_Tb[3].iBitmap = 2 ;
	_Tb[3].idCommand = BTN_INSERT_ROW_ABOVE ;

	_Tb[4].iBitmap = 3 ;
	_Tb[4].idCommand = BTN_INSERT_ROW_BELOW ;

	_Tb[5].fsStyle = TBSTYLE_SEP ;
	_Tb[5].dwData = 0 ;

	_Tb[6].iBitmap = 4 ;
	_Tb[6].idCommand = BTN_DELETE_COL ;

	_Tb[7].iBitmap = 5 ;
	_Tb[7].idCommand = BTN_DELETE_ROW ;

	_Tb[8].fsStyle = TBSTYLE_SEP ;
	_Tb[8].dwData = 0 ;

	_Tb[9].iBitmap = 6 ;
	_Tb[9].idCommand = BTN_MIRROR_HORIZONTAL ;

	_Tb[10].iBitmap = 7 ;
	_Tb[10].idCommand = BTN_MIRROR_VERTICAL ;

	_Tb[11].fsStyle = TBSTYLE_SEP ;
	_Tb[11].dwData = 0 ;

	_Tb[12].iBitmap = 8 ;
	_Tb[12].idCommand = BTN_CIRULAR_ROTATE_TOP ;

	_Tb[13].iBitmap = 9 ;
	_Tb[13].idCommand = BTN_CIRULAR_ROTATE_BOTTOM ;

	_Tb[14].fsStyle = TBSTYLE_SEP ;
	_Tb[14].dwData = 0 ;

	_Tb[15].iBitmap = 10 ;
	_Tb[15].idCommand = BTN_CIRULAR_ROTATE_LEFT ;

	_Tb[16].iBitmap = 11 ;
	_Tb[16].idCommand = BTN_CIRULAR_ROTATE_RIGHT ;

	_Tb[17].dwData = 0 ;
	_Tb[17].fsStyle = TBSTYLE_SEP ;

	_Tb[18].iBitmap = 12 ;
	_Tb[18].idCommand = BTN_SHIFT_TOP ;

	_Tb[19].iBitmap = 13 ;
	_Tb[19].idCommand = BTN_SHIFT_BOTTOM ;

	_Tb[20].fsStyle = TBSTYLE_SEP ;
	_Tb[20].dwData = 0 ;

	_Tb[21].iBitmap = 14 ;
	_Tb[21].idCommand = BTN_SHIFT_LEFT ;

	_Tb[22].iBitmap = 15 ;
	_Tb[22].idCommand = BTN_SHIFT_RIGHT ;

	_Tb[23].fsStyle = TBSTYLE_SEP ;
	_Tb[23].dwData = 0 ;

	_Tb[24].iBitmap = 16 ;
	_Tb[24].idCommand = BTN_ROTATE_LEFT ;

	_Tb[25].iBitmap = 17 ;
	_Tb[25].idCommand = BTN_ROTATE_RIGHT ;

	_hWndToolbar = CreateWindowEx(	0,TOOLBARCLASSNAME,NULL,
									WS_CHILD|TBSTYLE_WRAPABLE|TBSTYLE_TOOLTIPS|TBSTYLE_TRANSPARENT|TBSTYLE_FLAT,
									0,0,0,0,__hWnd,NULL,hInstance,NULL) ;
	if (_hWndToolbar == NULL)
		return (NULL) ;

	hImageListEditor = ImageList_LoadImage(hInstance,(LPCSTR) BMP_EDITOR,32,0,CLR_DEFAULT,IMAGE_BITMAP,LR_CREATEDIBSECTION) ;
	SendMessage(_hWndToolbar,TB_SETIMAGELIST,0,(LPARAM) hImageListEditor) ;

	SendMessage(_hWndToolbar,TB_SETBITMAPSIZE,0,MAKELPARAM(32,32)) ;
	SendMessage(_hWndToolbar,TB_SETBUTTONSIZE,0,MAKELPARAM(34,34)) ;
	SendMessage(_hWndToolbar,TB_SETBUTTONWIDTH,MAKELPARAM(34,34),0) ;
	SendMessage(_hWndToolbar,TB_SETEXTENDEDSTYLE,0,TBSTYLE_EX_DOUBLEBUFFER|TBSTYLE_EX_MIXEDBUTTONS) ;
	SendMessage(_hWndToolbar,TB_BUTTONSTRUCTSIZE,(WPARAM) sizeof(TBBUTTON),0) ;
	SendMessage(_hWndToolbar,TB_ADDBUTTONS,MAX_EDITOR_BUTTONS,(LPARAM) &_Tb) ;
	SendMessage(_hWndToolbar,TB_SETMAXTEXTROWS,128,0) ;
	SendMessage(_hWndToolbar,TB_AUTOSIZE,0,0) ;

	ShowWindow(_hWndToolbar,TRUE) ;

	return (_hWndToolbar) ;
}

LRESULT CALLBACK AboutDlgProc(HWND __hWnd,UINT __uMsg,WPARAM __wParam,LPARAM __lParam)
{
	__lParam = __lParam ;

	switch (__uMsg)
	{
		case	WM_CTLCOLORDLG :	return ((LRESULT) hBarbieBrush) ;

 		case	WM_CTLCOLORSTATIC :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorStatic) ;
									SetBkColor((HDC) __wParam,lpCurrentTheme->CtlColorDlg) ;
									return ((LRESULT) hBarbieBrush) ;

		case	WM_CTLCOLORBTN :	SetTextColor((HDC) __wParam,lpCurrentTheme->CtlColorBtn);
									SetBkColor((HDC) __wParam,lpCurrentTheme->CtlColorDlg);
									return ((LRESULT) hBarbieBrush) ;

		case	WM_INITDIALOG :		SetWindowTheme(GetDlgItem(__hWnd,IDOK), L"explorer",NULL) ;
									return (TRUE) ;

		case	WM_COMMAND :		switch(LOWORD(__wParam))
									{
										case	IDOK :
										case	IDCANCEL :
															EndDialog(__hWnd,TRUE) ;
															return(TRUE) ;
									}

									break ;
	}

	return(FALSE) ;
}


void PaintDlgModless(HWND __hWnd)
{
	HDC			_hDC, _hDCMem1, _hDCMem2 ;
	HBITMAP		_hBitmap, _hBmpSrc, _hBmpOld1, _hBmpOld2 ;
	RECT		_rcClient ;
	PAINTSTRUCT	_Ps ;
	BITMAP		_Bitmap ;

	_hDC = BeginPaint(__hWnd,&_Ps) ;
	if(_hDC)
	{
		GetClientRect(__hWnd,&_rcClient) ;

		_hDCMem1 = CreateCompatibleDC(_hDC) ;
		if(_hDCMem1)
		{
			_hBitmap = CreateCompatibleBitmap(_hDC,_rcClient.right,_rcClient.bottom) ;
			if(_hBitmap)
			{
				_hBmpOld1 = SelectObject(_hDCMem1,_hBitmap) ;
				if(_hBmpOld1)
				{
					FillRect(_hDCMem1,&_rcClient,GetStockObject(WHITE_BRUSH)) ;

					_hDCMem2 = CreateCompatibleDC(_hDC) ;
					if(_hDCMem2)
					{
						_hBmpSrc = LoadBitmap(hInstance,(LPSTR) BMP_GRINCHEUX) ;
						if(_hBmpSrc)
						{
							GetObject(_hBmpSrc,sizeof(BITMAP),&_Bitmap) ;

							_hBmpOld2 = SelectObject(_hDCMem2,_hBmpSrc) ;
							if(_hBmpOld2)
							{
								SetStretchBltMode(_hDCMem1,STRETCH_HALFTONE) ;
								SetStretchBltMode(_hDCMem2,STRETCH_HALFTONE) ;

								BitBlt(_hDCMem1,(_rcClient.right - _Bitmap.bmWidth) / 2,0,_Bitmap.bmWidth,_Bitmap.bmHeight,_hDCMem2,0,0,SRCCOPY) ;
								SelectObject(_hDCMem2,_hBmpOld2) ;
							}

							DeleteObject(_hBmpSrc) ;
						}

						DeleteDC(_hDCMem2) ;
					}

					BitBlt(_hDC,0,0,_rcClient.right,_rcClient.bottom,_hDCMem1,0,0,SRCCOPY) ;
					SelectObject(_hDCMem1,_hBmpOld1) ;
				}

				DeleteObject(_hBitmap) ;
			}

			DeleteDC(_hDCMem1) ;
		}
	}

	EndPaint(__hWnd,&_Ps) ;

	return ;
}

LRESULT CALLBACK DlgModlessProc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam)
{
	__lParam = __lParam ;

	switch(__Message)
	{
		case	WM_PAINT :			PaintDlgModless(__hWnd) ;
									break ;

		case	WM_CTLCOLORSTATIC :	SetTextColor((HDC) __wParam,RGB(255,255,0));
									SetBkColor((HDC) __wParam,RGB(0,0,0));
									return ((LRESULT) GetStockObject(BLACK_BRUSH)) ;

		case	WM_CTLCOLORBTN :	SetTextColor((HDC) __wParam,RGB(255,0,255));
									SetBkColor((HDC) __wParam,RGB(0,0,0));
									return ((LRESULT) GetStockObject(BLACK_BRUSH)) ;

		case	WM_CTLCOLORDLG :	SetTextColor((HDC) __wParam,RGB(0,0,255));
									SetBkColor((HDC) __wParam,RGB(0,0,0));
									return ((LRESULT) GetStockObject(BLACK_BRUSH)) ;

		case	WM_COMMAND :		switch(LOWORD(__wParam))
									{
										case	IDM_SET_TEXT_01 :	SetDlgItemText(__hWnd,IDM_SET_TEXT_01,szDlgStatic_01) ;
																	break ;

										case	IDM_SET_TEXT_02 :	SetDlgItemText(__hWnd,IDM_SET_TEXT_02,szDlgStatic_02) ;
																	break ;
									}

									break ;

		default :					return (FALSE) ;
	}

	return (TRUE) ;
}

void PaintDlgWaiting(HWND __hWnd)
{
	HDC			_hDC, _hDCMem1 ;
	HBITMAP		_hBitmap, _hBmpOld1 ;
	RECT		_rcClient ;
	PAINTSTRUCT	_Ps ;

	_hDC = BeginPaint(__hWnd,&_Ps) ;
	if(_hDC)
	{
		GetClientRect(__hWnd,&_rcClient) ;

		_hDCMem1 = CreateCompatibleDC(_hDC) ;
		if(_hDCMem1)
		{
			_hBitmap = CreateCompatibleBitmap(_hDC,_rcClient.right,_rcClient.bottom) ;
			if(_hBitmap)
			{
				_hBmpOld1 = SelectObject(_hDCMem1,_hBitmap) ;
				if(_hBmpOld1)
				{
					FillRect(_hDCMem1,&_rcClient,GetStockObject(BLACK_BRUSH)) ;

					SetStretchBltMode(_hDCMem1,STRETCH_HALFTONE) ;
					SetStretchBltMode(_hDC,STRETCH_HALFTONE) ;

					BitBlt(_hDC,0,0,_rcClient.right,_rcClient.bottom,_hDCMem1,0,0,SRCCOPY) ;
					SelectObject(_hDCMem1,_hBmpOld1) ;
				}

				DeleteObject(_hBitmap) ;
			}

			DeleteDC(_hDCMem1) ;
		}
	}

	EndPaint(__hWnd,&_Ps) ;

	return ;
}

LRESULT CALLBACK DlgWaitingProc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam)
{
	__lParam = __lParam ;

	switch(__Message)
	{
		case	WM_PAINT :			PaintDlgWaiting(__hWnd) ;
									break ;

		case	WM_CTLCOLORSTATIC :	SetTextColor((HDC) __wParam,RGB(255,0,0));
									SetBkColor((HDC) __wParam,RGB(0,0,0));
									return ((LRESULT) GetStockObject(BLACK_BRUSH)) ;

		case	WM_CTLCOLORBTN :	SetTextColor((HDC) __wParam,RGB(255,0,255));
									SetBkColor((HDC) __wParam,RGB(0,0,0));
									return ((LRESULT) GetStockObject(BLACK_BRUSH)) ;

		case	WM_CTLCOLORLISTBOX :SetTextColor((HDC) __wParam,RGB(255,0,0));
									SetBkColor((HDC) __wParam,RGB(0,0,0));
									return ((LRESULT) GetStockObject(BLACK_BRUSH)) ;

		case	WM_CTLCOLOREDIT :	SetTextColor((HDC) __wParam,RGB(0,255,255));
									SetBkColor((HDC) __wParam,RGB(0,0,0));
									return ((LRESULT) GetStockObject(BLACK_BRUSH)) ;

		case	WM_CTLCOLORDLG :	SetTextColor((HDC) __wParam,RGB(0,0,255));
									SetBkColor((HDC) __wParam,RGB(0,0,0));
									return ((LRESULT) GetStockObject(BLACK_BRUSH)) ;

		default :					return (FALSE) ;
	}

	return (TRUE) ;
}

int CreateNewToolbar(HWND __hWnd)
{
	register	HWND 		_hButton ;
	register	int			_i ;
				int			_k, _j ;

				HWND		_hWndTip ;

				TOOLINFO	_ToolInfo ;

	_k = TLB_BTN_01_OPEN ;
	_j = 0 ;

	memset(&_ToolInfo,0,sizeof(_ToolInfo)) ;

	_ToolInfo.cbSize = sizeof(_ToolInfo) ;
	_ToolInfo.hwnd = __hWnd ;
	_ToolInfo.uFlags = TTF_IDISHWND|TTF_SUBCLASS|TTF_CENTERTIP ;

	for(_i = 0 ; _i < MAX_TOOLBAR_BUTTONS ; _i++)
	{
		_hButton = CreateWindowEx(	0,"BUTTON",NULL,
									WS_CHILD|WS_VISIBLE|BS_OWNERDRAW|BS_NOTIFY|BS_PUSHBUTTON,
									_j,0,34,34,__hWnd,(HMENU) _k,hInstance,NULL) ;
		if(_hButton)
		{
			_hWndTip = CreateWindowEx(0,TOOLTIPS_CLASS,NULL,WS_POPUP|TTS_ALWAYSTIP|TTS_BALLOON|TTS_NOPREFIX,
                              CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
                              __hWnd,0,hInstance,NULL) ;
			if(_hWndTip)
			{
				SetWindowPos(_hWndTip,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE) ;

				_ToolInfo.uId = (UINT_PTR) _hButton ;

				if(_i == (MAX_TOOLBAR_BUTTONS - 1))
				{
					_ToolInfo.lpszText = ToolTips(21) ;
					_ToolInfo.lParam = TLB_BTN_25_DIAPORAMA_ON ;
				}
				else
				{
					_ToolInfo.lpszText = ToolTips(_i + 1) ;
					_ToolInfo.lParam = _k ;
				}

				SendMessage(_hWndTip,TTM_ADDTOOL,0,(LPARAM) &_ToolInfo) ;
				SendMessage(_hWndTip,TTM_SETMAXTIPWIDTH,0,256) ;
				SendMessage(_hWndTip,TTM_SETDELAYTIME,TTDT_AUTOPOP,5000) ;
				SendMessage(_hWndTip,TTM_SETTITLE,TTI_INFO,(LPARAM) "Information") ;
			}
		}

		_k++ ;
		_j += 34 ;
	}

	return (TRUE) ;
}

