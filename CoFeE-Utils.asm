							.Const

							INCLUDE	Win_AsmLibrary.inc

							.Code

;	**********************************************************************************
;	***************************** FileFlush ******************************************
;	**********************************************************************************

							ALIGN	16

FileFlush					PROC	__hFile:HANDLE

							jrcxz	@Error							; NULL Handle

							call	FlushFileBuffers

							test	rax,rax
							jz		@EndOfFunction

							mov		rax,1
							ret

;	===========================================================================================
;	===========================================================================================

							ALIGN	16

@Error :

							xor		rax,rax
							ret

;	===========================================================================================
;	===========================================================================================

							ALIGN	16

@EndOfFunction :

							ret
FileFlush					ENDP

;	**********************************************************************************
;	***************************** FileWrite ******************************************
;	**********************************************************************************

							ALIGN	16

FileWrite					PROC	USES RDI __hFile:HANDLE,__lpBuffer:LPBYTE,__dwBufferSize:QWORD PARMAREA=8*QWORD
							LOCAL	_NumberOfBytesWritten:QWORD

							jrcxz	@Error							; NULL Handle

							test	rdx,rdx							; NULL Buffer
							jz		@Error

							and		r8,00000000ffffffffh			; 0 bytes to Write?
							jz		@Error

							mov		rdi,r8

							lea		r9,_NumberOfBytesWritten
							mov		QWORD PTR [r9],0
							mov		QWORD PTR [rsp + 32],0
							call	WriteFile

							test	rax,rax
							je		@EndOfFunction

							mov		rax,_NumberOfBytesWritten

							test	rax,rax						; Passed End of File ?
							je		@EndOfFunction

							sub		rax,rdi
							jnz		@Error

							mov		rax,1						; OK
							ret

;	===========================================================================================
;	===========================================================================================

							ALIGN	16

@Error :

							xor		rax,rax
							ret

;	===========================================================================================
;	===========================================================================================

							ALIGN	16

@EndOfFunction :

							ret
FileWrite					ENDP

;	**********************************************************************************
;	*********************** MemorySetTo0 *********************************************
;	**********************************************************************************

							ALIGN	16

comment^
	INPUT	RCX	=	Source
			RDX	=	Number of bytes to copy

	OUTPUT	RAX	=	Source
^

PUBLIC	MemorySetTo0

MemorySetTo0 :

							push	rdi
							mov		rdi,rcx
							push	rdi
							xor		rax,rax

							xorpd	xmm0,xmm0
							shufpd	xmm0,xmm0,0

							mov		rcx,rdx
							shr		rcx,4
							mov		rax,16
							mov		r10,rcx
							jrcxz	@MemorySetTo0_Loop_128_End
							jmp		@MemorySetTo0_Loop_128

;	------------------> Move 128 bits

;	===========================================================================================
;	===========================================================================================

							ALIGN	16

@MemorySetTo0_Loop_128 :

							movdqu	[rdi],xmm0
							add		rdi,rax
							dec		rcx
							jnz		@MemorySetTo0_Loop_128
							jmp		@MemorySetTo0_Loop_128_End

;	===========================================================================================
;	===========================================================================================

							ALIGN	16

@MemorySetTo0_Loop_128_End :

;	------------------> Move 64 bits

							shl		r10,4
							sub		rdx,r10
							jz		@Finished_MemorySetTo0

							mov		rcx,rdx
							shr		rcx,3
							mov		r10,rcx
							rep		stosq
							shl		r10,3
							sub		rdx,r10
							jz		@Finished_MemorySetTo0

;	------------------> Move 32 bits

							mov		rcx,rdx
							shr		rcx,2
							mov		r10,rcx
							rep		stosd
							shl		r10,2
							sub		rdx,r10
							jz		@Finished_MemorySetTo0

;	------------------> Move 16 bits

							mov		rcx,rdx
							shr		rcx,1
							mov		r10,rcx
							rep		stosw
							shl		r10,1
							sub		rdx,r10
							jz		@Finished_MemorySetTo0

;	------------------> Move 8 bits

							stosb
							jmp		@Finished_MemorySetTo0

;	===========================================================================================
;	===========================================================================================

							ALIGN	16

@Finished_MemorySetTo0 :

							pop		rax
							pop		rdi
							ret

							.Data?

							ALIGN	4

szCurrentPath				BYTE		MAX_PATH dup(?)

							.Code

;	**********************************************************************************
;	*********************** BrowseForFolder ******************************************
;	**********************************************************************************

							ALIGN	16

MyBrowseCallbackProc		PROC	__hWnd:HWND,__uMsg:UINT,__wParam:WPARAM,__lParam:LPARAM  PARMAREA=4*QWORD

							cmp		rdx,BFFM_INITIALIZED
							jne		@Exit

							mov		rdx,BFFM_SETSELECTIONA
							mov		r8,1
							mov		rax,OFFSET szCurrentPath
							mov		r9,rax
							call	SendMessageA

@Exit :

							xor		rax,rax
							ret
MyBrowseCallbackProc		ENDP

							ALIGN	16

BrowseForFolder				PROC	USES RDI RSI __hWnd:HWND,__lpszFolderName:LPSTR,__lpszBrowseForFolderTitle:LPSTR PARMAREA=4*QWORD
							LOCAL	_Bi:BROWSEINFO
							LOCAL	_szDisplayName[MAX_PATH]:BYTE
							LOCAL	_hWnd:HWND
							LOCAL	_lpszFolderName:LPSTR
							LOCAL	_lpszBrowseForFolderTitle:LPSTR

							mov		_hWnd,rcx

							test	rdx,rdx
							jz		@Exit

							mov		_lpszFolderName,rdx
							mov		_lpszBrowseForFolderTitle,r8

							lea		rcx,_Bi
							mov		rdx,SIZEOF BROWSEINFO
							call	MemorySetTo0

							lea		rcx,_szDisplayName
							xor		rdx,rdx
							mov		r8,MAX_PATH
							call	MemorySetTo0

							mov		rax,_hWnd
							mov		_Bi.hwndOwner,rax
							mov		_Bi.ulFlags,BIF_USENEWUI or BIF_DONTGOBELOWDOMAIN or BIF_EDITBOX or BIF_NEWDIALOGSTYLE
							lea		rax,_szDisplayName
							mov		_Bi.pszDisplayName,rax
							mov		rax,_lpszBrowseForFolderTitle
							mov		_Bi.lpszTitle,rax
							mov		rax,OFFSET MyBrowseCallbackProc
							mov		_Bi.lpfn,rax
							lea		rcx,_Bi

							call	SHBrowseForFolderA

							test	rax,rax
							jz		@EndOfFuction

							mov		rcx,rax
							mov		rdx,_lpszFolderName

							call	SHGetPathFromIDList

							mov		rax,_lpszFolderName
							jmp		@EndOfFuction

@Exit :

							xor		rax,rax
							ret

@EndOfFuction :

							ret
BrowseForFolder				ENDP

;	**********************************************************************************
;	*********************** F I N I S H E D ******************************************
;	**********************************************************************************

							END
