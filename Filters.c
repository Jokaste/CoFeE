#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <CommCtrl.h>
#include <commdlg.h>
#include <Shlobj.h>
#include <ShellApi.h>
#include <math.h>
#include <shlwapi.h>

#pragma warn(disable:2805)

#include "zeegrid.h"
#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Languages.h"
#include "Ressources.h"

#pragma warn(disable:2228)

LPSTR CreateKeccak(LPPGM_FILTERS __lpFilter)
{
	int		_iModele, _iRecordNumber ;
	int		_iStarRecordNumber, _iRate ;
	char	_szResult[256] ;
	char	_szStarName[MAX_STAR_NAME_LENGTH + 4] ;
	char	_szComment[MAX_COMMENT_LENGTH + 4] ;

	if(__lpFilter->Divisor == 0)
		__lpFilter->Divisor = 1 ;

	_iModele = __lpFilter->Model ;
	_iRecordNumber = __lpFilter->FilterRecordNumber ;
	_iStarRecordNumber = __lpFilter->StarRecordNumber ;
	_iRate = __lpFilter->Rate ;

	__lpFilter->FilterRecordNumber = 0 ;
	__lpFilter->Model = 0 ;
	__lpFilter->StarRecordNumber = 0 ;
	__lpFilter->Rate = 0 ;

	lstrcpy(_szStarName,__lpFilter->StarName) ;
	lstrcpy(_szComment,__lpFilter->Comment) ;

	RtlSecureZeroMemory(__lpFilter->Keccak_Filter,MAX_KECCAK_LENGTH) ;
	RtlSecureZeroMemory(__lpFilter->StarName,MAX_STAR_NAME_LENGTH) ;
	RtlSecureZeroMemory(__lpFilter->Comment,MAX_COMMENT_LENGTH) ;

	Keccak((unsigned char *) __lpFilter,sizeof(PGM_FILTERS),(unsigned char *) _szResult) ;
	Hex2Str((unsigned char *) _szResult,(unsigned char *) __lpFilter->Keccak_Filter,256 / 8) ;

	__lpFilter->Model = _iModele ;
	__lpFilter->FilterRecordNumber = _iRecordNumber ;
	__lpFilter->StarRecordNumber = _iStarRecordNumber ;
	__lpFilter->Rate = _iRate ;

	lstrcpy(__lpFilter->StarName,_szStarName) ;
	lstrcpy(__lpFilter->Comment,_szComment) ;

	return (__lpFilter->Keccak_Filter) ;
}

int IsThisKeccackPresentInDatabase(LPSTR __lpszKeccak_Filter)
{
	HSTMT			_hFind ;
	int				_iResultat, _iRes ;
	char			_szTmp[1024] ;

	if(!__lpszKeccak_Filter)		return (FALSE) ;

	_iResultat = FALSE ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"SELECT [FilterRecordNumber] FROM Filters WHERE [Keccak_Filter] LIKE \"%s\";",__lpszKeccak_Filter) ;

	if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,NULL) != SQLITE_OK)
	{
		sqlite3_reset(_hFind) ;
		return (Error(52)) ;
	}

	_iRes = sqlite3_step(_hFind) ;
	switch(_iRes)
	{
		case	SQLITE_ROW :	_iResultat = sqlite3_column_int(_hFind,0) ;	break ;
		case	SQLITE_DONE :	_iResultat = 0 ;							break ;
		default :				SQLite_Error() ; _iResultat = -1 ;			break ;
	}

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	return (_iResultat) ;
}

int QuickSave(DWORD __iImageWidth,DWORD __iImageHeight,LPBYTE __lpImageBits,LPSTR __lpszFileName)
{
	GFL_BITMAP		*GflBitmap ;
	GFL_SAVE_PARAMS	_Params ;
	HBITMAP			_hBitmap ;

	_hBitmap = SetBitsToBitmap(__iImageWidth,__iImageHeight,__lpImageBits) ;
	if(_hBitmap)
	{
		if(gflConvertDDBIntoBitmap(_hBitmap,&GflBitmap) == GFL_NO_ERROR)
		{
			gflGetDefaultSaveParams(&_Params) ;

			_Params.Flags = GFL_SAVE_REPLACE_EXTENSION|GFL_SAVE_ANYWAY ;
			_Params.FormatIndex = gflGetFormatIndexByName("jpeg") ; ;
			_Params.Compression = GFL_NO_COMPRESSION ;
			_Params.Quality = 100 ;
			_Params.Progressive  = TRUE ;
			_Params.OptimizeHuffmanTable = TRUE ;

			if(gflSaveBitmap(__lpszFileName,GflBitmap,&_Params) != GFL_NO_ERROR)
			{
				DeleteObject(_hBitmap) ;

				gflFreeBitmapData(GflBitmap) ;
				gflFreeBitmap(GflBitmap) ;

				return (Error(20)) ;
			}

			gflFreeBitmapData(GflBitmap) ;
			gflFreeBitmap(GflBitmap) ;
		}

		DeleteObject(_hBitmap) ;

		return (TRUE) ;
	}

	return (FALSE) ;
}

int ComputeSimilarBytes(void)
{
				LPIMAGEINFOS	_lpImageInfos ;
				int				_i, _iOriginal, _iResult, _iCount ;
				LPDWORD			_lpOriginal ;
				LPDWORD			_lpResult, _lpTmpResult, _lpTmpOriginal ;
				DWORD			_dwOriginal, _dwResult, _dwAdd10, _dwSub10 ;
				DWORD			_dwOriginalSize, _dwResultSize ;

	_dwResult = FALSE ;
	_dwOriginalSize = _dwResultSize = 0 ;
	_lpOriginal = _lpResult = NULL ;

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(_lpImageInfos)
	{
		_lpOriginal = (LPDWORD) SuperCropImage(_lpImageInfos,_lpImageInfos->lpOriginal,&_dwOriginalSize,MAX_EDIT_COLUMNS,MAX_EDIT_ROWS) ;
		if(_lpOriginal)
		{
			_lpTmpOriginal = _lpOriginal ;

			_lpResult = (LPDWORD) SuperCropImage(_lpImageInfos,_lpImageInfos->lpResult,&_dwResultSize,MAX_EDIT_COLUMNS,MAX_EDIT_ROWS) ;
			if(_lpResult)
			{
				_lpTmpResult = _lpResult ;

//				QuickSave(_lpImageInfos->BitmapInfo.bmiHeader.biWidth - 58,_lpImageInfos->BitmapInfo.bmiHeader.biHeight - 58,(LPBYTE) _lpOriginal,".\\Original.jpg") ;
//				QuickSave(_lpImageInfos->BitmapInfo.bmiHeader.biWidth - 58,_lpImageInfos->BitmapInfo.bmiHeader.biHeight - 58,(LPBYTE) _lpResult,".\\Result.jpg") ;

				_iOriginal = _iCount = (int) _dwOriginalSize >> 2 ;

				_iResult = 0 ;

				for(_i = 0 ; _i < _iCount ; _i++)
				{
					_dwOriginal = *_lpTmpOriginal++ ;
					_dwOriginal &= 0x00FFFFFF ;			// Remove alpha channel

					_dwResult = *_lpTmpResult++ ;
					_dwResult &= 0x00FFFFFF ;			// Remove alpha channel

					_dwAdd10 = _dwSub10 = _dwResult * 0.10 ;
					_dwSub10 = _dwResult - _dwSub10 ;
					_dwAdd10 = _dwResult + _dwAdd10 ;

//			*****************************************************
//			*** See if colors are within +/-10% of each other ***
//			*****************************************************

					if((_dwOriginal <= _dwAdd10) && (_dwOriginal >= _dwSub10))
						_iResult++ ;
				}

				SetSimilarity(100.0 * ((double) _iResult) / ((double) _iOriginal)) ;

				_dwResult = TRUE ;

				PhR_Free_Memory(_lpResult) ;
			}

			PhR_Free_Memory(_lpOriginal) ;
		}
	}

	return (_dwResult) ;
}

int Convolve_64_Bias_0(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1 ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 6 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 6 ;
								_b = _b >> 6 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 6 ;
			_Green >>= 6 ;
			_Blue >>= 6 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_64(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1, _iBias ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBias = lpCurrentFilter->Bias ;
	if(_iBias == 0)
		return (Convolve_64_Bias_0(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 6 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 6 ;
								_b = _b >> 6 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					if(_iBias)
					{
						_r += _iBias ;
						_g += _iBias ;
						_b += _iBias ;
					}

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 6 ;
			_Green >>= 6 ;
			_Blue >>= 6 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_32_Bias_0(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1 ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 5 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 5 ;
								_b = _b >> 5 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 5 ;
			_Green >>= 5 ;
			_Blue >>= 5 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_32(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1, _iBias ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBias = lpCurrentFilter->Bias ;
	if(_iBias == 0)
		return (Convolve_32_Bias_0(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;


	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 5 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 5 ;
								_b = _b >> 5 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					if(_iBias)
					{
						_r += _iBias ;
						_g += _iBias ;
						_b += _iBias ;
					}

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 5 ;
			_Green >>= 5 ;
			_Blue >>= 5 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_16_Bias_0(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1 ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 4 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 4 ;
								_b = _b >> 4 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 4 ;
			_Green >>= 4 ;
			_Blue >>= 4 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_16(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1, _iBias ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBias = lpCurrentFilter->Bias ;
	if(_iBias == 0)
		return (Convolve_16_Bias_0(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;


	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 4 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 4 ;
								_b = _b >> 4 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					if(_iBias)
					{
						_r += _iBias ;
						_g += _iBias ;
						_b += _iBias ;
					}

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 4 ;
			_Green >>= 4 ;
			_Blue >>= 4 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_8_Bias_0(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1 ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 3 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 3 ;
								_b = _b >> 3 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 3 ;
			_Green >>= 3 ;
			_Blue >>= 3 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_8(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1, _iBias ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBias = lpCurrentFilter->Bias ;
	if(_iBias == 0)
		return (Convolve_8_Bias_0(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;


	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 3 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 3 ;
								_b = _b >> 3 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					if(_iBias)
					{
						_r += _iBias ;
						_g += _iBias ;
						_b += _iBias ;
					}

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 3 ;
			_Green >>= 3 ;
			_Blue >>= 3 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_4_Bias_0(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1 ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 2 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 2 ;
								_b = _b >> 2 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 2 ;
			_Green >>= 2 ;
			_Blue >>= 2 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_4(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1, _iBias ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBias = lpCurrentFilter->Bias ;
	if(_iBias == 0)
		return (Convolve_4_Bias_0(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 2 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 2 ;
								_b = _b >> 2 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					if(_iBias)
					{
						_r += _iBias ;
						_g += _iBias ;
						_b += _iBias ;
					}

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 2 ;
			_Green >>= 2 ;
			_Blue >>= 2 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_2_Bias_0(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1 ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 1 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 1 ;
								_b = _b >> 1;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 1 ;
			_Green >>= 1 ;
			_Blue >>= 1 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_2(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1, _iBias ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBias = lpCurrentFilter->Bias ;
	if(_iBias == 0)
		return (Convolve_2_Bias_0(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r = _r >> 1 ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g = _g >> 1 ;
								_b = _b >> 1 ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					if(_iBias)
					{
						_r += _iBias ;
						_g += _iBias ;
						_b += _iBias ;
					}

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red >>= 1 ;
			_Green >>= 1 ;
			_Blue >>= 1 ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_1_Bias_0(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1 ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve_1(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1, _iBias ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_iBias = lpCurrentFilter->Bias ;
	if(_iBias == 0)
		return (Convolve_1_Bias_0(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					if(_iBias)
					{
						_r += _iBias ;
						_g += _iBias ;
						_b += _iBias ;
					}

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int Convolve(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre)
{
	register	int		_iMilieu ;
	register	LPDWORD	_lpTmp ;

	int		_Red, _Green, _Blue, _Rgb ;
	int		_r, _g, _b ;
	int		_v, _f ;
	int		_K1, _K2, _K3, _K4, _K7, _K8 ;
	int		_x1, _y1, _iBias, _iDivisor ;
	int		_x, _y, _i, _Ii, _j, _Jj ;
	int		_iBorneInferieure, _iBorneSuperieure ;

	if(!__lpFiltre)										return (FALSE) ;
	if(!__lpImageBits)									return (FALSE) ;

	if(__iImageWidth < 60)								return (FALSE) ;
	if(__iImageHeight < 60)								return (FALSE) ;

	_iDivisor = lpCurrentFilter->Divisor ;
	_iBias = lpCurrentFilter->Bias ;

	switch(_iDivisor)
	{
		case	0 :		_iDivisor = 1 ;
						lpCurrentFilter->Divisor = 1 ;
		case	1 :		return (Convolve_1(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;
		case	2 :		return (Convolve_2(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;
		case	4 :		return (Convolve_4(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;
		case	8 :		return (Convolve_8(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;
		case	16 :	return (Convolve_16(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;
		case	32 :	return (Convolve_32(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;
		case	64 :	return (Convolve_64(__iImageWidth,__iImageHeight,__lpImageBits,__lpFiltre)) ;
	}

	_iBorneSuperieure = _iMilieu = __lpFiltre->ColsNumber >> 1 ;
	_iBorneInferieure = -_iMilieu ;

	_lpTmp = (LPDWORD) __lpImageBits ;

	_K7 = __iImageWidth  - __lpFiltre->ColsNumber - 1 ;
	_K8 = __iImageHeight - __lpFiltre->ColsNumber - 1 ;

	for(_x = 1 ; _x < _K7 ; _x++)
	{
		_x1 = _x - 1 ;
		_y1 = __iImageWidth ;

		for(_y = 1 ; _y < _K8 ; _y++)
		{
			_y1 += __iImageWidth ;
			_Ii = _iBorneInferieure ;

			_Red = _Green = _Blue = _Rgb = 0 ;

			for(_i = 0 ; _i < __lpFiltre->ColsNumber ; _i++, _Ii++)
			{
				_Jj = _iBorneInferieure ;

				_K2 = _iMilieu + _Ii ;
				_K4 = _y1 + (_K2 * __iImageWidth) ;

				for(_j = 0 ; _j < __lpFiltre->ColsNumber ; _j++, _Jj++)
				{
					_K1 = _iMilieu + _Jj ;
					_K3 = _x1 + _K1 + _K4 ;

					_v = *(_lpTmp + _K3) ;
					if(_v)
					{
						_r = _g = _b = _v ;

						_r = _r >> 16 ;
						_g = _g >> 8 ;

						_r &= 255 ;
						_g &= 255 ;
						_b &= 255 ;

						if(__lpFiltre->Grey & GREY_INPUT)
							_r = _g = _b = TabDiv3[_r + _g + _b] ;

						_f = __lpFiltre->Matrix[(__lpFiltre->ColsNumber * _K1) + _K2] ;
						if(_f)
						{
							if(_f != 1)
							{
								_r *= _f ;
								_g *= _f ;
								_b *= _f ;
							}

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}

							_r /= _iDivisor ;

							if(__lpFiltre->Grey & GREY_INPUT)
							{
								_g = _b = _r ;
							}
							else
							{
								_g /= _iDivisor ;
								_b /= _iDivisor ;
							}
						}
						else
							_r = _g = _b = 0 ;
					}
					else
						_r = _g = _b = 0 ;

					if(_iBias)
					{
						_r += _iBias ;
						_g += _iBias ;
						_b += _iBias ;
					}

					_Red += _r ;
					_Green += _g ;
					_Blue += _b ;
				}
			}

			_Red /= _iDivisor ;
			_Green /= _iDivisor ;
			_Blue /= _iDivisor ;

			if(_Red)
			{
				if(_Red > 255)		_Red = 255 ;
				if(_Red < 0)		_Red = 0 ;
			}

			if(_Green)
			{
				if(_Green > 255)	_Green = 255 ;
				if(_Green < 0)		_Green = 0 ;
			}

			if(_Blue)
			{
				if(_Blue > 255)		_Blue = 255 ;
				if(_Blue < 0)		_Blue = 0 ;
			}

			if(__lpFiltre->Grey & GREY_INPUT)
				_Green = _Blue = _Red ;

			if(__lpFiltre->Grey & GREY_OUTPUT)
			{
				_Red = TabDiv3[_Red + _Green + _Blue] ;
				_Rgb = (_Red << 16) | (_Red << 8) | _Red ;
			}
			else
				_Rgb = (_Red << 16) | (_Green << 8) | _Blue ;

			*(_lpTmp + _x1 + _y1) = _Rgb ;
		}
	}

	ComputeSimilarBytes() ;

	return (TRUE) ;
}

int SetFiltersToListBox(void)
{
	register		LPPGM_FILTERS	_lpFilter ;
	HSTMT			_hFind ;
	HWND			_hListBox, _hWorkSpace ;
	LPSTR			_lpszTmp ;
	PGM_FILTERS		_Filter ;
	int				_iIndex, _iValue, _iColsNumber, _iCount ;
	char			_szTmp[1024] ;

	_lpFilter = &_Filter ;
	_hFind = NULL ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	_iIndex = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETCURSEL,0,0) ;
	if(_iIndex == CB_ERR)
		return (FALSE) ;

	_iColsNumber = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETITEMDATA,_iIndex,0) ;

	_hListBox = GetDlgItem(_hWorkSpace,LISTBOX_MODELS) ;

	SendMessage(_hListBox,LB_RESETCONTENT,0,0) ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"SELECT [FilterRecordNumber],[StarName] FROM Filters WHERE [ColsNumber] = %d ORDER BY [StarName];",_iColsNumber) ;
	if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,NULL) != SQLITE_OK)
		return (Error(52)) ;

	while(sqlite3_step(_hFind) == SQLITE_ROW)
	{
		_iValue = sqlite3_column_int(_hFind,0) ;
		_lpszTmp = (LPSTR) sqlite3_column_text(_hFind,1) ;
		if(_lpszTmp)
		{
			_iIndex = (int) SendMessage(_hListBox,LB_ADDSTRING,0,(LPARAM) _lpszTmp) ;
			SendMessage(_hListBox,LB_SETITEMDATA,_iIndex,(LPARAM) _iValue) ;
		}
	}

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	SendMessage(_hListBox,LB_SETCURSEL,0,0) ;

	SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,1,TRUE) ;

	_iCount = (int) SendMessage(_hListBox,LB_GETCOUNT,0,0) ;

	if(_iCount > 1)	sqlite3_snprintf(sizeof(_szTmp),_szTmp,Formats(2),_iCount) ;
	else if(_iCount == 1)	lstrcpy(_szTmp,Strings(4)) ;
	else if(_iCount == 0)	lstrcpy(_szTmp,Strings(5)) ;

	SendDlgItemMessage(hToolBox,IDC_STATUSBAR,SB_SETTEXT,SBT_NOBORDERS,(LPARAM) _szTmp) ;

	return (_iCount) ;
}

int SetAllTheFilters(void)
{
	register		LPPGM_FILTERS	_lpFilter ;
	HSTMT			_hFind ;
	HWND			_hListBox, _hWorkSpace ;
	LPSTR			_lpszTmp ;
	PGM_FILTERS		_Filter ;
	int				_iIndex, _iValue, _iCount ;
	char			_szTmp[256] ;

	_lpFilter = &_Filter ;
	_hFind = NULL ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	_hListBox = GetDlgItem(_hWorkSpace,LISTBOX_MODELS) ;

	SendMessage(_hListBox,LB_RESETCONTENT,0,0) ;
	_iCount = 0 ;

	if(sqlite3_prepare_v2(hCfeDb,"SELECT [FilterRecordNumber],[StarName] FROM Filters ORDER BY [ColsNumber],[StarName];",-1,&_hFind,NULL) != SQLITE_OK)
		return (Error(52)) ;

	while(sqlite3_step(_hFind) == SQLITE_ROW)
	{
		_iValue = sqlite3_column_int(_hFind,0) ;
		_lpszTmp = (LPSTR) sqlite3_column_text(_hFind,1) ;
		if(_lpszTmp)
		{
			_iIndex = (int) SendMessage(_hListBox,LB_ADDSTRING,0,(LPARAM) _lpszTmp) ;
			SendMessage(_hListBox,LB_SETITEMDATA,_iIndex,(LPARAM) _iValue) ;

			_iCount++ ;
		}
	}

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	if(_iCount > 1)	sqlite3_snprintf(sizeof(_szTmp),_szTmp,Formats(2),_iCount) ;
	else if(_iCount == 1)	lstrcpy(_szTmp,Strings(4)) ;
	else if(_iCount == 0)	lstrcpy(_szTmp,Strings(5)) ;

	SendDlgItemMessage(hToolBox,IDC_STATUSBAR,SB_SETTEXT,SBT_NOBORDERS,(LPARAM) _szTmp) ;

	SendMessage(_hListBox,LB_SETCURSEL,0,0) ;

	return (_iCount) ;
}

void PushFilter(void)
{
	register	int		_i ;

	if(iStackPointer == -1)
	{
		lpFilterStack = &FilterStack[0] ;							//	**************************
		memcpy(lpFilterStack,lpCurrentFilter,sizeof(PGM_FILTERS)) ;	//	*** The stack is empty ***
		iStackPointer = 0 ;											//	**************************
		return ;
	}

	if(iStackPointer < MAX_FILTER_STACK - 1)
	{
		iStackPointer++ ;

		memcpy(&FilterStack[iStackPointer],lpCurrentFilter,sizeof(PGM_FILTERS)) ;
		lpFilterStack = &FilterStack[iStackPointer] ;

		return ;
	}

	for(_i = 0 ; _i < MAX_FILTER_STACK - 1 ; _i++)
		memcpy(&FilterStack[_i],&FilterStack[_i + 1],sizeof(PGM_FILTERS)) ;

	iStackPointer = MAX_FILTER_STACK - 1 ;
	lpFilterStack = &FilterStack[iStackPointer] ;

	memcpy(lpFilterStack,lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	return ;
}

void PopFilter(void)
{									//	*************************************
	if(iStackPointer == -1)			//	*** Do nothing the stack is empty ***
		return ;					//	*************************************

	if(iStackPointer == 0)
	{
		memcpy(lpCurrentFilter,&FilterStack[0],sizeof(PGM_FILTERS)) ;
		RtlSecureZeroMemory(&FilterStack[0],sizeof(PGM_FILTERS)) ;

		lpFilterStack = NULL ;		// The stack is empty
		iStackPointer = -1 ;

		return ;
	}

//	****************************************************
//	*** Copy the current stack pointer to the filter ***
//	****************************************************

	memcpy(lpCurrentFilter,&FilterStack[iStackPointer],sizeof(PGM_FILTERS)) ;
	RtlSecureZeroMemory(&FilterStack[iStackPointer],sizeof(PGM_FILTERS)) ;

	iStackPointer-- ;

	lpFilterStack = &FilterStack[iStackPointer] ;

	return ;
}

LPPGM_FILTERS ReadFilter(int __iFilterRecordNumber,LPPGM_FILTERS __lpFilter)
{
	register		LPPGM_FILTERS	_lpFilter ;
	register		size_t			_i ;
					HSTMT			_hFind ;
					char			_szTmp[1024] ;

	Log("Entering ReadFilter") ;

	_lpFilter = __lpFilter ;

	if(_lpFilter == NULL)
	{
		_lpFilter = (LPPGM_FILTERS) PhR_Alloc_Memory(sizeof(PGM_FILTERS)) ;
		if(!_lpFilter)
		{
			Log("Error while allocating memory. Error 38") ;

			Error(38) ;
			return (NULL) ;
		}
	}
	else
	{
		RtlSecureZeroMemory(_lpFilter,sizeof(PGM_FILTERS)) ;
	}

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"SELECT * FROM Filters INNER JOIN Convolution ON Filters.FilterRecordNumber = Convolution.FilterRecordNumber WHERE Filters.FilterRecordNumber == %d;",__iFilterRecordNumber) ;

	Log(_szTmp) ;

	if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,NULL) != SQLITE_OK)
	{
		if(__lpFilter == NULL)
		{
			PhR_Free_Memory(_lpFilter) ;

			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;
		}

		Log("Error 52 when executing the previous query") ;

		Error(52) ;
		return (NULL) ;
	}

	if(sqlite3_step(_hFind) == SQLITE_ROW)
	{
		Log("Record found") ;

		_lpFilter->FilterRecordNumber	= sqlite3_column_int(_hFind,0) ;
		_lpFilter->StarRecordNumber		= sqlite3_column_int(_hFind,1) ;
		_lpFilter->Model				= sqlite3_column_int(_hFind,2) ;
		_lpFilter->Rate					= sqlite3_column_int(_hFind,3) ;
		_lpFilter->ColsNumber			= sqlite3_column_int(_hFind,4) ;
		_lpFilter->TotalColsNumber		= sqlite3_column_int(_hFind,5) ;
		_lpFilter->Divisor				= sqlite3_column_int(_hFind,6) ;
		_lpFilter->Bias					= sqlite3_column_int(_hFind,7) ;
		_lpFilter->Grey					= sqlite3_column_int(_hFind,8) ;
		_lpFilter->BlackAndWhite		= sqlite3_column_int(_hFind,9) ;

		lstrcpy(_lpFilter->Keccak_Filter,	(LPSTR) sqlite3_column_text(_hFind,10)) ;
		lstrcpy(_lpFilter->StarName,		(LPSTR) sqlite3_column_text(_hFind,11)) ;
		lstrcpy(_lpFilter->Comment,			(LPSTR) sqlite3_column_text(_hFind,12)) ;

		if(_lpFilter->Model == -1)
			_lpFilter->Model = 0 ;

		if(_lpFilter->ColsNumber == 0)
		{
			Log("Number of columns = 0") ;
			_lpFilter->ColsNumber = 3 ;
		}

		if(_lpFilter->TotalColsNumber == 0)
		{
			Log("Total Number of columns = 0") ;
			_lpFilter->TotalColsNumber = _lpFilter->ColsNumber * _lpFilter->ColsNumber ;
		}

		if(_lpFilter->TotalColsNumber > 841)
		{
			Log("Number of colums greather than 29") ;
			Log("CFE corrects it") ;

			_lpFilter->TotalColsNumber = 841 ;
			_lpFilter->ColsNumber = 29 ;
		}

		if(_lpFilter->Comment[0] == '\0')
			lstrcpy(_lpFilter->Comment,Strings(1)) ;

		if(_lpFilter->Divisor == 0)
			_lpFilter->Divisor = 1 ;

		for(_i = 0 ; (int) _i < _lpFilter->TotalColsNumber ; _i++)
			_lpFilter->Matrix[_i] = sqlite3_column_int(_hFind,14 + _i) ;
	}

	Log("The filter has been read successfuly") ;

	return (_lpFilter) ;
}

void ComputeTheDivisor(void)
{
	register		int				_i ;
	register		int				_j ;

	HWND			_hWorkSpace ;
	int				_iNegatif, _iPositif, _iDivisor, _k ;

	if(lpCurrentFilter->Divisor == 0)
		lpCurrentFilter->Divisor = 1 ;

	_iNegatif = _iPositif = 0 ;

	_k = lpCurrentFilter->ColsNumber + 1 ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		for(_j = 0 ; _j < lpCurrentFilter->ColsNumber ; _j++)
		{
			SendMessage(hGrid,ZGM_GETCELLINT,_k,(LPARAM) &_iDivisor) ;

			if(_iDivisor > 0)		_iPositif += _iDivisor ;
			else if(_iDivisor < 0)	_iNegatif -= _iDivisor ;

			_k++ ;
		}
	}

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	SetDlgItemInt(_hWorkSpace,EDIT_NEGATIF,_iNegatif,TRUE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_POSITIF,_iPositif,TRUE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_NEGPOS,_iNegatif - _iPositif,TRUE) ;

	ComputeSimilarBytes() ;

	return ;
}

void ApplyFilter(int __bCopyToClipboard)
{
	register		int			_i ;
	register		int			_j ;

	LPIMAGEINFOS	_lpImageInfos ;
	LPBYTE			_lpResult ;
	HANDLE			_hBitmap ;
	HWND			_hWorkSpace ;
	GFL_BITMAP		*_GflBitmap ;
	DWORD			_dwSize ;
	int				_iDivisor, _iNegatif, _iPositif, _k, _iMatrix ;

	if(lpCurrentFilter->TotalColsNumber == 0)
		return ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	if(SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0) == 0)
	{
		lpCurrentFilter = &CurrentFilter ;

		lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
		lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
		lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

		SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,lpCurrentFilter->Rate,0) ;
		SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

		if(lpCurrentFilter->Model == 0)
			CreateFilter() ;

		Log("The user wants to apply changes to a filter but the listbox is empty. CoFeE has created a new filter.") ;
	}

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	hWaiting = (HWND) CreateDialog(hInstance,MAKEINTRESOURCE(DLG_WAITING),hToolBox,(DLGPROC) DlgWaitingProc) ;
	if(hWaiting != NULL)	ShowWindow(hWaiting,SW_SHOW) ;
	else					return ;

	if(SendDlgItemMessage(_hWorkSpace,LISTBOX_RNG_LIMITS,LB_GETCURSEL,0,0) == LB_ERR)
	{
		Error(46) ;

		DestroyWindow(hWaiting) ;
		hWaiting = NULL ;

		return ;
	}

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(_lpImageInfos)
	{
		_iNegatif = _iPositif = 0 ;

		_k = lpCurrentFilter->ColsNumber + 1 ;
		_iMatrix = 0 ;

		for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
		{
			for(_j = 0 ; _j < lpCurrentFilter->ColsNumber ; _j++)
			{
				SendMessage(hGrid,ZGM_GETCELLINT,_k,(LPARAM) &lpCurrentFilter->Matrix[_iMatrix]) ;

				_iDivisor = lpCurrentFilter->Matrix[_iMatrix] ;

				if(_iDivisor > 0)		_iPositif += _iDivisor ;
				else if(_iDivisor < 0)	_iNegatif -= _iDivisor ;

				_iMatrix ++ ;
				_k++ ;
			}
		}

		SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,lpCurrentFilter->Rate,0) ;

		SetDlgItemInt(_hWorkSpace,EDIT_NEGATIF,_iNegatif,TRUE) ;
		SetDlgItemInt(_hWorkSpace,EDIT_POSITIF,_iPositif,TRUE) ;
		SetDlgItemInt(_hWorkSpace,EDIT_NEGPOS,_iNegatif - _iPositif,TRUE) ;

		_lpResult = GetBitsFromBitmap(_lpImageInfos->hImgBitmap,&_dwSize,_lpImageInfos->lpWork) ;
		if(_lpResult)
		{
			_lpImageInfos->lpWork = _lpImageInfos->lpResult ;
			_lpImageInfos->lpResult = _lpResult ;

			lpCurrentFilter->Divisor	= GetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,NULL,TRUE) ;
			lpCurrentFilter->Bias		= GetDlgItemInt(_hWorkSpace,EDIT_BIAS,NULL,TRUE) ;

			if(lpCurrentFilter->Divisor == 0)
			{
				lpCurrentFilter->Divisor = 1 ;
				SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,lpCurrentFilter->Divisor,TRUE) ;
			}

			if(bFirstDrop)
			{
				PaintResultWindow() ;

				Child2_PaintResultWindow() ;
			}
			else
			{
				Convolve(_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpResult,lpCurrentFilter) ;
				PaintResultWindow() ;

				Child2_PaintResultWindow() ;

				lpCurrentFilter->BlackAndWhite = GetDlgItemInt(_hWorkSpace,EDIT_NEGPOS,NULL,TRUE) ;
			}

			if(__bCopyToClipboard == 1)
			{
				_hBitmap = SetBitsToBitmap(_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpResult) ;
				if(_hBitmap)
				{
					if(gflConvertDDBIntoBitmap(_hBitmap,&_GflBitmap) == GFL_NO_ERROR)
					{
						gflExportIntoClipboard(_GflBitmap) ;

						gflFreeBitmapData(_GflBitmap) ;
						gflFreeBitmap(_GflBitmap) ;
					}

					DeleteObject(_hBitmap) ;
				}
			}
		}
	}

	DestroyWindow(hWaiting) ;
	hWaiting = NULL ;

	SetCursor(LoadCursor(NULL,IDC_ARROW)) ;

	return ;
}

void ClearAllFields(void)
{
	register	int		_i ;
	register	int		_k ;

				int		_iInteger ;
				HWND	_hWorkSpace ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	lpCurrentFilter = &CurrentFilter ;

	PushFilter() ;

	_k = lpCurrentFilter->ColsNumber + 1 ;

	RtlSecureZeroMemory(lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	lpCurrentFilter->Divisor = 1 ;

	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;

	SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,1,TRUE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_BIAS,0,TRUE) ;

	CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
	CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
	CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;

	_iInteger = 0 ;

	for(_i = 0 ; _i < lpCurrentFilter->TotalColsNumber ; _i++)
	{
		SendMessage(hGrid,ZGM_SETCELLINT,_k,(LPARAM) &_iInteger) ;

		_k++ ;
	}

	SendMessage(hGrid,ZGM_AUTOSIZE_ALL_COLUMNS,0,0) ;

	lpCurrentFilter->Model = FALSE ;

	ApplyFilter(FALSE) ;

	return ;
}

int myRandomizer(long long int _iMini,long long int _iMaxi)
{
	return (int)(((long long int)rand() / ((long long int)RAND_MAX / (_iMaxi - _iMini))) + _iMini) ;
}

void Randomize(void)
{
	register	int		_i ;
	register	int		_k ;
				int		_iInteger, _iIndex, _iMilieu, _j ;
				int		_iValue, _iRandomLimitPositive, _iRandomLimitNegative ;
				size_t	_iMatrix ;
				HWND	_hWorkSpace ;
				char	_szTmp[256] ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	_iIndex = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_RNG_LIMITS,LB_GETCURSEL,0,0) ;
	if(_iIndex == LB_ERR)
	{
		Error(46) ;

		return ;
	}

	if(SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0) == 0)
	{
		if(lpCurrentFilter->Model != -1)
		{
			Log("A new filter will be created because the models listbox is empty.") ;
			CreateFilter() ;
		}
	}

	PushFilter() ;

	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

	if(lpCurrentFilter->Model != -1)
		lpCurrentFilter->Model = 0 ;

	lpCurrentFilter->Rate = 0 ;
	lpCurrentFilter->BlackAndWhite = 0 ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;
	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	RtlSecureZeroMemory(lpCurrentFilter->Matrix,MAX_EDIT_FIELDS * sizeof(int)) ;

	_iValue = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_RNG_LIMITS,LB_GETITEMDATA,_iIndex,0) ;

	_iRandomLimitNegative = iRandomLimitNegative[_iValue] ;
	_iRandomLimitPositive = iRandomLimitPositive[_iValue] ;

	_k = lpCurrentFilter->ColsNumber + 1 ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	SendMessage(hGrid,ZGM_DIMGRID,lpCurrentFilter->ColsNumber,0) ;
	SendMessage(hGrid,ZGM_SHOWGRIDLINES,TRUE,0) ;
	SendMessage(hGrid,ZGM_EMPTYGRID,TRUE,0) ;

	SetEditorColors() ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		_iIndex = (int) SendMessage(hGrid,ZGM_APPENDROW,0,0) ;
		SendMessage(hGrid,ZGM_SETROWTYPE,_iIndex,3) ;

		sqlite3_snprintf(sizeof(_szTmp),_szTmp,"C%2.2d",_i + 1) ;
		SendMessage(hGrid,ZGM_SETCELLTEXT,_i + 1,(LPARAM) _szTmp) ;

		SendMessage(hGrid,ZGM_SETCOLEDIT,_i + 1,1) ;
		SendMessage(hGrid,ZGM_SETCOLFORMAT,_i + 1,0) ;
	}

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		for(_j = 0 ; _j < lpCurrentFilter->ColsNumber ; _j++)
		{
			_iValue = myRandomizer((long long int) _iRandomLimitNegative,(long long int) _iRandomLimitPositive) ;
			lpCurrentFilter->Matrix[_iMatrix] = _iValue ;

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

	ApplyFilter(FALSE) ;

	return ;
}

void ReverseValues(void)
{
	register	int		_i ;
	register	int		_k ;
				int		_iValue, _iMatrix ;
				HWND	_hWorkSpace ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	PushFilter() ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;
	lpCurrentFilter->Rate = 0 ;

	SetEditorColors() ;

	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

	lpCurrentFilter->Model = -1 ;
	lpCurrentFilter->Rate = 0 ;
	lpCurrentFilter->BlackAndWhite = 0 ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;
	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMatrix = 0 ;

	for(_i = 0 ; _i < lpCurrentFilter->TotalColsNumber ; _i++)
	{
		_iValue = 1+~lpCurrentFilter->Matrix[_iMatrix] ;

		if((_iValue < -9999) || (_iValue > 9999))
			_iValue = 0 ;

		lpCurrentFilter->Matrix[_iMatrix] = _iValue ;

		SendMessage(hGrid,ZGM_SETCELLINT,_k,(LPARAM) &_iValue) ;

		_iMatrix ++ ;
		_k++ ;
	}

	SendMessage(hGrid,ZGM_AUTOSIZE_ALL_COLUMNS,0,0) ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;

	ApplyFilter(FALSE) ;

	lpCurrentFilter->Model = 0 ;

	return ;
}

void InverseValues(void)
{
	register	int		_i ;
	register	int		_k ;
				int		_iValue, _iMatrix ;
				HWND	_hWorkSpace ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	PushFilter() ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;
	lpCurrentFilter->Rate = 0 ;

	SetEditorColors() ;

	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

	lpCurrentFilter->Model = -1 ;
	lpCurrentFilter->Rate = 0 ;
	lpCurrentFilter->BlackAndWhite = 0 ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;
	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iMatrix = 0 ;

	for(_i = 0 ; _i < lpCurrentFilter->TotalColsNumber ; _i++)
	{
		_iValue = ~lpCurrentFilter->Matrix[_iMatrix] ;

		if((_iValue < -9999) || (_iValue > 9999))
			_iValue = 0 ;

		lpCurrentFilter->Matrix[_iMatrix] = _iValue ;

		SendMessage(hGrid,ZGM_SETCELLINT,_k,(LPARAM) &_iValue) ;

		_iMatrix ++ ;
		_k++ ;
	}

	SendMessage(hGrid,ZGM_AUTOSIZE_ALL_COLUMNS,0,0) ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;

	ApplyFilter(FALSE) ;

	lpCurrentFilter->Model = 0 ;

	return ;
}

void Initialize(void)
{
	register		DWORD	_i ;
	register		int		_iIndex ;
					int		_bFound ;
					HWND	_hWorkSpace ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	PushFilter() ;

	RtlSecureZeroMemory(lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	lpCurrentFilter->Divisor = 1 ;

	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;
	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;

	SendMessage(hGrid,ZGM_EMPTYGRID,TRUE,0) ;

	CreateEditor(lpCurrentFilter->ColsNumber,TRUE) ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,1,TRUE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_BIAS,0,TRUE) ;

	CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
	CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
	CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;

	_bFound = FALSE ;

	_i = 0 ;

	while(TRUE)
	{
		_iIndex = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETITEMDATA,_i,0) ;
		if(_iIndex == CB_ERR)
			break ;

		if(lpCurrentFilter->ColsNumber == _iIndex)
		{
			SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,_i,0) ;
			_bFound = TRUE ;
			break ;
		}

		_i++ ;
	}

	if(_bFound == FALSE)
	{
		SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,0,0) ;
		lpCurrentFilter->ColsNumber = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETITEMDATA,0,0) ;
	}

	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	SendMessage(hGrid,ZGM_AUTOSIZE_ALL_COLUMNS,0,0) ;

	ApplyFilter(FALSE) ;

	return ;
}

LPPGM_FILTERS CreateFilter(void)
{
	int		_i ;
	HWND	_hWorkSpace ;

	lpCurrentFilter = &CurrentFilter ;

	PushFilter() ;

	SendMessage(hGrid,ZGM_EMPTYGRID,TRUE,0) ;

	lpCurrentFilter = &CurrentFilter ;
	RtlSecureZeroMemory(lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	_i = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETCURSEL,0,0) ;
	lpCurrentFilter->ColsNumber = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETITEMDATA,_i,0) ;

	lpCurrentFilter->Divisor = 1 ;

	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;

	lpCurrentFilter->TotalColsNumber = lpCurrentFilter->ColsNumber * lpCurrentFilter->ColsNumber ;

	CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
	CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
	CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;

	SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,1,TRUE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_BIAS,0,TRUE) ;

	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;

	CreateEditor(lpCurrentFilter->ColsNumber,TRUE) ;
	lpCurrentFilter->Model = -1 ;

	return (lpCurrentFilter) ;
}

void ChangeFilterSize(int __iNewSize,int __bApplyFilter)
{
	register		int		_i ;
	register		int		_j ;
					int		_k, _iIndex, _iInteger, _iMatrix, _iMilieu ;
					int		_iValue, _iRandomLimitPositive, _iRandomLimitNegative ;
					HWND	_hWorkSpace ;
					char	_szTmp[256] ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	_i = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETCURSEL,0,0) ;
	if(_i == LB_ERR)
		return ;

	_j = Combos[_i].Index ;

	PushFilter() ;

	lpCurrentFilter = &CurrentFilter ;

	RtlSecureZeroMemory(lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	if(__iNewSize == 0)
	{
		lpCurrentFilter->TotalColsNumber = _j * _j ;
		lpCurrentFilter->ColsNumber = _j ;
	}
	else
	{
		lpCurrentFilter->TotalColsNumber = __iNewSize * __iNewSize ;
		lpCurrentFilter->ColsNumber = __iNewSize ;
	}

	lpCurrentFilter->Divisor = 1 ;

	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;
	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
	CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
	CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;

	SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,(lpCurrentFilter->ColsNumber - 3) / 2,0) ;

	SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,1,TRUE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_BIAS,0,TRUE) ;

	SendMessage(hGrid,ZGM_DIMGRID,lpCurrentFilter->ColsNumber,0) ;
	SendMessage(hGrid,ZGM_SHOWGRIDLINES,TRUE,0) ;

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;

	SetEditorColors() ;

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
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	_iIndex = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_RNG_LIMITS,LB_GETCURSEL,0,0) ;
	if(_iIndex == LB_ERR)
		_iIndex = 0 ;

	_iValue = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_RNG_LIMITS,LB_GETITEMDATA,_iIndex,0) ;

	_iRandomLimitNegative = iRandomLimitNegative[_iValue] ;
	_iRandomLimitPositive = iRandomLimitPositive[_iValue] ;

	_iInteger = 0 ;
	_iMatrix = 0 ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		for(_j = 0 ; _j < lpCurrentFilter->ColsNumber ; _j++)
		{
			SendMessage(hGrid,ZGM_SETCELLEDIT,_k,1) ;			//	1 = Editable with an edit control
			SendMessage(hGrid,ZGM_SETCELLFORMAT,_k,0) ;			//	Numeric data that has a format of 0 will display its data in a normal numeric fashion.

			_iValue = myRandomizer((long long int) _iRandomLimitNegative,(long long int) _iRandomLimitPositive) ;

			lpCurrentFilter->Matrix[_iMatrix] = _iValue ;

			SendMessage(hGrid,ZGM_SETCELLINT,_k,(LPARAM) &_iValue) ;
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

	if(__bApplyFilter)
		ApplyFilter(FALSE) ;

	return ;
}

void PopInitialize(void)
{
	register		int		_i ;
	register		int		_iIndex ;
					int		_bFound, _iMatrix, _k, _iMilieu, _j, _iInteger ;
					HWND	_hWorkSpace ;
					char	_szTmp[1024] ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	SendMessage(hGrid,ZGM_EMPTYGRID,TRUE,0) ;

	_bFound = FALSE ;

	_i = 0 ;
	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	while(TRUE)
	{
		_iIndex = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETITEMDATA,_i,0) ;
		if(_iIndex == CB_ERR)
			break ;

		if(lpCurrentFilter->ColsNumber == _iIndex)
		{
			SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,_i,0) ;
			_bFound = TRUE ;
			break ;
		}

		_i++ ;
	}

	if(_bFound == FALSE)
	{
		SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,0,0) ;
		lpCurrentFilter->ColsNumber = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETITEMDATA,0,0) ;
	}

	SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,(lpCurrentFilter->ColsNumber - 3) / 2,0) ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,lpCurrentFilter->Rate,0) ;

	SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,lpCurrentFilter->Divisor,TRUE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_BIAS,lpCurrentFilter->Bias,TRUE) ;

	_iIndex = GREY_NONE ;

	if(lpCurrentFilter->Grey & GREY_INPUT)		_iIndex |= GREY_INPUT ;
	if(lpCurrentFilter->Grey & GREY_OUTPUT)		_iIndex |= GREY_OUTPUT ;

	switch(_iIndex)
	{
		case	GREY_NONE :					CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;

											lpCurrentFilter->Grey = GREY_NONE ;
											break ;

		case	GREY_INPUT :				CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_UNCHECKED) ;
											break ;

		case	GREY_OUTPUT :				CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_UNCHECKED) ;
											break ;

		case	GREY_INPUT|GREY_OUTPUT :	CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_UNCHECKED) ;
											break ;

		default :							CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;

											lpCurrentFilter->Grey = GREY_NONE ;
											break ;

	}

	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	SendMessage(hGrid,ZGM_DIMGRID,lpCurrentFilter->ColsNumber,0) ;
	SendMessage(hGrid,ZGM_SHOWGRIDLINES,TRUE,0) ;
	SendMessage(hGrid,ZGM_EMPTYGRID,TRUE,0) ;

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;

	SetEditorColors() ;

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

	ApplyFilter(FALSE) ;

	return ;
}

void SetPrevious(void)
{
	PopFilter() ;

	PopInitialize() ;

	return ;
}

void SetAllFields(int __iValue)
{
	register	int		_i ;
	register	int		_j ;
				int		_k, _iInteger, _iMatrix, _iMilieu ;
				HWND	_hWorkSpace ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	PushFilter() ;

	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

	lpCurrentFilter->Model = 0 ;
	lpCurrentFilter->Rate = 0 ;
	lpCurrentFilter->BlackAndWhite = 0 ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;
	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;

	__stosq((unsigned long long *) lpCurrentFilter->Matrix,0,(MAX_EDIT_FIELDS * sizeof(int)) / sizeof(QWORD)) ;

	_k = lpCurrentFilter->ColsNumber + 1 ;
	_iInteger = 0 ;
	_iMatrix = 0 ;
	_iMilieu = lpCurrentFilter->ColsNumber / 2 ;

	SetEditorColors() ;

	for(_i = 0 ; _i < lpCurrentFilter->ColsNumber ; _i++)
	{
		for(_j = 0 ; _j < lpCurrentFilter->ColsNumber ; _j++)
		{
			lpCurrentFilter->Matrix[_iMatrix] = __iValue ;

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

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,lpCurrentFilter->Rate,0) ;

	lpCurrentFilter->Model = FALSE ;

	SendMessage(hGrid,ZGM_AUTOSIZE_ALL_COLUMNS,0,0) ;

	ApplyFilter(FALSE) ;

	return ;
}

int SaveDatas(void)
{
	register			DWORD	_i ;
						LPSTR	_lpszTmp ;
						HSTMT	_hFind ;
						HWND	_hListBox ;
						int		_iIndex, _iLen ;
						char	_szResult[1024] ;

	CreateKeccak(lpCurrentFilter) ;

	if(IsThisKeccackPresentInDatabase(lpCurrentFilter->Keccak_Filter))
	{
		Log("The Keccak hash key has not been found") ;

		return (0) ;
	}

	if(sqlite3_prepare_v2(hCfeDb,"SELECT MAX([FilterRecordNumber]) FROM [Filters];",-1,&_hFind,NULL) != SQLITE_OK)
		return (FALSE) ;

	if(sqlite3_step(_hFind) == SQLITE_ROW)
		lpCurrentFilter->FilterRecordNumber = sqlite3_column_int(_hFind,0) + 1 ;

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	if(lpCurrentFilter->Model == -1)
		lpCurrentFilter->Model = 0 ;

	GetAndUpdateAsteroidName(lpCurrentFilter->FilterRecordNumber,lpCurrentFilter->Keccak_Filter,lpCurrentFilter->StarName,(LPDWORD) &lpCurrentFilter->StarRecordNumber) ;

	sqlite3_snprintf(sizeof(szBigBuffer),szBigBuffer,
		"INSERT OR IGNORE INTO [Filters] VALUES(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\",\"%w\",\"%w\");\n",
					lpCurrentFilter->FilterRecordNumber,
					lpCurrentFilter->StarRecordNumber,
					lpCurrentFilter->Model,
					lpCurrentFilter->Rate,
					lpCurrentFilter->ColsNumber,
					lpCurrentFilter->TotalColsNumber,
					lpCurrentFilter->Divisor,
					lpCurrentFilter->Bias,
					lpCurrentFilter->Grey,
					lpCurrentFilter->BlackAndWhite,
					lpCurrentFilter->Keccak_Filter,
					lpCurrentFilter->StarName,
					lpCurrentFilter->Comment) ;

	while(sqlite3_exec(hCfeDb,"BEGIN TRANSACTION",0,0,NULL) == SQLITE_BUSY) ;

	if(sqlite3_exec(hCfeDb,szBigBuffer,0,0,NULL) != SQLITE_OK)
	{
		Log("Cannot insert this new record into the Filters table; Error 49.") ;
		SQLite_Error() ;

		sqlite3_exec(hCfeDb,"COMMIT",0,0,NULL) ;

		return (Error(49)) ;
	}

	sqlite3_snprintf(sizeof(szBigBuffer),szBigBuffer,"INSERT OR IGNORE INTO [Convolution] VALUES(%lu,",lpCurrentFilter->FilterRecordNumber) ;
	_lpszTmp = szBigBuffer + lstrlen(szBigBuffer) ;

	for(_i = 0 ; (int) _i < MAX_EDIT_FIELDS ; _i++)
	{
		_iLen = wsprintf(_szResult,"%d,",lpCurrentFilter->Matrix[_i]) ;

		lstrcat(_lpszTmp,_szResult) ;
		_lpszTmp += _iLen ;
	}

	lstrcpy(_lpszTmp - 1,");\n") ;

	if(sqlite3_exec(hCfeDb,szBigBuffer,0,0,NULL) != SQLITE_OK)
	{
		Log("Cannot insert this new record into the Convolution table; Error 49.") ;

		SQLite_Error() ;

		sqlite3_exec(hCfeDb,"COMMIT",0,0,NULL) ;

		return (Error(49)) ;
	}

	sqlite3_exec(hCfeDb,"COMMIT",0,0,NULL) ;

	//	Ajouter aux listboxes et à la combo

	_hListBox = GetDlgItem(hTabsWindows[HWND_WORKSPACE],LISTBOX_MODELS) ;
	_iIndex = (int) SendMessage(_hListBox,LB_ADDSTRING,0,(LPARAM) lpCurrentFilter->StarName) ;
	SendMessage(_hListBox,LB_SETITEMDATA,_iIndex,(LPARAM) lpCurrentFilter->FilterRecordNumber) ;
	SendMessage(_hListBox,LB_SETCURSEL,_iIndex,0) ;

	Child2_SetRedOrGreen() ;

	Log("Filter added to the database") ;

	return (TRUE) ;
}

void SaveFilter(int __bSaveData)
{
	register		HBITMAP			_hBitmap ;
	register		LPIMAGEINFOS	_lpImageInfos ;

	GFL_BITMAP		*GflBitmap ;
	GFL_SAVE_PARAMS	_Params ;
	HBITMAP			_hNewBitmap ;
	HWND			_hWorkSpace ;
	int				_i, _iCount, _bModel ;
	char			_szFileName[MAX_PATH] ;
	char			_szTmp[1024] ;
	char			_szDest[MAX_PATH] ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	if(GetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment,MAX_COMMENT_LENGTH) == 0)
	{
		lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
		SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;
	}

	if(__bSaveData)
	{
		_bModel = lpCurrentFilter->Model ;

		if(_bModel == 1)
			lpCurrentFilter->Model = 0 ;

		SaveDatas() ;

		_iCount = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0) ;

		if(_iCount > 1)	sqlite3_snprintf(sizeof(_szTmp),_szTmp,Formats(2),_iCount) ;
		else if(_iCount == 1)	lstrcpy(_szTmp,Strings(4)) ;
		else if(_iCount == 0)	lstrcpy(_szTmp,Strings(5)) ;

		SendDlgItemMessage(hToolBox,IDC_STATUSBAR,SB_SETTEXT,SBT_NOBORDERS,(LPARAM) _szTmp) ;

		lpCurrentFilter->Model = _bModel ;
	}

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(_lpImageInfos)
	{
		_hBitmap = SetBitsToBitmap(_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight,_lpImageInfos->lpResult) ;
		if(_hBitmap)
		{
			_hNewBitmap = CropImage(_hBitmap,MAX_EDIT_COLUMNS,MAX_EDIT_ROWS) ;
			if(!_hNewBitmap)
			{
				DeleteObject(_hBitmap) ;
				SetCursor(LoadCursor(NULL,IDC_ARROW)) ;

				return ;
			}

			*PathFindFileName(lstrcpy(_szDest,_lpImageInfos->Image.szImageFile)) = '\0' ;
			PathRemoveBackslash(_szDest) ;

			if(lpCurrentFilter->StarName[0])
				sqlite3_snprintf(sizeof(_szFileName),_szFileName,"%s\\(%s)-%5.5lu-%s",
																_szDest,
																lpCurrentFilter->StarName,
																lpCurrentFilter->FilterRecordNumber,
																PathFindFileName(_lpImageInfos->Image.szImageFile)) ;
			else
				sqlite3_snprintf(sizeof(_szFileName),_szFileName,"%s\\%5.5lu-%s",
																_szDest,
																lpCurrentFilter->FilterRecordNumber,
																PathFindFileName(_lpImageInfos->Image.szImageFile)) ;

			lstrcpy(_szTmp,_szFileName) ;

			_i = 1 ;

			while(PathFileExists(_szTmp))
			{
				lstrcpy(_szFileName,_szTmp) ;
				*PathFindExtension(_szFileName) = '\0' ;

				if(lpCurrentFilter->Model)
					lstrcat(_szFileName," (M") ;
				else
					lstrcat(_szFileName," (") ;

				wsprintf(_szTmp,"%2.2d)",_i) ;

				lstrcat(_szTmp,PathFindExtension(_lpImageInfos->Image.szImageFile)) ;
				lstrcat(_szFileName,_szTmp) ;

				_i++ ;
			}

			if(gflConvertDDBIntoBitmap(_hNewBitmap,&GflBitmap) == GFL_NO_ERROR)
			{
				gflGetDefaultSaveParams(&_Params) ;

				_Params.Flags = GFL_SAVE_REPLACE_EXTENSION|GFL_SAVE_ANYWAY ;
				_Params.FormatIndex = _lpImageInfos->Image.dwFormatIndex ;
				_Params.Compression = GFL_NO_COMPRESSION ;
				_Params.Quality = 100 ;
				_Params.Progressive  = TRUE ;
				_Params.OptimizeHuffmanTable = TRUE ;

				if(gflSaveBitmap(_szFileName,GflBitmap,&_Params) != GFL_NO_ERROR)
				{
					gflFreeBitmapData(GflBitmap) ;
					gflFreeBitmap(GflBitmap) ;

					DeleteObject(_hBitmap) ;
					DeleteObject(_hNewBitmap) ;

					SetCursor(LoadCursor(NULL,IDC_ARROW)) ;
					Error(20) ;

					return ;
				}

				gflFreeBitmapData(GflBitmap) ;
				gflFreeBitmap(GflBitmap) ;
			}

			DeleteObject(_hBitmap) ;
			DeleteObject(_hNewBitmap) ;
		}
	}

	SetCursor(LoadCursor(NULL,IDC_ARROW)) ;

	return ;
}

int UpdateFilter(void)
{
	register	DWORD	_i ;
				LPSTR	_lpszTmp ;
				HSTMT	_hFind ;
				HWND	_hWorkSpace ;
				int		_iLen ;
				char	_szResult[1024] ;

	Log("Entering UpdateFilter") ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	if(!lpCurrentFilter->FilterRecordNumber)
		return (FALSE) ;

	if(SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0) == 0)
	{
		Log("The user wants to update a filter but the listbox is empty. CoFeE has created a new filter.") ;

		NewFilter() ;

		return (Error(1)) ;
	}

	if(lpCurrentFilter->Model == 1)
	{
		Log("Cannot update the filter, it is locked; Error 25.") ;

		return (Error(25)) ;
	}

	if(!ReadFilter(lpCurrentFilter->FilterRecordNumber,lpCurrentFilter))
	{
		Log("Cannot read the filter. Error 50.") ;

		return (Error(50)) ;
	}

	if(GetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment,MAX_COMMENT_LENGTH) == 0)
	{
		lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
		SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;
	}

	CreateKeccak(lpCurrentFilter) ;

	lpCurrentFilter->Divisor = GetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,NULL,TRUE) ;
	lpCurrentFilter->Bias = GetDlgItemInt(_hWorkSpace,EDIT_BIAS,NULL,TRUE) ;

	if(lpCurrentFilter->Divisor == 0)
		lpCurrentFilter->Divisor = 1 ;

	lpCurrentFilter->Rate = (int) SendDlgItemMessage(_hWorkSpace,COMBO_FILTER_RATING,CB_GETCURSEL,0,0) ;

	sqlite3_snprintf(sizeof(szBigBuffer),szBigBuffer,
			"SELECT [UnUsed_1] FROM [Filters] WHERE [FilterRecordNumber] = %d;\n",
			lpCurrentFilter->StarRecordNumber) ;

	if(sqlite3_prepare_v2(hCfeDb,szBigBuffer,-1,&_hFind,NULL) == SQLITE_OK)
	{
		if(sqlite3_step(_hFind) == SQLITE_DONE)
		{
			SaveFilter(TRUE) ;

			sqlite3_reset(_hFind) ;
			sqlite3_finalize(_hFind) ;

			return (TRUE) ;
		}

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;
	}

	Log("Updating filter") ;

	if(lpCurrentFilter->Model == -1)
		lpCurrentFilter->Model = 0 ;

	sqlite3_snprintf(sizeof(szBigBuffer),szBigBuffer,
		"UPDATE OR REPLACE [Filters] SET "
		"StarRecordNumber=%d,Model=%d,Rate=%d,ColsNumber=%d,TotalColsNumber=%d,Divisor=%d,Bias=%d,"
		"Grey=%d,BlackAndWhite=%d,Keccak_Filter=\"%s\",StarName=\"%w\",Comment=\"%w\""
		" WHERE [FilterRecordNumber] = %d;\n",
					lpCurrentFilter->StarRecordNumber,
					lpCurrentFilter->Model,
					lpCurrentFilter->Rate,
					lpCurrentFilter->ColsNumber,
					lpCurrentFilter->TotalColsNumber,
					lpCurrentFilter->Divisor,
					lpCurrentFilter->Bias,
					lpCurrentFilter->Grey,
					lpCurrentFilter->BlackAndWhite,
					lpCurrentFilter->Keccak_Filter,
					lpCurrentFilter->StarName,
					lpCurrentFilter->Comment,
					lpCurrentFilter->FilterRecordNumber) ;

	Log(szBigBuffer) ;

	if(sqlite3_exec(hCfeDb,szBigBuffer,0,0,NULL) != SQLITE_OK)
	{
		SQLite_Error() ;

		return (FALSE) ;
	}

	_lpszTmp = szBigBuffer ;

	_iLen = wsprintf(szBigBuffer,"UPDATE OR REPLACE [Convolution] SET FilterRecordNumber=%d,",lpCurrentFilter->StarRecordNumber) ;

	Log(szBigBuffer) ;

	_lpszTmp = szBigBuffer + _iLen ;

	for(_i = 0 ; (int) _i < lpCurrentFilter->TotalColsNumber ; _i++)
	{
		_iLen = wsprintf(_szResult,"V%3.3d=%d,",_i + 1,lpCurrentFilter->Matrix[_i]) ;

		lstrcat(_lpszTmp,_szResult) ;
		_lpszTmp += _iLen ;
	}

	*_lpszTmp = '\0' ;

	_lpszTmp = szBigBuffer ;

	while(*_lpszTmp++) ;

	_iLen = ~(int) (szBigBuffer - _lpszTmp) ;
	_lpszTmp = szBigBuffer + _iLen ;

	sqlite3_snprintf(sizeof(szBigBuffer) - _iLen,_lpszTmp - 1," WHERE [FilterRecordNumber] = %d;\n",lpCurrentFilter->FilterRecordNumber) ;

	if(sqlite3_exec(hCfeDb,szBigBuffer,0,0,NULL) != SQLITE_OK)
	{
		Log("Cannot update this filter") ;

		SQLite_Error() ;

		return (Error(25)) ;
	}

	return (TRUE) ;
}

void ChangeEditorValues(int __iNumberOfColonnes)
{
	register	int		_i, _j ;
				int		_k, _iIndex, _iInteger, _iMatrix, _iMilieu ;
				HWND	_hWorkSpace ;
				char	_szTmp[256] ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	switch(lpCurrentFilter->Grey)
	{
		case	GREY_NONE :					CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_CHECKED) ;
											break ;

		case	GREY_INPUT :				CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_UNCHECKED) ;
											break ;


		case	GREY_OUTPUT :				CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_UNCHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_UNCHECKED) ;
											break ;

		case	GREY_INPUT | GREY_OUTPUT :	CheckDlgButton(_hWorkSpace,RADIO_GREY_INPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_OUTPUT,BST_CHECKED) ;
											CheckDlgButton(_hWorkSpace,RADIO_GREY_NONE,BST_UNCHECKED) ;
											break ;
	}

	SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,(__iNumberOfColonnes - 3) / 2,0) ;

	SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,lpCurrentFilter->Divisor,TRUE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_BIAS,lpCurrentFilter->Bias,TRUE) ;

	SendMessage(hGrid,ZGM_DIMGRID,lpCurrentFilter->ColsNumber,0) ;
	SendMessage(hGrid,ZGM_SHOWGRIDLINES,TRUE,0) ;

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;

	SetEditorColors() ;

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

void ChangeFilter(LPPGM_FILTERS __lpPgmFilter)
{
	register		int				_i ;
	register		int				_iIndex ;
					int				_bFound, _iValue ;
					HWND			_hWorkSpace ;

	PushFilter() ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	CurrentFilter.FilterRecordNumber = __lpPgmFilter->FilterRecordNumber ;

	SendMessage(hGrid,ZGM_EMPTYGRID,TRUE,0) ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	_iValue = 0 ;
	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;

	if(SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0))
	{
		_iIndex = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCURSEL,0,0) ;
		_iValue = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETITEMDATA,_iIndex,0) ;

		lpCurrentFilter = ReadFilter(_iValue,&CurrentFilter) ;
		SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,lpCurrentFilter->Rate,0) ;
		ChangeEditorValues(lpCurrentFilter->ColsNumber) ;
	}

	_bFound = FALSE ;

	_i = 0 ;

	while(TRUE)
	{
		_iIndex = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETITEMDATA,_i,0) ;
		if(_iIndex == CB_ERR)
			break ;

		if(lpCurrentFilter->ColsNumber == _iIndex)
		{
			SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,_i,0) ;
			_bFound = TRUE ;
			break ;
		}

		_i++ ;
	}

	if(_bFound == FALSE)
	{
		SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_SETCURSEL,0,0) ;
		lpCurrentFilter->ColsNumber = (int) SendDlgItemMessage(_hWorkSpace,COMBO_SIZES,CB_GETITEMDATA,0,0) ;
	}

	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

//	***********************************
//	*** Apply the corresponding led ***
//	***********************************

	Child2_SetRedOrGreen() ;

	ApplyFilter(FALSE) ;

	return ;
}

int DeleteFilter(void)
{
	HSTMT			_hFind ;
	HWND			_hWorkSpace ;
	int				_iIndex, _iValue, _iCount, _iRes ;
	LPSTR			_lpszError ;
	char			_szTmp[256], _szError[1024] ;

	Log("Entering DeleteFilter") ;

	if(SendDlgItemMessage(hTabsWindows[HWND_WORKSPACE],LISTBOX_MODELS,LB_GETCOUNT,0,0) == 0)
		return (Error(1)) ;

	if(lpCurrentFilter->Model == 1)
		return (Error(21)) ;

	PushFilter() ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	if(SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0) == 0)
	{
		NewFilter() ;

		Log("The user wants to delete a filter but the listbox is empty. CoFeE has created a new filter.") ;
		return (Error(1)) ;
	}

	_iIndex = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCURSEL,0,0) ;
	_iValue = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETITEMDATA,_iIndex,0) ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,
			"UPDATE OR IGNORE [Asteroids] SET Keccak_Star=\"(None)\", FreeStar=1 WHERE [StarRecordNumber] = %d;",lpCurrentFilter->StarRecordNumber) ;

	Log("Freeing the asteroid name") ;
	Log(_szTmp) ;

	do
	{
		_iRes = sqlite3_exec(hCfeDb,_szTmp,0,0,&_lpszError) ;
	} while(_iRes == SQLITE_BUSY) ;

	if(_iRes == SQLITE_OK)
		Log("Asteroid name freed") ;
	else
	{
		Log(_lpszError) ;
		Log(Get_SQLite_Error(_szError)) ;

		Log("Error while freeing asteroid name") ;
		Log("This asteroid will not be used because [FreeStar] flag is equal to '0'") ;
		Log("The user must set this field to '1'") ;

		return (Error(81)) ;
	}

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"DELETE FROM Filters WHERE [FilterRecordNumber] = %d;\n",_iValue) ;

	Log(_szTmp) ;

	if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,(const char **) &_lpszError) != SQLITE_OK)
	{
		Log(_lpszError) ;
		Log("Error 52 while excuting the previous query") ;

		return (Error(52)) ;
	}

	do
	{
		_iRes = sqlite3_step(_hFind) ;
	} while(_iRes == SQLITE_BUSY) ;

	if(_iRes != SQLITE_DONE)
	{
		Log(Get_SQLite_Error(_szError)) ;

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		Log("Not possible to delete this filter. Error 12") ;

		return (Error(12)) ;
	}

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	_hFind = NULL ;

	Log("Filter deleted from Filters table") ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"DELETE FROM Convolution WHERE [FilterRecordNumber] = %d;\n",_iValue) ;

	Log(_szTmp) ;

	if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,(const char **) &_lpszError) != SQLITE_OK)
	{
		Log(_lpszError) ;
		Log("Error 52 while excuting the previous query") ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_DONE)
	{
		Log(Get_SQLite_Error(_szError)) ;

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		Log("Not possible to delete this filter in Convolution table. Error 12") ;

		return (Error(12)) ;
	}

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	Log("Filter deleted from Convolution table") ;

	SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_DELETESTRING,_iIndex,0) ;
	SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_SETCURSEL,_iIndex,0) ;

	Child2_SetRedOrGreen() ;

	lpCurrentFilter = ReadFilter((int) SendMessage(GetDlgItem(_hWorkSpace,LISTBOX_MODELS),LB_GETITEMDATA,_iIndex,0),&CurrentFilter) ;
	if(lpCurrentFilter)
		ChangeFilter(lpCurrentFilter) ;
	else
		Log("Cannot read this filter") ;

	_iCount = (int) SendDlgItemMessage(_hWorkSpace,LISTBOX_MODELS,LB_GETCOUNT,0,0) ;

	if(_iCount > 1)	sqlite3_snprintf(sizeof(_szTmp),_szTmp,Formats(2),_iCount) ;
	else if(_iCount == 1)	lstrcpy(_szTmp,Strings(4)) ;
	else if(_iCount == 0)	lstrcpy(_szTmp,Strings(5)) ;

	SendDlgItemMessage(hToolBox,IDC_STATUSBAR,SB_SETTEXT,SBT_NOBORDERS,(LPARAM) _szTmp) ;

	return (_iCount) ;
}

void ResetPaintAllWindows(void)
{
	register	LPIMAGEINFOS	_lpImageInfos ;
	register	LPBYTE			_lpTemp ;

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(_lpImageInfos)
	{
		_lpTemp = GetBitsFromBitmap(_lpImageInfos->hImgBitmap,NULL,_lpImageInfos->lpWork) ;
		if(_lpTemp)
		{
			_lpImageInfos->lpWork = _lpImageInfos->lpResult ;
			_lpImageInfos->lpResult = _lpTemp ;
		}

		PaintResultWindow() ;
	}

	return ;
}

void NewFilter(void)
{
	register	HWND	_hCombo ;
	register	int		_iIndex ;
				HWND	_hWorkSpace ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	_hCombo = GetDlgItem(_hWorkSpace,COMBO_SIZES) ;
	if(_hCombo == NULL)		return ;

	_iIndex = (int) SendMessage(_hCombo,CB_GETCURSEL,0,0) ;
	if(_iIndex == CB_ERR)	return ;

	_iIndex = (int) SendMessage(_hCombo,CB_GETITEMDATA,_iIndex,0) ;
	if(_iIndex == CB_ERR)	return ;

	lpCurrentFilter = &CurrentFilter ;

	RtlSecureZeroMemory(lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,lpCurrentFilter->Rate,0) ;
	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;

	ChangeFilterSize(_iIndex,TRUE) ;

	return ;
}

void SaveAllTheFilters(void)
{
	register	LPIMAGEINFOS	_lpImageInfos ;
				int				_iIndex, _iCount, _iValue ;
				HWND			_hListBox ;
				HWND			_hWorkSpace ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	if(!_lpImageInfos)
	{
		Error(2) ;
		return ;
	}

	if(SendDlgItemMessage(_hWorkSpace,LISTBOX_RNG_LIMITS,LB_GETCURSEL,0,0) == LB_ERR)
	{
		Error(46) ;
		return ;
	}

	if(Questions(1,szQuestion_01) == IDNO)
		return ;

	SetThreadPriority(GetCurrentThread(),THREAD_MODE_BACKGROUND_BEGIN) ;

	_hListBox = GetDlgItem(_hWorkSpace,LISTBOX_MODELS) ;

	_iCount = (int) SendMessage(_hListBox,LB_GETCOUNT,0,0) ;
	if(_iCount == 0)
	{
		Error(1) ;
		return ;
	}

	ClearAllFields() ;
	ChangeFilterSize(FALSE,TRUE) ;

	_iIndex = (int) SendMessage(_hListBox,LB_GETCURSEL,0,0) ;
	if(_iIndex == LB_ERR)
		_iIndex = 0 ;

	SendMessage(_hListBox,LB_SETCURSEL,_iIndex,0) ;

	for( ; _iIndex < _iCount ; _iIndex++)
	{
		if(SendMessage(_hListBox,LB_SETCURSEL,_iIndex,0) == LB_ERR)
			break ;

		_iValue = (int) SendMessage(_hListBox,LB_GETITEMDATA,_iIndex,0) ;
		if(_iValue == LB_ERR)
			break ;

		lpCurrentFilter = ReadFilter(_iValue,&CurrentFilter) ;
		if(!lpCurrentFilter)
			break ;

		lpCurrentFilter->Model = 1 ;

		ChangeFilter(lpCurrentFilter) ;
		SaveFilter(FALSE) ;
	}

	ClearAllFields() ;

	SendMessage(_hListBox,LB_SETCURSEL,0,0) ;

	CreateFilter() ;
	ChangeFilterSize(3,TRUE) ;

	SetThreadPriority(GetCurrentThread(),THREAD_MODE_BACKGROUND_END) ;

	DragAcceptFiles(hToolBox,TRUE) ;
	SendMessage(hStatus,(UINT) SB_SETTEXT,(WPARAM)(INT) 0,(LPARAM) (LPSTR) GetInformation(1)) ;
	Informations(1,szInformation_01) ;

	return ;
}

void OpenThisFile(void)
{
	OPENFILENAME	_Ofn ;
	LPIMAGEINFOS	_lpImageInfos ;
	char			_szFileName[1024], _szTmp[1024] ;

	RtlSecureZeroMemory(&_Ofn,sizeof(OPENFILENAME)) ;
	RtlSecureZeroMemory(_szFileName,sizeof(_szFileName)) ;

	_Ofn.lStructSize	=	sizeof(OPENFILENAME) ;
	_Ofn.hwndOwner		=	hToolBox ;
	_Ofn.lpstrFilter	=	"Images files (Bmp, Jpg, Png)\0*.bmp;*.jpg;*.png\0All files (*.*)\0*.*\0\0" ;
	_Ofn.nMaxCustFilter	=	0 ;
	_Ofn.nFilterIndex	=	1 ;
	_Ofn.lpstrFile		=	_szFileName ;
	_Ofn.nMaxFile		=	sizeof(_szFileName) ;
	_Ofn.Flags			=	OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NONETWORKBUTTON|OFN_PATHMUSTEXIST|OFN_NOREADONLYRETURN ;
	_Ofn.lpstrDefExt	=	"jpg" ;

	if(GetOpenFileName(&_Ofn))
	{
		_lpImageInfos = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
		if(_lpImageInfos)
		{
			ImageFree(_lpImageInfos) ;
			SetWindowLongPtr(hToolBox,GWLP_USERDATA,(long long int) NULL) ;
		}

		_lpImageInfos = LoadImageFromFile(_szFileName,MAX_EDIT_COLUMNS,MAX_EDIT_ROWS) ;
		if(!_lpImageInfos)
		{
			Error(6) ;
			return ;
		}

		_lpImageInfos->StructureSize = sizeof(IMAGEINFOS) ;

		SetWindowLongPtr(hToolBox,GWLP_USERDATA,(long long int) _lpImageInfos) ;

		lstrcpy(_szFileName,_lpImageInfos->Image.szImageFile) ;
		PathStripPath(_szFileName) ;
		sqlite3_snprintf(sizeof(_szTmp),_szTmp,"Convolution Filters Editor: [%s]",_szFileName) ;
		SetWindowText(hToolBox,_szTmp) ;

		sqlite3_snprintf(sizeof(_szTmp),_szTmp,"%lu x %lu",_lpImageInfos->BitmapInfo.bmiHeader.biWidth,_lpImageInfos->BitmapInfo.bmiHeader.biHeight) ;
		SendMessage(hStatus,(UINT) SB_SETTEXT,(WPARAM)(INT) 2,(LPARAM) (LPSTR) _szTmp) ;

		InvalidateRect(hTabsWindows[HWND_ORIGINAL],NULL,TRUE) ;
		InvalidateRect(hTabsWindows[HWND_WORKSPACE],NULL,TRUE) ;
		InvalidateRect(hTabsWindows[HWND_RESULTS],NULL,TRUE) ;
		InvalidateRect(hTabsWindows[HWND_DIAPORAMA],NULL,TRUE) ;

		bFirstDrop = TRUE ;
	}

	return ;
}

void PasteFromClipboard(void)
{
	LPIMAGEINFOS	_lpImageInfos, _lpImageInfos1 ;
	char			_szFileName[MAX_PATH + 4] ;
	char			_szTempPathBuffer[MAX_PATH + 4] ;

	if(!OpenClipboard(NULL))
	{
		Error(34) ;
		return ;
	}

	if(!IsClipboardFormatAvailable(CF_BITMAP))
	{
		if(!IsClipboardFormatAvailable(CF_DIB))
		{
			if(!IsClipboardFormatAvailable(CF_DIBV5))
			{
				Error(35) ;

				CloseClipboard() ;
				return ;
			}
		}
	}

	CloseClipboard() ;

	_lpImageInfos = LoadImageFromClipboard(MAX_EDIT_COLUMNS,MAX_EDIT_ROWS) ;
	if(!_lpImageInfos)
	{
		Error(36) ;

		return ;
	}

	_lpImageInfos1 = (LPIMAGEINFOS) GetWindowLongPtr(hToolBox,GWLP_USERDATA) ;
	SetWindowLongPtr(hToolBox,GWLP_USERDATA,(long long int) _lpImageInfos) ;

	if(_lpImageInfos1)
		ImageFree(_lpImageInfos1) ;

	GetTempPath(MAX_PATH,_szTempPathBuffer) ;
	GetTempFileName(_szTempPathBuffer,"CLP",0,_lpImageInfos->Image.szImageFile) ;

	_lpImageInfos->StructureSize = sizeof(IMAGEINFOS) ;

	sqlite3_snprintf(sizeof(_szFileName),_szFileName,"Convolution Filters Editor: [%s]",_lpImageInfos->Image.szImageFile) ;
	SetWindowText(hToolBox,_szFileName) ;

	bFirstDrop = TRUE ;

	lpCurrentFilter->Rate = 0 ;

	SendMessage(GetDlgItem(hTabsWindows[HWND_WORKSPACE],COMBO_FILTER_RATING),CB_SETCURSEL,lpCurrentFilter->Rate,0) ;

	InvalidateRect(hTabsWindows[HWND_ORIGINAL],NULL,TRUE) ;
	InvalidateRect(hTabsWindows[HWND_WORKSPACE],NULL,TRUE) ;
	InvalidateRect(hTabsWindows[HWND_RESULTS],NULL,TRUE) ;
	InvalidateRect(hTabsWindows[HWND_DIAPORAMA],NULL,TRUE) ;

	return ;
}

void ResetFilter(void)
{
	HWND	_hWorkSpace ;

	_hWorkSpace = hTabsWindows[HWND_WORKSPACE] ;

	SetCursor(LoadCursor(NULL,IDC_WAIT)) ;

	PushFilter() ;

	SendMessage(hGrid,ZGM_AUTOSIZECOLONEDIT,TRUE,0) ;

	RtlSecureZeroMemory(lpCurrentFilter,sizeof(PGM_FILTERS)) ;

	lstrcpy(lpCurrentFilter->Keccak_Filter,Strings(2)) ;
	lstrcpy(lpCurrentFilter->Comment,Strings(1)) ;
	lstrcpy(lpCurrentFilter->StarName,Strings(2)) ;

	lpCurrentFilter->Divisor = 1 ;

	SetDlgItemText(_hWorkSpace,EDIT_COMMENT,lpCurrentFilter->Comment) ;
	SetDlgItemInt(_hWorkSpace,EDIT_DIVISOR,1,FALSE) ;
	SetDlgItemInt(_hWorkSpace,EDIT_BIAS,0,FALSE) ;

	SendMessage(GetDlgItem(_hWorkSpace,COMBO_FILTER_RATING),CB_SETCURSEL,0,0) ;

	ChangeEditorValues(lpCurrentFilter->ColsNumber ) ;

	ResetPaintAllWindows() ;

	return ;
}

void RequestFilter(void)
{
	register	int		_iIndex ;
	register	int		_iRadio ;

				int		_iSize, _iRecordNumber, _iCount ;

				HWND	_hWnd, _hCombo, _hListBox ;
				HSTMT	_hFind ;

				LPSTR	_lpszTmp ;

				char	_szTmp[1024] ;
				char	_szRequest[1024] ;

	_hWnd = hTabsWindows[HWND_WORKSPACE] ;
	_iRadio = 0 ;	// Suppose RADIO_1_BLACK_WHITE is unchecked

	_hCombo = GetDlgItem(_hWnd,COMBO_SIZES) ;

	_iIndex = (int) SendMessage(_hCombo,CB_GETCURSEL,0,0) ;
	if(_iIndex == CB_ERR)	return ;

	_iSize = (int) SendMessage(_hCombo,CB_GETITEMDATA,_iIndex,0) ;
	if(_iSize == CB_ERR)	return ;

	_hListBox = GetDlgItem(_hWnd,LISTBOX_MODELS) ;
	SendMessage(_hListBox,LB_RESETCONTENT,0,0) ;
/*
	32	16	8	4	2	1
	5	4	3	2	1	0
	-----------------------
			RRU			RBW
			RRU		RB
			RRU	RW
		RR				RBW
		RR			RB
		RR		RW
	RU					RBW
	RU				RB
	RU			RW

	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE ([ColsNumber] == 3);

	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE  ([Size] = %d);										//	RRU + RBW
	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE (([Size] = %d) AND ([BW] < 1));						//	RRU + RB
	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE (([Size] = %d) AND ([BW] > 0));						//	RRU + RW

	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE (([Size] = %d) AND ([Rate] > 0));					//	RR + RBW
	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE (([Size] = %d) AND ([Rate] > 0) AND ([BW] < 1));	//	RR + RB
	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE (([Size] = %d) AND ([Rate] > 0) AND ([BW] > 0));	//	RR + RW

	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE (([Size] = %d) AND ([Rate] < 1));					//	RU + RBW
	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE (([Size] = %d) AND ([Rate] < 1) AND ([BW] < 1));	//	RU + RB
	SELECT Filters.FilterRecordNumber,Filters.StarRecordNumber, Asteroids.StarName FROM [Filters] INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber WHERE (([Size] = %d) AND ([Rate] < 1) AND ([BW] > 0));	//	RU + RW

	-----------------
	--- ***** = 1 ---
	--- ****  = 2 ---
	--- ***   = 3 ---
	--- **    = 4 ---
	--- *     = 5 ---
	--- 0     = 0 ---
	-----------------
*/
	_lpszTmp = _szTmp ;

	if(IsDlgButtonChecked(_hWnd,RADIO_1_BLACK_WHITE) == BST_CHECKED)
	{
		//	RADIO_1_BLACK_WHITE

		if(IsDlgButtonChecked(_hWnd,RADIO_4_RATED_UNRATED) == BST_CHECKED)
		{
			//	RADIO_1_BLACK_WHITE + RADIO_4_RATED_UNRATED => SIZE

			*_lpszTmp = '\0' ;
		}
		else
		{
			//	RADIO_1_BLACK_WHITE

			if(IsDlgButtonChecked(_hWnd,RADIO_5_RATED) == BST_CHECKED)
			{
				//	RADIO_1_BLACK_WHITE + RADIO_5_RATED

				_lpszTmp = "[Rate] > 0" ;
			}
			else
			{
				//	RADIO_1_BLACK_WHITE + RADIO_6_UNRATED

				_lpszTmp = "[Rate] == 0" ;
			}
		}
	}
	else
	{
		if(IsDlgButtonChecked(_hWnd,RADIO_2_BLACK) == BST_CHECKED)
		{
			//	RADIO_2_BLACK

			if(IsDlgButtonChecked(_hWnd,RADIO_4_RATED_UNRATED) == BST_CHECKED)
			{
				//	RADIO_2_BLACK + RADIO_4_RATED_UNRATED => RADIO_2_BLACK

				_lpszTmp = "[BlackAndWhite] < 1" ;
			}
			else
			{
				//	RADIO_2_BLACK

				if(IsDlgButtonChecked(_hWnd,RADIO_5_RATED) == BST_CHECKED)
				{
					//	RADIO_2_BLACK + RADIO_5_RATED

					_lpszTmp = "([BlackAndWhite] < 1) AND ([Rate] > 0)" ;
				}
				else
				{
					//	RADIO_2_BLACK + RADIO_6_UNRATED

					_lpszTmp = "([BlackAndWhite] < 1) AND ([Rate] == 0)" ;
				}
			}
		}
		else
		{
			//	RADIO_3_WHITE

			if(IsDlgButtonChecked(_hWnd,RADIO_4_RATED_UNRATED) == BST_CHECKED)
			{
				//	RADIO_3_WHITE + RADIO_4_RATED_UNRATED = RADIO_3_WHITE

				_lpszTmp = "[BlackAndWhite] > 0" ;
			}
			else
			{
				//	RADIO_3_WHITE

				if(IsDlgButtonChecked(_hWnd,RADIO_5_RATED) == BST_CHECKED)
				{
					//	RADIO_3_WHITE + RADIO_5_RATED

					_lpszTmp = "([BlackAndWhite] > 0) AND ([Rate] > 0)" ;
				}
				else
				{
					//	RADIO_3_WHITE + RADIO_6_UNRATED

					_lpszTmp = "([BlackAndWhite] > 0) AND ([Rate] == 0)" ;
				}
			}
		}
	}

	if(*_lpszTmp)
	{
		sqlite3_snprintf(sizeof(_szRequest),_szRequest,
					"SELECT Filters.FilterRecordNumber,Asteroids.StarName FROM [Filters] "
					"INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber "
					"WHERE (([ColsNumber] == %d) AND (%w)) "
					"ORDER BY Asteroids.StarName;",_iSize,_lpszTmp) ;
	}
	else
	{
		sqlite3_snprintf(sizeof(_szRequest),_szRequest,
					"SELECT Filters.FilterRecordNumber,Asteroids.StarName FROM [Filters] "
					"INNER JOIN [Asteroids] ON Filters.StarRecordNumber = Asteroids.StarRecordNumber "
					"WHERE ([ColsNumber] == %d) "
					"ORDER BY Asteroids.StarName;",_iSize,_lpszTmp) ;
	}

	if(sqlite3_prepare_v2(hCfeDb,_szRequest,-1,&_hFind,NULL) == SQLITE_OK)
	{
		while(sqlite3_step(_hFind) == SQLITE_ROW)
		{
			_iRecordNumber = sqlite3_column_int(_hFind,0) ;
			 _lpszTmp = (LPSTR) sqlite3_column_text(_hFind,1) ;
			_iIndex = (int) SendMessage(_hListBox,LB_ADDSTRING,0,(LPARAM) _lpszTmp) ;
			SendMessage(_hListBox,LB_SETITEMDATA,_iIndex,_iRecordNumber) ;
		}

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		_iCount = (int) SendMessage(_hListBox,LB_GETCOUNT,0,0) ;

		if(_iCount > 1)	sqlite3_snprintf(sizeof(_szTmp),_szTmp,Formats(2),_iCount) ;
		else if(_iCount == 1)	lstrcpy(_szTmp,Strings(4)) ;
		else if(_iCount == 0)	lstrcpy(_szTmp,Strings(5)) ;

		SendDlgItemMessage(hToolBox,IDC_STATUSBAR,SB_SETTEXT,SBT_NOBORDERS,(LPARAM) _szTmp) ;

		SendMessage(_hListBox,LB_SETCURSEL,0,0) ;

		return ;
	}

	Error(52) ;

	return ;
}

int WriteFilter(HANDLE __hFilter,LPPGM_FILTERS __lpFilter)
{
	DWORD	_dwTmp ;
	int		_iLen ;

	sqlite3_snprintf(sizeof(szBigBuffer),szBigBuffer,
							"\t\t\t\t\t\t\t\t\t{%lu,%lu,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\",\"%w\",\"%w\"},\n",
							__lpFilter->FilterRecordNumber,
							__lpFilter->StarRecordNumber,
							__lpFilter->Model,
							__lpFilter->Rate,
							__lpFilter->ColsNumber,
							__lpFilter->TotalColsNumber,
							__lpFilter->Divisor,
							__lpFilter->Bias,
							__lpFilter->Grey,
							__lpFilter->BlackAndWhite,
							__lpFilter->Keccak_Filter,
							__lpFilter->StarName,
							__lpFilter->Comment) ;

	_dwTmp = 0 ;
	_iLen = lstrlen(szBigBuffer) ;

	if(!FileWrite(__hFilter,szBigBuffer,_iLen))
		return (Error(23)) ;

	return (TRUE) ;
}

int WriteConvolution(HANDLE __hConvolution,LPPGM_FILTERS __lpFilter)
{
	register	size_t	_i ;
	register	LPSTR	_lpszTmp ;
				DWORD	_dwTmp ;
				int		_iLen, _k ;

	sqlite3_snprintf(sizeof(szBigBuffer),szBigBuffer,"{%lu,",__lpFilter->FilterRecordNumber) ;

	_dwTmp = 0 ;
	_lpszTmp = szBigBuffer + lstrlen(szBigBuffer) ;

	for(_i = 0 ; (int) _i < __lpFilter->TotalColsNumber ; _i++)
	{
		_k = sizeof(szBigBuffer) - lstrlen(szBigBuffer) ;
		sqlite3_snprintf(_k,_lpszTmp,"%d,",__lpFilter->Matrix[_i]) ;
		_lpszTmp += lstrlen(_lpszTmp) ;
	}

	lstrcpy(_lpszTmp - 1,"},\n") ;

	_iLen = lstrlen(szBigBuffer) ;

	if(!FileWrite(__hConvolution,szBigBuffer,_iLen))
		return (Error(24)) ;

	return (TRUE) ;
}

int SQLite_WriteConvolution(HANDLE __hSQLConvolution,LPPGM_FILTERS __lpFilter)
{
	DWORD	_dwTmp ;
	int		_iLen, _iLength, _i ;
	LPSTR	_lpszTmp ;

	_iLen = wsprintf(szBigBuffer,"INSERT INTO Convolution VALUES(%lu,",__lpFilter->FilterRecordNumber) ;

	_dwTmp = 0 ;

	_lpszTmp = szBigBuffer + _iLen ;

	for(_i = 0 ; _i < MAX_EDIT_FIELDS ; _i++)
	{
		_iLength = wsprintf(_lpszTmp,"%d,",__lpFilter->Matrix[_i]) ;
		_lpszTmp += _iLength ;
	}

	lstrcpy(_lpszTmp - 1,");\n") ;
	_iLen = lstrlen(szBigBuffer) ;

	if(!FileWrite(__hSQLConvolution,szBigBuffer,_iLen))
		return (Error(31)) ;

	return (TRUE) ;
}

int SQLite_WriteFilter(HANDLE __hSQLFilter,LPPGM_FILTERS __lpFilter)
{
	DWORD	_dwTmp ;
	int		_iLen ;

	sqlite3_snprintf(sizeof(szBigBuffer),szBigBuffer,
				"INSERT INTO Filters VALUES(%lu,%lu,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\",\"%w\",\"%w\");\n",
							__lpFilter->FilterRecordNumber,
							__lpFilter->StarRecordNumber,
							__lpFilter->Model,
							__lpFilter->Rate,
							__lpFilter->ColsNumber,
							__lpFilter->TotalColsNumber,
							__lpFilter->Divisor,
							__lpFilter->Bias,
							__lpFilter->Grey,
							__lpFilter->BlackAndWhite,
							__lpFilter->Keccak_Filter,
							__lpFilter->StarName,
							__lpFilter->Comment) ;

	_dwTmp = 0 ;
	_iLen = lstrlen(szBigBuffer) ;

	if(!FileWrite(__hSQLFilter,szBigBuffer,_iLen))
		return (Error(30)) ;

	return (TRUE) ;
}

void SQLite_WriteHeaders(HANDLE __hFilters,HANDLE __hConvolution,HANDLE __hSQLFilter,HANDLE __hSQLConvolution)
{
	int		_iLen ;
	LPSTR	_lpszBuffer ;

	_lpszBuffer =		"typedef	signed int INTEGER ;\n"
						"\n"
						"#define	MAX_EDIT_ROWS				29\n"
						"#define	MAX_EDIT_COLUMNS			29\n"
						"#define	MAX_EDIT_FIELDS				MAX_EDIT_ROWS * MAX_EDIT_COLUMNS\n"
						"\n"
						"#define	MAX_LENGTH_MASK				128\n"
						"#define	MAX_LENGTH_NAMES			64\n"
						"#define	MAX_KECCAK_LENGTH			256 / 4\n"
						"#define	MAX_FILTER_NAME_LENGTH		64\n"
						"#define	MAX_STAR_NAME_LENGTH		64\n"
						"#define	MAX_COMMENT_LENGTH			256\n"
						"\n"
						"#define	LEN_THEME_NAME				64\n"
						"\n"
						"#define	GREY_NONE					0\n"
						"#define	GREY_INPUT					1\n"
						"#define	GREY_OUTPUT					2\n"
						"#define	GREY_INOUT					3\n"
						"\n"
						"#define	MODEL_NO					0\n"
						"#define	MODEL_YES					1\n"
						"\n"
						"typedef	struct tagPGM_FILTERS\n"
						"{\n"
						"	DWORD			FilterRecordNumber ;\n"
						"	DWORD			StarRecordNumber ;\n"
						"	INTEGER			Model ;\n"
						"	INTEGER			Rate ;\n"
						"	INTEGER			ColsNumber ;\n"
						"	INTEGER			TotalColsNumber ;\n"
						"	INTEGER			Divisor ;\n"
						"	INTEGER			Bias ;\n"
						"	INTEGER			Grey ;\n"
						"	INTEGER			BlackAndWhite ;\n"
						"	char			Keccak_Filter[MAX_KECCAK_LENGTH + 4] ;\n"
						"	char			StarName[MAX_STAR_NAME_LENGTH + 4] ;\n"
						"	char			Comment[MAX_COMMENT_LENGTH + 4] ;\n"
						"	int				Matrix[MAX_EDIT_FIELDS] ;\n"
						"} PGM_FILTERS, *LPPGM_FILTERS ;\n\n" ;

	_iLen = lstrlen(_lpszBuffer) ;
	FileWrite(__hFilters,_lpszBuffer,_iLen) ;

	_lpszBuffer = "PGM_FILTERS RescueFilters[]	=	{\n" ;
	_iLen = lstrlen(_lpszBuffer) ;
	FileWrite(__hFilters,_lpszBuffer,_iLen) ;

	_lpszBuffer =		"typedef	signed int INTEGER ;\n"
						"\n"
						"#define	MAX_EDIT_ROWS				29\n"
						"#define	MAX_EDIT_COLUMNS			29\n"
						"#define	MAX_EDIT_FIELDS				MAX_EDIT_ROWS * MAX_EDIT_COLUMNS\n"
						"\n"
						"#define	MAX_LENGTH_MASK				128\n"
						"#define	MAX_LENGTH_NAMES			64\n"
						"#define	MAX_KECCAK_LENGTH			256 / 4\n"
						"#define	MAX_FILTER_NAME_LENGTH		64\n"
						"#define	MAX_STAR_NAME_LENGTH		64\n"
						"#define	MAX_COMMENT_LENGTH			256\n"
						"\n"
						"#define	LEN_THEME_NAME				64\n"
						"\n"
						"#define	GREY_NONE					0\n"
						"#define	GREY_INPUT					1\n"
						"#define	GREY_OUTPUT					2\n"
						"#define	GREY_INOUT					3\n"
						"\n"
						"#define	MODEL_NO					0\n"
						"#define	MODEL_YES					1\n"
						"\n"
						"typedef	struct tagPGM_CONVOLUTION\n"
						"{\n"
						"	DWORD			FilterRecordNumber ;\n"
						"	INTEGER			Matrix[MAX_EDIT_FIELDS] ;\n"
						"} PGM_CONVOLUTION, *LPPGM_CONVOLUTION ;\n\n" ;

	_iLen = lstrlen(_lpszBuffer) ;
	FileWrite(__hConvolution,_lpszBuffer,_iLen) ;

	_lpszBuffer = "PGM_CONVOLUTION RescueConvolution[]	=	{\n" ;
	_iLen = lstrlen(_lpszBuffer) ;
	FileWrite(__hConvolution,_lpszBuffer,_iLen) ;

	_lpszBuffer = "BEGIN TRANSACTION;\n" ;

	FileWrite(__hSQLFilter,_lpszBuffer,lstrlen(_lpszBuffer)) ;
	FileWrite(__hSQLConvolution,_lpszBuffer,lstrlen(_lpszBuffer)) ;

	return ;
}

int ExportFilters(void)
{
	HSTMT			_hFind ;
	HANDLE			_hFilter, _hConvolution ;
	HANDLE			_hSQLFilter, _hSQLConvolution ;
	LPPGM_FILTERS	_lpFilter ;
	LPSTR			_lpszBuffer ;
	int				_iFilterRecordNumber, _bRes, _iLen, _iCount ;
	char			_szBuffer[4096] ;
	PGM_FILTERS		_Filter ;

	_hFind = NULL ;

	sqlite3_snprintf(sizeof(_szBuffer),_szBuffer,"%s\\Export Filters.c",szExportDirectory) ;
	DeleteFile(_szBuffer) ;
	_hFilter = CreateFile(_szBuffer,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL) ;
	if(_hFilter == INVALID_HANDLE_VALUE)
		return (Error(13)) ;

	sqlite3_snprintf(sizeof(_szBuffer),_szBuffer,"%s\\Export Convolution.c",szExportDirectory) ;
	DeleteFile(_szBuffer) ;
	_hConvolution = CreateFile(_szBuffer,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL) ;
	if(_hConvolution == INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hFilter) ;

		return (Error(19)) ;
	}

	sqlite3_snprintf(sizeof(_szBuffer),_szBuffer,"%s\\Export Filters.sql",szExportDirectory) ;
	DeleteFile(_szBuffer) ;
	_hSQLFilter = CreateFile(_szBuffer,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL) ;
	if(_hSQLFilter == INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hConvolution) ;
		CloseHandle(_hFilter) ;

		return (Error(26)) ;
	}

	sqlite3_snprintf(sizeof(_szBuffer),_szBuffer,"%s\\Export Convolution.sql",szExportDirectory) ;
	DeleteFile(_szBuffer) ;
	_hSQLConvolution = CreateFile(_szBuffer,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL) ;
	if(_hSQLConvolution == INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hSQLFilter) ;
		CloseHandle(_hConvolution) ;
		CloseHandle(_hFilter) ;

		return (Error(32)) ;
	}

	SQLite_WriteHeaders(_hFilter,_hConvolution,_hSQLFilter,_hSQLConvolution) ;

	if(sqlite3_prepare_v2(hCfeDb,"SELECT [FilterRecordNumber] FROM [Filters] ORDER BY [TotalColsNumber];",-1,&_hFind,NULL) != SQLITE_OK)
		return (Error(52)) ;

	_iFilterRecordNumber = 0 ;
	_iCount = 0 ;

	while(sqlite3_step(_hFind) == SQLITE_ROW)
	{
		_iFilterRecordNumber = sqlite3_column_int(_hFind,0) ;

		_lpFilter = ReadFilter(_iFilterRecordNumber,&_Filter) ;
		if(_lpFilter)
		{
			if(_lpFilter->ColsNumber == 0)
				_lpFilter->ColsNumber = 3 ;

			if(_lpFilter->TotalColsNumber == 0)
				_lpFilter->TotalColsNumber = _lpFilter->ColsNumber * _lpFilter->ColsNumber ;

			_bRes = WriteFilter(_hFilter,_lpFilter) ;
			_bRes &= WriteConvolution(_hConvolution,_lpFilter) ;
			_bRes &= SQLite_WriteFilter(_hSQLFilter,_lpFilter) ;
			_bRes &= SQLite_WriteConvolution(_hSQLConvolution,_lpFilter) ;

			if(_bRes == 0)
			{
				FlushFileBuffers(_hFilter) ;
				FlushFileBuffers(_hConvolution) ;
				FlushFileBuffers(_hSQLFilter) ;
				FlushFileBuffers(_hSQLConvolution) ;

				CloseHandle(_hFilter) ;
				CloseHandle(_hConvolution) ;
				CloseHandle(_hSQLFilter) ;
				CloseHandle(_hSQLConvolution) ;

				sqlite3_reset(_hFind) ;
				sqlite3_finalize(_hFind) ;

				return (FALSE) ;
			}

			_iCount++ ;

			if(_iCount == 100)
			{
				_lpszBuffer = "COMMIT;\n\nBEGIN TRANSACTION;\n" ;
				_iLen = lstrlen(_lpszBuffer) ;

				FileWrite(_hSQLFilter,_lpszBuffer,_iLen) ;
				FileWrite(_hSQLConvolution,_lpszBuffer,_iLen) ;

				_iCount = 0 ;
			}
		}
	}

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	_lpszBuffer = "\t\t\t\t\t\t\t\t\t{0}\n\t\t\t\t\t\t\t\t} ;\n\n" ;
	_iLen = lstrlen(_lpszBuffer) ;

	FileWrite(_hFilter,_lpszBuffer,_iLen) ;

	_lpszBuffer = "{0}\n} ;\n\n" ;
	_iLen = lstrlen(_lpszBuffer) ;

	FileWrite(_hConvolution,_lpszBuffer,_iLen) ;

	_lpszBuffer = "COMMIT;\n" ;
	_iLen = lstrlen(_lpszBuffer) ;

	FileWrite(_hSQLFilter,_lpszBuffer,_iLen) ;
	FileWrite(_hSQLConvolution,_lpszBuffer,_iLen) ;

	FlushFileBuffers(_hFilter) ;
	FlushFileBuffers(_hConvolution) ;
	FlushFileBuffers(_hSQLFilter) ;
	FlushFileBuffers(_hSQLConvolution) ;

	CloseHandle(_hFilter) ;
	CloseHandle(_hConvolution) ;
	CloseHandle(_hSQLFilter) ;
	CloseHandle(_hSQLConvolution) ;

	return (TRUE) ;
}

void InitializeTheTables(void)
{
	register	size_t		_i ;

	_i = 0 ;

	while(RescueFilters[_i].FilterRecordNumber)
	{
		lpCurrentFilter = &RescueFilters[_i] ;
		memcpy(lpCurrentFilter->Matrix,RescueConvolution[_i].Matrix,sizeof(RescueConvolution[_i].Matrix)) ;

		SaveDatas() ;

		_i++ ;
	}

	return ;
}

int Filter_SetLocked(DWORD __dwRecordNumber)
{
	HSTMT		_hFind ;
	char		_szTmp[1024] ;

	_hFind = NULL ;

	Log("Entering Filter_SetLocked") ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"UPDATE Filters SET Model=1 WHERE [FilterRecordNumber] = %d;\n",__dwRecordNumber) ;

	Log(_szTmp) ;

	if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,NULL) != SQLITE_OK)
	{
		Log("Error 52 while executing the previous query") ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_DONE)
	{
		Log("Cannot update Filters table. Error 49.") ;

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		return (Error(49)) ;
	}

	Log("The filter has been locked") ;

	lpCurrentFilter->Model = 1 ;

//	***********************************
//	*** Apply the corresponding led ***
//	***********************************

	Child2_SetRedOrGreen() ;

	return (TRUE) ;
}

int Filter_SetUnLocked(DWORD __dwRecordNumber)
{
	HSTMT		_hFind ;
	char		_szTmp[1024] ;

	Log("Entering Filter_SetUnLocked") ;

	_hFind = NULL ;

	sqlite3_snprintf(sizeof(_szTmp),_szTmp,"UPDATE Filters SET Model=0 WHERE [FilterRecordNumber] = %d;\n",__dwRecordNumber) ;

	Log(_szTmp) ;

	if(sqlite3_prepare_v2(hCfeDb,_szTmp,-1,&_hFind,NULL) != SQLITE_OK)
	{
		Log("Error 52 while executing the previous query") ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_DONE)
	{
		Log("Cannot update Filters table. Error 49.") ;

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		return (Error(49)) ;
	}

	Log("The filter has been unlocked") ;

	lpCurrentFilter->Model = 0 ;

//	***********************************
//	*** Apply the corresponding led ***
//	***********************************

	Child2_SetRedOrGreen() ;

	return (TRUE) ;
}

int SetNewValuesIntoTheTables(void)
{
	register	int			_i ;
				HSTMT		_hFind ;
				DWORD		_dwFilterRecordNumber ;

	_i = 0 ;
	lpCurrentFilter = &CurrentFilter ;

	if(sqlite3_prepare_v2(hCfeDb,"SELECT [FilterRecordNumber] FROM Filters;",-1,&_hFind,NULL) == SQLITE_OK)
	{
		while(sqlite3_step(_hFind) == SQLITE_ROW)
		{
			_dwFilterRecordNumber = sqlite3_column_int(_hFind,0) ;

			lpCurrentFilter = ReadFilter(_dwFilterRecordNumber,&CurrentFilter) ;
			if(!lpCurrentFilter)
				break ;

			lpCurrentFilter->Model = 1 ;	// Lock imported record

			SaveDatas() ;

			_i++ ;
		}

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		return (TRUE) ;
	}

	return (Error(52)) ;
}

int Filters_Import(void)
{
	register	LPBYTE	_lpFilters, _lpConvolution ;

	int			_i, _k ;
	int			_iRandom_Nom, _iRandom_Prenom, _iError ;

	HANDLE		_hFilters ;
	HANDLE		_hConvolution ;

	HSTMT		_hFind ;

	LPSTR		_lpszError ;

	DWORD		_dwFiltersSize, _dwConvolutionSize ;
	DWORD		_dwFilters, _dwConvolution ;

	char		_szResult[1024] ;
	char		szFile_Filters[MAX_PATH + 4] ;
	char		szFile_Convolution[MAX_PATH + 4] ;

	NOM_PRENOMS	_NamesAndSurnames ;

	if(BrowseForFolder(hToolBox,_szResult,Strings(14)) == NULL)
		return (FALSE) ;

	hModless = (HWND) CreateDialog(hInstance,MAKEINTRESOURCE(DLG_MODLESS),hToolBox,(DLGPROC) DlgModlessProc) ;
	if(hModless == NULL)
		return (FALSE) ;

	ShowWindow(hModless,SW_SHOW) ;

	lstrcpy(szDlgStatic_01,Strings(12)) ;
	lstrcpy(szDlgStatic_02,Strings(13)) ;

	SendMessage(hModless,WM_COMMAND,IDM_SET_TEXT_01,(LPARAM) NULL) ;
	SendMessage(hModless,WM_COMMAND,IDM_SET_TEXT_02,(LPARAM) NULL) ;

	sqlite3_close_v2(hCfeDb) ;
	hCfeDb = NULL ;

//	*****************************
//	*** Build the files names ***
//	*****************************

	sqlite3_snprintf(sizeof(szFile_Filters),szFile_Filters,"%w\\Filters.sql",_szResult) ;
	sqlite3_snprintf(sizeof(szFile_Convolution),szFile_Convolution,"%w\\Convolution.sql",_szResult) ;

//	***************************************************************
//	*** Verify that the two files exist in the import directory ***
//	***************************************************************

	if(!PathFileExists(szFile_Filters))
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(61)) ;		// Import\\Filters.sql does not exist
	}

	if(!PathFileExists(szFile_Filters))
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(62)) ;		// Import\\Convolution.sql does not exist
	}

//	**************************
//	*** Open the two files ***
//	**************************

	_hFilters = CreateFile(szFile_Filters,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL) ;
	if(_hFilters == INVALID_HANDLE_VALUE)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(63)) ;
	}

	_hConvolution = CreateFile(szFile_Convolution,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL) ;
	if(_hFilters == INVALID_HANDLE_VALUE)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		CloseHandle(_hFilters) ;
		return (Error(64)) ;
	}

//	***********************************************
//	*** Verify that the two files are not empty ***
//	***********************************************

	_dwFiltersSize = GetFileSize(_hFilters,NULL) ;
	if(_dwFiltersSize == 0)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		CloseHandle(_hFilters) ;
		CloseHandle(_hConvolution) ;

		return (Error(65)) ;
	}

	_dwConvolutionSize = GetFileSize(_hConvolution,NULL) ;
	if(_dwConvolutionSize == 0)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		CloseHandle(_hFilters) ;
		CloseHandle(_hConvolution) ;

		return (Error(66)) ;
	}

//	*****************************************
//	*** Allocate memory for the two files ***
//	*****************************************

	_lpFilters = PhR_Alloc_Memory(_dwFiltersSize) ;
	if(_lpFilters == NULL)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		CloseHandle(_hFilters) ;
		CloseHandle(_hConvolution) ;

		return (Error(67)) ;
	}

	_lpConvolution = PhR_Alloc_Memory(_dwConvolutionSize) ;
	if(_lpConvolution == NULL)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		PhR_Free_Memory(_lpFilters) ;

		CloseHandle(_hFilters) ;
		CloseHandle(_hConvolution) ;

		return (Error(68)) ;
	}

//	**************************
//	*** Read the two files ***
//	**************************

	if(!ReadFile(_hFilters,_lpFilters,_dwFiltersSize,&_dwFilters,NULL))
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		PhR_Free_Memory(_lpFilters) ;
		PhR_Free_Memory(_lpConvolution) ;

		CloseHandle(_hFilters) ;
		CloseHandle(_hConvolution) ;

		return (Error(69)) ;
	}

	if(_dwFiltersSize != _dwFilters)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		PhR_Free_Memory(_lpFilters) ;
		PhR_Free_Memory(_lpConvolution) ;

		CloseHandle(_hFilters) ;
		CloseHandle(_hConvolution) ;

		return (Error(70)) ;
	}

	if(!ReadFile(_hConvolution,_lpConvolution,_dwConvolutionSize,&_dwConvolution,NULL))
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		PhR_Free_Memory(_lpFilters) ;
		PhR_Free_Memory(_lpConvolution) ;

		CloseHandle(_hFilters) ;
		CloseHandle(_hConvolution) ;

		return (Error(71)) ;
	}

	if(_dwConvolutionSize != _dwConvolution)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		PhR_Free_Memory(_lpFilters) ;
		PhR_Free_Memory(_lpConvolution) ;

		CloseHandle(_hFilters) ;
		CloseHandle(_hConvolution) ;

		return (Error(72)) ;
	}

//	***********************************************
//	*** Not useful to keep the two files opened ***
//	***********************************************

	CloseHandle(_hFilters) ;
	CloseHandle(_hConvolution) ;

	if(sqlite3_open_v2(szDatabaseFileName,&hCfeDb,SQLITE_OPEN_READWRITE,NULL) != SQLITE_OK)
	{
		PhR_Free_Memory(_lpFilters) ;
		PhR_Free_Memory(_lpConvolution) ;

		hCfeDb = NULL ;
		return (Error(51)) ;
	}

//	***************************
//	*** Run the two scripts ***
//	***************************

	sqlite3_exec(hCfeDb,"DROP TABLE IF EXISTS Filters;",NULL,NULL,NULL) ;
	sqlite3_exec(hCfeDb,szNewCreateFilters,NULL,NULL,NULL) ;

	if(sqlite3_exec(hCfeDb,(LPCSTR) _lpFilters,NULL,NULL,NULL) != SQLITE_OK)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		PhR_Free_Memory(_lpFilters) ;
		PhR_Free_Memory(_lpConvolution) ;

		return (Error(73)) ;
	}

	sqlite3_exec(hCfeDb,"DROP INDEX IF EXISTS PremierIndex;",NULL,NULL,NULL) ;
	sqlite3_exec(hCfeDb,"DROP TABLE IF EXISTS Convolution;",NULL,NULL,NULL) ;
	sqlite3_exec(hCfeDb,szNewCreateDatas,NULL,NULL,&_lpszError) ;

	if(sqlite3_exec(hCfeDb,(LPCSTR) _lpConvolution,NULL,NULL,NULL) != SQLITE_OK)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		PhR_Free_Memory(_lpFilters) ;
		PhR_Free_Memory(_lpConvolution) ;

		return (Error(74)) ;
	}

	sqlite3_exec(hCfeDb,"CREATE UNIQUE INDEX [PremierIndex] ON [Asteroids]([StarName] COLLATE [BINARY] ASC);\n\nBEGIN TRANSACTION;",NULL,NULL,NULL) ;

//	****************************
//	*** Free the two buffers ***
//	****************************

	PhR_Free_Memory(_lpFilters) ;
	PhR_Free_Memory(_lpConvolution) ;

	sqlite3_exec(hCfeDb,"VACUUM",0,0,NULL) ;
	sqlite3_db_cacheflush(hCfeDb) ;

//	********************************************************************
//	*** Close the database to flush all the records back to the disk ***
//	********************************************************************

	sqlite3_close_v2(hCfeDb) ;

//	***************************
//	*** Reopen the database ***
//	***************************

	if(sqlite3_open_v2(szDatabaseFileName,&hCfeDb,SQLITE_OPEN_READWRITE,NULL) != SQLITE_OK)
	{
		hCfeDb = NULL ;
		return (Error(51)) ;
	}

//	********************************
//	*** The file has been opened ***
//	********************************

	SQLitePragmas() ;

//	********************************************
//	*** Verify if the table "Filters" exists ***
//	********************************************

	if(sqlite3_prepare_v2(hCfeDb,"SELECT [name] FROM sqlite_master WHERE [name] LIKE \"Filters\"",-1,&_hFind,NULL) != SQLITE_OK)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_ROW)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(53)) ;
	}

//	***********************************
//	**** The table "Filters" exists ***
//	***********************************

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	_hFind = NULL ;

//	**********************************
//	*** Is the table empty or full ***
//	**********************************

//	---> Full means at least one record

	if(sqlite3_prepare_v2(hCfeDb,"SELECT COUNT([FilterRecordNumber]) FROM [Filters];",-1,&_hFind,NULL) != SQLITE_OK)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_ROW)		// La table est vide
	{
//		************************************
//		*** The table "Filters" is empty ***
//		************************************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(55)) ;
	}

	_dwFilters = sqlite3_column_int(_hFind,0) ;

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

//	************************************************
//	*** Verify if the table "Convolution" exists ***
//	************************************************

	if(sqlite3_prepare_v2(hCfeDb,"SELECT [name] FROM sqlite_master WHERE [name] LIKE \"Convolution\"",-1,&_hFind,NULL) != SQLITE_OK)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_ROW)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(54)) ;
	}

//	***************************************
//	**** The table "Convolution" exists ***
//	***************************************

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	_hFind = NULL ;

//	**********************************
//	*** Is the table empty or full ***
//	**********************************

//	---> Full means at least one record

	if(sqlite3_prepare_v2(hCfeDb,"SELECT COUNT([FilterRecordNumber]) FROM [Convolution];",-1,&_hFind,NULL) != SQLITE_OK)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_ROW)		// La table est vide
	{
//		****************************************
//		*** The table "Convolution" is empty ***
//		****************************************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(56)) ;
	}

	_dwConvolution = sqlite3_column_int(_hFind,0) ;

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

//	**********************************************
//	*** Verify if the table "Asteroids" exists ***
//	**********************************************

	if(sqlite3_prepare_v2(hCfeDb,"SELECT [name] FROM sqlite_master WHERE [name] LIKE \"Asteroids\"",-1,&_hFind,NULL) != SQLITE_OK)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_ROW)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(58)) ;
	}

//	*************************************
//	**** The table "Asteroids" exists ***
//	*************************************

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

	_hFind = NULL ;

//	**********************************
//	*** Is the table empty or full ***
//	**********************************

//	---> Full means at least one record

	if(sqlite3_prepare_v2(hCfeDb,"SELECT COUNT([StarRecordNumber]) FROM [Asteroids];",-1,&_hFind,NULL) != SQLITE_OK)
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(52)) ;
	}

	if(sqlite3_step(_hFind) != SQLITE_ROW)		// La table est vide
	{
//		**************************************
//		*** The table "Asteroids" is empty ***
//		**************************************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(59)) ;
	}

	if(sqlite3_column_int(_hFind,0) == 0)
	{
//		**************************************
//		*** The table "Asteroids" is empty ***
//		**************************************

		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(59)) ;
	}

//	********************************************
//	*** If there are less that 4 000 records ***
//	*** Consider the table is empty          ***
//	********************************************

	if(sqlite3_column_int(_hFind,0) < (MAX_ASTEROIDS - 100))
	{
		sqlite3_reset(_hFind) ;
		sqlite3_finalize(_hFind) ;

		sqlite3_close_v2(hCfeDb) ;

		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(59)) ;
	}

	sqlite3_reset(_hFind) ;
	sqlite3_finalize(_hFind) ;

//	****************************************************************
//	*** Verify if the two tables have the same number of records ***
//	****************************************************************

	if(_dwFilters == 0)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(77)) ;
	}

	if(_dwConvolution == 0)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(78)) ;
	}

	if(_dwFilters != _dwConvolution)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(79)) ;
	}

//	**********************************
//	*** Drop the table "Asteroids" ***
//	**********************************

	if(sqlite3_exec(hCfeDb,"DROP TABLE IF EXISTS Asteroids;",NULL,NULL,NULL) != SQLITE_OK)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(75)) ;
	}

//	************************************
//	*** Create the table "Asteroids" ***
//	************************************

	if(sqlite3_exec(hCfeDb,szCreateAsteroids,NULL,NULL,NULL) != SQLITE_OK)
	{
		DestroyWindow(hModless) ;
		hModless = NULL ;

		return (Error(76)) ;
	}

//	*****************************************
//	*** Fill the table Names and Surnames ***
//	*****************************************

	RtlSecureZeroMemory(&_NamesAndSurnames,sizeof(NOM_PRENOMS)) ;

	_NamesAndSurnames.Freedom = 1 ;

	_i = 0 ;
	_k = 0 ;

	while(_i < (MAX_ASTEROIDS + 100))
	{
		_k++ ;

		_NamesAndSurnames.StarRecordNumber = _k ;

		_iRandom_Nom = (int)((long long int) rand() % 1628) ;
		_iRandom_Prenom = (int)((long long int) rand() % 1783) ;

		sqlite3_snprintf(MAX_STAR_NAME_LENGTH,_NamesAndSurnames.StarName,"%w %w",Prenoms[_iRandom_Prenom].Name,NomsDeFamille[_iRandom_Nom].Name) ;

		Keccak((unsigned char *) _NamesAndSurnames.StarName,MAX_STAR_NAME_LENGTH,(unsigned char *) _szResult) ;
		Hex2Str((unsigned char *) _szResult,(unsigned char *) _NamesAndSurnames.Keccak,256 / 8) ;

		_iError = StoreNamesAndSurnames(&_NamesAndSurnames) ;
		if(_iError == -1)				// SQLITE_CONSTRAINT (19)
		{
//			************************
//			*** An error occured ***
//			************************

//				---> Generaly it is a contraint violation
//				---> The names and surnames have already been stored

			_i-- ;
			_k-- ;
		}
		else
		{
			if(_iError == FALSE)
			{
				sqlite3_close_v2(hCfeDb) ;
				hCfeDb = NULL ;

				DestroyWindow(hModless) ;
				hModless = NULL ;

				return (Error(19)) ;
			}
		}

		_i++ ;
	}

	SetNewValuesIntoTheTables() ;

	sqlite3_exec(hCfeDb,"VACUUM",0,0,NULL) ;

	DestroyWindow(hModless) ;
	hModless = NULL ;

	Informations(4,szInformation_04) ;

	return (TRUE) ;
}
