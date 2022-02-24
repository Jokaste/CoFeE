AdjustWindowRect	PROTO	:LPRECT,:DWORD,:BOOL
AppendMenuA	PROTO	:HMENU,:QWORD,:QWORD,:LPSTR
AssocQueryStringA	PROTO	:QWORD,:QWORD,:QWORD,:QWORD,:QWORD,:QWORD
BindMoniker TEXTEQU <__imp_BindMoniker>
CallNextHookEx	PROTO :HANDLE,:DWORD,:WPARAM,:LPARAM
UnhookWindowsHookEx	PROTO	:HANDLE
CLIPFORMAT_UserFree TEXTEQU <__imp_CLIPFORMAT_UserFree>
CLIPFORMAT_UserFree64 TEXTEQU <__imp_CLIPFORMAT_UserFree64>
CLIPFORMAT_UserMarshal TEXTEQU <__imp_CLIPFORMAT_UserMarshal>
CLIPFORMAT_UserMarshal64 TEXTEQU <__imp_CLIPFORMAT_UserMarshal64>
CLIPFORMAT_UserSize TEXTEQU <__imp_CLIPFORMAT_UserSize>
CLIPFORMAT_UserSize64 TEXTEQU <__imp_CLIPFORMAT_UserSize64>
CLIPFORMAT_UserUnmarshal TEXTEQU <__imp_CLIPFORMAT_UserUnmarshal>
CLIPFORMAT_UserUnmarshal64 TEXTEQU <__imp_CLIPFORMAT_UserUnmarshal64>
CLSIDFromOle1Class TEXTEQU <__imp_CLSIDFromOle1Class>
CLSIDFromProgID TEXTEQU <__imp_CLSIDFromProgID>
CLSIDFromProgIDEx TEXTEQU <__imp_CLSIDFromProgIDEx>
CLSIDFromString TEXTEQU <__imp_CLSIDFromString>
CalculatePopupWindowPosition	PROTO	:LPPOINT,:LPSIZE,:QWORD,:LPRECT,:LPRECT
ChangeMenuA	PROTO	:HMENU,:QWORD,:LPSTR,:QWORD,:QWORD
CheckMenuItem	PROTO	:HMENU,:QWORD,:QWORD
CloseHandle	PROTO	:HANDLE
CoAddRefServerProcess TEXTEQU <__imp_CoAddRefServerProcess>
CoAllowSetForegroundWindow TEXTEQU <__imp_CoAllowSetForegroundWindow>
CoBuildVersion TEXTEQU <__imp_CoBuildVersion>
CoCancelCall TEXTEQU <__imp_CoCancelCall>
CoCopyProxy TEXTEQU <__imp_CoCopyProxy>
CoCreateFreeThreadedMarshaler TEXTEQU <__imp_CoCreateFreeThreadedMarshaler>
CoCreateGuid TEXTEQU <__imp_CoCreateGuid>
CoCreateInstance TEXTEQU <__imp_CoCreateInstance>
CoCreateInstanceEx TEXTEQU <__imp_CoCreateInstanceEx>
CoCreateObjectInContext TEXTEQU <__imp_CoCreateObjectInContext>
CoDeactivateObject TEXTEQU <__imp_CoDeactivateObject>
CoDisableCallCancellation TEXTEQU <__imp_CoDisableCallCancellation>
CoDisconnectObject TEXTEQU <__imp_CoDisconnectObject>
CoDosDateTimeToFileTime TEXTEQU <__imp_CoDosDateTimeToFileTime>
CoEnableCallCancellation TEXTEQU <__imp_CoEnableCallCancellation>
CoFileTimeNow TEXTEQU <__imp_CoFileTimeNow>
CoFileTimeToDosDateTime TEXTEQU <__imp_CoFileTimeToDosDateTime>
CoFreeAllLibraries TEXTEQU <__imp_CoFreeAllLibraries>
CoFreeLibrary TEXTEQU <__imp_CoFreeLibrary>
CoFreeUnusedLibraries TEXTEQU <__imp_CoFreeUnusedLibraries>
CoFreeUnusedLibrariesEx TEXTEQU <__imp_CoFreeUnusedLibrariesEx>
CoGetApartmentID TEXTEQU <__imp_CoGetApartmentID>
CoGetCallContext TEXTEQU <__imp_CoGetCallContext>
CoGetCallerTID TEXTEQU <__imp_CoGetCallerTID>
CoGetCancelObject TEXTEQU <__imp_CoGetCancelObject>
CoGetClassObject TEXTEQU <__imp_CoGetClassObject>
CoGetClassVersion TEXTEQU <__imp_CoGetClassVersion>
CoGetComCatalog TEXTEQU <__imp_CoGetComCatalog>
CoGetContextToken TEXTEQU <__imp_CoGetContextToken>
CoGetCurrentLogicalThreadId TEXTEQU <__imp_CoGetCurrentLogicalThreadId>
CoGetCurrentProcess TEXTEQU <__imp_CoGetCurrentProcess>
CoGetDefaultContext TEXTEQU <__imp_CoGetDefaultContext>
CoGetInstanceFromFile TEXTEQU <__imp_CoGetInstanceFromFile>
CoGetInstanceFromIStorage TEXTEQU <__imp_CoGetInstanceFromIStorage>
CoGetInterceptor TEXTEQU <__imp_CoGetInterceptor>
CoGetInterceptorFromTypeInfo TEXTEQU <__imp_CoGetInterceptorFromTypeInfo>
CoGetInterfaceAndReleaseStream TEXTEQU <__imp_CoGetInterfaceAndReleaseStream>
CoGetMalloc TEXTEQU <__imp_CoGetMalloc>
CoGetMarshalSizeMax TEXTEQU <__imp_CoGetMarshalSizeMax>
CoGetModuleType TEXTEQU <__imp_CoGetModuleType>
CoGetObject TEXTEQU <__imp_CoGetObject>
CoGetObjectContext TEXTEQU <__imp_CoGetObjectContext>
CoGetPSClsid TEXTEQU <__imp_CoGetPSClsid>
CoGetProcessIdentifier TEXTEQU <__imp_CoGetProcessIdentifier>
CoGetStandardMarshal TEXTEQU <__imp_CoGetStandardMarshal>
CoGetState TEXTEQU <__imp_CoGetState>
CoGetStdMarshalEx TEXTEQU <__imp_CoGetStdMarshalEx>
CoGetSystemSecurityPermissions TEXTEQU <__imp_CoGetSystemSecurityPermissions>
CoGetTreatAsClass TEXTEQU <__imp_CoGetTreatAsClass>
CoImpersonateClient TEXTEQU <__imp_CoImpersonateClient>
CoInitialize TEXTEQU <__imp_CoInitialize>
CoInitializeEx TEXTEQU <__imp_CoInitializeEx>
CoInitializeSecurity TEXTEQU <__imp_CoInitializeSecurity>
CoInitializeWOW TEXTEQU <__imp_CoInitializeWOW>
CoInstall TEXTEQU <__imp_CoInstall>
CoInvalidateRemoteMachineBindings TEXTEQU <__imp_CoInvalidateRemoteMachineBindings>
CoIsHandlerConnected TEXTEQU <__imp_CoIsHandlerConnected>
CoIsOle1Class TEXTEQU <__imp_CoIsOle1Class>
CoLoadLibrary TEXTEQU <__imp_CoLoadLibrary>
CoLockObjectExternal TEXTEQU <__imp_CoLockObjectExternal>
CoMarshalHresult TEXTEQU <__imp_CoMarshalHresult>
CoMarshalInterThreadInterfaceInStream TEXTEQU <__imp_CoMarshalInterThreadInterfaceInStream>
CoMarshalInterface TEXTEQU <__imp_CoMarshalInterface>
CoPopServiceDomain TEXTEQU <__imp_CoPopServiceDomain>
CoPushServiceDomain TEXTEQU <__imp_CoPushServiceDomain>
CoQueryAuthenticationServices TEXTEQU <__imp_CoQueryAuthenticationServices>
CoQueryClientBlanket TEXTEQU <__imp_CoQueryClientBlanket>
CoQueryProxyBlanket TEXTEQU <__imp_CoQueryProxyBlanket>
CoQueryReleaseObject TEXTEQU <__imp_CoQueryReleaseObject>
CoReactivateObject TEXTEQU <__imp_CoReactivateObject>
CoRegisterChannelHook TEXTEQU <__imp_CoRegisterChannelHook>
CoRegisterClassObject TEXTEQU <__imp_CoRegisterClassObject>
CoRegisterInitializeSpy TEXTEQU <__imp_CoRegisterInitializeSpy>
CoRegisterMallocSpy TEXTEQU <__imp_CoRegisterMallocSpy>
CoRegisterMessageFilter TEXTEQU <__imp_CoRegisterMessageFilter>
CoRegisterPSClsid TEXTEQU <__imp_CoRegisterPSClsid>
CoRegisterSurrogate TEXTEQU <__imp_CoRegisterSurrogate>
CoRegisterSurrogateEx TEXTEQU <__imp_CoRegisterSurrogateEx>
CoReleaseMarshalData TEXTEQU <__imp_CoReleaseMarshalData>
CoReleaseServerProcess TEXTEQU <__imp_CoReleaseServerProcess>
CoResumeClassObjects TEXTEQU <__imp_CoResumeClassObjects>
CoRetireServer TEXTEQU <__imp_CoRetireServer>
CoRevertToSelf TEXTEQU <__imp_CoRevertToSelf>
CoRevokeClassObject TEXTEQU <__imp_CoRevokeClassObject>
CoRevokeInitializeSpy TEXTEQU <__imp_CoRevokeInitializeSpy>
CoRevokeMallocSpy TEXTEQU <__imp_CoRevokeMallocSpy>
CoSetCancelObject TEXTEQU <__imp_CoSetCancelObject>
CoSetProxyBlanket TEXTEQU <__imp_CoSetProxyBlanket>
CoSetState TEXTEQU <__imp_CoSetState>
CoSuspendClassObjects TEXTEQU <__imp_CoSuspendClassObjects>
CoSwitchCallContext TEXTEQU <__imp_CoSwitchCallContext>
CoTaskMemAlloc TEXTEQU <__imp_CoTaskMemAlloc>
CoTaskMemFree TEXTEQU <__imp_CoTaskMemFree>
CoTaskMemRealloc TEXTEQU <__imp_CoTaskMemRealloc>
CoTestCancel TEXTEQU <__imp_CoTestCancel>
CoTreatAsClass TEXTEQU <__imp_CoTreatAsClass>
CoUninitialize TEXTEQU <__imp_CoUninitialize>
CoUnloadingWOW TEXTEQU <__imp_CoUnloadingWOW>
CoUnmarshalHresult TEXTEQU <__imp_CoUnmarshalHresult>
CoUnmarshalInterface TEXTEQU <__imp_CoUnmarshalInterface>
CoVrfCheckThreadState TEXTEQU <__imp_CoVrfCheckThreadState>
CoVrfGetThreadState TEXTEQU <__imp_CoVrfGetThreadState>
CoVrfReleaseThreadState TEXTEQU <__imp_CoVrfReleaseThreadState>
CoWaitForMultipleHandles TEXTEQU <__imp_CoWaitForMultipleHandles>
ComPs_NdrDllCanUnloadNow TEXTEQU <__imp_ComPs_NdrDllCanUnloadNow>
ComPs_NdrDllGetClassObject TEXTEQU <__imp_ComPs_NdrDllGetClassObject>
ComPs_NdrDllRegisterProxy TEXTEQU <__imp_ComPs_NdrDllRegisterProxy>
ComPs_NdrDllUnregisterProxy TEXTEQU <__imp_ComPs_NdrDllUnregisterProxy>
CopyImage	PROTO :QWORD,:QWORD,:QWORD,:QWORD,:QWORD
CreateAntiMoniker TEXTEQU <__imp_CreateAntiMoniker>
CreateBindCtx TEXTEQU <__imp_CreateBindCtx>
CreateClassMoniker TEXTEQU <__imp_CreateClassMoniker>
CreateDataAdviseHolder TEXTEQU <__imp_CreateDataAdviseHolder>
CreateDataCache TEXTEQU <__imp_CreateDataCache>
CreateDirectoryA	PROTO	:LPCTSTR,:QWORD
CreateErrorInfo TEXTEQU <__imp_CreateErrorInfo>
CreateFileA	PROTO	:LPCTSTR,:DWORD,:DWORD,:QWORD,:DWORD,:DWORD,:HANDLE
CreateFileMoniker TEXTEQU <__imp_CreateFileMoniker>
CreateGenericComposite TEXTEQU <__imp_CreateGenericComposite>
CreateILockBytesOnHGlobal TEXTEQU <__imp_CreateILockBytesOnHGlobal>
CreateItemMoniker TEXTEQU <__imp_CreateItemMoniker>
CreateMenu	PROTO
CreateObjrefMoniker TEXTEQU <__imp_CreateObjrefMoniker>
CreateOleAdviseHolder TEXTEQU <__imp_CreateOleAdviseHolder>
CreatePointerMoniker TEXTEQU <__imp_CreatePointerMoniker>
CreatePopupMenu	PROTO
CreateStatusWindowA	PROTO	:DWORD,:LPCSTR,:HWND,:DWORD
CreateStdProgressIndicator TEXTEQU <__imp_CreateStdProgressIndicator>
CreateStreamOnHGlobal	PROTO :QWORD,:QWORD,:QWORD
CreateThread	PROTO	:QWORD,:DWORD,:QWORD,:QWORD,:DWORD,:QWORD
CreateWindowExA	PROTO	:QWORD,:LPSTR,:LPSTR,:QWORD,:QWORD,:QWORD,:QWORD,:QWORD,:HWND,:HMENU,:HINSTANCE,:LPARAM
DcomChannelSetHResult TEXTEQU <__imp_DcomChannelSetHResult>
DefWindowProcA	PROTO	:HWND,:QWORD,:WPARAM,:LPARAM
DeleteFileA	PROTO	:LPCTSTR
DeleteMenu	PROTO	:HMENU,:QWORD,:QWORD
DeleteUrlCacheEntry	PROTO	:LPSTR
DestroyMenu	PROTO	:HMENU
DialogBoxA	PROTO	:HINSTANCE,:LPCTSTR,:HWND,:QWORD
DialogBoxParamA	PROTO	:HINSTANCE,:LPCTSTR,:HWND,:QWORD,:QWORD
DispatchMessageA	PROTO	:QWORD
DllDebugObjectRPCHook TEXTEQU <__imp_DllDebugObjectRPCHook>
DllGetClassObject TEXTEQU <__imp_DllGetClassObject>
DllGetClassObjectWOW TEXTEQU <__imp_DllGetClassObjectWOW>
DllRegisterServer TEXTEQU <__imp_DllRegisterServer>
DoDragDrop TEXTEQU <__imp_DoDragDrop>
DrawMenuBar	PROTO	:HWND
DrawStatusTextA	PROTO	:HDC,:LPRECT,:LPCSTR,:DWORD
EnableHookObject TEXTEQU <__imp_EnableHookObject>
EnableMenuItem	PROTO	:HMENU,:QWORD,:QWORD
EndMenu	PROTO
EndPaint	PROTO	:HWND,:QWORD
EnumWindows	PROTO	:QWORD,:LPARAM
ExitProcess	PROTO	:QWORD
FindClose	PROTO	:HANDLE
FindFirstFileA	PROTO	:LPCTSTR,:LPWIN32_FIND_DATA
FindNextFileA	PROTO	:HANDLE,:LPWIN32_FIND_DATA
FlashWindow	PROTO	:HWND,:BOOL
FlushFileBuffers	PROTO	:HANDLE
FmtIdToPropStgName TEXTEQU <__imp_FmtIdToPropStgName>
FreeLibraryAndExitThread	PROTO	:HINSTANCE,:DWORD
FreePropVariantArray TEXTEQU <__imp_FreePropVariantArray>
GetClassFile TEXTEQU <__imp_GetClassFile>
GetCommandLineA	PROTO
GetConvertStg TEXTEQU <__imp_GetConvertStg>
GetCurrentDirectory	EQU	<GetCurrentDirectoryA>
GetCurrentDirectoryA	PROTO	:QWORD,:QWORD
GetDlgItemTextA	PROTO	:QWORD,:DWORD,:QWORD,:DWORD
GetDocumentBitStg TEXTEQU <__imp_GetDocumentBitStg>
GetErrorInfo TEXTEQU <__imp_GetErrorInfo>
GetHGlobalFromILockBytes TEXTEQU <__imp_GetHGlobalFromILockBytes>
GetHGlobalFromStream TEXTEQU <__imp_GetHGlobalFromStream>
GetHookInterface TEXTEQU <__imp_GetHookInterface>
GetMenu	PROTO	:HWND
GetMenuCheckMarkDimensions	PROTO
GetMenuDefaultItem	PROTO	:HMENU,:QWORD,:QWORD
GetMenuInfo	PROTO	:HMENU,:QWORD
GetMenuItemCount	PROTO	:HMENU
GetMenuItemID	PROTO	:HMENU,:QWORD
GetMenuItemInfoA	PROTO	:HMENU,:QWORD,:BOOL,:QWORD
GetMenuItemRect	PROTO	:HWND,:HMENU,:QWORD,:LPRECT
GetMenuState	PROTO	:HMENU,:QWORD,:QWORD
GetMenuStringA	PROTO	:HMENU,:QWORD,:LPSTR,:QWORD,:QWORD
GetMessageA	PROTO	:QWORD,:QWORD,:QWORD,:QWORD
GetModuleFileNameA	PROTO	:HMODULE,:LPTSTR,:DWORD
GetModuleFileNameExA	PROTO :HANDLE,:HINSTANCE,:LPSTR,:DWORD
GetModuleHandleA	PROTO	:HINSTANCE
GetProcessHeap	PROTO 
GetRunningObjectTable TEXTEQU <__imp_GetRunningObjectTable>
GetStockObject	PROTO	:QWORD
GetSubMenu	PROTO	:HMENU,:QWORD
GetSystemMenu	PROTO	:HWND,:BOOL
GetTempFileNameA	PROTO	:LPCTSTR,:LPCTSTR,:QWORD,:LPTSTR
GetWindowRect	PROTO	:HWND,:LPRECT
GetWindowText	TEXTEQU	<GetWindowTextA>
GetWindowTextA	PROTO	:HWND,:LPSTR,:DWORD
GlobalAlloc	PROTO :QWORD,:QWORD
GlobalFree	PROTO :QWORD
GlobalLock	PROTO :QWORD
GlobalUnlock	PROTO :QWORD
HACCEL_UserFree TEXTEQU <__imp_HACCEL_UserFree>
HACCEL_UserFree64 TEXTEQU <__imp_HACCEL_UserFree64>
HACCEL_UserMarshal TEXTEQU <__imp_HACCEL_UserMarshal>
HACCEL_UserMarshal64 TEXTEQU <__imp_HACCEL_UserMarshal64>
HACCEL_UserSize TEXTEQU <__imp_HACCEL_UserSize>
HACCEL_UserSize64 TEXTEQU <__imp_HACCEL_UserSize64>
HACCEL_UserUnmarshal TEXTEQU <__imp_HACCEL_UserUnmarshal>
HACCEL_UserUnmarshal64 TEXTEQU <__imp_HACCEL_UserUnmarshal64>
HBITMAP_UserFree TEXTEQU <__imp_HBITMAP_UserFree>
HBITMAP_UserFree64 TEXTEQU <__imp_HBITMAP_UserFree64>
HBITMAP_UserMarshal TEXTEQU <__imp_HBITMAP_UserMarshal>
HBITMAP_UserMarshal64 TEXTEQU <__imp_HBITMAP_UserMarshal64>
HBITMAP_UserSize TEXTEQU <__imp_HBITMAP_UserSize>
HBITMAP_UserSize64 TEXTEQU <__imp_HBITMAP_UserSize64>
HBITMAP_UserUnmarshal TEXTEQU <__imp_HBITMAP_UserUnmarshal>
HBITMAP_UserUnmarshal64 TEXTEQU <__imp_HBITMAP_UserUnmarshal64>
HBRUSH_UserFree TEXTEQU <__imp_HBRUSH_UserFree>
HBRUSH_UserFree64 TEXTEQU <__imp_HBRUSH_UserFree64>
HBRUSH_UserMarshal TEXTEQU <__imp_HBRUSH_UserMarshal>
HBRUSH_UserMarshal64 TEXTEQU <__imp_HBRUSH_UserMarshal64>
HBRUSH_UserSize TEXTEQU <__imp_HBRUSH_UserSize>
HBRUSH_UserSize64 TEXTEQU <__imp_HBRUSH_UserSize64>
HBRUSH_UserUnmarshal TEXTEQU <__imp_HBRUSH_UserUnmarshal>
HBRUSH_UserUnmarshal64 TEXTEQU <__imp_HBRUSH_UserUnmarshal64>
HDC_UserFree TEXTEQU <__imp_HDC_UserFree>
HDC_UserFree64 TEXTEQU <__imp_HDC_UserFree64>
HDC_UserMarshal TEXTEQU <__imp_HDC_UserMarshal>
HDC_UserMarshal64 TEXTEQU <__imp_HDC_UserMarshal64>
HDC_UserSize TEXTEQU <__imp_HDC_UserSize>
HDC_UserSize64 TEXTEQU <__imp_HDC_UserSize64>
HDC_UserUnmarshal TEXTEQU <__imp_HDC_UserUnmarshal>
HDC_UserUnmarshal64 TEXTEQU <__imp_HDC_UserUnmarshal64>
HENHMETAFILE_UserFree TEXTEQU <__imp_HENHMETAFILE_UserFree>
HENHMETAFILE_UserFree64 TEXTEQU <__imp_HENHMETAFILE_UserFree64>
HENHMETAFILE_UserMarshal TEXTEQU <__imp_HENHMETAFILE_UserMarshal>
HENHMETAFILE_UserMarshal64 TEXTEQU <__imp_HENHMETAFILE_UserMarshal64>
HENHMETAFILE_UserSize TEXTEQU <__imp_HENHMETAFILE_UserSize>
HENHMETAFILE_UserSize64 TEXTEQU <__imp_HENHMETAFILE_UserSize64>
HENHMETAFILE_UserUnmarshal TEXTEQU <__imp_HENHMETAFILE_UserUnmarshal>
HENHMETAFILE_UserUnmarshal64 TEXTEQU <__imp_HENHMETAFILE_UserUnmarshal64>
HGLOBAL_UserFree TEXTEQU <__imp_HGLOBAL_UserFree>
HGLOBAL_UserFree64 TEXTEQU <__imp_HGLOBAL_UserFree64>
HGLOBAL_UserMarshal TEXTEQU <__imp_HGLOBAL_UserMarshal>
HGLOBAL_UserMarshal64 TEXTEQU <__imp_HGLOBAL_UserMarshal64>
HGLOBAL_UserSize TEXTEQU <__imp_HGLOBAL_UserSize>
HGLOBAL_UserSize64 TEXTEQU <__imp_HGLOBAL_UserSize64>
HGLOBAL_UserUnmarshal TEXTEQU <__imp_HGLOBAL_UserUnmarshal>
HGLOBAL_UserUnmarshal64 TEXTEQU <__imp_HGLOBAL_UserUnmarshal64>
HICON_UserFree TEXTEQU <__imp_HICON_UserFree>
HICON_UserFree64 TEXTEQU <__imp_HICON_UserFree64>
HICON_UserMarshal TEXTEQU <__imp_HICON_UserMarshal>
HICON_UserMarshal64 TEXTEQU <__imp_HICON_UserMarshal64>
HICON_UserSize TEXTEQU <__imp_HICON_UserSize>
HICON_UserSize64 TEXTEQU <__imp_HICON_UserSize64>
HICON_UserUnmarshal TEXTEQU <__imp_HICON_UserUnmarshal>
HICON_UserUnmarshal64 TEXTEQU <__imp_HICON_UserUnmarshal64>
HMENU_UserFree TEXTEQU <__imp_HMENU_UserFree>
HMENU_UserFree64 TEXTEQU <__imp_HMENU_UserFree64>
HMENU_UserMarshal TEXTEQU <__imp_HMENU_UserMarshal>
HMENU_UserMarshal64 TEXTEQU <__imp_HMENU_UserMarshal64>
HMENU_UserSize TEXTEQU <__imp_HMENU_UserSize>
HMENU_UserSize64 TEXTEQU <__imp_HMENU_UserSize64>
HMENU_UserUnmarshal TEXTEQU <__imp_HMENU_UserUnmarshal>
HMENU_UserUnmarshal64 TEXTEQU <__imp_HMENU_UserUnmarshal64>
HMETAFILEPICT_UserFree TEXTEQU <__imp_HMETAFILEPICT_UserFree>
HMETAFILEPICT_UserFree64 TEXTEQU <__imp_HMETAFILEPICT_UserFree64>
HMETAFILEPICT_UserMarshal TEXTEQU <__imp_HMETAFILEPICT_UserMarshal>
HMETAFILEPICT_UserMarshal64 TEXTEQU <__imp_HMETAFILEPICT_UserMarshal64>
HMETAFILEPICT_UserSize TEXTEQU <__imp_HMETAFILEPICT_UserSize>
HMETAFILEPICT_UserSize64 TEXTEQU <__imp_HMETAFILEPICT_UserSize64>
HMETAFILEPICT_UserUnmarshal TEXTEQU <__imp_HMETAFILEPICT_UserUnmarshal>
HMETAFILEPICT_UserUnmarshal64 TEXTEQU <__imp_HMETAFILEPICT_UserUnmarshal64>
HMETAFILE_UserFree TEXTEQU <__imp_HMETAFILE_UserFree>
HMETAFILE_UserFree64 TEXTEQU <__imp_HMETAFILE_UserFree64>
HMETAFILE_UserMarshal TEXTEQU <__imp_HMETAFILE_UserMarshal>
HMETAFILE_UserMarshal64 TEXTEQU <__imp_HMETAFILE_UserMarshal64>
HMETAFILE_UserSize TEXTEQU <__imp_HMETAFILE_UserSize>
HMETAFILE_UserSize64 TEXTEQU <__imp_HMETAFILE_UserSize64>
HMETAFILE_UserUnmarshal TEXTEQU <__imp_HMETAFILE_UserUnmarshal>
HMETAFILE_UserUnmarshal64 TEXTEQU <__imp_HMETAFILE_UserUnmarshal64>
HPALETTE_UserFree TEXTEQU <__imp_HPALETTE_UserFree>
HPALETTE_UserFree64 TEXTEQU <__imp_HPALETTE_UserFree64>
HPALETTE_UserMarshal TEXTEQU <__imp_HPALETTE_UserMarshal>
HPALETTE_UserMarshal64 TEXTEQU <__imp_HPALETTE_UserMarshal64>
HPALETTE_UserSize TEXTEQU <__imp_HPALETTE_UserSize>
HPALETTE_UserSize64 TEXTEQU <__imp_HPALETTE_UserSize64>
HPALETTE_UserUnmarshal TEXTEQU <__imp_HPALETTE_UserUnmarshal>
HPALETTE_UserUnmarshal64 TEXTEQU <__imp_HPALETTE_UserUnmarshal64>
HWND_UserFree TEXTEQU <__imp_HWND_UserFree>
HWND_UserFree64 TEXTEQU <__imp_HWND_UserFree64>
HWND_UserMarshal TEXTEQU <__imp_HWND_UserMarshal>
HWND_UserMarshal64 TEXTEQU <__imp_HWND_UserMarshal64>
HWND_UserSize TEXTEQU <__imp_HWND_UserSize>
HWND_UserSize64 TEXTEQU <__imp_HWND_UserSize64>
HWND_UserUnmarshal TEXTEQU <__imp_HWND_UserUnmarshal>
HWND_UserUnmarshal64 TEXTEQU <__imp_HWND_UserUnmarshal64>
HeapAlloc	PROTO :QWORD,:QWORD,:QWORD
HeapFree	PROTO :QWORD,:QWORD,:QWORD
HiliteMenuItem	PROTO	:HWND,:HMENU,:QWORD,:QWORD
HkOleRegisterObject TEXTEQU <__imp_HkOleRegisterObject>
IIDFromString TEXTEQU <__imp_IIDFromString>
ImageList_Create	PROTO	:QWORD,:QWORD,:QWORD,:QWORD,:QWORD
ImageList_SetBkColor	PROTO	:HIMAGELIST,:QWORD
ImageList_SetIconSize	PROTO	:HIMAGELIST,:QWORD,:QWORD
InitCommonControls		PROTO
InitCommonControlsEx	PROTO	:LPINITCOMMONCONTROLSEX
InsertMenuA	PROTO	:HMENU,:QWORD,:QWORD,:QWORD,:LPSTR
InsertMenuItemA	PROTO	:HMENU,:QWORD,:BOOL,:QWORD
InternetCloseHandle	PROTO	:HINTERNET
InternetCombineUrlA	PROTO	:LPCTSTR,:LPCTSTR,:LPTSTR,:QWORD,:QWORD
InternetOpenA	PROTO	:LPCTSTR,:DWORD,:LPCTSTR,:LPCTSTR,:DWORD
InternetOpenUrlA	PROTO	:HINTERNET,:LPCTSTR,:LPCTSTR,:DWORD,:DWORD,:QWORD
InternetReadFile	PROTO	:HINTERNET,:LPVOID,:DWORD,:QWORD
IsAccelerator TEXTEQU <__imp_IsAccelerator>
IsEqualGUID TEXTEQU <__imp_IsEqualGUID>
IsValidIid TEXTEQU <__imp_IsValidIid>
IsValidInterface TEXTEQU <__imp_IsValidInterface>
IsValidPtrIn TEXTEQU <__imp_IsValidPtrIn>
IsValidPtrOut TEXTEQU <__imp_IsValidPtrOut>
LoadAcceleratorsA	PROTO	:HINSTANCE,:LPCTSTR
LoadCursorA	PROTO	:HINSTANCE,:QWORD
LoadIconA	PROTO	:HINSTANCE,:QWORD
LoadLibrary  TEXTEQU <LoadLibraryA>
LoadLibraryA PROTO :QWORD
LoadMenuA	PROTO	:HINSTANCE,:LPSTR
LoadMenuIndirectA	PROTO	:QWORD
LoadStringA	PROTO	:HINSTANCE,:UINT,:LPTSTR,:QWORD
MenuItemFromPoint	PROTO	:HWND,:HMENU,:LPPOINT
MessageBeep	PROTO	:DWORD
MessageBoxA	PROTO	:HWND,:LPCTSTR,:LPCTSTR,:DWORD
MkParseDisplayName TEXTEQU <__imp_MkParseDisplayName>
ModifyMenuA	PROTO	:HMENU,:QWORD,:QWORD,:QWORD,:LPSTR
MonikerCommonPrefixWith TEXTEQU <__imp_MonikerCommonPrefixWith>
MonikerRelativePathTo TEXTEQU <__imp_MonikerRelativePathTo>
MoveFileA	PROTO	lpExistingFileName:LPSTR,lpNewFileName:LPSTR
MoveWindow	PROTO	:HWND,:QWORD,:QWORD,:QWORD,:QWORD,:QWORD
MultiByteToWideChar	PROTO :QWORD,:QWORD,:QWORD,:QWORD,:QWORD,:QWORD
OleBuildVersion TEXTEQU <__imp_OleBuildVersion>
OleConvertIStorageToOLESTREAM TEXTEQU <__imp_OleConvertIStorageToOLESTREAM>
OleConvertIStorageToOLESTREAMEx TEXTEQU <__imp_OleConvertIStorageToOLESTREAMEx>
OleConvertOLESTREAMToIStorage TEXTEQU <__imp_OleConvertOLESTREAMToIStorage>
OleConvertOLESTREAMToIStorageEx TEXTEQU <__imp_OleConvertOLESTREAMToIStorageEx>
OleCreate TEXTEQU <__imp_OleCreate>
OleCreateDefaultHandler TEXTEQU <__imp_OleCreateDefaultHandler>
OleCreateEmbeddingHelper TEXTEQU <__imp_OleCreateEmbeddingHelper>
OleCreateEx TEXTEQU <__imp_OleCreateEx>
OleCreateFromData TEXTEQU <__imp_OleCreateFromData>
OleCreateFromDataEx TEXTEQU <__imp_OleCreateFromDataEx>
OleCreateFromFile TEXTEQU <__imp_OleCreateFromFile>
OleCreateFromFileEx TEXTEQU <__imp_OleCreateFromFileEx>
OleCreateLink TEXTEQU <__imp_OleCreateLink>
OleCreateLinkEx TEXTEQU <__imp_OleCreateLinkEx>
OleCreateLinkFromData TEXTEQU <__imp_OleCreateLinkFromData>
OleCreateLinkFromDataEx TEXTEQU <__imp_OleCreateLinkFromDataEx>
OleCreateLinkToFile TEXTEQU <__imp_OleCreateLinkToFile>
OleCreateLinkToFileEx TEXTEQU <__imp_OleCreateLinkToFileEx>
OleCreateMenuDescriptor TEXTEQU <__imp_OleCreateMenuDescriptor>
OleCreateStaticFromData TEXTEQU <__imp_OleCreateStaticFromData>
OleDestroyMenuDescriptor TEXTEQU <__imp_OleDestroyMenuDescriptor>
OleDoAutoConvert TEXTEQU <__imp_OleDoAutoConvert>
OleDraw TEXTEQU <__imp_OleDraw>
OleDuplicateData TEXTEQU <__imp_OleDuplicateData>
OleFlushClipboard TEXTEQU <__imp_OleFlushClipboard>
OleGetAutoConvert TEXTEQU <__imp_OleGetAutoConvert>
OleGetClipboard TEXTEQU <__imp_OleGetClipboard>
OleGetIconOfClass TEXTEQU <__imp_OleGetIconOfClass>
OleGetIconOfFile TEXTEQU <__imp_OleGetIconOfFile>
OleInitialize TEXTEQU <__imp_OleInitialize>
OleInitializeWOW TEXTEQU <__imp_OleInitializeWOW>
OleIsCurrentClipboard TEXTEQU <__imp_OleIsCurrentClipboard>
OleIsRunning TEXTEQU <__imp_OleIsRunning>
OleLoad TEXTEQU <__imp_OleLoad>
OleLoadFromStream TEXTEQU <__imp_OleLoadFromStream>
OleLoadPicture	PROTO :QWORD,:QWORD,:QWORD,:QWORD,:QWORD
OleLoadPicturePath	PROTO :QWORD,:QWORD,:QWORD,:QWORD,:QWORD,:QWORD
OleLockRunning TEXTEQU <__imp_OleLockRunning>
OleMetafilePictFromIconAndLabel TEXTEQU <__imp_OleMetafilePictFromIconAndLabel>
OleNoteObjectVisible TEXTEQU <__imp_OleNoteObjectVisible>
OleQueryCreateFromData TEXTEQU <__imp_OleQueryCreateFromData>
OleQueryLinkFromData TEXTEQU <__imp_OleQueryLinkFromData>
OleRegEnumFormatEtc TEXTEQU <__imp_OleRegEnumFormatEtc>
OleRegEnumVerbs TEXTEQU <__imp_OleRegEnumVerbs>
OleRegGetMiscStatus TEXTEQU <__imp_OleRegGetMiscStatus>
OleRegGetUserType TEXTEQU <__imp_OleRegGetUserType>
OleRun TEXTEQU <__imp_OleRun>
OleSave TEXTEQU <__imp_OleSave>
OleSaveToStream TEXTEQU <__imp_OleSaveToStream>
OleSetAutoConvert TEXTEQU <__imp_OleSetAutoConvert>
OleSetClipboard TEXTEQU <__imp_OleSetClipboard>
OleSetContainedObject TEXTEQU <__imp_OleSetContainedObject>
OleSetMenuDescriptor TEXTEQU <__imp_OleSetMenuDescriptor>
OleTranslateAccelerator TEXTEQU <__imp_OleTranslateAccelerator>
OleUninitialize TEXTEQU <__imp_OleUninitialize>
OpenOrCreateStream TEXTEQU <__imp_OpenOrCreateStream>
PathAddBackslashA	PROTO	:LPSTR
PathAddExtensionA	PROTO	:LPSTR,:LPCSTR
PathAppendA	PROTO	:LPSTR,:LPCSTR
PathBuildRootA	PROTO	:LPSTR,:QWORD
PathCanonicalizeA	PROTO	:LPSTR,:LPCSTR
PathCombineA	PROTO	:LPSTR,:LPCSTR,:LPCSTR
PathCommonPrefixA	PROTO	:LPCSTR,:LPCSTR,:LPSTR
PathCompactPathA	PROTO	:HDC,:LPSTR,:QWORD
PathCompactPathExA	PROTO	:LPSTR,:LPCSTR,:QWORD,:DWORD
PathFileExistsA	PROTO	:LPCSTR
PathFindExtensionA	PROTO	:LPCSTR
PathFindFileNameA	PROTO	:LPCSTR
PathFindFileNameA	PROTO	:LPTSTR
PathFindNextComponentA	PROTO	:LPCSTR
PathFindOnPathA	PROTO	:LPSTR,:LPCSTR
PathFindSuffixArrayA	PROTO	:LPCSTR,:LPCSTR,:QWORD
PathGetArgsA	PROTO	:LPCSTR
PathGetCharTypeA	PROTO	:QWORD
PathGetDriveNumberA	PROTO	:LPCSTR
PathIsContentTypeA	PROTO	:LPCSTR,:LPCSTR
PathIsDirectoryA	PROTO	:LPCSTR
PathIsDirectoryEmptyA	PROTO	:LPCSTR
PathIsFileSpecA	PROTO	:LPCSTR
PathIsLFNFileSpecA	PROTO	:LPCSTR
PathIsNetworkPathA	PROTO	:LPCSTR
PathIsPrefixA	PROTO	:LPCSTR,:LPCSTR
PathIsRelativeA	PROTO	:LPCSTR
PathIsRootA	PROTO	:LPCSTR
PathIsSameRootA	PROTO	:LPCSTR,:LPCSTR
PathIsSystemFolderA	PROTO	:LPCSTR,:DWORD
PathIsUNCA	PROTO	:LPCSTR
PathIsUNCServerA	PROTO	:LPCSTR
PathIsUNCServerShareA	PROTO	:LPCSTR
PathIsURLA	PROTO	:LPCSTR
PathMakePrettyA	PROTO	:LPSTR
PathMakeSystemFolderA	PROTO	:LPCSTR
PathMatchSpecA	PROTO	:LPCSTR,:LPCSTR
PathMatchSpecExA	PROTO	:LPCSTR,:LPCSTR,:DWORD
PathParseIconLocationA	PROTO	:LPSTR
PathQuoteSpacesA	PROTO	:LPSTR
PathRelativePathToA	PROTO	:LPSTR,:LPCSTR,:DWORD,:LPCSTR,:DWORD
PathRemoveArgsA	PROTO	:LPSTR
PathRemoveBackslashA	PROTO	:LPSTR
PathRemoveBackslashA	PROTO	:LPTSTR
PathRemoveBlanksA	PROTO	:LPSTR
PathRemoveExtensionA	PROTO	:LPSTR
PathRemoveFileSpecA	PROTO	:LPSTR
PathRenameExtensionA	PROTO	:LPSTR,:LPCSTR
PathSearchAndQualifyA	PROTO	:LPCSTR,:LPSTR,:QWORD
PathSetDlgItemPathA	PROTO	:HWND,:QWORD,:LPCSTR
PathSkipRootA	PROTO	:LPCSTR
PathStripPathA	PROTO	:LPSTR
PathStripToRootA	PROTO	:LPSTR
PathUnExpandEnvStringsA	PROTO	:LPCSTR,:LPSTR,:QWORD
PathUndecorateA	PROTO	:LPSTR
PathUnmakeSystemFolderA	PROTO	:LPCSTR
PathUnquoteSpacesA	PROTO	:LPSTR
PostQuitMessage	PROTO	:QWORD
ProgIDFromCLSID TEXTEQU <__imp_ProgIDFromCLSID>
PropStgNameToFmtId TEXTEQU <__imp_PropStgNameToFmtId>
PropSysAllocString TEXTEQU <__imp_PropSysAllocString>
PropSysFreeString TEXTEQU <__imp_PropSysFreeString>
PropVariantChangeType TEXTEQU <__imp_PropVariantChangeType>
PropVariantClear TEXTEQU <__imp_PropVariantClear>
PropVariantCopy TEXTEQU <__imp_PropVariantCopy>
ReadClassStg TEXTEQU <__imp_ReadClassStg>
ReadClassStm TEXTEQU <__imp_ReadClassStm>
ReadFile	PROTO	:HANDLE,:LPVOID,:DWORD,:QWORD,:QWORD
ReadFmtUserTypeStg TEXTEQU <__imp_ReadFmtUserTypeStg>
ReadOleStg TEXTEQU <__imp_ReadOleStg>
ReadStringStream TEXTEQU <__imp_ReadStringStream>
RegisterClassExA	PROTO	:LPWNDCLASSEX
RegisterDragDrop TEXTEQU <__imp_RegisterDragDrop>
ReleaseStgMedium TEXTEQU <__imp_ReleaseStgMedium>
RemoveMenu	PROTO	:HMENU,:QWORD,:QWORD
RevokeDragDrop TEXTEQU <__imp_RevokeDragDrop>
RpcStringFreeA	PROTO	:PTR LPSTR
SHGetFolderPathA	PROTO	:HWND,:QWORD,:HANDLE,:DWORD,:LPTSTR
SHGetSpecialFolderPathA	PROTO	:HWND,:LPTSTR,:QWORD,:QWORD
SNB_UserFree TEXTEQU <__imp_SNB_UserFree>
SNB_UserFree64 TEXTEQU <__imp_SNB_UserFree64>
SNB_UserMarshal TEXTEQU <__imp_SNB_UserMarshal>
SNB_UserMarshal64 TEXTEQU <__imp_SNB_UserMarshal64>
SNB_UserSize TEXTEQU <__imp_SNB_UserSize>
SNB_UserSize64 TEXTEQU <__imp_SNB_UserSize64>
SNB_UserUnmarshal TEXTEQU <__imp_SNB_UserUnmarshal>
SNB_UserUnmarshal64 TEXTEQU <__imp_SNB_UserUnmarshal64>
STGMEDIUM_UserFree TEXTEQU <__imp_STGMEDIUM_UserFree>
STGMEDIUM_UserFree64 TEXTEQU <__imp_STGMEDIUM_UserFree64>
STGMEDIUM_UserMarshal TEXTEQU <__imp_STGMEDIUM_UserMarshal>
STGMEDIUM_UserMarshal64 TEXTEQU <__imp_STGMEDIUM_UserMarshal64>
STGMEDIUM_UserSize TEXTEQU <__imp_STGMEDIUM_UserSize>
STGMEDIUM_UserSize64 TEXTEQU <__imp_STGMEDIUM_UserSize64>
STGMEDIUM_UserUnmarshal TEXTEQU <__imp_STGMEDIUM_UserUnmarshal>
STGMEDIUM_UserUnmarshal64 TEXTEQU <__imp_STGMEDIUM_UserUnmarshal64>
SendMessageA	PROTO	:HWND,:QWORD,:WPARAM,:LPARAM
SetConvertStg TEXTEQU <__imp_SetConvertStg>
SetDocumentBitStg TEXTEQU <__imp_SetDocumentBitStg>
SetFocus	PROTO	:HWND
SetErrorInfo TEXTEQU <__imp_SetErrorInfo>
SetErrorMode	PROTO	:QWORD
SetMenu	PROTO	:HWND,:HMENU
SetMenuDefaultItem	PROTO	:HMENU,:QWORD,:QWORD
SetMenuInfo	PROTO	:HMENU,:QWORD
SetMenuItemBitmaps	PROTO	:HMENU,:QWORD,:QWORD,:HBITMAP,:HBITMAP
SetMenuItemInfoA	PROTO	:HMENU,:QWORD,:BOOL,:QWORD
SetStretchBltMode	PROTO	:HDC,:QWORD
SetTextColor		PROTO	:HDC,:QWORD
SetThreadErrorMode	PROTO	:QWORD,:QWORD
SetWindowLongPtrA	PROTO	:HWND,:DWORD,:QWORD
ShellAboutA	PROTO	:HWND,:LPSTR,:LPSTR,:HICON
ShowWindow	PROTO	:HWND,:QWORD
StgConvertPropertyToVariant TEXTEQU <__imp_StgConvertPropertyToVariant>
StgConvertVariantToProperty TEXTEQU <__imp_StgConvertVariantToProperty>
StgCreateDocfile TEXTEQU <__imp_StgCreateDocfile>
StgCreateDocfileOnILockBytes TEXTEQU <__imp_StgCreateDocfileOnILockBytes>
StgCreatePropSetStg TEXTEQU <__imp_StgCreatePropSetStg>
StgCreatePropStg TEXTEQU <__imp_StgCreatePropStg>
StgCreateStorageEx TEXTEQU <__imp_StgCreateStorageEx>
StgGetIFillLockBytesOnFile TEXTEQU <__imp_StgGetIFillLockBytesOnFile>
StgGetIFillLockBytesOnILockBytes TEXTEQU <__imp_StgGetIFillLockBytesOnILockBytes>
StgIsStorageFile TEXTEQU <__imp_StgIsStorageFile>
StgIsStorageILockBytes TEXTEQU <__imp_StgIsStorageILockBytes>
StgOpenAsyncDocfileOnIFillLockBytes TEXTEQU <__imp_StgOpenAsyncDocfileOnIFillLockBytes>
StgOpenPropStg TEXTEQU <__imp_StgOpenPropStg>
StgOpenStorage TEXTEQU <__imp_StgOpenStorage>
StgOpenStorageEx TEXTEQU <__imp_StgOpenStorageEx>
StgOpenStorageOnHandle TEXTEQU <__imp_StgOpenStorageOnHandle>
StgOpenStorageOnILockBytes TEXTEQU <__imp_StgOpenStorageOnILockBytes>
StgPropertyLengthAsVariant TEXTEQU <__imp_StgPropertyLengthAsVariant>
StgSetTimes TEXTEQU <__imp_StgSetTimes>
StrCmpLogicalW	PROTO	:LPCWSTR,:LPCWSTR
StringFromCLSID TEXTEQU <__imp_StringFromCLSID>
StringFromGUID2 TEXTEQU <__imp_StringFromGUID2>
StringFromIID TEXTEQU <__imp_StringFromIID>
SystemParametersInfoA	PROTO	:QWORD,:QWORD,:QWORD,:QWORD
TrackPopupMenu	PROTO	:HMENU,:QWORD,:QWORD,:QWORD,:QWORD,:HWND,:LPRECT
TrackPopupMenuEx	PROTO	:HMENU,:QWORD,:QWORD,:QWORD,:HWND,:QWORD
TranslateMessage	PROTO	:QWORD
UpdateDCOMSettings TEXTEQU <__imp_UpdateDCOMSettings>
UpdateProcessTracing TEXTEQU <__imp_UpdateProcessTracing>
UpdateWindow	PROTO	:HWND
UtConvertDvtd16toDvtd32 TEXTEQU <__imp_UtConvertDvtd16toDvtd32>
UtConvertDvtd32toDvtd16 TEXTEQU <__imp_UtConvertDvtd32toDvtd16>
UtGetDvtd16Info TEXTEQU <__imp_UtGetDvtd16Info>
UtGetDvtd32Info TEXTEQU <__imp_UtGetDvtd32Info>
UuidCreateSequential	PROTO	:LPUUID
UuidToStringA	PROTO	:LPUUID,:PTR LPSTR
VirtualAlloc	PROTO	:LPVOID,:DWORD,:DWORD,:DWORD
VirtualFree	PROTO	:LPVOID,:DWORD,:DWORD
WdtpInterfacePointer_UserFree TEXTEQU <__imp_WdtpInterfacePointer_UserFree>
WdtpInterfacePointer_UserFree64 TEXTEQU <__imp_WdtpInterfacePointer_UserFree64>
WdtpInterfacePointer_UserMarshal TEXTEQU <__imp_WdtpInterfacePointer_UserMarshal>
WdtpInterfacePointer_UserMarshal64 TEXTEQU <__imp_WdtpInterfacePointer_UserMarshal64>
WdtpInterfacePointer_UserSize TEXTEQU <__imp_WdtpInterfacePointer_UserSize>
WdtpInterfacePointer_UserSize64 TEXTEQU <__imp_WdtpInterfacePointer_UserSize64>
WdtpInterfacePointer_UserUnmarshal TEXTEQU <__imp_WdtpInterfacePointer_UserUnmarshal>
WdtpInterfacePointer_UserUnmarshal64 TEXTEQU <__imp_WdtpInterfacePointer_UserUnmarshal64>
WinMain	PROTO	:HINSTANCE,:HINSTANCE,:LPSTR,:QWORD
WriteClassStg TEXTEQU <__imp_WriteClassStg>
WriteClassStm TEXTEQU <__imp_WriteClassStm>
WriteFile	PROTO	:HANDLE,:QWORD,:DWORD,:QWORD,:QWORD
WriteFmtUserTypeStg TEXTEQU <__imp_WriteFmtUserTypeStg>
WriteOleStg TEXTEQU <__imp_WriteOleStg>
WriteStringStream TEXTEQU <__imp_WriteStringStream>
_itoa		PROTO C :DWORD,:LPSTR,:DWORD
asctime		PROTO C :VARARG
extern	EndDialog:PROC
extern __imp_BindMoniker:QWORD
extern __imp_CLIPFORMAT_UserFree64:QWORD
extern __imp_CLIPFORMAT_UserFree:QWORD
extern __imp_CLIPFORMAT_UserMarshal64:QWORD
extern __imp_CLIPFORMAT_UserMarshal:QWORD
extern __imp_CLIPFORMAT_UserSize64:QWORD
extern __imp_CLIPFORMAT_UserSize:QWORD
extern __imp_CLIPFORMAT_UserUnmarshal64:QWORD
extern __imp_CLIPFORMAT_UserUnmarshal:QWORD
extern __imp_CLSIDFromOle1Class:QWORD
extern __imp_CLSIDFromProgID:QWORD
extern __imp_CLSIDFromProgIDEx:QWORD
extern __imp_CLSIDFromString:QWORD
extern __imp_CoAddRefServerProcess:QWORD
extern __imp_CoAllowSetForegroundWindow:QWORD
extern __imp_CoBuildVersion:QWORD
extern __imp_CoCancelCall:QWORD
extern __imp_CoCopyProxy:QWORD
extern __imp_CoCreateFreeThreadedMarshaler:QWORD
extern __imp_CoCreateGuid:QWORD
extern __imp_CoCreateInstance:QWORD
extern __imp_CoCreateInstanceEx:QWORD
extern __imp_CoCreateObjectInContext:QWORD
extern __imp_CoDeactivateObject:QWORD
extern __imp_CoDisableCallCancellation:QWORD
extern __imp_CoDisconnectObject:QWORD
extern __imp_CoDosDateTimeToFileTime:QWORD
extern __imp_CoEnableCallCancellation:QWORD
extern __imp_CoFileTimeNow:QWORD
extern __imp_CoFileTimeToDosDateTime:QWORD
extern __imp_CoFreeAllLibraries:QWORD
extern __imp_CoFreeLibrary:QWORD
extern __imp_CoFreeUnusedLibraries:QWORD
extern __imp_CoFreeUnusedLibrariesEx:QWORD
extern __imp_CoGetApartmentID:QWORD
extern __imp_CoGetCallContext:QWORD
extern __imp_CoGetCallerTID:QWORD
extern __imp_CoGetCancelObject:QWORD
extern __imp_CoGetClassObject:QWORD
extern __imp_CoGetClassVersion:QWORD
extern __imp_CoGetComCatalog:QWORD
extern __imp_CoGetContextToken:QWORD
extern __imp_CoGetCurrentLogicalThreadId:QWORD
extern __imp_CoGetCurrentProcess:QWORD
extern __imp_CoGetDefaultContext:QWORD
extern __imp_CoGetInstanceFromFile:QWORD
extern __imp_CoGetInstanceFromIStorage:QWORD
extern __imp_CoGetInterceptor:QWORD
extern __imp_CoGetInterceptorFromTypeInfo:QWORD
extern __imp_CoGetInterfaceAndReleaseStream:QWORD
extern __imp_CoGetMalloc:QWORD
extern __imp_CoGetMarshalSizeMax:QWORD
extern __imp_CoGetModuleType:QWORD
extern __imp_CoGetObject:QWORD
extern __imp_CoGetObjectContext:QWORD
extern __imp_CoGetPSClsid:QWORD
extern __imp_CoGetProcessIdentifier:QWORD
extern __imp_CoGetStandardMarshal:QWORD
extern __imp_CoGetState:QWORD
extern __imp_CoGetStdMarshalEx:QWORD
extern __imp_CoGetSystemSecurityPermissions:QWORD
extern __imp_CoGetTreatAsClass:QWORD
extern __imp_CoImpersonateClient:QWORD
extern __imp_CoInitialize:QWORD
extern __imp_CoInitializeEx:QWORD
extern __imp_CoInitializeSecurity:QWORD
extern __imp_CoInitializeWOW:QWORD
extern __imp_CoInstall:QWORD
extern __imp_CoInvalidateRemoteMachineBindings:QWORD
extern __imp_CoIsHandlerConnected:QWORD
extern __imp_CoIsOle1Class:QWORD
extern __imp_CoLoadLibrary:QWORD
extern __imp_CoLockObjectExternal:QWORD
extern __imp_CoMarshalHresult:QWORD
extern __imp_CoMarshalInterThreadInterfaceInStream:QWORD
extern __imp_CoMarshalInterface:QWORD
extern __imp_CoPopServiceDomain:QWORD
extern __imp_CoPushServiceDomain:QWORD
extern __imp_CoQueryAuthenticationServices:QWORD
extern __imp_CoQueryClientBlanket:QWORD
extern __imp_CoQueryProxyBlanket:QWORD
extern __imp_CoQueryReleaseObject:QWORD
extern __imp_CoReactivateObject:QWORD
extern __imp_CoRegisterChannelHook:QWORD
extern __imp_CoRegisterClassObject:QWORD
extern __imp_CoRegisterInitializeSpy:QWORD
extern __imp_CoRegisterMallocSpy:QWORD
extern __imp_CoRegisterMessageFilter:QWORD
extern __imp_CoRegisterPSClsid:QWORD
extern __imp_CoRegisterSurrogate:QWORD
extern __imp_CoRegisterSurrogateEx:QWORD
extern __imp_CoReleaseMarshalData:QWORD
extern __imp_CoReleaseServerProcess:QWORD
extern __imp_CoResumeClassObjects:QWORD
extern __imp_CoRetireServer:QWORD
extern __imp_CoRevertToSelf:QWORD
extern __imp_CoRevokeClassObject:QWORD
extern __imp_CoRevokeInitializeSpy:QWORD
extern __imp_CoRevokeMallocSpy:QWORD
extern __imp_CoSetCancelObject:QWORD
extern __imp_CoSetProxyBlanket:QWORD
extern __imp_CoSetState:QWORD
extern __imp_CoSuspendClassObjects:QWORD
extern __imp_CoSwitchCallContext:QWORD
extern __imp_CoTaskMemAlloc:QWORD
extern __imp_CoTaskMemFree:QWORD
extern __imp_CoTaskMemRealloc:QWORD
extern __imp_CoTestCancel:QWORD
extern __imp_CoTreatAsClass:QWORD
extern __imp_CoUninitialize:QWORD
extern __imp_CoUnloadingWOW:QWORD
extern __imp_CoUnmarshalHresult:QWORD
extern __imp_CoUnmarshalInterface:QWORD
extern __imp_CoVrfCheckThreadState:QWORD
extern __imp_CoVrfGetThreadState:QWORD
extern __imp_CoVrfReleaseThreadState:QWORD
extern __imp_CoWaitForMultipleHandles:QWORD
extern __imp_ComPs_NdrDllCanUnloadNow:QWORD
extern __imp_ComPs_NdrDllGetClassObject:QWORD
extern __imp_ComPs_NdrDllRegisterProxy:QWORD
extern __imp_ComPs_NdrDllUnregisterProxy:QWORD
extern __imp_CreateAntiMoniker:QWORD
extern __imp_CreateBindCtx:QWORD
extern __imp_CreateClassMoniker:QWORD
extern __imp_CreateDataAdviseHolder:QWORD
extern __imp_CreateDataCache:QWORD
extern __imp_CreateErrorInfo:QWORD
extern __imp_CreateFileMoniker:QWORD
extern __imp_CreateGenericComposite:QWORD
extern __imp_CreateILockBytesOnHGlobal:QWORD
extern __imp_CreateItemMoniker:QWORD
extern __imp_CreateObjrefMoniker:QWORD
extern __imp_CreateOleAdviseHolder:QWORD
extern __imp_CreatePointerMoniker:QWORD
extern __imp_CreateStdProgressIndicator:QWORD
extern __imp_CreateStreamOnHGlobal:QWORD
extern __imp_DcomChannelSetHResult:QWORD
extern __imp_DllDebugObjectRPCHook:QWORD
extern __imp_DllGetClassObject:QWORD
extern __imp_DllGetClassObjectWOW:QWORD
extern __imp_DllRegisterServer:QWORD
extern __imp_DoDragDrop:QWORD
extern __imp_EnableHookObject:QWORD
extern __imp_FmtIdToPropStgName:QWORD
extern __imp_FreePropVariantArray:QWORD
extern __imp_GetClassFile:QWORD
extern __imp_GetConvertStg:QWORD
extern __imp_GetDocumentBitStg:QWORD
extern __imp_GetErrorInfo:QWORD
extern __imp_GetHGlobalFromILockBytes:QWORD
extern __imp_GetHGlobalFromStream:QWORD
extern __imp_GetHookInterface:QWORD
extern __imp_GetRunningObjectTable:QWORD
extern __imp_HACCEL_UserFree64:QWORD
extern __imp_HACCEL_UserFree:QWORD
extern __imp_HACCEL_UserMarshal64:QWORD
extern __imp_HACCEL_UserMarshal:QWORD
extern __imp_HACCEL_UserSize64:QWORD
extern __imp_HACCEL_UserSize:QWORD
extern __imp_HACCEL_UserUnmarshal64:QWORD
extern __imp_HACCEL_UserUnmarshal:QWORD
extern __imp_HBITMAP_UserFree64:QWORD
extern __imp_HBITMAP_UserFree:QWORD
extern __imp_HBITMAP_UserMarshal64:QWORD
extern __imp_HBITMAP_UserMarshal:QWORD
extern __imp_HBITMAP_UserSize64:QWORD
extern __imp_HBITMAP_UserSize:QWORD
extern __imp_HBITMAP_UserUnmarshal64:QWORD
extern __imp_HBITMAP_UserUnmarshal:QWORD
extern __imp_HBRUSH_UserFree64:QWORD
extern __imp_HBRUSH_UserFree:QWORD
extern __imp_HBRUSH_UserMarshal64:QWORD
extern __imp_HBRUSH_UserMarshal:QWORD
extern __imp_HBRUSH_UserSize64:QWORD
extern __imp_HBRUSH_UserSize:QWORD
extern __imp_HBRUSH_UserUnmarshal64:QWORD
extern __imp_HBRUSH_UserUnmarshal:QWORD
extern __imp_HDC_UserFree64:QWORD
extern __imp_HDC_UserFree:QWORD
extern __imp_HDC_UserMarshal64:QWORD
extern __imp_HDC_UserMarshal:QWORD
extern __imp_HDC_UserSize64:QWORD
extern __imp_HDC_UserSize:QWORD
extern __imp_HDC_UserUnmarshal64:QWORD
extern __imp_HDC_UserUnmarshal:QWORD
extern __imp_HENHMETAFILE_UserFree64:QWORD
extern __imp_HENHMETAFILE_UserFree:QWORD
extern __imp_HENHMETAFILE_UserMarshal64:QWORD
extern __imp_HENHMETAFILE_UserMarshal:QWORD
extern __imp_HENHMETAFILE_UserSize64:QWORD
extern __imp_HENHMETAFILE_UserSize:QWORD
extern __imp_HENHMETAFILE_UserUnmarshal64:QWORD
extern __imp_HENHMETAFILE_UserUnmarshal:QWORD
extern __imp_HGLOBAL_UserFree64:QWORD
extern __imp_HGLOBAL_UserFree:QWORD
extern __imp_HGLOBAL_UserMarshal64:QWORD
extern __imp_HGLOBAL_UserMarshal:QWORD
extern __imp_HGLOBAL_UserSize64:QWORD
extern __imp_HGLOBAL_UserSize:QWORD
extern __imp_HGLOBAL_UserUnmarshal64:QWORD
extern __imp_HGLOBAL_UserUnmarshal:QWORD
extern __imp_HICON_UserFree64:QWORD
extern __imp_HICON_UserFree:QWORD
extern __imp_HICON_UserMarshal64:QWORD
extern __imp_HICON_UserMarshal:QWORD
extern __imp_HICON_UserSize64:QWORD
extern __imp_HICON_UserSize:QWORD
extern __imp_HICON_UserUnmarshal64:QWORD
extern __imp_HICON_UserUnmarshal:QWORD
extern __imp_HMENU_UserFree64:QWORD
extern __imp_HMENU_UserFree:QWORD
extern __imp_HMENU_UserMarshal64:QWORD
extern __imp_HMENU_UserMarshal:QWORD
extern __imp_HMENU_UserSize64:QWORD
extern __imp_HMENU_UserSize:QWORD
extern __imp_HMENU_UserUnmarshal64:QWORD
extern __imp_HMENU_UserUnmarshal:QWORD
extern __imp_HMETAFILEPICT_UserFree64:QWORD
extern __imp_HMETAFILEPICT_UserFree:QWORD
extern __imp_HMETAFILEPICT_UserMarshal64:QWORD
extern __imp_HMETAFILEPICT_UserMarshal:QWORD
extern __imp_HMETAFILEPICT_UserSize64:QWORD
extern __imp_HMETAFILEPICT_UserSize:QWORD
extern __imp_HMETAFILEPICT_UserUnmarshal64:QWORD
extern __imp_HMETAFILEPICT_UserUnmarshal:QWORD
extern __imp_HMETAFILE_UserFree64:QWORD
extern __imp_HMETAFILE_UserFree:QWORD
extern __imp_HMETAFILE_UserMarshal64:QWORD
extern __imp_HMETAFILE_UserMarshal:QWORD
extern __imp_HMETAFILE_UserSize64:QWORD
extern __imp_HMETAFILE_UserSize:QWORD
extern __imp_HMETAFILE_UserUnmarshal64:QWORD
extern __imp_HMETAFILE_UserUnmarshal:QWORD
extern __imp_HPALETTE_UserFree64:QWORD
extern __imp_HPALETTE_UserFree:QWORD
extern __imp_HPALETTE_UserMarshal64:QWORD
extern __imp_HPALETTE_UserMarshal:QWORD
extern __imp_HPALETTE_UserSize64:QWORD
extern __imp_HPALETTE_UserSize:QWORD
extern __imp_HPALETTE_UserUnmarshal64:QWORD
extern __imp_HPALETTE_UserUnmarshal:QWORD
extern __imp_HWND_UserFree64:QWORD
extern __imp_HWND_UserFree:QWORD
extern __imp_HWND_UserMarshal64:QWORD
extern __imp_HWND_UserMarshal:QWORD
extern __imp_HWND_UserSize64:QWORD
extern __imp_HWND_UserSize:QWORD
extern __imp_HWND_UserUnmarshal64:QWORD
extern __imp_HWND_UserUnmarshal:QWORD
extern __imp_HkOleRegisterObject:QWORD
extern __imp_IIDFromString:QWORD
extern __imp_IsAccelerator:QWORD
extern __imp_IsEqualGUID:QWORD
extern __imp_IsValidIid:QWORD
extern __imp_IsValidInterface:QWORD
extern __imp_IsValidPtrIn:QWORD
extern __imp_IsValidPtrOut:QWORD
extern __imp_MkParseDisplayName:QWORD
extern __imp_MonikerCommonPrefixWith:QWORD
extern __imp_MonikerRelativePathTo:QWORD
extern __imp_OleBuildVersion:QWORD
extern __imp_OleConvertIStorageToOLESTREAM:QWORD
extern __imp_OleConvertIStorageToOLESTREAMEx:QWORD
extern __imp_OleConvertOLESTREAMToIStorage:QWORD
extern __imp_OleConvertOLESTREAMToIStorageEx:QWORD
extern __imp_OleCreate:QWORD
extern __imp_OleCreateDefaultHandler:QWORD
extern __imp_OleCreateEmbeddingHelper:QWORD
extern __imp_OleCreateEx:QWORD
extern __imp_OleCreateFromData:QWORD
extern __imp_OleCreateFromDataEx:QWORD
extern __imp_OleCreateFromFile:QWORD
extern __imp_OleCreateFromFileEx:QWORD
extern __imp_OleCreateLink:QWORD
extern __imp_OleCreateLinkEx:QWORD
extern __imp_OleCreateLinkFromData:QWORD
extern __imp_OleCreateLinkFromDataEx:QWORD
extern __imp_OleCreateLinkToFile:QWORD
extern __imp_OleCreateLinkToFileEx:QWORD
extern __imp_OleCreateMenuDescriptor:QWORD
extern __imp_OleCreateStaticFromData:QWORD
extern __imp_OleDestroyMenuDescriptor:QWORD
extern __imp_OleDoAutoConvert:QWORD
extern __imp_OleDraw:QWORD
extern __imp_OleDuplicateData:QWORD
extern __imp_OleFlushClipboard:QWORD
extern __imp_OleGetAutoConvert:QWORD
extern __imp_OleGetClipboard:QWORD
extern __imp_OleGetIconOfClass:QWORD
extern __imp_OleGetIconOfFile:QWORD
extern __imp_OleInitialize:QWORD
extern __imp_OleInitializeWOW:QWORD
extern __imp_OleIsCurrentClipboard:QWORD
extern __imp_OleIsRunning:QWORD
extern __imp_OleLoad:QWORD
extern __imp_OleLoadFromStream:QWORD
extern __imp_OleLockRunning:QWORD
extern __imp_OleMetafilePictFromIconAndLabel:QWORD
extern __imp_OleNoteObjectVisible:QWORD
extern __imp_OleQueryCreateFromData:QWORD
extern __imp_OleQueryLinkFromData:QWORD
extern __imp_OleRegEnumFormatEtc:QWORD
extern __imp_OleRegEnumVerbs:QWORD
extern __imp_OleRegGetMiscStatus:QWORD
extern __imp_OleRegGetUserType:QWORD
extern __imp_OleRun:QWORD
extern __imp_OleSave:QWORD
extern __imp_OleSaveToStream:QWORD
extern __imp_OleSetAutoConvert:QWORD
extern __imp_OleSetClipboard:QWORD
extern __imp_OleSetContainedObject:QWORD
extern __imp_OleSetMenuDescriptor:QWORD
extern __imp_OleTranslateAccelerator:QWORD
extern __imp_OleUninitialize:QWORD
extern __imp_OpenOrCreateStream:QWORD
extern __imp_ProgIDFromCLSID:QWORD
extern __imp_PropStgNameToFmtId:QWORD
extern __imp_PropSysAllocString:QWORD
extern __imp_PropSysFreeString:QWORD
extern __imp_PropVariantChangeType:QWORD
extern __imp_PropVariantClear:QWORD
extern __imp_PropVariantCopy:QWORD
extern __imp_ReadClassStg:QWORD
extern __imp_ReadClassStm:QWORD
extern __imp_ReadFmtUserTypeStg:QWORD
extern __imp_ReadOleStg:QWORD
extern __imp_ReadStringStream:QWORD
extern __imp_RegisterDragDrop:QWORD
extern __imp_ReleaseStgMedium:QWORD
extern __imp_RevokeDragDrop:QWORD
extern __imp_SNB_UserFree64:QWORD
extern __imp_SNB_UserFree:QWORD
extern __imp_SNB_UserMarshal64:QWORD
extern __imp_SNB_UserMarshal:QWORD
extern __imp_SNB_UserSize64:QWORD
extern __imp_SNB_UserSize:QWORD
extern __imp_SNB_UserUnmarshal64:QWORD
extern __imp_SNB_UserUnmarshal:QWORD
extern __imp_STGMEDIUM_UserFree64:QWORD
extern __imp_STGMEDIUM_UserFree:QWORD
extern __imp_STGMEDIUM_UserMarshal64:QWORD
extern __imp_STGMEDIUM_UserMarshal:QWORD
extern __imp_STGMEDIUM_UserSize64:QWORD
extern __imp_STGMEDIUM_UserSize:QWORD
extern __imp_STGMEDIUM_UserUnmarshal64:QWORD
extern __imp_STGMEDIUM_UserUnmarshal:QWORD
extern __imp_SetConvertStg:QWORD
extern __imp_SetDocumentBitStg:QWORD
extern __imp_SetErrorInfo:QWORD
extern __imp_StgConvertPropertyToVariant:QWORD
extern __imp_StgConvertVariantToProperty:QWORD
extern __imp_StgCreateDocfile:QWORD
extern __imp_StgCreateDocfileOnILockBytes:QWORD
extern __imp_StgCreatePropSetStg:QWORD
extern __imp_StgCreatePropStg:QWORD
extern __imp_StgCreateStorageEx:QWORD
extern __imp_StgGetIFillLockBytesOnFile:QWORD
extern __imp_StgGetIFillLockBytesOnILockBytes:QWORD
extern __imp_StgIsStorageFile:QWORD
extern __imp_StgIsStorageILockBytes:QWORD
extern __imp_StgOpenAsyncDocfileOnIFillLockBytes:QWORD
extern __imp_StgOpenPropStg:QWORD
extern __imp_StgOpenStorage:QWORD
extern __imp_StgOpenStorageEx:QWORD
extern __imp_StgOpenStorageOnHandle:QWORD
extern __imp_StgOpenStorageOnILockBytes:QWORD
extern __imp_StgPropertyLengthAsVariant:QWORD
extern __imp_StgSetTimes:QWORD
extern __imp_StringFromCLSID:QWORD
extern __imp_StringFromGUID2:QWORD
extern __imp_StringFromIID:QWORD
extern __imp_UpdateDCOMSettings:QWORD
extern __imp_UpdateProcessTracing:QWORD
extern __imp_UtConvertDvtd16toDvtd32:QWORD
extern __imp_UtConvertDvtd32toDvtd16:QWORD
extern __imp_UtGetDvtd16Info:QWORD
extern __imp_UtGetDvtd32Info:QWORD
extern __imp_WdtpInterfacePointer_UserFree64:QWORD
extern __imp_WdtpInterfacePointer_UserFree:QWORD
extern __imp_WdtpInterfacePointer_UserMarshal64:QWORD
extern __imp_WdtpInterfacePointer_UserMarshal:QWORD
extern __imp_WdtpInterfacePointer_UserSize64:QWORD
extern __imp_WdtpInterfacePointer_UserSize:QWORD
extern __imp_WdtpInterfacePointer_UserUnmarshal64:QWORD
extern __imp_WdtpInterfacePointer_UserUnmarshal:QWORD
extern __imp_WriteClassStg:QWORD
extern __imp_WriteClassStm:QWORD
extern __imp_WriteFmtUserTypeStg:QWORD
extern __imp_WriteOleStg:QWORD
extern __imp_WriteStringStream:QWORD
localtime	PROTO C :VARARG
lstrcat	EQU	<lstrcatA>
lstrcatA	PROTO	:LPSTR,:LPSTR
lstrcpyA	PROTO	:LPSTR,:LPSTR
lstrcmpA	PROTO	:LPSTR,:LPSTR
printf		PROTO C :VARARG
puts		PROTO C :VARARG
strftime	PROTO C :VARARG
time		PROTO C :VARARG
wsprintf equ <wsprintfA>
wsprintfA PROTO :QWORD,:VARARG

