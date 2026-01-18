#pragma once
#include <string>
#include "ResourceManager.h"
#include <memory>

namespace Core {
	extern std::wstring Directory;
	extern bool PlayerSpawned;
	extern float DeltaTime;
	extern std::unique_ptr<ResourceManager> ResourceMgr;
	void Initialize();
	bool IsGamePaused();
}