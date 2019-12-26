#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <stddef.h>
#include <commctrl.h>
#include "../ntdll.h"
#include "../cmdlineargs.h"
#include "../library.hpp"


VOID CreateProcessWithDll(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPCSTR lpDllName)
{
	PROCESS_INFORMATION pi;

	if(!CreateProcess(lpApplicationName, lpCommandLine, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, lpCurrentDirectory, lpStartupInfo, &pi))
		ExitProcess(MessageBox(0, "CreateProcess error!", lpApplicationName, 0));

	// DEP off
	ULONG ExecuteFlags = 0x2|0x8; // MEM_EXECUTE_OPTION_ENABLE|MEM_EXECUTE_OPTION_PERMANENT;
	NtSetInformationProcess(pi.hProcess, (PROCESSINFOCLASS)0x22/*ProcessExecuteFlags*/, &ExecuteFlags, sizeof(ExecuteFlags));

	//Sleep(2000);

	PVOID pBuff = VirtualAllocEx(pi.hProcess, 0, lstrlen(lpDllName) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if(pBuff && WriteProcessMemory(pi.hProcess, pBuff, lpDllName, lstrlen(lpDllName) + 1, NULL ) != FALSE)
	{
		HANDLE hThread = CreateRemoteThread(	pi.hProcess,
							NULL, 0,
							(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32"), "LoadLibraryA"),
							pBuff,
							0,
							NULL);
		if(!SetThreadPriority(hThread,THREAD_PRIORITY_TIME_CRITICAL))
			MessageBox(0,"!SetThreadPriority",0,0);
	}

}
int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	InitCommonControls();
	SetPriorityClass(NtCurrentProcess(),HIGH_PRIORITY_CLASS);
	STARTUPINFO si;
	GetStartupInfo(&si);

	LPSTR profile_filename = NULL;
	CmdLineArgs args;
	for(UINT i = 0; i < args.size(); i++)
	{
		if (stricmp(args[i], "-profile") == 0)
		{ 
			profile_filename = args[i+1];
			i++;
		}
	}
	if(!profile_filename)
		ExitProcess(MessageBox(0, "No profile file.\nUse -profile FILE commandline. Like ""nwnhook.exe -profile wtf.ini""", "Error", 0));

	CHAR dll_file[256], config_file[256];
	UINT uLen = GetModuleFileName(GetModuleHandle(NULL), dll_file, 256);
	memcpy(&config_file, &dll_file, uLen + 1);
	*(PDWORD)(config_file + uLen - 4) = 'ini.';
	*(PDWORD)(dll_file + uLen - 4) = 'lld.';
	
	CHAR nwn_file[256];
	GetPrivateProfileString("config", "exe", 0, nwn_file, sizeof(nwn_file), config_file);

	if(!*nwn_file)
		ExitProcess(MessageBox(0, "Config error:\n\n[config]\nexe=NULL", "Error", 0));

	CHAR cmdline[256];
	GetPrivateProfileString("config", "cmd", 0, cmdline, sizeof(cmdline), config_file);

	CHAR nwn_dir[MAX_PATH];
	lstrcpy(nwn_dir, nwn_file);

	if(nwn_dir[1] != ':')
		lstrcpy(nwn_dir, dll_file);
	StripFilename(nwn_dir);

	char loader_dir[MAX_PATH];
	lstrcpy(loader_dir, dll_file);
	StripFilename(loader_dir);

	char profile_file[MAX_PATH];
	lstrcpy(profile_file, loader_dir);
	lstrcat(profile_file, profile_filename);
	WritePrivateProfileString("config", "profile", profile_file, config_file);
	//MessageBox(0,cmdline,0,0);
	CreateProcessWithDll(nwn_file, cmdline, nwn_dir, &si, dll_file);
	ExitProcess(0);
	
	return 0;
}
