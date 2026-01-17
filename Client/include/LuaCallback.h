#pragma once
#include "LuaScheduler.h"
#include <sol/sol.hpp>

class LuaCallback {
public:
	LuaCallback(LuaScheduler* scheduler, sol::state* lua) {
		m_Scheduler = scheduler;
		m_LuaState = lua;
	}
	template <typename... Args>
	void Fire(Args&&... args) {
		for (int i = 0; i < m_Functions.size(); i++) {
			sol::coroutine co(m_Functions[i]);
			m_Scheduler->AddTask(co, std::forward<Args>(args)...);
		}
	}
	void AddCallback(sol::function callback);
	std::vector<sol::function> m_Functions;

private:
	LuaScheduler* m_Scheduler;
	sol::state* m_LuaState;
};