#include "stdafx.h"

#if defined (BO2)

weaponDef_s* get_weapon_def(int weaponId) {
	weaponDef_s* weaponDef = nullptr;

	jmp_buf jWeaponDef;
	int ret2 = setjmp(jWeaponDef);
	if (!ret2) {
		try {
			weaponDef = (weaponDef_s*)BG_GetWeaponDef(Structs::get_centity(Structs::get_cg()->clientNumber).weaponNum);
		}
		catch (...) {
			longjmp(jWeaponDef, 420);
		}
	}

	return weaponDef;
}

void Bullet_FirePenetrate(int weaponId, BulletFireParams* bp, BulletTraceResults* br, centity_s* centity)
{
	weaponDef_s* weapDef = get_weapon_def(weaponId);

	if (weapDef == nullptr)
		return;

	bool hasFMJ = BG_WeaponHasPerk(weaponId, 6);

	float bulletMultiplier = hasFMJ ? 2.0f : 1.0f;
	float fxDist = 30.0f;

	if (!BulletTrace(0, bp, centity, br, 0, false))
		return;

	if (weapDef->penetrateType <= 0)
		return;

	if ((Trace_GetEntityHitId(&br->Trace) & 0xFFFF) == bp->iIgnoreEntIndex) {
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		float frontSurfaceDepth = BG_GetSurfacePenetrationDepth(weapDef->penetrateType, br->surfaceType) * bulletMultiplier;

		if (br->Trace.surfaceFlags & 4)
			frontSurfaceDepth = 100.0f;

		if (frontSurfaceDepth <= 0.0f)
			break;

		vec3 firstHitPos = br->hitPos;
		vec3 savedLastHitPos = br->hitPos;

		if (!BG_AdvanceTrace(bp, br, 0.13500000536441803f))
			break;

		bool frontWallTrace = BulletTrace(0, bp, centity, br, br->surfaceType, false);

		BulletFireParams backWallBp;
		memcpy(&backWallBp, bp, sizeof BulletFireParams);

		backWallBp.vDirection.x = bp->vDirection.x * -1.0f;
		backWallBp.vDirection.y = bp->vDirection.y * -1.0f;
		backWallBp.vDirection.z = bp->vDirection.z * -1.0f;

		backWallBp.vStart = bp->vEnd;

		backWallBp.vEnd.x = (backWallBp.vDirection.x * 0.009999999776482582f) + savedLastHitPos.x;
		backWallBp.vEnd.y = (backWallBp.vDirection.y * 0.009999999776482582f) + savedLastHitPos.y;
		backWallBp.vEnd.z = (backWallBp.vDirection.z * 0.009999999776482582f) + savedLastHitPos.z;

		BulletTraceResults backWallBr;
		memcpy(&backWallBr, br, sizeof BulletTraceResults);

		backWallBr.Trace.normal.x = backWallBr.Trace.normal.x * -1.0f;
		backWallBr.Trace.normal.y = backWallBr.Trace.normal.y * -1.0f;
		backWallBr.Trace.normal.z = backWallBr.Trace.normal.z * -1.0f;

		if (frontWallTrace)
			BG_AdvanceTrace(&backWallBp, &backWallBr, 0.009999999776482582f);

		bool backWallTrace = BulletTrace(0, &backWallBp, centity, &backWallBr, backWallBr.surfaceType, false);
		bool staticModel = (backWallTrace) && backWallBr.Trace.allsolid || br->Trace.startsolid && backWallBr.Trace.startsolid;

		if (backWallTrace || staticModel)
			goto next;

		if (!frontWallTrace)
			break;

		if (br->Trace.allsolid)
			continue;

		float x = (br->hitPos.x - firstHitPos.x);
		float y = (br->hitPos.y - firstHitPos.y);
		float z = (br->hitPos.z - firstHitPos.z);
		float length = ((x * x) + (y * y), (z * z));

		if (length <= (fxDist * fxDist))
			continue;

		if (weapDef->weapType != 0)
			continue;

	next:
		float totalDepth = 0.0f;

		if (staticModel)
			totalDepth = (backWallBp.vEnd - backWallBp.vStart).getLength();
		else
			totalDepth = (backWallBr.hitPos - savedLastHitPos).getLength();

		if (totalDepth < 1.0f)
			totalDepth = 1.0f;

		if (backWallTrace)
		{
			float backSurfaceDepth = BG_GetSurfacePenetrationDepth(weapDef->penetrateType, backWallBr.surfaceType);

			frontSurfaceDepth = min(frontSurfaceDepth, backSurfaceDepth) * bulletMultiplier;

			if (backWallBr.Trace.surfaceType & 4)
				frontSurfaceDepth = 200.0f;

			if (frontSurfaceDepth <= 0.0f)
				break;
		}

		float dmg = bp->fDamageMultiplier - (totalDepth / frontSurfaceDepth);
		bp->fDamageMultiplier = dmg;

		if (dmg <= 0.0f)
			break;

		if (staticModel || weapDef->weapType != 0)
			continue;

		float x2 = (backWallBr.hitPos.x - firstHitPos.x);
		float y2 = (backWallBr.hitPos.y - firstHitPos.y);
		float z2 = (backWallBr.hitPos.z - firstHitPos.z);
		float length2 = ((x2 * x2) + (y2 * y2) + (z2 * z2));

		if (length2 <= (fxDist * fxDist))
			continue;

		if (frontWallTrace)
			continue;
	}
}
#endif

bool Autowall::IsPlayerPenetrable(int clientNum, short tag) {
	vec3 end = Game::GetTagPos(&Structs::get_centity(clientNum), tag);

	vec3 viewOrigin = Game::GetPlayerViewOrigin();

	BulletFireParams bulletFireParams;
	BulletTraceResults bulletTraceResults;

	bulletFireParams.iIgnoreEntIndex = Structs::get_cg()->clientNumber;
	bulletFireParams.iWeaponEntIndex = 0x3FE;
	bulletFireParams.fDamageMultiplier = 1.0f;
	bulletFireParams.iMethodOfDeath = 2;
	bulletFireParams.vOrigStart = viewOrigin;
	bulletFireParams.vStart = viewOrigin;
	bulletFireParams.vEnd = end;

	vec3 endDirection = end - viewOrigin;

	vec3 angle;
	Utilities::VectorToAngle(&endDirection, &angle);

	vec3 hax;
	Utilities::AngleVectors(angle, &hax, nullptr, nullptr);

	bulletFireParams.vDirection = hax;

#if defined (BO2)
	// Call FireBulletPenetrate
	Bullet_FirePenetrate(Structs::get_centity(Structs::get_cg()->clientNumber).weaponNum, &bulletFireParams, &bulletTraceResults, &Structs::get_centity(Structs::get_cg()->clientNumber));
#elif defined (BO1)
	* (DWORD*)decryptDWORD(Security->addrs.Autowall[0]) = 0x4E800020;
	*(DWORD*)decryptDWORD(Security->addrs.Autowall[1]) = 0x4E800020;
	*(DWORD*)decryptDWORD(Security->addrs.Autowall[2]) = 0x60000000;
	*(DWORD*)decryptDWORD(Security->addrs.Autowall[3]) = 0x60000000;

	FireBulletPenetrate(0, &bulletFireParams, Structs::get_centity(Structs::get_cg()->clientNumber).weaponNum, &Structs::get_centity(Structs::get_cg()->clientNumber), viewOrigin, 0, 1, &bulletTraceResults);

	*(DWORD*)decryptDWORD(Security->addrs.Autowall[0]) = 0x7D8802A6;
	*(DWORD*)decryptDWORD(Security->addrs.Autowall[1]) = 0x7D8802A6;
	*(DWORD*)decryptDWORD(Security->addrs.Autowall[2]) = 0x409A0008;
	*(DWORD*)decryptDWORD(Security->addrs.Autowall[3]) = 0x4BFE2CD9;
#endif

#if defined (BO2)
	if (!CG::bIsZombies) {
		// Check if the trace hit a shield
		if (bulletTraceResults.Trace.partGroup == 0x14) {
			return false;
		}
	}
#endif

	return (bulletFireParams.iIgnoreEntIndex == clientNum);
}