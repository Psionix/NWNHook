#define PRIVATE
#define VERSION "nwnhook_1.1 psi(c) all your cdkeys are belong to us"

#include <windows.h>
#include <time.h>
#include "command.h"
#include "xor.h"
#include "detourxs.h"
#include "CryptString.h"
#include "ntdll.h"
#include <gl/gl.h>
#include "config.h"
#include "other.h"
#include "../library.hpp"


typedef void (__stdcall *tglEnable)(unsigned int);
void __stdcall hglEnable(unsigned int iCap);

typedef int (WINAPI *tSendTo)(SOCKET, LPSTR, int, int, const struct sockaddr *, int);
int WINAPI hSendTo(SOCKET s, LPSTR lpBuff, int nLen, 
				   int flags, const struct sockaddr* to, int tolen);

typedef HANDLE (WINAPI *tCreateMutex)(LPSECURITY_ATTRIBUTES, BOOL, LPCTSTR);
HANDLE WINAPI hCreateMutex(  
	__in_opt  LPSECURITY_ATTRIBUTES lpMutexAttributes, 
	__in      BOOL bInitialOwner, 
	__in_opt  LPCTSTR lpName
	);
typedef HANDLE (WINAPI *tOpenMutex)(DWORD, BOOL, LPCTSTR);
HANDLE WINAPI hOpenMutex(DWORD dwDesiredAccess, BOOL bInitialOwner, LPCTSTR lpName);

typedef HWND (WINAPI *tCreateWindowExA)(DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
HWND WINAPI hCreateWindowExA(
	DWORD dwExStyle,
	LPCTSTR lpClassName,
	LPCTSTR lpWindowName,
	DWORD dwStyle,
	int x,
	int y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam
	);

typedef int (WINAPI *tRecvFrom)(SOCKET, LPSTR, int, int, const struct sockaddr *, int*);
int WINAPI hRecvFrom(SOCKET s, LPSTR lpBuff, int nLen, 
				   int flags, const struct sockaddr* from, int* fromlen);

typedef DWORD (WINAPI *tGetPrivateProfileString)(LPCTSTR, LPCTSTR, LPCTSTR, LPTSTR, DWORD, LPCTSTR);
DWORD WINAPI hGetPrivateProfileString(
	__in   LPCTSTR lpAppName, 
	__in   LPCTSTR lpKeyName, 
	__in   LPCTSTR lpDefault, 
	__out  LPTSTR lpReturnedString, 
	__in   DWORD nSize, 
	__in   LPCTSTR lpFileName
	);


#ifdef PRIVATE
void CloneSelf(bool bCrash = 0);
LPSTR generateCdKey();
LPSTR generateLogin();
#endif
void Disconnect();
