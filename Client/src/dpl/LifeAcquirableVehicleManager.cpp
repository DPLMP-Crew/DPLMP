#include "dpl/LifeAcquirableVehicleManager.h"

void LifeAcquirableVehicleManager::AddVehicle(CVehicle* pVehicle, int a2)
{
	((void(__thiscall*)(LifeAcquirableVehicleManager*, CVehicle*, int))0x4A2D97)(this, pVehicle, a2);
}

int LifeAcquirableVehicleManager::NumberOfVehicles()
{
	return *(int*)(this + 0x20C);
}

LifeAcquirableVehicleManager* LifeAcquirableVehicleManager::GetInstance()
{
	return  ((LifeAcquirableVehicleManager * (__cdecl*)())0x4A2CA1)();
}