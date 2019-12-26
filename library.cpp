#include "library.hpp"

BOOL IATHook(HMODULE ModHandle, DWORD OriginalFunc, DWORD HookFunc, void **pOriginalFunc)
{
	DWORD pe_offset,CurAddr,CurPointer,IATanfang,IATende,base;
	BOOL Hooked=FALSE;
	IMAGE_NT_HEADERS *pehdr;

	if(!ModHandle || !OriginalFunc || !HookFunc)
		return FALSE;

	base=(DWORD)ModHandle;

	memcpy(&pe_offset,(void *)(base+0x3C),sizeof(DWORD));
	pehdr=(IMAGE_NT_HEADERS *)((DWORD)base + pe_offset);

	IATanfang=(DWORD)base+pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress;
	IATende=IATanfang+pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size;

	CurAddr=IATanfang;

	while(CurAddr<IATende)
	{
		memcpy(&CurPointer,(void *)CurAddr,sizeof(DWORD));

		if(CurPointer==OriginalFunc)
		{
			if(pOriginalFunc)
				*pOriginalFunc=(PVOID)CurPointer;
			DWORD old_attributes,old_attributes2;
			if(!VirtualProtect((void *)CurAddr,sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old_attributes))
				return FALSE;
			memcpy((void *)CurAddr,&HookFunc,sizeof(DWORD));
			if(!VirtualProtect((void *)CurAddr,sizeof(DWORD), old_attributes, &old_attributes2))
				return FALSE;
			Hooked=TRUE;
		}

		CurAddr+=sizeof(DWORD);
	}
	return Hooked;
}

HMODULE GetCurrentModule()
{ // NB: XP+ solution!
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, 
		(LPCTSTR)GetCurrentModule, 
		&hModule);

	return hModule;
}

void __cdecl add_log (const char * fmt, ...)
{
	va_list va_alist;
	char logbuf[256];
	FILE * fp;
	struct tm * current_tm;
	time_t current_time;

	time (&current_time);
	current_tm = localtime (&current_time);

	sprintf (logbuf, "%02d:%02d:%02d ", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec);

	va_start (va_alist, fmt);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	if ( (fp = fopen (".\\nwnhook.log", "a")) != NULL ){
		fprintf ( fp, "%s\n", logbuf );
		fclose (fp);
	}
}
// void RemoveModuleFromPEB(void) 
// { 
// 	PTEB pTEB; 
// 	PPEB_LDR_DATA pLDR; 
// 	PLIST_ENTRY pMark, pEntry; 
// 	PLDR_MODULE pLM; 
// 	char *pSig = /*Heroin*/XorStr<0xE9,7,0x00A15E08>("\xA1\x8F\x99\x83\x84\x80"+0x00A15E08).s;
// 
// 	__asm
// 	{
// 		xor eax, eax;				// zero eax
// 		mov eax, fs:[0x18];			// get TEB
// 		mov pTEB, eax;				// set TEB struct
// 	}
// 
// 	pLDR = pTEB->Peb->LoaderData;  // get a pointer to the loader data structure within the PEB (process environment block) within the TEB 
// 
// 	pMark = &(pLDR->InMemoryOrderModuleList);  // the list is circular-linked, so we have to mark the point at which we start traversing it so we know when we've made a full traversal 
// 
// 	for(pEntry = pMark->Flink; pEntry != pMark; pEntry = pEntry->Flink) 
// 	{ 
// 		pLM = CONTAINING_RECORD(pEntry, LDR_MODULE, InMemoryOrderModuleList);  // CONTAINING_RECORD is in the DDK, it basically just gets a pointer to the actual structure from the linked list element 
// 		if((DWORD)pSig > (DWORD)pLM->BaseAddress && (DWORD)pSig < ((DWORD)pLM->BaseAddress + (DWORD)pLM->SizeOfImage))  // check if the "signature" variable is inside this module, if so it is our module 
// 		{ 
// 			pEntry->Blink->Flink = pEntry->Flink;  // change the previous element to point to the next element, so that traversing the list in a forward direction no longer yields our module 
// 			pEntry->Flink->Blink = pEntry->Blink;  // change the next element to point to the previous element, so that traversing the list in a reverse direction no longer yields our module 
// 		} 
// 	} 
// 
// 	pMark = &(pLDR->InLoadOrderModuleList); 
// 
// 	for(pEntry = pMark->Flink; pEntry != pMark; pEntry = pEntry->Flink) 
// 	{ 
// 		pLM = CONTAINING_RECORD(pEntry, LDR_MODULE, InLoadOrderModuleList); 
// 		if((DWORD)pSig > (DWORD)pLM->BaseAddress && (DWORD)pSig < ((DWORD)pLM->BaseAddress + (DWORD)pLM->SizeOfImage)) 
// 		{ 
// 			pEntry->Blink->Flink = pEntry->Flink; 
// 			pEntry->Flink->Blink = pEntry->Blink; 
// 		} 
// 	} 
// 
// 	pMark = &(pLDR->InInitializationOrderModuleList); 
// 
// 	for(pEntry = pMark->Flink; pEntry != pMark; pEntry = pEntry->Flink) 
// 	{ 
// 		pLM = CONTAINING_RECORD(pEntry, LDR_MODULE, InInitializationOrderModuleList); 
// 		if((DWORD)pSig > (DWORD)pLM->BaseAddress && (DWORD)pSig < ((DWORD)pLM->BaseAddress + (DWORD)pLM->SizeOfImage)) 
// 		{ 
// 			pEntry->Blink->Flink = pEntry->Flink; 
// 			pEntry->Flink->Blink = pEntry->Blink; 
// 		} 
// 	} 
// }

VOID StripFilename( LPSTR lpPath )
{
	LPSTR p = lpPath + lstrlen(lpPath);
	p = lpPath + lstrlen(lpPath);
	while(p >= lpPath && *p != '\\' && *p != '/')
		--p;
	if(p != lpPath) p[1] = 0;
}
