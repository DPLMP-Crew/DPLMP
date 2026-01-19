#include "dpl/LifeAcquirableVehicleManager.h"
#include "dpl/AutoPtr.h"

void LifeAcquirableVehicleManager::AddVehicle(CVehicle* pVehicle, int a2)
{
	((void(__thiscall*)(LifeAcquirableVehicleManager*, CVehicle*, int))0x4A2D97)(this, pVehicle, a2);
}

void LifeAcquirableVehicleManager::RemoveVehicle(CVehicle* pVehicle)
{
	((void(__thiscall*)(LifeAcquirableVehicleManager*, CVehicle*))0x4a2de5)(this, pVehicle);
}

int LifeAcquirableVehicleManager::NumberOfVehicles()
{
	return *(int*)(this + 0x20C);
}

LifeAcquirableVehicleManager* LifeAcquirableVehicleManager::GetInstance()
{
	return  ((LifeAcquirableVehicleManager * (__cdecl*)())0x4A2CA1)();
}