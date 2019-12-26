#include <windows.h>
#include <TlHelp32.h>

typedef LPSTR (*Cmd_t)();

typedef LPSTR (*tCommandProc)(LPSTR);

typedef struct
{
	LPSTR	Name;
	Cmd_t	pCommand;
} CommandList_t, *pCommandList_t;

#ifdef PRIVATE
LPSTR Command_WalkmeshOff();
LPSTR getCharDesc();
LPSTR Command_SetRunSpeed();
LPSTR Command_SetWalkSpeed();
LPSTR Command_SetDefWalkSpeed();
LPSTR Command_SetDefRunSpeed();
LPSTR Command_Clone();
LPSTR Command_DMhook();
LPSTR Command_ForceDebugMode();
LPSTR Command_ServerAuth();
void MetamagicAll();
void PolymorphCast();
#endif

LPSTR Command_test();
LPSTR Command_noTexture();
LPSTR Command_FogHook();
LPSTR Command_SetFogDist();
LPSTR Command_SetCamPosition();
LPSTR Command_DisconnectFromSrv();
LPSTR Command_GetDeity();
LPSTR Command_Teleport();
LPSTR CommandAppearanceHead();
LPSTR CommandAppearanceTail();
LPSTR CommandAppearanceWings();

LPSTR HookCmd_controlpart();
LPSTR HookCmd_mainscene_fog();
void AuthorizationPass();
void MultiWindow();

DWORD WINAPI SetFogDistThread(LPARAM lParam);

LPSTR new_CommandProc(LPSTR text);

VOID Args(LPSTR);
int ArgC();
DOUBLE ArgD(int i);
INT ArgI(int);
LPSTR ArgV(int);

void Initialize();



