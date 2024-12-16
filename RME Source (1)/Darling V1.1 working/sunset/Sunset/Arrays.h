#pragma once

struct mapNames_s {
	const char* internalName;
	const char* displayName;
};

struct gameTypes_s {
	const char* internalName;
	const char* displayName;
};

extern CHAR pModdedGamertags[10][100];
extern CHAR pModdedGTs[10][100];
extern CHAR pRME_AllClient_Presets[10][100];

extern char* Array_AimbotType[0x2];
extern char* Array_TargetType[0x2];
extern char* Array_TargetBone[0x10];

extern char* DrawingTypes[0x4];

extern char* RME_Toggle[0x2];
extern char* RME_Teams[0x2];
extern char* RME_SetModes[0x4];

extern char* RME_GiveWeapons[39];
extern BYTE RME_weaponIndexes[39];

extern char* RME_GiveWeapons_ZM[27];
extern BYTE RME_weaponIndexes_ZM[27];

extern char* RME_AntiQuit[2];

extern char* espBoneNames[14];
extern char* bonesESP[26];

extern char* modelList_BO2[4];
extern short modelListIndex_BO2[4];

#if defined (BO2)
extern mapNames_s mapNames[15];
#elif defined (BO1)
extern mapNames_s mapNames[14];
extern gameTypes_s gameTypes[8];
#endif