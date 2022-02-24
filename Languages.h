#ifndef _LANGUAGES_H
#define _LANGUAGES_H

typedef LPSTR (CALLBACK *GetLanguageNameProc)(void) ;
typedef LPSTR (CALLBACK *GetSerialNumberProc)(void) ;
typedef LPSTR (*GetTitleProc)(int) ;
typedef LPSTR (*GetStringProc)(int) ;
typedef LPSTR (*GetFormatProc)(int) ;
typedef LPSTR (*GetToolTipProc)(int) ;
typedef LPSTR (*GetErrorProc)(int) ;
typedef LPSTR (*GetQuestionProc)(int) ;
typedef LPSTR (*GetInformationProc)(int) ;
typedef LPSTR (*GetWarningProc)(int) ;
typedef LPSTR (*GetStaticTextProc)(int) ;
typedef LPSTR (*GetFormatProc)(int) ;
typedef LPSTR (*GetButtonProc)(int) ;
typedef LPSTR (*GetLanguageProc)(int) ;

extern GetSerialNumberProc	GetSerialNumber ;
extern GetLanguageNameProc	GetLanguageName ;
extern GetTitleProc			GetTitle ;
extern GetStringProc		GetString ;
extern GetFormatProc		GetFormat ;
extern GetToolTipProc		GetToolTip ;
extern GetErrorProc			GetError ;
extern GetQuestionProc		GetQuestion ;
extern GetInformationProc	GetInformation ;
extern GetWarningProc		GetWarning ;
extern GetStaticTextProc	GetStaticText ;
extern GetFormatProc		GetFormat ;
extern GetButtonProc		GetButton ;
extern GetLanguageProc		GetLanguage ;

int Error(int __iErrorNumber) ;
int SpecialError(LPSTR __lpszString) ;
int Warnings(int __iWarningNumber,LPSTR __lpszString) ;
int Questions(int __iQuestionNumber,LPSTR __lpszString) ;
int Informations(int __iInformationNumber,LPSTR __lpszString) ;

LPSTR Formats(int __iFormatsNumber) ;
LPSTR Strings(int __iStringsNumber) ;
LPSTR Buttons(int __iButtonNumber) ;
LPSTR Statics(int __iStaticsNumber) ;
LPSTR ToolTips(int __iToolTipsNumber) ;
LPSTR Titles(int __iTitleNumber) ;
LPSTR Languages(int __iLanguageNumber) ;

void UnLoadLanguageFile(void) ;
int LoadLanguageFile(LPSTR __lpszLanguageFile) ;

#endif /* _LANGUAGES_H */
