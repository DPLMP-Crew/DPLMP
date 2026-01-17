#pragma once
#include <sol/sol.hpp>

class LuaInput {
public:
	void Register(sol::state* lua);
};