#pragma once

#define EAArrayCount(x) (sizeof(x) / sizeof(x[0]))
#define sizeof_array(array) static_cast<int>(EAArrayCount(array))

namespace fb
{
	class ITypedObject
	{
	public:
		virtual void* getType() = 0;
	};

	class Message : ITypedObject
	{
	public:
		int m_category;
		int m_type;
		int m_localPlayerId;

	};

    class String
    {
    public:
        char* m_chars;

        String()
        {
            m_chars = (char*)of_v.g_emptyString;
        }
        ~String()
        {
        }
    };

    struct LevelSetupOption
    {
        const char* m_criterion;
        const char* m_value;
    };

    struct LevelSetup {
        LevelSetup()
        {
        }

        void setInclusionOptions(const char* inclusionCriteria) {
            CALL_FUNC(void, of_f.LevelSetup_setInclusionOptions, (LevelSetup* thisPtr, const char* inclusionCriteria), this, inclusionCriteria);
        }

        fb::String Name;
        LevelSetupOption* InclusionOptions = (LevelSetupOption*)(of_v.g_emptyArray);
        unsigned int DifficultyIndex = 0;
        fb::String StartPoint;
        fb::String unkStr1;
        fb::String LoadScreen_GameMode;
        fb::String LoadScreen_LevelName;
        fb::String LoadScreen_LevelDescription;
        fb::String LoadScreen_UIAssetPath;
        bool unkBool1 = false;
        bool unkBool2 = false;
        bool ForceReloadResources = false;
    };

    struct ServerSpawnInfo
    {
        ServerSpawnInfo(LevelSetup* setup)
            : levelSetup(setup)
        {
        }

        void* fileSystem = nullptr;
        void* damageArbitrator = nullptr;
        void* playerManager = nullptr;
        LevelSetup* levelSetup;
        unsigned int tickFrequency = 0;
        bool isSinglePlayer = false;
        bool isLocalHost = false;
        bool isDedicated = false;
        bool isEncrypted = false;
        bool isCoop = false;
        bool isMenu = false;
        bool keepResources = false;
        void* saveData = nullptr;
        void* serverCallbacks = nullptr;
        void* runtimeModules = nullptr;
        void* loadInfo = nullptr;
        unsigned int serverPort = 0;
        unsigned int validLocalPlayerMask = 1;
    };

    class ServerPlayer
    {
    public:
        char pad_01[0x18]; //0x0000
        const char* m_name; //0x0018
        char pad_02[0x18]; //0x0020
        UINT64 m_onlineId; //0x0038
        char pad_03[0x1378]; //0x0040
        bool m_isPersistentAIPlayer; //0x13B8
        bool m_isSpectator; //0x13B9
        bool m_isAIPlayer; //0x13BA

        unsigned int getPlayerId()
        {
            return *(unsigned int*)((INT64)this + 0x14B0);
        }

        bool isAIPlayer()
        {
            return m_isAIPlayer;
        }

        bool isSpectator()
        {
            return m_isSpectator;
        }

        bool isPersistentAIPlayer()
        {
            return m_isPersistentAIPlayer;
        }

        bool isAIOrPersistentAIPlayer()
        {
            return isAIPlayer() || isPersistentAIPlayer();
        }

    };
}
