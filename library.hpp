#include <Windows.h>
#include <stdio.h>
#include <time.h>

BOOL IATHook(HMODULE ModHandle, DWORD OriginalFunc, DWORD HookFunc, void **pOriginalFunc);

HMODULE GetCurrentModule();

void __cdecl add_log (const char * fmt, ...);


VOID StripFilename(LPSTR lpPath);