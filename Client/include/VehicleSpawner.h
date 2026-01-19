#pragma once
#include <memory>
#include "dpl/VehicleModel.h"
#include "dpl/CVehicle.h"
#include <vector>

enum VehicleSpawnState {
	Processing,
	Spawned,
	Failed
};

class VehicleSpawnRequest {
public:
	CVehicle* m_Vehicle = nullptr;
	VehicleSpawnState m_State = VehicleSpawnState::Processing;
	tVehicleModelUID m_Model = tVehicleModelUID_Andec;
	float m_X = 0.0;
	float m_Y = 0.0;
	float m_Z = 0.0;
	float m_Angle = 0.0;
	bool m_SnapToGround = false;
};

class VehicleSpawner {
public:
	std::shared_ptr<VehicleSpawnRequest> RequestVehicle(tVehicleModelUID tVehicle, float x, float y, float z, float angle, bool snapToGround);
	void DestroyVehicle(VehicleSpawnRequest* request);
	void Update();
private:
	std::vector<std::shared_ptr<VehicleSpawnRequest>> m_Requests;
};