#include <Windows.h>

PVOID AllocatePages(ULONG);
PVOID Allocate(ULONG);
PBYTE FindPattern(PBYTE pbMask, LPSTR szMask, PBYTE pbAddress, DWORD dwSize);
inline bool DataCompare(PBYTE pData, PBYTE pMask, LPSTR szMask);
void *vprot_memcpy(void* dest, const void* src, size_t size);
