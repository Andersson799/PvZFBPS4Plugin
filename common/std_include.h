#pragma once

#include <iostream>
#include <stdio.h>
#include <thread>
#include <atomic>

#include <sys\types.h>
#include <netinet/in.h>
#include <arpa\inet.h>
#include <sys\socket.h>
#include <net.h>
#include <sys\uio.h>
#include "_kernel.h"

#include <kernel.h>
#include <libdbg.h>
#include <libsysmodule.h>
#include <sceerror.h>

#include <user_service.h>

typedef long long INT64;
typedef unsigned short UINT16;
typedef unsigned long long UINT64;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef void VOID;
typedef const char* LPCWSTR;
typedef const char* LPCSTR;
typedef unsigned char UINT8;
typedef unsigned int UINT;
typedef bool BOOL;
typedef unsigned long long DWORD;
typedef unsigned long long ULONG_PTR;
typedef ULONG_PTR DWORD_PTR;
typedef void* PVOID;
typedef int WORD;
typedef size_t SIZE_T;
typedef DWORD* LPDWORD;
typedef unsigned char BYTE;
typedef char CCHAR;
typedef char CHAR;
typedef BYTE* LPBYTE;
typedef unsigned int UINT32;
typedef int INT32;
typedef int8_t INT8;
typedef int16_t INT16;
typedef UINT32* PUINT32;
typedef void* HANDLE;
typedef long LONG;
typedef DWORD* PDWORD;
typedef unsigned long long DWORD64;
typedef unsigned long long ULONG64;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;

#define CREATE_HOOK_ERR_MSG ("Failed to hook %s (0x%llX)\n")

#define DEF_FUNC(type, name, addr, ...) type (*name)(__VA_ARGS__) = reinterpret_cast<type(*)(__VA_ARGS__)>(addr)
#define CALL_FUNC(type, addr, params, ...) reinterpret_cast<type(*)params>(addr)(__VA_ARGS__)

#define DFH(type, name, ...) type (*name)(__VA_ARGS__); type name##_H(__VA_ARGS__)

#define CREATE_HOOK2(func, hook_func, length) func = (decltype(func))DetourFunction(of_f.func, (void*)hook_func, length); \
if (!func) { printf(CREATE_HOOK_ERR_MSG, (#func), of_f.func); } else LOG_D("[hook] %s stub 0x%llX\n", #func, (UINT64)func);

#define SET_FUNC_ADDR(func, addr) func = (decltype(func))addr
#define CLEAR_FUNC_ADDR(func) func = nullptr

extern UINT64 baseAddress;

inline UINT64 operator"" _b(UINT64 val) {
	return baseAddress + val;
}

