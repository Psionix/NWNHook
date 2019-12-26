#include "../memory.h"
#include "main.h"
#include <iostream>
#include <stdio.h>
#include <limits>
MODULEENTRY32 hModuleEntry;


bool g_bTex = 0;
bool g_bFogHook = 0;
bool g_bFogDistHook = 0;
FLOAT  g_fFogDist = 0.0;
PFLOAT g_pfCamMin;
PFLOAT g_pfCamMax;
PFLOAT g_pfCamAngle;
INT ap_head = -1;
INT ap_tail = -1;
INT ap_wings = -1;

#ifdef PRIVATE
bool g_bWlk = 0;
FLOAT speedDefRun = 6000.0;
FLOAT speedDefWalk = 2000.0;
PBYTE g_pPolybar1 = (PBYTE)0x004CE31E;
PBYTE g_pPolybar2 = (PBYTE)0x004CE344;
PBYTE g_pPolybarMod1 = (PBYTE)0x0051FF76;
PBYTE g_pPolybarMod2 = (PBYTE)0x005201BC;
PBYTE g_pPolyMenu;
PBYTE g_pMetaMenu;
PBYTE g_pMagEmpow;
PBYTE g_pMagMax;
PBYTE g_pMagStill;
PBYTE g_pMagSilent;
PBYTE g_pMagExtend;
PBYTE g_pMagQuick;
PBYTE g_pWalkmesh;
#endif

PBYTE g_pCommandProc;
PBYTE g_pTex1;
PBYTE g_pTex2;
PBYTE g_pTex3;
PBYTE g_pAuth;
PBYTE g_pMultiWindow;


tCommandProc oCommandProc;

CommandList_t CommandList[]=
{
#ifdef PRIVATE
	{"wlk", Command_WalkmeshOff}, 
	{"ssr", Command_SetRunSpeed}, 
	{"ssw", Command_SetWalkSpeed}, 
	{"ssrd", Command_SetDefRunSpeed}, 
	{"sswd", Command_SetDefWalkSpeed}, 
	{"clone", Command_Clone}, 
	{"dmch", Command_DMhook}, 
	{"forcedebug", Command_ForceDebugMode}, 
	{"auth", Command_ServerAuth},
	{"deity",Command_GetDeity},
	{"tp", Command_Teleport},
	//{"stat", getCharDesc}, 
#endif
	{"q", Command_DisconnectFromSrv}, 
	{"c", Command_test}, 	
	{"tex", Command_noTexture}, 
	{"foghook", Command_FogHook},
	{"fogdist", Command_SetFogDist}, 
	{"camera", Command_SetCamPosition}, 

	{"ap_head", CommandAppearanceHead}, 
	{"ap_tail", CommandAppearanceTail}, 
	{"ap_wing", CommandAppearanceWings}, 

	//Command Hooks
	{"controlpart", HookCmd_controlpart}, 
	{"mainscene.fog", HookCmd_mainscene_fog},
	{0, 0}, 
};

////////////////////////////////
LPSTR g_ArgsBuff = NULL;
LPSTR g_Command = NULL;
LPSTR g_Args[0xFF];
BYTE g_ArgsNum = 0;
////////////////////////////////
LPSTR new_CommandProc(LPSTR text)
{
	if(!*text) return oCommandProc(text);
	Args(text);
	pCommandList_t List = CommandList;

	if(*g_Command)
		while(true)
		{
			if(!strcmp(List->Name, g_Command))
				return List->pCommand();
			List++;
			if(!List->Name || !List->pCommand)
				break;
		}
	CHAR newCommandStr[4096];
	strcpy(newCommandStr, g_Command);
	for(INT i = 1; i <= g_ArgsNum; i++)
	{
		strcat(newCommandStr, " ");
		strcat(newCommandStr, g_Args[i-1]);
	}
	return oCommandProc(newCommandStr);
}

LPSTR ArgV(INT i)
{
	if(i >= 0 && i < g_ArgsNum)
		return g_Args[i];
	return 0;
}

INT ArgC()
{
	return g_ArgsNum;
}

DOUBLE ArgD(int i)
{
	if(i >= 0 && i < g_ArgsNum)
		return atof(g_Args[i]);
	return 0.0;
}

INT ArgI(int i)
{
	if(i >= 0 && i < g_ArgsNum)
		return atoi(g_Args[i]);
	return 0;
}


LPSTR HookCmd_controlpart()
{
	if(g_bFogHook) return "";
	else return oCommandProc(g_ArgsBuff);
}

LPSTR HookCmd_mainscene_fog()
{
	if(g_bFogHook) return "";
	else return oCommandProc(g_ArgsBuff);
}

LPSTR Command_FogHook()
{
	static CHAR c[32];
	g_bFogHook = !g_bFogHook;
	wsprintf(c, "fog hook: %d", g_bFogHook);
	return c;
}

LPSTR Command_SetFogDist()
{
	g_fFogDist = (FLOAT)ArgD(0);
	if(g_fFogDist > 0.0){
		g_bFogDistHook = true;
		//CreateThread(0,0, PTHREAD_START_ROUTINE(SetFogDistThread),0,0,0);
	}
	else{
		//g_fFogDist = 45.0;
		g_bFogDistHook = false;
	}
	return "Success";
}

DWORD WINAPI SetFogDistThread(LPARAM lParam)
{
	PDWORD pFogDist_lvl1 = NULL;
	PDWORD pFogDist_lvl2 = NULL;
	PFLOAT pOldFogDist = NULL;
	Sleep(1000);
	while(true){
		if(g_fFogDist > 0.0){
			pFogDist_lvl1 = (PDWORD)0x0093165C;
			if(!IsBadReadPtr((LPVOID)*pFogDist_lvl1, sizeof(PDWORD))){
				pFogDist_lvl2 = (PDWORD)(*pFogDist_lvl1 + 0x28);
				if(!IsBadReadPtr((PVOID)*pFogDist_lvl2, sizeof(PDWORD))){
					pOldFogDist = (PFLOAT)(*pFogDist_lvl2 + 0xA4);
					if(InGame() && !IsBadWritePtr((LPVOID)pOldFogDist, sizeof(FLOAT))){
						*(pOldFogDist) = g_fFogDist;
					}
				}
			}
			
		}
		Sleep(5000);
	}
	return 0;
}

LPSTR Command_SetCamPosition()
{
	static CHAR c[64];

	// нет аргументов - выдаем текущие данные
	if(ArgD(0) == 0.0)
	{
		EncryptString("min: %.02f, max: %.02f, angle: %.02f", buf1);
		sprintf_s(c, buf1, *g_pfCamMin, *g_pfCamMax, *g_pfCamAngle);
	}
	// аргументы есть - устанавливаем их
	else
	{
		DOUBLE dNewMinDist = ArgD(0), 
				dNewMaxDist = ArgD(1), 
				dNewAngle   = ArgD(2);
		DWORD dwOldProt;
		VirtualProtect(g_pfCamMin, 0x04, PAGE_EXECUTE_READWRITE, &dwOldProt);
		*(g_pfCamMin) = dNewMinDist;
		VirtualProtect(g_pfCamMin, 0x04, dwOldProt, &dwOldProt);
		if(g_ArgsNum >= 2 && dNewMaxDist != 0.0){
			VirtualProtect(g_pfCamMax, 0x04, PAGE_EXECUTE_READWRITE, &dwOldProt);
			*(g_pfCamMax) = dNewMaxDist;
			VirtualProtect(g_pfCamMax, 0x04, dwOldProt, &dwOldProt);
		}
		if(g_ArgsNum >= 2 && dNewAngle != 0.0){
			VirtualProtect(g_pfCamAngle, 0x04, PAGE_EXECUTE_READWRITE, &dwOldProt);
			*(g_pfCamAngle) = dNewAngle;
			VirtualProtect(g_pfCamAngle, 0x04, dwOldProt, &dwOldProt);
		}
		EncryptString("Re-join to game for change camera limits.", buf3);
		strcpy(c, buf3);
	}
	return c;
}

VOID Args(LPSTR pText)
{
	if(!pText || !*pText)
		return;
	g_ArgsNum = 0;
	memset(g_ArgsBuff, 0, 0x1000);
	memset(g_Command, 0, 0x100);
	strcpy(g_ArgsBuff, pText);
	LPSTR c = pText;
	INT i = 0;
	while(*c && *c != ' '){
		*g_Command++ = *c++;
		i++;
	}
	g_Command -= i;
	while(*c == ' ')
		c++;

	if(lstrlen(c) > 0x1000)
		return;

	while(*c)
	{
		while(*c == ' ')
			c++;

		if(!*c)
			break;
		if(*c == '\"')
		{
			g_Args[g_ArgsNum++] = ++c;

			while(*c && *c != '\"')
				c++;
		}
		else
		{
			g_Args[g_ArgsNum++] = c;

			while(*c && *c != ' ')
				c++;
		}
		if(!*c)
			break;
		*c = 0;
		c++;
	}
}

LPSTR Command_DisconnectFromSrv()
{
	Disconnect();
	return "Disconnected";
}


LPSTR Command_test()
{
	EncryptString(VERSION, buf);
	return buf;
}

VOID AuthorizationPass()
{
	BYTE b[] = {0x84, 0xF2, 0x06};
	vprot_memcpy((PVOID)g_pAuth, &b, sizeof(b));
}

LPSTR Command_noTexture()
{
	static char c[32];
	BYTE oldbytes[] = {0x8A, 0x88, 0x6C, 0x01, 0x00, 0x00};
	BYTE newbytes[] = {0xB1, 0x01, 0x90, 0x90, 0x90, 0x90};
	if(g_bTex)
	{
		vprot_memcpy(g_pTex1, oldbytes, sizeof(oldbytes));
		vprot_memcpy(g_pTex2, oldbytes, sizeof(oldbytes));
		vprot_memcpy(g_pTex3, oldbytes, sizeof(oldbytes));
	}
	else
	{
		vprot_memcpy(g_pTex1, newbytes, sizeof(newbytes));
		vprot_memcpy(g_pTex2, newbytes, sizeof(newbytes));
		vprot_memcpy(g_pTex3, newbytes, sizeof(newbytes));
	}
	g_bTex = !g_bTex;
	wsprintf(c, "LEGO: %d", g_bTex);
	return c;
}

void MultiWindow()
{
	BYTE ch[] = {0xEB};
	vprot_memcpy(g_pMultiWindow, ch, sizeof(ch));	
}

LPSTR CommandAppearanceHead()
{
	int arg = ArgI(0);
	if(arg != -1)
	{
		ap_head = arg;
	}
	return "Success";
}
LPSTR CommandAppearanceTail()
{
	int arg = ArgI(0);
	if(arg != -1)
	{
		ap_tail = arg;
	}
	return "Success";
}
LPSTR CommandAppearanceWings()
{
	int arg = ArgI(0);
	if(arg != -1){
		ap_wings = arg;
	}
	return "Success";
}

#ifdef PRIVATE
LPSTR Command_Teleport()
{
	PFLOAT pOldX = (PFLOAT)(*(PDWORD)0x0093165C + 0x1E4);
	PFLOAT pOldY = (PFLOAT)(*(PDWORD)0x0093165C + 0x1E8);
	if(pOldX && pOldY){
		*(pOldX) = (FLOAT)ArgD(0);
		*(pOldY) = (FLOAT)ArgD(1);
		return "Success";
	}
	else	
		return "Failure";

	return "";
}
LPSTR Command_GetDeity()
{
	static char c[128];
	PDWORD p_Deity = (PDWORD)0x00000000;

	// Valid Pointer - char list
	if(true){

	}
	// Valid Pointer - in game
	else if(true){

	}
	// invalid
	else{
		strcpy(c, "ERROR: null_ptr");
	}
	return c;
}
LPSTR Command_ServerAuth()
{
	extern bool g_bServAuthHook;
	static char c[2];
	g_bServAuthHook = !g_bServAuthHook;
	EncryptString("%d", buf);
	wsprintf(c, buf, g_bServAuthHook);
	return c;
}
LPSTR Command_ForceDebugMode()
{
	static CHAR c[32];
	bool b = ArgI(0);
	*((PDWORD)0x06BAB5CC) = b;
	wsprintf(c, "%d", b);
	return c;
}


LPSTR Command_Clone()
{
	CloneSelf(ArgI(0));
	return "Success";
}

LPSTR Command_SetRunSpeed()
{
	PDWORD pp_OldSpeed = (PDWORD)0x0093165C;
	//wsprintf(c,"%p",*p1);
	if(*pp_OldSpeed == NULL)
	{
		//strcpy("ERROR: INVALID POINTER", feedback);
		return "ERROR: null_ptr";
	} 
	
	PFLOAT pfSpeedOld = (PFLOAT)(*(pp_OldSpeed) + 0x218);
	FLOAT fSpeedNew = (FLOAT)ArgD(0);

	if(fSpeedNew == 0.0) return "Invalid value.";

	if(*pfSpeedOld == fSpeedNew)	{*(pfSpeedOld) = speedDefRun; return "run speed restored";}
	else						*(pfSpeedOld) = fSpeedNew;
	return "Success";
}

LPSTR Command_SetWalkSpeed()
{
	PDWORD pp_OldSpeed = (PDWORD)0x0093165C;
	//wsprintf(c,"%p",*p1);
	if(*pp_OldSpeed == NULL)
	{
		return "ERROR: null_ptr";
	}
	
	PFLOAT pfSpeedOld = (PFLOAT)(*(pp_OldSpeed) + 0x214);
	DOUBLE speed_new = (FLOAT)ArgD(0);

	if(speed_new <= 0.0) return "Invalid value.";

	if(*pfSpeedOld == speed_new)	{*(pfSpeedOld) = speedDefWalk; return "walk speed restored";}
	else							*(pfSpeedOld) = speed_new;
	return "Success";
}

LPSTR Command_SetDefRunSpeed()
{
	FLOAT argspeed = ArgD(0);
	if(argspeed != 0.0) speedDefRun = argspeed; 
	return "Success";
}

LPSTR Command_SetDefWalkSpeed()
{
	FLOAT argspeed = ArgD(0);
	if(argspeed != 0.0) speedDefWalk = argspeed; 
	return "Success";
}

LPSTR getCharDesc()
{
	static char c[256];
	const PDWORD p_hp = (PDWORD)0x009315FC;
	const PDWORD p_resref = (PDWORD)0x0093165C;

	if( !*p_hp || !*p_resref)  
	{
		EncryptString("NULLPOINTER", buf2);
		strcpy(c, buf2);
		return c;
	}
	else
	{
		int hp = *((PDWORD)(*p_hp + 0xE0));
		//PDWORD pp_OldSpeed = (PDWORD)0x0093165C;
		//PFLOAT pfSpeedOld = (PFLOAT)(*(pp_OldSpeed) + 0x218);

		PDWORD p1 = (PDWORD)(0x0093165C) + 0x2B8;
		PUCHAR pac = (PUCHAR)*((PDWORD)*(p1) + 0x50);
		unsigned int ac = (INT)*pac;
		//unsigned char ac = *((PDWORD)(*(*p_resref + 0x2B8)) + 0x50);
		//LPSTR resref = (LPSTR)(*((PDWORD)(*p_resref + 0x28)) + 0xB4);
		//float srun = (FLOAT)((*(PDWORD)0x0043165C) + 0x218);
		//float swalk = (FLOAT)(*(PDWORD)0x0043165C + 0x214);
		//float px = (FLOAT)*((PDWORD)0x092E588);
		//float py = (FLOAT)*((PDWORD)0x092E58C);
		EncryptString(
			"hp: %d, ac: %u", buf);
		sprintf_s(c, buf, 
			hp, ac); //srun, swalk, resref, px, py);
	}
	return c;
}

LPSTR Command_WalkmeshOff()
{
	static CHAR c[128];
	BYTE wlk[] = {0x90, 0x90};
	if(g_bWlk)
	{wlk[0] = 0x75; wlk[1] = 0x1B;}
	vprot_memcpy((void*)g_pWalkmesh, &wlk, sizeof(wlk));
	g_bWlk = !g_bWlk;
	wsprintf(c, "walkhack: %d", g_bWlk);
	return c;
}

LPSTR Command_DMhook()
{
	extern bool g_bDMhook;
	static CHAR c[2];
	g_bDMhook = !g_bDMhook;
	EncryptString("%d", buf);
	wsprintf(c, buf, g_bDMhook);
	return c;
}

void MetamagicAll()
{
	BYTE asmNOP2[] = {0x90, 0x90};
	BYTE asmJMP[] = {0xEB, 0x23};
	BYTE menu[] = {0xE9, 0xEA, 0x00, 0x00, 0x00, 0x90}; 
	vprot_memcpy(g_pMetaMenu, menu, sizeof(menu));
	vprot_memcpy(g_pMagEmpow, asmNOP2, sizeof(asmNOP2));
	vprot_memcpy(g_pMagExtend, asmNOP2, sizeof(asmNOP2));
	vprot_memcpy(g_pMagSilent, asmNOP2, sizeof(asmNOP2));
	vprot_memcpy(g_pMagMax, asmNOP2, sizeof(asmNOP2));
	vprot_memcpy(g_pMagQuick, asmNOP2, sizeof(asmNOP2));
	vprot_memcpy(g_pMagStill, asmJMP, sizeof(asmJMP));
}

void PolymorphCast()
{
	BYTE asmJMP[] = {0xEB};
	BYTE polyMod[] = {0xB8, 0x00, 0x00, 0x00, 0x00, 0x90};
	BYTE polyMenu[] = {0x83, 0xFE, 0x00, 0x90, 0x90, 0x90};
	vprot_memcpy((PVOID)g_pPolybar1, &asmJMP, sizeof(asmJMP));
	vprot_memcpy((PVOID)g_pPolybar2, &asmJMP, sizeof(asmJMP));
	vprot_memcpy((PVOID)g_pPolybarMod1, &polyMod, sizeof(polyMod));
	vprot_memcpy((PVOID)g_pPolybarMod2, &polyMod, sizeof(polyMod));
	vprot_memcpy((PVOID)g_pPolyMenu, &polyMenu, sizeof(polyMenu));
}
#endif

void Initialize()
{
#ifdef PRIVATE
	BYTE SIG_PolyBar[] =   "\x74\x0F\x33\xC9\x85\xF6\x0F\x94\xC1\x51\x8B\xC8";
	EncryptString("xxxxxxxxxxxx", MSK_PolyBar);
	BYTE SIG_PolyBar2[] =  "\x74\x0E\x8B\x40\x38\x85\xC0\x74\x07\x8B\xC8\xE8\xFF\xFF\xFF\xFF\x5F\x5E\xC2\x04\x00";
	EncryptString("xxxxxxxxxxxx????xxxxx", MSK_PolyBar2);
	BYTE SIG_PolyBarMod[]= "\x8B\x81\x00\x00\x00\x00\x81\xE5\x00\x00\x00\x00\x83\xE7\x03\x83\xE3\x3F\x85\xC0";
	EncryptString("xx????xx????xxxxxxxx", MSK_PolyBarMod);
	BYTE SIG_PolyBarMod2[]="\x8B\x82\x00\x00\x00\x00\x85\xC0\x74\x09\x8B\x06\x6A\x00\x8B\xCE";
	EncryptString("xx????xxxxxxxxxx", MSK_PolyBarMod2);
	BYTE SIG_PolyMenu[] =  "\x39\xB3\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\x8B\xBB\x00\x00\x00\x00\x83\xFF\xFF";
	EncryptString("xx????xx????xx????xxx", MSK_PolyMenu);

	BYTE SIG_WlkCheck[] =  "\x75\x1B\x8B\xCF\x8B\x11\x8B\x41\x04\x8B\x49\x08\x89\x54\x24\x44\x89\x44\x24\x48\x89\x4C\x24\x4C\xE9\x00\x00\x00\x00\x8A\x84\x24\x30\x01\x00\x00\x3C\x01\x74\x08\x3C\x02";
	EncryptString("xxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxx", MSK_WlkCheck);

	BYTE SIG_MetaMenu[] =  "\x0F\x85\xE9\x00\x00\x00\x6A\x19\x8D\x8A\xC4\x00\x00\x00\xE8\x83\x33\xF6\xFF\x85\xC0\x0F\x85\xD4\x00\x00\x00\x8B\x44\x24\x10";
	EncryptString("xx????xxxxx????xxxxxxxxxxxxxxxx", MSK_MetaMenu);
	BYTE SIG_Empower[] =   "\x74\x35\x8B\x44\x24\x14\x8B\xCF\x83\xC9\x08\xC1\xE1\x08";
	EncryptString("xxxxxxxxxxxxxx", MSK_Empower);
	BYTE SIG_Extend[] =    "\x74\x35\x8B\x44\x24\x14\x8B\xCF\x83\xC9\x10\xC1\xE1\x08\x8D\xD5";
	EncryptString("xxxxxxxxxxxxx", MSK_Extend);
	BYTE SIG_Silent[] =    "\x74\x38\x8B\x44\x24\x14\x8B\xCF\x81\xC9\x80\x00\x00\x00\xC1\xE1\x08\x8B\xD5";
	EncryptString("xxxxxxxxxxxxxxxxxxx", MSK_Silent);
	BYTE SIG_Still[] =     "\x75\x23\x8B\x88\xC8\x00\x00\x00\x33\xD2\x85\xC9\x7E\x4B\x8B\x80\xC4\x00\x00\x00\x66\x83\x38\x25";
	EncryptString("xxxxxxxxxxxxxxxxxxxxxxxx", MSK_Still);
	BYTE SIG_Max[]   =     "\x74\x35\x8B\x44\x24\x14\x8B\xCF\x83\xC9\x20\xC1\xE1\x08\x8B\xD5\x81\xE2\x00\x00\x00\x00\x0B\xCA\xC1\xE1\x0C\x51\x8B\x4C\x24\x14\x68\x00\x00\x00\x00\x50\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\x6A\x00\xE8\x00\x00\x00\x00\x8B\x93";
	EncryptString("xxxxxxxxxxxxxxxxxx????xxxxxxxxxxx????xx????x????xxx????xx", MSK_Max);
	BYTE SIG_Quick[] =     "\x74\x35\x8B\x44\x24\x14\x8B\xCF\x83\xC9\x40\xC1\xE1\x08\x8B\xD5\x81\xE2\xFF\x00\x00\x00\x0B\xCA\xC1\xE1\x0C\x51\x8B\x4C\x24\x14\x68\x70\x4F\x59\x00\x50\x68\xD8\x00\x00\x00\x68\x64\xD0\x90\x00\x6A\x04";
	EncryptString("xxxxxxxxxxxxxxxxxx????xxxxxxxxxxx????xx????x????xx", MSK_Quick);
#endif


	BYTE SIG_Command[] =   "\xB8\x84\xC0\x00\x00\xE8\xFF\xFF\xFF\xFF\x53\x8B\x9C\x24\x8C\xC0\x00\x00\x55\x56\x57\x8A\x03\x3C";
	EncryptString("xxxxxx????xxxxxxxxxxxxxx", MSK_Command);

	BYTE SIG_Auth[] =      "\x85\x01\x07\x00\x00\x8B\x46\x6C\x83\xF8\x04";
	EncryptString("xxxxxxxxxxx", MSK_Auth);

	BYTE SIG_Tex1[] =       "\x8A\x88\x6C\x01\x00\x00\x84\xC9\x0F\x84\x00\x00\x00\x00\xA0\x90\xA4\x94\x00\xBB\x00\x00\x00\x00\x84\xC3\x0F\x85";
	EncryptString("xxxxxxxxxx????xxxxxx????xxxx", MSK_Tex1);
	BYTE SIG_Tex2[] =       "\x8A\x88\x6C\x01\x00\x00\x84\xC9\x74\x43\x8B\x73\x6C\x85\xF6";
	EncryptString("xxxxxxxxxxxxxxx", MSK_Tex2);
	BYTE SIG_Tex3[] =       "\x8A\x88\x6C\x01\x00\x00\x84\xC9\x75\x0F\x8B\x54\x24\x5C";
	EncryptString("xxxxxxxxxxxxxx", MSK_Tex3);
	BYTE SIG_MWindow[] = "\x74\x0F\x5F\x5E\x83\xC8\xFF\x5B\x81\xC4\x00\x00\x00\x00\xC2\x10\x00\x33\xC0";
	EncryptString("xxxxxxxxxx????xxxxx", MSK_MWindow);

	
	g_ArgsBuff = (LPSTR)AllocatePages(0x1000);
	g_Command = (LPSTR)AllocatePages(0x100);

	PBYTE base_addr = (PBYTE)0x401000;
	DWORD scansize = 0x4a5000;

	g_pMultiWindow = FindPattern(SIG_MWindow, MSK_MWindow, base_addr, scansize);
	MultiWindow();

	g_pAuth = FindPattern(SIG_Auth, MSK_Auth, base_addr, scansize);
	g_pTex1 = FindPattern(SIG_Tex1, MSK_Tex1, base_addr, scansize);
	g_pTex2 = FindPattern(SIG_Tex2, MSK_Tex2, base_addr, scansize);
	g_pTex3 = FindPattern(SIG_Tex3, MSK_Tex3, base_addr, scansize);
	g_pfCamMin = (PFLOAT)(PDWORD(0x004A93ED));
	g_pfCamMax = (PFLOAT)(PDWORD(0x004A93F7));
	g_pfCamAngle = (PFLOAT)(PDWORD(0x004A940B));
	g_pCommandProc = FindPattern(SIG_Command, MSK_Command, base_addr, scansize);
#ifdef PRIVATE

	g_pPolyMenu = FindPattern(SIG_PolyMenu, MSK_PolyMenu, base_addr, scansize);
	g_pWalkmesh = FindPattern(SIG_WlkCheck, MSK_WlkCheck, base_addr, scansize);
	g_pMetaMenu = FindPattern(SIG_MetaMenu, MSK_MetaMenu, base_addr, scansize);
	g_pMagEmpow = FindPattern(SIG_Empower, MSK_Empower, base_addr, scansize);
	g_pMagExtend = FindPattern(SIG_Extend, MSK_Extend, base_addr, scansize);
	g_pMagQuick = FindPattern(SIG_Quick, MSK_Quick, base_addr, scansize);
	g_pMagSilent = FindPattern(SIG_Silent, MSK_Silent, base_addr, scansize);
	g_pMagStill = FindPattern(SIG_Still, MSK_Still, base_addr, scansize);
	g_pMagMax = FindPattern(SIG_Max, MSK_Max, base_addr, scansize);
#endif

	EncryptString("Unable to hook command line", MsgErrorHook)
	if(!g_pCommandProc) MessageBox(0, MsgErrorHook, 0, 0);


	oCommandProc = (tCommandProc) 
		DetourCreate((LPVOID)g_pCommandProc, new_CommandProc, DETOUR_TYPE_JMP);



	// set profile filename from config
	CHAR ini_path[MAX_PATH];
	DWORD dwLength = GetModuleFileName(GetCurrentModule(), ini_path, MAX_PATH);
	*(PDWORD)(ini_path + dwLength - 4) = 'ini.';
	CHAR profile_path[MAX_PATH];
	GetPrivateProfileString("config", "profile", 0, profile_path, sizeof(profile_path), ini_path);

	LPSTR config_section = "hook";

	g_bFogHook = GetPrivateProfileInt(config_section, "foghook", 0, profile_path);
	
	g_fFogDist = (FLOAT)GetPrivateProfileInt(config_section, "fogdist", 0, profile_path);
	if(g_fFogDist > 0.0)
	{
		g_bFogDistHook = true;
		CreateThread(0,0, PTHREAD_START_ROUTINE(SetFogDistThread),0,0,0);
	}
	DWORD dwOldProt;
	// set camera limits from config
	VirtualProtect(g_pfCamMin, 0x04, PAGE_EXECUTE_READWRITE, &dwOldProt);
	*g_pfCamMin =   (FLOAT)GetPrivateProfileInt(config_section, "camera_min", 1, profile_path);
	VirtualProtect(g_pfCamMin, 0x04, dwOldProt, &dwOldProt);
	VirtualProtect(g_pfCamMax, 0x04, PAGE_EXECUTE_READWRITE, &dwOldProt);
	*g_pfCamMax =   (FLOAT)GetPrivateProfileInt(config_section, "camera_max", 25, profile_path);
	VirtualProtect(g_pfCamMax, 0x04, dwOldProt, &dwOldProt);
	VirtualProtect(g_pfCamAngle, 0x04, PAGE_EXECUTE_READWRITE, &dwOldProt);
	*g_pfCamAngle = (FLOAT)GetPrivateProfileInt(config_section, "camera_angle", 89, profile_path);
	VirtualProtect(g_pfCamAngle, 0x04, dwOldProt, &dwOldProt);

	extern CHAR account[65];
	GetPrivateProfileString(config_section, "account", "default", account, sizeof(account), profile_path);
	
	extern CHAR password[33];
	GetPrivateProfileString(config_section, "password", "pass", password, sizeof(password), profile_path);

	extern CHAR cdkey_file[MAX_PATH];
	GetPrivateProfileString(config_section, "cdkey_file", ".\\nwncdkey.ini", cdkey_file, sizeof(cdkey_file), profile_path);

	extern CHAR cpu_affinity[6];
	GetPrivateProfileString(config_section, "affinity", "-1", cpu_affinity, sizeof(cdkey_file), profile_path);

#ifdef PRIVATE
	PolymorphCast();
	MetamagicAll();
#endif
	AuthorizationPass();
}