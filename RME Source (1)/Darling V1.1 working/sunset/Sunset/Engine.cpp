#include "stdafx.h"

BYTE bSunsetInfoKey[0x10] = { 0xBA, 0xB1, 0x2A, 0x55, 0xEE, 0xFC, 0xF4, 0x62, 0x73, 0x8A, 0xB9, 0x6D, 0xA0, 0xDE, 0xE3, 0x7C };

DWORD SupportedVersion = 17559;
bool bWasSignedIn = false;

//bool bLocalClientAlive = false;
//bool bJustSpawned = false;
//bool bSetGodMode = false;

DWORD dwEnteredIngame = 0;

bool bWasDead = false;

bool bShouldSetAntiQuit = false;

#if defined (BO2)
DWORD TitleID = 0x415608C3;
#elif defined (BO1) || defined (BO1_ZM)
DWORD TitleID = 0x41560855;
#endif

BOOL AddressesSet = FALSE;
PBYTE Addresses_Org = nullptr;

detour<void> ExTerminalTitleProcessDetour;

void ExTerminateTitleProcessHook(int r3) {
	Global.UnloadEngine = TRUE;

	ExTerminalTitleProcessDetour.callOriginal(NULL);
}

void GameThread() {
	while (XamGetCurrentTitleId() != TitleID)
		Sleep(100);

	PLDR_DATA_TABLE_ENTRY entry;
	XexPcToFileHeader((PVOID)0x82000000, &entry);
#if defined (BO1) || defined (BO2)
	if (wcscmp(entry->BaseDllName.Buffer, L"default_mp.xex") == 0) {
#elif defined (BO1_ZM)
	if (wcscmp(entry->BaseDllName.Buffer, L"default.xex") == 0) {
#endif
		PBYTE buf = (PBYTE)XPhysicalAlloc(0x2000, MAXULONG_PTR, 128, MEM_LARGE_PAGES | PAGE_READWRITE);
		mManager = new MemoryManager((DWORD)buf, 0x2000);
		mManager->alloc(sizeof(cSecurity), &Security);

		Global.Initialize();

		while (!Global.g_killThread) {
			if (Security->bHasPatches) {
				Sunset::bSignedIntoLive = XUserGetSigninState(0) == eXUserSigninState_SignedInToLive;
				if (Sunset::bSignedIntoLive && !bWasSignedIn) {
					bWasSignedIn = true;
					Game::ResetGamertag();
				}
				else if (!Sunset::bSignedIntoLive) {
					bWasSignedIn = false;
					Game::ResetGamertag();
				}

				if (!Global.g_Hooked) {
					INI_Manager::Load_INI();
					Sleep(1000);
#if defined (DEBUG_BUILD)
					Hooking::LoadHooks();
#else
					if (Shield_CheckStatus()) {
						Hooking::LoadHooks();
					}
#endif
					ExTerminalTitleProcessDetour.setupDetour((DWORD)Utilities::ResolveFunction(MODULE_KERNEL, 0x1A), (PVOID)ExTerminateTitleProcessHook);
					Global.g_Hooked = TRUE;
				}

				if (Sunset::bSunsetSave) {
					INI_Manager::Save_INI();
					Sunset::bSunsetSave = FALSE;
				}

				if (MenuOptions::bLaunchCustomGT) {
					MenuOptions::bLaunchCustomGT = FALSE;

					wchar_t wcOldGT[33];
					wchar_t wcGT[33];
					XOVERLAPPED XOver;
					ZeroMemory(&XOver, sizeof(XOVERLAPPED));

					size_t convertedChars = 0;
					mbstowcs_s(&convertedChars, wcOldGT, strlen(OriginalGT) + 1, OriginalGT, _TRUNCATE);

					XShowKeyboardUI(0, VKBD_HIGHLIGHT_TEXT, wcOldGT, L"Set Gamertag", L"Please enter a gamertag you'd like to set.", wcGT, 33, &XOver);

					while (!XHasOverlappedIoCompleted(&XOver)) Sleep(25);

					Sleep(100);

					if (wcGT[0] != 0 && wcGT[1] != 0)
					{
						wcscpy_s(wcOldGT, wcGT);
						wcstombs(OriginalGT, wcGT, 33);

						Game::SetIngameInfo("", OriginalGT);

						Game::SetGamertag(OriginalGT);
					}
				}

				if (MenuOptions::bEditCustomGT) {
					MenuOptions::bEditCustomGT = FALSE;

					// Setup our variables, and clear everything out before we begin
					XOVERLAPPED xOvl;
					WCHAR wResult[33], wInitialText[33];
					wmemset(wResult, 0, 33);
					wmemset(wInitialText, 0, 33);
					ZeroMemory(&xOvl, sizeof(XOVERLAPPED));

					size_t convertedChars = 0;
					mbstowcs_s(&convertedChars, wInitialText, strlen((PCHAR)pModdedGamertags[SelectedGamertagIndex]) + 1, (PCHAR)pModdedGamertags[SelectedGamertagIndex], _TRUNCATE);

					// Launch the virtual keyboard UI
					XShowKeyboardUI(0, VKBD_DEFAULT | VKBD_HIGHLIGHT_TEXT, wInitialText, L"Edit Custom Gamertag", L"Please enter a gamertag you'd like to set.", wResult, 100, &xOvl);

					// Wait for the user to close the keyboard
					while (!XHasOverlappedIoCompleted(&xOvl)) {
						Sleep(33);
					}

					if (xOvl.dwExtendedError == ERROR_SUCCESS) {
						CHAR gamertag[33];
						wcstombs(gamertag, wResult, 33);
						if (strlen(gamertag) >= 1 && strlen(gamertag) <= 32) {
							strcpy(pModdedGamertags[SelectedGamertagIndex], gamertag);
						}
					}
				}

				if (MenuOptions::bCustomRenameAllClients) {
					MenuOptions::bCustomRenameAllClients = FALSE;

					wchar_t wcOldGT[33];
					wchar_t wcGT[33];
					XOVERLAPPED XOver;
					ZeroMemory(&XOver, sizeof(XOVERLAPPED));
					XShowKeyboardUI(0, VKBD_HIGHLIGHT_TEXT, wcOldGT, L"Set Gamertag", L"Please enter a gamertag you'd like to set for all clients.", wcGT, 33, &XOver);

					while (!XHasOverlappedIoCompleted(&XOver)) Sleep(25);

					Sleep(100);

					if (wcGT[0] != 0 && wcGT[1] != 0)
					{
						wcscpy_s(wcOldGT, wcGT);
						wcstombs(cCustomRenameAll, wcGT, 33);

						RenameAll(cCustomRenameAll);
					}
				}

				if (MenuOptions::bRenameSelectedClient) {
					MenuOptions::bRenameSelectedClient = FALSE;

					wchar_t wcOldGT[33];
					wchar_t wcGT[33];
					XOVERLAPPED XOver;
					ZeroMemory(&XOver, sizeof(XOVERLAPPED));
					XShowKeyboardUI(0, VKBD_HIGHLIGHT_TEXT, wcOldGT, L"Set Gamertag", L"Please enter a gamertag you'd like to set this player.", wcGT, 33, &XOver);

					while (!XHasOverlappedIoCompleted(&XOver)) Sleep(25);

					Sleep(100);

					if (wcGT[0] != 0 && wcGT[1] != 0)
					{
						wcscpy_s(wcOldGT, wcGT);
						wcstombs(cCustomRenameClient, wcGT, 33);

						RenameSelectedClient(SelectedPlayer, cCustomRenameClient);
					}
				}

#if defined (DEBUG_BUILD)
				if (Sunset::DumpBuffer) {
					//DbgPrint("weaponId: 0x%X", Structs::get_cg()->playerState.weaponId);
					//DbgPrint("Index: 0x%X", CG_GetWeaponIndexForName("m1911_zm"));

					//int weapFlags = 0x00001000;

					//weapFlags &= 0xFFFFFFFC;

					//DbgPrint("weapFlags: 0x%X", weapFlags);


					Sunset::DumpBuffer = FALSE;
				}
#endif

				if (MenuOptions::bRME_GodMode) {
					if (Game::CheckInGame() && CG::bLobbyInitialized) {
						Structs::UpdateStructures();

						if (!Game::IsAlive(Structs::get_cg()->clientNumber)) {
							if (!bWasDead) {
								//DbgPrint("[Godmode] Reset!");
								bWasDead = true;
							}
						}
						else {
							if (bWasDead) {
								Sleep(500);

								ChangeGodMode(-1, true);
								//DbgPrint("[Godmode] Set!");

								bWasDead = false;
							}
						}
					}
					else {
						if (!bWasDead) {
							//DbgPrint("[Godmode] Not Ingame!");
							bWasDead = true;
						}
					}
				}

				if (MenuOptions::bAntiQuitAuto) {
					if (Game::CheckInGame() && CG::bLobbyInitialized) {
						if (bShouldSetAntiQuit) {
							Sleep(500);

							//DbgPrint("[ANTI QUIT] Set!");

							AntiQuit(true);

							bShouldSetAntiQuit = false;
						}
					}
					else {
						if (!bShouldSetAntiQuit) {
							//DbgPrint("[ANTI QUIT] Not Ingame!");
							bShouldSetAntiQuit = true;
						}
					}
				}

				if (Global.UnloadEngine) {
					Utilities::Notify("Sunset BETA - Unloading!");
					Global.g_killThread = TRUE;
				}

				if (MenuOptions::bMakeEveryoneThrowNades) {
					ThrowGrenadesForEveryone();
				}

				//if (MenuOptions::bMoveEveryoneAround) {
				//	MoveEveryoneAround();
				//}

				//if (MenuOptions::bWalkOnCountdown) {
				//	//gClient(iClientNum) + 0x5684

				//	if (Game::CheckInGame() && CG::bLobbyInitialized) {
				//		if (dwEnteredIngame == 0) {
				//			dwEnteredIngame = GetTickCount();
				//			DbgPrint("Catched enter time!");
				//		}
				//		else {
				//			if (GetTickCount() - dwEnteredIngame < 3000) {
				//				if (*(int*)(gClient(Structs::get_cg()->clientNumber) + 0x5684) == 0x04) { //spawned and frozen controls
				//					FreezeMovement(-1, false);
				//					dwEnteredIngame = 0;
				//					DbgPrint("Disabled movement freeze!");
				//				}
				//			}
				//		}
				//	}
				//	else {
				//		if (dwEnteredIngame != 0) {
				//			dwEnteredIngame = 0;
				//			DbgPrint("Reset enter time!");
				//		}
				//	}
				//}

				if (gRME_Options.size() > 0) {
					for (int i = 0; i < gRME_Options.size(); i++) {
						if (gRME_Options[i].dwAddress != 0x00) {

							//if (gRME_Options[i].dwValue != 0x00) {
							//	if (gRME_Options[i].bAllowNullValue) {
									//char buf[4] = { 0 };
									//memcpy(buf, &gRME_Options[i].dwValue, 4);
									//g_Netchan.write(gRME_Options[i].dwAddress, buf, 4);
							//	}
							//}
							if ((gRME_Options[i].dwValue != 0x00 && !gRME_Options[i].bAllowNullValue) || (gRME_Options[i].dwValue == 0 && gRME_Options[i].bAllowNullValue)) {
								char buf[4] = { 0 };
								memcpy(buf, &gRME_Options[i].dwValue, 4);
								g_Netchan.write(gRME_Options[i].dwAddress, buf, 4);
							}
							else if ((gRME_Options[i].sValue != 0x00 && !gRME_Options[i].bAllowNullValue) || (gRME_Options[i].sValue == 0 && gRME_Options[i].bAllowNullValue)) {
								char buf[2] = { 0 };
								memcpy(buf, &gRME_Options[i].sValue, 2);
								g_Netchan.write(gRME_Options[i].dwAddress, buf, 2);
							}
							else if ((gRME_Options[i].bValue != 0x00 && !gRME_Options[i].bAllowNullValue) || (gRME_Options[i].bValue == 0 && gRME_Options[i].bAllowNullValue)) {
								char buf[1] = { 0 };
								memcpy(buf, &gRME_Options[i].bValue, 1);
								g_Netchan.write(gRME_Options[i].dwAddress, buf, 1);
							}
							else if ((gRME_Options[i].qwValue != 0x00 && !gRME_Options[i].bAllowNullValue) || (gRME_Options[i].qwValue == 0 && gRME_Options[i].bAllowNullValue)) {
								char buf[8] = { 0 };
								memcpy(buf, &gRME_Options[i].qwValue, 8);
								g_Netchan.write(gRME_Options[i].dwAddress, buf, 8);
							}
							else if ((gRME_Options[i].fValue != 0.0f && !gRME_Options[i].bAllowNullValue) || (gRME_Options[i].fValue == 0.0f && gRME_Options[i].bAllowNullValue)) {
								char buf[4] = { 0 };
								memcpy(buf, &gRME_Options[i].fValue, 4);
								g_Netchan.write(gRME_Options[i].dwAddress, buf, 4);
							}
							else if (gRME_Options[i].vValue != vec3(0, 0, 0)) {
								char buf[12] = { 0 };
								memcpy(buf, &gRME_Options[i].vValue, sizeof(vec3));
								g_Netchan.write(gRME_Options[i].dwAddress, buf, sizeof(vec3));
							}
							else if (gRME_Options[i].data != "") {
								g_Netchan.write(gRME_Options[i].dwAddress, (char*)gRME_Options[i].data.data(), gRME_Options[i].data.length() + 1);
							}

							Sleep(100);
						}

						//DbgPrint("found function");
						gRME_Options.erase(gRME_Options.begin() + i);
					}
				}

				Sleep(100);
			}
			else {
#ifdef DEBUG_BUILD
				Security->SetAddresses(NULL);
				DbgPrint("Security->SetAddresses");

				Security->DumpPatchBinary();
#else
				if (AddressesSet) {
					Security->SetAddresses(Addresses_Org);
					free(Addresses_Org);
				}
#endif
			}
		}

		if (Global.g_Hooked) {
			Hooking::UnloadHooks();
			Global.g_Hooked = FALSE;
		}

#ifndef DEBUG_BUILD
		Security->DissolveAddresses();
#endif

		Sleep(500);
	}
	ExTerminalTitleProcessDetour.takeDownDetour();
	Global.g_MainLoopExited = true;

	if (Global.UnloadEngine) {
		*(WORD*)((DWORD)Global.hME + 0x40) = 1;
		DWORD exitCode;
		GetExitCodeThread(GetCurrentThread(), &exitCode);
		XexUnloadImageAndExitThread(Global.hME, (HANDLE)exitCode);
	}
}

DWORD flags = EX_CREATE_FLAG_SUSPENDED | EX_CREATE_FLAG_SYSTEM | EX_CREATE_FLAG_CORE2 | EX_CREATE_FLAG_PRIORITY1;
void HiddenThreadStartup() {
	if (XboxKrnlVersion->Build != SupportedVersion)
		return;

	flags |= 0x18000424;

	HANDLE Handle;
	ExCreateThread(&Handle, 0, 0, XapiThreadStartup, (LPTHREAD_START_ROUTINE)GameThread, 0, flags);
	XSetThreadProcessor(Handle, 4);
	SetThreadPriority(Handle, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(Handle);
	CloseHandle(Handle);
	srand((int)time(NULL));
}


HRESULT InitializeEngine() {
	Global.hKernel = GetModuleHandle(MODULE_KERNEL);
	Global.hXam = GetModuleHandle(MODULE_XAM);
	Global.hXBDM = GetModuleHandle(MODULE_XBDM);

	//if (Global.hShield == NULL)
	//	Global.hShield = GetModuleHandle("Shield.xex");


	if (Global.hME == 0 || Global.hKernel == 0 || Global.hXam == 0 || Global.hXBDM == 0 /*|| Global.hShield == 0*/) {
		return E_FAIL;
	}

	//if (!hypervisor::InitializePeekPoke())
	//	return E_FAIL;

	DWORD startFunc = Utilities::GetModuleImportAddr(Global.hME, Global.hKernel, 0x19);
	if (!startFunc)
		return E_FAIL;

#ifndef DEBUG_BUILD
	*(DWORD*)startFunc = (DWORD)HiddenThreadStartup;
#else
	CG::bAllowUsingZombies = TRUE;
	//CG::bAllowExtras = TRUE;

	for (int i = 0; i < sizeof(SunsetInfo_s); i++) {
		*(BYTE*)((DWORD)&Global.sunsetInfo + i) = 0xAA;
	}

	//Shield_CheckStatus = (bool(*)())Utilities::ResolveFunctionFromHandle(Global.hShield, 1);

	HiddenThreadStartup();
#endif

	return ERROR_SUCCESS;
}


extern "C" {
	__declspec(dllexport) void InitalizeAddrs(PBYTE InvokerSection, HANDLE hShield) {
		DWORD dwLink;
		__asm {
			mflr r31
			mr dwLink, r31
		}

		Global.hShield = hShield;
		memset(&Global.sunsetInfo, 0x00, sizeof(SunsetInfo_s));

		DWORD AddrsPtr = NULL;
		DWORD AddrsSize = NULL;

		BYTE SecondaryCipherKey[0x10] = { 0x00 };

		memcpy(&AddrsPtr, InvokerSection, 0x4);
		memcpy(&AddrsSize, (InvokerSection + 0x4), 0x4);
		memcpy(SecondaryCipherKey, (InvokerSection + 0x8), 0x10);

		memcpy(&Global.sunsetInfo, (InvokerSection + 0x18), sizeof(SunsetInfo_s));

		XeCryptRc4(bSunsetInfoKey, 0x10, (PBYTE)&Global.sunsetInfo, sizeof(SunsetInfo_s));

		BYTE NameDigest[0x14] = { 0x00 };

		PLDR_DATA_TABLE_ENTRY entry = (PLDR_DATA_TABLE_ENTRY)Global.hME;
		XeCryptSha((PBYTE)entry->BaseDllName.Buffer, 0x10, NULL, NULL, NULL, NULL, NameDigest, XECRYPT_SHA_DIGEST_SIZE);

		XeCryptRc4(SecondaryCipherKey, 0x10, (PBYTE)AddrsPtr, AddrsSize);
		XeCryptRc4(NameDigest, 0x14, (PBYTE)AddrsPtr, AddrsSize);

		Addresses_Org = (PBYTE)malloc(AddrsSize);

		memcpy(Addresses_Org, (PBYTE)AddrsPtr, AddrsSize);

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwAllowZombies))
			CG::bAllowUsingZombies = TRUE;

		Shield_CheckStatus = (bool(*)())Utilities::ResolveFunctionFromHandle(hShield, 1);

		AddressesSet = TRUE;

		ZeroMemory(InvokerSection, 0x38);
		ZeroMemory(SecondaryCipherKey, 0x10);
		ZeroMemory(NameDigest, 0x14);
		AddrsPtr = 0x00;
		AddrsSize = 0x00;
		entry = NULL;
		return;
	}
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		Global.hME = hModule;
		Global.EnginePDLR = (PLDR_DATA_TABLE_ENTRY)hModule;

		if (InitializeEngine() != ERROR_SUCCESS) {
			*(WORD*)((DWORD)hModule + 64) = 1;
			return FALSE;
		}
		else {
#ifndef  DEBUG_BUILD
			Utilities::RemoveFromList(Global.hME);
#endif
			return TRUE;
		}
		break;
	case DLL_PROCESS_DETACH:
		Global.g_killThread = TRUE;
		while (!Global.g_MainLoopExited)
			Sleep(10);

		break;
	}
	return TRUE;
}