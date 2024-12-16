#include "stdafx.h"

char* SpinbotTypesX[3] = { "Look Up", "Look Down", "Up & Down"};
char* SpinbotTypesY[4] = { "Anti Aim", "Random", "Left", "Right" };

vec2 spinAngles = vec2();

int Spinbot::FindBestAntiAimTarget()
{
	FLOAT fClosestDistance = FLT_MAX;
	int iAntiAimClient = -1;
	// Find the highest priority client, taking into consideration if the player is preferred or not
	for (int i = 0; i < CG::dwEntityNum; i++) {
		if (i == Structs::get_cg()->clientNumber)
			continue;

		if (!Game::IsPlayer(i))
			continue;

		if (!Game::IsAlive(i))
			continue;

		if (!Game::IsEnemy(i))
			continue;

		if (i != Structs::get_cg()->clientNumber && !pInfo[i].Whitelisted) {
			if (iAntiAimClient != -1) {
				// If we already have a client selected, check if they are preferred. If they are, make sure we don't change our anti-aim target for a non-preferred player
				if (pInfo[iAntiAimClient].Prioritized && !pInfo[i].Prioritized) {
					continue;
				}
			}

			FLOAT TempDistance = Structs::get_centity(Structs::get_cg()->clientNumber).Origin.distanceTo(Structs::get_centity(i).Origin);

			if (TempDistance < fClosestDistance || (iAntiAimClient != -1 && pInfo[i].Prioritized && !pInfo[iAntiAimClient].Prioritized)) {
				fClosestDistance = TempDistance;
				iAntiAimClient = i;
			}
		}
	}
	return iAntiAimClient;
}

void Spinbot::HandleSpinbot(usercmd_s* currentCmd) {
	// If we aren't prone, not mantling, not dead, not climbing a ladder, not pressing LB or RB

#if defined (BO2)
	if ((*(BYTE*)((DWORD)Structs::get_cg() + 0x18F) & 0x8) || (Structs::get_cg()->Health <= 0) || currentCmd->weapon == 0 || currentCmd->buttons & 0x00010000 || currentCmd->buttons & 0x00020000)
		return;
#elif defined (BO1)
	if ((*(int*)((int)Structs::get_cg() + 0x170) & 0x8) || (*(int*)((int)Structs::get_cg() + 0x254) <= 0) || (currentCmd->buttons & 0x00010000) || (currentCmd->buttons & 0x00020000) || (currentCmd->weapon == 0))
		return;
#elif defined (BO1_ZM)

#endif

	if (MenuOptions::bAntiAim) {
#if defined (BO2)
		int AAClient = FindBestAntiAimTarget();
		if (AAClient != -1) {
			vec3 viewOrigin = Game::GetPlayerViewOrigin();

			vec3 pos = Game::GetTagPos(&Structs::get_centity(AAClient), Game::CacheBone("j_mainroot"));
			vec3 subtracted = pos - viewOrigin;

			vec3 angle;
			Utilities::VectorToAngle(&subtracted, &angle);

			spinAngles.y = (angle.y - 180.0f) - Structs::get_cg()->playerState.deltaAngles.y;
			currentCmd->ViewAngles[1] = ANGLE2SHORT(spinAngles.y);
		}

		spinAngles.x = -69.99f - Structs::get_cg()->playerState.deltaAngles.x;
		currentCmd->ViewAngles[0] = ANGLE2SHORT(spinAngles.x);

#elif defined (BO1) || defined (BO1_ZM)
		spinAngles.x += 7.5f;
		if (spinAngles.x > 69.99f)
			spinAngles.x = -69.99f;

		if (spinAngles.y > 360.f)
			spinAngles.y -= 360.f;

		spinAngles.y += 25.0f;

		spinAngles.x = (spinAngles.x - Structs::get_cg()->playerState.deltaAngles.x);
		spinAngles.y = (spinAngles.y - Structs::get_cg()->playerState.deltaAngles.y);

		currentCmd->ViewAngles[0] = ANGLE2SHORT(spinAngles.x);
		currentCmd->ViewAngles[1] = ANGLE2SHORT(spinAngles.y);
#endif
	}
}