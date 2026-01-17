#pragma once
#include <sol/sol.hpp>
#include "LuaCallback.h"

enum HookType {
	OnLocalPlayerDead = 0
};

class LuaHooks {
public:
	void Register(sol::state* lua, LuaScheduler* scheduler);
	template <typename... Args>
	void FireHook(HookType hook, Args&&... args) {
		switch (hook) {
		case HookType::OnLocalPlayerDead:
			m_LocalPlayerDeadCallback->Fire(std::forward<Args>(args)...);
			break;
		}
	}
private:
	std::unique_ptr<LuaCallback> m_LocalPlayerDeadCallback;
};