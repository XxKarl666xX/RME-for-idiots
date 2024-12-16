#include "stdafx.h"
#include <string>

char cCustomRenameAll[33];
char cCustomRenameClient[33];
char OriginalGT[33];
char DefaultGT[33];

int riotShieldIndex = NULL;

#if defined (BO2)
partyData_s* partyData, * lobbyData = nullptr;
#endif

#if defined (BO2)
char* Dvar_GetString(char* dvarName) {
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return false;
	return (char*)(*(int*)(dvar_t + 0x18));
}

bool Game::Dvar_GetBool(char* dvarName) {
	int dvar_t = Dvar_FindVar(dvarName);
	if (dvar_t == 0)
		return false;
	return *(bool*)(dvar_t + 0x18);
}
#elif defined (BO1) || defined (BO1_ZM)
char* Dvar_GetString(int dvar) {
	return *(char**)(*(int*)(dvar)+0x18);
}

bool cl_ingame() {
	return *(bool*)(*(int*)(decryptDWORD(Security->addrs.cl_ingame)) + 0x18);
}
#endif

bool Game::CheckIsMigrating() {
	return *(int*)(decryptDWORD(Security->addrs.strct_clientUIActive) + 0x10) != 0;
}

bool Game::CheckInGame() {
	// Make sure we are connected to a game session
	CG::bConnected = !CL_AllLocalClientsDisconnected() ? TRUE : FALSE;

	// Check the cl_ingame dvar
#if defined (BO2)
	bool bInGame = Dvar_GetBool("cl_ingame");
#elif defined (BO1) || defined (BO1_ZM)
	bool bInGame = cl_ingame();
#endif
	if (!bInGame) {
		if (CG::bQuittingGame) {
			CG::bQuittingGame = FALSE;
		}
	}

	// If we're in game and all local clients aren't disconnected, we're fully ingame.
	return bInGame && CG::bConnected && !CheckIsMigrating() && !CG::bQuittingGame;
}

bool Game::IsSpectating() {
	if (Structs::get_cg()->playerState.otherFlags & 0x1A)
		return true;

	return false;
}

void Game::Notify(const char* title, const char* description, const char* material, int displayTime) { //UI_OpenToastPopup
	((void(*)(...))0x82454800)(0, material, title, description, displayTime);
}

int Game::Party_FindMember(int partyData, netAdr_t from) {
#if defined (BO2)
	return ((int(*)(...))0x8228EA70)(partyData, from);
#elif defined (BO1)
	return ((int(*)(...))0x822596A8)(partyData, from);
#endif
}

int Game::Party_GetPartyData() {
#if defined (BO2)
	return ((int(*)(...))0x8228DC88)();
#elif defined (BO1)
	//return ((int(*)(...))0x8228DC88)();
	return *(int*)0x829F7038;
#endif
}

FLOAT Game::GetTextWidth(const char* text, DWORD font) {
#if defined (BO2)
	return (FLOAT)((int(*)(...))decryptDWORD(Security->addrs.R_TextWidth))(0, text, 0x7FFFFFFF, font);
#elif defined (BO1) || defined (BO1_ZM)
	return (FLOAT)((int(*)(...))decryptDWORD(Security->addrs.R_TextWidth))(text, 0x7FFFFFFF, font);
#endif
	return 0.0f;
}

FLOAT Game::GetFontHeight(DWORD dwFont, FLOAT fScale) {
	if (dwFont) {
		return (FLOAT)((int(*)(...))decryptDWORD(Security->addrs.R_TextHeight))(dwFont) * fScale;
	}
	return 25.0f * fScale;
}

void Game::LeaveGame() {
	Cbuf_AddText(0, "disconnect;");
}

void Game::EndGame() {
	Cbuf_AddText(0, va("cmd mr %i -1 endround;", *(int*)(decryptDWORD(Security->addrs.ServerID))));
}

void Game::SuperEndGame() {
	Cbuf_AddText(0, va("cmd sl %i 3 _SUNSETCRASH;", *(int*)(decryptDWORD(Security->addrs.ServerID))));
}

bool Game::IsEnemy(int i) {
#if defined (BO2)
	if (!CG::bIsZombies) {
		//if (!strcmp(Dvar_GetString("ui_gametype"), "dm"))
		//	return true;

		//return Structs::get_cg()->clientInfo[i].Team != Structs::get_cg()->clientInfo[Structs::get_cg()->clientNumber].Team;

		return (!CG_IsEntityFriendlyNotEnemy(0, &Structs::get_centity(i)));
	}
	else {
		return (!CG_IsEntityFriendlyNotEnemy(0, &Structs::get_centity(i)));
	}


#elif defined (BO1)
	if (!strcmp(Dvar_GetString(decryptDWORD(Security->addrs.ui_gametype)), "dm"))
		return true;

	return Structs::get_cg()->clientInfo[i].Team != Structs::get_cg()->clientInfo[Structs::get_cg()->clientNumber].Team;
#elif defined (BO1_ZM)
	if (Structs::get_centity(i).eType == 16)
		return true;

	if (Structs::get_centity(i).eType == 1)
		return false;

#endif
	return false;
}

bool Game::WorldToScreen(vec3& WorldLocation, vec2& Screen) {
	vec3 Local = WorldLocation - Structs::get_cg()->refDef.viewOrigin, Transform;
	Transform.x = Local.dot(Structs::get_cg()->refDef.viewAxis[1]);
	Transform.y = Local.dot(Structs::get_cg()->refDef.viewAxis[2]);
	Transform.z = Local.dot(Structs::get_cg()->refDef.viewAxis[0]);

	if (Transform.z < 0.01)
	{
		return false;
	}

	Screen.x = (Structs::get_cg()->refDef.Width / 2) * (1 - (Transform.x / Structs::get_cg()->refDef.tanHalfFov.x / Transform.z));
	Screen.y = (Structs::get_cg()->refDef.Height / 2) * (1 - (Transform.y / Structs::get_cg()->refDef.tanHalfFov.y / Transform.z));

	return (Transform.z > 0);
}

bool Game::IsPlayer(int i) {
#if defined (BO2)
	//if (!CG::bIsZombies) {
	//	if (Structs::get_centity(i).eType != 1) {
	//		return false;
	//	}
	//}
	//else {
	//	if (Structs::get_centity(i).eType != 16) {
	//		return false;
	//	}
	//}

	if (!CG::bIsZombies) {
		if (Structs::get_centity(i).eType != 1)
			return false;
	}
	else {
		if (Structs::get_centity(i).eType != 1 && Structs::get_centity(i).eType != 16)
			return false;
	}

#elif defined (BO1)
	if (Structs::get_centity(i).GetType() != 1)
		return false;

#elif defined (BO1_ZM)
	//if (centity[i].eType != 1)
	//	return false;

	if (Structs::get_centity(i).eType != 1 && Structs::get_centity(i).eType != 16)
		return false;
#endif

	return true;
}

bool Game::IsAlive(int i) {
#if defined (BO2)
	if ((*(int*)((DWORD)&Structs::get_centity(i) + 0x36C) & 0x40000000) == 0 || (*(int*)((DWORD)&Structs::get_centity(i) + 0x1D4) & 0x40000))
		return false;
#elif defined (BO1)
	if ((Structs::get_centity(i)._0x0318 & 0x40000000) == 0 || (Structs::get_centity(i)._0x01E0 & 0x40000))
		return false;
#elif defined (BO1_ZM)
	if ((Structs::get_centity(i)._0x304 & 0x40000000) == 0)
		return false;
#endif

	return true;
}

bool Game::ClientUsingRiotShield(int clientNum, bool bSecondaryWeapon) {
#if defined (BO2)
	if (!CG::bIsZombies) {
	jRetry:
		if (riotShieldIndex == NULL) {
			riotShieldIndex = CG_GetWeaponIndexForName("riotshield_mp");
			goto jRetry;
		}
		else {
			if (!bSecondaryWeapon) {
				if (Structs::get_centity(clientNum).weaponNum == riotShieldIndex) {
					return true;
				}
			}
			else {
				if (Structs::get_centity(clientNum).WeaponID2 == riotShieldIndex) {
					return true;
				}
			}
		}
	}

	return false;
#endif
	return false;
}

short Game::CacheBone(char* bone) {
#if defined (BO2)
	return ((short(*)(...))decryptDWORD(Security->addrs.SL_GetString))(bone, 0);
#elif defined (BO1) || defined (BO1_ZM)
	return ((short(*)(...))decryptDWORD(Security->addrs.SL_GetString))(bone, 0, 0);
#endif
}

DWORD Get_UnknownClientStruct() {
	DWORD UnknownClientStruct = NULL;
#if defined (BO1)
	UnknownClientStruct = 0x82969190;
#elif defined (BO1_ZM)
	UnknownClientStruct = 0x836DE110;
#endif
	if (UnknownClientStruct) {
		return *(DWORD*)UnknownClientStruct;
	}
	return 0;
}

void Game::BG_GetSpreadForWeapon(int weaponID, float* minSpreadOut, float* maxSpreadOut) {
#if defined (BO2)
	((void(*)(...))decryptDWORD(Security->addrs.BG_GetSpreadForWeapon))(&Structs::get_cg()->playerState, weaponID, minSpreadOut, maxSpreadOut);
#elif defined (BO1)
	((void(*)(...))decryptDWORD(Security->addrs.BG_GetSpreadForWeapon))(Structs::get_cg()->GetPlayerState(), BG_GetWeaponDef(weaponID), minSpreadOut, maxSpreadOut);
#elif defined (BO1_ZM)
	//DWORD cg_t = Get_UnknownClientStruct();
	//if (cg_t) {
	//	((void(*)(...))decryptDWORD(Security->addrs.BG_GetSpreadForWeapon))(Structs::get_cg()->GetPlayerState(), BG_GetWeaponDef(weaponID), minSpreadOut, maxSpreadOut);
	//}
	((void(*)(...))decryptDWORD(Security->addrs.BG_GetSpreadForWeapon))(&Structs::get_cg()->playerState, BG_GetWeaponDef(weaponID), minSpreadOut, maxSpreadOut);
#endif
}

vec3 Game::GetPlayerViewOrigin() {
	vec3 vOut;
#if defined (BO2)
	((VOID(*)(...))decryptDWORD(Security->addrs.CG_GetPlayerViewOrigin))(0, &Structs::get_cg()->playerState, &vOut);
#elif defined (BO1)
	((VOID(*)(...))decryptDWORD(Security->addrs.CG_GetPlayerViewOrigin))(0, Structs::get_cg()->GetPlayerState(), &vOut);
#elif defined (BO1_ZM)
	DWORD cg_t = Get_UnknownClientStruct();
	if (cg_t) {
		((VOID(*)(...))decryptDWORD(Security->addrs.CG_GetPlayerViewOrigin))(0, cg_t, &vOut);
	}
#endif
	return vOut;
}

vec3 Game::GetTagPos(centity_s* cent, short tag) {
	vec3 tagPos = vec3();
	int dobj = Com_GetClientDObj(cent->ClientNumber, 0);

	if (dobj == 0)
		return cent->Origin;

	if (CG_DObjGetWorldTagPos(cent, dobj, tag, tagPos)) {
		return tagPos;
	}

	return vec3();
}

void Game::getWeaponAspectRatio(DWORD killIconType, float *aspectWidth, float *aspectHeight) {
	float width = 1.4f, height = 1.4f;
	if (killIconType == 0) //1:1
	{
		width = 1.4f;
		height = 1.4f;
	}
	else if (killIconType == 1) //2:1
	{
		width = 2.8f;
		height = 1.4f;
	}
	else if (killIconType == 2) //3:1
	{
		width = 2.8f;
		height = 0.7f;
	}
	width *= 20.0f;
	height *= 20.0f;
	*aspectWidth = width;
	*aspectHeight = height;
}

int Game::GetWeaponIcon(int Weapon, float *aspectX, float *aspectY) {
#if defined (BO2)
	int weaponDef = BG_GetWeaponDef(Weapon);
	if (weaponDef) {
		DWORD weapDef2 = ((DWORD(*)(...))decryptDWORD(Security->addrs.BG_GetWeaponDef2))(Weapon);
		if (weapDef2) {
			DWORD killIconType = *(DWORD*)(*(DWORD*)(weapDef2 + 0x8) + 0x664);

			getWeaponAspectRatio(killIconType, aspectX, aspectY);
			return *(int*)(weaponDef + 0x3AC);
		}
	}
#elif defined (BO1)
	int weaponDef = BG_GetWeaponDef(Weapon);
	if (weaponDef) {
		DWORD killIconType = *(DWORD*)(weaponDef + 0x57C);
		getWeaponAspectRatio(killIconType, aspectX, aspectY);
		return *(int*)(weaponDef + 0x578);
	}
#endif
	return 0;
}

bool Game::IsClientVisible(int clientNum, short tag) {
	Trace trace;

	vec3 tagPos = GetTagPos(&Structs::get_centity(clientNum), tag);
	vec3 viewOrigin = Game::GetPlayerViewOrigin();

	if (!tagPos.IsNull() && !viewOrigin.IsNull()) {
		((void(*)(...))decryptDWORD(Security->addrs.CG_LocationalTrace))(&trace, &viewOrigin, &tagPos, CG::bIsZombies ? 1022 : clientNum, 0x280E833, FALSE, FALSE);
	}
	else {
		return false;
	}

#if defined (BO2)
	if (!CG::bIsZombies) {
		// Check if the trace hit a shield
		if (trace.partGroup == 0x14) {
			return false;
		}
	}
#endif

	return (trace.fFraction >= 0.95f);
}

void Game::ResetGamertag() {
	XUSER_SIGNIN_INFO userInfo; ZeroMemory(&userInfo, sizeof(XUSER_SIGNIN_INFO));
	if (XUserGetSigninInfo(0, XUSER_GET_SIGNIN_INFO_ONLINE_XUID_ONLY, &userInfo) == ERROR_SUCCESS) {
		strcpy(OriginalGT, userInfo.szUserName);
		strcpy(DefaultGT, userInfo.szUserName);

		SetIngameInfo("", DefaultGT);

		strcpy((char*)decryptDWORD(Security->addrs.GT_Addrs[0]), OriginalGT);
#if defined (BO2)
		strcpy((char*)decryptDWORD(Security->addrs.GT_Addrs[1]), OriginalGT);
#endif
	}
}

string Game::BuildUserInfoString(string name, string clantag, string xuid) {
	string userInfoString;
	userInfoString = "userinfo \"\\rate\\20000\\snaps\\20\\name\\" + name;
	if (clantag != "")
		userInfoString += "\\clanAbbrev\\" + clantag;
	userInfoString += "\\xuid\\000" + xuid + "\\natType\\1\\rank\\1\\prestige\\0\"";
	return userInfoString;
}
void Game::SetIngameInfo(string clantag, string gamertag) {
	if (CG::bInGameCached && CG::bLobbyInitialized) {
		char buff[17] = { 0 };
		string userinfo = BuildUserInfoString(gamertag, clantag, _ui64toa(partyMember[Structs::get_cg()->clientNumber].XUID, buff, 0x10));

		CL_AddReliableCommand(0, (PCHAR)userinfo.c_str());
	}
}

CHAR pColorString[4] = { 0 };
void Game::SetGamertag(PCHAR pNewGamertag) {
	if (pNewGamertag) {
		if (strlen(pNewGamertag) >= 1) {
			strcpy(OriginalGT, pNewGamertag);

			strcpy((char*)decryptDWORD(Security->addrs.GT_Addrs[0]), pNewGamertag);

#if defined (BO2)
			strcpy((char*)decryptDWORD(Security->addrs.GT_Addrs[1]), pNewGamertag);
#endif

			if (CG::bInGameCached && CG::bLobbyInitialized) {
				if (pNewGamertag[0] == '^' && pNewGamertag[1] >= '0' && pNewGamertag[1] <= '9') {
					memcpy(pColorString, pNewGamertag, 0x2);
					pNewGamertag += 2;
				}
				SetIngameInfo(pColorString, pNewGamertag);
			}
		}
	}
}

DWORD dwCurrentCycle = 0;
DWORD dwLastSwitchTime = 0, dwLastRainbowTime = 0;
int iRandomColor = 0;
int iLastColor = 0;

void Game::ModGamertag() {
	bool bSwitchTime = false;
	if (MenuOptions::bCycleGT) {
		if (dwLastSwitchTime < GetTickCount()) {
			dwLastSwitchTime = GetTickCount() + (!CG::bInGameCached ? MenuOptions::fCycleDelay : (MenuOptions::fCycleDelay < 1000 ? 1000 : MenuOptions::fCycleDelay));
			bSwitchTime = true;
		}
	}

	PCHAR pSetGT = NULL;
	if (bSwitchTime) {
		if (MenuOptions::bCycleGT) {
		retry:
			if (dwCurrentCycle >= 9)
				dwCurrentCycle = 0;
			else
				dwCurrentCycle++;

			if (!strcmp((const char*)pModdedGamertags[dwCurrentCycle], "Empty")) {
				pSetGT = NULL;
				goto retry;
			}
			else {
				pSetGT = pModdedGamertags[dwCurrentCycle];
			}
		}
	}

	CHAR newGT[100] = { 0 };
	if (MenuOptions::bRainbowGT) {
		if (bSwitchTime) {
			dwLastRainbowTime = 0;
		}

		if (dwLastRainbowTime < GetTickCount()) {
			dwLastRainbowTime = GetTickCount() + (!CG::bInGameCached ? MenuOptions::fRainbowDelay : (MenuOptions::fRainbowDelay < 1000 ? 1000 : MenuOptions::fRainbowDelay));

			PCHAR gt = NULL;
			if (!pSetGT)
				gt = OriginalGT;
			else
				gt = pSetGT;

			if (gt) {
				if (gt[0] == '^' && gt[1] >= '0' && gt[1] <= '9') {
					gt += 2;
				}

				iRandomColor++;
				if (iRandomColor > 6)
					iRandomColor = 1;

				sprintf(pColorString, "^%i", iRandomColor);
				sprintf(newGT, "%s%s", pColorString, gt);
				pSetGT = newGT;
			}
		}
	}
	else {
		pColorString[0] = 0;
	}

	if (pSetGT != NULL) {
		SetGamertag(pSetGT);
	}
}

DWORD dwLastFrameTime = GetTickCount();
DWORD dwLastFrameTimes[64] = { 0 };
DWORD dwLastFPSValue = 60;
DWORD dwLastStableValue = 60;
DWORD dwFramesSinceChange = 0;
DWORD Game::CalculateFPS() {
	DWORD dwRunningTotal = 0;
	DWORD dwFrameTime = GetTickCount();
	for (DWORD i = 0; i < 63; i++) {
		dwLastFrameTimes[i] = dwLastFrameTimes[i + 1];
		dwRunningTotal += dwLastFrameTimes[i];
	}
	dwLastFrameTimes[63] = dwFrameTime - dwLastFrameTime;
	dwRunningTotal += dwLastFrameTimes[31];
	dwLastFrameTime = dwFrameTime;

	if (dwRunningTotal != 0) {
		DWORD dwAverage = dwRunningTotal / 64;
		if (dwAverage != 0) {
			DWORD dwFPS = (FLOAT)(1000 / dwAverage);
			if (dwLastFPSValue != dwFPS) {
				if (dwLastStableValue != dwLastFPSValue) {
					dwLastStableValue = dwLastFPSValue;
					dwFramesSinceChange = 0;
				}

				if (dwFPS != dwLastStableValue) {
					dwFramesSinceChange++;
				}

				if (dwFramesSinceChange > 1) {
					if (dwLastFPSValue > dwFPS) {
						dwLastFPSValue--;
					}
					else {
						dwLastFPSValue++;
					}
					if (dwLastFPSValue > 60) {
						dwLastFPSValue = 60;
					}
				}
			}
		}
	}
	return dwLastFPSValue;
}

void Game::ToggleNoRecoil(BOOL* bState) {
	DWORD dwNoRecoil = decryptDWORD(Security->addrs.NoRecoil);
	if (dwNoRecoil) {
		if (*bState) {
			*(DWORD*)dwNoRecoil = 0x60000000;
		}
		else {
#if defined (BO2)
			* (DWORD*)dwNoRecoil = 0x48461341;
#elif defined (BO1)
			* (DWORD*)dwNoRecoil = 0x4BF67EC5;
#elif defined (BO1_ZM) 
			* (DWORD*)dwNoRecoil = 0x4BF822E5;
#endif
		}
	}
}

void Game::ToggleNoSway(BOOL* bState) {
	DWORD dwNoSway = decryptDWORD(Security->addrs.NoSway1);
	if (dwNoSway) {
		if (*bState) {
			*(DWORD*)dwNoSway = 0x60000000;
		}
		else {
#if defined (BO2)
			* (DWORD*)dwNoSway = 0x4BFFE975;
#elif defined (BO1)
			* (DWORD*)dwNoSway = 0x4BFFEB09;
#elif defined (BO1_ZM)
			* (DWORD*)dwNoSway = 0x4BFFEA89;
#endif
		}
	}
}

void Game::ToggleNoFlinch(BOOL* bState) {
	DWORD dwNoFlinch = decryptDWORD(Security->addrs.NoFlinch1), dwNoFlinch2 = decryptDWORD(Security->addrs.NoFlinch2);
	if (dwNoFlinch) {
		if (*bState) {
#if defined (BO2)
			*(DWORD*)dwNoFlinch = 0x60000000;
			*(DWORD*)dwNoFlinch2 = 0x60000000;
#elif defined (BO1) || defined (BO1_ZM)
			 *(DWORD*)dwNoFlinch = 0x60000000;
#endif
		}
		else {
#if defined (BO2)
		    *(DWORD*)dwNoFlinch = 0x4BFFF95D;
			*(DWORD*)dwNoFlinch2 = 0x4BFFAC41;
#elif defined (BO1)
			*(DWORD*)dwNoFlinch = 0x4BFFF77D;
#elif defined (BO1_ZM)
			* (DWORD*)dwNoFlinch = 0x4BFFF785;
#endif
		}
	}
}

void Game::ToggleVSAT(BOOL* bState) {
	DWORD dwVSAT = decryptDWORD(Security->addrs.Radar);
	if (dwVSAT) {
		if (*bState) {
			*(DWORD*)dwVSAT = 0x60000000;
		}
		else {
#if defined (BO2)
			*(DWORD*)dwVSAT = 0x419A0060;
#elif defined (BO1)
			*(DWORD*)dwVSAT = 0x419A0050;
#endif
		}
	}
}

void Game::ToggleLaser(BOOL* bState) {
	DWORD dwLaser = decryptDWORD(Security->addrs.Laser);
	if (dwLaser) {
		if (*bState) {
			*(DWORD*)dwLaser = 0x2B0B0001;
		}
		else {
			*(DWORD*)dwLaser = 0x2B0B0000;
		}
	}
}

void Game::ToggleThirdPerson(BOOL* bState) {
#if defined (BO2)
	DWORD dwThirdPerson = ((DWORD)Structs::get_cg() + 0x4809C);
	if (MmIsAddressValid((PVOID)dwThirdPerson)) {
		if (*bState)
			*(BYTE*)dwThirdPerson = 1;
		else
			*(BYTE*)dwThirdPerson = 0;
	}
#elif defined (BO1) || defined (BO1_ZM)
	*(byte*)(*(int*)(decryptDWORD(Security->addrs.ThirdPersonView)) + 0x18) = (byte)*bState;
#endif
}

void Game::SetThirdPersonRange(FLOAT fThirdPersonRange) {
	*(float*)(*(int*)(decryptDWORD(Security->addrs.ThirdPersonRange)) + 0x18) = fThirdPersonRange;
}

void Game::SetNoBob(FLOAT fNoBob) {
#if defined (BO2)
	* (float*)(decryptDWORD(Security->addrs.NoBob)) = fNoBob;
#elif defined (BO1) || defined (BO1_ZM)
	*(float*)(*(int*)(decryptDWORD(Security->addrs.NoBob)) + 0x18) = fNoBob;
#endif
}

void Game::SetFOV(FLOAT fFov) {
	*(float*)(*(int*)(decryptDWORD(Security->addrs.FOV)) + 0x18) = fFov;
}

void Game::SetOverheadNames(BOOL bState) {
	DWORD dwOverheadName = decryptDWORD(Security->addrs.CG_DrawOverheadNames);
	if (dwOverheadName) {
		if (bState) {
			*(DWORD*)dwOverheadName = 0x60000000;
		}
		else {
#if defined (BO2)
			* (DWORD*)dwOverheadName = 0x4BFFEC35;
#elif defined (BO1)
			* (DWORD*)dwOverheadName = 0x4BFFEF21;
#elif defined (BO1_ZM)
			* (DWORD*)dwOverheadName = 0x4BFFF4F5;
#endif
		}
	}
}

void Game::SetNoBulletImpacts(BOOL* bState) {
	DWORD dwNoBulletImpacts = decryptDWORD(Security->addrs.NoBulletImpacts);
	if (dwNoBulletImpacts) {
		if (*bState) {
			*(DWORD*)dwNoBulletImpacts = 0x60000000;
		}
		else {
			*(DWORD*)dwNoBulletImpacts = 0x4BFFF669;
		}
	}
}

void Game::SetJoinInProgressAllowed(BOOL* bState) {
	*(byte*)(*(int*)(decryptDWORD(Security->addrs.JoinInProgressAllowed)) + 0x18) = (byte)*bState;
}

void Game::SendCommand(int clientNum, char* command) {
	netAdr_t netAdr = netAdr_t();
	memset(&netAdr, 0x00, sizeof(netAdr_t));

	Live_GetPlayerNetAddr(&netAdr, Live_GetCurrentSession(), clientNum);

	NET_OutOfBandPrint(NS_SERVER, netAdr, command);
}

void Game::OffhostKick(int clientNum) {
	SendCommand(clientNum, "connectResponseMigration");
}

void Game::GiveMigrationScreen(int clientNum) {
	SendCommand(clientNum, "mstart");
}

void(*NET_SendPacketxx)(int sock, int size, char* data, netAdr_t to)
= (void(*)(int, int, char*, netAdr_t))0x8234F0F0;

void Game::SendPlayerCrash(int clientNum) {
#if defined (BO2)
	partyData_s* party = get_active_party();

	char buf[0x400];
	msg_t msg;

	MSG_Init(&msg, buf, sizeof(buf));
	MSG_WriteString(&msg, va("%ijoinParty", party->partyId));
	MSG_WriteLong(&msg, 0);
	MSG_WriteLong(&msg, 0);
	MSG_WriteLong(&msg, 0);
	MSG_WriteInt64(&msg, 0);
	MSG_WriteByte(&msg, 0);
	MSG_WriteShort(&msg, 0);
	MSG_WriteLong(&msg, 0);
	MSG_WriteByte(&msg, 1234567890);

	netAdr_t netAdr = netAdr_t();
	memset(&netAdr, 0x00, sizeof(netAdr_t));

	Live_GetPlayerNetAddr(&netAdr, Live_GetCurrentSession(), clientNum);

	NET_OutOfBandData(NS_SERVER, netAdr, msg.data, msg.cursize);
#elif defined (BO1)
	char buf[0x4EC];
	msg_t msg;

	MSG_Init(&msg, buf, sizeof(buf));
	MSG_WriteString(&msg, va("relay %i", 1234567890));

	netAdr_t netAdr = netAdr_t();
	memset(&netAdr, 0x00, sizeof(netAdr_t));

	Live_GetPlayerNetAddr(&netAdr, Live_GetCurrentSession(), clientNum);

	NET_OutOfBandData(NS_CLIENT1, netAdr, msg.data, msg.cursize);

#endif
}

void Game::SendPlayerCrash2(int clientNum) {
#if defined (BO2)
	partyData_s* party = get_active_party();

	char buf[1264];
	msg_t msg;

	MSG_Init(&msg, buf, sizeof(buf));
	MSG_WriteString(&msg, va("%ipseg", party->partyId));
	MSG_WriteByte(&msg, 0xFF);
	MSG_WriteLong(&msg, 0xFFFFFFFF);
	MSG_WriteLong(&msg, 0xFFFFFFFF);
	MSG_WriteShort(&msg, 0xFFFF);
	MSG_WriteShort(&msg, 0xFFFF);

	netAdr_t netAdr = netAdr_t();
	memset(&netAdr, 0x00, sizeof(netAdr_t));

	Live_GetPlayerNetAddr(&netAdr, Live_GetCurrentSession(), clientNum);

	NET_OutOfBandData(NS_CLIENT1, netAdr, msg.data, msg.cursize);
#endif
}

#if defined (BO2)
bool Game::is_in_party(partyData_s* party) {
	return party->party_systemActive == 1 && party->inParty;
}

partyData_s* Game::get_active_party()
{
	partyData = (partyData_s*)(decryptDWORD(Security->addrs.g_partyData));
	lobbyData = (partyData_s*)(decryptDWORD(Security->addrs.g_lobbyData));

	return is_in_party(partyData) ? partyData : lobbyData;
}
#endif

int Game::GetLocalClientMaxHealth() {
	cg_s* cg = Structs::get_cg();

	if (cg) {
#if defined (BO2)
		return *(int*)((int)cg + 0x2D0);
#elif defined (BO1)
		return *(int*)((int)cg + 0x25C);
#elif defined (BO1_ZM)
		return *(int*)((int)cg + 0x210);
#endif
	}
	return 0;
}

int Game::GetLocalClientHealth() {
	cg_s* cg = Structs::get_cg();

	if (cg) {
#if defined (BO2)
		return *(int*)((int)cg + 0x2C8);
#elif defined (BO1)
		return *(int*)((int)cg + 0x254);
#elif defined (BO1_ZM)
		return *(int*)((int)cg + 0x208);
#endif
	}
	return 0;
}

void Game::LoadConfig() {
	//Main Menu
	MenuOptions::bNoRecoil = TRUE;
	MenuOptions::bNoSway = TRUE;
	MenuOptions::bNoFlinch = TRUE;
	MenuOptions::bNoBob = TRUE;
	MenuOptions::bNoSpread = TRUE;
	MenuOptions::bNoBulletImpacts = TRUE;
	MenuOptions::bRadar = TRUE;
	MenuOptions::bLaser = TRUE;
	MenuOptions::bFastReload = TRUE;
	MenuOptions::bThirdPerson = FALSE;
	MenuOptions::fThirdPersonRange = 80.0f;
	MenuOptions::fFieldOfView = 90.0f;

	//Aimbot Menu
	MenuOptions::bAimbot = TRUE;
	MenuOptions::dwAimbotType = 1;
	MenuOptions::dwTargetType = 1;
	MenuOptions::dwTargetBone = 1;
	MenuOptions::bAutoShoot = TRUE;
	MenuOptions::bPingPrediction = TRUE;

	if (!CG::bIsZombies) {
		MenuOptions::bAntiAim = TRUE;
	}

	//Visual Menu
	MenuOptions::bDrawAllies = TRUE;
	MenuOptions::bDrawAxis = TRUE;
	MenuOptions::dwBoxType = BOX_TYPE_CORNERS;
	MenuOptions::bDrawNames = TRUE;
	MenuOptions::bDrawDistances = TRUE;
	if (!CG::bIsZombies) {
		MenuOptions::bDrawWeapons = TRUE;
	}
	MenuOptions::bDrawBones = TRUE;
	MenuOptions::bDrawSnaplines = TRUE;
	MenuOptions::bSmallCrosshair = TRUE;
	MenuOptions::bHealthBar = TRUE;
}

#if defined (BO2)
void Game::SetZombieMaxRank() {
	*(BYTE*)(decryptDWORD(Security->addrs.ZombieRank[0])) = 0x7F;
	memset((PVOID)decryptDWORD(Security->addrs.ZombieRank[1]), 0xFF, 0x1F);
	memset((PVOID)decryptDWORD(Security->addrs.ZombieRank[2]), 0xFF, 0x33);
}
#endif

struct Material {
	const char* pName; //0x00
};

struct GfxCmdHeader
{
	unsigned __int16 byteCount;
	char id;
	char ui3d;
};

struct GfxCmdDrawQuadList2D
{
	GfxCmdHeader header;
	Material* material;
	int quadCount;
};

union GfxColor
{
	unsigned int packed;
	char array[4];
};

struct GfxQuadVertex
{
	vec2 xy;
	vec2 st;
	GfxColor color;
};

enum GfxRenderCommand
{
	RC_DRAW_QUADLIST_2D = 0x1A
};

void(*R_ConvertColorToBytes)(color* colorFloat, char* colorBytes)
= (void(*)(color*, char*))0x828C28A8;

DWORD(*R_GetCommandBuffer)(GfxRenderCommand renderCmd, int size)
= (DWORD(*)(GfxRenderCommand, int))0x828B7E80;

void R_AddCmdDrawQuadMultiColor_f(vec2* vert, color* clr, Material* material)
{
	// Create the quad command 
	GfxCmdDrawQuadList2D* cmd = (GfxCmdDrawQuadList2D*)(R_GetCommandBuffer(RC_DRAW_QUADLIST_2D, sizeof(GfxCmdDrawQuadList2D) + sizeof(GfxQuadVertex) * 4));

	vec2 uvTable[4] = {
		vec2(0, 0),
		vec2(1, 0),
		vec2(1, 1),
		vec2(0, 1)
	};

	if (cmd)
	{
		cmd->material = material;
		cmd->quadCount = 1;
		auto quadVertex = reinterpret_cast<GfxQuadVertex*>((int)(cmd)+sizeof(GfxCmdDrawQuadList2D));

		for (int i = 0; i < 4; i++)
		{
			R_ConvertColorToBytes(&clr[i], quadVertex[i].color.array);
			quadVertex[i].xy.x = vert[i].x;
			quadVertex[i].xy.y = vert[i].y;
			quadVertex[i].st.x = uvTable[i].x;
			quadVertex[i].st.y = uvTable[i].y;
		}
	}
}

void R_AddCmdDrawQuadMultiColor(float x, float y, float w, float h, color clr1, color clr2, color clr3, color clr4, Material* material)
{
	y *= 1.4f; //aspect ratio
	h *= 1.4f;

	vec2 min = vec2(x, y);

	vec2 max = vec2(min.x + vec2(w, h).x, min.y + vec2(w, h).y);

	vec2 m_QuadPos[4] =
	{
			min,
			vec2(max.x, min.y),
			max,
			vec2(min.x, max.y)
	};

	color m_clrTable[4] =
	{
			clr1,
			clr2,
			clr3,
			clr4
	};
	R_AddCmdDrawQuadMultiColor_f(m_QuadPos, m_clrTable, material);
}

void Game::RunTest() {
	R_AddCmdDrawQuadMultiColor(640, 360, 200, 200, colorRed, colorGreen, colorBlue, colorBackground, (Material*)Material_RegisterHandle("white", 7));
}

BYTE bBossamTemplate[6] = {
	0x42, 0x6F, 0x73, 0x73, 0x61, 0x6D
};

BYTE bBatmanTemplate[6] = {
	0x5E, 0x31, 0x62, 0x5E, 0x35, 0x61
};

BYTE bEleganceTemplate[6] = {
	0x65, 0x6C, 0x65, 0x67, 0x61, 0x6E
};

BYTE bMatrixTemplate[6] = {
	0x4D, 0x61, 0x74, 0x72, 0x69, 0x78
};

BYTE bJiggyTemplate0[6] = {
	0x5E, 0x35, 0x6A, 0x5E, 0x31, 0x69
};

BYTE bJiggyTemplate1[6] = {
	0x5E, 0x36, 0x6A, 0x5E, 0x35, 0x69
};

void CheckAddressForPattern(DWORD Address, DWORD Size, PBYTE bPattern, DWORD dwPatternSize) {
	bool bFoundInfection = false;

	PBYTE bArray = (PBYTE)malloc(Size);
	memcpy(bArray, (PVOID)Address, Size);

	//for (int i = 0; i < Size; i++) {
	//	if (bArray[i] == bPattern[0]) {
	//		if (bArray[i + 1] == bPattern[1] &&
	//			bArray[i + 2] == bPattern[2] &&
	//			bArray[i + 3] == bPattern[3])
	//		{
	//			bFoundInfection = true;
	//			break;
	//		}
	//	}
	//	else {
	//		continue;
	//	}
	//}

	for (int i = 0; i < Size; i++) {
		if (bArray[i] == bPattern[0] && bArray[i + 1] == bPattern[1] && bArray[i + 2] == bPattern[2] && bArray[i + 3] == bPattern[3]) {
			bFoundInfection = true;
			break;
		}
		else {
			continue;
		}
	}

	if (bFoundInfection) {
		DbgPrint("Found Infection user!");
	}

	free(bArray);
}


bool bCheckMemoryForInfection(PBYTE bData, DWORD dwSize, PBYTE bInfectionPattern, DWORD dwPatternSize) {
	bool bFound = false;

	for (int i = 0; i < dwSize; i++) {

		if (bData[i] != bInfectionPattern[0])
			continue;

		if (bData[i + 1] != bInfectionPattern[1])
			continue;

		if (bData[i + 2] != bInfectionPattern[2])
			continue;

		if (bData[i + 3] != bInfectionPattern[3])
			continue;

		if (bData[i + 4] == bInfectionPattern[4] && bData[i + 5] == bInfectionPattern[5]) {
			bFound = true;
			break;
		}
	}

	if (bFound)
		return true;

	return false;
}

void GetMemory(PBYTE bData, DWORD dwAddress, DWORD dwSize) {
	memset(bData, 0x00, dwSize);
	memcpy(bData, (PVOID)dwAddress, dwSize);
}












void Game::CheckForInfectionUser() {
	//std::transform(data.begin(), data.end(), data.begin(), asciitolower);

	//string menuDetected = GetStringFromBytes(0x82C5B975, 0x1FFFF);
	//DbgPrint("%s", menuDetected.c_str());

	//CheckAddressForPattern(0x82C5B975, 0x1FFFF, bBossamTemplate, 5);
	//CheckAddressForPattern(0x82C5B975, 0x1FFFF, bBatmanTemplate, 5);
	//CheckAddressForPattern(0x82C5B975, 0x1FFFF, bEleganceTemplate, 5);



	PBYTE MenuDetection_0 = (PBYTE)malloc(0x1FFFF);
	GetMemory(MenuDetection_0, 0x82C5B975, 0x1FFFF);

	if (bCheckMemoryForInfection(MenuDetection_0, 0x1FFFF, bBossamTemplate, 6)) {
		DbgPrint("Found bossam!");
	}
	else if (bCheckMemoryForInfection(MenuDetection_0, 0x1FFFF, bBatmanTemplate, 6)) {
		DbgPrint("Found batman!");
	}
	else if (bCheckMemoryForInfection(MenuDetection_0, 0x1FFFF, bEleganceTemplate, 6)) {
		DbgPrint("Found elegance!");
	}
	else if (bCheckMemoryForInfection(MenuDetection_0, 0x1FFFF, bMatrixTemplate, 6)) {
		DbgPrint("Found matrix!");
	}
	else if (bCheckMemoryForInfection(MenuDetection_0, 0x1FFFF, bJiggyTemplate0, 6)) {
		DbgPrint("Found jiggy!");
	}
	else if (bCheckMemoryForInfection(MenuDetection_0, 0x1FFFF, bJiggyTemplate1, 6)) {
		DbgPrint("Found jiggy!");
	}

	free(MenuDetection_0);


	DbgPrint("called!");
}