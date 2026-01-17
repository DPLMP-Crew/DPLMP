#pragma once
#include <sol/sol.hpp>
#include <string>
#include <vector>

class LuaScheduler {
public:
	void Update(float delta);
};

class LuaEngine {
public:
	LuaEngine();
	void Update(float delta);
	void RunScriptFromFile(const std::wstring& filename);
	sol::state m_LuaState;
	LuaScheduler m_Scheduler;
};