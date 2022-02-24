							INCLUDE Win_Constantes.inc
							INCLUDE Win_Typedefs.inc
							INCLUDE Win_Structures.inc
							INCLUDE Win_Functions.inc

Bin							PROTO	__qwNumber:QWORD,__lpszResult:LPSTR,__dwOutputLength:DWORD
Bin8						PROTO	__qwNumber:QWORD,__lpszResult:LPSTR
Bin16						PROTO	__qwNumber:QWORD,__lpszResult:LPSTR
Bin32						PROTO	__qwNumber:QWORD,__lpszResult:LPSTR
Bin64						PROTO	__qwNumber:QWORD,__lpszResult:LPSTR

FileClose					PROTO	__FileHandle:HANDLE
FileCreate					PROTO	__lpszFileName:LPSTR
FileOpen					PROTO	__lpszFileName:LPSTR
FileDelete					PROTO	__lpszFileName:LPSTR
FileSecureDelete			PROTO	__lpszFileName:LPSTR
FileOpenForRead				PROTO	__lpszFileName:LPSTR
FileOpenForWrite			PROTO	__lpszFileName:LPSTR
FileRead					PROTO	__hFile:HANDLE,__lpBuffer:LPBYTE,__dwBufferSize:QWORD
FileWrite					PROTO	__hFile:HANDLE,__lpBuffer:LPBYTE,__dwBufferSize:QWORD
FileGetPosition				PROTO	__hFile:HANDLE
FileFlush					PROTO	__hFile:HANDLE

Error						PROTO	__hWnd:HWND,__lpszErrorMessage:LPSTR
Warning						PROTO	__hWnd:HWND,__lpszWarningMessage:LPSTR
Information					PROTO	__hWnd:HWND,__lpszInformationMessage:LPSTR

MemoryAlloc					PROTO	__dwNumberOfBytes:DWORD
MemoryFree					PROTO	__lpAddress:LPVOID

GetSpecialFolderPath		PROTO	__hWnd:HWND,__iCSIDL:QWORD,__lpszPath:LPSTR

RecycleBinInformations		PROTO	__lpNumberOfItem:PTR QWORD,__lpSize:PTR QWORD
EmptyRecycleBin				PROTO

GetUnreadMailCount			PROTO
DiskFreeSpace				PROTO	__lpszVolume:LPSTR,__lpqwTot:PTR QWORD,__lpqwFree:PTR QWORD
BrowseForFolder				PROTO	__hWnd:HWND,__lpszFolderName:LPSTR,__lpszBrowseForFolderTitle:LPSTR

LoadImageFromFile			PROTO	__lpszImageFileName:LPSTR
Ascii2QWord					PROTO	__lpszString:LPSTR

StringLower					PROTO	__lpszString:LPSTR
StringUpper					PROTO	__lpszString:LPSTR
;StringLength				PROTO	__lpszString:LPSTR
IsAlpha						PROTO	__cLetter:QWORD
IsNumeric					PROTO	__cLetter:QWORD
IsAlphaNumeric				PROTO	__cLetter:QWORD

extern	MemorySetTo0:PROC
extern	MemorySet:PROC
extern	MemoryAlloc:PROC
extern	MemoryFree:PROC

extern	Bin:PROC
extern	Bin8:PROC
extern	Bin16:PROC
extern	Bin32:PROC
extern	Bin64:PROC

extern	FileOpen:PROC
extern	FileCreate:PROC
extern	FileClose:PROC
extern	FileDelete:PROC
extern	FileSecureDelete:PROC
extern	FileOpenForRead:PROC
extern	FileOpenForWrite:PROC
extern	FileRead:PROC
extern	FileWrite:PROC
extern	FileGetPosition:PROC
extern	FileFlush:PROC
extern	Error:PROC
extern	Warning:PROC
extern	Information:PROC
extern	GetSpecialFolderPath:PROC
extern	RecycleBinInformations:PROC
extern	EmptyRecycleBin:PROC
extern	GetUnreadMailCount:PROC
extern	DiskFreeSpace:PROC
extern	BrowseForFolder:PROC
extern	LoadImageFromFile:PROC
extern	Ascii2QWord:PROC
extern	StringLength:PROC
extern	StringLower:PROC
extern	StringUpper:PROC
extern	IsAlpha:PROC
extern	IsNumeric:PROC
extern	IsAlphaNumeric:PROC
