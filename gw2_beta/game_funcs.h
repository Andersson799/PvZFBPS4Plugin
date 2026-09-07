#pragma once

namespace fbc
{
	char* ec_getOptionValue(const char* optionName, const char* defaultValue = NULL);
	bool AddCmdLineOptionsFromFile(const char* path);

}

