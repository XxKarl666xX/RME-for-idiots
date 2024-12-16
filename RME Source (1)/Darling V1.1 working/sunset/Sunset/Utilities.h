#pragma once

class MemoryBuffer {
public:
	MemoryBuffer(DWORD Size = 0x200)
	{
		m_Buffer = 0;
		m_DataLength = 0;
		m_BufferSize = 0;
		if ((Size != 0) && (Size < UINT_MAX))
		{
			m_Buffer = (PBYTE)malloc(Size + 1);
			if (m_Buffer)
			{
				m_BufferSize = Size;
				m_Buffer[0] = 0;
			}
		}
	}
	~MemoryBuffer()
	{
		if (m_Buffer) free(m_Buffer);
		m_Buffer = 0;
		m_DataLength = 0;
		m_BufferSize = 0;
	}
	BOOL CheckSize(DWORD Size)
	{
		if (m_BufferSize >= (m_DataLength + Size)) return TRUE;
		else
		{
			DWORD NewSize = max((m_DataLength + Size), (m_BufferSize * 2));
			PBYTE NewBuffer = (PBYTE)realloc(m_Buffer, NewSize + 1);
			if (NewBuffer)
			{
				m_BufferSize = NewSize;
				m_Buffer = NewBuffer;
				return TRUE;
			}
			else return FALSE;
		}
	}
	VOID Add(CONST PVOID Buffer, DWORD Size)
	{
		if (CheckSize(Size))
		{
			memcpy(m_Buffer + m_DataLength, Buffer, Size);
			m_DataLength += Size;
			*(m_Buffer + m_DataLength) = 0;
		}
	}

	VOID Rewind() {
		m_DataLength = 0;
		m_Buffer[0] = 0;
	}

	DWORD GetSize() CONST { return m_DataLength; }
	PBYTE GetBuffer() CONST { return m_Buffer; }
private:
	PBYTE m_Buffer;
	DWORD m_DataLength;
	DWORD m_BufferSize;
};

typedef struct _LDR_DATA_TABLE_ENTRY_2 {
	LIST_ENTRY     InLoadOrderLinks;
	LIST_ENTRY     InClosureOrderLinks;
	LIST_ENTRY     InInitializationOrderLinks;
	VOID* NtHeadersBase;
	VOID* ImageBase;
	DWORD          SizeOfNtImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	DWORD          Flags;
	DWORD          SizeOfFullImage;
	VOID* EntryPoint;
	WORD           LoadCount;
	WORD           ModuleIndex;
	VOID* DllBaseOriginal;
	DWORD          CheckSum;
	DWORD          ModuleLoadFlags;
	DWORD          TimeDateStamp;
	VOID* LoadedImports;
	VOID* XexHeaderBase;
	union {
		ANSI_STRING               LoadFileName;
		struct {
			_LDR_DATA_TABLE_ENTRY* ClosureRoot;
			_LDR_DATA_TABLE_ENTRY* TraversalParent;
		} asEntry;
	};
} LDR_DATA_TABLE_ENTRY_2, * PLDR_DATA_TABLE_ENTRY_2;

typedef struct _XEX_IMPORT_TABLE_2 {
	DWORD                 TableSize;
	HV_IMAGE_IMPORT_TABLE ImportTable;
} XEX_IMPORT_TABLE_2, * PXEX_IMPORT_TABLE_2;


class Invoke {
public:
	template<typename T>
	static T Call(DWORD dwAddress) { return ((T(*)())dwAddress)(); }

	template<typename T, typename P1>
	static T Call(DWORD dwAddress, P1 p1) { return ((T(*)(P1))dwAddress)(p1); }

	template<typename T, typename P1, typename P2>
	static T Call(DWORD dwAddress, P1 p1, P2 p2) { return ((T(*)(P1, P2))dwAddress)(p1, p2); }

	template<typename T, typename P1, typename P2, typename P3>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3) { return ((T(*)(P1, P2, P3))dwAddress)(p1, p2, p3); }

	template<typename T, typename P1, typename P2, typename P3, typename P4>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4) { return ((T(*)(P1, P2, P3, P4))dwAddress)(p1, p2, p3, p4); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) { return ((T(*)(P1, P2, P3, P4, P5))dwAddress)(p1, p2, p3, p4, p5); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) { return ((T(*)(P1, P2, P3, P4, P5, P6))dwAddress)(p1, p2, p3, p4, p5, p6); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7))dwAddress)(p1, p2, p3, p4, p5, p6, p7); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7, P8))dwAddress)(p1, p2, p3, p4, p5, p6, p7, p8); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9))dwAddress)(p1, p2, p3, p4, p5, p6, p7, p8, p9); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10))dwAddress)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11))dwAddress)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12))dwAddress)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10, typename P11, typename P12, typename P13>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12, P13 p13) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13))dwAddress)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13); }
};

class Utilities {
public:
	static void* ResolveFunction(const char* moduleName, DWORD ordinal);
	static void* ResolveFunctionFromHandle(HANDLE hModule, DWORD ordinal);
	static void Notify(const char* text, XNOTIFYQUEUEUI_TYPE exnq = XNOTIFYUI_TYPE_PREFERRED_REVIEW);
	static BOOL FileExists(PCHAR FilePath);
	static BOOL CReadFile(PCHAR FilePath, MemoryBuffer& Buffer);
	static BOOL CWriteFile(PCHAR FilePath, PVOID Buffer, DWORD Size);
	static BOOL WriteFileAlt(PCHAR Path, LPCVOID Buffer, DWORD dwBytesToWrite);
	static VOID PatchInJump(DWORD* Address, void* Dest, BOOL Linked);
	static float GetDistance(vec3 c1, vec3 c2);
	static void AngleVectors(vec3 angles, vec3* forward, vec3* right, vec3* up);
	static void VectorToAngle(vec3* direction, vec3* out);
	static vec3 VectorToAngles(vec3 &Delta);
	static float FloatMinMax(float min, float max);
	static char* GetSelectedString(char** array, int index);
	static int GetSelectedIndexForString(char** array, int arraySize, const char* inputString);
	static void RemoveFromList(HANDLE hModule);
	static DWORD GetModuleImportAddr(HANDLE HModule, HANDLE HImportedModule, DWORD Ordinal);
	static BOOL Compare(PCHAR str1, PCHAR str2, BOOL caseSensitive = TRUE);
	static unsigned char* GetCPUKey();
	static bool CheckIfCallIsFromOurEngine(DWORD linkRegister);
	static bool ReadBoolFromDWORD(DWORD dwValue);
};