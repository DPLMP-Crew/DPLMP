#pragma once
#include <sol/sol.hpp>
#include <string>

class LuaEngine {
public:
	LuaEngine();
	void RunScriptFromFile(const std::wstring& filename);
	sol::state m_LuaState;
};