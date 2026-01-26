#pragma once
#include <string>
#include "ResourceManager.h"
#include "VehicleSpawner.h"
#include <memory>

namespace Core {
	constexpr const wchar_t* UserDirectory = L"My Games\\DPLMP";
	extern std::wstring AbsoluteUserDirectory;
	extern std::wstring GameDirectory;
	extern std::wstring Directory;
	extern bool PlayerSpawned;
	extern float DeltaTime;
	extern std::unique_ptr<ResourceManager> ResourceMgr;
	extern std::unique_ptr<VehicleSpawner> VehSpawner;
	void Initialize();
	bool IsGamePaused();
}