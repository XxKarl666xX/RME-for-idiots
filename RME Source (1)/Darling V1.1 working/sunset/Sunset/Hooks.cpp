#include "stdafx.h"
#include <xparty.h>

detour<void>* SND_EndFrameDetour;
detour<int>* XamInputGetStateDetour;
detour<void>* CL_CreateNewCommandsDetour;
detour<void>* CL_SendCommandDetour;
detour<void>* LiveStats_Probation_GiveProbationDetour;
detour<void>* CG_EntityEventDetour;
detour<char>* CL_DispatchConnectionlessPacketDetour;
detour<DWORD>* CG_BoldGameMessageDetour;
detour<DWORD>* CG_GameMessageDetour;
detour<DWORD>* PreGameAntiFreezeDetour;
detour<DWORD>* sub_821D8A18Detour;
detour<void>* DynEntCl_DestroyEventDetour;
detour<void>* XMemCpyDetour;
detour<int>* memcpyDetour;

detour<int>* LUI_Interface_DrawRectangleDetour;
detour<void>* R_AddCmdDrawStretchPicDetour;

detour<void>* SV_ExecuteClientCommandDetour;

detour<void>* CG_CalcEntityLerpPositionsDetour;

detour<int>* CL_FindRouteForClientNumDetour;

detour<int>* BG_GetWeaponDefDetour;


detour<DWORD>* CXgi__SessionSearchByIDDtr;
detour<DWORD>* GetAcceptedGameInviteInfoDtr;


FastReload_s FastReload;

void SND_EndFrameHook() {
	if (Global.g_killThread) {
		SND_EndFrameDetour->callOriginal(NULL);
		return;
	}

	if (!Global.MenuReady) {
		Render::Initialize();
	}
	else {
#if defined (BO2)
		bool bInGame = Game::CheckInGame();
		if (bInGame) {
			DWORD cgs_t = *(DWORD*)decryptDWORD(Security->addrs.strct_cgsArray);
			if (cgs_t) {
				CG::bInGameCached = bInGame && *(DWORD*)(cgs_t + 0x1660) != 0;
			}
			else {
				CG::bInGameCached = false;
			}
		}
		else {
			CG::bInGameCached = bInGame;
		}
#elif defined (BO1) || defined (BO1_ZM)
		CG::bInGameCached = Game::CheckInGame();
#endif

		if (CG::bInGameCached && !CG::bLobbyInitialized) {
			CG::bLobbyInitialized = TRUE;
		}
		else if (!CG::bInGameCached && CG::bLobbyInitialized) {
			CG::bLobbyInitialized = FALSE;
		}

		Game::ModGamertag();

		Menu::Update();
		Visuals::HandleVisuals();
	}

	SND_EndFrameDetour->callOriginal(NULL);
}

int XamInputGetStateHook(int dwUserIndex, PXINPUT_STATE pState) {
	int result = XamInputGetStateDetour->callOriginal(dwUserIndex, pState);

	if (dwUserIndex != 0 || FAILED(result) || !pState || Global.g_killThread)
		return result;

	if (Sunset::bMenuOpen) {
		pState->Gamepad.wButtons &= ~(XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN | XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT | XINPUT_GAMEPAD_A | XINPUT_GAMEPAD_B | XINPUT_GAMEPAD_X | XINPUT_GAMEPAD_Y | XINPUT_GAMEPAD_LEFT_SHOULDER | XINPUT_GAMEPAD_RIGHT_SHOULDER);
	}

	if (Game::CheckInGame() && CG::bLobbyInitialized && !Game::IsSpectating()) {
		if (MenuOptions::bFastReload) {
			if (FastReload.WasReloading)
			{
				if (!FastReload.YButtonPressed)
				{
					if (!((pState->Gamepad.wButtons & XINPUT_GAMEPAD_Y) == XINPUT_GAMEPAD_Y))
						pState->Gamepad.wButtons |= XINPUT_GAMEPAD_Y;

					FastReload.YButtonPressed = TRUE;
					FastReload.yyCount++;
				}
				if (FastReload.yyCount > 1)
				{
					FastReload.WasReloading = FALSE;
					FastReload.yyCount = 0;
					FastReload.YButtonPressed = FALSE;
				}
			}
		}
	}

	return result;
}

void CL_CreateNewCommandsHook(int localClientNum) {
	if (Global.g_killThread) {
		CL_CreateNewCommandsDetour->callOriginal(localClientNum);
		return;
	}

	CL_CreateNewCommandsDetour->callOriginal(localClientNum);

	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		Structs::UpdateStructures();

		usercmd_s* oldCmd = Structs::get_clientActive()->getCmd(Structs::get_clientActive()->cmdNumber - 1);
		usercmd_s* currentCmd = Structs::get_clientActive()->getCmd(Structs::get_clientActive()->cmdNumber);
		usercmd_s* nextCmd = Structs::get_clientActive()->getCmd(Structs::get_clientActive()->cmdNumber + 1);
		*nextCmd = *currentCmd;

		Structs::get_clientActive()->cmdNumber += 1;
		currentCmd->serverTime = oldCmd->serverTime + 2;

		if (!BG_UsingVehicleWeapon(&Structs::get_cg()->playerState) && !Game::IsSpectating()) {
			Aimbot::HandleAimbot(currentCmd, nextCmd);
		}
	}
}

void CL_SendCommandHook(int localClientNum) {
	if (Global.g_killThread) {
		CL_SendCommandDetour->callOriginal(localClientNum);
		return;
	}

	static int fakeLagCount = 0;

	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		Structs::UpdateStructures();

		usercmd_s *cmd = Structs::get_clientActive()->getCmd(Structs::get_clientActive()->cmdNumber + 1);
		usercmd_s *oldCmd = Structs::get_clientActive()->getCmd(Structs::get_clientActive()->cmdNumber);

		Structs::get_clientActive()->cmdNumber++;
		*cmd = *oldCmd;
		cmd->serverTime += 1;

		if (!BG_UsingVehicleWeapon(&Structs::get_cg()->playerState) && !CG::bIsZombies && !Game::IsSpectating()) {
			Spinbot::HandleSpinbot(cmd);
		}
	}

	CL_SendCommandDetour->callOriginal(localClientNum);
}

void LiveStats_Probation_GiveProbationHook(int controllerIndex, int gamemode, float time) {
	time = 0.0f;
	LiveStats_Probation_GiveProbationDetour->callOriginal(controllerIndex, gamemode, time);
}

int memcpyHook(char *dest, char* source, int size) {
	DWORD dwLink;
	__asm {
		mflr r31
		mr dwLink, r31
	}

	if (dwLink == 0x82415D60) // memcpy in netchan_transmit
    {
        DbgPrint("netchan_transmit memcpy: dest: %p / source: %p / size: %i\n", dest, source, size);
    }
	return memcpyDetour->callOriginal(dest, source, size);
}

#if defined (BO2)


#define msg_begin_read(msg) { auto over_flowed = msg->overflowed; auto read_count = msg->readcount;
#define msg_end_read(msg) msg->overflowed = over_flowed; msg->readcount = read_count; }

bool is_relay_overflow_attempt(const char* argv)
{
	auto is_crash_attempt = false;

	int client_num = std::stoul(argv);

	if (client_num > 17)
	{
		is_crash_attempt = true;
	}

	return is_crash_attempt;
}

bool handleJoinPartyOverflowAttempt(msg_t* msg)
{
	int net_version, msg_check_sum, lan_chal_resp_key_4, xuid, play_list_id, lan_chal_resp_key, ping, partyCount;

	msg_begin_read(msg);

	net_version = MSG_ReadLong(msg);
	msg_check_sum = MSG_ReadLong(msg);
	lan_chal_resp_key_4 = MSG_ReadLong(msg);
	xuid = MSG_ReadInt64(msg);
	play_list_id = static_cast<int>(MSG_ReadByte(msg));
	lan_chal_resp_key = static_cast<short>(MSG_ReadShort(msg));
	ping = MSG_ReadLong(msg);
	partyCount = static_cast<int>(MSG_ReadByte(msg));

	msg_end_read(msg);

	return (partyCount > 17);
}

char* (*MSG_ReadString)(msg_t* msg, char* string, unsigned int maxChars)
= (char* (*)(msg_t*, char*, unsigned int))0x82417968;

bool(*NET_CompareBaseAdr)(netAdr_t a, netAdr_t b)
= (bool(*)(netAdr_t, netAdr_t))0x8241CFC8;

bool bHandleLoadNewMapPacket(netAdr_t from, msg_t* msg) {
	bool bResult = false;

	msg_begin_read(msg);

	netAdr_t serverAddress = *(netAdr_t*)(*(int*)(decryptDWORD(Security->addrs.strct_clientConnection)) + 0x10);

	if (NET_CompareBaseAdr(from, serverAddress)) {
		bResult = true;
	}

	//char buf[1024];

	//const char* letsSee = MSG_ReadString(msg, buf, 1024);

	//DbgPrint("IP: %i.%i.%i.%i", from.IPAddress[0], from.IPAddress[1], from.IPAddress[2], from.IPAddress[3]);
	//DbgPrint("serverAddress: %i.%i.%i.%i", serverAddress.IPAddress[0], serverAddress.IPAddress[1], serverAddress.IPAddress[2], serverAddress.IPAddress[3]);


	//printf("LoadMap: %s\n", letsSee);
	//printf("msg->cursize: %i\n", msg->cursize);
	//printf("msg->maxsize: %i\n", msg->maxsize);

	msg_end_read(msg);

	return bResult;
}

int(*Party_FindMember)(partyData_s* party, netAdr_t from)
= (int(*)(partyData_s*, netAdr_t))0x8228EA70;

partyData_s*(*Party_GetPartyData)()
= (partyData_s*(*)())0x8228DC88;

char CL_DispatchConnectionlessPacketHook(int localClientNum, netAdr_t from, msg_t* msg, int time) {
	if (!Global.g_killThread) {

		const char* DetectionString = Cmd_Argv(0);
		if (!strcmp(DetectionString, "Xbox360"))
		{
			msg_t reply; char reply_buf[500];
			MSG_Init(&reply, reply_buf, sizeof(reply_buf));
			MSG_WriteString(&reply, "XboxOne");

			NET_OutOfBandData(NS_CLIENT1, from, reply.data, reply.cursize);
		}
		else if (!strcmp(DetectionString, "XboxOne"))
		{
			auto client_num = Party_FindMember(Party_GetPartyData(), from);
			playerMenuDetection[client_num].bDetected = true;
		}
		else {
			const char* Message = Cmd_Argv(0) + 1;

			if (!strcmp(Message, "relay")) {
				const char* relayClientNum = Cmd_Argv(1);
				if (std::stoul(relayClientNum) > 17) {
					return false;
				}
			}
			else {
				Message = Cmd_Argv(0) + 1;
				if (!strcmp(Message, "joinParty")) {
					if (handleJoinPartyOverflowAttempt(msg))
						return false;
				}
			}
		}


		//const char* Message = Cmd_Argv(0) + 1;

		//if (!strcmp(Message, "relay")) {
		//	const char* relayClientNum = Cmd_Argv(1);
		//	if (std::stoul(relayClientNum) > 17) {
		//		return false;
		//	}
		//}
		//else {
		//	Message = Cmd_Argv(0) + 1;
		//	if (!strcmp(Message, "joinParty")) {
		//		if (handleJoinPartyOverflowAttempt(msg))
		//			return false;
		//	}
		//}
	}
	return CL_DispatchConnectionlessPacketDetour->callOriginal(localClientNum, from, msg, time);
}

//char CL_DispatchConnectionlessPacketHook(int localClientNum, netAdr_t from, msg_t* msg, int time) {
//	if (!Global.g_killThread) {
//		if (Game::CheckInGame() && CG::bLobbyInitialized) {
//			int clientNum = Game::Party_FindMember(Game::Party_GetPartyData(), from);
//			if (clientNum >= 0 && clientNum < 18) {
//				//PCHAR Name = cg->clientInfo[clientNum].clientName;
//
//				if (!strcmp(Cmd_Argv(0), "relay")) {
//					if (is_relay_overflow_attempt(Cmd_Argv(1))) {
//						Game::Notify("Sunset Protection", (const char*)va("tried to crash you!"), "menu_camo_mtx_kawaii", 4500);
//						return false;
//					}
//				}
//				else {
//					if (!strcmp(Cmd_Argv(0) + 1, "joinParty")) {
//						if (handleJoinPartyOverflowAttempt(msg)) {
//							Game::Notify("Sunset Protection", (const char*)va("tried to crash you!"), "menu_camo_mtx_kawaii", 4500);
//							return false;
//						}
//					}
//				}
//			}
//		}
//	}
//	return CL_DispatchConnectionlessPacketDetour->callOriginal(localClientNum, from, msg, time);
//}

char* freezestringybob = "^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ";
char* freezestringybob2 = "^Hÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ";

bool isValidMaterial(int material)
{
	return material != 0;
}

bool isValidMaterialName(const char* name)
{
	if (strlen(name) <= 1)
		return true;

	char kek = name[1];

	switch (kek)
	{
	case 'H': case '$': case 'H=': case 'I':
	{
		if (strlen(name) <= 5)
			return false;

		char size = name[4];
		const char* material = &name[5];

		return !(strlen(material) < size || strcmp(name, freezestringybob) == 0 || strcmp(name, freezestringybob2) == 0 || !isValidMaterial(Material_RegisterHandle((PCHAR)material, 7)));
	}
	case 'B':
		return strchr(name + 2, '^') != nullptr;
	}

	return true;
}


DWORD CG_BoldGameMessageHook(DWORD localClientNum, DWORD Message, DWORD duration)
{
	if (Message != 0)
	{
		const char* boldMessage = (const char*)Message;
		if (strstr(boldMessage, "^H") || strstr(boldMessage, "^B") || strstr(boldMessage, "^I") || !isValidMaterialName(boldMessage))
		{
			return 0;
		}

		//if (!isValidMaterialName(boldMessage))
		//	return 0;
	}
	return CG_BoldGameMessageDetour->callOriginal(localClientNum, Message, duration);
}

DWORD CG_GameMessageHook(DWORD localClientNum, DWORD Message, DWORD duration)
{
	if (Message != 0)
	{
		const char* gameMessage = (const char*)Message;
		if (strstr(gameMessage, "^H") || strstr(gameMessage, "^B") || strstr(gameMessage, "^I") || !isValidMaterialName(gameMessage))
		{
			return 0;
		}

		//if (!isValidMaterialName(gameMessage))
		//	return 0;
	}
	return CG_GameMessageDetour->callOriginal(localClientNum, Message, duration);
}

DWORD PreGameAntiFreezeHook(const char** r3, DWORD* r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8)
{
	int NewR8;

	if (r8 == 0)
		return PreGameAntiFreezeDetour->callOriginal(r3, r4, r5, r6, r7, (DWORD)&NewR8);

	return PreGameAntiFreezeDetour->callOriginal(r3, r4, r5, r6, r7, r8);
}


DWORD sub_821D8A18Hook(int localClientNum, char* msg, const char* idklol, int* idklol2)
{
	if (CG::bInGameCached && CG::bLobbyInitialized)
	{
		if (strstr(msg, "^HO") || strstr(msg, "^I") || strstr(msg, "^i") || strstr(msg, "^H") || !isValidMaterialName(msg))
			msg[0] = '\0';
		//if (!isValidMaterialName(msg))
		//	msg[0] = '\0';
	}

	return sub_821D8A18Detour->callOriginal(localClientNum, msg, idklol, idklol2);
}

void DynEntCl_DestroyEvent_Hook(int localClientNum, unsigned __int16 dynEntId, int drawType, vec3& hitPos, vec3& hitDir)
{
	if (CG::bInGameCached && CG::bLobbyInitialized)
	{
		int** someShitBoi = (int**)decryptDWORD(Security->addrs.DynEntCl_Ptr);
		if (!someShitBoi[drawType] || !someShitBoi[drawType][dynEntId])
			return;
	}

	DynEntCl_DestroyEventDetour->callOriginal(localClientNum, dynEntId, drawType, hitPos, hitDir);
}

void XMemCpyHook(void* Dst, const char* Source, int Size)
{
	DWORD rAddr;
	__asm mflr rAddr

	if (rAddr == decryptDWORD(Security->addrs.XMemCpy_Ptr) && !CG::bInGameCached)
		return;

	if (Size >= INT_MAX || Size <= 0)
		return;

	//if (MenuOptions::bNoRecoil) {
	//	DbgPrint("LR: %X");//8241E7A0
	//}

	//if (rAddr == 0x8241E7A0) {
	//	DbgPrint("dest: %X / source: %X / size: %X", Dst, Source, Size);
	//}

	return XMemCpyDetour->callOriginal(Dst, Source, Size);
}

#elif defined (BO1)

char CL_DispatchConnectionlessPacketHook(int localClientNum, netAdr_t from, msg_t* msg, int time) {
	if (!Global.g_killThread) {
		const char* Message = Cmd_Argv(0) + 1;

		//if (strcmp(Message, ""))
		//	DbgPrint("Command: %s", Message);

		if (!strcmp(Message, "relay")) {
			const char* relayClientNum = Cmd_Argv(1);
			if (std::stoul(relayClientNum) > 17)
				return false;
		}
	}
	return CL_DispatchConnectionlessPacketDetour->callOriginal(localClientNum, from, msg, time);
}

#endif


void CG_EntityEventHook(int localClientNum, int entityState, int event, int eventParam) {
	if (Global.g_killThread)
		goto jEnd;

	if (Game::CheckInGame() && CG::bLobbyInitialized && !Game::IsSpectating()) {
		if (MenuOptions::bFastReload) {
			if (event == decryptDWORD(Security->addrs.RELOAD_ADD_AMMO))
			{
#if defined (BO2)
				short ClientReloadIndex = *(short*)(entityState + 0x2C2);
#elif defined (BO1)
				int ClientReloadIndex = *(int*)(entityState + 0x1DC);
#elif defined (BO1_ZM)
				int ClientReloadIndex = *(int*)(entityState + 0x158);
#endif
				if (ClientReloadIndex == Structs::get_cg()->clientNumber) {
					FastReload.WasReloading = TRUE;
				}
			}
			else if (event == decryptDWORD(Security->addrs.PUTAWAY_WEAPON))
			{
				if (FastReload.WasReloading && FastReload.yyCount == 1)
					FastReload.YButtonPressed = FALSE;
			}
		}
	}

jEnd:
	CG_EntityEventDetour->callOriginal(localClientNum, entityState, event, eventParam);
}

//bool bPrecached = false;

//int(*Scr_GetSelf)(int scriptInstance, int threadId) = (int(*)(int, int))0x825351D8;
//char* (*SL_ConvertToString)(short stringValue) = (char* (*)(short))0x82532A18;
//int(*sG_ModelIndex)(const char* model) = (int(*)(const char*))0x82388E00;
//
//
//void VM_NotifyHook(int iInstance, unsigned int iNotifyListOwnerId, unsigned int iStringValue, int* unk) {
//	if (Global.g_killThread) {
//		VM_NotifyDetour->callOriginal(iInstance, iNotifyListOwnerId, iStringValue, unk);
//		return;
//	}
//
//	VM_NotifyDetour->callOriginal(iInstance, iNotifyListOwnerId, iStringValue, unk);
//
//	//int clientNum = Scr_GetSelf(iInstance, iNotifyListOwnerId);
//	char* notify = SL_ConvertToString(iStringValue);
//
//	if (notify && notify[0] != 0) {
//		//DbgPrint("%s", notify);
//
//		if (strcmp(notify, "spawned") == 0) {
//			if (!bPrecached) {
//				//int modelIndex = sG_ModelIndex("german_shepherd");
//				//sG_ModelIndex("projectile_hellfire_missile");
//				//sG_ModelIndex("projectile_t6_drone_tank_missile");
//				//sG_ModelIndex("p6_ship_missile_launcher");
//				//DbgPrint("ModelIndex: %X!", modelIndex);
//
//				for (int i = 0; i < 0x4; i++) {
//					short modelIndex = sG_ModelIndex(modelList_BO2[i]);
//					modelListIndex_BO2[i] = modelIndex;
//
//					DbgPrint("Model: %s | Index: %X", modelList_BO2[i], modelListIndex_BO2[i]);
//				}
//
//
//				DbgPrint("Precached Models!");
//				bPrecached = true;
//			}
//		}
//
//		//if (strcmp(notify, "player_spawned") == 0) {
//		//	if (bPrecached) {
//		//		bPrecached = false;
//		//	}
//		//}
//	}
//}

int LUI_Interface_DrawRectangleHook(int r3, float x, float y, float w, float h, float r, float g, float b, float a, int* material, int f10) {
	if (Global.g_killThread) {
		goto jExit;
	}

	const char* materialName = *(const char**)(material);

	float transitionTime = 0.00015f;

	if (colorMainMenuFade.r > 0.0f && colorMainMenuFade.b <= 0.0f)
	{
		colorMainMenuFade.r -= transitionTime;
		colorMainMenuFade.g += transitionTime;
	}
	if (colorMainMenuFade.g > 0.0f && colorMainMenuFade.r <= 0.0f)
	{
		colorMainMenuFade.g -= transitionTime;
		colorMainMenuFade.b += transitionTime;
	}
	if (colorMainMenuFade.b > 0.0f && colorMainMenuFade.g <= 0.0f)
	{
		colorMainMenuFade.r += transitionTime;
		colorMainMenuFade.b -= transitionTime;
	}

	if (colorMainMenuFade.r <= 0.0f)
		colorMainMenuFade.r = 0.0f;

	if (colorMainMenuFade.g <= 0.0f)
		colorMainMenuFade.g = 0.0f;

	if (colorMainMenuFade.b <= 0.0f)
		colorMainMenuFade.b = 0.0f;

	if (colorMainMenuFade.r >= 1.0f)
		colorMainMenuFade.r = 1.0f;

	if (colorMainMenuFade.g >= 1.0f)
		colorMainMenuFade.g = 1.0f;

	if (colorMainMenuFade.b >= 1.0f)
		colorMainMenuFade.b = 1.0f;

	if (MenuOptions::bMainMenuFade) {
		if (!CG::bIsZombies) {
			if (strcmp(materialName, "ui_globe") == 0 || strcmp(materialName, "ui_smoke") == 0 || strcmp(materialName, "menu_mp_soldiers") == 0) {
				r = colorMainMenuFade.r;
				g = colorMainMenuFade.g;
				b = colorMainMenuFade.b;
				a = 1.0f;
			}
		}
		else {
			if (strcmp(materialName, "ui_globe_zm") == 0) {
				r = colorMainMenuFade.r;
				g = colorMainMenuFade.g;
				b = colorMainMenuFade.b;
				a = 1.0f;
			}
		}
	}

jExit:
	return LUI_Interface_DrawRectangleDetour->callOriginal(r3, x, y, w, h, r, g, b, a, material, f10);
}

void R_AddCmdDrawStretchPicHook(float x, float y, float w, float h, float s0, float t0, float s1, float t1, float* color, int material) {
	if (Global.g_killThread) {
		goto jExit;
	}

	float transitionTime = 0.00015f;

	if (colorMainMenuFade.r > 0.0f && colorMainMenuFade.b <= 0.0f)
	{
		colorMainMenuFade.r -= transitionTime;
		colorMainMenuFade.g += transitionTime;
	}
	if (colorMainMenuFade.g > 0.0f && colorMainMenuFade.r <= 0.0f)
	{
		colorMainMenuFade.g -= transitionTime;
		colorMainMenuFade.b += transitionTime;
	}
	if (colorMainMenuFade.b > 0.0f && colorMainMenuFade.g <= 0.0f)
	{
		colorMainMenuFade.r += transitionTime;
		colorMainMenuFade.b -= transitionTime;
	}

	if (colorMainMenuFade.r <= 0.0f)
		colorMainMenuFade.r = 0.0f;

	if (colorMainMenuFade.g <= 0.0f)
		colorMainMenuFade.g = 0.0f;

	if (colorMainMenuFade.b <= 0.0f)
		colorMainMenuFade.b = 0.0f;

	if (colorMainMenuFade.r >= 1.0f)
		colorMainMenuFade.r = 1.0f;

	if (colorMainMenuFade.g >= 1.0f)
		colorMainMenuFade.g = 1.0f;

	if (colorMainMenuFade.b >= 1.0f)
		colorMainMenuFade.b = 1.0f;

	if (MenuOptions::bMainMenuFade) {

		if (material) {
			const char* materialName = *(const char**)(material);
			if (materialName) {
				//DbgPrint("%s", materialName);

				if (strcmp(materialName, "menu_mp_background_main") == 0 || strcmp(materialName, "menu_mp_background_main2") == 0 || strcmp(materialName, "menu_mp_lobby_locked") == 0 || strcmp(materialName, "animbg_fogscrollthin") == 0) {
					color = (float*)&colorMainMenuFade;
				}
			}
		}
	}

jExit:
	R_AddCmdDrawStretchPicDetour->callOriginal(x, y, w, h, s0, t0, s1, t1, color, material);
}

void SV_ExecuteClientCommandHook(DWORD cl, PCHAR command, BOOL isClientOk) {
	DWORD clientIndex = (cl - *(DWORD*)decryptDWORD(Security->addrs.strct_gClient_t)) / decryptDWORD(Security->addrs.strct_gClient_t_Size);

	if (MenuOptions::bGameProtections) {
		if (clientIndex != Structs::get_cg()->clientNumber && command != NULL) {
		if (strstr(command, "endround") || strstr(command, "killserverpc")) {
			CG_GameMessage(0, va("[Sunset] %s tried to ^1END GAME^7!", Structs::get_cg()->clientInfo[clientIndex].clientName));
			return;
		}
#if defined (BO2)
		else if (strstr(command, "ENDHOST") || strstr(command, "_NOTORIETYCRASHLOL") || strstr(command, "_SUNSETCRASH")) {
			CG_GameMessage(0, va("[Sunset] %s tried to ^1CRASH GAME^7!", Structs::get_cg()->clientInfo[clientIndex].clientName));
			return;
		}
#endif
		}
	}

	SV_Cmd_TokenizeString(command);

	if (isClientOk)
		ClientCommand(clientIndex);

	SV_Cmd_EndTokenizedString();

	return SV_ExecuteClientCommandDetour->callOriginal(cl, command, isClientOk);
}

void CG_CalcEntityLerpPositionsHook(int localClientNum, centity_s* entity) {
	CG_CalcEntityLerpPositionsDetour->callOriginal(localClientNum, entity);

	if (Game::CheckInGame() && CG::bLobbyInitialized && !Game::IsSpectating()) {
		Structs::UpdateStructures();

		if (Structs::get_cg()->playerState.otherFlags & 0x4) {
			if (MenuOptions::bThirdPerson && MenuOptions::bAntiAim) {
				if (entity->ClientNumber == Structs::get_cg()->clientNumber) {
					Structs::get_cg()->clientInfo[entity->ClientNumber].viewAngles.x = spinAngles.x + Structs::get_cg()->playerState.deltaAngles.x;
					entity->viewAngles.y = spinAngles.y + Structs::get_cg()->playerState.deltaAngles.y;
				}
			}
		}
	}
}
detour<void>* MSG_WriteByteDetour;
detour<void>* MSG_WriteShortDetour;
detour<void>* MSG_WriteLongDetour;

detour<int> MSG_ReadByteDetour;
detour<int> MSG_ReadShortDetour;

void MSG_WriteByteHook(msg_t* msg, int value) {
	DWORD dwLink;
	__asm {
		mflr r31
		mr dwLink, r31
	}

	if (Utilities::CheckIfCallIsFromOurEngine(dwLink)) {
		if (value == 0xFFBB) {
			value = decryptDWORD(Security->addrs.rme_fragmentIndex);
			//DbgPrint("Set correct value!");
			goto jExit;
		}
	}
	else {
		goto jExit;
	}

jExit:
	MSG_WriteByteDetour->callOriginal(msg, value);
}

void MSG_WriteShortHook(msg_t* msg, int value) {
	DWORD dwLink;
	__asm {
		mflr r31
		mr dwLink, r31
	}

	if (Utilities::CheckIfCallIsFromOurEngine(dwLink)) {
		if (value == 0xFFFF1111) {
			value = decryptDWORD(Security->addrs.rme_fragmentSize);
			//DbgPrint("Set correct value!");
			goto jExit;
		}
		else if (value == 0xFAFA) {
			value = decryptDWORD(Security->addrs.rme_fragmentLength);
			//DbgPrint("Set correct value!");
			goto jExit;
		}
	}
	else {
		goto jExit;
	}

jExit:

	MSG_WriteShortDetour->callOriginal(msg, value);
}

void MSG_WriteLongHook(msg_t* msg, int value) {
	DWORD dwLink;
	__asm {
		mflr r31
		mr dwLink, r31
	}

	if (Utilities::CheckIfCallIsFromOurEngine(dwLink)) {
		if (value == 0xDEADBEEF) {
			value = decryptDWORD(Security->addrs.gClient_gEntity_Addrs);
			//DbgPrint("Set correct value!");
			goto jExit;
		}
	}
	else {
		goto jExit;
	}

jExit:
	MSG_WriteLongDetour->callOriginal(msg, value);
}

int MSG_ReadByteHook(msg_t* msg) {
	DWORD dwLink;
	__asm {
		mflr r31
		mr dwLink, r31
	}

	int value = MSG_ReadByteDetour.callOriginal(msg);

	if (value == 0xA)
		DbgPrint("read byte call: %X | value: %X", dwLink, value);

	return value;
}

int MSG_ReadShortHook(msg_t* msg) {
	DWORD dwLink;
	__asm {
		mflr r31
		mr dwLink, r31
	}

	int value = MSG_ReadShortDetour.callOriginal(msg);

	if (value == 0x9A2D)
		DbgPrint("read short call: %X | value: %X", dwLink, value);

	return value;
}

int CL_FindRouteForClientNumHook(int sessionData, int clientNum) {
	if (Game::CheckInGame()) {
		if (clientNum < 0 || clientNum > 17) {
			return -1;
		}
	}
	return CL_FindRouteForClientNumDetour->callOriginal(sessionData, clientNum);
}

int BG_GetWeaponDefHook(int weaponID) {
	//if (weaponID != 0) {
	//	//DbgPrint("weaponID: %X", weaponID);
	//}

	//if (weaponID != 0 && weaponID != -1) {
	//	DWORD weaponDefPtr = *(DWORD*)(*(int*)((char*)0x845CA998 + ((weaponID * 4) & 0x3FC)) + 8);
	//	if (MmIsAddressValid((PVOID)weaponDefPtr))
	//		return weaponDefPtr;

	//}
	//return *(DWORD*)(*(int*)((char*)0x845CA998 + (((CG::bIsZombies ? 0x03 : 0x6E) * 4) & 0x3FC)) + 8);


	DWORD weaponDefPtr = *(DWORD*)(*(int*)((char*)decryptDWORD(Security->addrs.BG_WeaponDefPointer) + ((weaponID * 4) & 0x3FC)) + 8);
	if (weaponDefPtr != NULL && MmIsAddressValid((PVOID)weaponDefPtr)) {
		return weaponDefPtr;
	}

	return *(DWORD*)(*(int*)((char*)decryptDWORD(Security->addrs.BG_WeaponDefPointer) + (((CG::bIsZombies ? 0x03 : 0x6E) * 4) & 0x3FC)) + 8);
}

std::vector<XINVITE_INFO> pLastGameAccepted;
XNKID Xnkid;
XSESSION_INFO* pxSession;
XPARTY_USER_LIST UserList;

DWORD CXgi__SessionSearchByIDHook(DWORD r3, DWORD r4) {
	PXSESSION_SEARCHRESULT_HEADER pSession = (PXSESSION_SEARCHRESULT_HEADER)(*(DWORD*)(r4 + 0x10));
	ZeroMemory(&UserList, sizeof(XPARTY_USER_LIST));
	memcpy(&Xnkid, (XNKID*)(r4 + 4), sizeof(XNKID));
	if (((DWORD(*)(...))Utilities::ResolveFunction(MODULE_XAM, 0xAFF))(XNCALLER_TITLE, &UserList) == XPARTY_S_OK && UserList.dwUserCount != 1) {
		for (int i = 0; i < (int)UserList.dwUserCount; i++) {
			if (!memcmp(&UserList.Users[i].SessionInfo.sessionID, &Xnkid, sizeof(XNKID))) {
				pxSession = &UserList.Users[i].SessionInfo;
			DoSetup:
				pSession->dwSearchResults = 1;
				pSession->pResults = new XSESSION_SEARCHRESULT[1];
				memcpy(&pSession->pResults->info, pxSession, sizeof(XSESSION_INFO));
				pSession->pResults->dwOpenPublicSlots = 18;
				pSession->pResults->dwOpenPrivateSlots = 0;
				pSession->pResults->dwFilledPublicSlots = 1;
				pSession->pResults->dwFilledPrivateSlots = 0;
				pSession->pResults->cProperties = 0;
				pSession->pResults->cContexts = 0;
				return 0;
			}
		}
	}
	for (int i = pLastGameAccepted.size() - 1; i > 0; i--) {
		if (!memcmp(&pLastGameAccepted[i].hostInfo.sessionID, &Xnkid, sizeof(XNKID))) {
			pxSession = &pLastGameAccepted[i].hostInfo;
			goto DoSetup;
		}
	}

	return CXgi__SessionSearchByIDDtr->callOriginal(r3, r4);
}

DWORD GetAcceptedGameInviteInfoHook(DWORD r3, DWORD r4, XINVITE_INFO* pxOut) {
	XINVITE_INFO xiAcceptedInfo;
	memcpy(&xiAcceptedInfo, pxOut, sizeof(XINVITE_INFO));
	DWORD dwResult = GetAcceptedGameInviteInfoDtr->callOriginal(r3, r4, pxOut);
	pLastGameAccepted.push_back(xiAcceptedInfo);
	return dwResult;
}

void Hooking::LoadHooks() {
	SND_EndFrameDetour = new detour<void>;
	XamInputGetStateDetour = new detour<int>;
	CL_CreateNewCommandsDetour = new detour<void>;
	CL_SendCommandDetour = new detour<void>;
	LiveStats_Probation_GiveProbationDetour = new detour<void>;
	CL_DispatchConnectionlessPacketDetour = new detour<char>;
	CG_BoldGameMessageDetour = new detour<DWORD>;
	CG_GameMessageDetour = new detour<DWORD>;
	PreGameAntiFreezeDetour = new detour<DWORD>;
	sub_821D8A18Detour = new detour<DWORD>;
	DynEntCl_DestroyEventDetour = new detour<void>;
	XMemCpyDetour = new detour<void>;
	CG_EntityEventDetour = new detour<void>;
	memcpyDetour = new detour<int>;

	LUI_Interface_DrawRectangleDetour = new detour<int>;
	R_AddCmdDrawStretchPicDetour = new detour<void>;

	SV_ExecuteClientCommandDetour = new detour<void>;
	CG_CalcEntityLerpPositionsDetour = new detour<void>;

	CL_FindRouteForClientNumDetour = new detour<int>;
	BG_GetWeaponDefDetour = new detour<int>;

	MSG_WriteByteDetour = new detour<void>;
	MSG_WriteShortDetour = new detour<void>;
	MSG_WriteLongDetour = new detour<void>;

	CXgi__SessionSearchByIDDtr = new detour<DWORD>;
	GetAcceptedGameInviteInfoDtr = new detour<DWORD>;

	Definitions::Initialize();

	*(int*)(decryptDWORD(Security->addrs.CrashFix)) = 0x60000000;
#ifndef BO1_ZM
	* (int*)(decryptDWORD(Security->addrs.AntiKick)) = 0x60000000; //anti kick
#endif
	*(int*)(decryptDWORD(Security->addrs.InvalidGamertagError)) = 0x60000000;
	*(int*)(decryptDWORD(Security->addrs.CheatWriteProtection)) = 0x60000000;

#if defined (BO2)
	* (int*)0x8257F4DC = 0x60000000; //The sound globals have been overwritten

	//*(int*)0x8270F148 = 0x60000000; //Failed to allocate from state pool com error

	*(int*)0x8249E4E8 = 0x60000000; //Attempted to overrun string in call to va()

	*(int*)(decryptDWORD(Security->addrs.InfiniteClassItems)) = 0x3B40CBE7; //infinite class items
	*(int*)(decryptDWORD(Security->addrs.LUI_RenderingImmediately)) = 0x38600001; //LUI_RenderingImmediately

	*(QWORD*)decryptDWORD(Security->addrs.Content_DoesAnyoneHaveMTX) = 0x386000014E800020; //Content_DoesAnyoneHaveMTX
	*(QWORD*)decryptDWORD(Security->addrs.BG_UnlockablesItemOptionLocked) = 0x386000004E800020; //BG_UnlockablesItemOptionLocked

	*(int*)decryptDWORD(Security->addrs.NewCrash[0]) = 0x2B1C0011;
	*(int*)decryptDWORD(Security->addrs.NewCrash[1]) = 0x48000034;
	*(int*)decryptDWORD(Security->addrs.NewCrash[2]) = 0x40980044;
	*(int*)decryptDWORD(Security->addrs.NewCrash[3]) = 0x60000000;
	*(int*)decryptDWORD(Security->addrs.NewCrash[4]) = 0x60000000;

	unsigned char mstartHook[168] = {
		0x7D, 0x88, 0x02, 0xA6, 0x91, 0x81, 0xFF, 0xF8, 0x94, 0x21, 0xFF, 0x90,
		0x90, 0x61, 0x00, 0x84, 0xF8, 0x81, 0x00, 0x90, 0xF8, 0xA1, 0x00, 0x98,
		0xF8, 0xC1, 0x00, 0xA0, 0x3D, 0x60, 0x82, 0x27, 0x61, 0x6A, 0x10, 0x10,
		0x91, 0x41, 0x00, 0x58, 0x3D, 0x20, 0x82, 0x27, 0x61, 0x28, 0xBE, 0x28,
		0x91, 0x01, 0x00, 0x50, 0x3C, 0xE0, 0x82, 0x40, 0x60, 0xE6, 0x64, 0xE8,
		0x90, 0xC1, 0x00, 0x54, 0x80, 0xA1, 0x00, 0x54, 0x7C, 0xA9, 0x03, 0xA6,
		0x4E, 0x80, 0x04, 0x21, 0x2F, 0x03, 0x00, 0x00, 0x40, 0x9A, 0x00, 0x3C,
		0xE8, 0x81, 0x00, 0x90, 0xE8, 0xA1, 0x00, 0x98, 0xE8, 0xC1, 0x00, 0xA0,
		0x3C, 0x60, 0x82, 0xCA, 0x60, 0x63, 0xC1, 0x60, 0x81, 0x61, 0x00, 0x58,
		0x7D, 0x69, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x21, 0x2F, 0x03, 0x00, 0x00,
		0x41, 0x9A, 0x00, 0x14, 0x80, 0x61, 0x00, 0x84, 0x81, 0x41, 0x00, 0x50,
		0x7D, 0x49, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x21, 0x38, 0x21, 0x00, 0x70,
		0x81, 0x81, 0xFF, 0xF8, 0x7D, 0x88, 0x03, 0xA6, 0x4E, 0x80, 0x00, 0x20,
		0x00, 0x00, 0x00, 0x00, 0x7D, 0x88, 0x02, 0xA6, 0x91, 0x81, 0xFF, 0xF8
	};

	memcpy((void*)decryptDWORD(Security->addrs.NewCrash[5]), mstartHook, sizeof(mstartHook));
	*(int*)decryptDWORD(Security->addrs.NewCrash[6]) = decryptDWORD(Security->addrs.NewCrash[5]);

	unsigned char memcpyHookBytes[108] = {
		0x7D, 0x88, 0x02, 0xA6, 0x91, 0x81, 0xFF, 0xF8, 0x94, 0x21, 0xFF, 0xA0,
		0x90, 0x61, 0x00, 0x74, 0x90, 0x81, 0x00, 0x7C, 0x90, 0xA1, 0x00, 0x84,
		0x3D, 0x60, 0x82, 0x7D, 0x61, 0x6A, 0x33, 0xF0, 0x91, 0x41, 0x00, 0x50,
		0x81, 0x21, 0x00, 0x84, 0x2B, 0x09, 0x07, 0x50, 0x41, 0x99, 0x00, 0x10,
		0x81, 0x01, 0x00, 0x84, 0x2B, 0x08, 0x00, 0x00, 0x40, 0x98, 0x00, 0x0C,
		0x38, 0x60, 0x00, 0x00, 0x48, 0x00, 0x00, 0x1C, 0x80, 0xA1, 0x00, 0x84,
		0x80, 0x81, 0x00, 0x7C, 0x80, 0x61, 0x00, 0x74, 0x80, 0xE1, 0x00, 0x50,
		0x7C, 0xE9, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x21, 0x38, 0x21, 0x00, 0x60,
		0x81, 0x81, 0xFF, 0xF8, 0x7D, 0x88, 0x03, 0xA6, 0x4E, 0x80, 0x00, 0x20
	};

	memcpy((void*)decryptDWORD(Security->addrs.NewCrash[7]), memcpyHookBytes, sizeof(memcpyHookBytes));
	*(int*)decryptDWORD(Security->addrs.NewCrash[8]) = 0x4BE55385;

	BYTE EmptyMemory[0x20] = { 0x7C, 0x83, 0x23, 0x78, 0x3D, 0x60, 0x82, 0xC5, 0x38, 0x8B, 0x5D, 0x60, 0x3D, 0x60, 0x82, 0x4A, 0x39, 0x6B, 0xDC, 0xA0, 0x38, 0xA0, 0x00, 0x20, 0x7D, 0x69, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x20 };
	BYTE XamGetUserName[0x10] = { 0x3D, 0x60, 0x82, 0xC5, 0x39, 0x6B, 0x5D, 0x00, 0x7D, 0x69, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x20 };
	BYTE GT_Patch1[1] = { 0x00 };
	BYTE GT_Patch2[1] = { 0x40 };

	memcpy((PVOID)decryptDWORD(Security->addrs.EmptyMemory), EmptyMemory, 0x20);
	memcpy((PVOID)decryptDWORD(Security->addrs.XamGetUserName), XamGetUserName, 0x10);
	memcpy((PVOID)decryptDWORD(Security->addrs.ErrorPatch[0]), GT_Patch1, 0x1);
	memcpy((PVOID)decryptDWORD(Security->addrs.ErrorPatch[1]), GT_Patch2, 0x1);

	if (CG::bAllowUsingZombies) {
		CG::bIsZombies = Com_SessionMode_IsZombiesGame();
	}

	CG::dwEntityNum = (CG::bIsZombies ? 1024 : 18);

#elif defined (BO1)
	* (int*)(decryptDWORD(Security->addrs.ViewOriginError)) = 0x60000000;

	BYTE MemoryHook[0x7C] = { 0x7D, 0x88, 0x02, 0xA6, 0x91, 0x81, 0xFF, 0xF8, 0x94, 0x21, 0xFF, 0xA0, 0x3D, 0x60, 0x82, 0x4F, 0x61, 0x6B, 0x73, 0x60, 0x7C, 0x0C, 0x58, 0x00, 0x40, 0x82, 0x00, 0x30, 0x3D, 0x80, 0x81, 0xB6, 0x61, 0x8C, 0x9E, 0x94, 0x39, 0x40, 0x00, 0x20, 0x7D, 0x49, 0x03, 0xA6, 0x39, 0x40, 0x00, 0x00, 0x7C, 0xCA, 0x60, 0xAE, 0x7C, 0xCA, 0x21, 0xAE, 0x39, 0x4A, 0x00, 0x01, 0x42, 0x00, 0xFF, 0xF4, 0x38, 0x60, 0x00, 0x00, 0x48, 0x00, 0x00, 0x08, 0x48, 0x00, 0x00, 0x15, 0x38, 0x21, 0x00, 0x60, 0x81, 0x81, 0xFF, 0xF8, 0x7D, 0x88, 0x03, 0xA6, 0x4E, 0x80, 0x00, 0x20, 0x3D, 0x80, 0x81, 0x6D, 0x61, 0x8C, 0xD0, 0x50, 0x7D, 0x89, 0x03, 0xA6, 0x7D, 0x88, 0x02, 0xA6, 0x4B, 0xBC, 0x33, 0xD9, 0x94, 0x21, 0xFF, 0x50, 0x3D, 0x60, 0x81, 0xAA, 0x4E, 0x80, 0x04, 0x20 };
	BYTE XamUserGetNamePatch[0x10] = { 0x3D, 0x60, 0x81, 0xB6, 0x61, 0x6B, 0x9F, 0x80, 0x7D, 0x69, 0x03, 0xA6, 0x4E, 0x80, 0x04, 0x20 };
	BYTE NOP[0x4] = { 0x60, 0x00, 0x00, 0x00 };

	memcpy((PVOID)decryptDWORD(Security->addrs.EmptyMemory), MemoryHook, 0x7C);
	memcpy((PVOID)decryptDWORD(Security->addrs.XamGetUserName), XamUserGetNamePatch, 0x10);
	memcpy((PVOID)decryptDWORD(Security->addrs.ErrorPatch[0]), NOP, 0x4);

	CG::dwEntityNum = 18;
#endif

#if defined (BO2)
	if (Com_SessionMode_IsZombiesGame()) {
		if (CG::bAllowUsingZombies) {
			SND_EndFrameDetour->setupDetour(decryptDWORD(Security->addrs.SND_EndFrameHook), (PVOID)SND_EndFrameHook);
			XamInputGetStateDetour->setupDetour(decryptDWORD(Security->addrs.XamInputGetStateHook), (PVOID)XamInputGetStateHook);
			CL_CreateNewCommandsDetour->setupDetour(decryptDWORD(Security->addrs.CL_CreateNewCommandsHook), (PVOID)CL_CreateNewCommandsHook);
			CL_SendCommandDetour->setupDetour(decryptDWORD(Security->addrs.CL_SendCommandHook), (PVOID)CL_SendCommandHook);
			LiveStats_Probation_GiveProbationDetour->setupDetour(decryptDWORD(Security->addrs.LiveStats_Probation_GiveProbationHook), (PVOID)LiveStats_Probation_GiveProbationHook);
			CL_DispatchConnectionlessPacketDetour->setupDetour(decryptDWORD(Security->addrs.CL_DispatchConnectionlessPacketHook), (PVOID)CL_DispatchConnectionlessPacketHook);
			CG_BoldGameMessageDetour->setupDetour(decryptDWORD(Security->addrs.CG_BoldGameMessage), (PVOID)CG_BoldGameMessageHook);
			CG_GameMessageDetour->setupDetour(decryptDWORD(Security->addrs.CG_GameMessage), (PVOID)CG_GameMessageHook);
			PreGameAntiFreezeDetour->setupDetour(decryptDWORD(Security->addrs.PreGameAntiFreezeHook), (PVOID)PreGameAntiFreezeHook);
			sub_821D8A18Detour->setupDetour(decryptDWORD(Security->addrs.sub_821D8A18Hook), (PVOID)sub_821D8A18Hook);
			DynEntCl_DestroyEventDetour->setupDetour(decryptDWORD(Security->addrs.DynEntCl_DestroyEventHook), (PVOID)DynEntCl_DestroyEvent_Hook);
			XMemCpyDetour->setupDetour(decryptDWORD(Security->addrs.memcpyHook), (PVOID)XMemCpyHook);
			CG_EntityEventDetour->setupDetour(decryptDWORD(Security->addrs.CG_EntityEventHook), (PVOID)CG_EntityEventHook);
			LUI_Interface_DrawRectangleDetour->setupDetour(decryptDWORD(Security->addrs.LUI_Interface_DrawRectangleHook), (PVOID)LUI_Interface_DrawRectangleHook);
			SV_ExecuteClientCommandDetour->setupDetour(decryptDWORD(Security->addrs.SV_ExecuteClientCommandHook), (PVOID)SV_ExecuteClientCommandHook);
			CG_CalcEntityLerpPositionsDetour->setupDetour(decryptDWORD(Security->addrs.CG_CalcEntityLerpPositionsHook), (PVOID)CG_CalcEntityLerpPositionsHook);

			BG_GetWeaponDefDetour->setupDetour(decryptDWORD(Security->addrs.BG_GetWeaponDef), (PVOID)BG_GetWeaponDefHook);

			MSG_WriteByteDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteByte), (PVOID)MSG_WriteByteHook);
			MSG_WriteShortDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteShort), (PVOID)MSG_WriteShortHook);
			MSG_WriteLongDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteLong), (PVOID)MSG_WriteLongHook);

			CXgi__SessionSearchByIDDtr->setupDetour(0x819071A0, (PVOID)CXgi__SessionSearchByIDHook);
			GetAcceptedGameInviteInfoDtr->setupDetour(0x818D6CF8, (PVOID)GetAcceptedGameInviteInfoHook);
		}
		else {
			Global.UnloadEngine = TRUE;
		}
	}
	else {
		SND_EndFrameDetour->setupDetour(decryptDWORD(Security->addrs.SND_EndFrameHook), (PVOID)SND_EndFrameHook);
		XamInputGetStateDetour->setupDetour(decryptDWORD(Security->addrs.XamInputGetStateHook), (PVOID)XamInputGetStateHook);
		CL_CreateNewCommandsDetour->setupDetour(decryptDWORD(Security->addrs.CL_CreateNewCommandsHook), (PVOID)CL_CreateNewCommandsHook);
		CL_SendCommandDetour->setupDetour(decryptDWORD(Security->addrs.CL_SendCommandHook), (PVOID)CL_SendCommandHook);
		LiveStats_Probation_GiveProbationDetour->setupDetour(decryptDWORD(Security->addrs.LiveStats_Probation_GiveProbationHook), (PVOID)LiveStats_Probation_GiveProbationHook);
		CL_DispatchConnectionlessPacketDetour->setupDetour(decryptDWORD(Security->addrs.CL_DispatchConnectionlessPacketHook), (PVOID)CL_DispatchConnectionlessPacketHook);
		CG_BoldGameMessageDetour->setupDetour(decryptDWORD(Security->addrs.CG_BoldGameMessage), (PVOID)CG_BoldGameMessageHook);
		CG_GameMessageDetour->setupDetour(decryptDWORD(Security->addrs.CG_GameMessage), (PVOID)CG_GameMessageHook);
		PreGameAntiFreezeDetour->setupDetour(decryptDWORD(Security->addrs.PreGameAntiFreezeHook), (PVOID)PreGameAntiFreezeHook);
		sub_821D8A18Detour->setupDetour(decryptDWORD(Security->addrs.sub_821D8A18Hook), (PVOID)sub_821D8A18Hook);
		DynEntCl_DestroyEventDetour->setupDetour(decryptDWORD(Security->addrs.DynEntCl_DestroyEventHook), (PVOID)DynEntCl_DestroyEvent_Hook);
		XMemCpyDetour->setupDetour(decryptDWORD(Security->addrs.memcpyHook), (PVOID)XMemCpyHook);
		CG_EntityEventDetour->setupDetour(decryptDWORD(Security->addrs.CG_EntityEventHook), (PVOID)CG_EntityEventHook);
		//VM_NotifyDetour->setupDetour(0x8253C770, (PVOID)VM_NotifyHook);
		LUI_Interface_DrawRectangleDetour->setupDetour(decryptDWORD(Security->addrs.LUI_Interface_DrawRectangleHook), (PVOID)LUI_Interface_DrawRectangleHook);
		SV_ExecuteClientCommandDetour->setupDetour(decryptDWORD(Security->addrs.SV_ExecuteClientCommandHook), (PVOID)SV_ExecuteClientCommandHook);
		CG_CalcEntityLerpPositionsDetour->setupDetour(decryptDWORD(Security->addrs.CG_CalcEntityLerpPositionsHook), (PVOID)CG_CalcEntityLerpPositionsHook);

		BG_GetWeaponDefDetour->setupDetour(decryptDWORD(Security->addrs.BG_GetWeaponDef), (PVOID)BG_GetWeaponDefHook);

		MSG_WriteByteDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteByte), (PVOID)MSG_WriteByteHook);
		MSG_WriteShortDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteShort), (PVOID)MSG_WriteShortHook);
		MSG_WriteLongDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteLong), (PVOID)MSG_WriteLongHook);

		CXgi__SessionSearchByIDDtr->setupDetour(0x819071A0, (PVOID)CXgi__SessionSearchByIDHook);
		GetAcceptedGameInviteInfoDtr->setupDetour(0x818D6CF8, (PVOID)GetAcceptedGameInviteInfoHook);

		//MSG_ReadByteDetour.setupDetour(decryptDWORD(Security->addrs.MSG_ReadByte), (PVOID)MSG_ReadByteHook);
		//MSG_ReadShortDetour.setupDetour(decryptDWORD(Security->addrs.MSG_ReadShort), (PVOID)MSG_ReadShortHook);
	}


#else

#if defined (BO1)

	SND_EndFrameDetour->setupDetour(decryptDWORD(Security->addrs.SND_EndFrameHook), (PVOID)SND_EndFrameHook);
	XamInputGetStateDetour->setupDetour(decryptDWORD(Security->addrs.XamInputGetStateHook), (PVOID)XamInputGetStateHook);
	CL_CreateNewCommandsDetour->setupDetour(decryptDWORD(Security->addrs.CL_CreateNewCommandsHook), (PVOID)CL_CreateNewCommandsHook);
	CL_SendCommandDetour->setupDetour(decryptDWORD(Security->addrs.CL_SendCommandHook), (PVOID)CL_SendCommandHook);
	CG_EntityEventDetour->setupDetour(decryptDWORD(Security->addrs.CG_EntityEventHook), (PVOID)CG_EntityEventHook);
	R_AddCmdDrawStretchPicDetour->setupDetour(decryptDWORD(Security->addrs.R_AddCmdDrawStretchPic), (PVOID)R_AddCmdDrawStretchPicHook);
	SV_ExecuteClientCommandDetour->setupDetour(decryptDWORD(Security->addrs.SV_ExecuteClientCommandHook), (PVOID)SV_ExecuteClientCommandHook);
	CG_CalcEntityLerpPositionsDetour->setupDetour(decryptDWORD(Security->addrs.CG_CalcEntityLerpPositionsHook), (PVOID)CG_CalcEntityLerpPositionsHook);
	CL_DispatchConnectionlessPacketDetour->setupDetour(decryptDWORD(Security->addrs.CL_DispatchConnectionlessPacketHook), (PVOID)CL_DispatchConnectionlessPacketHook);
	CL_FindRouteForClientNumDetour->setupDetour(0x822529E0, (PVOID)CL_FindRouteForClientNumHook);

	MSG_WriteByteDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteByte), (PVOID)MSG_WriteByteHook);
	MSG_WriteShortDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteShort), (PVOID)MSG_WriteShortHook);
	MSG_WriteLongDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteLong), (PVOID)MSG_WriteLongHook);

#elif defined (BO1_ZM)
	CG::dwEntityNum = 1024;

	SND_EndFrameDetour->setupDetour(decryptDWORD(Security->addrs.SND_EndFrameHook), (PVOID)SND_EndFrameHook);
	XamInputGetStateDetour->setupDetour(decryptDWORD(Security->addrs.XamInputGetStateHook), (PVOID)XamInputGetStateHook);
	//CL_CreateNewCommandsDetour->setupDetour(decryptDWORD(Security->addrs.CL_CreateNewCommandsHook), (PVOID)CL_CreateNewCommandsHook);
	//CL_SendCommandDetour->setupDetour(decryptDWORD(Security->addrs.CL_SendCommandHook), (PVOID)CL_SendCommandHook);
	//CG_EntityEventDetour->setupDetour(decryptDWORD(Security->addrs.CG_EntityEventHook), (PVOID)CG_EntityEventHook);
	//R_AddCmdDrawStretchPicDetour->setupDetour(decryptDWORD(Security->addrs.R_AddCmdDrawStretchPic), (PVOID)R_AddCmdDrawStretchPicHook);
	//SV_ExecuteClientCommandDetour->setupDetour(decryptDWORD(Security->addrs.SV_ExecuteClientCommandHook), (PVOID)SV_ExecuteClientCommandHook);
	CG_CalcEntityLerpPositionsDetour->setupDetour(decryptDWORD(Security->addrs.CG_CalcEntityLerpPositionsHook), (PVOID)CG_CalcEntityLerpPositionsHook);
	// 
	//CL_DispatchConnectionlessPacketDetour->setupDetour(decryptDWORD(Security->addrs.CL_DispatchConnectionlessPacketHook), (PVOID)CL_DispatchConnectionlessPacketHook);
	//CL_FindRouteForClientNumDetour->setupDetour(0x822529E0, (PVOID)CL_FindRouteForClientNumHook);

	MSG_WriteByteDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteByte), (PVOID)MSG_WriteByteHook);
	MSG_WriteShortDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteShort), (PVOID)MSG_WriteShortHook);
	MSG_WriteLongDetour->setupDetour(decryptDWORD(Security->addrs.MSG_WriteLong), (PVOID)MSG_WriteLongHook);
	memcpyDetour->setupDetour(decryptDWORD(Security->addrs.memcpyHook), (PVOID)memcpyHook);
#endif

#endif
	
	//memcpyDetour->setupDetour(0x827D33F0, (PVOID)memcpyHook);
}

template<typename T>
void UnloadDetour(detour<T>* detour) {
	if (detour && detour->IsValid()) {
		detour->takeDownDetour();
		delete detour;
	}
}

void Hooking::UnloadHooks() {
	UnloadDetour(SND_EndFrameDetour);
	UnloadDetour(XamInputGetStateDetour);
	UnloadDetour(CL_CreateNewCommandsDetour);
	UnloadDetour(CL_SendCommandDetour);
	UnloadDetour(LiveStats_Probation_GiveProbationDetour);
	UnloadDetour(CL_DispatchConnectionlessPacketDetour);
	UnloadDetour(CG_BoldGameMessageDetour);
	UnloadDetour(CG_GameMessageDetour);
	UnloadDetour(CG_EntityEventDetour);
	UnloadDetour(PreGameAntiFreezeDetour);
	UnloadDetour(sub_821D8A18Detour);
	UnloadDetour(DynEntCl_DestroyEventDetour);
	UnloadDetour(XMemCpyDetour);
	UnloadDetour(memcpyDetour);
	UnloadDetour(LUI_Interface_DrawRectangleDetour);
	UnloadDetour(R_AddCmdDrawStretchPicDetour);
	UnloadDetour(SV_ExecuteClientCommandDetour);
	UnloadDetour(CG_CalcEntityLerpPositionsDetour);
	UnloadDetour(CL_FindRouteForClientNumDetour);
	UnloadDetour(BG_GetWeaponDefDetour);
	UnloadDetour(MSG_WriteByteDetour);
	UnloadDetour(MSG_WriteShortDetour);
	UnloadDetour(MSG_WriteLongDetour);
	UnloadDetour(CXgi__SessionSearchByIDDtr);
	UnloadDetour(GetAcceptedGameInviteInfoDtr);

	//MSG_WriteByteDetour.takeDownDetour();
	//MSG_WriteShortDetour.takeDownDetour();
	//MSG_ReadByteDetour.takeDownDetour();
	//MSG_ReadShortDetour.takeDownDetour();


	/*if (SND_EndFrameDetour->IsValid())
		SND_EndFrameDetour->takeDownDetour();

	if (XamInputGetStateDetour->IsValid())
		XamInputGetStateDetour->takeDownDetour();

	if (CL_CreateNewCommandsDetour->IsValid())
		CL_CreateNewCommandsDetour->takeDownDetour();

	if (CL_SendCommandDetour->IsValid())
		CL_SendCommandDetour->takeDownDetour();

	if (LiveStats_Probation_GiveProbationDetour->IsValid())
		LiveStats_Probation_GiveProbationDetour->takeDownDetour();

	if (CL_DispatchConnectionlessPacketDetour->IsValid())
		CL_DispatchConnectionlessPacketDetour->takeDownDetour();

	if (CG_BoldGameMessageDetour->IsValid())
		CG_BoldGameMessageDetour->takeDownDetour();

	if (CG_GameMessageDetour->IsValid())
		CG_GameMessageDetour->takeDownDetour();

	if (CG_EntityEventDetour->IsValid())
		CG_EntityEventDetour->takeDownDetour();

	if (PreGameAntiFreezeDetour->IsValid())
		PreGameAntiFreezeDetour->takeDownDetour();

	if (sub_821D8A18Detour->IsValid())
		sub_821D8A18Detour->takeDownDetour();

	if (DynEntCl_DestroyEventDetour->IsValid())
		DynEntCl_DestroyEventDetour->takeDownDetour();

	if (XMemCpyDetour->IsValid())
		XMemCpyDetour->takeDownDetour();

	if (memcpyDetour->IsValid())
		memcpyDetour->takeDownDetour();

	if (LUI_Interface_DrawRectangleDetour->IsValid())
		LUI_Interface_DrawRectangleDetour->takeDownDetour();

	delete SND_EndFrameDetour;
	delete XamInputGetStateDetour;
	delete CL_CreateNewCommandsDetour;
	delete CL_SendCommandDetour;
	delete LiveStats_Probation_GiveProbationDetour;
	delete CL_DispatchConnectionlessPacketDetour;
	delete CG_BoldGameMessageDetour;
	delete CG_GameMessageDetour;
	delete CG_EntityEventDetour;
	delete PreGameAntiFreezeDetour;
	delete sub_821D8A18Detour;
	delete DynEntCl_DestroyEventDetour;
	delete XMemCpyDetour;
	delete memcpyDetour;
	delete LUI_Interface_DrawRectangleDetour;*/
}