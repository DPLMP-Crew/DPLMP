#include "LuaVehicle.h"
#include "Core.h"

VehicleSpawnState LuaVehicleRequest::GetState() {
	return m_Handle->m_State;
}

GameVehicle LuaVehicleRequest::GetVehicle() {
	if (m_Handle->m_Vehicle != nullptr)
		return GameVehicle(m_Handle->m_Vehicle);
	return nullptr;
}

void LuaVehicle::Register(sol::state* lua) {
	sol::table vehManagerTable = lua->create_named_table("VehicleManager");

	vehManagerTable.set_function("CreateVehicle",
		[](tVehicleModelUID model, float x, float y, float z, float angle, bool snapToGround) {
			std::shared_ptr<VehicleSpawnRequest> spawnRequest = Core::VehSpawner->RequestVehicle(model, x, y, z, angle, snapToGround);
			return LuaVehicleRequest(spawnRequest);
		}
	);

	vehManagerTable.set_function("DestroyVehicle",
		[](LuaVehicleRequest vehicleRequest) {
			Core::VehSpawner->DestroyVehicle(vehicleRequest.m_Handle.get());
		}
	);

	vehManagerTable["Processing"] = VehicleSpawnState::Processing;
	vehManagerTable["Failed"] = VehicleSpawnState::Failed;
	vehManagerTable["Spawned"] = VehicleSpawnState::Spawned;
}