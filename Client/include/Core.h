#pragma once
#include <string>

namespace Core {
	extern std::wstring Directory;
	extern bool PlayerSpawned;
	extern float DeltaTime;
	void Initialize();
	bool IsGamePaused();
}