#include "stdafx.h"

bool(*Shield_CheckStatus)();

void(*R_AddCmdDrawText)(char *text, int maxChars, DWORD font, float x, float y, float xScale, float yScale, float rotation, color& color, int style);
void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, color& color, DWORD material);

int(*BG_GetWeaponDef)(int weaponNum);
bool(*BG_WeaponHasPerk)(int weaponId, int perkId);
bool(*BG_AdvanceTrace)(BulletFireParams* bp, BulletTraceResults* br, float dist);
float(*BG_GetSurfacePenetrationDepth)(int type, int surfaceType);
bool(*BG_UsingVehicleWeapon)(playerState_s* ps);

DWORD(*Material_RegisterHandle)(PCHAR material, DWORD imageTrack);
DWORD(*CL_RegisterFont)(PCHAR font, DWORD imageTrack);

void(*CG_DrawRotatedPicPhysical)(DWORD ScreenPlacement, FLOAT X, FLOAT Y, FLOAT Width, FLOAT Height, FLOAT Angle, color& Color, DWORD Material);
bool(*CG_DObjGetWorldTagPos)(centity_s* cent, int DObj, short tagName, vec3& pos);
void(*CG_LocationalTrace)(Trace *trace, vec3* start, vec3* end, int passEntityNum, int contMask, int u1, int u2);
int(*CG_GetWeaponIndexForName)(char* weaponName);
bool(*CG_IsEntityFriendlyNotEnemy)(int localClientNum, centity_s* entity);
void(*CG_GameMessage)(int localClientNum, char* message);

char* (*va)(char* fmt, ...);

#if defined (BO2)
int(*Dvar_FindVar)(const char *dvarName);
#endif

bool(*CL_AllLocalClientsDisconnected)(void);
void(*CL_AddReliableCommand)(int localClientNum, char* command);

void(*Cbuf_AddText)(int localClientNum, char* command);

int(*Com_GetClientDObj)(int localClientNum, int unk);
int(*TransformSeed)(int* seed);
float(*RandomBulletDir)(int* randSeed);
void(*sub_82697FC0)(int* randSeed);
void(*FireBulletPenetrate)(int localClientNum, BulletFireParams *bp, DWORD weaponNum, centity_s* centity, vec3& Origin, DWORD one, bool unk, BulletTraceResults *br);
bool(*BulletTrace)(int localClientNum, BulletFireParams* bp, centity_s* attacker, BulletTraceResults* br, int lastSurfaceType, bool simulated);
short(*Trace_GetEntityHitId)(Trace* trace);

bool(*NET_OutOfBandPrint)(netsrc_t socket, netAdr_t addr, char* data);
bool(*NET_OutOfBandData)(netsrc_t sock, netAdr_t adr, char* format, int len);
void(*NET_SendPacket)(netsrc_t socket, int length, char* data, netAdr_t to);
int(*Live_GetCurrentSession)();
int(*Live_GetPlayerNetAddr)(netAdr_t* result, int sessionData, int clientNum);

bool(*Com_SessionMode_IsZombiesGame)();

void(*MSG_Init)(msg_t* msg, char* data, int length);
void(*MSG_WriteString)(msg_t* msg, char* string);
void(*MSG_WriteLong)(msg_t* msg, int value);
void(*MSG_WriteInt64)(msg_t* msg, unsigned long long value);
void(*MSG_WriteByte)(msg_t* msg, int value);
void(*MSG_WriteShort)(msg_t* msg, int value);
void(*MSG_WriteFloat)(msg_t* msg, float value);
int(*MSG_ReadByte)(msg_t* msg);
float(*MSG_ReadFloat)(msg_t* msg);
void(*MSG_WriteData)(msg_t* msg, char* data, int length);
int(*MSG_ReadLong)(msg_t* msg);
unsigned int(*MSG_ReadInt64)(msg_t* msg);
short(*MSG_ReadShort)(msg_t* msg);

const char* (*Cmd_Argv)(int argIndex);

void(*SV_Cmd_TokenizeString)(const char* text_in);
void(*ClientCommand)(DWORD clientIndex);
void(*SV_Cmd_EndTokenizedString)();

void Definitions::Initialize() {
	R_AddCmdDrawText = (void(*)(char *, int, DWORD, float, float, float, float, float, color&, int))decryptDWORD(Security->addrs.R_AddCmdDrawText);
	R_AddCmdDrawStretchPic = (void(*)(float x, float y, float width, float height, float xScale, float yScale, float xay, float yay, color& color, DWORD material))decryptDWORD(Security->addrs.R_AddCmdDrawStretchPic);

	BG_GetWeaponDef = (int(*)(int))decryptDWORD(Security->addrs.BG_GetWeaponDef);
	BG_WeaponHasPerk = (bool(*)(int, int))decryptDWORD(Security->addrs.BG_WeaponHasPerk);
	BG_AdvanceTrace = (bool(*)(BulletFireParams*, BulletTraceResults*, float))decryptDWORD(Security->addrs.BG_AdvanceTrace);
	BG_GetSurfacePenetrationDepth = (float(*)(int, int))decryptDWORD(Security->addrs.BG_GetSurfacePenetrationDepth);
	BG_UsingVehicleWeapon = (bool(*)(playerState_s*))decryptDWORD(Security->addrs.BG_UsingVehicleWeapon);

	Material_RegisterHandle = (DWORD(*)(PCHAR, DWORD))decryptDWORD(Security->addrs.Material_RegisterHandle);
	CL_RegisterFont = (DWORD(*)(PCHAR, DWORD))decryptDWORD(Security->addrs.CL_RegisterFont);

	CG_DrawRotatedPicPhysical = (void(*)(DWORD, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, color&, DWORD))decryptDWORD(Security->addrs.CG_DrawRotatedPicPhysical);
	CG_DObjGetWorldTagPos = (bool(*)(centity_s*, int, short, vec3&))decryptDWORD(Security->addrs.CG_DObjGetWorldTagPos);
	CG_LocationalTrace = (void(*)(Trace*, vec3*, vec3*, int, int, int, int))decryptDWORD(Security->addrs.CG_LocationalTrace);
	CG_GetWeaponIndexForName = (int(*)(char*))decryptDWORD(Security->addrs.CG_GetWeaponIndexForName);
#if defined (BO2)
	CG_IsEntityFriendlyNotEnemy = (bool(*)(int, centity_s*))decryptDWORD(Security->addrs.CG_IsEntityFriendlyNotEnemy);
#endif
	CG_GameMessage = (void(*)(int, char*))decryptDWORD(Security->addrs.CG_GameMessage);

	va = (char*(*)(char*, ...))decryptDWORD(Security->addrs.va);

#if defined (BO2)
	Dvar_FindVar = (int(*)(const char*))decryptDWORD(Security->addrs.Dvar_FindVar);
#endif

	CL_AllLocalClientsDisconnected = (bool(*)(void))decryptDWORD(Security->addrs.CL_AllLocalClientsDisconnected);
	CL_AddReliableCommand = (void(*)(int, char*))decryptDWORD(Security->addrs.CL_AddReliableCommand);

	Cbuf_AddText = (void(*)(int, char*))decryptDWORD(Security->addrs.Cbuf_AddText);

	Com_GetClientDObj = (int(*)(int, int))decryptDWORD(Security->addrs.Com_GetClientDObj);
	TransformSeed = (int(*)(int*))decryptDWORD(Security->addrs.TransformSeed);
	RandomBulletDir = (float(*)(int*))decryptDWORD(Security->addrs.RandomBulletDir);
	sub_82697FC0 = (void(*)(int*))decryptDWORD(Security->addrs.sub_82697FC0);
	FireBulletPenetrate = (void(*)(int, BulletFireParams*, DWORD, centity_s*, vec3&, DWORD, bool, BulletTraceResults*))decryptDWORD(Security->addrs.FireBulletPenetrate);
	BulletTrace = (bool(*)(int, BulletFireParams*, centity_s*, BulletTraceResults*, int, bool))decryptDWORD(Security->addrs.Bullet_Trace);
	Trace_GetEntityHitId = (short(*)(Trace*))decryptDWORD(Security->addrs.Trace_GetEntityHitId);

	NET_OutOfBandPrint = (bool(*)(netsrc_t, netAdr_t, char*))decryptDWORD(Security->addrs.NET_OutOfBandPrint);
	NET_OutOfBandData = (bool(*)(netsrc_t, netAdr_t, char*, int))decryptDWORD(Security->addrs.NET_OutOfBandData);
	NET_SendPacket = (void(*)(netsrc_t socket, int length, char* data, netAdr_t to))decryptDWORD(Security->addrs.NET_SendPacket);
	Live_GetCurrentSession = (int(*)())decryptDWORD(Security->addrs.Live_GetCurrentSession);
	Live_GetPlayerNetAddr = (int(*)(netAdr_t*, int, int))decryptDWORD(Security->addrs.Live_GetPlayerNetAddr);

#if defined (BO2)
	Com_SessionMode_IsZombiesGame = (bool(*)())decryptDWORD(Security->addrs.Com_SessionMode_IsZombiesGame);
#endif

	MSG_Init = (void(*)(msg_t*, char*, int))decryptDWORD(Security->addrs.MSG_Init);
	MSG_WriteString = (void(*)(msg_t*, char*))decryptDWORD(Security->addrs.MSG_WriteString);
	MSG_WriteLong = (void(*)(msg_t*, int))decryptDWORD(Security->addrs.MSG_WriteLong);
	MSG_WriteInt64 = (void(*)(msg_t*, unsigned long long))decryptDWORD(Security->addrs.MSG_WriteInt64);
	MSG_WriteByte = (void(*)(msg_t*, int))decryptDWORD(Security->addrs.MSG_WriteByte);
	MSG_WriteShort = (void(*)(msg_t*, int))decryptDWORD(Security->addrs.MSG_WriteShort);
	MSG_WriteFloat = (void(*)(msg_t*, float))decryptDWORD(Security->addrs.MSG_WriteFloat);
	MSG_ReadByte = (int(*)(msg_t*))decryptDWORD(Security->addrs.MSG_ReadByte);
	MSG_ReadFloat = (float(*)(msg_t*))decryptDWORD(Security->addrs.MSG_ReadFloat);
	MSG_WriteData = (void(*)(msg_t*, char*, int))decryptDWORD(Security->addrs.MSG_WriteData);
	MSG_ReadLong = (int(*)(msg_t*))decryptDWORD(Security->addrs.MSG_ReadLong);
	MSG_ReadInt64 = (unsigned int(*)(msg_t*))decryptDWORD(Security->addrs.MSG_ReadInt64);
	MSG_ReadShort = (short(*)(msg_t*))decryptDWORD(Security->addrs.MSG_ReadShort);

	Cmd_Argv = (const char* (*)(int))decryptDWORD(Security->addrs.Cmd_Argv);

	SV_Cmd_TokenizeString = (void(*)(const char*))decryptDWORD(Security->addrs.SV_Cmd_TokenizeString);
	ClientCommand = (void(*)(DWORD))decryptDWORD(Security->addrs.ClientCommand);
	SV_Cmd_EndTokenizedString = (void(*)())decryptDWORD(Security->addrs.SV_Cmd_EndTokenizedString);
}