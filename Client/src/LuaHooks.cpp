#include "LuaHooks.h"

void LuaHooks::Register(sol::state* lua, LuaScheduler* scheduler) {

	m_LocalPlayerDeadCallback = std::make_unique<LuaCallback>(scheduler, lua);

	sol::table hooksTable = lua->create_named_table("Hooks");

	hooksTable["OnLocalPlayerDead"] = HookType::OnLocalPlayerDead;

	hooksTable.set_function("AddHook",
		[this](int hookType, sol::function fn) {
			switch (hookType) {
				case HookType::OnLocalPlayerDead:
					m_LocalPlayerDeadCallback->AddCallback(fn);
				break;
			}
		}
	);
}