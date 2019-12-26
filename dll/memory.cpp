#include "main.h"
#include "../memory.h"

PBYTE m_pbAllocBuff_Local = NULL;
ULONG m_ulAllocBuffPos_Local = 0;

#define MLA 0x100000

PVOID AllocatePages(ULONG ulSize)
{
	PVOID pAddress = NULL;
	ULONG ulAllocationSize = ulSize;
	NtAllocateVirtualMemory(INVALID_HANDLE_VALUE, &pAddress, 0, &ulAllocationSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	return pAddress;
}

PVOID Allocate(ULONG ulSize)
{
	PBYTE pbAllocBuff = m_pbAllocBuff_Local;
	ULONG ulAllocBuffPos = m_ulAllocBuffPos_Local;

	if(!ulSize)
		return NULL;

	PVOID pAddress = NULL;

	if(!pbAllocBuff || ulAllocBuffPos + ulSize > MLA)
	{
		pAddress = (PBYTE)AllocatePages(MLA);
		m_pbAllocBuff_Local = (PBYTE)pAddress;
		ulAllocBuffPos = ulSize;
	}
	else
	{
		pAddress = pbAllocBuff + ulAllocBuffPos;
		ulAllocBuffPos += ulSize;
	}

	ulAllocBuffPos += 8 - (ulAllocBuffPos & 7);// 16 - (m_ulAllocBuffPos & 15);
	m_ulAllocBuffPos_Local = ulAllocBuffPos;

	return pAddress;
}

// VOID Free(PVOID pAddress)
// {
// 
// }

inline bool DataCompare(PBYTE pData, PBYTE pMask, LPSTR szMask)
{
	for( ; *szMask; ++szMask, ++pData, ++pMask )
		if(*szMask == 'x' && *pData != *pMask )
			return false;

	return (*szMask) == NULL;
}

PBYTE FindPattern(PBYTE pbMask, LPSTR szMask, PBYTE pbAddress, DWORD dwSize)
{
	for(DWORD i = 0; i < dwSize; i++)
		if(DataCompare(pbAddress + i, pbMask, szMask))
			return pbAddress + i;

	return 0;
}

void *vprot_memcpy(void* dest, const void* src, size_t size)
{
	if(!dest) return (void*)NULL;
	unsigned long Protect;
	VirtualProtect(dest, size, PAGE_READWRITE, &Protect);
	void* ret = memcpy(dest, src, size);
	VirtualProtect(dest, size, Protect, &Protect);
	return ret;
}
