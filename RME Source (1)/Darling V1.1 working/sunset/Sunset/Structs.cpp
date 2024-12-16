#include "stdafx.h"

//uiContext_s context;
//clientActive_s* clientActive;
//centity_s* centity;
//cg_s* cg;
playerInfo_s pInfo[1024];
menuDetection_s playerMenuDetection[18];
partyMember_s partyMember[18] = { NULL };

uiContext_s Structs::get_uiContext() {
	return *(uiContext_s*)(decryptDWORD(Security->addrs.strct_uiContext));
}

clientActive_s* Structs::get_clientActive() {
	return (clientActive_s*)*(int*)(decryptDWORD(Security->addrs.strct_clientActive));
}

centity_s Structs::get_centity(int clientNum) {
	centity_s* entity = (centity_s*)*(int*)(decryptDWORD(Security->addrs.strct_centity));
	return entity[clientNum];
}

cg_s* Structs::get_cg() {
	return (cg_s*)*(int*)(decryptDWORD(Security->addrs.strct_cgArray));
}

void Structs::UpdateStructures() {
	//context = *(uiContext_s*)(decryptDWORD(Security->addrs.strct_uiContext));
	//clientActive = (clientActive_s*)*(int*)(decryptDWORD(Security->addrs.strct_clientActive));
	//centity = (centity_s*)*(int*)(decryptDWORD(Security->addrs.strct_centity));
	//cg = (cg_s*)*(int*)(decryptDWORD(Security->addrs.strct_cgArray));

#if defined (BO2)
	partyData_s* partyData = Game::get_active_party();

	for (int i = 0; i < 18; i++) {
		memcpy(&partyMember[i], &partyData->partyMember[i], sizeof(partyMember_s));
	}

#elif defined (BO1)  || defined (BO1_ZM)
	for (int i = 0; i < 18; i++) {
		partyMember[i] = *(partyMember_s*)(decryptDWORD(Security->addrs.g_partyMember) + (i * decryptDWORD(Security->addrs.g_partyMemberSize)));
	}
#endif
}