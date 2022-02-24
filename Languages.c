#define WIN32_LEAN_AND_MEAN  /* speed up compilations */
#include <windows.h>
#include <CommCtrl.h>
#include <shlwapi.h>

#pragma warn(disable:2006 2145 2215 2228 2805)

#include "sqlite3.h"

#include "CoFeE.h"
#include "Datas.h"
#include "Languages.h"

GetSerialNumberProc	GetSerialNumber ;
GetLanguageNameProc	GetLanguageName ;
GetTitleProc		GetTitle ;
GetStringProc		GetString ;
GetFormatProc		GetFormat ;
GetToolTipProc		GetToolTip ;
GetErrorProc		GetError ;
GetQuestionProc		GetQuestion ;
GetInformationProc	GetInformation ;
GetWarningProc		GetWarning ;
GetStaticTextProc	GetStaticText ;
GetFormatProc		GetFormat ;
GetButtonProc		GetButton ;
GetLanguageProc		GetLanguage ;

char	szErrorTitle[]			=	"Error" ;
char	szWarningTitle[]		=	"Warning" ;
char	szQuestionTitle[]		=	"Question" ;
char	szInformationTitle[]	=	"Information" ;

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

void UnLoadLanguageFile(void)
{
	GetSerialNumber			= NULL ;
	GetLanguageName 		= NULL ;
	GetTitle				= NULL ;
	GetString				= NULL ;
	GetFormat				= NULL ;
	GetToolTip				= NULL ;
	GetError				= NULL ;
	GetQuestion				= NULL ;
	GetInformation			= NULL ;
	GetWarning				= NULL ;
	GetStaticText			= NULL ;

	FreeLibrary(hLanguage) ;

	hLanguage = NULL ;

	return ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int LoadLanguageFile(LPSTR __lpszLanguageFile)
{
	if(!PathFileExists(__lpszLanguageFile))
	{
		hLanguage = NULL ;
		RtlSecureZeroMemory(szLanguageFile,sizeof(szLanguageFile)) ;

		return (Error(33)) ;
	}

	UnLoadLanguageFile() ;

	hLanguage = LoadLibrary(__lpszLanguageFile) ;
	if(hLanguage == NULL)
		return (Error(33)) ;

	GetSerialNumber = GetProcAddress(hLanguage,"CFE_GetSerialNumber") ;
	if(GetSerialNumber)
	{
		GetLanguageName = GetProcAddress(hLanguage,"CFE_GetLanguageName") ;
		if(GetLanguageName)
		{
			GetTitle = GetProcAddress(hLanguage,"CFE_GetTitle") ;
			if(GetTitle)
			{
				GetString = GetProcAddress(hLanguage,"CFE_GetString") ;
				if(GetString)
				{
					GetFormat = GetProcAddress(hLanguage,"CFE_GetFormat") ;
					if(GetFormat)
					{
						GetToolTip = GetProcAddress(hLanguage,"CFE_GetToolTip") ;
						if(GetToolTip)
						{
							GetError = GetProcAddress(hLanguage,"CFE_GetError") ;
							if(GetError)
							{
								GetQuestion = GetProcAddress(hLanguage,"CFE_GetQuestion") ;
								if(GetQuestion)
								{
									GetInformation = GetProcAddress(hLanguage,"CFE_GetInformation") ;
									if(GetInformation)
									{
										GetWarning = GetProcAddress(hLanguage,"CFE_GetWarning") ;
										if(GetWarning)
										{
											GetStaticText = GetProcAddress(hLanguage,"CFE_GetStaticText") ;
											if(GetStaticText)
											{
												GetButton = GetProcAddress(hLanguage,"CFE_GetButtons") ;
												if(GetButton)
												{
													GetLanguage = GetProcAddress(hLanguage,"CFE_GetLanguage") ;
													if(GetLanguage)
													{
														return (TRUE) ;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	UnLoadLanguageFile() ;

	return (Error(5)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR Titles(int __iTitleNumber)
{
	if(hLanguage == NULL)
		return (szEmptyString) ;

	return (GetTitle(__iTitleNumber)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR Strings(int __iStringsNumber)
{
	if(hLanguage == NULL)
		return (szEmptyString) ;

	return (GetString(__iStringsNumber)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR Buttons(int __iButtonNumber)
{
	if(hLanguage == NULL)
		return (szEmptyString) ;

	return (GetButton(__iButtonNumber)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR Formats(int __iFormatsNumber)
{
	if(hLanguage == NULL)
		return (szEmptyString) ;

	return (GetFormat(__iFormatsNumber)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int Questions(int __iQuestionNumber,LPSTR __lpszString)
{
	LPSTR	_lpszQuestionMessage ;
	LPSTR	_lpszQuestionTitle ;

	if(__iQuestionNumber == -1)
	{
		_lpszQuestionMessage = __lpszString ;

		if(hLanguage == NULL)
			_lpszQuestionTitle = szQuestionTitle ;
		else
			_lpszQuestionTitle = GetQuestion(0)  ;
	}
	else
	{
		if(hLanguage == NULL)
		{
			_lpszQuestionTitle = szQuestionTitle ;
			_lpszQuestionMessage = __lpszString ;
		}
		else
		{
			_lpszQuestionTitle = GetQuestion(0)  ;
			_lpszQuestionMessage = GetQuestion(__iQuestionNumber) ;
		}
	}

//	**************************
//	*** Emit a sound error ***
//	**************************

	MessageBeep(MB_ICONQUESTION) ;

//	***********************************
//	*** Display the translated text ***
//	***********************************

	return (MessageBox(hToolBox,_lpszQuestionMessage,_lpszQuestionTitle,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2|MB_SYSTEMMODAL|MB_TOPMOST)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int Warnings(int __iWarningNumber,LPSTR __lpszString)
{
	LPSTR	_lpszWarningMessage ;
	LPSTR	_lpszWarningTitle ;

	if(__iWarningNumber == -1)
	{
		_lpszWarningMessage = __lpszString ;

		if(hLanguage == NULL)
			_lpszWarningTitle = szWarningTitle ;
		else
			_lpszWarningTitle = GetWarning(0)  ;
	}
	else
	{
		if(hLanguage == NULL)
		{
			_lpszWarningTitle = szWarningTitle ;
			_lpszWarningMessage = __lpszString ;
		}
		else
		{
			_lpszWarningTitle = GetWarning(0)  ;
			_lpszWarningMessage = GetWarning(__iWarningNumber) ;
		}
	}

//	**************************
//	*** Emit a sound error ***
//	**************************

	MessageBeep(MB_ICONWARNING) ;

//	***********************************
//	*** Display the translated text ***
//	***********************************

	MessageBox(hToolBox,_lpszWarningMessage,_lpszWarningTitle,MB_OK|MB_ICONWARNING|MB_SYSTEMMODAL|MB_TOPMOST) ;

//	*****************************************
//	*** The function always returns FALSE ***
//	*****************************************

	return (FALSE) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int Informations(int __iInformationNumber,LPSTR __lpszString)
{
	LPSTR	_lpszInformationMessage ;
	LPSTR	_lpszInformationTitle ;

	if(__iInformationNumber == -1)
	{
		_lpszInformationMessage = __lpszString ;

		if(hLanguage == NULL)
			_lpszInformationTitle = szInformationTitle ;
		else
			_lpszInformationTitle = GetInformation(0)  ;
	}
	else
	{
		if(hLanguage == NULL)
		{
			_lpszInformationTitle = szInformationTitle ;
			_lpszInformationMessage = __lpszString ;
		}
		else
		{
			_lpszInformationTitle = GetInformation(0)  ;
			_lpszInformationMessage = GetInformation(__iInformationNumber) ;
		}
	}

//	**************************
//	*** Emit a sound error ***
//	**************************

	MessageBeep(MB_ICONINFORMATION) ;

//	***********************************
//	*** Display the translated text ***
//	***********************************

	MessageBox(hToolBox,_lpszInformationMessage,_lpszInformationTitle,MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL|MB_TOPMOST) ;

//	*****************************************
//	*** The function always returns FALSE ***
//	*****************************************

	return (TRUE) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int SpecialError(LPSTR __lpszString)
{
	FLASHWINFO	_Flash ;

	if(!__lpszString)
		return (0) ;

	Log(__lpszString) ;

//	********************************
//	*** Make the window flashing ***
//	********************************

	_Flash.cbSize = sizeof(FLASHWINFO) ;
	_Flash.hwnd = hToolBox ;
	_Flash.dwFlags = FLASHW_ALL ;
	_Flash.uCount = 8 ;
	_Flash.dwTimeout = 0 ;

	FlashWindowEx(&_Flash) ;

//	**************************
//	*** Emit a sound error ***
//	**************************

	MessageBeep(MB_ICONERROR) ;

//	***********************************
//	*** Display the translated text ***
//	***********************************

	MessageBox(hToolBox,__lpszString,"Special Error",MB_OK|MB_ICONERROR|MB_SYSTEMMODAL|MB_TOPMOST) ;

//	*****************************************
//	*** The function always returns FALSE ***
//	*****************************************

	return (FALSE) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

int Error(int __iErrorNumber)
{
	LPSTR	_lpszErrorMessage ;
	LPSTR	_lpszErrorTitle ;

	FLASHWINFO	_Flash ;

	if(hLanguage == NULL)
		return (-14789) ;

	_lpszErrorTitle = GetError(0)  ;
	_lpszErrorMessage = GetError(__iErrorNumber) ;

//	********************************
//	*** Make the window flashing ***
//	********************************

	_Flash.cbSize = sizeof(FLASHWINFO) ;
	_Flash.hwnd = hToolBox ;
	_Flash.dwFlags = FLASHW_ALL ;
	_Flash.uCount = 8 ;
	_Flash.dwTimeout = 0 ;

	FlashWindowEx(&_Flash) ;

//	**************************
//	*** Emit a sound error ***
//	**************************

	MessageBeep(MB_ICONERROR) ;

//	***********************************
//	*** Display the translated text ***
//	***********************************

	MessageBox(hToolBox,_lpszErrorMessage,_lpszErrorTitle,MB_OK|MB_ICONERROR|MB_SYSTEMMODAL|MB_TOPMOST) ;

//	*****************************************
//	*** The function always returns FALSE ***
//	*****************************************

	return (FALSE) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR Statics(int __iStaticsNumber)
{
	if(hLanguage == NULL)
		return (szEmptyString) ;

	return (GetStaticText(__iStaticsNumber)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR ToolTips(int __iToolTipsNumber)
{
	if(hLanguage == NULL)
		return (szEmptyString) ;

	return (GetToolTip(__iToolTipsNumber)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________

LPSTR Languages(int __iLanguageNumber)
{
	if(hLanguage == NULL)
		return (szEmptyString) ;

	return (GetLanguage(__iLanguageNumber)) ;
}

//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
//	_____________________________________________________________________________________________
