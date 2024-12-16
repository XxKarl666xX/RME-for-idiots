#pragma once

extern bool(*Shield_CheckStatus)();

extern void(*R_AddCmdDrawText)(char *text, int maxChars, DWORD font, float x, float y, float xScale, float yScale, float rotation, color& color, int style);
extern void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, color& color, DWORD material);

extern int(*BG_GetWeaponDef)(int weaponNum);
extern bool(*BG_WeaponHasPerk)(int weaponId, int perkId);
extern bool(*BG_AdvanceTrace)(BulletFireParams* bp, BulletTraceResults* br, float dist);
extern float(*BG_GetSurfacePenetrationDepth)(int type, int surfaceType);
extern bool(*BG_UsingVehicleWeapon)(playerState_s* ps);

extern DWORD(*Material_RegisterHandle)(PCHAR material, DWORD imageTrack);
extern DWORD(*CL_RegisterFont)(PCHAR font, DWORD imageTrack);

extern void(*CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, FLOAT Angle, color& Color, DWORD Material);
extern bool(*CG_DObjGetWorldTagPos)(centity_s* cent, int DObj, short tagName, vec3& pos);
extern void(*CG_LocationalTrace)(Trace *trace, vec3* start, vec3* end, int passEntityNum, int contMask, int u1, int u2);
extern int(*CG_GetWeaponIndexForName)(char* weaponName);
extern bool(*CG_IsEntityFriendlyNotEnemy)(int localClientNum, centity_s* entity);
extern void(*CG_GameMessage)(int localClientNum, char* message);

extern char* (*va)(char* fmt, ...);

#if defined (BO2)
extern int(*Dvar_FindVar)(const char* dvarName);
#endif

extern bool(*CL_AllLocalClientsDisconnected)(void);
extern void(*CL_AddReliableCommand)(int localClientNum, char* command);

extern void(*Cbuf_AddText)(int localClientNum, char* command);

extern int(*Com_GetClientDObj)(int localClientNum, int unk);
extern int(*TransformSeed)(int* seed);
extern float(*RandomBulletDir)(int* randSeed);
extern void(*sub_82697FC0)(int* randSeed);
extern void(*FireBulletPenetrate)(int localClientNum, BulletFireParams *bp, DWORD weaponNum, centity_s* centity, vec3& Origin, DWORD one, bool unk, BulletTraceResults *br);
extern bool(*BulletTrace)(int localClientNum, BulletFireParams* bp, centity_s* attacker, BulletTraceResults* br, int lastSurfaceType, bool simulated);
extern short(*Trace_GetEntityHitId)(Trace* trace);

extern bool(*NET_OutOfBandPrint)(netsrc_t socket, netAdr_t addr, char* data);
extern bool(*NET_OutOfBandData)(netsrc_t sock, netAdr_t adr, char* format, int len);
extern void(*NET_SendPacket)(netsrc_t socket, int length, char* data, netAdr_t to);
extern int(*Live_GetCurrentSession)();
extern int(*Live_GetPlayerNetAddr)(netAdr_t* result, int sessionData, int clientNum);

extern bool(*Com_SessionMode_IsZombiesGame)();

extern void(*MSG_Init)(msg_t* msg, char* data, int length);
extern void(*MSG_WriteString)(msg_t* msg, char* string);
extern void(*MSG_WriteLong)(msg_t* msg, int value);
extern void(*MSG_WriteInt64)(msg_t* msg, unsigned long long value);
extern void(*MSG_WriteByte)(msg_t* msg, int value);
extern void(*MSG_WriteShort)(msg_t* msg, int value);
extern void(*MSG_WriteFloat)(msg_t* msg, float value);
extern int(*MSG_ReadByte)(msg_t* msg);
extern float(*MSG_ReadFloat)(msg_t* msg);
extern void(*MSG_WriteData)(msg_t* msg, char* data, int length);
extern int(*MSG_ReadLong)(msg_t* msg);
extern unsigned int(*MSG_ReadInt64)(msg_t* msg);
extern short(*MSG_ReadShort)(msg_t* msg);

extern const char* (*Cmd_Argv)(int argIndex);

extern void(*SV_Cmd_TokenizeString)(const char* text_in);
extern void(*ClientCommand)(DWORD clientIndex);
extern void(*SV_Cmd_EndTokenizedString)();

class Definitions {
public:
	static void Initialize();
};

#define	ANGLE2SHORT(x)	((int)((x)*65536/360) & 65535)
#define	SHORT2ANGLE(x)	((x)*(360.0/65536))
#define PI 3.14159265358979323846264338327950288f