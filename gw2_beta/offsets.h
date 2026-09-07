#pragma once

void InitOffsets();

struct addr_func
{
	INT64 ExecutionContext_getOptionValue;
	INT64 ExecutionContext_addOptions;

	INT64 LevelSetup_setInclusionOptions;
	INT64 Common_applyCmdline;

	INT64 Main_spawnServer;

	//=== hook
	INT64 ExecutionContextCtor;
	INT64 MainCtor;

	INT64 MemorySocketManagerCtor;
	INT64 getServerBackendType;

	INT64 Environment_getHostIdentifier;

	INT64 ServerPlayerManager_addPlayer;
	INT64 ServerPlayer_disconnect;
	//===

};

struct addr_vars
{
	INT64 g_emptyString;
	INT64 g_emptyArray;
};

struct addr_mem
{
	INT64 ptch_Client_initNetwork;
	INT64 ptch_Client_startServer;
	INT64 ptch_networkNameToAddress;
	INT64 ptch_userNameFormat;

	INT64 ptch_allowCommandlineSettings;

	INT64 ptch_initDedicatedServerPtr;
	INT64 ptch_dedicatedServer_1;
	INT64 ptch_dedicatedServer_2;
};

extern addr_func of_f;
extern addr_vars of_v;
extern addr_mem of_m;

