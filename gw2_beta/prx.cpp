#include <Main.h>

UINT64 baseAddress = 0;

static ScePthread g_thread{};
static bool g_thread_created = false;
bool g_exit_requested = false;

void Cleanup()
{
	g_exit_requested = true;

	if (g_thread_created) {
		scePthreadJoin(g_thread, nullptr);
		g_thread_created = false;
	}
	g_thread = nullptr;

	MainCleanup();
}

int main()
{
	static bool s_initialized = false;
	if (s_initialized) return 0;
	s_initialized = true;

	int ret = scePthreadCreate(&g_thread, nullptr, MainPrxThread, nullptr, "gw2Beta_ps4_thread");
	g_thread_created = !ret;
	if (ret) {
		NOTIFY_1("scePthreadCreate failed 0x%x", ret);
		g_thread = nullptr;
	}

	return 0;
}

extern "C"
{
#if IS_GOLDHEN_PLUGIN
	int attr_public plugin_load(int argc, const char* argv[])
	{
		(void)argc;
		(void)argv;
		return main();
	}

	int attr_public plugin_unload(int argc, const char* argv[])
	{
		(void)argc;
		(void)argv;
		Cleanup();
		return 0;
	}
#endif
	int attr_module_hidden module_start(size_t args, const void* argp)
	{
		(void)args;
		(void)argp;
		return main();
	}

	int attr_module_hidden module_stop(size_t args, const void* argp)
	{
		(void)args;
		(void)argp;
		Cleanup();
		return 0;
	}
}

