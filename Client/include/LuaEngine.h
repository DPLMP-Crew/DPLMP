#pragma once
#include <sol/sol.hpp>

class LuaEngine {
public:
	LuaEngine();
	void RunScriptFromFile(const wchar_t* filename);
private:
	sol::state m_LuaState;
};