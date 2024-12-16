#include "stdafx.h"

Aimbot cAimbot;

int aimbotClient = -1;

double Radians(float Degrees) {
	return  0.01745329238474369 * Degrees;
}

void Aimbot::GetWeaponSpread(int* seed, float* x, float* y) {
#if defined (BO2)
	float min, max, spread, fx, fy;
	int randSeed = *seed;
	Game::BG_GetSpreadForWeapon(Structs::get_centity(Structs::get_cg()->clientNumber).weaponNum, &min, &max);
	spread = (max - min) * (Structs::get_cg()->SpreadScale * 0.00392156862) + min;
	fx = Radians(RandomBulletDir(&randSeed) * 360);
	sub_82697FC0(&randSeed);
	fy = RandomBulletDir(&randSeed);
	*x = cos(fx) * fy * spread;
	*y = sin(fx) * fy * spread;
#elif defined (BO1)
	float flSpread, Spread, FinalSpread, minSpread, maxSpread, fx, fy;
	int randSeed = *seed;

	fx = Radians(RandomBulletDir(&randSeed) * 360);
	fy = RandomBulletDir(&randSeed);

	//flSpread = *(float*)((DWORD)cg + 0x69264);
	flSpread = Structs::get_cg()->playerState.aimSpreadScale;
	Spread = (flSpread * 0.00392156862);
	Game::BG_GetSpreadForWeapon(Structs::get_centity(Structs::get_cg()->clientNumber).weaponNum, &minSpread, &maxSpread);
	FinalSpread = minSpread + ((maxSpread - minSpread) * Spread);

	*x = cos(fx) * fy * FinalSpread;
	*y = sin(fx) * fy * FinalSpread;
#elif defined (BO1_ZM)
	float flSpread, Spread, FinalSpread, minSpread, maxSpread, fx, fy;
	int randSeed = *seed;

	fx = Radians(RandomBulletDir(&randSeed) * 360);
	fy = RandomBulletDir(&randSeed);

	flSpread = Structs::get_cg()->playerState.aimSpreadScale;
	Spread = (flSpread * 0.00392156862);

	Game::BG_GetSpreadForWeapon(Structs::get_cg()->playerState.weaponId, &minSpread, &maxSpread);
	FinalSpread = minSpread + ((maxSpread - minSpread) * Spread);

	*x = cos(fx) * fy * FinalSpread;
	*y = sin(fx) * fy * FinalSpread;
#endif
}

void FirstBulletFix() {
	cg_s* cg = Structs::get_cg();
	clientActive_s* clientActive = Structs::get_clientActive();

	float v3, v4, v5, v6;
#if defined (BO2)

	v3 = *(float*)((int)cg + 0x682F8);

	if (*(float*)((int)cg + 0x698CC) != 0.0)
		v3 = *(float*)((int)cg + 0x698CC) * *(float*)((int)cg + 0x682F8);

	*(float*)((int)clientActive + 0x2BF4) = v3;

	if (*(float*)((int)cg + 0x48288) == 1.0f) {

		float v7 = (*(float*)((int)cg + 0x697FC) * 0.25f);
		float v8 = (*(float*)((int)cg + 0x69800) * 0.25f);

		v4 = (*(float*)((int)cg + 0x69804) + *(float*)((int)cg + 0x697F8) * 0.25f);
		v5 = *(float*)((int)cg + 0x69808) + (float)v7;
		v6 = *(float*)((int)cg + 0x6980C) + (float)v8;
	}
	else {
		v4 = *(float*)((int)cg + 0x697F8) + *(float*)((int)cg + 0x69804);
		v5 = *(float*)((int)cg + 0x69808) + *(float*)((int)cg + 0x697FC);
		v6 = *(float*)((int)cg + 0x6980C) + *(float*)((int)cg + 0x69800);
	}

	*(float*)((int)clientActive + 0x2C00) = v4;
	*(float*)((int)clientActive + 0x2C04) = v5;
	*(float*)((int)clientActive + 0x2C08) = v6;

	*(int*)((int)clientActive + 0x2BE8) = *(int*)((int)cg + 0x696A4);
	*(int*)((int)clientActive + 0x2BEC) = *(int*)((int)cg + 0x696FC);

	*(int*)((int)clientActive + 0x35A8) = *(int*)((int)cg + 0x80B60);
	*(int*)((int)cg + 0x80B60) = 0;
#elif defined (BO1)
	v3 = *(float*)((int)cg + 0x5D07C);

	if (*(float*)((int)cg + 0x5D940) != 0.0)
		v3 = *(float*)((int)cg + 0x5D940) * *(float*)((int)cg + 0x5D07C);

	*(float*)((int)clientActive + 0x27C8) = v3;

	v4 = *(float*)((int)cg + 0x5D870) + *(float*)((int)cg + 0x5D87C);
	v5 = *(float*)((int)cg + 0x5D874) + *(float*)((int)cg + 0x5D880);
	v6 = *(float*)((int)cg + 0x5D878) + *(float*)((int)cg + 0x5D884);

	*(float*)((int)clientActive + 0x27D4) = v4;
	*(float*)((int)clientActive + 0x27D8) = v5;
	*(float*)((int)clientActive + 0x27DC) = v6;

	*(int*)((int)clientActive + 0x27BC) = *(int*)((int)cg + 0x5D744);
	*(int*)((int)clientActive + 0x27C0) = *(int*)((int)cg + 0x5D77C);

	*(int*)((int)clientActive + 0x2804) = *(int*)((int)cg + 0x69248);
	*(int*)((int)cg + 0x69248) = 0;
	*(int*)((int)cg + 0x6924C) = 0;
#endif
}

void Aimbot::NoSpread(usercmd_s* nextCmd, usercmd_s* currentCmd) {
	FirstBulletFix();

	vec3 SpreadAngles = vec3();

	int Seed = nextCmd->serverTime;

#if !defined (BO1) && !defined (BO1_ZM)
	TransformSeed(&Seed);
#endif
	GetWeaponSpread(&Seed, &SpreadAngles.x, &SpreadAngles.y);

	currentCmd->ViewAngles[0] -= ANGLE2SHORT(-SpreadAngles.y);
	currentCmd->ViewAngles[1] -= ANGLE2SHORT(-SpreadAngles.x);
}

void Aimbot::FixMovement(usercmd_s* pCmd, float CurAngle, float OldAngle, float fOldForward, float fOldSidemove) {
	float deltaView = CurAngle - OldAngle;
	int forward = (int)(cos(deltaView * (PI / 180.0f)) * fOldForward + cos((deltaView + 90.f) * (PI / 180.0f)) * fOldSidemove);
	int right = (int)(sin(deltaView * (PI / 180.0f)) * fOldForward + sin((deltaView + 90.f) * (PI / 180.0f)) * fOldSidemove);

	if (forward < -128) forward = -128;
	else if (forward > 127) forward = 127;
	if (right < -128) right = -128;
	else if (right > 127) right = 127;

	pCmd->forwardmove = (char)forward;
	pCmd->sidemove = (char)right;
}

void Aimbot::NormalAim(vec3 toAngle) {
	clientActive_s* clientActive = Structs::get_clientActive();

	if (clientActive) {
		clientActive->viewAngles.x = toAngle.x - clientActive->spawnAngles.x;
		clientActive->viewAngles.y = toAngle.y - clientActive->spawnAngles.y;
	}
}

void Aimbot::SilentAim(usercmd_s* oldCommand, vec3 toAngle) {
	clientActive_s* clientActive = Structs::get_clientActive();

	if (clientActive) {
		float oldAngle = SHORT2ANGLE(oldCommand->ViewAngles[1]);

		oldCommand->ViewAngles[0] = ANGLE2SHORT(toAngle.x - clientActive->spawnAngles.x);
		oldCommand->ViewAngles[1] = ANGLE2SHORT(toAngle.y - clientActive->spawnAngles.y);

		FixMovement(oldCommand, SHORT2ANGLE(oldCommand->ViewAngles[1]), oldAngle, oldCommand->forwardmove, oldCommand->sidemove);
	}
}

void Aimbot::ApplyPrediction(int clientNum, vec3 origin) {
	vec3 trDelta = (Structs::get_centity(clientNum).GetNextPredictedPosition() - Structs::get_centity(clientNum).GetPreviousPredictedPosition());

	origin.x = ((origin.x + trDelta.x) * (Structs::get_cg()->snap->Ping * 0.001f));
	origin.y = ((origin.y + trDelta.y) * (Structs::get_cg()->snap->Ping * 0.001f));
	origin.z = ((origin.z + trDelta.z) * (Structs::get_cg()->snap->Ping * 0.001f));
}

int Aimbot::GetClosestClient() {
	int dwClosestClient = -1;
	float closestDistance = FLT_MAX;

#if defined (BO2) || defined (BO1)
	for (int i = 0; i < CG::dwEntityNum; i++) {
#elif defined (BO1_ZM)
	for (int i = 0; i < 1024; i++) {
#endif
		pInfo[i].Wallbangable = false;
		pInfo[i].Visible = false;
		pInfo[i].Bone = nullptr;

		if (i == Structs::get_cg()->clientNumber)
			continue;

#if defined (BO2) || defined (BO1)
		if (!Game::IsPlayer(i))
			continue;

		if (CG::bIsZombies) {
			if (*(byte*)((DWORD)&Structs::get_centity(i) + 0x155) == 0x4)
				continue;

			if (*(byte*)((DWORD)&Structs::get_centity(i) + 0x157) == 0x20)
				continue;
		}

		if (!Game::IsAlive(i))
			continue;

		if (!Game::IsEnemy(i))
			continue;

		if (pInfo[i].Whitelisted)
			continue;
#elif defined (BO1_ZM)
		if (Structs::get_centity(i).eType != 16)
			continue;

		if (!Game::IsAlive(i))
			continue;
#endif

		pInfo[i].Bone = Array_TargetBone[MenuOptions::dwTargetBone];

//#ifndef BO1
//		if (centity[i].weaponNum == CG_GetWeaponIndexForName("riotshield_mp")) {
//			bool bLeftAnkle = (Game::IsClientVisible(i, Game::CacheBone("j_ankle_le")) || Autowall::IsPlayerPenetrable(i, Game::CacheBone("j_ankle_le")));
//			bool bRightAnkle = (Game::IsClientVisible(i, Game::CacheBone("j_ankle_ri")) || Autowall::IsPlayerPenetrable(i, Game::CacheBone("j_ankle_ri")));
//
//			if (!bLeftAnkle && !bRightAnkle) {
//				continue;
//			}
//			else {
//				if (bLeftAnkle)
//					pInfo[i].Bone = "j_ankle_le";
//
//				if (bRightAnkle)
//					pInfo[i].Bone = "j_ankle_ri";
//			}
//		}
//#endif


#if defined (BO2) || defined (BO1)
		if (MenuOptions::dwAimbotType == 1) {
			pInfo[i].Visible = Game::IsClientVisible(i, Game::CacheBone(pInfo[i].Bone));
			if (!pInfo[i].Visible) {
				pInfo[i].Wallbangable = Autowall::IsPlayerPenetrable(i, Game::CacheBone(pInfo[i].Bone));
				if (!pInfo[i].Wallbangable)
					continue;
			}
		}
		else {
			pInfo[i].Visible = Game::IsClientVisible(i, Game::CacheBone(pInfo[i].Bone));
			if (!pInfo[i].Visible)
				continue;
		}
#elif defined (BO1_ZM)
		pInfo[i].Visible = Game::IsClientVisible(i, Game::CacheBone(pInfo[i].Bone));
		if (!pInfo[i].Visible)
			continue;
#endif

		FLOAT TempDistance = Structs::get_centity(Structs::get_cg()->clientNumber).Origin.distanceTo(Structs::get_centity(i).Origin);
		if (pInfo[i].Prioritized) {
			TempDistance = 0.0001f;
		}

		if (TempDistance < closestDistance)
		{
			dwClosestClient = i;
			closestDistance = TempDistance;
		}
	}
	return dwClosestClient;
}

void Aimbot::HandleAimbot(usercmd_s* currentCmd, usercmd_s* nextCmd) {
	if (MenuOptions::bAimbot) {
		aimbotClient = GetClosestClient();
		if (aimbotClient != -1) {
			vec3 viewOrigin = Game::GetPlayerViewOrigin();

			if (viewOrigin.IsNull()) return;

			if (pInfo[aimbotClient].Bone == nullptr) return;

			vec3 bonePosition = Game::GetTagPos(&Structs::get_centity(aimbotClient), Game::CacheBone(pInfo[aimbotClient].Bone));

			if (MenuOptions::bPingPrediction)
				ApplyPrediction(aimbotClient, bonePosition);

			if (bonePosition.IsNull()) return;

			vec3 subtracted = (bonePosition - viewOrigin);

			vec3 angle;
			Utilities::VectorToAngle(&subtracted, &angle);

			if (MenuOptions::dwTargetType == 0) {
				NormalAim(angle);
			}
			else if (MenuOptions::dwTargetType == 1) {
				SilentAim(currentCmd, angle);
			}

			if (MenuOptions::bNoSpread)
				NoSpread(nextCmd, currentCmd);

			if (MenuOptions::bAutoShoot && !Game::ClientUsingRiotShield(Structs::get_cg()->clientNumber, false)) {
				nextCmd->buttons |= 0x80000000;
				if (currentCmd->buttons & 0x80000000) {
					currentCmd->buttons &= 0x7FFFFFFFu;
				}
			}
		}
	}
	else {
		aimbotClient = -1;
	}

	XINPUT_STATE Controller;
	XInputGetState(0, &Controller);

	if (Controller.Gamepad.bRightTrigger > 35) {
		if (!MenuOptions::bAutoShoot)
		{
			nextCmd->buttons |= 0x80000000;
			if (currentCmd->buttons & 0x80000000) {
				currentCmd->buttons &= 0x7FFFFFFFu;
			}
		}
	}
}