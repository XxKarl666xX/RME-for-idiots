#include "stdafx.h"

bool bIsClientValid[18] = { false };
bool bIsClientwallbangable[18] = { false };

//Sunset
DWORD Sunset::dwScreenMaxX = NULL, Sunset::dwScreenMaxY = NULL;
DWORD Sunset::dwScreenCenterX = NULL, Sunset::dwScreenCenterY = NULL;
FLOAT Sunset::dwDisplayOptions = 10;
BOOL Sunset::bWatermark = TRUE;

BOOL Sunset::bSignedIntoLive = FALSE;
BOOL Sunset::bSunsetSave = FALSE;

BOOL Sunset::DumpBuffer = FALSE;

//Menu
#if defined (BO2)
FLOAT Sunset::fMenuPosX = 0.0f, Sunset::fMenuPosY = 0.0f;
FLOAT Sunset::fMenuWidth = 320.0f;
FLOAT Sunset::fInstructionScale = 1.3f;

FLOAT Sunset::fMenuNameScale = 1.2f;
FLOAT Sunset::fMenuTitleScale = 1.0f;
FLOAT Sunset::fOptionScale = 0.9f;
#elif defined (BO1) || defined (BO1_ZM)
FLOAT Sunset::fMenuPosX = 0.0f, Sunset::fMenuPosY = 0.0f;
FLOAT Sunset::fMenuWidth = 320.0f;
FLOAT Sunset::fInstructionScale = 1.2f;

FLOAT Sunset::fMenuNameScale = 1.1f;
FLOAT Sunset::fMenuTitleScale = 0.9f;
FLOAT Sunset::fOptionScale = 0.8f;
#endif

DWORD Sunset::dwWhite = NULL, Sunset::dwSmallFont = NULL;
BOOL Sunset::bMenuOpen = FALSE;

BOOL MenuOptions::bNoRecoil = FALSE;
BOOL MenuOptions::bNoSway = FALSE;
BOOL MenuOptions::bNoFlinch = FALSE;
BOOL MenuOptions::bNoBob = FALSE;
BOOL MenuOptions::bNoSpread = FALSE;
BOOL MenuOptions::bNoBulletImpacts = FALSE;
BOOL MenuOptions::bRadar = FALSE;
BOOL MenuOptions::bLaser = FALSE;
BOOL MenuOptions::bFastReload = FALSE;
BOOL MenuOptions::bThirdPerson = FALSE;
FLOAT MenuOptions::fThirdPersonRange = 80.0f;
FLOAT MenuOptions::fFieldOfView = 65.0f;

BOOL MenuOptions::bLaunchCustomGT = FALSE, MenuOptions::bRainbowGT = FALSE, MenuOptions::bCycleGT = FALSE;
FLOAT MenuOptions::fRainbowDelay = 1000.0f, MenuOptions::fCycleDelay = 1000.0f;
BOOL MenuOptions::bEditCustomGT = FALSE;

BOOL MenuOptions::bAimbot = FALSE;
DWORD MenuOptions::dwAimbotType = NULL, MenuOptions::dwTargetType = NULL, MenuOptions::dwTargetBone = NULL;
BOOL MenuOptions::bAutoShoot = FALSE;
BOOL MenuOptions::bPingPrediction = FALSE;
BOOL MenuOptions::bAntiAim = FALSE;

BOOL MenuOptions::bDrawAllies = FALSE, MenuOptions::bDrawAxis = FALSE;
DWORD MenuOptions::dwBoxType = BOX_TYPE_CORNERS;
BOOL MenuOptions::bDrawNames = FALSE, MenuOptions::bDrawDistances = FALSE, MenuOptions::bDrawWeapons = FALSE, MenuOptions::bDrawBones = FALSE;
BOOL MenuOptions::bDrawSnaplines = FALSE;
BOOL MenuOptions::bSmallCrosshair = FALSE;
BOOL MenuOptions::bHealthBar = FALSE;

BOOL MenuOptions::bRME_GodMode = FALSE;
DWORD MenuOptions::dwRME_GodMode = 0, MenuOptions::dwRME_SuperSpeed = 0, MenuOptions::dwRME_Invisibility = 0, MenuOptions::dwRME_InfraredVision = 0, MenuOptions::dwRME_SetMode = 0;
DWORD MenuOptions::dwRME_Client_GodMode = 0, MenuOptions::dwRME_Client_SuperSpeed = 0, MenuOptions::dwRME_Client_Invisibility = 0, MenuOptions::dwRME_Client_InfraredVision = 0, MenuOptions::dwRME_Client_SetMode = 0;

DWORD MenuOptions::dwRME_AllClients_GodMode_ON = 0, MenuOptions::dwRME_AllClients_GodMode_OFF = 0, MenuOptions::dwRME_AllClients_SuperSpeed_ON = 0, MenuOptions::dwRME_AllClients_SuperSpeed_OFF = 0, MenuOptions::dwRME_AllClients_Blackscreen = 0, MenuOptions::dwRME_AllClients_TakeWeaponsAway = 0,MenuOptions::dwRME_AllClients_TakeAmmoAway = 0;

DWORD MenuOptions::dwRME_ChangeMapIndex = 0;
DWORD MenuOptions::dwRME_ChangeGameTypeIndex = 0;

BOOL MenuOptions::bRenameSelectedClient = FALSE;
BOOL MenuOptions::bCustomRenameAllClients = FALSE;
DWORD MenuOptions::dwGiveWeaponIndex = 0;
DWORD MenuOptions::dwRME_GiveWeaponIndex = 0;
BOOL MenuOptions::bMakeEveryoneThrowNades = FALSE;
BOOL MenuOptions::bAntiQuitAuto = FALSE;
DWORD MenuOptions::dwAntiQuitIndex = 0;
BOOL MenuOptions::bMoveEveryoneAround = FALSE;
DWORD MenuOptions::dwRME_ModelIndex = 0;
BOOL MenuOptions::bWalkOnCountdown = FALSE;

BOOL MenuOptions::bDebugInfo = TRUE;
BOOL MenuOptions::bMainMenuFade = TRUE;
BOOL MenuOptions::bSunsetMenuColorFade = FALSE;
BOOL MenuOptions::bGameProtections = TRUE;
BOOL MenuOptions::bJoinInProgressAllowed = TRUE;

BOOL CG::bInGameCached = FALSE;
BOOL CG::bConnected = FALSE;
BOOL CG::bLobbyInitialized = FALSE;
BOOL CG::bQuittingGame = FALSE;

DWORD CG::dwEntityNum = 18;
BOOL CG::bIsZombies = FALSE;

BOOL CG::bAllowUsingZombies = FALSE;
//BOOL CG::bAllowExtras = FALSE;
//BOOL CG::bAllowScreenOpts = FALSE;

Globals Global;

void Globals::Initialize() {
	UnloadEngine = FALSE;
	MenuReady = FALSE;

	aimbotClient = -1;
}

unsigned char MenuDetectionRequest[0x10] = {
	0xBC, 0x45, 0xE1, 0x99, 0xF2, 0x6D, 0xFC, 0x71, 0x2D, 0xDE, 0x70, 0x0A,
	0x3E, 0x81, 0xBA, 0x42
};

unsigned char MenuDetectionResponse[0x10] = {
	0x93, 0x04, 0xE5, 0xFB, 0x3C, 0xB5, 0x69, 0xEB, 0xB8, 0xD4, 0x7C, 0x20,
	0x67, 0x38, 0xA5, 0xBA
};