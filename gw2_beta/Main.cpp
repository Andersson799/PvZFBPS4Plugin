#include <Main.h>

using namespace fb;

fbc::Main* g_main;

namespace fbc
{
	Main::Main()
	{
	}

	Main::~Main()
	{
	}

	void Main::init()
	{
		InitOffsets();
		InitMemPatch();
		InitGameHooks();
	}

	void Main::postInit()
	{
#if FBC_HAS_DEDICATED_SERVER
		g_main->m_isServer = ec_getOptionValue("server");
		if (g_main->isServer()) {
			LOG_D("Setting server mode\n");
			UINT64 initDedicatedServerPtr = reinterpret_cast<UINT64>(&Main::initDedicatedServer);
			//MemPatch(of_m.ptch_initDedicatedServerPtr, (uint8_t*)&initDedicatedServerPtr, 8);
			memcpy((void*)of_m.ptch_initDedicatedServerPtr, (uint8_t*)&initDedicatedServerPtr, 8);

			MemSet(of_m.ptch_dedicatedServer_1, 0x90, 2); //int 41

			MemSet(of_m.ptch_dedicatedServer_2, 0x00, 1); //fix arena for local server backend
		}
#endif
	}

	void Main::initDedicatedServer(void* thisPtr)
	{
#if FBC_HAS_DEDICATED_SERVER
		LOG_PF("Starting Dedicated Server...\n");

		LevelSetup levelSetup;

		// for some reason manually setting the game mode screws up game mode loading, resulting on the client getting stuck on black screen.
		// perhaps the game already handles it?

		ServerSpawnInfo* info = new ServerSpawnInfo(&levelSetup);
		info->isDedicated = true;
		info->saveData = (void*)of_v.g_emptyString;

		CALL_FUNC(void, of_f.Main_spawnServer, (void* thisPtr, ServerSpawnInfo* info), thisPtr, info);
#endif
	}

	void InitMemPatch()
	{
		{
			BYTE ptch[] = { 0xE9, 0x0B, 0x0A, 0x00, 0x00 };
			MemPatch(of_f.MemorySocketManagerCtor, ptch, sizeof(ptch)); //replace SocketManager
		}
		{
			BYTE moveax3ret[] = { 0xB8, 0x03, 0x00, 0x00, 0x00, 0xC3 };
			MemPatch(of_f.getServerBackendType, moveax3ret, sizeof(moveax3ret)); //ret 3
		}
		
		MemSet(of_m.ptch_Client_initNetwork, 0xEB, 1); //force socketMode Local
		MemSet(of_m.ptch_Client_startServer, 0xEB, 1); //force localhost ip addr
		MemSet(of_m.ptch_networkNameToAddress, 0xEB, 1); //force INADDR_ANY in fb::networkNameToAddress

		MemSet(of_m.ptch_userNameFormat, 0x00, 1); //change player name format str from %s_%u to %s

		{
			BYTE ptch[] = { 0x48, 0x8D, 0x35, 0x0C, 0xA5, 0x6B, 0x02 };
			MemPatch(of_m.ptch_allowCommandlineSettings, ptch, sizeof(ptch)); //allowCommandlineSettings true
		}
	}
}

void* MainPrxThread(void* arg)
{
	(void)arg;

	NOTIFY_1("Initializing gw2Beta project by And799");

	if (g_exit_requested)
		return nullptr;

	baseAddress = PS_BASE_ADDR;

	g_main = new fbc::Main();
	g_main->init();

	//scePthreadExit(nullptr);
	return nullptr;
}

void MainCleanup()
{
	if (g_main) delete g_main;
}
