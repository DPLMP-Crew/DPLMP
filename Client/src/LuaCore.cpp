#include "LuaCore.h"
#include "Core.h"

void LuaCore::Register(sol::state* lua, LuaScheduler* scheduler) {
	sol::table coreTable = lua->create_named_table("Core");
	coreTable.set_function("CreateCoroutine",
		[lua, scheduler](sol::function fn, sol::variadic_args args) {
			sol::thread thread = sol::thread::create(lua->lua_state());
			sol::coroutine co(thread.thread_state(), fn);
			return scheduler->AddTask(std::move(co), std::move(thread), args).m_Value;
		}
	);
	coreTable.set_function("AbortCoroutine",
		[scheduler](uint64_t id) {
			scheduler->AbortTask(Id64(id));
		}
	);
	coreTable.set(
		"DeltaTime",
		sol::readonly_property([]() -> float {
			return Core::DeltaTime;
		})
	);
}