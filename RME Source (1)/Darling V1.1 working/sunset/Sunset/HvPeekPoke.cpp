#include "stdafx.h"


#pragma warning(push)
#pragma warning(disable:4826)

bool hypervisor::InitializePeekPoke() {
	void* Buffer = XPhysicalAlloc(0x1000, (~((unsigned long)0)), 0, 4);
	unsigned long Address = (unsigned long)((void* (*)(...))Utilities::ResolveFunction("xboxkrnl.exe", 190))(Buffer);
	memset(Buffer, 0, 0x1000);
	memcpy(Buffer, PeekPokeExp, sizeof(PeekPokeExp));
	unsigned long Result = (unsigned long)ExpansionInstall(Address, 0x1000);
	XPhysicalFree(Buffer);
	return (Result == ((long)0x00000000L));
}

unsigned char hypervisor::PeekBYTE(unsigned long long Address) {
	return (unsigned char)ExpansionCall(0x53533230, PEEK_BYTE, Address);
}

unsigned short hypervisor::PeekWORD(unsigned long long Address) {
	return (unsigned short)ExpansionCall(0x53533230, PEEK_WORD, Address);
}

unsigned long hypervisor::PeekDWORD(unsigned long long Address) {
	return (unsigned long)ExpansionCall(0x53533230, PEEK_DWORD, Address);
}

unsigned long long hypervisor::PeekQWORD(unsigned long long Address) {
	return ExpansionCall(0x53533230, PEEK_QWORD, Address);
}

unsigned long hypervisor::PeekBytes(unsigned long long Address, void* Buffer, unsigned long Size) {
	void* pBuffer = XPhysicalAlloc(Size, (~((unsigned long)0)), 0, 4);
	memset(pBuffer, 0, Size);
	unsigned long Result = (unsigned long)ExpansionCall(0x53533230, PEEK_BYTES, Address, (unsigned long long)((void* (*)(...))Utilities::ResolveFunction("xboxkrnl.exe", 190))(pBuffer), Size);
	if (Result == ((long)0x00000000L)) memcpy(Buffer, pBuffer, Size);
	XPhysicalFree(pBuffer);
	return Result;
}

unsigned long hypervisor::PokeBYTE(unsigned long long Address, unsigned char Value) {
	return (unsigned long)ExpansionCall(0x53533230, POKE_BYTE, Address, Value);
}

unsigned long hypervisor::PokeWORD(unsigned long long Address, unsigned short Value) {
	return (unsigned long)ExpansionCall(0x53533230, POKE_WORD, Address, Value);
}

unsigned long hypervisor::PokeDWORD(unsigned long long Address, unsigned long Value) {
	return (unsigned long)ExpansionCall(0x53533230, POKE_DWORD, Address, Value);
}

unsigned long hypervisor::PokeQWORD(unsigned long long Address, unsigned long long Value) {
	return (unsigned long)ExpansionCall(0x53533230, POKE_QWORD, Address, Value);
}

unsigned long hypervisor::PokeBytes(unsigned long long Address, const void* Buffer, unsigned long Size) {
	void* pBuffer = XPhysicalAlloc(Size, (~((unsigned long)0)), 0, 4);
	memcpy(pBuffer, Buffer, Size);
	unsigned long Result = (unsigned long)ExpansionCall(0x53533230, POKE_BYTES, Address, (unsigned long long)((void* (*)(...))Utilities::ResolveFunction("xboxkrnl.exe", 190))(pBuffer), Size);
	XPhysicalFree(pBuffer);
	return Result;
}