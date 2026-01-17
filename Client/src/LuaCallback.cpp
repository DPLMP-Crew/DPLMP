#include "LuaCallback.h"

void LuaCallback::AddCallback(sol::function callback) {
	m_Functions.emplace_back(std::move(callback));
}