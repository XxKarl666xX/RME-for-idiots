#pragma once

#if defined (DEBUG_BUILD)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DbgPrint(x, ...) { printf("[%s::%s] -> "  x  "\n",  __FILENAME__, __FUNCTION__, __VA_ARGS__); }
#else
#define DbgPrint(x, ...)
#endif

#define ENGINE_NAME "Darling V1 ^1^F(Extended)"

#if defined (BO2)
#define ENGINE_VERSION "v1"
#elif defined (BO1)
#define ENGINE_VERSION "v1"
#elif defined (BO1_ZM)
#define ENGINE_VERSION "v1"
#endif

extern unsigned char MenuDetectionRequest[0x10];
extern unsigned char MenuDetectionResponse[0x10];

extern bool bIsClientValid[18];
extern bool bIsClientwallbangable[18];

namespace Sunset {
	extern DWORD dwScreenMaxX, dwScreenMaxY;
	extern DWORD dwScreenCenterX, dwScreenCenterY;
	extern FLOAT dwDisplayOptions;
	extern BOOL bWatermark;

	extern BOOL bSignedIntoLive;
	extern BOOL bSunsetSave;

	extern BOOL DumpBuffer;

	extern FLOAT fMenuPosX, fMenuPosY;
	extern FLOAT fMenuWidth;
	extern FLOAT fInstructionScale;

	extern FLOAT fMenuNameScale;
	extern FLOAT fMenuTitleScale;
	extern FLOAT fOptionScale;

	extern DWORD dwWhite, dwSmallFont;
	extern BOOL bMenuOpen;
}

namespace MenuOptions {
	extern BOOL bNoRecoil, bNoSway, bNoFlinch, bNoBob, bNoSpread, bNoBulletImpacts, bRadar, bLaser, bFastReload, bThirdPerson;
	extern FLOAT fThirdPersonRange, fFieldOfView;

	extern BOOL bLaunchCustomGT, bRainbowGT, bCycleGT;
	extern FLOAT fRainbowDelay, fCycleDelay;
	extern BOOL bEditCustomGT;

 	extern BOOL bAimbot;
	extern DWORD dwAimbotType, dwTargetType, dwTargetBone;
	extern BOOL bAutoShoot;
	extern BOOL bPingPrediction;
	extern BOOL bAntiAim;

	extern BOOL bDrawAllies, bDrawAxis;
	extern DWORD dwBoxType;
	extern BOOL bDrawNames, bDrawDistances, bDrawWeapons, bDrawBones;
	extern BOOL bDrawSnaplines;
	extern BOOL bSmallCrosshair;
	extern BOOL bHealthBar;

	extern BOOL bRME_GodMode;
	extern DWORD dwRME_GodMode, dwRME_SuperSpeed, dwRME_Invisibility, dwRME_InfraredVision, dwRME_SetMode;
	extern DWORD dwRME_Client_GodMode, dwRME_Client_SuperSpeed, dwRME_Client_Invisibility, dwRME_Client_InfraredVision, dwRME_Client_SetMode;

	extern DWORD dwRME_AllClients_GodMode_ON, dwRME_AllClients_GodMode_OFF, dwRME_AllClients_SuperSpeed_ON, dwRME_AllClients_SuperSpeed_OFF, dwRME_AllClients_Blackscreen, dwRME_AllClients_TakeWeaponsAway, dwRME_AllClients_TakeAmmoAway;

	extern DWORD dwRME_ChangeMapIndex;
	extern DWORD dwRME_ChangeGameTypeIndex;

	extern BOOL bRenameSelectedClient;
	extern BOOL bCustomRenameAllClients;
	extern DWORD dwGiveWeaponIndex, dwRME_GiveWeaponIndex;
	extern BOOL bMakeEveryoneThrowNades;
	extern BOOL bAntiQuitAuto;
	extern DWORD dwAntiQuitIndex;
	extern BOOL bMoveEveryoneAround;
	extern DWORD dwRME_ModelIndex;
	extern BOOL bWalkOnCountdown;

	extern BOOL bDebugInfo;
	extern BOOL bMainMenuFade;
	extern BOOL bSunsetMenuColorFade;
	extern BOOL bGameProtections;
	extern BOOL bJoinInProgressAllowed;
}

namespace CG {
	extern BOOL bInGameCached;
	extern BOOL bConnected;
	extern BOOL bLobbyInitialized;
	extern BOOL bQuittingGame;

	extern DWORD dwEntityNum;
	extern BOOL bIsZombies;

	extern BOOL bAllowUsingZombies;

	//extern BOOL bAllowExtras;
	//extern BOOL bAllowScreenOpts;
}

struct SunsetInfo_s {
	DWORD dwAllowZombies; //0x00
	DWORD dwHasExtras; //0x04
	DWORD dwHasScreenOpts; //0x08
	DWORD dwHasMapOptions; //0x0C
	DWORD dwHasKickBanOptions; //0x10
	DWORD dwHasFreezeMigrate; //0x14
	DWORD dwHasFastRestart; //0x18
	DWORD dwHasChangeMap; //0x1C
	DWORD dwHasResetStats; //0x20
	DWORD dwHasHearEveryone; //0x24
	DWORD dwHasKillPlayer; //0x28
	DWORD dwHasFreezeMovement; //0x2C
	DWORD dwWalkOnCountdown; //0x30
	DWORD dwHasUnbindControls; //0x34
	CHAR _0x38[0x8];

}; //0x40

class Globals {
public:
	XINPUT_STATE state;

	BOOL g_killThread;
	BOOL g_Hooked;
	BOOL g_MainLoopExited;

	BOOL UnloadEngine;
	BOOL MenuReady;

	HANDLE hME;
	HANDLE hKernel;
	HANDLE hXam;
	HANDLE hXBDM;
	HANDLE hShield;

	PLDR_DATA_TABLE_ENTRY EnginePDLR;

	SunsetInfo_s sunsetInfo;

	void Initialize();
};

extern Globals Global;