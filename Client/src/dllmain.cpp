#include <Windows.h>
#include <iostream>
#include "Core.h"
#include <string>

static DWORD WINAPI CoreThread(LPVOID param)
{
    Core::Initialize();
    return 0;
}

static void CacheGameDirectory() {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(NULL, path, MAX_PATH);
    std::wstring finalPath = path;
    size_t pos = finalPath.find_last_of(L"\\/");
    Core::GameDirectory = finalPath.substr(0, pos);
    CharUpperBuffW(Core::GameDirectory.data(), static_cast<DWORD>(Core::GameDirectory.size()));
}

static void CacheCoreDirectory(HMODULE handle) {
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(handle, path, MAX_PATH);
    std::wstring finalPath = path;
    size_t pos = finalPath.find_last_of(L"\\/");
    Core::Directory = finalPath.substr(0, pos);
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
        CacheCoreDirectory(hModule);
        CacheGameDirectory();
        CreateThread(0, 0, CoreThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}