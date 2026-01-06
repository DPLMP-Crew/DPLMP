#pragma once
#include "dpl/CVehicle.h"
#include "dpl/VehicleModel.h"

class CVehicle;

class CVehicleManager {
public:
	CVehicle** CreateVehicle(CVehicle** pVehicle, tVehicleModelUID tVehicle, float x, float y, float z, float w, float fRotation, bool unk1, bool unk2, bool unk3);
};


CVehicleManager* GetVehicleManager();