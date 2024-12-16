#pragma once

#define docryptoBypass(a,b)  ((QWORD)((QWORD)( ((QWORD)0x610e7a17d2b42090) * ((QWORD)(b)) ) ^ (~(QWORD)(a))))

#if !defined (DEBUG_BUILD)
#define decryptDWORD(a) ((DWORD)(docryptoBypass(a, (QWORD)((((DWORD*)&a - (DWORD*)&Security->addrs))))))
#else
#define	decryptDWORD(a) (a)
#endif

struct addresses_s {
	/*[Hooks]*/
	DWORD SND_EndFrameHook;
	DWORD XamInputGetStateHook;
	DWORD CL_CreateNewCommandsHook;
	DWORD CL_SendCommandHook;
	DWORD CL_MigrationPacketHook;
	DWORD CL_HandleRelayPacketHook;
	DWORD LiveStats_Probation_GiveProbationHook;
	DWORD CL_DispatchConnectionlessPacketHook;
	DWORD memcpyHook;
	DWORD PreGameAntiFreezeHook;
	DWORD sub_821D8A18Hook;
	DWORD DynEntCl_DestroyEventHook;
	DWORD CG_EntityEventHook;
	DWORD LUI_Interface_DrawRectangleHook;
	DWORD SV_ExecuteClientCommandHook;
	DWORD CG_CalcEntityLerpPositionsHook;

	/*[Game Functions]*/
	DWORD R_AddCmdDrawText;
	DWORD R_AddCmdDrawStretchPic;
	DWORD R_TextWidth;
	DWORD R_TextHeight;

	DWORD BG_GetSpreadForWeapon;
	DWORD BG_GetWeaponDef, BG_GetWeaponDef2, BG_WeaponDefPointer;
	DWORD BG_WeaponHasPerk;
	DWORD BG_AdvanceTrace;
	DWORD BG_GetSurfacePenetrationDepth;
	DWORD BG_UsingVehicleWeapon;

	DWORD Material_RegisterHandle;
	DWORD CL_RegisterFont;

	DWORD CG_DrawRotatedPicPhysical;
	DWORD CG_DObjGetWorldTagPos;
	DWORD CG_GetPlayerViewOrigin;
	DWORD CG_LocationalTrace;
	DWORD CG_GetWeaponIndexForName;
	DWORD CG_IsEntityFriendlyNotEnemy;
	DWORD CG_GameMessage;
	DWORD CG_BoldGameMessage;

	DWORD va;

#if defined (BO2)
	DWORD Dvar_FindVar;
#endif

#if defined (BO1) || defined (BO1_ZM)
	DWORD cl_ingame;
	DWORD ui_gametype;
	DWORD party_hostname;
#endif;

	DWORD CL_AllLocalClientsDisconnected;
	DWORD CL_AddReliableCommand;
	DWORD CL_GetScreenDimensions;

	DWORD Cbuf_AddText;

	DWORD Com_GetClientDObj;
	DWORD SL_GetString;
	DWORD TransformSeed;
	DWORD RandomBulletDir;
	DWORD sub_82697FC0;
	DWORD FireBulletPenetrate;
	DWORD Bullet_Trace;
	DWORD Trace_GetEntityHitId;

	DWORD NET_OutOfBandPrint;
	DWORD NET_OutOfBandData;
	DWORD NET_SendPacket;
	DWORD Live_GetCurrentSession;
	DWORD Live_GetPlayerNetAddr;

#if defined (BO2)
	DWORD Com_SessionMode_IsZombiesGame;
#endif

	DWORD MSG_Init;
	DWORD MSG_WriteString;
	DWORD MSG_WriteLong;
	DWORD MSG_WriteInt64;
	DWORD MSG_WriteByte;
	DWORD MSG_WriteShort;
	DWORD MSG_WriteFloat;
	DWORD MSG_ReadByte;
	DWORD MSG_ReadFloat;
	DWORD MSG_WriteData;
	DWORD MSG_ReadLong;
	DWORD MSG_ReadInt64;
	DWORD MSG_ReadShort;

	DWORD Cmd_Argv;

	DWORD SV_Cmd_TokenizeString;
	DWORD ClientCommand;
	DWORD SV_Cmd_EndTokenizedString;

#if defined (BO2)
	DWORD g_partyData;
	DWORD g_lobbyData;
#elif defined (BO1) || defined (BO1_ZM)
	DWORD g_partyMember;
	DWORD g_partyMemberSize;
#endif

	/*[Game Addresses]*/
	DWORD ScreenPlacement;
	DWORD ServerID;
	DWORD NoRecoil;
	DWORD NoSway1;
	DWORD NoFlinch1, NoFlinch2;
	DWORD NoBob;
	DWORD Radar;
	DWORD Laser;
	DWORD FOV;
#if defined (BO1) || defined (BO1_ZM)
	DWORD ThirdPersonView;
#endif
	DWORD ThirdPersonRange;
	DWORD CG_DrawOverheadNames;
	DWORD NoBulletImpacts;
	DWORD JoinInProgressAllowed;

	DWORD CrashFix;

	DWORD Autowall[8];

	DWORD EmptyMemory;
	DWORD XamGetUserName;
	DWORD ErrorPatch[2];
	DWORD GT_Addrs[2];

#if defined (BO2)
	DWORD ZombieRank[3];
#endif

	DWORD AntiKick;
#if defined (BO2)
	DWORD InfiniteClassItems;
	DWORD Content_DoesAnyoneHaveMTX;
	DWORD BG_UnlockablesItemOptionLocked;

	DWORD NewCrash[9];

	DWORD DynEntCl_Ptr;
	DWORD XMemCpy_Ptr;

	DWORD LUI_RenderingImmediately;
#endif

	DWORD InvalidGamertagError;
	DWORD CheatWriteProtection;

#if defined (BO1) || defined (BO1_ZM)
	DWORD ViewOriginError;
#endif

	/*[Structures]*/
	DWORD strct_uiContext;
	DWORD strct_clientActive;
	DWORD strct_centity;
	DWORD strct_cgArray;
	DWORD strct_cgsArray;
	DWORD strct_clientUIActive;
	DWORD strct_gEntity;
	DWORD strct_gEntity_Size;
	DWORD strct_gClient;
	DWORD strct_gClient_Size;
	DWORD strct_level_locals_t;

	DWORD cmdInfoAddrs;
	DWORD cmdInfoSizeAddrs;

	DWORD strct_clientConnection;
	DWORD strct_netchanAdditive;

	DWORD rme_fragmentIndex;
	DWORD rme_fragmentSize;
	DWORD rme_fragmentLength;

	DWORD gClient_gEntity_Addrs;

	DWORD strct_gClient_t;
	DWORD strct_gClient_t_Size;

	DWORD RELOAD_ADD_AMMO;
	DWORD PUTAWAY_WEAPON;
};

class cSecurity {
public:
	BOOL bHasPatches;
	addresses_s addrs;

	BOOL SetAddresses(PBYTE data) {
#if defined (DEBUG_BUILD)
#if defined(BO2)
		/*[Hooks]*/
		addrs.SND_EndFrameHook = 0x828B9F58;
		addrs.XamInputGetStateHook = 0x827D8A48;
		addrs.CL_CreateNewCommandsHook = 0x82261A40;
		addrs.CL_SendCommandHook = 0x8225EAA8;
		addrs.LiveStats_Probation_GiveProbationHook = 0x825C5330;
		addrs.CL_MigrationPacketHook = 0x8227BBA0;
		addrs.CL_HandleRelayPacketHook = 0x82273B20;
		addrs.CL_DispatchConnectionlessPacketHook = 0x82277888;
		addrs.CG_EntityEventHook = 0x821D3D78;
		addrs.memcpyHook = 0x827D33F0;
		addrs.PreGameAntiFreezeHook = 0x82717AE0;
		addrs.sub_821D8A18Hook = 0x821D8A18;
		addrs.DynEntCl_DestroyEventHook = 0x822DDE20;
		addrs.LUI_Interface_DrawRectangleHook = 0x82717790;
		addrs.SV_ExecuteClientCommandHook = 0x8242D410;
		addrs.CG_CalcEntityLerpPositionsHook = 0x821D0D18;

		/*[Game Functions]*/
		addrs.R_AddCmdDrawText = 0x828B8BA0;
		addrs.R_AddCmdDrawStretchPic = 0x828B86C0;
		addrs.R_TextWidth = 0x828B6FD8;
		addrs.R_TextHeight = 0x82490390;

		addrs.BG_GetSpreadForWeapon = 0x826BB4E0;
		addrs.BG_GetWeaponDef = 0x826BF988;
		addrs.BG_GetWeaponDef2 = 0x826BF970;
		addrs.BG_WeaponDefPointer = 0x845CA998;
		addrs.BG_WeaponHasPerk = 0x82691798;
		addrs.BG_AdvanceTrace = 0x826B0148;
		addrs.BG_GetSurfacePenetrationDepth = 0x826B0278;
		addrs.BG_UsingVehicleWeapon = 0x826B5048;

		addrs.Material_RegisterHandle = 0x828B78F0;
		addrs.CL_RegisterFont = 0x82275F78;

		addrs.CG_DrawRotatedPicPhysical = 0x821C7F58;
		addrs.CG_DObjGetWorldTagPos = 0x821D03F0;
		addrs.CG_GetPlayerViewOrigin = 0x822544B0;
		addrs.CG_LocationalTrace = 0x8225C568;
		addrs.CG_GetWeaponIndexForName = 0x826C06E8;
		addrs.CG_IsEntityFriendlyNotEnemy = 0x821CD948;
		addrs.CG_GameMessage = 0x821E6518;
		addrs.CG_BoldGameMessage = 0x821E6530;

		addrs.va = 0x8249E440;

		addrs.Dvar_FindVar = 0x82496430;

		addrs.CL_AllLocalClientsDisconnected = 0x827504D0;
		addrs.CL_AddReliableCommand = 0x822786E0;
		addrs.CL_GetScreenDimensions = 0x82261D30;

		addrs.Cbuf_AddText = 0x824015E0;

		addrs.Com_GetClientDObj = 0x82414578;
		addrs.SL_GetString = 0x82533528;
		addrs.TransformSeed = 0x826961B8;
		addrs.RandomBulletDir = 0x82696250;
		addrs.sub_82697FC0 = 0x82697FC0;
		addrs.FireBulletPenetrate = 0x82258840;
		addrs.Bullet_Trace = 0x8224CDA8;
		addrs.Trace_GetEntityHitId = 0x823F7160;

		addrs.NET_OutOfBandPrint = 0x8241D580;
		addrs.NET_OutOfBandData = 0x8241D6B8;
		addrs.NET_SendPacket = 0x8241D3C0;
		addrs.Live_GetCurrentSession = 0x8259B878;
		addrs.Live_GetPlayerNetAddr = 0x825B70B8;

		addrs.Com_SessionMode_IsZombiesGame = 0x82406578;

		addrs.MSG_Init = 0x824191F8;
		addrs.MSG_WriteString = 0x824172E8;
		addrs.MSG_WriteLong = 0x8241A440;
		addrs.MSG_WriteInt64 = 0x82417280;
		addrs.MSG_WriteByte = 0x82416D98;
		addrs.MSG_WriteShort = 0x8241A3D8;
		addrs.MSG_WriteFloat = 0x82416E30;
		addrs.MSG_ReadByte = 0x824183B8;
		addrs.MSG_ReadFloat = 0x82417908;
		addrs.MSG_WriteData = 0x82416DD0;
		addrs.MSG_ReadLong = 0x824175E8;
		addrs.MSG_ReadInt64 = 0x82417718;
		addrs.MSG_ReadShort = 0x82417520;

		addrs.Cmd_Argv = 0x821BBBD8;

		addrs.SV_Cmd_TokenizeString = 0x824021D0;
		addrs.ClientCommand = 0x823246F0;
		addrs.SV_Cmd_EndTokenizedString = 0x82402200;

		addrs.g_partyData = 0x82CA0898;
		addrs.g_lobbyData = 0x82CAC160;

		//BO1
		/*addrs.g_partyData = 0x829F0350;
		addrs.g_lobbyData = 0x829F7038;
		size 0x6798 partyData
		*/

		/*[Game Addresses]*/
		addrs.ScreenPlacement = 0x82CBC168;
		addrs.ServerID = 0x82C15758;

		addrs.NoRecoil = 0x82259BC8;
		addrs.NoSway1 = 0x826C6E6C;
		addrs.NoFlinch1 = 0x826C7A64;
		addrs.NoFlinch2 = 0x8223FE80;
		addrs.NoBob = 0x82093600;
		addrs.Radar = 0x821B8FD4;
		addrs.Laser = 0x82255E1C;
		addrs.FOV = 0x82BC2774;
		addrs.ThirdPersonRange = 0x82BBC584;
		addrs.CG_DrawOverheadNames = 0x821C3C74;
		addrs.NoBulletImpacts = 0x82259B40;
		addrs.JoinInProgressAllowed = 0x82CB7518;

		addrs.CrashFix = 0x8295FFF8;

		addrs.Autowall[0] = 0x822DFB90;
		addrs.Autowall[1] = 0x82258520;
		addrs.Autowall[2] = 0x82257E30;
		addrs.Autowall[3] = 0x82258CE4;
		addrs.Autowall[4] = 0x82258FAC;
		addrs.Autowall[5] = 0x8225900C;
		addrs.Autowall[6] = 0x82258D60;
		addrs.Autowall[7] = 0x82259B40;

		addrs.EmptyMemory = 0x82C55D00;
		addrs.XamGetUserName = 0x8293D724;
		addrs.ErrorPatch[0] = 0x8259B6A7;
		addrs.ErrorPatch[1] = 0x822D1110;
		addrs.GT_Addrs[0] = 0x82C55D60;
		addrs.GT_Addrs[1] = 0x841E1B30;

		addrs.ZombieRank[0] = 0x84348A27;
		addrs.ZombieRank[1] = 0x84348C0B;
		addrs.ZombieRank[2] = 0x84348C13;

		addrs.AntiKick = 0x822781D8;

		addrs.InfiniteClassItems = 0x826A5FBC;
		addrs.Content_DoesAnyoneHaveMTX = 0x82413340;
		addrs.BG_UnlockablesItemOptionLocked = 0x826A3A90;
		addrs.NewCrash[0] = 0x822A0638;
		addrs.NewCrash[1] = 0x822A0680;
		addrs.NewCrash[2] = 0x822A063C;
		addrs.NewCrash[3] = 0x82273C88;
		addrs.NewCrash[4] = 0x822781D8;
		addrs.NewCrash[5] = 0x822D1C58;
		addrs.NewCrash[6] = 0x82015F04;
		addrs.NewCrash[7] = 0x82273B20;
		addrs.NewCrash[8] = 0x8241E79C;
		addrs.DynEntCl_Ptr = 0x83798218;
		addrs.XMemCpy_Ptr = 0x82717C8C;
		addrs.LUI_RenderingImmediately = 0x8270E45C;

		addrs.InvalidGamertagError = 0x825DE240;
		addrs.CheatWriteProtection = 0x82498084;

		/*[Structures]*/
		addrs.strct_uiContext = 0x83BA29F0;
		addrs.strct_clientActive = 0x82C70F4C;
		addrs.strct_centity = 0x82BBC554;
		addrs.strct_cgArray = 0x82BBAE68;
		addrs.strct_cgsArray = 0x82BBAE44;
		addrs.strct_clientUIActive = 0x82C6FDC8;

		addrs.strct_gClient = 0x83551A10;
		addrs.strct_gClient_Size = 0x57F8;

		addrs.strct_gEntity = 0x833D0640;
		addrs.strct_gEntity_Size = 0x31C;

		addrs.strct_level_locals_t = 0x83547A00;

		addrs.cmdInfoAddrs = 0x837B3F50;
		addrs.cmdInfoSizeAddrs = 0x837F683C;

		addrs.strct_clientConnection = 0x82C70F50;
		addrs.strct_netchanAdditive = 0x24350;

		addrs.rme_fragmentIndex = 0xA;
		addrs.rme_fragmentSize = 0x9A2D;
		addrs.rme_fragmentLength = 0x6;

		addrs.gClient_gEntity_Addrs = 0x83557000;
		//addrs.gClient_gEntity_Addrs = 0x83416720;

		addrs.strct_gClient_t = 0x83B50F40;
		addrs.strct_gClient_t_Size = 0x4E100;

		addrs.RELOAD_ADD_AMMO = 23;
		addrs.PUTAWAY_WEAPON = 29;

		bHasPatches = TRUE;
#elif defined(BO1)
        /*[Hooks]*/
        addrs.SND_EndFrameHook = 0x82234B00;
		addrs.XamInputGetStateHook = 0x82572718;
		addrs.CL_CreateNewCommandsHook = 0x8223BED8;
		addrs.CL_SendCommandHook = 0x8223B8C8;
		addrs.CL_MigrationPacketHook = 0x8224A560;
		addrs.CG_EntityEventHook = 0x821C0818;
		addrs.SV_ExecuteClientCommandHook = 0x823637F0;
		addrs.CG_CalcEntityLerpPositionsHook = 0x821BDE50;
		addrs.CL_DispatchConnectionlessPacketHook = 0x82246120;

		/*[Game Functions]*/
		addrs.R_AddCmdDrawText = 0x8263EF00;
		addrs.R_AddCmdDrawStretchPic = 0x8263EA68;
		addrs.R_TextWidth = 0x8263D378;
		addrs.R_TextHeight = 0x823F6998;

		addrs.BG_GetSpreadForWeapon = 0x82186940;
		addrs.BG_GetWeaponDef = 0x821919D8;
		addrs.BG_UsingVehicleWeapon = 0x82187188;

		addrs.Material_RegisterHandle = 0x82624B78;
		addrs.CL_RegisterFont = 0x82243FA0;

		addrs.CG_DrawRotatedPicPhysical = 0x821B86A8;
		addrs.CG_DObjGetWorldTagPos = 0x821BDB10;
		addrs.CG_GetPlayerViewOrigin = 0x82223E48;
		addrs.CG_LocationalTrace = 0x822296E0;
		addrs.CG_GetWeaponIndexForName = 0x82191FE0;
		addrs.CG_GameMessage = 0x821CD128;

		addrs.va = 0x823EECA0;

		addrs.cl_ingame = 0x829D557C;
		addrs.ui_gametype = 0x8391111C;
		addrs.party_hostname = 0x829F6B4C;

		addrs.CL_AllLocalClientsDisconnected = 0x8255E2D0;
		addrs.CL_AddReliableCommand = 0x822408E8;
		addrs.CL_GetScreenDimensions = 0x8222ABD8;

		addrs.Cbuf_AddText = 0x8233E8D8;

		addrs.Com_GetClientDObj = 0x82346710;
		addrs.SL_GetString = 0x824612D0;
		addrs.RandomBulletDir = 0x822AC660;
		addrs.FireBulletPenetrate = 0x822252F8;

		addrs.NET_OutOfBandPrint = 0x8234F238;
		addrs.NET_OutOfBandData = 0x8234F368;
		addrs.NET_SendPacket = 0x8234F0F0;
		addrs.Live_GetCurrentSession = 0x824F3388;
		addrs.Live_GetPlayerNetAddr = 0x824F5648;

		addrs.MSG_Init = 0x8234D788;
		addrs.MSG_WriteString = 0x8234A380;
		addrs.MSG_WriteLong = 0x8234A2B0;
		//addrs.MSG_WriteInt64 = 0x82417280;
		addrs.MSG_WriteByte = 0x8234A1E8;
		addrs.MSG_WriteShort = 0x8234A280;
		//addrs.MSG_WriteFloat = 0x82416E30;
		addrs.MSG_ReadByte = 0x8234A6C0;
		//addrs.MSG_ReadFloat = 0x82417908;
		addrs.MSG_WriteData = 0x8234A220;
		addrs.MSG_ReadLong = 0x8234A7C8;
		//addrs.MSG_ReadInt64 = 0x82417718;
		addrs.MSG_ReadShort = 0x8234A720;

		addrs.Cmd_Argv = 0x82164040;

		addrs.SV_Cmd_TokenizeString = 0x8233F450;
		addrs.ClientCommand = 0x822C0238;
		addrs.SV_Cmd_EndTokenizedString = 0x8233F480;

		addrs.g_partyMember = 0x829F8450;
		addrs.g_partyMemberSize = 0x200;

		/*[Game Addresses]*/
		addrs.ScreenPlacement = 0x82A06718;
		addrs.ServerID = 0x829BE624;

		addrs.NoRecoil = 0x82227624;
		addrs.NoSway1 = 0x82195200;
		addrs.NoFlinch1 = 0x821E0824;
		addrs.NoBob = 0x82854914;
		addrs.Radar = 0x821A81A0;
		addrs.FOV = 0x82968374;
		addrs.ThirdPersonView = 0x828D7118;
		addrs.ThirdPersonRange = 0x829680DC;
		addrs.CG_DrawOverheadNames = 0x821B4AC8;
		addrs.JoinInProgressAllowed = 0x829FD840;

		addrs.CrashFix = 0x826DFFF8;

		addrs.Autowall[0] = 0x82223DA8;
		addrs.Autowall[1] = 0x82292230;
		addrs.Autowall[2] = 0x82225C88;
		addrs.Autowall[3] = 0x82225570;

		addrs.EmptyMemory = 0x81B69F80;
		addrs.XamGetUserName = 0x816DD040;
		addrs.ErrorPatch[0] = 0x824FA18C;
		addrs.GT_Addrs[0] = 0x81B69E94;
		addrs.GT_Addrs[1] = 0x841987D5;

		addrs.AntiKick = 0x822468CC;

		addrs.InvalidGamertagError = 0x824FA18C;
		addrs.CheatWriteProtection = 0x823E24E8;
		addrs.ViewOriginError = 0x82223EE4;

		/*[Structures]*/
		addrs.strct_uiContext = 0x83911158;
		addrs.strct_clientActive = 0x829D9728;
		addrs.strct_centity = 0x8296808C;
		addrs.strct_cgArray = 0x82964EA0;
		addrs.strct_cgsArray = 0x82964E64;
		addrs.strct_clientUIActive = 0x829D5580;

		addrs.strct_gClient = 0x8303A608;
		addrs.strct_gClient_Size = 0x2A28;

		addrs.strct_gEntity = 0x82EDE540;
		addrs.strct_gEntity_Size = 0x2F8;

		addrs.strct_level_locals_t = 0x83036D80;

		addrs.cmdInfoAddrs = 0x8313CDF0;
		addrs.cmdInfoSizeAddrs = 0x8317F6DC;

		addrs.strct_clientConnection = 0x829D972C;
		addrs.strct_netchanAdditive = 0x24354;

		addrs.rme_fragmentIndex = 0xC;
		addrs.rme_fragmentSize = 0x8864;
		addrs.rme_fragmentLength = 0x4;

		addrs.gClient_gEntity_Addrs = 0x831758F8;

		addrs.strct_gClient_t = 0x8356CA9C;
		addrs.strct_gClient_t_Size = 0x65070;

		addrs.RELOAD_ADD_AMMO = 23;
		addrs.PUTAWAY_WEAPON = 26;

		bHasPatches = TRUE;

#elif defined (BO1_ZM)

        /*[Hooks]*/
        addrs.SND_EndFrameHook = 0x821CD420;
        addrs.XamInputGetStateHook = 0x824BEED8;
        addrs.CL_CreateNewCommandsHook = 0x823D3910;
        addrs.CL_SendCommandHook = 0x823D3348;
        addrs.CG_EntityEventHook = 0x821765C0;
		addrs.SV_ExecuteClientCommandHook = 0x823BF858;
		addrs.CG_CalcEntityLerpPositionsHook = 0x82173918;
		addrs.memcpyHook = 0x824BD060;

        /*[Game Functions]*/
        addrs.R_AddCmdDrawText = 0x82586DC0;
        addrs.R_AddCmdDrawStretchPic = 0x82586928;
        addrs.R_TextWidth = 0x82584A90;
        addrs.R_TextHeight = 0x82455880;

        addrs.BG_GetSpreadForWeapon = 0x8213AB60;
        addrs.BG_GetWeaponDef = 0x82144C48;
		addrs.BG_UsingVehicleWeapon = 0x8213B228;

        addrs.Material_RegisterHandle = 0x82585308;
        addrs.CL_RegisterFont = 0x823D7A00;

        addrs.CG_DrawRotatedPicPhysical = 0x8216CD58;
        addrs.CG_DObjGetWorldTagPos = 0x82172E88;
        addrs.CG_GetPlayerViewOrigin = 0x821BF578;
        addrs.CG_LocationalTrace = 0x821C4288;
        addrs.CG_GetWeaponIndexForName = 0x82145270;
        addrs.CG_GameMessage = 0x823FBED0;

        addrs.va = 0x82385D30;

        addrs.cl_ingame = 0x83632DE4;
        addrs.ui_gametype = 0x832D3FC4;
        addrs.party_hostname = 0x83649A50;

        addrs.CL_AllLocalClientsDisconnected = 0x821D66C8;
        addrs.CL_AddReliableCommand = 0x823D3C90;
        addrs.CL_GetScreenDimensions = 0x823CA568;

        addrs.Cbuf_AddText = 0x8230FD58;

        addrs.Com_GetClientDObj = 0x82317D10;
        addrs.SL_GetString = 0x8242A5D0;
        addrs.RandomBulletDir = 0x82245BF0;
        addrs.FireBulletPenetrate = 0x822252F8; //not updated

        addrs.NET_OutOfBandPrint = 0x82414E78;
        addrs.NET_OutOfBandData = 0x82414FA8;
        addrs.NET_SendPacket = 0x82414D30;
        addrs.Live_GetCurrentSession = 0x8246E950;
        addrs.Live_GetPlayerNetAddr = 0x82470B00;

        addrs.MSG_Init = 0x82410570;
        addrs.MSG_WriteString = 0x8240F8B8;
        addrs.MSG_WriteLong = 0x8240F788;
        //addrs.MSG_WriteInt64 = 0x82417280;
        addrs.MSG_WriteByte = 0x8240F6C0;
        addrs.MSG_WriteShort = 0x8240F758;
        //addrs.MSG_WriteFloat = 0x82416E30;
        addrs.MSG_ReadByte = 0x8240FAB0;
        //addrs.MSG_ReadFloat = 0x82417908;
        addrs.MSG_WriteData = 0x8240F6F8;
        addrs.MSG_ReadLong = 0x8240FBB8;
        //addrs.MSG_ReadInt64 = 0x82417718;
        addrs.MSG_ReadShort = 0x8240FB10;

        addrs.g_partyMember = 0x8364B1B8;
        addrs.g_partyMemberSize = 0x2B8;

        /*[Game Addresses]*/
        addrs.ScreenPlacement = 0x828398F8;
        addrs.ServerID = 0x8361983C;

        addrs.NoRecoil = 0x821C1024;
        addrs.NoSway1 = 0x821480A8;
        addrs.NoFlinch1 = 0x8218AC14;
        addrs.NoBob = 0x8278E608;
        addrs.Radar = 0x00; //doesnt exist
        addrs.FOV = 0x836DD010;
        addrs.ThirdPersonView = 0x8364DCD0;
        addrs.ThirdPersonRange = 0x836DCE44;
        addrs.CG_DrawOverheadNames = 0x8216AA84;

        addrs.CrashFix = 0x826DFFF8;

        addrs.Autowall[0] = 0x82223DA8;
        addrs.Autowall[1] = 0x82292230;
        addrs.Autowall[2] = 0x82225C88;
        addrs.Autowall[3] = 0x82225570;

        addrs.EmptyMemory = 0x81B69F80;
        addrs.XamGetUserName = 0x816DD040;
        addrs.ErrorPatch[0] = 0x82475514;
        addrs.GT_Addrs[0] = 0x81B69E94;

        addrs.AntiKick = 0x00; //

        addrs.InvalidGamertagError = 0x82475514;
        addrs.CheatWriteProtection = 0x823793C8;
        addrs.ViewOriginError = 0x821BF614;

        /*[Structures]*/
        addrs.strct_uiContext = 0x832D4000;
        addrs.strct_clientActive = 0x8363338C;
        addrs.strct_centity = 0x836DCDDC;
        addrs.strct_cgArray = 0x836DBAAC;
        addrs.strct_cgsArray = 0x836DBA7C;
        addrs.strct_clientUIActive = 0x83632DE8;

        addrs.strct_gClient = 0x82DC1828;
        addrs.strct_gClient_Size = 0x1D28;

        addrs.strct_gEntity = 0x82C32558;
        addrs.strct_gEntity_Size = 0x34C;

		addrs.strct_level_locals_t = 0x82DBBF98;

        addrs.strct_clientConnection = 0x83633390;
        addrs.strct_netchanAdditive = 0x20150;

		addrs.rme_fragmentIndex = 0x8;
		addrs.rme_fragmentSize = 0x88FE;
		addrs.rme_fragmentLength = 0x4;

        addrs.gClient_gEntity_Addrs = 0x82EFCB18;

		addrs.strct_gClient_t = 0x8358B79C;
		addrs.strct_gClient_t_Size = 0x3DCF8;

        addrs.RELOAD_ADD_AMMO = 23;
        addrs.PUTAWAY_WEAPON = 26;

		bHasPatches = TRUE;
#endif
#else
		if (data) {
			memcpy(&addrs, data, sizeof(addresses_s));
			bHasPatches = TRUE;
			return TRUE;
		}
		else {
			bHasPatches = FALSE;
			return FALSE;
		}
#endif

		return TRUE;
	}

	VOID DissolveAddresses() {
		if (bHasPatches) {
			XeCryptRandom((PBYTE)&addrs, sizeof(addresses_s));
			bHasPatches = FALSE;
		}
	}

#if defined (DUMP_BINS)
	void binToFile(const char* fname, PVOID dPtr, ULONG len);
	void DumpPatchBinary();
#endif
};

extern cSecurity* Security;
extern MemoryManager *mManager;