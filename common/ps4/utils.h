#pragma once

typedef int	            vm_prot_t;
#if !IS_SHADPS4
#define PS_BASE_ADDR    0x400000
#else
#define PS_BASE_ADDR    FindExecutableBase()
#endif

#define HOOK_LENGTH     14
#define VM_PROT_COPY    ((vm_prot_t) 0x08)
#define VM_PROT_NONE    ((vm_prot_t) 0x00)
#define VM_PROT_READ    ((vm_prot_t) 0x01)
#define VM_PROT_WRITE   ((vm_prot_t) 0x02)
#define VM_PROT_EXECUTE ((vm_prot_t) 0x04)
#define VM_PROT_ALL     (VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)
#define VM_PROT_ALLTWO  (SCE_KERNEL_PROT_CPU_ALL)
#define VM_PROT_RW      (VM_PROT_READ|VM_PROT_WRITE)
#define VM_PROT_DEFAULT VM_PROT_ALL

inline
void MemSet(DWORD64 address, uint8_t value, uint64_t size)
{
	int oldProt = 0;
	sceKernelQueryMemoryProtection((PVOID*)address, 0, 0, &oldProt);
	sceKernelMprotect((PVOID*)address, size, VM_PROT_ALL);
	memset((void*)address, value, size);
	sceKernelMprotect((PVOID*)address, size, oldProt);
}

inline
void MemPatch(DWORD64 address, uint8_t* pByte, int numberofbytestowrite)
{
	int oldProt = 0;
	sceKernelQueryMemoryProtection((PVOID*)address, 0, 0, &oldProt);
	sceKernelMprotect((PVOID*)address, numberofbytestowrite, VM_PROT_ALL);
	memcpy((void*)address, pByte, numberofbytestowrite);
	sceKernelMprotect((PVOID*)address, numberofbytestowrite, oldProt);
}

inline
uint64_t FindExecutableBase() {
	SceKernelVirtualQueryInfo info;
	void* start_addr = 0;
	void* addr = 0;

	while (sceKernelVirtualQuery(addr, SCE_KERNEL_VQ_FIND_NEXT, &info, sizeof(SceKernelVirtualQueryInfo)) >= 0)
	{
		addr = (void*)((uint64_t)info.end + 1);
		if (info.protection == 5) {
			start_addr = info.start;
			break;
		}
	}

	if (!start_addr) {
		printf("[FindExecutableBase] ERROR: No executable region found!\n");
		return 0;
	}

	printf("[FindExecutableBase] start_addr 0x%llX\n", start_addr);

	return (uint64_t)start_addr;
}

inline
void WriteJump(uint64_t address, uint64_t destination) {
	*(uint8_t*)(address) = 0xFF;
	*(uint8_t*)(address + 1) = 0x25;
	*(uint8_t*)(address + 2) = 0x00;
	*(uint8_t*)(address + 3) = 0x00;
	*(uint8_t*)(address + 4) = 0x00;
	*(uint8_t*)(address + 5) = 0x00;
	*(uint64_t*)(address + 6) = destination;
}

inline
void* DetourFunction(uint64_t address, void* destination, int length) {
	if (!address || !destination)
		return 0;

	if (length < HOOK_LENGTH)
		return 0;

	int res2 = sceKernelMprotect((void*)address, length, VM_PROT_ALL);
	if (res2 != 0)
	{
		printf("[DetourFunction] sceKernelMprotect Failed:  0x%11X\n", res2);
	}
	else
	{
		printf("[DetourFunction] sceKernelMprotect passed:  0x%11X\n", res2);
	}

	int stubLength = length + HOOK_LENGTH;
	void* stubAddress;
	int res = sceKernelMmap(0, stubLength, VM_PROT_ALL, 0x1000 | 0x2, -1, 0, &stubAddress);
	if (res < 0 || stubAddress == 0)
	{
		printf("[DetourFunction] sceKernelMmap Failed: 0x%11X\n", res);
		return 0;
	}
	else
	{
		printf("[DetourFunction] sceKernelMmap Passed: 0x%11X\n", res);
	}

	memcpy((void*)stubAddress, (void*)address, length);
	WriteJump(((uint64_t)stubAddress + length), (uint64_t)(address + length));
	WriteJump(address, (uint64_t)destination);
	return stubAddress;
}
