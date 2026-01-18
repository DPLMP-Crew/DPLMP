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
			sol::thread thread = sol::thread::create(m_LuaState->lua_state());
			sol::coroutine co(thread.thread_state(), m_Functions[i]);
			m_Scheduler->AddTask(std::move(co), std::move(thread), std::forward<Args>(args)...);
		}
	}
	void AddCallback(sol::function callback);
	std::vector<sol::function> m_Functions;

private:
	LuaScheduler* m_Scheduler;
	sol::state* m_LuaState;
};