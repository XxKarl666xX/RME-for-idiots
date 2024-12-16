#pragma once

extern int aimbotClient;

class Aimbot {
public:
	static void GetWeaponSpread(int* seed, float* x, float* y);
	static void NoSpread(usercmd_s* nextCmd, usercmd_s* currentCmd);
	static void FixMovement(usercmd_s* pCmd, float CurAngle, float OldAngle, float fOldForward, float fOldSidemove);
	static void NormalAim(vec3 toAngle);
	static void SilentAim(usercmd_s* oldCommand, vec3 toAngle);
	static void ApplyPrediction(int clientNum, vec3 origin);
	static int GetClosestClient();
	static void HandleAimbot(usercmd_s* currentCmd, usercmd_s* nextCmd);
};

extern Aimbot cAimbot;