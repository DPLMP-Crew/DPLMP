#pragma once
#include "dpl/CVehicle.h"
#include "dpl/VehicleModel.h"
#include "dpl/AutoPtr.h"

class CVehicle;

class CVehicleManager {
public:
	static CVehicleManager* GetInstance();
	AutoPtr<CVehicle, int>* CreateVehicle(AutoPtr<CVehicle, int>* out, tVehicleModelUID tVehicle, float x, float y, float z, float w, float fRotation, bool snapToTerrain, bool snapToPhysics, bool networkPublish);
	void FreeVehicle(AutoPtr<CVehicle, int>* vehiclePtr);
};