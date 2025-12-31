#include <Windows.h>
#include <iostream>
#include "Core.h"

static DWORD WINAPI CoreThread(LPVOID param)
{
    Core::Initialize();
    return 0;
}

BOOL WINAPI DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
        DisableThreadLibraryCalls(hModule);
		printf("Initializing DPLMP Core.\n");
        CreateThread(0, 0, CoreThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}