#pragma once
#include <sol/sol.hpp>
#include "LuaCallback.h"

enum HookType {
	OnLocalPlayerDead,
	OnFrameUpdate,
	Count
};

class LuaHooks {
public:
	void Register(sol::state* lua, LuaScheduler* scheduler);
	template <typename... Args>
	void FireHook(HookType hook, Args&&... args) {
		if (hook >= HookType::Count) return;
		m_Callbacks[hook]->Fire(std::forward<Args>(args)...);
	}
private:
	std::unique_ptr<LuaCallback> m_Callbacks[HookType::Count];
};