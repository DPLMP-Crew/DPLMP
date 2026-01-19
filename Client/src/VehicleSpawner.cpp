#include "VehicleSpawner.h"
#include "dpl/CVehicleManager.h"
#include "dpl/LifeAcquirableVehicleManager.h"
#include "Core.h"

static CVehicle* SpawnVehicle(VehicleSpawnRequest* request) {
	AutoPtr<CVehicle, int> createdVehicle{};

	CVehicleManager* vehManager = CVehicleManager::GetInstance();
	LifeAcquirableVehicleManager* lifeManager = LifeAcquirableVehicleManager::GetInstance();
	vehManager->CreateVehicle(&createdVehicle, request->m_Model, request->m_X, request->m_Y, request->m_Z, 1.0f, request->m_Angle, request->m_SnapToGround, request->m_SnapToGround, false);
	if (createdVehicle.m_pPointer != nullptr) {
		lifeManager->AddVehicle(createdVehicle.m_pPointer, 1);
		request->m_State = VehicleSpawnState::Spawned;
		request->m_Vehicle = createdVehicle.m_pPointer;
		return createdVehicle.m_pPointer;
	}
	else
	{
		// keep trying i guess?
		// request->m_State = VehicleSpawnState::Failed;
		return nullptr;
	}
}

std::shared_ptr<VehicleSpawnRequest> VehicleSpawner::RequestVehicle(tVehicleModelUID tVehicle, float x, float y, float z, float angle, bool snapToGround) {
	std::shared_ptr<VehicleSpawnRequest> request = std::make_shared<VehicleSpawnRequest>();

	request->m_Model = tVehicle;
	request->m_X = x;
	request->m_Y = y;
	request->m_Z = z;
	request->m_Angle = angle;
	request->m_SnapToGround = snapToGround;

	Core::ResourceMgr->Acquire(SpooledPackageType::Vehicles, request->m_Model);

	if (Core::ResourceMgr->IsLoaded(SpooledPackageType::Vehicles, request->m_Model)) {
		SpawnVehicle(request.get());
	}
	else
	{
		m_Requests.push_back(request);
	}
	return request;
}

void VehicleSpawner::DestroyVehicle(VehicleSpawnRequest* request) {
	if (request->m_State == VehicleSpawnState::Processing) {
		for (auto it = m_Requests.begin(); it != m_Requests.end(); ) {
			auto myrequest = *it;
			if (myrequest.get() == request) {
				Core::ResourceMgr->Release(SpooledPackageType::Vehicles, request->m_Model);
				it = m_Requests.erase(it);
				return;
			}
			else
			{
				++it;
			}
		}
	}
	else if (request->m_State == VehicleSpawnState::Spawned) {
		CVehicleManager* vehManager = CVehicleManager::GetInstance();
		Core::ResourceMgr->Release(SpooledPackageType::Vehicles, request->m_Model);
		AutoPtr<CVehicle, int> deletedVehicle{};
		deletedVehicle.m_pPointer = request->m_Vehicle;
		vehManager->FreeVehicle(&deletedVehicle);
	}
}

void VehicleSpawner::Update() {
	for (auto it = m_Requests.begin(); it != m_Requests.end(); ) {
		auto request = *it;

		if (Core::ResourceMgr->IsLoaded(SpooledPackageType::Vehicles, request->m_Model)) {
			SpawnVehicle(request.get());
			if (request->m_State == VehicleSpawnState::Spawned)
			{
				it = m_Requests.erase(it);
			}
			else if (request->m_State == VehicleSpawnState::Failed)
			{
				Core::ResourceMgr->Release(SpooledPackageType::Vehicles, request->m_Model);
				it = m_Requests.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}
}