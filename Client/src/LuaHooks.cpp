#include "LuaHooks.h"

void LuaHooks::Register(sol::state* lua, LuaScheduler* scheduler) {

	for (int i = 0; i < HookType::Count; i++) {
		m_Callbacks[i] = std::make_unique<LuaCallback>(scheduler, lua);
	}

	sol::table hooksTable = lua->create_named_table("Hooks");

	hooksTable["OnLocalPlayerDead"] = HookType::OnLocalPlayerDead;

	hooksTable.set_function("AddHook",
		[this](int hookType, sol::function fn) {
			if (hookType >= HookType::Count) return;
			m_Callbacks[hookType]->AddCallback(fn);
		}
	);
}