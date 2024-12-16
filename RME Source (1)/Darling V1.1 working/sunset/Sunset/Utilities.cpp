#include "stdafx.h"

unsigned long long Kink = 0x265FC73CB7E11A8B;

void* Utilities::ResolveFunction(const char* moduleName, DWORD ordinal) {
	PVOID proc = NULL;
	HANDLE hModule;

	if (!NT_SUCCESS(XexGetModuleHandle((char*)moduleName, &hModule)))
		return 0;

	if (!NT_SUCCESS(XexGetProcedureAddress(hModule, ordinal, &proc)))
		return 0;

	return proc;
}

void* Utilities::ResolveFunctionFromHandle(HANDLE hModule, DWORD ordinal) {
	PVOID proc = NULL;

	if (!NT_SUCCESS(XexGetProcedureAddress(hModule, ordinal, &proc)))
		return 0;

	return proc;
}

void Utilities::Notify(const char* text, XNOTIFYQUEUEUI_TYPE exnq) {
	wchar_t* buf = (wchar_t*)calloc(1024, 1);
	wsprintfW(buf, L"%S", text);
	XNotifyQueueUI(exnq, 0, 2, buf, 0);
	free(buf);
}

BOOL Utilities::FileExists(PCHAR FilePath)
{
	if (GetFileAttributes(FilePath) == (DWORD)INVALID_HANDLE_VALUE)
	{
		DWORD LastError = GetLastError();
		if ((LastError == ERROR_PATH_NOT_FOUND) || (LastError == ERROR_FILE_NOT_FOUND)) return FALSE;
	}
	return TRUE;
}

BOOL Utilities::CReadFile(PCHAR FilePath, MemoryBuffer& Buffer)
{
	HANDLE Handle = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (Handle == INVALID_HANDLE_VALUE) return FALSE;
	DWORD FileSize = GetFileSize(Handle, 0);
	PBYTE FileBuffer = (PBYTE)malloc(FileSize);
	if (FileBuffer == 0)
	{
		CloseHandle(Handle);
		return FALSE;
	}
	DWORD ReadSize = 0;
	if (!ReadFile(Handle, FileBuffer, FileSize, &ReadSize, 0))
	{
		free(FileBuffer);
		CloseHandle(Handle);
		return FALSE;
	}
	else if (ReadSize != FileSize)
	{
		free(FileBuffer);
		CloseHandle(Handle);
		return FALSE;
	}
	Buffer.Add(FileBuffer, FileSize);
	free(FileBuffer);
	CloseHandle(Handle);
	return TRUE;
}

BOOL Utilities::CWriteFile(PCHAR FilePath, PVOID Buffer, DWORD Size)
{
	DWORD WriteSize = Size;
	HANDLE Handle = CreateFile(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (Handle == INVALID_HANDLE_VALUE) return FALSE;
	if (!WriteFile(Handle, Buffer, WriteSize, &WriteSize, 0))
	{
		CloseHandle(Handle);
		return FALSE;
	}
	CloseHandle(Handle);
	return (WriteSize == Size);
}

DWORD BytesRead;
BOOL Utilities::WriteFileAlt(PCHAR Path, LPCVOID Buffer, DWORD dwBytesToWrite)
{
	HANDLE hFile = CreateFile(Path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile); return FALSE;
	}
	WriteFile(hFile, Buffer, dwBytesToWrite, &BytesRead, NULL);
	CloseHandle(hFile);
	return TRUE;
}

VOID Utilities::PatchInJump(DWORD* Address, void* Dest, BOOL Linked) {

	DWORD Bytes[4];
	DWORD Destination = (DWORD)Dest;

	Bytes[0] = 0x3C000000 + ((Destination >> 16) & 0xFFFF);// lis 	%r0, dest>>16
	Bytes[1] = 0x60000000 + (Destination & 0xFFFF); // ori	%r0, %r0, dest&0xFFFF
	Bytes[2] = 0x7C0903A6; // mtctr	%r0
	Bytes[3] = 0x4E800420; // bctr

	if (Linked)
		Bytes[3] += 1; // bctrl

					   //memcpy(Address, Bytes, 0x10);
	*(__int64 *)((DWORD)Address + 0) = *(__int64 *)&Bytes[0];
	*(__int64 *)((DWORD)Address + 8) = *(__int64 *)&Bytes[2];
	__dcbst(0, Address);
	__sync();
	__isync();
}

float Utilities::GetDistance(vec3 c1, vec3 c2) {
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;
	float dz = c2.z - c1.z;

	return sqrt((float)((dx * dx) + (dy * dy) + (dz * dz)));
}

void Utilities::AngleVectors(vec3 angles, vec3* forward, vec3* right, vec3* up) {
	float angle;
	float sr, sp, sy, cr, cp, cy;
	angle = angles.y * (PI * 2.0f / 360.0f);
	sy = sinf(angle);
	cy = cosf(angle);
	angle = angles.x * (PI * 2.0f / 360.0f);
	sp = sinf(angle);
	cp = cosf(angle);
	angle = angles.z * (PI * 2.0f / 360.0f);
	sr = sinf(angle);
	cr = cosf(angle);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up) {
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}

void Utilities::VectorToAngle(vec3* direction, vec3* out) {
	float forward, yaw, pitch, PiDiv = (180.0f / XM_PI);

	if (direction->y == 0 && direction->x == 0) {
		yaw = 0;
		if (direction->z > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else {
		if (direction->x)
			yaw = (atan2(direction->y, direction->x) * PiDiv);
		else if (direction->y > 0)
			yaw = 90.0f;
		else
			yaw = 270.0f;

		if (yaw < 0.0f)
			yaw += 360.0f;

		forward = sqrt((direction->x * direction->x) + (direction->y * direction->y));
		pitch = (atan2(direction->z, forward) * PiDiv);

		if (pitch < 0.0f)
			pitch += 360;
	}

	out->x = 360.0f - pitch;
	out->y = yaw;
	out->z = 0.0f;

	if (out->x > 360.0f)
		out->x -= 360.0f;
	if (out->x < 0.0f)
		out->x += 360.0f;

	if (out->y > 360.0f)
		out->y -= 360.0f;
	if (out->y < 0.0f)
		out->y += 360.0f;

	if (out->z > 360.0f)
		out->z -= 360.0f;
	if (out->z < 0.0f)
		out->z += 360.0f;
}

vec3 Utilities::VectorToAngles(vec3 &Delta)
{
	float Forward, Yaw, Pitch;

	if (Delta.x == 0 && Delta.y == 0)
	{
		Yaw = 0;
		if (Delta.z > 0)
			Pitch = 90.0f;
		else
			Pitch = 270.0f;
	}
	else
	{
		if (Delta.x)
			Yaw = (atan2(Delta.y, Delta.x) * 57.2957804f);
		else if (Delta.y > 0)
			Yaw = 90.0f;
		else
			Yaw = 270.0f;

		if (Yaw < 0)
			Yaw += 360.0f;

		Forward = sqrt((Delta.x * Delta.x) + (Delta.y * Delta.y));
		Pitch = (atan2(Delta.z, Forward) * 57.2957804f);

		if (Pitch < 0)
			Pitch += 360;
	}
	return vec3(-Pitch, Yaw, 0);
}

float Utilities::FloatMinMax(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random * range) + min;
}

char* Utilities::GetSelectedString(char** array, int index) {
	return array[index];
}

int Utilities::GetSelectedIndexForString(char** array, int arraySize, const char* inputString) {
	int finalIndex = 0;

	for (int i = 0; i < arraySize; i++) {
		if (!strcmp(array[i], inputString))
			finalIndex = i;
	}

	return finalIndex;
}

PLIST_ENTRY PsLoadedModuleList = NULL;

DWORD _resolve(HMODULE Module, DWORD Export, DWORD off1, DWORD off2)
{
	SHORT t1, t2;
	DWORD *f;

	if (FAILED(XexGetProcedureAddress(Module, Export, &f)))
		return 0;

	t1 = f[off1] & 0xFFFF;
	t2 = f[off2] & 0xFFFF;

	return (t1 << 16) + t2;
}

void Utilities::RemoveFromList(HANDLE hModule)
{
	PLDR_DATA_TABLE_ENTRY Module;
	PLIST_ENTRY preMod, nextMod, ps;

	if (hModule == 0)
		return;

	Module = (PLDR_DATA_TABLE_ENTRY)hModule;

	// Remove the module from the PsLoadedModuleList
	if (PsLoadedModuleList == 0)
		PsLoadedModuleList = (PLIST_ENTRY)_resolve(GetModuleHandle("xboxkrnl.exe"), 412, 9, 11);

	ps = PsLoadedModuleList->Flink;
	while (ps != PsLoadedModuleList)
	{
		// if the base address is the same
		if (CONTAINING_RECORD(ps, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks)->NtHeadersBase == Module->NtHeadersBase)
		{
			preMod = ps->Blink;
			nextMod = ps->Flink;

			preMod->Flink = nextMod;
			nextMod->Blink = preMod;
		}
		ps = ps->Flink;
	}
}


DWORD Utilities::GetModuleImportAddr(HANDLE HModule, HANDLE HImportedModule, DWORD Ordinal) {

	PLDR_DATA_TABLE_ENTRY_2 Module = (PLDR_DATA_TABLE_ENTRY_2)HModule;

	DWORD address = (DWORD)GetProcAddress((HMODULE)HImportedModule, (LPCSTR)Ordinal);

	if (address == NULL || HModule == NULL)
		return 0;

	// Get our header field from this module
	VOID* headerBase = Module->XexHeaderBase;
	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)
		RtlImageXexHeaderField(headerBase, 0x000103FF);
	if (importDesc == NULL)
		return 0;

	// Get our string table position
	CHAR* stringTable = (CHAR*)(importDesc + 1);

	// Get our first entry
	PXEX_IMPORT_TABLE_2 importTable = (PXEX_IMPORT_TABLE_2)
		(stringTable + importDesc->NameTableSize);

	// Loop through our table
	for (DWORD x = 0; x < importDesc->ModuleCount; x++) {

		// Go through and search all addresses for something that links
		DWORD* importAdd = (DWORD*)(importTable + 1);
		for (DWORD y = 0; y < importTable->ImportTable.ImportCount; y++) {

			// Check the address of this import
			DWORD value = *((DWORD*)importAdd[y]);
			if (value == address) {
				return (DWORD)(&importAdd[y + 1]);
				//return importAdd[y + 1];
			}
		}

		// Goto the next table
		importTable = (PXEX_IMPORT_TABLE_2)(((BYTE*)importTable) +
			importTable->TableSize);
	}

	return 0;
}

BOOL Utilities::Compare(PCHAR str1, PCHAR str2, BOOL caseSensitive) {
	if (str1 && str2) {
		if (strlen(str1) == strlen(str2)) {
			if (caseSensitive) {
				return strcmp(str1, str2) == 0;
			}
			else {
				return stricmp(str1, str2) == 0;
			}
		}
	}
	return FALSE;
}

unsigned char* Utilities::GetCPUKey() {
	BYTE* CPUKey = (BYTE*)malloc(0x10);
	ZeroMemory(CPUKey, 0x10);

	*(QWORD*)(CPUKey) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x600)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x800)));
	*(QWORD*)(CPUKey + 0x08) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xA00)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xC00)));

	BYTE* Buffer = (BYTE*)malloc(0x10);
	ZeroMemory(Buffer, 0x10);

	for (DWORD i = 0x00; i < 0x10; i++)
	{
		Buffer[i] = CPUKey[i];
	}

	BYTE RandomKey[0x20] = { 0x00 };
	memcpy(RandomKey, (PBYTE)"The cow goes moo", 0x10);
	memcpy(RandomKey + 0x10, (PBYTE)"Shield", 0x06);

	for (unsigned long i = 0x00; i < 0x06; i++)
	{
		RandomKey[(0x10 + i)] ^= 0x12;
		RandomKey[(0x10 + i)] = ~RandomKey[(0x10 + i)];
	}

	XeCryptRandom(RandomKey + 0x1A, 0x06);
	XeCryptRc4(RandomKey, 0x10, RandomKey + 0x10, 0x0A);

	unsigned short HighLowKey = 0x00;
	XeCryptRandom((PBYTE)&HighLowKey, 0x02);

	for (unsigned long i = 0x00; i < 0x1A; i++)
	{
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0A);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0B);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0C);
	}

	XeCryptRc4((PBYTE)Global.hShield + 0x2E, 0x12, RandomKey, 0x20);
	XeCryptRc4((PBYTE)Global.hShield + 0x12, 0x30, RandomKey, 0x20);

	for (unsigned long i = 0x00; i < 0x20; i++)
	{
		RandomKey[i] ^= ((HighLowKey & 0xFF) + 0x0D);
		RandomKey[i] ^= ((HighLowKey & 0xFF) + 0x0E);
		RandomKey[i] ^= ((HighLowKey & 0xFF) + 0x0F);
	}

	*(QWORD*)(CPUKey) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x600)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x800)));
	*(QWORD*)(CPUKey + 0x08) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xA00)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xC00)));

	for (DWORD i = 0x00; i < (0x10 / 0x02); i++)
	{
		WORD TDI = (((Buffer[i] << 0x08) & 0xFF00) | Buffer[((0x10 - 0x01) - i)]);
		Buffer[i] = (TDI & 0xFF);
		Buffer[((0x10 - 0x01) - i)] = (((TDI & 0xFF00) >> 0x08) & 0xFF);
	}

	XeCryptRandom(RandomKey + 0x1A, 0x06);
	XeCryptRc4(RandomKey, 0x10, RandomKey + 0x10, 0x0A);
	XeCryptRandom((PBYTE)&HighLowKey, 0x02);

	for (unsigned long i = 0x00; i < 0x20; i++)
	{
		RandomKey[i] ^= ((HighLowKey & 0xFF) + 0x0D);
		RandomKey[i] ^= ((HighLowKey & 0xFF) + 0x0E);
		RandomKey[i] ^= ((HighLowKey & 0xFF) + 0x0F);
	}

	XeCryptRc4((PBYTE)Global.hShield + 0x2E, 0x12, RandomKey, 0x20);
	XeCryptRc4((PBYTE)Global.hShield + 0x12, 0x30, RandomKey, 0x20);

	for (unsigned long i = 0x00; i < 0x06; i++)
	{
		RandomKey[(0x10 + i)] ^= 0x12;
		RandomKey[(0x10 + i)] = ~RandomKey[(0x10 + i)];
	}

	for (unsigned long i = 0x00; i < 0x1A; i++)
	{
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0A);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0B);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0C);
	}

	BYTE* RomKey = (BYTE*)malloc(0x10);
	ZeroMemory(RomKey, 0x10);
	hypervisor::PeekBytes(0x80000102000163E0, RomKey, 0x10);

	for (unsigned long i = 0x00; i < 0x20; i++)
	{
		RandomKey[i] ^= (RomKey[1] + 0x0D);
		RandomKey[i] ^= (RomKey[1] + 0x0E);
		RandomKey[i] ^= (RomKey[1] + 0x0F);
	}

	*(QWORD*)(CPUKey) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x600)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x800)));
	*(QWORD*)(CPUKey + 0x08) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xA00)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xC00)));

	for (DWORD i = 0x00; i < 0x10; i++)
	{
		RomKey[i] ^= ((((XboxHardwareInfo->Flags & 0xFF000000) >> 0x18) & 0xFF) * i);
		RandomKey[i] += i;
	}

	BYTE* BootloaderKey = (BYTE*)malloc(0x10);
	ZeroMemory(BootloaderKey, 0x10);
	hypervisor::PeekBytes(0x80000102000163F0, BootloaderKey, 0x10);

	*(WORD*)(RandomKey + 0x08) = *(WORD*)(BootloaderKey);
	*(QWORD*)(CPUKey) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x600)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x800)));
	*(QWORD*)(CPUKey + 0x08) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xA00)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xC00)));

	for (int i = 0x00; i < 0x10; i++)
	{
		BootloaderKey[i] ^= ((((XboxHardwareInfo->Flags & 0xFF000000) >> 0x18) & 0xFF) * i);
	}

	for (unsigned long i = 0x00; i < 0x1A; i++)
	{
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0A);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0B);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0C);
	}

	BYTE* RomBLKey = (BYTE*)malloc(0x10);
	ZeroMemory(RomBLKey, 0x10);

	for (DWORD i = 0x00; i < 0x07; i++)
	{
		BYTE KeyHalves = (((BootloaderKey[i] & 0xF0) | (RomKey[i] & 0x0F)));
		RomBLKey[i] = KeyHalves;
	}

	for (unsigned long i = 0x00; i < 0x1A; i++)
	{
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0A);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0B);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0C);
	}

	*(QWORD*)(CPUKey) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x600)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x800)));
	*(QWORD*)(CPUKey + 0x08) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xA00)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xC00)));

	for (DWORD i = 0x00; i < 0x10; i++)
	{
		Buffer[i] ^= RomBLKey[i];
	}

	for (unsigned long i = 0x00; i < 0x1A; i++)
	{
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0A);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0B);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0C);
	}

	*(QWORD*)(CPUKey) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x600)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x800)));
	*(QWORD*)(CPUKey + 0x08) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xA00)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xC00)));

	for (unsigned long i = 0x00; i < 0x1A; i++)
	{
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0A);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0B);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0C);
	}

	ZeroMemory(RomBLKey, 0x10);
	free(RomBLKey);

	XeCryptRc4((BYTE*)"Oh hey, what's up Paul?", 0x17, Buffer, 0x10);

	BYTE Funny[0x10] = { 0x54, 0x3B, 0xCA, 0xD8, 0xB7, 0xFD, 0xB3, 0xBE, 0xFC, 0x1A, 0xFE, 0xE7, 0x75, 0xAE, 0x73, 0x73 };
	XeCryptRc4((BYTE*)"Must be a new source!", 0x15, Funny, 0x10);

	*(QWORD*)(CPUKey) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x600)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x800)));
	*(QWORD*)(CPUKey + 0x08) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xA00)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xC00)));

	for (DWORD i = 0x00; i < 0x10; i++)
	{
		Buffer[i] ^= (Funny[i]);
		Buffer[i] ^= (BootloaderKey[i]);
		Buffer[i] -= i;
	}

	ZeroMemory(BootloaderKey, 0x10);
	ZeroMemory(RomKey, 0x10);
	free(BootloaderKey);
	free(RomKey);

	for (unsigned long i = 0x00; i < 0x1A; i++)
	{
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0A);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0B);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0C);
	}

	XeCryptRc4((BYTE*)"Must be a new source!", 0x15, Funny, 0x10);
	XeCryptSha(Buffer, 0x10, NULL, NULL, NULL, NULL, (BYTE*)0x8E03AA30, 0x10);
	ZeroMemory(Buffer, 0x10);
	free(Buffer);

	*(QWORD*)(CPUKey) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x600)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0x800)));
	*(QWORD*)(CPUKey + 0x08) = (ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xA00)) | ExpansionCall(0x53533230, PEEK_QWORD, (Kink | 0xC00)));

	for (unsigned long i = 0x00; i < 0x1A; i++)
	{
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0A);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0B);
		RandomKey[i] ^= (((HighLowKey & 0xFF00) >> 0x08) + 0x0C);
	}

	return CPUKey;
}

bool Utilities::CheckIfCallIsFromOurEngine(DWORD linkRegister) {
	PLDR_DATA_TABLE_ENTRY pldrEntry = (PLDR_DATA_TABLE_ENTRY)Global.hME;
	if (((linkRegister >= (DWORD)pldrEntry->NtHeadersBase) && (linkRegister <= ((DWORD)pldrEntry->NtHeadersBase + pldrEntry->SizeOfNtImage))))
		return true;
	else
		return false;
}

bool Utilities::ReadBoolFromDWORD(DWORD dwValue) {
	return (dwValue == 0xAAAAAAAA);
}