#pragma once
#include <sol/sol.hpp>
#include <memory>
#include "VehicleSpawner.h"

class GameVehicle {
public:
	GameVehicle(CVehicle* handle) {
		m_Handle = handle;
	};
	CVehicle* m_Handle;
};

class LuaVehicleRequest {
public:
	LuaVehicleRequest(std::shared_ptr<VehicleSpawnRequest> handle) {
		m_Handle = handle;
	};
	std::shared_ptr<VehicleSpawnRequest> m_Handle;
	VehicleSpawnState GetState();
	GameVehicle GetVehicle();
};

class LuaVehicle {
public:
	void Register(sol::state* lua);
};