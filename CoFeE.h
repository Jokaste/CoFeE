#ifndef _COFEE_H
#define _COFEE_H

#include "libgfl.h"
#include "libgfle.h"

#include "ToolBar Buttons.h"

typedef	sqlite3* 		HSQLITE ;
typedef	sqlite3_stmt*	HSTMT ;

typedef	unsigned long long QWORD ;
typedef	signed int INTEGER ;

#define	MAX_TOOLBAR_BUTTONS			25
#define	MAX_EDITOR_BUTTONS			26

#define	MAX_FILTER_STACK			16
#define	MAX_ASTEROIDS				5000

#define	ID_DIAPORAMA				0x230714

#define	MIN_WINDOW_WIDTH			900
#define	MIN_WINDOW_HEIGHT			700

#define	HWND_ORIGINAL				0
#define	HWND_WORKSPACE				1
#define	HWND_EDITOR					2
#define	HWND_RESULTS				3
#define	HWND_DIAPORAMA				4
#define	HWND_OPTIONS				5

#define	IMAGE_SENS_HORIZONTAL		1
#define	IMAGE_SENS_VERTICAL			2
#define	IMAGE_SENS_CARRE			3

#define	MAX_EDIT_ROWS				29
#define	MAX_EDIT_COLUMNS			29
#define	MAX_EDIT_FIELDS				MAX_EDIT_ROWS * MAX_EDIT_COLUMNS

#define	MAX_LENGTH_NAMES			64
#define	MAX_KECCAK_LENGTH			256 / 4
#define	MAX_FILTER_NAME_LENGTH		64
#define	MAX_STAR_NAME_LENGTH		64
#define	MAX_COMMENT_LENGTH			1024

#define	LEN_THEME_NAME				32

#define	GREY_NONE					0
#define	GREY_INPUT					1
#define	GREY_OUTPUT					2
#define	GREY_INOUT					3

#define	MODEL_NO					0
#define	MODEL_YES					1

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))

typedef struct tagBITMAPINFOV5
{
	BITMAPV5HEADER	bmiHeader;
	RGBQUAD			bmiColors[1];
} BITMAPINFOV5, *LPBITMAPINFOV5;

typedef struct tagIMAGEFILE
{
	LPSTR					lpszImageFile ;

	DWORD					dwSensImage ;		// 1 = Horizontal & 2 = Vertical
	DWORD					dwFileSize ;		// Uniquement FileSizeLow
	DWORD					dwFormatIndex ;		// Fourni par GFL

	double					dRatio ;			// Height Divided by Width	(double = 64 bits)

	size_t					iPixelSize ;
	size_t					iScanlineSize ;
	size_t					iBitmapSize ;

	char					szImageFile[MAX_PATH + 4] ;
} IMAGEFILE, *LPIMAGEFILE ;

typedef struct tagIMAGEINFOS
{
	QWORD					StructureSize ;

	LPBYTE					lpOriginal ;
	LPBYTE					lpResult ;
	LPBYTE					lpWork ;

	HBITMAP					hImgBitmap ;
	DWORD					dwMemBlockSize ;	// Quantité de mémoire allouée pour une image

	IMAGEFILE				Image ;

	BITMAPINFO				BitmapInfo ;
} IMAGEINFOS, *LPIMAGEINFOS ;

typedef	struct tagPGM_FILTERS
{
	DWORD			FilterRecordNumber ;
	DWORD			StarRecordNumber ;
	INTEGER			Model ;
	INTEGER			Rate ;
	INTEGER			ColsNumber ;
	INTEGER			TotalColsNumber ;
	INTEGER			Divisor ;
	INTEGER			Bias ;
	INTEGER			Grey ;
	INTEGER			BlackAndWhite ;
	char			Keccak_Filter[MAX_KECCAK_LENGTH + 4] ;
	char			StarName[MAX_STAR_NAME_LENGTH + 4] ;
	char			Comment[MAX_COMMENT_LENGTH + 4] ;
	int				Matrix[MAX_EDIT_FIELDS] ;
} PGM_FILTERS, *LPPGM_FILTERS ;

typedef	struct tagPGM_CONVOLUTION
{
	DWORD			FilterRecordNumber ;
	INTEGER			Matrix[MAX_EDIT_FIELDS] ;
} PGM_CONVOLUTION, *LPPGM_CONVOLUTION ;

typedef	struct tagPGM_DUMMY_FILTERS
{
	DWORD			FilterRecordNumber ;
	DWORD			StarRecordNumber ;
	INTEGER			Model ;
	INTEGER			Rate ;
	INTEGER			ColsNumber ;
	INTEGER			TotalColsNumber ;
	INTEGER			Grey ;
	INTEGER			Divisor ;
	INTEGER			Bias ;
	INTEGER			BlackAndWhite ;
	INTEGER			UnUsed_1 ;
	INTEGER			UnUsed_2 ;
	char			Keccak_Filter[MAX_KECCAK_LENGTH + 4] ;
	char			StarName[MAX_STAR_NAME_LENGTH + 4] ;
	char			Comment[MAX_COMMENT_LENGTH + 4] ;
	int				Matrix[MAX_EDIT_FIELDS] ;
} PGM_DUMMY_FILTERS, *LPPGM_DUMMY_FILTERS ;

typedef	struct tagPGM_DUMMY_CONVOLUTION
{
	DWORD			FilterRecordNumber ;
	int				Matrix[MAX_EDIT_FIELDS] ;
} PGM_DUMMY_CONVOLUTION, *LPPGM_DUMMY_CONVOLUTION ;

typedef	struct tagCOMBO_SIZE
{
	DWORD			Index ;
	LPSTR			lpszName ;
} COMBO_SIZE, *LPCOMBO_SIZE ;

typedef	struct tagCOMBO_YESNO
{
	DWORD			Index ;
	LPSTR			lpszName ;
} COMBO_YESNO, *LPCOMBO_YESNO ;

typedef	struct tagTEXTES
{
	DWORD			Index ;
	char			Name[MAX_LENGTH_NAMES + 4] ;
} TEXTES, *LPTEXTES ;

typedef	struct tagNOM_PRENOMS
{
	DWORD			StarRecordNumber ;
	DWORD			FilterRecordNumber ;
	DWORD			Freedom ;
	char			Keccak[MAX_KECCAK_LENGTH + 4] ;
	char			StarName[MAX_STAR_NAME_LENGTH + 4] ;
} NOM_PRENOMS, *LPNOM_PRENOMS ;

typedef	struct tagCFE_COLORS
{
	DWORD			RecordNumber ;
	char			Name[LEN_THEME_NAME + 4] ;
	DWORD			CtlColorDlg ;
	DWORD			CtlColorListBox ;
	DWORD			CtlColorStatic ;
	DWORD			CtlColorBtn ;
	DWORD			CtlColorEdit ;
	DWORD			Zgm_RowBgcolor ;
	DWORD			Zgm_RowFgColor ;
	DWORD			Zgm_CellBgColor ;
	DWORD			Zgm_CellFgColor ;
	DWORD			Zgm_ColBgColor ;
	DWORD			Zgm_ColFgColor ;
} CFE_COLORS, *LPCFE_COLORS ;

typedef void (*TypeChronos)(void) ;
typedef void (*TypeChronosParam)(void (*__lpFunction)(int),int __iArgument) ;

extern	PGM_FILTERS	StackFilter[10] ;

HWND CreateEditorToolBar(HWND __hWnd) ;

int Convolve(int __iImageWidth,int __iImageHeight,LPBYTE __lpImageBits,LPPGM_FILTERS __lpFiltre) ;

LPPGM_FILTERS CreateFilter(void) ;
LPPGM_FILTERS ReadFilter(int __Index,LPPGM_FILTERS __lpFilter) ;

LPSTR BrowseForFolder(HWND __hWnd,LPSTR __lpszFolderName,LPSTR __lpszBrowseForFolderTitle) ;
LPSTR CreateKeccak(LPPGM_FILTERS __lpFilter) ;
LPSTR GetAndUpdateAsteroidName(int __iFilterRecordNumber,LPSTR __lpszKeccack,LPSTR __lpszName,LPDWORD __lpdwRecordNumber) ;
LPSTR GetAsteroidName(LPSTR __lpszName,DWORD __dwRecordNumber) ;
LPSTR GetSpecialFolderPath(HWND __hWnd,QWORD __iCSIDL,LPSTR __lpszPath) ;

BOOL	FileFlush(HANDLE __hFile) ;

int DeleteFilter(void) ;
int ExportFilters(void) ;
int FileWrite(HANDLE __hFile,LPSTR __lpBuffer,QWORD __dwBufferSize) ;
int Filter_SetLocked(DWORD __dwRecordNumber) ;
int Filter_SetUnLocked(DWORD __dwRecordNumber) ;
int Filters_Import(void) ;
int IsThisKeccackPresentInDatabase(LPSTR __lpszKeccak_Filter) ;
int SetAllTheFilters(void) ;
int SetFiltersToListBox(void) ;
int StoreNamesAndSurnames(LPNOM_PRENOMS __lpNomPrenoms) ;
int UpdateFilter(void) ;

void SetSimilarity(double __dSimilarity) ;

void ApplyFilter(int __bCopyToClipboard) ;
void ChangeFilter(LPPGM_FILTERS __lpPgmFilter) ;
void ChangeFilterSize(int __iNewSize,int __bApplyFilter) ;
void ClearAllFields(void) ;
void ComputeTheDivisor(void) ;
void CreateEditor(int __iNumberOfColonnes, int __iResetFilter) ;
void Diaporama(void) ;
void Initialize(void) ;
void InitializeTheTables(void) ;
void InverseValues(void) ;
void ReverseValues(void) ;
void LoadEditor(void) ;
void NewFilter(void) ;
void OpenThisFile(void) ;
void PasteFromClipboard(void) ;
void PopFilter(void) ;
void PushFilter(void) ;
void Randomize(void) ;
void RequestFilter(void) ;
void ResetFilter(void) ;
void ResetPaintAllWindows(void) ;
void SaveAllTheFilters(void) ;
void SaveFilter(int __bSaveData) ;
void SetAllFields(int __iValue) ;
void SetEditorColors(void) ;
void SetPrevious(void) ;
void Child2_SetRedOrGreen(void) ;

int myRandomizer(long long int _iMini,long long int _iMaxi) ;

void PaintResultWindow(void) ;
void Child2_PaintResultWindow(void) ;

LRESULT CALLBACK AboutDlgProc(HWND __hWnd,UINT __uMsg,WPARAM __wParam,LPARAM __lParam) ;
LRESULT CALLBACK DiaporamaProc(HWND __hWnd,UINT __Msg,WPARAM __wParam,LPARAM __lParam) ;

LRESULT CALLBACK Ch6_Options_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;
LRESULT CALLBACK Ch5_Diaporama_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;
LRESULT CALLBACK Ch4_Result_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;
LRESULT CALLBACK Ch3_Editor_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;
LRESULT CALLBACK Ch2_Workspace_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;
LRESULT CALLBACK Ch1_Original_Proc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;

LRESULT CALLBACK ToolBoxWndProc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;

LRESULT CALLBACK DlgWaitingProc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;
LRESULT CALLBACK DlgModlessProc(HWND __hWnd,UINT __Message,WPARAM __wParam,LPARAM __lParam) ;

LPBYTE PhR_Alloc_Memory(DWORD __dwSize) ;
int PhR_Free_Memory(LPVOID __lpcbBuffer) ;

LPBYTE GetBitsFromBitmap(HBITMAP __hBitmap,LPDWORD __lpdwSize,LPBYTE __lpResult) ;
void ImageResize(DWORD __dwOriginalWidth,DWORD __dwOriginalHeight,DWORD __dwExpectedWidth,DWORD __dwExpectedHeight,DWORD *__lpdwNewWidth,DWORD *__lpdwNewHeight) ;
LPIMAGEINFOS LoadImageFromFile(register LPSTR __lpszImageFile,DWORD __dwBorderWidth,DWORD __dwBorderHeight) ;
void ImageFree(LPIMAGEINFOS __lpImageInfos) ;
HBITMAP SetBitsToBitmap(int __iWidth,int __iHeight,LPBYTE __lpBits) ;
HBITMAP CropImage(HBITMAP __hBitmap,DWORD __dwBorderWidth,DWORD __dwBorderHeight) ;
LPIMAGEINFOS LoadImageFromClipboard(DWORD __dwBorderWidth,DWORD __dwBorderHeight) ;
HBITMAP ImageToBitmap(LPIMAGEINFOS __lpImage,LPBYTE __lpImageBits) ;
LPBYTE SuperCropImage(LPIMAGEINFOS __lpImage,LPBYTE __lpImageBits,LPDWORD __lpdwImageSize,DWORD __dwBorderWidth,DWORD __dwBorderHeight) ;

void Keccak(register LPBYTE __lpcbData,DWORD __dwSize,register LPBYTE __lpcbResult) ;
int CreateNewToolbar(HWND __hWnd) ;
void Keccak(register LPBYTE __lpcbData,DWORD __dwSize,register LPBYTE __lpcbResult) ;
LPBYTE Hex2Str(LPBYTE __lpBuffer,LPBYTE __lpszResult,size_t __iSize) ;
void SQLitePragmas(void) ;
LRESULT CALLBACK SignedIntegerSubclassProc(HWND __hWnd,UINT __uMsg,WPARAM __wParam,LPARAM __lParam,UINT_PTR __uIdSubclass,DWORD_PTR __dwRefData) ;
BOOL EnforceSignedIntegerEdit(HWND __hWnd) ;
int IsAtStartOfEditControl(HWND __hWnd) ;
int SQLite_Error(void) ;
void DeleteTempFiles(LPSTR __lpszDirectory) ;
LPSTR Get_SQLite_Error(LPSTR __lpszErrorText) ;

void Log(LPSTR __lpszTxt) ;
void Log_Init(void) ;
void Log_Start(void) ;
void Log_Stop(void) ;
void Log_Exit(void) ;
void Log_SetFileName(LPSTR) ;
LPSTR Log_GetFileName(void) ;

#endif /* _COFEE_H */
