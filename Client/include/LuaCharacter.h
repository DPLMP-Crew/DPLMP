#pragma once
#include "dpl/CCharacter.h"
#include <sol/sol.hpp>

class GameCharacter {
public:
	GameCharacter(CCharacter* handle) {
		m_Handle = handle;
	};
	void Revive();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z, float w);
	void SetHealth(float health);
	float GetHealth();
	std::tuple<float, float, float> GetPosition();
	std::tuple<float, float, float, float> GetRotation();
	CCharacter* m_Handle;
};

class LuaCharacter {
public:
	void Register(sol::state* lua);
};