#pragma once
#include <std_include.h>

#define IS_GOLDHEN_PLUGIN 1
#define IS_SHADPS4 1

#if IS_GOLDHEN_PLUGIN
#define attr_module_hidden __attribute__((weak)) __attribute__((visibility("hidden")))
#define attr_public __attribute__((visibility("default")))
#else
#define attr_module_hidden
#define attr_public
#endif

#define NOTIFY_1(...) { NotifyDevice((char*)"cxml://psnotification/tex_icon_caution", (char*)__VA_ARGS__); printf("[Notify] "); printf(__VA_ARGS__); printf("\n"); }

#define FBC_DEBUG 0
#define FBC_HAS_DEDICATED_SERVER 0
#define GW2_BETA 1

#include <log.h>

#include <ps4/utils.h>
#include <ps4/notify.h>

#include <offsets.h>
#include <engine/GameSDK.h>

#include <game_funcs.h>
#include <game_hooks.h>

extern bool g_exit_requested;

void* MainPrxThread(void* arg);
void MainCleanup();

namespace fbc
{
	void InitMemPatch();

	class Main
	{
	public:
		Main();
		~Main();

		void init();
		void postInit();
		static void initDedicatedServer(void* thisPtr);

		bool isServer() const { return m_isServer; };

		INT64 m_primaryUser{};

		char* m_levelName{};
		char* m_gameMode{};

		bool m_isServer{};
	};
}

extern fbc::Main* g_main;

