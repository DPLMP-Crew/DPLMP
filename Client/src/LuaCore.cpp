#include "LuaCore.h"

void LuaCore::Register(sol::state* lua, LuaScheduler* scheduler) {
	sol::table coreTable = lua->create_named_table("Core");
	coreTable.set_function("CreateCoroutine",
		[lua, this](sol::function fn, sol::variadic_args args) {
			sol::thread thread = sol::thread::create(lua->lua_state());
			sol::coroutine co(thread.thread_state(), fn);
			m_Scheduler->AddTask(std::move(co), std::move(thread), args);
		}
	);
}