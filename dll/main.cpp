#include "main.h"
#include "../memory.h"
#include <WinSock.h>
#include <stdlib.h>
#include <shlwapi.h>
#include "Font.h"


extern bool g_bDMhook = 0;
extern bool g_bServAuthHook = 0;

char account[65];
char password[33];
char cpu_affinity[6];

char cdkey_file[MAX_PATH];
extern char ini_path[MAX_PATH];

char WINDOW_NAME[128];
HWND g_hwndMainWindow = NULL;

struct {
	int s, flags, tolen;
	sockaddr to;
} ServerInfo;


struct {
	PINT piHP;
	PBYTE pbyAC;
	LPSTR AreaResRef;

} GameInfo;

struct AreaSize_s{
	INT x;
	INT y;
}*pAreaSize;

struct PlayerPos_s{
	FLOAT x;
	FLOAT y;
}*pPlayerPos;

tSendTo oSendTo;
INT WINAPI hSendTo(SOCKET s, LPSTR lpBuff, int nLen, int flags, const struct sockaddr* to, int tolen)
{
	if(!HIWORD(lpBuff) || nLen < 6)
		return oSendTo(s, lpBuff, nLen, flags, to, tolen);

	// refrest server info
	if(*lpBuff == 'M' && !ServerInfo.tolen)
	{
		ServerInfo.s = s;
		ServerInfo.flags = flags;
		ServerInfo.tolen = tolen;
		
		memcpy(&ServerInfo.to, to, tolen);
	}
	//cdkey check hook
	else if(g_bServAuthHook && *(PDWORD)lpBuff == 'SVNB' && lpBuff[6] == '(')
	{
		ServerInfo.tolen = 0;
		memset(lpBuff + 15, '\x00', 32);
		memset(lpBuff + 24, '\x00', 32);
		memset(lpBuff + 33, '\x00', 32);
	}
#ifdef PRIVATE
	else if(*(PDWORD)lpBuff == 'SCNB')
	{
		// Always login as player.
		// We have't DM rights, but have bypass dm password and have access to local vault
		if(g_bDMhook) lpBuff[6] = '\x10'; // 20 - dm
	}
#endif // PRIVATE	
	return oSendTo(s, lpBuff, nLen, flags, to, tolen);
}

VOID Disconnect()
{
	oSendTo(ServerInfo.s, "BNDM", 4, ServerInfo.flags, &ServerInfo.to, ServerInfo.tolen);
}

#ifdef PRIVATE
void CloneSelf(bool bCrash)
{
	char buff[36] =
		"\x42\x4E\x43\x53" // ASCI: BNCS - 4
		"\x00\x14\x10" // - 3
		"\xAD\x1F\x00\x00" // nwn client build version(8109) - 4
		"\x03\x00\x00\xAC\x49\xCF\x00" // unk - 7
		"\x08" // account name length in bytes
		"\x00\x00\x00\x00\x00\x00\x00\x00" // account name - 8
		"\x08"; // cdkey length, always 8
	//	"\x44\x44\x48\x4F\x46\x46\x42\x4D"; // cdkey short - 8 len

	memcpy((PVOID)(buff + 19), generateLogin(), 8);
	memcpy((PVOID)(buff + 28), generateCdKey(), 8);
	//add_log("login: %s, key: %s", login, cdkey);
	oSendTo(ServerInfo.s, buff, 36, ServerInfo.flags, &ServerInfo.to, ServerInfo.tolen);
	Sleep(10); //55 > 00
	oSendTo(ServerInfo.s, 
		"\x4D\xE6\x55\x00\x00\xFF\xFF\x0A\x00\x01\x00\x03\x00\x70\x01\x00", 
		16, ServerInfo.flags, &ServerInfo.to, ServerInfo.tolen);
	if(bCrash)
	{
		oSendTo(ServerInfo.s, buff, 36, ServerInfo.flags, &ServerInfo.to, ServerInfo.tolen);
		oSendTo(ServerInfo.s,  //55 > 00
			"\x4D\xE6\x55\x00\x00\xFF\xFF\x0A\x00\x01\x00\x03\x00\x70\x01\x00", 
			16, ServerInfo.flags, &ServerInfo.to, ServerInfo.tolen);
	}

}

LPSTR generateCdKey()
{
	static CHAR key[9];
	CHAR c;
	srand(time(0));
	for(int i = 0; i < 8; i++)
	{
		c = (char)(65 + (rand() % 25));
		key[i] = c;
	}
	return key;
}

LPSTR generateLogin()
{
	static char login[8];
	char c;
	char tmp;
	srand(time(0));
	for(int i = 0; i < 8; i++)
	{
		//tmp = (char)();
		switch(rand() % 2)
		{
		case 0:
			c = (char)(65 + (rand() % 25)); // заглавные
			break;
		case 1:
			c = (char)(97 + (rand() % 25)); // строчные
			break;
		case 2:
			c = (char)(48 + (rand() % 9)); // цифры
		}
		login[i] = c;
	}
	return login;
}
#endif

tGetPrivateProfileString oGetPrivateProfileString;
DWORD WINAPI hGetPrivateProfileString(
	__in   LPCTSTR lpAppName, 
	__in   LPCTSTR lpKeyName, 
	__in   LPCTSTR lpDefault, 
	__out  LPTSTR lpReturnedString, 
	__in   DWORD nSize, 
	__in   LPCTSTR lpFileName
	)
{
	LPTSTR lpFileNameNew = (LPTSTR)lpFileName;
	if(lpFileNameNew != NULL){
		extern char cdkey_file[MAX_PATH];
		extern char account[65];
		extern char cpu_affinity[6];
		if(!strcmp(".\\nwncdkey.ini", lpFileNameNew)){
			lpFileNameNew = cdkey_file;
		}
		else if(!strcmp(".\\nwnplayer.ini", lpFileNameNew)){
			if(!strcmp("Player Name", lpKeyName)){
				WritePrivateProfileString("Profile", "Player Name", account, lpFileNameNew);
			}
			else if(!strcmp("Client CPU Affinity", lpKeyName)){
				WritePrivateProfileString("Game Options", "Client CPU Affinity", cpu_affinity, lpFileNameNew);
			}
		}
	}
	return oGetPrivateProfileString(
		__in   lpAppName, 
		__in   lpKeyName, 
		__in   lpDefault, 
		__out  lpReturnedString, 
		__in   nSize, 
		__in   (LPCTSTR)lpFileNameNew);
}

tCreateMutex oCreateMutex;
HANDLE WINAPI hCreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName)
{
	//DWORD pId = GetCurrentProcessId();	
	//char c[32];
	//itoa(pId, c, 16);
	return oCreateMutex(lpMutexAttributes, bInitialOwner, lpName);
}

tOpenMutex oOpenMutex;
HANDLE WINAPI hOpenMutex(DWORD dwDesiredAccess, BOOL bInitialOwner, LPCTSTR lpName)
{
	if(strcmp("neverwinter", lpName) != 0)
		return oOpenMutex( dwDesiredAccess,  bInitialOwner,  lpName);
	else
		return NULL;
}


tCreateWindowExA oCreateWindowExA;
HWND WINAPI hCreateWindowExA(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,HINSTANCE hInstance, LPVOID lpParam
	)
{
	extern char account[65];
	char ClassName[256];
	
	HWND result = oCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	
	GetClassName(result, ClassName, 256);
	if(strcmp(ClassName, "Render Window") == 0){
		wsprintf(WINDOW_NAME, "NWN - %s", account);
		SetWindowText(result, WINDOW_NAME);	
		g_hwndMainWindow = result;
	}

	return result;
}

DWORD WINAPI HudWindowThread(LPVOID lpParam)
{
	Sleep(1000);
	char info[256];
	
	// hp
	PDWORD p_hp = NULL;
	INT hp = NULL;
	// ac
	PDWORD pAC_level1 = NULL;
	PDWORD pAC_level2 = NULL;



	PDWORD pAreaResRef_level1 = NULL;
	PDWORD pAreaResRef_level2 = NULL;

	while(true){
		if(g_hwndMainWindow){
			pAC_level1 = (PDWORD)0x0093165C;
			pAreaResRef_level1 = pAC_level1;
			p_hp = (PDWORD)0x009315FC;
			Sleep(100);
			if(InGame() && 
			  !IsBadReadPtr((PVOID)*p_hp, sizeof(PDWORD)) && 
			  !IsBadReadPtr((PVOID)*pAC_level1, sizeof(PDWORD))){
				GameInfo.piHP = (PINT)((PDWORD)(*(p_hp) + 0xE0));
				pAC_level2 = (PDWORD)(*pAC_level1 + 0x2B8);
				PDWORD pAreaResRef_level2 = (PDWORD)(*pAreaResRef_level1 + 0x28);
				if(!IsBadReadPtr((PVOID)*pAC_level2, sizeof(PDWORD)) && *GameInfo.piHP && *pAreaResRef_level2){
					GameInfo.pbyAC = (PBYTE)(*pAC_level2 + 0x50);
					GameInfo.AreaResRef = (LPSTR)(*pAreaResRef_level2 + 0xB4);
					pAreaSize = (AreaSize_s*)(*pAreaResRef_level2 + 0x8);
					pPlayerPos = (PlayerPos_s*)(*pAreaResRef_level1 + 0x1E4);
					sprintf_s(info, 
						"%s, HP: %d AC: %u x:%.1f, y:%.1f Area: %s SizeX: %d SizeY: %d", 
						WINDOW_NAME, *(GameInfo.piHP), *(GameInfo.pbyAC), pPlayerPos->x, pPlayerPos->y ,GameInfo.AreaResRef, 10 * pAreaSize->x, 10 * pAreaSize->y);
					SetWindowText(g_hwndMainWindow, info);
				}
			}
			else{
				SetWindowText(g_hwndMainWindow, WINDOW_NAME);
				Sleep(5000);
			}
			
		}
		Sleep(350);
	}
	return 0;
}

tglEnable oglEnable;

extern Config c;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	
	switch(fdwReason){
	case DLL_PROCESS_ATTACH:	
		Initialize();
//		font->InitFont();

		oSendTo = (tSendTo)DetourCreate("ws2_32.dll", "sendto", hSendTo, DETOUR_TYPE_JMP);
		oGetPrivateProfileString = 
			(tGetPrivateProfileString)DetourCreate("kernel32.dll", "GetPrivateProfileStringA", hGetPrivateProfileString, DETOUR_TYPE_JMP);
		oCreateMutex = (tCreateMutex)DetourCreate("kernel32.dll", "CreateMutexA", hCreateMutex, DETOUR_TYPE_JMP);
		oOpenMutex = (tOpenMutex)DetourCreate("kernel32.dll", "OpenMutexA", hOpenMutex, DETOUR_TYPE_JMP);	
		oCreateWindowExA = (tCreateWindowExA)DetourCreate("user32.dll", "CreateWindowExA", hCreateWindowExA, DETOUR_TYPE_JMP);	
		//oglEnable = (tglEnable)DetourCreate("opengl32.dll", "glEnable", hglEnable, DETOUR_TYPE_JMP);

		CreateThread(0, 0, PTHREAD_START_ROUTINE(HudWindowThread), 0, 0, 0);

		break;
	}
	return 0;
}

void __stdcall hglEnable(unsigned int iCap)
{
// 	add_log("glenable hooked");
// 	float color[] = {233,233,233};
// 	font->DrawString(350, 350, color, FL_NONE, "TEST STRING");
	oglEnable(iCap);
}