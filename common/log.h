#pragma once

#define LOG_PF(...) printf(__VA_ARGS__)
#if FBC_DEBUG
#define LOG_D(...) printf(__VA_ARGS__)
#else
#define LOG_D(...) ((void)0)
#endif
