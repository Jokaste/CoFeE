WNDCLASSEX				STRUCT
	cbSize				DWORD	?
	style				DWORD	?
	lpfnWndProc			QWORD	?
	cbClsExtra			DWORD	?
	cbWndExtra			DWORD	?
	hInstance			QWORD	?
	hIcon				QWORD	?
	hCursor				QWORD	?
	hbrBackground		QWORD	?
	lpszMenuName		QWORD	?
	lpszClassName		QWORD	?
	hIconSm				QWORD	?
WNDCLASSEX				ENDS

LPWNDCLASSEX	TYPEDEF PTR WNDCLASSEX

POINT					STRUCT
	x					DWORD	?
	y					DWORD	?
POINT					ENDS

LPPOINT	TYPEDEF PTR POINT

RECT					STRUCT
  left					DWORD	?
  top					DWORD	?
  right					DWORD	?
  bottom				DWORD	?
RECT					ENDS

LPRECT	TYPEDEF PTR RECT

MSG						STRUCT		
	hwnd				HWND	?
	message				DWORD	?
	padding1			DWORD	?	   ; padding
	wParam				WPARAM	?
	lParam				LPARAM	?
	time				DWORD	?
	pt					POINT	<>
	padding2			DWORD	?	   ; padding
MSG						ENDS

LPMSG	TYPEDEF PTR MSG

INITCOMMONCONTROLSEX	STRUCT
	dwSize				DWORD	?
	dwICC				DWORD	?
INITCOMMONCONTROLSEX	ENDS

LPINITCOMMONCONTROLSEX	TYPEDEF PTR INITCOMMONCONTROLSEX

SIZE					STRUCT
	cxWidth				DWORD	?
	cyHeight			DWORD	?
SIZE					ENDS

LPSIZE	TYPEDEF PTR SIZE

FILETIME				STRUCT
	dwLowDateTime		DWORD	?
	dwHighDateTime		DWORD	?
FILETIME				ENDS

LPFILETIME	TYPEDEF PTR FILETIME

WIN32_FIND_DATA			STRUCT
	dwFileAttributes	DWORD		?
	ftCreationTime		FILETIME	<>
	ftLastAccessTime	FILETIME	<>
	ftLastWriteTime		FILETIME	<>
	nFileSizeHigh		DWORD		?
	nFileSizeLow		DWORD		?
	dwReserved0			DWORD		?
	dwReserved1			DWORD		?
	cFileName			BYTE		(MAX_PATH) dup(?)
	cAlternateFileName	BYTE		(14) dup(?)
WIN32_FIND_DATA			ENDS

LPWIN32_FIND_DATA	TYPEDEF PTR WIN32_FIND_DATA

PAINTSTRUCT				STRUCT
	hdc					HDC		?
	fErase				DWORD	?
	rcPaint				RECT	<>
	fRestore			DWORD	?
	fIncUpdate			DWORD	?
	rgbReserved			BYTE	32 dup(?)
PAINTSTRUCT				ENDS

LPPAINTSTRUCT	TYPEDEF PTR PAINTSTRUCT

GUID					STRUCT
	Data1				DWORD	?
	Data2				WORD	?
	Data3				WORD	?
	Data4				BYTE	8 dup(?)
GUID					ENDS

UUID	TYPEDEF	GUID
LPUUID	TYPEDEF PTR UUID
LPGUID	TYPEDEF	PTR	GUID

LVITEM					STRUCT
	_mask				DWORD		?
	iItem				DWORD		?
	iSubItem			DWORD		?
	iState				DWORD		?
	stateMask			DWORD		?
	Padding_01			DWORD		?
	pszText				LPSTR		?
	cchTextMax			DWORD		?
	iImage				DWORD		?
	lParam				LPARAM		?
	iIndent				DWORD		?
	iGroupId			DWORD		?
	cColumns			DWORD		?
	Padding_02			DWORD		?
	puColumns			QWORD		?
	piColFmt			QWORD		?
	iGroup				DWORD		?
LVITEM					ENDS

LPLVITEM	TYPEDEF	PTR	LVITEM

NMHDR					STRUCT
	hwndFrom			HWND		?
	idFrom				UINT_PTR	?
	code				QWORD		?
NMHDR					ENDS

LPNMHDR	TYPEDEF PTR NMHDR

TCITEMHEADERA			STRUCT
	_mask				UINT		?
	lpReserved1			UINT		?
	lpReserved2			UINT		?
	pszText				LPSTR		?
	cchTextMax			UINT		?
	iImage				UINT		?
TCITEMHEADERA			ENDS

LPTCITEMHEADERA	TYPEDEF PTR TCITEMHEADERA

TCITEM					STRUCT
	_mask				UINT		?
	dwState				DWORD		?
	dwStateMask			DWORD		?
	padding_1			DWORD		?
	pszText				LPSTR		?
	cchTextMax			UINT		?
	iImage				UINT		?
	lParam				LPARAM		?
TCITEM					ENDS

LPTCITEM	TYPEDEF PTR TCITEM

TCHITTESTINFO			STRUCT
	pt					POINT		<>
	flags				UINT		?
TCHITTESTINFO			ENDS

LPTCHITTESTINFO	TYPEDEF PTR TCHITTESTINFO

TCKEYDOWN				STRUCT
	hdr					NMHDR		<>
	wVKey				WORD		?
	flags				UINT		?
TCKEYDOWN				ENDS

LPNMTCKEYDOWN	TYPEDEF PTR NMTCKEYDOWN

NMTTDISPINFO			STRUCT
	hdr					NMHDR		<>
	lpszText			LPSTR		?
	szText				BYTE		80 dup(?)
	hinst				HINSTANCE	?
	uFlags				QWORD		?
	lParam				LPARAM		?
NMTTDISPINFO			ENDS

TOOLTIPTEXT	EQU	<NMTTDISPINFO>
LPNMTTDISPINFO	TYPEDEF	PTR	NMTTDISPINFO
LPTOOLTIPTEXT	EQU	<LPNMTTDISPINFO>

SYSTEMTIME				STRUCT
	wYear				WORD		?
	wMonth				WORD		?
	wDayOfWeek			WORD		?
	wDay				WORD		?
	wHour				WORD		?
	wMinute				WORD		?
	wSecond				WORD		?
	wMilliseconds		WORD		?
SYSTEMTIME				ENDS

LPSYSTEMTIME	TYPEDEF	PTR	SYSTEMTIME

CWPSTRUCT				STRUCT
	lParam				QWORD		?
	wParam				QWORD		?
	message				QWORD		?
	hwnd				QWORD		?
CWPSTRUCT				ENDS

LPCWPSTRUCT	TYPEDEF	PTR	CWPSTRUCT

CWPRETSTRUCT			STRUCT
	lResult				QWORD		?
	lParam				QWORD		?
	wParam				QWORD		?
	message				QWORD		?
	hwnd				QWORD		?
CWPRETSTRUCT			ENDS

LPCWPRETSTRUCT	TYPEDEF	PTR	CWPRETSTRUCT

KBDLLHOOKSTRUCT			STRUCT
	vkCode				DWORD		?
	scanCode			DWORD		?
	flags				DWORD		?
	time				DWORD		?
	dwExtraInfo			QWORD		?
KBDLLHOOKSTRUCT			ENDS

LPKBDLLHOOKSTRUCT	TYPEDEF	PTR	KBDLLHOOKSTRUCT

MSLLHOOKSTRUCT			STRUCT
	pt					POINT		<>
	mouseData			DWORD		?
	flags				DWORD		?
	time				DWORD		?
	dwExtraInfo			QWORD		?
MSLLHOOKSTRUCT			ENDS

LPMSLLHOOKSTRUCT	TYPEDEF	PTR MSLLHOOKSTRUCT

MOUSEHOOKSTRUCT			STRUCT
	pt					POINT		<>
	hwnd				HWND		?
	wHitTestCode		UINT		?
	UnUsed_1			DWORD		?		; Padding
	dwExtraInfo			QWORD		?
MOUSEHOOKSTRUCT			ENDS

LPMOUSEHOOKSTRUCT	TYPEDEF	PTR	MOUSEHOOKSTRUCT

MOUSEHOOKSTRUCTEX		STRUCT
	MouseHookStruct		MOUSEHOOKSTRUCT	<>
	mouseData			DWORD			?
MOUSEHOOKSTRUCTEX		ENDS

LPMOUSEHOOKSTRUCTEX	TYPEDEF	PTR	MOUSEHOOKSTRUCTEX

LVCOLUMN				STRUCT
	_mask				DWORD		?
	fmt					DWORD		?
	_cx					DWORD		?
	UnUsed				DWORD		?
	pszText				LPSTR		?
	cchTextMax			DWORD		?
	iSubItem			DWORD		?
	iImage				DWORD		?
	iOrder				DWORD		?
	cxMin				DWORD		?
	cxDefault			DWORD		?
	cxIdeal				DWORD		?
LVCOLUMN				ENDS

LPLVCOLUMN	TYPEDEF	PTR	LVCOLUMN

RTL_CRITICAL_SECTION	STRUCT
	DebugInfo			QWORD		?
	LockCount			DWORD		?
	RecursionCount		DWORD		?
	OwningThread		HANDLE		?
	LockSemaphore		HANDLE		?
	SpinCount			QWORD		?
RTL_CRITICAL_SECTION	ENDS

CRITICAL_SECTION	TYPEDEF	RTL_CRITICAL_SECTION

SHFILEOPSTRUCT				STRUCT
	hwnd					HWND	?
	wFunc					DWORD	?
	Padding_1				DWORD	?
	pFrom					LPSTR	?
	pTo						LPSTR	?
	fFlags					FILEOP_FLAGS	?
	Padding_2				WORD	?
	fAnyOperationsAborted	DWORD	?
	hNameMappings			LPVOID	?
	lpszProgressTitle		LPSTR	?
SHFILEOPSTRUCT				ENDS

LPSHFILEOPSTRUCT	TYPEDEF	PTR	SHFILEOPSTRUCT

SHQUERYRBINFO				STRUCT
	cbSize					DWORD	?
	Padding					DWORD	?
	i64Size					QWORD	?
	i64NumItems				QWORD	?
SHQUERYRBINFO				ENDS

LPSHQUERYRBINFO	TYPEDEF	PTR	SHQUERYRBINFO

BROWSEINFO					STRUCT
	hwndOwner				HWND	?
	pidlRoot				QWORD	?
	pszDisplayName			LPTSTR	?
	lpszTitle				LPCTSTR	?
	ulFlags					DWORD	?
	Padding_1				DWORD	?
	lpfn					QWORD	?
	lParam					LPARAM	?
	iImage					DWORD	?
	Padding_2				DWORD	?
BROWSEINFO					ENDS

LPBROWSEINFO	TYPEDEF	PTR	BROWSEINFO

UNKNOWN						STRUCT
	QueryInterface			QWORD	?
	AddRef					QWORD	?
	Release					QWORD	?
UNKNOWN						ENDS

IStream						STRUCT
	IUnknown				UNKNOWN <>
	Read					QWORD	?
	Write					QWORD	?
	Seek					QWORD	?
	SetSize					QWORD	?
	CopyTo					QWORD	?
	Commit					QWORD	?
	Revert					QWORD	?
	LockRegion				QWORD	?
	UnlockRegion			QWORD	?
	Stat					QWORD	?
	Clone					QWORD	?
IStream						ENDS

IPicture					STRUCT
	QueryInterface			QWORD	?
	AddRef					QWORD	?
	Release					QWORD	?
	get_Handle				QWORD	?
	get_hPal				QWORD	?
	get_Type				QWORD	?
	get_Width				QWORD	?
	get_Height				QWORD	?
	Render					QWORD	?
	set_hPal				QWORD	?
	get_CurDC				QWORD	?
	SelectPicture			QWORD	?
	get_KeepOriginalFormat	QWORD	?
	put_KeepOriginalFormat	QWORD	?
	PictureChanged			QWORD	?
	SaveAsFile				QWORD	?
	get_Attributes			QWORD	?
IPicture					ENDS

PROPSHEETPAGE				STRUC
	dwSize					DWORD		?
	dwFlags					DWORD		?
	hInstance				HINSTANCE	?
	UNION
		pszTemplate			LPCTSTR		?
		pResource			QWORD		?
	ENDS
	UNION  
		hIcon				HICON		?
		pszIcon				LPCTSTR		?
	ENDS
	pszTitle				LPCTSTR		?
	pfnDlgProc				DLGPROC		?
	lParam					LPARAM		?
	pfnCallback				QWORD		?
	pcRefParent				PUINT		?
	pszHeaderTitle			LPCWSTR		?
	pszHeaderSubTitle		LPCWSTR		?				
PROPSHEETPAGE				ENDS

LPPROPSHEETPAGE TYPEDEF PTR PROP

BITMAP						STRUC
	bmType					LONG	?
	bmWidth					LONG	?
	bmHeight				LONG	?
	bmWidthBytes			LONG	?
	mPlanes					WORD	?
	mBitsPixel				WORD	?
							DWORD	?
	mBits					LPVOID	?
BITMAP						ENDS

LPBITMAP TYPEDEF PTR BITMAP

CREATESTRUCT				STRUC
	lpCreateParams			LPVOID		?
	hInstance				HINSTANCE	?
	hMenu					HMENU		?
	hwndParent				HWND		?
	ly						SDWORD		?
	lx						SDWORD		?
	y						SDWORD		?
	x						SDWORD		?
	style					LONG		?
							DWORD		?
	lpszName				LPCTSTR		?
	lpszClass				LPCTSTR		?
	dwExStyle				DWORD		?
							DWORD		?
CREATESTRUCT				ENDS

LPCREATESTRUCT TYPEDEF PTR CREATESTRUCT

IMAGE_FILE_HEADER			STRUCT
	Machine					WORD	?
	NumberOfSections		WORD	?
	TimeDateStamp			DWORD	?
	PointerToSymbolTable	DWORD	?
	NumberOfSymbols			DWORD	?
	SizeOfOptionalHeader	WORD	?
	Characteristics			WORD	?
IMAGE_FILE_HEADER			ENDS

LPIMAGE_FILE_HEADER	TYPEDEF	PTR	IMAGE_FILE_HEADER

IMAGE_DATA_DIRECTORY			STRUCT
	VirtualAddress				DWORD	?
	Size						DWORD	?
IMAGE_DATA_DIRECTORY			ENDS

LPIMAGE_DATA_DIRECTORY	TYPEDEF	PTR	IMAGE_DATA_DIRECTORY

IMAGE_OPTIONAL_HEADER			STRUCT
	Magic						WORD	?
	MajorLinkerVersion			BYTE	?
	MinorLinkerVersion			BYTE	?
	SizeOfCode					DWORD	?         
	SizeOfInitializedData		DWORD	?
	SizeOfUninitializedData		DWORD	?
	AddressOfEntryPoint			DWORD	?
	BaseOfCode					DWORD	?
	BaseOfData					DWORD	?
	ImageBase					DWORD	?
	SectionAlignment			DWORD	?
	FileAlignment				DWORD	?
	MajorOperatingSystemVersion	WORD	?
	MinorOperatingSystemVersion	WORD	?
	MajorImageVersion			WORD	?
	MinorImageVersion			WORD	?
	MajorSubsystemVersion		WORD	?
	MinorSubsystemVersion		WORD	?
	Win32VersionValue			DWORD	?
	SizeOfImage					DWORD	?
	SizeOfHeaders				DWORD	?
	CheckSum					DWORD	?
	Subsystem					WORD	?
	DllCharacteristics			WORD	?
	SizeOfStackReserve			DWORD	?
	SizeOfStackCommit			DWORD	?
	SizeOfHeapReserve			DWORD	?
	SizeOfHeapCommit			DWORD	?
	LoaderFlags					DWORD	?
	NumberOfRvaAndSizes			DWORD	?
	DataDirectory				IMAGE_DATA_DIRECTORY IMAGE_NUMBEROF_DIRECTORY_ENTRIES dup(<>)
IMAGE_OPTIONAL_HEADER			ENDS

LPIMAGE_OPTIONAL_HEADER	TYPEDEF	PTR	IMAGE_OPTIONAL_HEADER

IMAGE_NT_HEADERS			STRUCT
	Signature				DWORD					?
	FileHeader				IMAGE_FILE_HEADER		<>
	OptionalHeader			IMAGE_OPTIONAL_HEADER	<>
IMAGE_NT_HEADERS			ENDS

LPIMAGE_NT_HEADERS	TYPEDEF	PTR	IMAGE_NT_HEADERS

IMAGE_DOS_HEADER				STRUCT
	e_magic						WORD	?
	e_cblp						WORD	?
	e_cp						WORD	?
	e_crlc						WORD	?
	e_cparhdr					WORD	?
	e_minalloc					WORD	?
	e_maxalloc					WORD	?
	e_ss						WORD	?
	e_sp						WORD	?
	e_csum						WORD	?
	e_ip						WORD	?
	e_cs						WORD	?
	e_lfarlc					WORD	?
	e_ovno						WORD	?
	e_res						WORD	4 dup(?)
	e_oemid						WORD	?
	e_oeminfo					WORD	?
	e_res2						WORD	10 dup(?)
	e_lfanew					DWORD	?
IMAGE_DOS_HEADER				ENDS

LPIMAGE_DOS_HEADER	TYPEDEF	PTR	IMAGE_DOS_HEADER

LIST_ENTRY					STRUCT
	Flink					LPVOID		?
	Blink					LPVOID		?
LIST_ENTRY					ENDS

LPLIST_ENTRY		TYPEDEF	PTR LIST_ENTRY
RESTRICTED_POINTER	TYPEDEF	PTR LIST_ENTRY
LPRLIST_ENTRY		TYPEDEF	PTR LIST_ENTRY

LOADED_IMAGE				STRUCT
	ModuleName				LPSTR	?				; Nom complet du fichier chargé
	hFile					HANDLE	?				; Handle du fichier mappé
	MappedAddress			PUCHAR	?				; The base address of the mapped file.
	FileHeader				LPIMAGE_NT_HEADERS	?	; A pointer to an IMAGE_NT_HEADERS structure.
	LastRvaSection			QWORD	?				; A pointer to an IMAGE_SECTION_HEADER structure.
	NumberOfSections		DWORD	?				; The number of COFF section headers.
							DWORD	?
	Sections				QWORD	?				; A pointer to an IMAGE_SECTION_HEADER structure.
	Characteristics			DWORD	?				; The image characteristics value. This member can be one of the following values.
	fSystemImage			BYTE	?				; If the image is a kernel mode executable image, this value is TRUE.
	fDOSImage				BYTE	?				; If the image is a 16-bit executable image, this value is TRUE.
	fReadOnly				BYTE	?				; If the image is read-only, this value is TRUE.
	Version					BYTE	?				; The version string.
	Links					LIST_ENTRY	<>			; The list of loaded images.
	SizeOfImage				DWORD	?				; The size of the image, in bytes.
LOADED_IMAGE				ENDS

LPLOADED_IMAGE TYPEDEF PTR LOADED_IMAGE

UNICODE_STRING 				STRUC
	woLength				WORD	?
	MaximumLength			WORD	?
							DWORD	?
	Buffer					LPSTR	?
UNICODE_STRING ENDS

LPUNICODE_STRING TYPEDEF PTR UNICODE_STRING

SETTEXTEX					STRUCT
	flags			 		DWORD	?
	codepage		 		DWORD	?
SETTEXTEX					ENDS

LPSETTEXTEX TYPEDEF PTR SETTEXTEX

SHFILEINFO					STRUCT
	hIcon					DWORD	?
	iIcon					DWORD	?
	dwAttributes			DWORD	?
	szDisplayName			WORD	MAX_PATH dup (?)
	szTypeName				WORD	80 dup (?)
SHFILEINFO					ENDS

LPSHFILEINFO TYPEDEF PTR SHFILEINFO

WINDOWPLACEMENT				STRUCT
	Length					DWORD	?
	Flags					DWORD	?
	ShowCmd					DWORD	?
	ptMinPosition			POINT	<>
	ptMaxPosition			POINT	<>
	rcNormalPosition		RECT	<>
	rcDevice				RECT	<>
WINDOWPLACEMENT				ENDS

LPWINDOWPLACEMENT TYPEDEF PTR WINDOWPLACEMENT

NMLISTVIEW					STRUCT
	hdr						NMHDR	<>
	iItem					DWORD	?
	iSubItem				DWORD	?
	uNewState				DWORD	?
	uOldState				DWORD	?
	uChanged				DWORD	?
	ptAction				POINT	<>
	lParam					LPARAM	?
NMLISTVIEW					ENDS

LPNMLISTVIEW TYPEDEF PTR NMLISTVIEW

NMITEMACTIVATE				STRUCT
	hdr						NMHDR	<>
	iItem					DWORD	?
	iSubItem				DWORD	?
	uNewState				DWORD	?
	uOldState				DWORD	?
	uChanged				DWORD	?
	ptAction				POINT	<>
	lParam					LPARAM	?
	uKeyFlags				DWORD	?
NMITEMACTIVATE				ENDS

LPNMITEMACTIVATE	TYPEDEF PTR NMITEMACTIVATE

PROCESSENTRY32			STRUCT
	dwSize				DWORD	?
	cntUsage			DWORD	?
	_Padding			DWORD	?
	th32ProcessID		DWORD	?
	th32DefaultHeapID	QWORD	?
	th32ModuleID		DWORD	?
	cntThreads			DWORD	?
	th32ParentProcessID	DWORD	?
	pcPriClassBase		DWORD	?
	dwFlags				DWORD	?
	szExeFile			BYTE	MAX_PATH dup(?)
PROCESSENTRY32			ENDS

LPPROCESSENTRY3	TYPEDEF PTR PROCESSENTRY3

MINMAXINFO				STRUCT
	ptReserved			POINT	<>
	ptMaxSize			POINT	<>
	ptMaxPosition		POINT	<>
	ptMinTrackSize		POINT	<>
	ptMaxTrackSize		POINT	<>
MINMAXINFO				ENDS

LPMINMAXINFO	TYPEDEF	PTR	MINMAXINFO

TBBUTTON				STRUCT
	iBitmap				DWORD	?
	idCommand			DWORD	?
	fsState				BYTE	?
	fsStyle				BYTE	?
	bReserved			BYTE	6 dup(?)
	dwData				QWORD	?
	iString				QWORD	?
TBBUTTON				ENDS

LPTBBUTTON	TYPEDEF PTR TBBUTTON

TBADDBITMAP				STRUCT
	hInst				HINSTANCE	?
	nID					DWORD		?
TBADDBITMAP				ENDS

LPTBADDBITMAP	TYPEDEF PTR TBADDBITMAP

RGBQUAD							STRUCT
  rgbBlue						BYTE	?
  rgbGreen						BYTE	?
  rgbRed						BYTE	?
  rgbReserved					BYTE	?
RGBQUAD							ENDS

LpRGBQUAD	TYPEDEF PTR RGBQUAD

BITMAPINFOHEADER				STRUCT
  biSize						DWORD	?
  biWidth						DWORD	?
  biHeight						DWORD	?
  biPlanes						WORD	?
  biBitCount					WORD	?
  biCompression					DWORD	?
  biSizeImage					DWORD	?
  biXPelsPerMeter				DWORD	?
  biYPelsPerMeter				DWORD	?
  biClrUsed						DWORD	?
  biClrImportant				DWORD	?
BITMAPINFOHEADER				ENDS

LPBITMAPINFOHEADER	TYPEDEF PTR BITMAPINFOHEADER

BITMAPINFO						STRUCT
  bmiHeader						BITMAPINFOHEADER	<>
  bmiColors						RGBQUAD				<>
BITMAPINFO						ENDS

LPBITMAPINFO	TYPEDEF PTR BITMAPINFO

CREATESTRUCTA					STRUCT
	lpCreateParams				LPVOID		?
	hInstance					HINSTANCE	?
	hMenu						HMENU		?
	hwndParent					HWND		?
	_cy							DWORD		?       
	_cx							DWORD		?
	y							DWORD		?
	x							DWORD		?
	style						DWORD		?
	lpszName					LPCSTR		?
	lpszClass					LPCSTR		?
	dwExStyle					DWORD		?
CREATESTRUCTA					ENDS

LPCREATESTRUCTA	TYPEDEF PTR CREATESTRUCTA

BITMAPV5HEADER					STRUCT
	bV5Size						DWORD		?
	bV5Width					DWORD		?
	bV5Height					DWORD		?
	bV5Planes					WORD		?
	bV5BitCount					WORD		?
	bV5Compression				DWORD		?
	bV5SizeImage				DWORD		?
	bV5XPelsPerMeter			DWORD		?
	bV5YPelsPerMeter			DWORD		?
	bV5ClrUsed					DWORD		?
	bV5ClrImportant				DWORD		?
	bV5RedMask					DWORD		?
	bV5GreenMask				DWORD		?
	bV5BlueMask					DWORD		?
	bV5AlphaMask				DWORD		?
	bV5CSType					DWORD		?
	bV5Endpoints				BYTE		108 dup(?)
	bV5GammaRed					DWORD		?
	bV5GammaGreen				DWORD		?
	bV5GammaBlue				DWORD		?
	bV5Intent					DWORD		?
	bV5ProfileData				DWORD		?
	bV5ProfileSize				DWORD		?
	bV5Reserved					DWORD		?
BITMAPV5HEADER					ENDS

LPBITMAPV5HEADER	TYPEDEF PTR BITMAPV5HEADER

LVFINDINFO						STRUCT
	flags						DWORD		?
	Padding						DWORD		?
	psz							LPSTR		?
	lParam						LPARAM		?
	pt							POINT		<>
	vkDirection					DWORD		?
LVFINDINFO						ENDS

LPFINDINFOA	TYPEDEF PTR LVFINDINFO
