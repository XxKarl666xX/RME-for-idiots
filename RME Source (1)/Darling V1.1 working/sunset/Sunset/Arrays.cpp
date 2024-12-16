#include "stdafx.h"

CHAR pModdedGamertags[10][100] = {
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty"
};

CHAR pModdedGTs[10][100] = {
	"Slot 1",
	"Slot 2",
	"Slot 3",
	"Slot 4",
	"Slot 5",
	"Slot 6",
	"Slot 7",
	"Slot 8",
	"Slot 9",
	"Slot 10",
};

CHAR pRME_AllClient_Presets[10][100] = {
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty",
	"Empty"
};

char* Array_AimbotType[0x2] = { "Visible", "Autowall" };
char* Array_TargetType[0x2] = { "Snap", "Silent" };
char* Array_TargetBone[0x10] = { "j_helmet", "j_head", "j_neck", "j_shoulder_le", "j_shoulder_ri", "j_mainroot", "j_elbow_le", "j_elbow_ri", "j_wrist_le", "j_wrist_ri", "j_spineupper", "j_spinelower", "j_knee_le", "j_knee_ri", "j_ankle_le", "j_ankle_ri" };

char* DrawingTypes[0x4] = { "2D", "2D Filled", "3D", "Corners" };

char* RME_Toggle[0x2] = { "Set", "Take" };
char* RME_Teams[0x2] = { "Enemies", "Friendlies" };
char* RME_SetModes[0x4] = { "Default", "No Clip", "Ufo Mode", "Freeze Controls"};

char* RME_GiveWeapons[39] = {
	"AN-94", "M8A1", "FAL OSW", "Type-25", "MTAR", "SMR", "SCAR-H", "SWAT-556",
	"M27", "MP7", "PDW", "Vector", "MSMC", "Chicom CQB", "Skorpion EVO", "Peacekeeper",
	"MK-48", "LSAW", "QBB LSW", "HAMR", "KSG", "R870 MCS", "S12", "M1216", "Ballista",
	"DSR-50", "SVU", "XPR", "Five-Seven", "Tac-45", "B23R", "Executioner", "KAP-40",
	"RPG", "SHAW", "FHJ-18", "Knife", "Riotshield", "Crossbow" };

BYTE RME_weaponIndexes[39] = {
	0x30, 0x2C, 0x1C, 0x14, 0x10, 0x28, 0x24, 0x18,
	0x20, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0C,
	0x38, 0x3A, 0x3C, 0x3E, 0x36, 0x34, 0x35, 0x37,
	0x42, 0x41, 0x40, 0x43, 0x4E, 0x4F, 0x50, 0x51,
	0x52, 0x56, 0x54, 0x55, 0x5B, 0x59, 0x5A };

char* RME_GiveWeapons_ZM[27] = {
	//"ak74u_zm",
	//"ak74u_upgraded_zm",
	//"mp5k_zm",
	//"mp5k_upgraded_zm",
	//"m14_zm",
	//"m14_upgraded_zm",
	//"m16_zm",
	//"m16_gl_upgraded_zm",
	//"xm8_zm",
	//"xm8_upgraded_zm",
	//"type95_zm",
	//"type95_upgraded_zm",
	//"galil_zm",
	//"galil_upgraded_zm",
	//"fnfal_zm",
	//"fnfal_upgraded_zm",
	//"rpd_zm",
	//"rpd_upgraded_zm",
	//"hamr_zm",
	//"hamr_upgraded_zm",
	//"dsr50_zm",
	//"dsr50_upgraded_zm",
	//"fiveseven_zm",
	//"fiveseven_upgraded_zm",
	//"python_zm",
	//"python_upgraded_zm",
	//"ray_gun_zm"

	"AK74u",
	"AK74u Upgraded",
	"MP5",
	"MP5 Upgraded",
	"M14",
	"M14 Upgraded",
	"M16A1",
	"M16A1 Upgraded",
	"M8A1",
	"M8A1 Upgraded",
	"Type-95",
	"Type-95 Upgraded",
	"Galil",
	"Galil Upgraded",
	"FAL",
	"FAL Upgraded",
	"RPD",
	"RPD Upgraded",
	"HAMR",
	"HAMR Upgraded",
	"DSR-50",
	"DSR-50 Upgraded",
	"Five-Seven",
	"Five-Seven Upgraded",
	"Python",
	"Python Upgraded",
	"Ray Gun"
};

BYTE RME_weaponIndexes_ZM[27] = {
	0x02, 0x05, 0x03, 0x06, 0x09, 0x0F, 0x0A, 0x10,
	0x0C, 0x14, 0x0D, 0x16, 0x22, 0x26, 0x23, 0x27,
	0x24, 0x28, 0x25, 0x29, 0x2A, 0x2C, 0x33, 0x3C,
	0x30, 0x39, 0x44
};

char* RME_AntiQuit[2] = {
	"Set",
	"Reset"
};

char* espBoneNames[14] = {
	"j_helmet", "j_neck", "j_head", "j_ankle_le",
	"j_ankle_ri", "j_wrist_le", "j_wrist_ri",
	"j_mainroot", "j_knee_le", "j_knee_ri",
	"j_elbow_le", "j_elbow_ri", "j_shoulder_le",
	"j_shoulder_ri"
};

char* bonesESP[26] = {
	"j_helmet", "j_head", "j_neck",
	"j_shoulder_le", "j_elbow_le", "j_wrist_le", "j_elbow_le", "j_shoulder_le", "j_neck",
	"j_shoulder_ri", "j_elbow_ri", "j_wrist_ri", "j_elbow_ri", "j_shoulder_ri", "j_neck",
	"j_spineupper", "j_spinelower",
	"j_hip_le", "j_knee_le", "j_ankle_le", "j_knee_le", "j_hip_le",
	"j_spinelower",
	"j_hip_ri", "j_knee_ri", "j_ankle_ri"
};



char* modelList_BO2[4] = {
	"german_shepherd",
	"projectile_hellfire_missile",
	"projectile_t6_drone_tank_missile",
	"p6_ship_missile_launcher"
};

short modelListIndex_BO2[4] = { NULL };

/*struct protection_s {
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
*/



#if defined (BO2)
mapNames_s mapNames[15] = {
	"mp_la", "Aftermath",
	"mp_dockside", "Cargo",
	"mp_carrier", "Carrier",
	"mp_drone", "Drone",
	"mp_express", "Express",
	"mp_hijacked", "Hijacked",
	"mp_meltdown", "Meltdown",
	"mp_overflow", "Overflow",
	"mp_nightclub", "Plaza",
	"mp_raid", "Raid",
	"mp_slums", "Slums",
	"mp_village", "Standoff",
	"mp_turbine", "Turbine",
	"mp_socotre", "Yemen",
	"mp_nuketown_2020", "Nuketown 2025"
};
#elif defined (BO1)
mapNames_s mapNames[14] = {
	"mp_array", "Array",
	"mp_cairo", "Havana",
	"mp_cosmodrome", "Launch",
	"mp_cracked", "Cracked",
	"mp_crisis", "Crisis",
	"mp_duga", "Grid",
	"mp_firingrange", "Firing Range",
	"mp_hanoi", "Hanoi",
	"mp_havoc", "Jungle",
	"mp_mountain", "Summit",
	"mp_nuked", "Nuketown",
	"mp_radiation", "Radiation",
	"mp_russianbase", "WMD",
	"mp_villa", "Villa"
};
/*
* tdm - Team Deathmatch
dm - Free-for-All (Deathmatch)
dom - Domination
sd - Search and Destroy
ctf - Capture the Flag
koth - Headquarters
sab - Sabotage
dem - Demolition
sab - Sabotage
dd - One in the Chamber (Wager Match)
gun - Gun Game (Wager Match)
shrp - Sharpshooter (Wager Match)
stick - Sticks and Stones (Wager Match)
tlm - Team Tactical (mixed modes)
ct - Combat Training (against bots)
*/
gameTypes_s gameTypes[8] = {
	"tdm", "Team Deathmatch",
	"dm", "Free-for-All",
	"dom", "Domination",
	"sd", "Search & Destroy",
	"ctf", "Capture the Flag",
	"koth", "Headquarters",
	"sab", "Sabotage",
	"dem", "Demolition"
};

#endif

//char* BO1_mapNames[] = {
//
//};