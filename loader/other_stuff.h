#include <windows.h>
#include <Psapi.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include "../dll/xor.h"


static HWND g_Hwnd;

HANDLE GetProcessByFilename(LPCSTR lpFilename, LPCSTR dllname);
VOID CreateProcessWithDll(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPCSTR lpCurrentDirectory, 
						  LPSTARTUPINFOA lpStartupInfo, LPCSTR lpDllName);

VOID InjectDll(HANDLE hProc, LPCSTR dllname);

bool IsInjected(HANDLE hProcess, LPCSTR dllname);
