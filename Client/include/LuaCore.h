#pragma once
#include <sol/sol.hpp>
#include "LuaScheduler.h"

class LuaCore {
public:
	void Register(sol::state* lua, LuaScheduler* scheduler);

private:
	LuaScheduler* m_Scheduler;
};