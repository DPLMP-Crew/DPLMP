#pragma once
#include "dpl/CVehicle.h"

class LifeAcquirableVehicleManager {
public:
	static LifeAcquirableVehicleManager* GetInstance();
	void AddVehicle(CVehicle* pVehicle, int a2);
	void RemoveVehicle(CVehicle* pVehicle);
	int  NumberOfVehicles();
};
