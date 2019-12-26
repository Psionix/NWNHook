#include "other_stuff.h"



HANDLE GetProcessByFilename(LPCSTR lpFilename, LPCSTR dllname)
{
	DWORD dwProcIds[1024], pRetn;
	TCHAR exeName[255];
	HANDLE hProcess;
	EnumProcesses(dwProcIds,sizeof(dwProcIds), &pRetn);
	DWORD cProcesses = pRetn/sizeof(DWORD);

	for(unsigned int i = 0; i < cProcesses; i++)
	{
		hProcess = OpenProcess(
			PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
		    PROCESS_VM_WRITE | PROCESS_VM_READ				
			,0,dwProcIds[i]);
		if(hProcess == INVALID_HANDLE_VALUE) continue;
		GetModuleBaseName(hProcess,0,exeName,sizeof(exeName)/sizeof(TCHAR));
		if(!lstrcmp(exeName,lpFilename) && !IsInjected(hProcess,dllname)) 
			return hProcess;

		CloseHandle(hProcess);
	}
	return INVALID_HANDLE_VALUE;
}

VOID CreateProcessWithDll(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPCSTR lpDllName)
{
	PROCESS_INFORMATION pi;

	if(!CreateProcess(lpApplicationName, lpCommandLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, lpCurrentDirectory, lpStartupInfo, &pi))
		ExitProcess(MessageBox(0, "CreateProcess error!", lpApplicationName, 0));

	PVOID pBuff = VirtualAllocEx(pi.hProcess, 0, lstrlen(lpDllName) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if(pBuff && WriteProcessMemory(pi.hProcess, pBuff, lpDllName, lstrlen(lpDllName) + 1, NULL ) != FALSE)
	{
		CreateRemoteThread(	pi.hProcess,
			NULL, 0,
			(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32"), "LoadLibraryA"),
			pBuff,
			0,
			NULL);
	}

}

VOID InjectDll(HANDLE hProc, LPCSTR lpDllName)
{
	PVOID pBuff = VirtualAllocEx(hProc, 0, lstrlen(lpDllName) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(pBuff && WriteProcessMemory(hProc, pBuff, lpDllName, lstrlen(lpDllName) + 1, NULL ) != FALSE)
	{	
		CreateRemoteThread(hProc,
			NULL, 0,
			(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32"), "LoadLibraryA"),
			pBuff,
			0,
			NULL);
	}
}


bool IsInjected(HANDLE hProcess, LPCSTR dllname)
{
	HMODULE hModules[1024];
	DWORD pNeeded;
	EnumProcessModules(hProcess,hModules,sizeof(hModules),&pNeeded);
	DWORD dwModules = pNeeded/sizeof(HMODULE);
	TCHAR c[256];
	for(DWORD i = 0;i < dwModules;i++)
	{
		GetModuleFileNameEx(hProcess,hModules[i],c,255);
		if(!strcmp(c,dllname)) 
		{
			return 1;
		}
	}
	return 0;
}