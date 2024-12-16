#pragma once

extern DWORD gClient(int clientNum);
extern DWORD gEntity(int clientNum);

struct RMEOptions_s {
	DWORD dwAddress;

	DWORD dwValue;
	SHORT sValue;
	BYTE bValue;
	QWORD qwValue;
	FLOAT fValue;
	vec3 vValue;
	string data;

	BOOL bAllowNullValue;


	RMEOptions_s() {
		dwAddress = 0x00;

		dwValue = 0x00;
		sValue = 0x00;
		bValue = 0x00;
		qwValue = 0x00;
		fValue = 0.0f;
		vValue = vec3();
		data = "";

		bAllowNullValue = FALSE;
	}
};

class Netchan
{
public:
	closest_addr_t GetClosestAddressFrom(DWORD target_address, DWORD base_address);
	static std::vector<closest_addr_t> closest_addresses;


	bool setup_packet_header(msg_t* msg, int size, closest_addr_t closest_addr);
	void setup_buffer_rme();
	void write(DWORD address, char* buf, int size);
	void write_int(DWORD address, DWORD value);
	void write_short(DWORD address, short value);
	void write_byte(DWORD address, byte value);
	void write_int64(DWORD address, QWORD value);
	void write_float(DWORD address, float value);
	void write_vector3(DWORD address, vec3 vec);
	void write_string(DWORD address, std::string string);
};

extern Netchan g_Netchan;
extern std::vector<RMEOptions_s> gRME_Options;

extern void RenameAll(std::string renameString);
extern void RenameSelectedClient(int clientNum, std::string renameString);

extern void ChangeGodMode(int clientNum, bool toggle);


extern void GodModeEveryone(bool toggle, bool bEnemy);
extern void ChangeSuperSpeed(int clientNum, bool toggle);
extern void SuperSpeedEveryone(bool toggle, bool bEnemy);
extern void ChangeInvisibility(int clientNum, bool toggle);
extern void ChangeInfraredVision(int clientNum, bool toggle);
extern void TeleportPlayerToMe(int clientNum);
extern void TeleportPlayerToHim(int clientNum);
extern void TeleportPlayerFarAway(int clientNum);
extern void TeleportAllPlayers();
extern void MoveEveryoneAround();
extern void SetUnlimitedAmmo(int clientNum);
extern void TakeAmmoAway(int clientNum);
extern void TakeAmmoAwayAll(bool bEnemy);
extern void ChangeMode(int clientNum, int mode);
extern void SetBlackscreen(int clientNum);
extern void BlackscreenEveryone(bool bEnemy);
extern void GiveWeapon(int clientNum, BYTE weaponIndex);
extern void TakeWeapon(int clientNum);
extern void TakeWeaponForEveryone(bool bEnemy);
extern void ThrowGrenadesForEveryone();
#if defined (BO2)
extern void GivePoints(int clientNum);
#endif
extern void AntiQuit(bool bEnable);
extern void SetHearEveryone(bool bEnable);
extern void SetModel(int clientNum);
extern void SetDisconnectState(int clientNum);
extern void SetSessionState(int clientNum, sessionState_t sessionState);
extern void SetFakeDerank(int clientNum, int rank);
extern void SetFakeDerankAll();
extern void KillPlayer(int clientNum);
extern void FreezeMovement(int clientNum, bool toggle);
extern void KickClient(int clientNum);
extern void BanClient(int clientNum);
extern void ResetHostStats();
extern void MapRestart();
extern void FastRestart();
extern void StartHostMigration();
extern void SendHostToZombies();
extern void SendHostToSingleplayer();
extern void ChangeMap();
extern void ChangeGametype();
extern void ExecuteTestPayload();
extern void SuperJump();
extern void UnbindHostControls();

//other
extern void send_relayed_packet(int client_num, char* message);
extern void Send_LoadingNewMap(int clientNum, bool bSendFreeze);
extern void Send_Disconnect(int clientNum);