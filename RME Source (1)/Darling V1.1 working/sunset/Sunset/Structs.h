#pragma once

enum eBoxTypes {
	BOX_TYPE_2D,
	BOX_TYPE_2D_FILLED,
	BOX_TYPE_3D,
	BOX_TYPE_CORNERS
};

enum EntityType {
	ET_GENERAL,
	ET_PLAYER,
	ET_CORPSE,
	ET_ITEM,
	ET_MISSLE,
	ET_INVISIBLE_ENTITY,
	ET_SCRIPTMOVER,
	ET_SOUND_BLEND,
	ET_FX,
	ET_LOOP_FX,
	ET_PRIMARY_LIGHT,
	ET_TURRET,
	ET_HELICOPTER,
	ET_PLANE,
	ET_VEHICLE,
	ET_VEHICLE_COLLMAP,
	ET_VEHICLE_CORPSE,
	ET_VEHICLE_SPAWNER
};

enum sessionState_t {
	SESS_STATE_PLAYING = 0x00,
	SESS_STATE_DEAD = 0x01,
	SESS_STATE_SPECTATOR = 0x02,
	SESS_STATE_INTERMISSION = 0x03
};

struct uiContext_s {
	int contextIndex; // 0x0
	float bias; // 0x4
	int realTime; // 0x8
	int frameTime; // 0xC
	float x; // 0x10
	float y; // 0x14
	int isCursorVisible; // 0x18
	int screenWidth; // 0x1C
	int screenHeight; // 0x20
	float screenAspect; // 0x24
	float fps; // 0x28
	float blurRadiusOut; // 0x2C
};

struct refDef_s {
#if defined (BO2)
	int Width; //0x0000 
	int Height; //0x0004 
	char _0x0008[20];
	vec2 tanHalfFov; //0x001C 
	char _0x0024[12];
	vec3 viewOrigin; //0x0030 
	char _0x003C[4];
	vec3 viewAxis[3]; //0x0040 
	char _0x0064[8];
#elif defined (BO1)
	int Width;//0x00
	int Height;//0x04
	char _0x0008[20];//0x08
	vec2 tanHalfFov;//0x1C
	FLOAT TotalFov;//0x24
	vec3 viewOrigin;//0x28
	char _0x0034[4];//0x34
	vec3 viewAxis[3];//0x38
	__int32 ServerTime;//0x5C
#elif defined (BO1_ZM)
	char _0x00[0x4];
	int Width;//0x04
	int Height;//0x08
	char _0x000C[0x14];
	vec2 tanHalfFov;//0x20
	FLOAT TotalFov;//0x28
	vec3 viewOrigin;//0x2C
	char _0x0038[4];//0x38
	vec3 viewAxis[3];//0x3C
	__int32 ServerTime;//0x60
	//0x64
#endif
};

struct clientInfo_s {
#if defined (BO2)
	int infoValid; //0x0000 
	int nextValid; //0x0004 
	int clientNumber; //0x0008 
	char clientName[32]; //0x000C
	int Team; //0x002C
	char _0x002C[48];
	int Rank; //0x0060 
	int Prestige; //0x0064 
	char _0x0068[0x10];
	char xuid[8];
	char clanAbbrev[8]; //0x0080 
	int Ping; //0x0088 
	int status_icon; //0x008C 
	int Place; //0x0090 
	int Score; //0x0094 
	int Kills; //0x0098 
	int Assists; //0x009C 
	int Deaths; //0x00A0 
	char _0x00A4[32];
	int Headshots; //0x00C4 
	char _0x00C8[4];
	int currentStreak; //0x00CC 
	char _0x00D0[80];
	char SecondaryWeapon[32]; //0x0120 
	char _0x0140[900];
	vec3 viewAngles; //0x04C4 
	char _0x04D0[192];
	int aimingIn; //0x0590 
	char _0x0594[628];
#elif defined (BO1)
	int Valid;//0x00
	BYTE _0x0004[8];//0x08
	CHAR clientName[16];//0x0C
	BYTE _0x001C[16];//0x1C
	int Team;//0x2C
	BYTE _0x0030[52];//0x30
	int isAlive;//0x64
	BYTE _0x0068[0x450 - 0x68];//0x68
	vec3 viewAngles;//0x450
	BYTE _0x045C[0x174];//0x45C
	//0x5D0
#elif defined (BO1_ZM)
	int Valid;//0x00
	BYTE _0x0004[8];//0x08
	CHAR clientName[16];//0x0C
	BYTE _0x001C[16];//0x1C
	int Team;//0x2C
	BYTE _0x0030[52];//0x30
	int isAlive;//0x64
	BYTE _0x0068[0x450 - 0x68];//0x68
	vec3 viewAngles;//0x450
	BYTE _0x045C[0x1A4];//0x45C
	//0x600
#endif
};

struct snapshot_s
{
#if defined (BO2)
	int snapFlags; //0x0000 
	int Ping; //0x0004 
	int serverTime; //0x0008 
	int physicsTime; //0x000C 
	char _0x0010[48];
#elif defined (BO1) || defined (BO1_ZM)
	int snapFlags;//0x00
	int Ping;//0x04
	int serverTime;//0x08
	int physicsTime;//0x0C
#endif
};

struct playerState_s {
	//char _0x0000[0x4C];
	//int weaponTime;
	//int weaponDelay;
	//char _0x0054[0x164];
	//int weapon; //0x01B8
	//char _0x01BC[0x284C];
#if defined (BO2)
	int commandTime; // 0x0
	int pm_type; // 0x4
	int bobCycle; // 0x8
	int pm_flags; // 0xC
	long long weapFlags; // 0x10
	int otherFlags; // 0x18
	int pm_time; // 0x1C
	unsigned int loopSoundId; // 0x20
	int loopSoundFade; // 0x24
	vec3 origin; // 0x28
	vec3 velocity; // 0x34
	int remoteEyesEnt; // 0x40
	int remoteEyesTagname; // 0x44
	int remoteControlEnt; // 0x48
	int weaponTime; // 0x4C
	int weaponDelay; // 0x50
	int weaponTimeLeft; // 0x54
	int weaponDelayLeft; // 0x58
	int weaponIdleTime; // 0x5C
	int grenadeTimeLeft; // 0x60
	int throwBackGrenadeOwner; // 0x64
	int throwBackGrenadeTimeLeft; // 0x68
	int weaponRestrictKickTime; // 0x6C
	char padding[0x1C]; // 0x80
	int gravity; // 0x8C
	float leanf; // 0x90
	int speed; // 0x94
	vec3 deltaAngles; // 0x98
	int groundEntityNum; // 0xA4
	int moverEntityNum; // 0xA8
	int moverTimestamp; // 0xAC
	int groundType; // 0xB0
	vec3 vLadderVec; // 0xB4
	int jumpTime; // 0xC0
	float jumpOriginZ; // 0xC4
	byte padding2[0x34]; // 0xC8
	int eFlags; // 0xFC
	byte padding3[0xA0]; // 0xFF
	int clientNum; // 0x1A0
	int offHandWeaponIndex; // 0x1A4
	byte padding4[0x10]; // 0x1A8
	int weaponId; // 0x1B8
	byte padding5[0x24]; // 0x1BC
	float weaponPosFrac; // 0x1E0
	int adsDelayTime; // 0x1E4
	int spreadOverride; // 0x1E8
	int spreadOverrideState; // 0x1EC
	float weaponSpinLerp; // 0x1F0
	int viewmodelIndex; // 0x1F4
	vec3 viewAngles; // 0x1F8
	int viewHeightTarget; // 0x204
	float viewHeightCurrent; // 0x208
	int viewHeightLerpTime; // 0x20C
	int viewHeightLerpTarget; // 0x210
	int viewHeightLerpDown; // 0x214
	float viewAngleClampBase[2]; // 0x218
	float viewAngleClampRange[2]; // 0x220
	int damageEvent; // 0x228
	int damageYaw; // 0x22C
	int damagePitch; // 0x230
	int damageCount; // 0x234
	int health; // 0x238
	byte padding6[0x4]; // 0x23C
	int maxHealth; // 0x240
	byte padding7[0x1F4]; // 0x244
	int secondaryAmmoCount; // 0x438
	int primaryAmmoCount; // 0x43C
	byte padding8[0x24]; // 0x440
	float proneDirection; // 0x464
	byte padding9[0x84]; // 0x440
	int chargeShotLevel; // 0x4EC
	byte padding10[0x98]; // 0x4F0
	float aimSpreadScale; // 0x588
	//0x480A8
#elif defined (BO1)
	char _0x00[0x14];
	int otherFlags; //0x14
	char _0x0018[0x64];
	float leanf; //0x7C
	float speed; //0x80
	vec3 deltaAngles; //0x84
	int groundEntityNum; //0x90
	char _0x0094[0x4C];
	int eFlags; //0xE0
	char _0x00E4[0x60];
	int weaponId; //0x144
	char _0x0148[0x338];
	int vehiclePos; //0x480
	char _0x0484[0xDC];
	float aimSpreadScale; //0x560
	char _0x0564[0x2184];

	//0x26E8
#elif defined (BO1_ZM) //at cg + 0x8A360
	char _0x00[0x14];
	int otherFlags; //0x14
	char _0x0018[0x6C];
	vec3 deltaAngles; //0x84
	char _0x0090[0xB4];
	int weaponId; //0x144
	char _0x0148[0x3E4];
	float aimSpreadScale; //0x52C
	char _0x530[0x14E4];

    //0x1A14
#endif
}; 

#if defined (BO2)
struct compassData_s {
	char _0x0000[0x24];
};

struct cg_s {
	int clientNumber; //0x0000
	int localClientNum; //0x0004
	char _0x0008[0x20];
	snapshot_s* snap; //0x0028
	char _0x002C[0x8C];
	vec3 Origin; //0x00B8
	char _0x00C4[0x1AC];
	float ZoomState; //0x270
	char _0x0274[0x54];
	int Health; //0x02C8
	char _0x02CC[0x4];
	int maxHealth; //0x02D0
	char _0x02D4[0x47DC8];
	BYTE bThirdPerson; //0x4809C
	char _0x4809D[0xB];
	playerState_s playerState; //0x480A8
	char _0x48638[0x5250];
	refDef_s refDef; //0x4D888
	char _0x4D8F4[0x1BF74];
	compassData_s compassData; //0x69868
	char _0x6988C[0x204];
	clientInfo_s clientInfo[18]; //0x69A90
	char _0x72B20[0xE060];
	float SpreadScale; //0x80B80
	char _0x80B84[0x6B7C];
};
#elif defined (BO1)
struct cg_s {
	int clientNumber;//0x00
	int localClientNumber;//0x04
	BYTE _0x0008[0x28 - 0x8];//0x08
	snapshot_s* snap;//0x28
	snapshot_s* nextSnap;//0x2C
	BYTE _0x0030[0xB4 - 0x30];//0x30
	vec3 Origin;//0xB4
	BYTE _0x00C0[0x3EEDC];//0xC0
	playerState_s playerState; //0x3EF9C
	BYTE _0x41684[0x384];
	refDef_s refDef;//0x41A08
	BYTE _0x41A64[114136];//0x41A64
	DWORD Sway;//0x5D840
	BYTE _0x5D844[692];//0x5D844
	clientInfo_s clientInfo[18];//0x5DAF4
	BYTE _0x6666C[19092];//0x66678

	int GetPlayerState() {
		return ((DWORD)this + 0x3EF9C);
	}
};
#elif defined (BO1_ZM)
struct cg_s {
	//int clientNumber;//0x00
	//int localClientNumber;//0x04
	//BYTE _0x0008[0x28 - 0x8];//0x08
	//snapshot_s* snap;//0x28
	//snapshot_s* nextSnap;//0x2C
	//BYTE _0x0030[0xB4 - 0x30];//0x30
	//vec3 Origin;//0xB4
	//BYTE _0x00C0[268616];//0xC0
	//refDef_s refDef;//0x41A04
	//BYTE _0x41A64[114136];//0x41A64
	//DWORD Sway;//0x5D840
	//BYTE _0x5D844[692];//0x5D844
	//clientInfo_s clientInfo[18];//0x5DAF4
	//BYTE _0x6666C[19092];//0x66678

	//int GetPlayerState() {
	//	return ((DWORD)this + 0x3EF9C);
	//}

	int clientNumber; //0x00
	int localClientNumber; //0x04
	char _0x08[0x28];
	snapshot_s* snap; //0x30
	snapshot_s* nextSnap; //0x34
	char _0x0038[0x30];
	vec3 Origin; //0x68
	char _0x0074[0x8A2EC];
	playerState_s playerState; //0x8A360
	char _0x8BD74[0x380];
	refDef_s refDef; //0x8C0F4
	char _0x8C158[0x2F528];
	clientInfo_s clientInfo[18]; //0xBB680 
	char _0xC2280[0xC2D0];
}; //0xCE550
#endif

#if defined (BO2)
struct usercmd_s {
	int serverTime; //0x0000
	int buttons; //0x0004
	int unk; //0x0008
	int ViewAngles[3]; // 0x000C
	int weapon; //0x0018
	int offHandIndex; //0x001C
	int lastWeaponAltModeSwitch;
	signed char forwardmove;
	signed char sidemove;
	char upmove;
	char pitchmove;
	char yawmove;
	char padding1;
	short faggot;
	float meleeChargeYaw;
	char meleeChargeDist;
	char padding2[3];
	float rollmove;
	char selectedLocation[2];
	char selectedYaw;
	char padding3;
};
#elif defined (BO1) || defined (BO1_ZM)
struct usercmd_s {
	int serverTime;
	int buttons;
	int unk;
	int ViewAngles[3];
	char weapon;
	char unk1;
	short offHandIndex;
	short lastWeaponAltModeSwitch;
	signed char forwardmove;
	signed char sidemove;
	char upmove;
	char pitchmove;
	char yawmove;
	char padding1;
	float meleeChargeYaw;
	char meleeChargeDist;
	char padding2[3];
	float rollmove;
	char selectedLocation[2];
	char selectedYaw;
	char padding3;
};
#endif

#if defined (BO2)
struct clientActive_s {
	char _0x0000[152];
	vec3 Origin; // 0x0098
	char __0x00A4[100];
	vec3 spawnAngles; // 0x0108
	char _0x0114[11128];
	vec3 viewAngles; // 0x2C8C
	char _0x02C9[0x40010];
	usercmd_s cmds[128];
	int cmdNumber;

	inline usercmd_s* getCmd(int id) {
		return &cmds[id & 0x7F];
	}

	inline usercmd_s* getCmd() {
		return getCmd(cmdNumber);
	}

	inline usercmd_s* getLastCmd() {
		return getCmd(cmdNumber - 1);
	}

	inline usercmd_s* getNextCmd() {
		return getCmd(cmdNumber + 1);
	}
};
#elif defined (BO1)
struct clientActive_s {
	char _0x0000[76];
	vec3 Origin; //0x004C 
	char _0x0058[84];
	vec3 spawnAngles; //0x00AC 
	char _0x00B8[10120];
	vec3 viewAngles; //0x2840 
	char _0x284C[0x40010]; //284C
	usercmd_s cmds[128]; //0x285C
	int cmdNumber; //0x425C

	inline usercmd_s* getCmd(int id) {
		return &cmds[id & 0x7F];
	}

	inline usercmd_s* getCmd() {
		return getCmd(cmdNumber);
	}

	inline usercmd_s* getLastCmd() {
		return getCmd(cmdNumber - 1);
	}

	inline usercmd_s* getNextCmd() {
		return getCmd(cmdNumber + 1);
	}
};
#elif defined (BO1_ZM)
struct clientActive_s {
	//char _0x0000[76];
	//vec3 Origin; //0x004C 
	//char _0x0058[84];
	//vec3 spawnAngles; //0x00AC 
	//char _0x00B8[10120];
	//vec3 viewAngles; //0x2840 
	//char _0x284C[0x40010]; //284C
	//usercmd_s cmds[128]; //0x285C
	//int cmdNumber; //0x425C

	char _0x00[0x48];
	vec3 Origin; //0x48
	char _0x54[0x54];
	vec3 spawnAngles; //0xA8
	char _0xB4[0x1AE0];
	vec3 viewAngles; //0x1B94
	char _0x1BA0[0x24];
	usercmd_s cmds[128]; //0x1BC4
	int cmdNumber; //0x35C4

	inline usercmd_s* getCmd(int id) {
		return &cmds[id & 0x7F];
	}

	inline usercmd_s* getCmd() {
		return getCmd(cmdNumber);
	}

	inline usercmd_s* getLastCmd() {
		return getCmd(cmdNumber - 1);
	}

	inline usercmd_s* getNextCmd() {
		return getCmd(cmdNumber + 1);
	}
};
#endif

#if defined (BO2)
struct centity_s
{
	char _0x0000[2];
	char Valid; //0x0002 
	char _0x0003[41];
	vec3 Origin; //0x002C 
	vec3 viewAngles;
	char _0x0038[156];
	char AliveZM; //0x00E0 
	char _0x00E1[115];
	int Flags; //0x0154 
	char _0x0158[16];
	vec3 lastOrigin; //0x0168 
	char _0x0174[92];
	int ClientNumber; //0x01D0 
	char _0x01D4[96];
	int WeaponID2; //0x0234 
	char _0x0238[112];
	short eType; //0x02A8 ;
	char modelIndex; //0x02AC 
	char _0x02AE[6];
	int weaponNum; //0x02B4 
	char _0x02B8[180];
	char isAlive; //0x036C 
	char _0x036D[7];

	vec3 GetNextPredictedPosition() {
		return vec3(
			*(float*)((DWORD)this + 0x168),
			*(float*)((DWORD)this + 0x16C),
			*(float*)((DWORD)this + 0x170)
		);
	}

	vec3 GetPreviousPredictedPosition() {
		return vec3(
			*(float*)((DWORD)this + 0x1E8),
			*(float*)((DWORD)this + 0x1EC),
			*(float*)((DWORD)this + 0x1F0)
		);
	}
};
#elif defined (BO1)
struct centity_s
{
	BYTE _0x0000[28];
	int iGodmodeCheck;
	BYTE _0x0020[16];
	vec3 Origin;
	vec3 viewAngles;
	BYTE _0x003C[0x121];
	BYTE bState1; // 2 = sprinting || 8 = aiming
	BYTE placeholder; //0x16A not actually type just a place holder
	BYTE bState2; // 0x40 = shooting || 0x04 or 0x06 or 0x44 or 0x46 = crouched || 0x0A or 0x4A or 0x8 or 0x48 = prone
	BYTE _0x016C[112];
	int ClientNumber;
	int _0x01E0;
	BYTE _0x01E4[130];
	BYTE bIsAlive;
	BYTE _0x0267[0x3D];
	DWORD weaponNum; //0x2A4
	BYTE _0x02A8[0x70];
	int _0x0318;
	//0x31C

	vec3 GetNextPredictedPosition() {
		return vec3(
			*(float*)((DWORD)this + 0x1F4),
			*(float*)((DWORD)this + 0x1F8),
			*(float*)((DWORD)this + 0x1FC)
		);
	}

	vec3 GetPreviousPredictedPosition() {
		return vec3(
			*(float*)((DWORD)this + 0x17C),
			*(float*)((DWORD)this + 0x180),
			*(float*)((DWORD)this + 0x184)
		);
	}

	short GetType() {
		return *(short*)((DWORD)this + 0x29A);
	}
};
#elif defined (BO1_ZM)
struct centity_s
{
	//BYTE _0x0000[0x1C];
	//int iGodmodeCheck;
	//BYTE _0x0020[0x10];
	//vec3 Origin;
	//vec3 ViewAngles;
	//BYTE _0x003C[0x121];
	//BYTE bState1; // 2 = sprinting || 8 = aiming
	//BYTE placeholder; //0x16A not actually type just a place holder
	//BYTE bState2; // 0x40 = shooting || 0x04 or 0x06 or 0x44 or 0x46 = crouched || 0x0A or 0x4A or 0x8 or 0x48 = prone
	//BYTE _0x016C[112];
	//int ClientNumber;
	//int _0x01E0;
	//BYTE _0x01E4[130];
	//BYTE bIsAlive;
	//BYTE _0x0267[0x3D];
	//DWORD weaponNum;
	//BYTE _0x02A8[0x70];
	//int _0x0318;

	char _0x0000[0x24];
	vec3 Origin; //0x24
	vec3 viewAngles; //0x30
	char _0x003C[0x11C];
	int ClientNumber; //0x158
	char _0x015C[0xBA];
	short eType; //0x216
	char _0x218[0x14];
	int weaponNum; //0x22C
	char _0x230[0xD4];
	int _0x304; //0x304
	char _0x308[0x8];

	vec3 GetNextPredictedPosition() {
		return vec3(
			*(float*)((DWORD)this + 0x170),
			*(float*)((DWORD)this + 0x174),
			*(float*)((DWORD)this + 0x178)
		);
	}

	vec3 GetPreviousPredictedPosition() {
		return vec3(
			*(float*)((DWORD)this + 0x244),
			*(float*)((DWORD)this + 0x248),
			*(float*)((DWORD)this + 0x24C)
		);
	}
};
#endif

struct BulletFireParams {
	int iWeaponEntIndex;
	int iIgnoreEntIndex;
	float fDamageMultiplier;
	int iMethodOfDeath;
	vec3 vOrigStart;
	vec3 vStart;
	vec3 vEnd;
	vec3 vDirection;
};

#if defined (BO2)
struct Trace {
	vec3 normal; // 0x0
	char padding[4]; // 0xC
	float fFraction; // 0x10
	int surfaceType; // 0x14
	int surfaceFlags; // 0x18
	int hitType; // 0x1C
	unsigned short hitId; // 0x20
	unsigned short modelIndex; // 0x22
	unsigned short boneIndex; // 0x24
	unsigned short partName; // 0x26
	unsigned short partGroup; // 0x28
	bool allsolid; // 0x2A
	bool startsolid; // 0x2B
	bool walkable; // 0x2C
	int hitPartition; // 0x30
};

struct BulletTraceResults {
	Trace Trace; // 0x0
	char padding[0xC]; // 0x30
	int hitEnt; // 0x40
	vec3 hitPos; // 0x44
	bool ignoreHitEnt; // 0x50
	int surfaceType; // 0x54
	char padding2[8]; // 0x58
};

#elif defined (BO1) || defined (BO1_ZM)
struct Trace {
	char _0x0000[0x10];
	float fFraction;
	char Buf04[0x60];
};

struct BulletTraceResults {
	Trace Trace;
	int iHitEnt;
	vec3 vHitPos;
	bool bIgnoreHitEnt;
	int iSurfaceType;
};
#endif


#ifndef BO1
struct weaponDef_s {
	byte padding[0x1C]; // 0x0
	int weapType; // 0x1C
	byte padding2[4]; // 0x20
	int penetrateType; // 0x24
	byte padding3[0x585]; // 0x20
	float fHipSpreadStandMin; // 0x5B0
	float fHipSpreadDuckedMin; // 0x5B4
	float fHipSpreadProneMin; // 0x5B8
	float hipSpreadStandMax; // 0x5BC
	float hipSpreadDuckedMax; // 0x5C0
	float hipSpreadProneMax; // 0x5C4
	byte padding4[0x69];
	bool rifleBullet; // 0x631
	bool armorPiercing; // 0x632
	byte padding5[0x28]; // 0x633
	DWORD fireTypeIcon; // 0x65C
	DWORD hudIcon; // 0x660
	int killIconRatio; // 0x664
	byte padding6[0xC7]; // 0x668
	bool bBulletImpactExplode; // 0x72F
};
#endif


struct playerInfo_s {
	BOOL Prioritized;
	BOOL Whitelisted;
	BOOL Visible;
	BOOL Wallbangable;
	char* Bone;
};

enum netadrtype_t
{
	NA_BOT = 0x0,
	NA_BAD = 0x1,
	NA_LOOPBACK = 0x2,
	NA_BROADCAST = 0x3,
	NA_IP = 0x4,
};

enum netsrc_t
{
#if defined (BO2)
	NS_NULL = 0xFFFFFFFF,
	NS_CLIENT1 = 0x0,
	NS_CLIENT2 = 0x1,
	NS_CLIENT3 = 0x2,
	NS_CLIENT4 = 0x3,
	NS_SERVER = 0x4,
	NS_MAXCLIENTS = 0x4,
	NS_PACKET = 0x5
#elif defined (BO1) || defined (BO1_ZM)
	NS_CLIENT1 = 0x00,
	NS_SERVER = 1,
	NS_MAXCLIENTS = 1,
	NS_PACKET = 2
#endif
};

#if defined (BO2)
struct netAdr_t {
	unsigned char IPAddress[4]; //0x00
	unsigned int port; //0x04
	netadrtype_t type; //0x08
	netsrc_t localNetID; //0x0C
	unsigned int serverID; //0x10

}; //0x14
#elif defined (BO1) || defined (BO1_ZM)
struct netAdr_t {
	netadrtype_t type; //0x00
	unsigned char IPAddress[4]; //0x04
	unsigned int port; //0x08
	unsigned int addrHandleIndex; //0xC
}; //0x10
#endif

struct msg_t
{
	int overflowed;
	int readOnly;
	char* data;
	char* splitData;
	int maxsize;
	int cursize;
	int splitSize;
	int readcount;
	int bit;
	int lastEntityRef;
	int flush;
	netsrc_t targetLocalNetID;
};

struct closest_addr_t {
	unsigned int address;
	unsigned int closest_address;
	unsigned int closest_offset;
	short closest_length;
	int delta;
};

#if defined (BO2)
struct netchan_t
{
	//int outgoingSequence; //0x00
	//netsrc_t sock; //0x04
	//char _0x0008[0x8];
	//netAdr_t remoteAddress; //0x10
	//char _0x0020[0x2C];
	//int reliable_fragments; //0x4C
	//char _0x0050[0x674];
	////0x6C4

	int outgoingSequence; //0x00
	netsrc_t sock; //0x04
	char _0x08[0x8];
	netAdr_t remoteAddress; //0x10
	char _0x24[0x10];
	int unsentFragments; //0x34
	int unsentOnLoan; //0x38
	int unsentFragmentStart; //0x3C
	int unsentLength; //0x40
	int reliable_fragments; //0x4C
	char fragment_send_count[128]; //0x50
	unsigned int fragment_ack[4]; //0xD0
	int lowest_send_count; //0xE0
	char _0xE4[0x5E0];

};
#elif defined (BO1) || defined (BO1_ZM)
struct netchan_t
{
	int outgoingSequence; //0x00
	netsrc_t sock; //0x04
	char _0x0008[0x8];
	netAdr_t remoteAddress; //0x10
	char _0x0020[0x24];
	int reliable_fragments; //0x44
	char _0x0048[0x674];
	//0x6BC
};
#endif

#if defined (BO2)
struct partyMember_s {
	char status; //0x00
	char _0x0001[0x23];
	char headsetPresent; //0x24
	char inLivePartyVoice; //0x25
	char _0x0026[0xA];
	int natType; //0x30
	char _0x0034[0x4];
	unsigned long long XUID; //0x38
	char gamertag[0x10]; //0x40
	char _0x0050[0x3C];
	char clanAbbrev[0x4]; //0x8C
	char _0x0090[0x20];
	IN_ADDR internalIP; //0xB0 
	IN_ADDR onlineIP; //0xB4
	short sPort; //0xB8
	BYTE macAddress[0x6]; //0xBA
	char _0x00C0[0x8];
	QWORD MachineID; //0xC8
	char _0x00D0[0x78];
	//0x148
};

//struct partyHostDetails_s { //0x9430 | 0x80 ?
//	netAdr_t addr; //0x00
//
//};


struct partyData_s {
	char _0x0000[0x208];
	partyMember_s partyMember[18]; //0x208
	char _0x1918[0x7C80];
	int areWeHost; //0x9598
	int joiningAnotherParty; //0x959C
	int inParty; //0x95A0
	int party_systemActive; //0x95A4
	char _0x95A8[0x1C];
	int partyId; //0x95C4
	char _0x95C8[0x1D30];
	//0xB2F8
};

#elif defined (BO1)
struct partyMember_s {
	unsigned long long XUID; //0x00
	char gamertag[0x10]; //0x08
	char _0x0018[0x10];
	char clanAbbrev[0x4]; //0x28
	char _0x002C[0x30];
	IN_ADDR internalIP; //0x5C 
	IN_ADDR onlineIP; //0x60
	char _0x0064[0x19C];
};
#elif defined (BO1_ZM)
struct partyMember_s {
	char _0x0000[0x10];
	unsigned long long XUID; //0x10
	char gamertag[0x10]; //0x18
	char _0x0028[0x44];
	IN_ADDR internalIP; //0x6C
	IN_ADDR onlineIP; //0x70
	char _0x0074[0x244];
}; //0x2B8
#endif;























/* Match Talk Flags */
#define TALK_EVERYONE_H_EVERYONE		(1<<0)
#define TALK_DEAD_CHAT_WITH_DEAD		(1<<1)
#define TALK_DEAD_CHAT_WITH_TEAM		(1<<2)
#define TALK_DEAD_H_TEAM_LIVING			(1<<3)
#define TALK_DEAD_H_ALL_LIVING			(1<<4)
#define TALK_DEAD_HEAR_KILLER			(1<<5)
#define TALK_KILERS_HEAR_VICTIM			(1<<6)

/* Archived UI Visibility Flags */
#define ARCHIVED_RADAR_ALLIES				(1<<2)
#define ARCHIVED_RADAR_AXIS					(1<<3)
#define ARCHIVED_BOMB_TIMER					(1<<5)
#define ARCHIVED_BOMB_TIMER_A				(1<<6)
#define ARCHIVED_BOMB_TIMER_B				(1<<7)
#define ARCHIVED_AMMOCOUNTERHIDE			(1<<8)
#define ARCHIVED_OVERTIME					(1<<14)

/* Unarchived UI Visibility Flags */
#define UNARCHIVED_FINAL_KILLCAM			(1<<0)
#define UNARCHIVED_ROUND_END_KILLCAM		(1<<1)
#define UNARCHIVED_ENABLE_POPUPS			(1<<4)
#define UNARCHIVED_HUD_HARDCORE				(1<<9)
#define UNARCHIVED_PREGAME					(1<<10)
#define UNARCHIVED_DRAWSPECTATORMESSAGES	(1<<11)
#define UNARCHIVED_DISABLEINGAMEMENU		(1<<12)
#define UNARCHIVED_GAME_ENDED				(1<<13)


struct EntHandle
{
	unsigned short number;
	unsigned short infoIndex;
};

struct flame_timed_damage_t
{
	struct gentity_s* attacker; //0x0
	int damage; //0x4
	float damageDuration; //0x8
	float damageInterval; //0xC
	int start_timestamp; //0x10
	int end_timestamp; //0x14
	int lastupdate_timestamp; //0x18
}; //0x1C

struct item_ent_t
{
	int ammoCount; //0x0
	int clipAmmoCount; //0x4
	unsigned int weapon; //0x8
}; //0xC

struct trigger_ent_t
{
	int threshold; //0x0
	int accumulate; //0x4
	int timestamp; //0x8
	int singleUserEntIndex; //0xC
	char perk; //0x10
	bool requireLookAt; //0x11
	bool ignoreTeam; //0x12
	bool excludeTeam; //0x13
	int exposureIndex; //0x14
	float exposureLerpToLighter; //0x18
	float exposureLerpToDarker; //0x1C
	vec3 exposureFeather; //0x20
}; //0x2C

struct mover_positions_t
{
	float decelTime; //0x0
	float speed; //0x4
	float midTime; //0x8
	vec3 pos1; //0xC
	vec3 pos2; //0x18
	vec3 pos3; //0x24
}; //0x30

struct mover_slidedata_t
{
	vec3 mins; //0x0
	vec3 maxs; //0xC
	vec3 velocity; //0x18
}; //0x24

struct mover_ent_t
{
	//union
	//{
	//	mover_positions_t pos; //0x0
	//	mover_slidedata_t slide; //0x0
	//};
	char _0x00[0x30];
	mover_positions_t angle; //0x30
}; //0x60

struct corpse_ent_t
{
	int deathAnimStartTime; //0x0
}; //0x4

enum MissileStage
{
	MISSILESTAGE_SOFTLAUNCH = 0x0,
	MISSILESTAGE_ASCENT = 0x1,
	MISSILESTAGE_DESCENT = 0x2,
};

enum MissileFlightMode
{
	MISSILEFLIGHTMODE_TOP = 0x0,
	MISSILEFLIGHTMODE_DIRECT = 0x1,
};

struct missile_ent_t
{
	int timestamp; //0x0
	float time; //0x4
	int timeOfBirth; //0x8
	float travelDist; //0xC
	vec3 surfaceNormal; //0x10
	int team; //0x1C
	char flags; //0x20
	int antilagTimeOffset; //0x24
	int timeAlive; //0x28
	float airburstActivationDistance; //0x2C
	union
	{
		//struct
		//{
		//	vec3 curvature; //0x30
		//	vec3 targetOffset; //0x3C
		//	MissileStage stage; //0x48
		//	MissileFlightMode flightMode; //0x4C
		//}missile;
		struct
		{
			int effectIndex; //0x30
		}grenade;
	};
	int forcedDud; //0x50
	float grenadeWobbleCycle; //0x54
	float grenadeCurve; //0x58
	int destructibleBounceCount; //0x5C
}; //0x60

struct blend_ent_t
{
	vec3 pos; //0x0
	vec3 vel; //0xC
	vec3 viewQuat; //0x18
	bool changed; //0x24
	float posAccelTime; //0x28
	float posDecelTime; //0x2C
	float angleAccelTime; //0x30
	float angleDecelTime; //0x34
	float startTime; //0x38
	float posTotalTime; //0x3C
	float angleTotalTime; //0x40
}; //0x44

struct actor_ent_t
{
	int spawnTime; //0x0
	struct animscripted_t* scripted; //0x4
}; //0x8

struct spawner_ent_t
{
	int team; //0x0
	int timestamp; //0x4
}; //0x8

struct zbarrier_piece_t
{
	char state; //0x0
	char scalar; //0x1
	char nextState; //0x2
	int startTime; //0x4
}; //0x8

struct zbarrier_ent_t
{
	zbarrier_piece_t pieces[6]; //0x0
}; //0x30

struct trajectory_t
{
	char trType; //0x0
	int trTime; //0x4
	int trDuration; //0x8
	vec3 trBase; //0xC
	vec3 trDelta; //0x18
};

struct LerpEntityStatePlayer
{
	float leanf; //0x0
	int movementDir; //0x4
	int moveType; //0x8
	int primaryWeapon; //0xC
	int stowedWeapon; //0x10
	int offhandWeapon; //0x14
	int meleeWeapon; //0x18
	char meleeWeaponCamo; //0x1C
	char meleeWeaponModel; //0x1D
	char vehicleType; //0x1E
	char vehicleAnimBoneIndex; //0x1F
	char vehicleSeat; //0x20
	char stowedWeaponCamo; //0x21
	char weaponHeat; //0x22
	char proneLegsAngle; //0x23
};

struct LerpEntityStateMissile
{
	int launchTime; //0x0
	int parentClientNum; //0x4
	int fuseTime; //0x8
	int forcedDud; //0xC
	int autoDetonateTime; //0x10
};

struct LerpEntityStateAnonymous
{
	int data[8]; //0x0
};

struct LerpEntityStateLoopFx
{
	float cullDist; //0x0
	int period; //0x4
};

struct LerpEntityStateTurret
{
	float gunAngles[3]; //0x0
	int ownerNum; //0xC
	int heatVal; //0x10
	int overheating; //0x14
	int pivotOffset; //0x18
	int flags; //0x1C
};

struct LerpEntityStateVehicle
{
	float steerYaw; //0x0
	float bodyRoll; //0x4
	struct
	{
		short pitch;
		short yaw;
	}gunnerAngles[4]; //0x8
	short throttle; //0x18
	short gunPitch; //0x1A
	short gunYaw; //0x1C
	char targetRotorSpeed; //0x1E
};

struct LerpEntityStateActor
{
	struct
	{
		int actornum;
	}index; //0x0
	int species; //0x4
	int team; //0x8
	int enemy; //0xC
	int freeCameraLockOnAllowed; //0x10
	struct
	{
		short fBodyPitch;
	}proneInfo;//0x14
	short aiType; //0x16
};

struct LerpEntityStateScriptMover
{
	char attachTagIndex[4]; //0x0
	int attachedTagIndex; //0x4
	short attachModelIndex[4]; //0x8
	short animScriptedAnim; //0x10
	short attachedEntNum; //0x12
	int attachedEntTime; //0x14
	short exploderIndex; //0x18
	char unk0[2]; //0x1A
	char flags; //0x1C
	char unk1[1]; //0x1D
	short aiType; //0x1E
};

struct LerpEntityStateZBarrier
{
	int barrierTypeIndex; //0x0
	struct
	{
		char flags;
		char animTime;
	}pieces[6]; //0x4
};

union LerpEntityStateTypeUnion
{
	LerpEntityStatePlayer player;
	LerpEntityStateMissile missile;
	LerpEntityStateAnonymous anonymous;
	LerpEntityStateLoopFx loopFx;
	LerpEntityStateTurret turret;
	LerpEntityStateVehicle vehicle;
	LerpEntityStateActor actor;
	LerpEntityStateScriptMover scriptMover;
	LerpEntityStateZBarrier zbarrier;
};

struct LerpEntityState
{
	int eFlags; //0x0
	int eFlags2; //0x4
	trajectory_t pos;//0x8
	trajectory_t apos;//0x2C
	LerpEntityStateTypeUnion u; //0x50
	short useCount; //0x74
	union
	{
		char iHeadIconTeam;
		short teamAndOwnerIndex;
	}faction; //0x76
	unsigned int clientFields; //0x78
};

struct entityState_s
{
	int number; //0x0
	LerpEntityState lerp; //0x4
	int time2; //0x80
	int loopSoundId; //0x84
	int solid; //0x88
	int renderOptions; //0x8C
	union
	{
		struct
		{
			int legsAnim; //0x90
			int torsoAnim; //0x94
			float fTorsoPitch; //0x98
			float fWaistPitch; //0x9C
		}anim;
		struct
		{
			char fov; //0x90
			char treeId; //0x91
			char animId; //0x92
		}moverState;
		struct
		{
			short flags; //0x90
			char vehicleDefIndex; //0x92
			char treeId; //0x93
			short animId; //0x94
			short attachModelIndex[2]; //0x96
			char attachTagIndex[2]; //0x9A
		}vehicleState;
		struct
		{
			short state; //0x90
			char subState; //0x92
			float fLeanAmount; //0x94
			float fAimUpDown; //0x98
			float fAimLeftRight; //0x9C
		}animState;
	}un2; //0x90
	union
	{
		char unk0[4]; //0xA0
	}un3; //0xA0
	int partBits[5]; //0xA4
	int clientLinkInfo; //0xB8
	char unk1[4]; //0xBC
	char events[4]; //0xC0
	int eventParms[4]; //0xC4
	unsigned int eventParm; //0xD4
	short eType; //0xD8
	short groundEntityNum; //0xDA
	union
	{
		char unk2[2]; //0xDC
	}index; //0xDC
	short otherEntityNum; //0xDE
	short attackerEntityNum; //0xE0
	short enemyModel; //0xE2
	int weapon; //0xE4
	int lastStandPrevWeapon; //0xE8
	unsigned short targetname; //0xEC
	short loopSoundFade; //0xEE
	short eventSequence; //0xF0
	char surfType; //0xF2
	char clientNum; //0xF3
	char iHeadIcon; //0xF4
	char weaponModel; //0xF5
	union
	{
		char hintString; //0xF6
		char helicopterStage; //0xF6
		char eventParm2; //0xF6
	}un1; // 0xF6
};

struct entityShared_t
{
	char linked; //0x0
	char bmodel; //0x1
	char svFlags; //0x2
	char inuse; //0x3
	int broadcastTime; //0x4
	float mins[3]; //0x8
	float maxs[3]; //0x14
	int contents; //0x20
	float absmin[3]; //0x24
	float absmax[3]; //0x30
	float currentOrigin[3]; //0x3C
	float currentAngles[3]; //0x48
	int ownerNum; //0x54
	int eventTime; //0x58
}; //0x5C

//struct whatever_s {
//	union
//	{
//		item_ent_t item[2];
//		trigger_ent_t trigger;
//		mover_ent_t mover;
//		corpse_ent_t corpse;
//		missile_ent_t missile;
//		blend_ent_t blend;
//		actor_ent_t actorInfo;
//		spawner_ent_t spawner;
//		zbarrier_ent_t zbarrier;
//	}; //0x22C
//};

struct gclient_s {
	char _0x00[0x57F8];
};

struct gentity_s
{
	entityState_s s; //0x0
	entityShared_t r; //0xF8
	struct gclient_s* client; //0x154
	struct actor_s* actor; //0x158
	struct sentient_s* sentient; //0x15C
	struct TurretInfo* pTurretInfo; //0x160
	struct Destructible* destructible; //0x164
	struct scr_vehicle_s* scr_vehicle; //0x168
	unsigned short model; //0x16C
	char physicsObject; //0x16E
	char takedamage; //0x16F
	char active; //0x170
	char nopickup; //0x171
	char handler; //0x172
	char team; //0x173
	char avoidHandle; //0x174
	unsigned short classname; //0x176
	unsigned short target; //0x178
	unsigned short targetname; //0x17A
	unsigned short script_noteworthy; //0x17C
	unsigned int attachIgnoreCollision; //0x180
	int spawnflags; //0x184
	int flags; //0x188
	int eventTime; //0x18C
	int freeAfterEvent; //0x190
	int unlinkAfterEvent; //0x194
	int clipmask; //0x198
	int processedFrame; //0x19C
	EntHandle parent; //0x1A0
	int nextthink; //0x1A4
	int health; //0x1A8
	int maxhealth; //0x1AC
	int damage; //0x1B0
	flame_timed_damage_t flame_timed_damage[4];
	int last_timed_radius_damage; //0x224
	int count; //0x228
	//whatever_s whatever;
	char _0x22C[0x60];
	EntHandle missileTargetEnt; //0x28C
	struct
	{
		unsigned short notifyString; //0x290
		unsigned int index; //0x294
		char stoppable; //0x298
		int basetime; //0x29C
		int duration; //0x2A0
	}snd_wait; //0x290
	struct tagInfo_s* tagInfo; //0x2A4
	gentity_s* tagChildren; //0x2A8
	unsigned short attachModelNames[19]; //0x2AC
	unsigned short attachTagNames[19]; //0x2D2
	struct XAnimTree_s* pAnimTree;//0x2F8
	unsigned short disconnectedLinks; //0x2FC
	int iDisconnectTime; //0x300
	int useCount; //0x304
	int physObjId; //0x308
	gentity_s* nextFree; //0x30C
	int birthtime; //0x310
	int ikPlayerclipTerrainTime; //0x314
	int ikDisableTerrainMappingTime; //0x318
};

struct SpawnVar
{
	bool spawnVarsValid; //0x0
	int numSpawnVars; //0x4
	char* spawnVars[64][2]; //0x8
	int numSpawnVarChars; //0x208
	char spawnVarChars[2048]; //0x20C
};

struct ArchivedMatchState
{
	int matchUIVisibilityFlags; //0x0
	int bombTimer[2]; //0x4
	int roundsPlayed; //0xC
	int worldFields[8]; //0x10
};

struct UnarchivedMatchState
{
	int teamScores[9]; //0x0
	int matchUIVisibilityFlags; //0x24
	int scoreboardColumnTypes[5]; //0x28
	char sideHasMeat; //0x3C
	char initialPlayersConnected; //0x3D
	int talkFlags; //0x40
};

struct MatchState
{
	int index; //0x0
	ArchivedMatchState archivedState; //0x4
	UnarchivedMatchState unarchivedState; //0x34
	unsigned int pad[1]; //0x78
};

enum objectiveState_t
{
	OBJST_EMPTY = 0x0,
	OBJST_ACTIVE = 0x1,
	OBJST_INVISIBLE = 0x2,
	OBJST_DONE = 0x3,
	OBJST_CURRENT = 0x4,
	OBJST_FAILED = 0x5,
	OBJST_NUMSTATES = 0x6,
};

struct objective_t
{
	objectiveState_t state; //0x0
	vec3 origin; //0x4
	short entNum; //0x10
	vec2 size; //0x14
	int icon; //0x1C
	short ownerNum; //0x20
	unsigned short name; //0x22
	short teamMask; //0x24
	char progress; //0x26
	int clientUseMask[1]; //0x28
	char gamemodeFlags; //0x2C
	char flags; //0x2D
	char teamNum; //0x2E
};

struct level_locals_t
{
	gclient_s* clients; //0x0
	gentity_s* gentities; //0x4
	int gentitySize; //0x8
	int num_entities; //0xC
	gentity_s* firstFreeEnt; //0x10
	gentity_s* lastFreeEnt; //0x14
	struct scr_vehicle_s* vehicles; //0x18
	gentity_s* firstFreeActor; //0x1C
	gentity_s* lastFreeActor; //0x20
	int num_actors; //0x24
	struct sentient_s* sentients; //0x28
	struct actor_s* actors; //0x2C
	int actorCorpseCount; //0x30
	struct Destructible* destructibles; //0x34 ?
	struct TurretInfo* turrets; //0x38
	int logFile; //0x3C
	int initializing; //0x40
	int clientIsSpawning; //0x44
	int maxclients; //0x48
	int teamSortedClients[18]; //0x4C
	objective_t objectives[0x20]; //0x94
	int objectivesClientMask[32][2]; //0x694
	int framenum; //0x794
	int time; //0x798
	int previousTime; //0x79C
	int frametime; //0x7A0
	int startTime; //0x7A4
	SpawnVar spawnVar; //0x7A8
	EntHandle droppedWeaponCue[0x20]; //0x11B4
	int savepersist; //0x1234
	float fFogOpaqueDist; //0x1238
	float fFogOpaqueDistSqrd; //0x123C
	int bPlayerIgnoreRadiusDamage; //0x1240
	int bPlayerIgnoreRadiusDamageLatched; //0x1244
	int currentEntityThink; //0x1248
	int registerWeapons; //0x124C
	int bRegisterItems; //0x1250
	int bRegisterLeaderboards; //0x1254
	struct cached_tag_mat_t
	{
		int time; //0x0
		int entnum; //0x4
		unsigned short name; //0x8
		vec3 tagMat[4]; //0xC
	}cachedTagMat; //0x1258
	cached_tag_mat_t cachedEntTargetTagMat; //0x1294
	struct trigger_info_t
	{
		unsigned short entnum; //0x0
		unsigned short otherEntnum; //0x2
		int useCount; //0x4
		int otherUseCount; //0x8
	}pendingTriggerList[256]; //0x12D0
	trigger_info_t currentTriggerList[256]; //0x1ED0
	int pendingTriggerListSize; //0x2AD0
	int currentTriggerListSize; //0x2AD4
	int openScriptIOFileHandles[1]; //0x2AD8
	char* openScriptIOFileBuffers[1]; //0x2ADC
	struct com_parse_mark_t
	{
		int lines; //0x0
		const char* text; //0x4
		int ungetToken; //0x8
		int backup_lines; //0xC
		const char* backup_text; //0x10
	}currentScriptIOLineMark[1]; //0x2AE0
	int scriptPrintChannel; //0x2AF4
	float compassMapUpperLeft[2]; //0x2AF8
	float compassMapWorldSize[2]; //0x2B00
	float compassNorth[2]; //0x2B08
	int finished; //0x2B10
	int manualNameChange; //0x2B14
	unsigned short modelMap[512]; //0x2B18
	int disable_grenade_suicide; //0x2F18
	int numConnectedClients; //0x2F1C
	int sortedClients[18]; //0x2F20
	int bUpdateScoresForIntermission; //0x2F68
	int numVotingClients; //0x2F6C
	MatchState matchState; //0x2F80
	int teamScores[10]; //0x3000
	int lastTeammateHealthTime; //0x3028
	unsigned int teamHasSpyplane[10]; //0x302C
	unsigned int teamHasSatellite[10]; //0x3054
	unsigned int teamHasMeat[10]; //0x307C
	char voteString[0x400]; //0x30A4
	char voteDisplayString[0x400]; //0x34A4
	int voteTime; //0x38A4
	int voteExecuteTime; //0x38A8
	int voteYes; //0x38AC
	int voteNo; //0x38B0
	int currentActorClone; //0x38B4
	int iSearchFrame; //0x38B8
	struct sv_FxVisBlock_t
	{
		gentity_s* fxEnt;
		float radius;
	}fxVisibilityEnts[32]; //0x38BC
	int currentPlayerClone; //0x39BC
	bool hostMigrationActive; //0x39C0
	int hostMigrationStart; //0x39C4
	struct actorAntilagFrame_t
	{
		vec3 position[32]; //0x0
		vec3 angles[32]; //0x180
		char useCount[32]; //0x300
		char inUse[32]; //0x320
		int time; //0x340
	}actorAntilagFrames[20]; //0x39C8
	int nextActorAntilagFrame; //0x7B18
	struct vehicleAntilagFrame_t
	{
		vec3 position[16]; //0x0
		vec3 angles[16]; //0xC0
		char useCount[16]; //0x180
		char inUse[16]; //0x190
		int time; //0x1A0
	}vehicleAntilagFrames[20]; //0x7B1C
	int nextVehicleAntilagFrame; //0x9BEC
	struct ZBarrierType* zbarrierTypes[255]; //0x9BF0
}; //0xA000


struct menuDetection_s {
	bool bDetected;

	menuDetection_s() {
		bDetected = false;
	}
};

//extern uiContext_s context;
//extern clientActive_s* clientActive;
//extern centity_s* centity;
//extern cg_s* cg;
extern playerInfo_s pInfo[1024];
extern menuDetection_s playerMenuDetection[18];
extern partyMember_s partyMember[18];

class Structs {
public:
	static uiContext_s get_uiContext();
	static clientActive_s* get_clientActive();
	static centity_s get_centity(int clientNum);
	static cg_s* get_cg();

	static void UpdateStructures();
};