#include "LuaHooks.h"

namespace LuaHooks {
	void Register(LuaEngine* lua) {
		sol::table hooksTable = lua->m_LuaState.create_named_table("Hooks");
	}
}