#include "stdafx.h"

float GetBoxWidth(int value)
{
	if (value & 0x8)
		return 35;
	else return 17;
}

float GetBoxHeight(int value)
{
	if (value & 0x8)
		return 25;
	else if (value & 0x4)
		return 55;
	else return 70;
}

void CalcBoundingBox(int index, color Color)
{
	vec2 corner2, corner3, corner4, corner5, corner6, corner7, vMins, vMaxes;

	vec3 Mins(999999.0f, 999999.0f, 999999.0f), Maxes(-999999.0f, -999999.0f, -999999.0f);

	vec3 BonePos[14];
	for (int t = 0; t < ARRAYSIZE(espBoneNames) - 1; t++)
	{
		vec3* Bones;
		BonePos[t] = Game::GetTagPos(&Structs::get_centity(index), Game::CacheBone(espBoneNames[t]));
		Bones = &BonePos[t];

		if (Bones->x < Mins.x)
			Mins.x = Bones->x;
		if (Bones->y < Mins.y)
			Mins.y = Bones->y;
		if (Bones->z < Mins.z)
			Mins.z = Bones->z;
		if (Bones->x > Maxes.x)
			Maxes.x = Bones->x;
		if (Bones->y > Maxes.y)
			Maxes.y = Bones->y;
		if (Bones->z > Maxes.z)
			Maxes.z = Bones->z;
	}

	vec3 crnr2 = vec3(Maxes.x, Mins.y, Mins.z);
	vec3 crnr3 = vec3(Maxes.x, Mins.y, Maxes.z);
	vec3 crnr4 = vec3(Mins.x, Mins.y, Maxes.z);
	vec3 crnr5 = vec3(Mins.x, Maxes.y, Maxes.z);
	vec3 crnr6 = vec3(Mins.x, Maxes.y, Mins.z);
	vec3 crnr7 = vec3(Maxes.x, Maxes.y, Mins.z);
	vec3 mins = vec3(Mins);
	vec3 maxes = vec3(Maxes);

	int checksPassed = 0;
	checksPassed += !Game::WorldToScreen(crnr2, corner2);
	checksPassed += !Game::WorldToScreen(crnr3, corner3);
	checksPassed += !Game::WorldToScreen(crnr4, corner4);
	checksPassed += !Game::WorldToScreen(crnr5, corner5);
	checksPassed += !Game::WorldToScreen(crnr6, corner6);
	checksPassed += !Game::WorldToScreen(crnr7, corner7);
	checksPassed += !Game::WorldToScreen(mins, vMins);
	checksPassed += !Game::WorldToScreen(maxes, vMaxes);

	if (checksPassed == 0)
	{
		//mins to 2, 4, and 6
		Render::DrawLine(vMins.x, vMins.y, corner2.x, corner2.y, 1.0f, Color, Sunset::dwWhite);
		Render::DrawLine(vMins.x, vMins.y, corner4.x, corner4.y, 1.0f, Color, Sunset::dwWhite);
		Render::DrawLine(vMins.x, vMins.y, corner6.x, corner6.y, 1.0f, Color, Sunset::dwWhite);
		//maxes to 3, 5, and 7
		Render::DrawLine(vMaxes.x, vMaxes.y, corner3.x, corner3.y, 1.0f, Color, Sunset::dwWhite);
		Render::DrawLine(vMaxes.x, vMaxes.y, corner5.x, corner5.y, 1.0f, Color, Sunset::dwWhite);
		Render::DrawLine(vMaxes.x, vMaxes.y, corner7.x, corner7.y, 1.0f, Color, Sunset::dwWhite);
		//from 2 to 3 and 7
		Render::DrawLine(corner2.x, corner2.y, corner3.x, corner3.y, 1.0f, Color, Sunset::dwWhite);
		Render::DrawLine(corner2.x, corner2.y, corner7.x, corner7.x, 1.0f, Color, Sunset::dwWhite);
		//from 4 to 3 and 5
		Render::DrawLine(corner4.x, corner4.y, corner3.x, corner3.y, 1.0f, Color, Sunset::dwWhite);
		Render::DrawLine(corner4.x, corner4.y, corner5.x, corner5.y, 1.0f, Color, Sunset::dwWhite);
		//from 6 to 5 and 7
		Render::DrawLine(corner6.x, corner6.y, corner5.x, corner5.y, 1.0f, Color, Sunset::dwWhite);
		Render::DrawLine(corner6.x, corner6.y, corner7.x, corner7.y, 1.0f, Color, Sunset::dwWhite);
	}
}

void Visuals::DrawESPBox(color color, int Index, int currentType) {
	float fHeight, fWidth;
	vec3 HeadPos;
	vec2 FeetLocation, HeadLocation;

	if (currentType == BOX_TYPE_CORNERS) //Corners
	{
		HeadPos = Game::GetTagPos(&Structs::get_centity(Index), Game::CacheBone("j_head"));

		if (Game::WorldToScreen(Structs::get_centity(Index).Origin, FeetLocation) && Game::WorldToScreen(HeadPos, HeadLocation))
		{
			fHeight = FeetLocation.y - HeadLocation.y;
			if (*(int*)((DWORD)&Structs::get_centity(Index) + 0x1D4) & 0x08)
				fWidth = fHeight / 0.9;
			else
				fWidth = fHeight / 2;

			Render::DrawBox((FeetLocation.x - (fWidth / 2)), (FeetLocation.y - fHeight), (fWidth / 4), 1, color);
			Render::DrawBox((FeetLocation.x - (fWidth / 2)), FeetLocation.y, (fWidth / 4), 1, color);
			Render::DrawBox((FeetLocation.x - (fWidth / 2)), (FeetLocation.y - fHeight), 1, (fHeight / 4), color);
			Render::DrawBox((FeetLocation.x + (fWidth / 2)), (FeetLocation.y - fHeight), 1, (fHeight / 4), color);
			Render::DrawBox((FeetLocation.x + ((fWidth / 2) - (fWidth / 4))), (FeetLocation.y - fHeight), (fWidth / 4), 1, color);
			Render::DrawBox((FeetLocation.x + ((fWidth / 2) - (fWidth / 4))), FeetLocation.y, (fWidth / 4), 1, color);
			Render::DrawBox((FeetLocation.x - (fWidth / 2)), (FeetLocation.y - (fHeight / 4)), 1, (fHeight / 4), color);
			Render::DrawBox((FeetLocation.x + (fWidth / 2)), (FeetLocation.y - (fHeight / 4)), 1, (fHeight / 4), color);

		}
	}
	else if (currentType == BOX_TYPE_2D || BOX_TYPE_2D_FILLED) //2D
	{

		vec3 MainRoot, Feet;
		vec2 MainRootOut, FeetOut;
		MainRoot = Game::GetTagPos(&Structs::get_centity(Index), Game::CacheBone("j_mainroot"));
		MainRoot.z += 10;

		int BoxHeight = 55; // 8 or 0x0A = prone|| 4 or 6 = crouched||0 or 2 = standing
		if (*(int*)((DWORD)&Structs::get_centity(Index) + 0x1D4) & 0x08)
			BoxHeight = 17;
		else if (*(int*)((DWORD)&Structs::get_centity(Index) + 0x1D4) & 0x04)
			BoxHeight = 35;

		Feet.x = MainRoot.x; Feet.y = MainRoot.y; Feet.z = (MainRoot.z - -BoxHeight - 10);

		if (Game::WorldToScreen(MainRoot, MainRootOut) == TRUE && Game::WorldToScreen(Feet, FeetOut) == TRUE)
		{
			FLOAT Height = (MainRootOut.y - FeetOut.y);
			FLOAT Width = (Height / 3.0f);

			if (currentType == BOX_TYPE_2D_FILLED) {
				R_AddCmdDrawStretchPic((MainRootOut.x - Width), (MainRootOut.y - (Height * 0.5f)), Width * 2.0f, Height * 1.5f, 0.0f, 0.0f, 0.0f, 0.0f, color, Sunset::dwWhite);
			}
			else {
				Render::DrawLine((int)(MainRootOut.x - Width), (int)(MainRootOut.y - Height * 0.5f), (int)(MainRootOut.x - Width), (int)(MainRootOut.y + Height), 1.0f, color, Sunset::dwWhite); // Left side line
				Render::DrawLine((int)(MainRootOut.x - Width), (int)(MainRootOut.y - Height * 0.5f), (int)(MainRootOut.x + Width), (int)(MainRootOut.y - Height * 0.5f), 1.0f, color, Sunset::dwWhite); // Bottom line
				Render::DrawLine((int)(MainRootOut.x + Width), (int)(MainRootOut.y - Height * 0.5f), (int)(MainRootOut.x + Width), (int)(MainRootOut.y + Height), 1.0f, color, Sunset::dwWhite); // Right side line
				Render::DrawLine((int)(MainRootOut.x - Width), (int)(MainRootOut.y + Height), (int)(MainRootOut.x + Width), (int)(MainRootOut.y + Height), 1.0f, color, Sunset::dwWhite); // Top line
			}
		}
	}
	else if (currentType == BOX_TYPE_3D) //3D
	{
		CalcBoundingBox(Index, color);
	}
}

void Visuals::DrawBones(int clientNum, color color) {
	vec3 TagPos[2];
	vec2 TagScreen[2];
	for (int b = 0; b < ARRAYSIZE(bonesESP) - 1; b++)
	{
		TagPos[0] = Game::GetTagPos(&Structs::get_centity(clientNum), Game::CacheBone(bonesESP[b]));
		TagPos[1] = Game::GetTagPos(&Structs::get_centity(clientNum), Game::CacheBone(bonesESP[b + 1]));

		if (TagPos[0] == vec3() || TagPos[1] == vec3())
			return;

		if (Game::WorldToScreen(TagPos[0], TagScreen[0]) && Game::WorldToScreen(TagPos[1], TagScreen[1]))
			Render::DrawLine(TagScreen[0].x, TagScreen[0].y, TagScreen[1].x, TagScreen[1].y, 2.0f, color, Sunset::dwWhite);
	}
}

void Visuals::DrawESP(int clientNum, color colorESP) {
	DrawESPBox(colorESP, clientNum, MenuOptions::dwBoxType);

	vec2 FeetLocation, HeadLocation;

	if (Game::WorldToScreen(Structs::get_centity(clientNum).Origin, FeetLocation) && Game::WorldToScreen(Game::GetTagPos(&Structs::get_centity(clientNum), Game::CacheBone("j_head")), HeadLocation)) {
		float fontSize = 0.8f;

		if (MenuOptions::bDrawNames) {
#if defined (BO1_ZM)
			char* nameTag = "Zombie";

			if (Structs::get_centity(clientNum).eType == 1) {
				nameTag = va("%s", Structs::get_cg()->clientInfo[clientNum].clientName);
			}

#elif defined (BO1)

			//char* nameTag = CG::bIsZombies ? "Zombie" : va("%s", Structs::get_cg()->clientInfo[clientNum].clientName);
			char* nameTag = nameTag = va("%s", Structs::get_cg()->clientInfo[clientNum].clientName);
#else
			char* nameTag = "Zombie";

			if (Structs::get_centity(clientNum).eType == 1) {
				nameTag = va("%s", Structs::get_cg()->clientInfo[clientNum].clientName);
			}
#endif

			if (nameTag && strlen(nameTag) > 0) {
				Render::DrawString(nameTag, FeetLocation.x, HeadLocation.y - 3, Sunset::dwSmallFont, 0.7f, colorESP, JustifyCenter);
			}
		}

		if (MenuOptions::bDrawDistances) {
			Render::DrawString(va("%.1fm", Utilities::GetDistance(Structs::get_cg()->refDef.viewOrigin, Structs::get_centity(clientNum).Origin) / 100), FeetLocation.x, FeetLocation.y + 18, Sunset::dwSmallFont, 0.7f, colorESP, JustifyCenter);
		}

		if (MenuOptions::bDrawBones) {
			DrawBones(clientNum, colorESP);
		}

		if (!CG::bIsZombies) {
			if (MenuOptions::bDrawWeapons) {
				float aspectX, aspectY;

				int WeaponIcon = Game::GetWeaponIcon(Structs::get_centity(clientNum).weaponNum, &aspectX, &aspectY);
				if (WeaponIcon != 0 && MmIsAddressValid((DWORD*)WeaponIcon)) {
					CG_DrawRotatedPicPhysical(decryptDWORD(Security->addrs.ScreenPlacement), FeetLocation.x - (aspectX / 2), FeetLocation.y + (aspectY / 2) + 6, aspectX, aspectY, 0.0, colorESP, WeaponIcon);
				}
			}
		}

		if (MenuOptions::bDrawSnaplines)
			Render::DrawLine((Structs::get_cg()->refDef.Width / 2), Sunset::dwScreenMaxY, FeetLocation.x, FeetLocation.y, 1, colorESP, Sunset::dwWhite);
	}
}

void Visuals::HandleDebugInfo() {
	int Index = 0;

	Render::DrawTopRightText(ENGINE_NAME, Structs::get_uiContext().screenWidth - 10.0f, (Structs::get_uiContext().screenHeight - 715.0f), Sunset::dwSmallFont, Sunset::fOptionScale, MenuOptions::bSunsetMenuColorFade ? colorMainMenuFade : colorRed, Index);
	Index++;

	CHAR pFPSBuff[10];
	Render::DrawTopRightText(va("FPS: %s", itoa(Game::CalculateFPS(), pFPSBuff, 10)), Structs::get_uiContext().screenWidth - 10.0f, (Structs::get_uiContext().screenHeight - 715.0f), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite, Index);
	Index++;

	//if (Game::CheckInGame() && CG::bLobbyInitialized) {
	//	if (MmIsAddressValid((PVOID)Structs::get_cg()->nextSnap->Ping)) {
	//		Render::DrawTopRightText(va("Ping: %i", Structs::get_cg()->nextSnap->Ping), Structs::get_uiContext().screenWidth - 10.0f, (Structs::get_uiContext().screenHeight - 715.0f), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite, Index);
	//		Index++;
	//	}
	//}

#if defined (BO2)
	PCHAR HostName = Dvar_GetString("party_hostname");
#elif defined (BO1) || defined (BO1_ZM)
	PCHAR HostName = Dvar_GetString(decryptDWORD(Security->addrs.party_hostname));
#endif

	char firstChar = HostName[0];

	if (firstChar != '^' || strstr(HostName, "^")) {
		Render::DrawTopRightText(va("Host: %s", HostName), Sunset::dwScreenMaxX - 10.0f, (Sunset::dwScreenMaxY - 715.0f), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite, Index);
		Index++;
	}


//#if defined (DEBUG_BUILD) && defined (BO1)
//	int cgs_t = *(int*)0x82964E64;
//	if (cgs_t != 0) {
//		int gameEndTime = *(int*)(cgs_t + 0x198);
//
//		int timeLeft = (gameEndTime - Structs::get_cg()->nextSnap->serverTime);
//
//
//		int seconds = timeLeft / 1000;
//		timeLeft %= 1000;
//
//		int minutes = seconds / 60;
//		seconds %= 60;
//
//		Render::DrawTopRightText(va("Time Left: %im, %is", minutes, seconds), Sunset::dwScreenMaxX - 10.0f, (Sunset::dwScreenMaxY - 715.0f), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite, Index);
//		Index++;
//	}
//#endif
}

int dwTimer = 0;

void Visuals::HandleVisuals() {
	Structs::UpdateStructures();

	if (MenuOptions::bDebugInfo)
		HandleDebugInfo();

	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		for (int i = 0; i < CG::dwEntityNum; i++) {
			if (Structs::get_cg()->clientNumber != i && Structs::get_centity(i).ClientNumber != Structs::get_cg()->clientNumber) {
				if (!Game::IsPlayer(i))
					continue;

				if (CG::bIsZombies) {
#if defined (BO2)
					if (*(byte*)((DWORD)&Structs::get_centity(i) + 0x155) == 0x4)
						continue;

					if (*(byte*)((DWORD)&Structs::get_centity(i) + 0x157) == 0x20)
						continue;
#endif
				}

				if (!Game::IsAlive(i))
					continue;

				bool bIsEnemy = Game::IsEnemy(i);
				color espColor = colorEnemy;

				if (bIsEnemy) {
					if (pInfo[i].Prioritized)
						espColor = colorPrioritized;
					else if (pInfo[i].Whitelisted)
						espColor = colorWhitelisted;
					else if (pInfo[i].Wallbangable)
						espColor = colorWallbangable;
					else if (pInfo[i].Visible)
						espColor = colorVisible;
					else
						espColor = colorEnemy;
				}
				else {
					espColor = colorFriendly;
				}

				if ((MenuOptions::bDrawAllies && !bIsEnemy) || (MenuOptions::bDrawAxis && bIsEnemy)) {
					DrawESP(i, espColor);
				}
			}
		}

		//if (MenuOptions::bDrawAxis) {
		//	for (int i = 0; i < 1024; i++) {
		//		if (cg->clientNumber != i && centity[i].ClientNumber != cg->clientNumber) {
		//			if (!Game::IsAlive(i))
		//				continue;

		//			if (centity[i].eType != 16)
		//				continue;

		//			DrawESP(i, colorRed);
		//		}
		//	}
		//}

		if (MenuOptions::bSmallCrosshair) {
			if (!Sunset::bMenuOpen) {
				Render::DrawLine((Structs::get_cg()->refDef.Width / 2) - 6, Structs::get_cg()->refDef.Height / 2, (Structs::get_cg()->refDef.Width / 2) + 6, Structs::get_cg()->refDef.Height / 2, 2, colorWhite, Sunset::dwWhite);
				Render::DrawLine(Structs::get_cg()->refDef.Width / 2, (Structs::get_cg()->refDef.Height / 2) - 6, Structs::get_cg()->refDef.Width / 2, (Structs::get_cg()->refDef.Height / 2) + 6, 2, colorWhite, Sunset::dwWhite);
			}
		}

		if (MenuOptions::bHealthBar) {
			float fBarWidth = 200.0f, fBarHeight = 30.0f;
			float center_bottom_X = (Sunset::dwScreenCenterX - (fBarWidth / 2));
			float bottom_Y = ((Sunset::dwScreenMaxY - 9.0f) - fBarHeight);

			float HealthbarLength;

			if (HealthbarLength != (Game::GetLocalClientHealth() * (fBarWidth / 100)))
			{
				HealthbarLength = (Game::GetLocalClientHealth() * (fBarWidth / 100));
			}

			Render::DrawShader(center_bottom_X, bottom_Y, fBarWidth, fBarHeight, Sunset::dwWhite, colorBackground);
			Render::DrawShader((center_bottom_X + 3.0f), (bottom_Y + 3.0f), (HealthbarLength - 6.0f), (fBarHeight - 6.0f), Sunset::dwWhite, colorRed);

#if defined (BO2)
			float text_bottomY = (bottom_Y + 25.0f);
#elif defined (BO1) || defined (BO1_ZM)
			float text_bottomY = (bottom_Y + 27.0f);
#endif
			Render::DrawString(va("Health %i/%i", Game::GetLocalClientHealth(), Game::GetLocalClientMaxHealth()), (center_bottom_X + (fBarWidth / 2)), text_bottomY, Sunset::dwSmallFont, Sunset::fMenuTitleScale, colorWhite, JustifyCenter);
		}
	}
}