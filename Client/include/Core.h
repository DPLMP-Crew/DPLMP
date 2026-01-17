#pragma once
#include <string>

namespace Core {
	extern std::wstring Directory;
	extern bool PlayerSpawned;
	void Initialize();
	bool IsGamePaused();
}