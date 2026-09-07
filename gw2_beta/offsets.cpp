#include <Main.h>

addr_func of_f;
addr_vars of_v;
addr_mem of_m;

void InitOffsets()
{
	of_f.ExecutionContext_getOptionValue = 0x36000_b;
	of_f.ExecutionContext_addOptions = 0x37AF0_b;

#if FBC_HAS_DEDICATED_SERVER
	of_f.LevelSetup_setInclusionOptions = 0x274DA0_b;
	of_f.Common_applyCmdline = 0x428930_b;

	of_f.Main_spawnServer = 0x25A0_b;
#endif

	//=== hook
	of_f.ExecutionContextCtor = 0x35B00_b;
	of_f.MainCtor = 0x120_b;

	of_f.MemorySocketManagerCtor = 0x64010_b;
	of_f.getServerBackendType = 0xD6EAB0_b;

	of_f.Environment_getHostIdentifier = 0x1A810_b;

	of_f.ServerPlayerManager_addPlayer = 0x4C3BE0_b;
	of_f.ServerPlayer_disconnect = 0x4C12C0_b;
	//===

	of_v.g_emptyString = 0x361B2B4_b;
	of_v.g_emptyArray = 0x361A340_b + 0x10;

	of_m.ptch_Client_initNetwork = 0x2BC986_b;
	of_m.ptch_Client_startServer = 0x2BC761_b;
	of_m.ptch_networkNameToAddress = 0x64978_b;
	of_m.ptch_userNameFormat = 0x279EE02_b;

	of_m.ptch_allowCommandlineSettings = 0x3B5C_b;

#if FBC_HAS_DEDICATED_SERVER
	of_m.ptch_initDedicatedServerPtr = 0x2FC8560_b;
	of_m.ptch_dedicatedServer_1 = 0x55F15_b;
	of_m.ptch_dedicatedServer_2 = 0xD6BC91_b;
#endif

}

