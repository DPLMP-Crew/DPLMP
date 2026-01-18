#pragma once
#include <sol/sol.hpp>
#include <string>
#include <vector>
#include "LuaHooks.h"
#include "LuaUI.h"
#include "LuaInput.h"
#include "LuaCore.h"

class LuaEngine {
public:
	LuaEngine();
	void Update(float delta);
	void RunScriptFromFile(const std::wstring& filename);
	sol::state m_LuaState;
	LuaScheduler m_Scheduler;

	LuaHooks m_LuaHooks;
	LuaUI m_LuaUI;
	LuaInput m_LuaInput;
	LuaCore m_LuaCore;
};