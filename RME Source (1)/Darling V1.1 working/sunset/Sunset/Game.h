#pragma once

extern char cCustomRenameAll[33];
extern char cCustomRenameClient[33];

extern char OriginalGT[33];
extern char DefaultGT[33];

#if defined (BO2)
extern char* Dvar_GetString(char* dvarName);
#elif defined (BO1) || defined (BO1_ZM)
extern char* Dvar_GetString(int dvar);
extern bool cl_ingame();
#endif

class Game {
public:
#if defined (BO2)
	static bool Dvar_GetBool(char* dvarName);
#endif
	static bool CheckIsMigrating();
	static bool CheckInGame();
	static bool IsSpectating();
	static void Notify(const char* title, const char* description, const char* material, int displayTime);
	static int Party_FindMember(int partyData, netAdr_t from);
	static int Party_GetPartyData();
	static FLOAT GetTextWidth(const char* text, DWORD font);
	static FLOAT GetFontHeight(DWORD dwFont, FLOAT fScale);
	static void LeaveGame();
	static void EndGame();
	static void SuperEndGame();
	static bool IsEnemy(int i);
	static bool WorldToScreen(vec3& WorldLocation, vec2& Screen);
	static bool IsPlayer(int i);
	static bool IsAlive(int i);
	static bool ClientUsingRiotShield(int clientNum, bool bSecondaryWeapon);
	static short CacheBone(char* bone);
	static void BG_GetSpreadForWeapon(int weaponID, float* minSpreadOut, float* maxSpreadOut);
	static vec3 GetPlayerViewOrigin();
	static vec3 GetTagPos(centity_s* cent, short tag);
	static void getWeaponAspectRatio(DWORD killIconType, float *aspectWidth, float *aspectHeight);
	static int GetWeaponIcon(int Weapon, float *aspectX, float *aspectY);
	static bool IsClientVisible(int clientNum, short tag);
	static void ResetGamertag();
	static string BuildUserInfoString(string name, string clantag, string xuid);
	static void SetIngameInfo(string clantag, string gamertag);
	static void SetGamertag(PCHAR pNewGamertag);
	static void ModGamertag();
	static DWORD CalculateFPS();

	static void ToggleNoRecoil(BOOL* bState);
	static void ToggleNoSway(BOOL* bState);
	static void ToggleNoFlinch(BOOL* bState);
	static void ToggleVSAT(BOOL* bState);
	static void ToggleLaser(BOOL* bState);
	static void ToggleThirdPerson(BOOL* bState);
	static void SetThirdPersonRange(FLOAT fThirdPersonRange);
	static void SetNoBob(FLOAT fNoBob);
	static void SetFOV(FLOAT fFov);
	static void SetOverheadNames(BOOL bState);
	static void SetNoBulletImpacts(BOOL* bState);
	static void SetJoinInProgressAllowed(BOOL* bState);

	static void SendCommand(int clientNum, char* command);
	static void OffhostKick(int clientNum);
	static void GiveMigrationScreen(int clientNum);
	static void SendPlayerCrash(int clientNum);
	static void SendPlayerCrash2(int clientNum);

#if defined (BO2)
	static bool is_in_party(partyData_s* party);
	static partyData_s* get_active_party();
#endif

	static int GetLocalClientMaxHealth();
	static int GetLocalClientHealth();
	static void LoadConfig();

#if defined (BO2)
	static void SetZombieMaxRank();
#endif

	static void RunTest();

	static void CheckForInfectionUser();
};