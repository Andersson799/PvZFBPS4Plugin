#include <Main.h>

using namespace fb;
namespace fbc
{
	DFH(INT64, ExecutionContextCtor, INT64 thisPtr, int argc, char** argv) {
		INT64 ret = ExecutionContextCtor(thisPtr, argc, argv);
		if (!AddCmdLineOptionsFromFile("/data/commandline.txt"))
		{
			AddCmdLineOptionsFromFile("/app0/commandline.txt");
		}
		return ret;
	}

	DFH(INT64, MainCtor, INT64 thisPtr, bool isDedicatedServer) {
		g_main->postInit();
		return MainCtor(thisPtr, g_main->isServer());
	}

	DFH(char*, Environment_getHostIdentifier) {
		static char userName[0x40] = { 0 };
		if (!userName[0])
		{
			bool fallback = false;

			int ret = sceUserServiceInitialize(nullptr);
			if (ret < 0 && ret != SCE_USER_SERVICE_ERROR_ALREADY_INITIALIZED) {
				LOG_D("[Environment::getHostIdentifier] sceUserServiceInitialize failed 0x%llX\n", (UINT64)ret);
				fallback = true;
			}
			else {
				SceUserServiceUserId userId = 0;
				if (sceUserServiceGetInitialUser(&userId) != SCE_OK) {
					LOG_D("[Environment::getHostIdentifier] Failed to get userId from sceUserServiceGetInitialUser\n");
					userId = 1000; //shadPS4's default userid
				}

				if (sceUserServiceGetUserName(userId, userName, sizeof(userName)) != SCE_OK) {
					LOG_D("[Environment::getHostIdentifier] Failed to get username from sceUserServiceGetUserName (userId %d)\n", userId);
					fallback = true;
				}
				else
					LOG_D("[Environment::getHostIdentifier] (%d) %s\n", userId, userName);
			}

			if (fallback)
				snprintf(userName, sizeof(userName), "ps4-SVC_DREAUTO_TABU");
		}
		return userName;
	}

	DFH(void, ServerPlayerManager_addPlayer, INT64 thisPtr, ServerPlayer* player, const char* nickName) {
		LOG_D("ServerPlayerManager::addPlayer [Id: %d] %s 0x%llX %d %d %d\n", player->getPlayerId(), nickName, (INT64)player, player->isPersistentAIPlayer(), player->isSpectator(), player->isAIPlayer());
		if (!player->isAIOrPersistentAIPlayer())
			LOG_PF("[Id: %d] %s has joined the server\n", player->getPlayerId(), nickName);
		ServerPlayerManager_addPlayer(thisPtr, player, nickName);
	}

	DFH(void, ServerPlayer_disconnect, ServerPlayer* thisPtr, INT64 reason, const char* reasonText) {
		LOG_PF("[Id: %d] %s has left the server (reason %d %s)\n", thisPtr->getPlayerId(), thisPtr->m_name, (int)reason, reasonText);
		ServerPlayer_disconnect(thisPtr, reason, reasonText);
	}

	void InitGameHooks()
	{
		CREATE_HOOK2(ExecutionContextCtor, ExecutionContextCtor_H, 17);
		CREATE_HOOK2(MainCtor, MainCtor_H, 17);
		
		CREATE_HOOK2(Environment_getHostIdentifier, Environment_getHostIdentifier_H, 17);

		CREATE_HOOK2(ServerPlayerManager_addPlayer, ServerPlayerManager_addPlayer_H, 17);
		//CREATE_HOOK2(ServerPlayer_disconnect, ServerPlayer_disconnect_H, 17); //crash
		LOG_D("[game] hooks installed\n");
	}

}

