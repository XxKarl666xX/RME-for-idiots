#include "stdafx.h"

cSecurity* Security;
MemoryManager *mManager;

#if defined (DUMP_BINS)
void cSecurity::binToFile(const char* fname, PVOID dPtr, ULONG len)
{
	FILE* fp;
	fopen_s(&fp, fname, "wb");
	fwrite(dPtr, len, 1, fp);
	fflush(fp);
	fclose(fp);
}

#define getCryptedCoDDump(a) ((DWORD)(docryptoBypass(a, (QWORD)((((DWORD*)&a - (DWORD*)&addrs))))))
BYTE dumpBuf[sizeof(addresses_s)];


#if defined (BO2)
#define DUMP_PATH "HDD:\\addresses_bo2.bin"
#elif defined (BO1) || defined (BO1_ZM)
#define DUMP_PATH "HDD:\\addresses_bo1.bin"
#endif


VOID cSecurity::DumpPatchBinary()
{
	if (!bHasPatches)
		return;

	memset(dumpBuf, 0, sizeof(addresses_s));

	DWORD* fileBuff = (DWORD*)&dumpBuf[0];
	for (BYTE i = 0; i * 4 < sizeof(addresses_s); i += 1)
		fileBuff[i] = (DWORD)getCryptedCoDDump(*(DWORD*)((DWORD)&addrs + ((DWORD)i * 4)));

	DbgPrint("Saving addresses to %s", DUMP_PATH);
	binToFile(DUMP_PATH, dumpBuf, sizeof(addresses_s));
}
#endif