extern	HINSTANCE			hInstance ;
extern	HINSTANCE			hZeeGrid ;
extern	HINSTANCE			hLanguage ;

extern	HBRUSH				hBarbieBrush ;

extern	HDC					hDCWorkSpace ;
extern	HDC					hDCOriginal ;
extern	HDC					hDCResults ;
extern	HDC					hDCDiaporama ;

extern	HICON				hIcon1 ;
extern	HICON				hIcon2 ;

extern	HBITMAP				hButtons ;
extern	HBITMAP				hIcons ;

extern	HWND				hTabsWindows[10] ;

extern	NUMBERFMT			NumFmt ;

extern	char				szBigBuffer[20 * 1024] ;

extern	SYSTEM_INFO			InfosSystem ;

extern	char				szIniBuffer[] ;

extern	int					StatWidths[] ;
extern	char				szEmptyString[] ;

extern	char				szDlgStatic_01[512] ;
extern	char				szDlgStatic_02[512] ;

extern	CFE_COLORS			Theme_Barbie ;
extern	CFE_COLORS			Theme_Olympic ;
extern	CFE_COLORS			Theme_ChiliHot ;
extern	CFE_COLORS			Theme_Green ;
extern	CFE_COLORS			Theme_Dark ;

extern	LPCFE_COLORS		lpCurrentTheme ;

extern	char				szTahoma[] ;

extern	char				szProgramPathAndName[MAX_PATH] ;
extern	char				szProgramName[MAX_PATH] ;
extern	char				szProgramFolder[MAX_PATH] ;
extern	char				szDatabaseFileName[MAX_PATH] ;
extern	char				szLanguageFile[MAX_PATH] ;
extern	char				szProfileFileName[MAX_PATH] ;
extern	char				szExportFileName[MAX_PATH] ;
extern	char				szZeeGridFileName[MAX_PATH] ;
extern	char				szExportDirectory[MAX_PATH] ;

extern	HWND				hWaiting, hModless ;
extern	HWND				hToolBox ;
extern	HWND				hToolBar, hEditorToolBar ;
extern	HWND				hTabs ;
extern	HWND				hStatus ;
extern	HFONT				hMainFont, hDialogFont ;
extern	HACCEL				hAccel ;

extern	HWND				hGrid ;

extern	HIMAGELIST			hImageListRating ;
extern	HIMAGELIST			hImageListEditor ;

extern	LPPGM_FILTERS		lpCurrentFilter ;
extern	RECT				RcWorkArea ;

extern	char				szDlgStatic_01[512] ;
extern	char				szDlgStatic_02[512] ;

extern	HSQLITE				hCfeDb ;

extern	PGM_FILTERS			CurrentFilter ;
extern	PGM_FILTERS			RescueFilters[] ;
extern	PGM_CONVOLUTION		RescueConvolution[] ;
extern	PGM_FILTERS			PreviousFilter ;

extern	PGM_FILTERS			FilterStack[MAX_FILTER_STACK] ;
extern	LPPGM_FILTERS		lpTopStack, lpFilterStack ;
extern	int					iStackPointer ;

extern	int					TabDiv3[] ;

extern	int					bFirstDrop ;
extern	int					bDiaporama ;
extern	int					bStopDiaporama ;

extern	COMBO_SIZE			Combos[] ;
extern	COMBO_SIZE			ComboDiaporama[] ;
extern	TEXTES				Prenoms[] ;
extern	TEXTES				NomsDeFamille[] ;

extern	char				szNewCreateFilters[] ;
extern	char				szNewCreateDatas[] ;
extern	char				szCreateAsteroids[] ;

extern	char				szLanguage_01[] ;
extern	char				szLanguage_02[] ;
extern	char				szLanguage_03[] ;
extern	char				szLanguage_04[] ;
extern	char				szLanguage_05[] ;

extern	LPSTR				lpszLanguages[] ;

extern	char				szInformation_01[] ;
extern	char				szInformation_02[] ;
extern	char				szInformation_03[] ;
extern	char				szInformation_04[] ;

extern	char				szQuestion_01[] ;
extern	char				szQuestion_03[] ;

extern	char				szPragma_01[] ;
extern	char				szPragma_02[] ;
extern	char				szPragma_03[] ;
extern	char				szPragma_04[] ;
extern	char				szPragma_05[] ;
extern	char				szPragma_06[] ;
extern	char				szPragma_07[] ;
extern	char				szPragma_08[] ;
extern	char				szPragma_09[] ;
extern	char				szPragma_10[] ;
extern	char				szPragma_11[] ;
extern	char				szPragma_12[] ;
extern	char				szPragma_13[] ;
extern	char				szPragma_14[] ;
extern	char				szPragma_15[] ;
extern	char				szPragma_16[] ;

extern	char				szProfile_01[] ;
extern	char				szProfile_09[] ;
extern	char				szProfile_10[] ;
extern	char				szProfile_11[] ;
extern	char				szProfile_12[] ;
extern	char				szProfile_13[] ;
extern	char				szProfile_18[] ;
extern	char				szProfile_19[] ;
extern	char				szProfile_20[] ;
extern	char				szProfile_22[] ;
extern	char				szProfile_23[] ;
extern	char				szProfile_24[] ;

extern	LPSTR				lpszRanDomLimits[] ;
extern	int					iRandomLimitNegative[], iRandomLimitPositive[] ;
