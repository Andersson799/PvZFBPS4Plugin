#include <Main.h>

namespace fbc
{
	char* ec_getOptionValue(const char* optionName, const char* defaultValue)
	{
		return CALL_FUNC(char*, of_f.ExecutionContext_getOptionValue, (const char* optionName, const char* defaultValue, int* token), optionName, defaultValue, NULL);
	}

	bool AddCmdLineOptionsFromFile(const char* path)
	{
		char cmdLine[0x1000] = { 0 };
		int fd = sceKernelOpen(path, SCE_KERNEL_O_RDONLY, SCE_KERNEL_S_INONE);
		if (fd >= 0)
		{
			sceKernelRead(fd, cmdLine, sizeof_array(cmdLine));
			sceKernelClose(fd);
			CALL_FUNC(void, of_f.ExecutionContext_addOptions, (bool replace, const char* text), false, cmdLine);
			return true;
		}
		return false;
	}
}


