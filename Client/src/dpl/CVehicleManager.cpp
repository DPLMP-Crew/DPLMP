#include "dpl/CVehicleManager.h"

CVehicleManager* CVehicleManager::GetInstance()
{
    return (CVehicleManager*)(*(int*)(0x70C830) - 4);
}

AutoPtr<CVehicle, int>* CVehicleManager::CreateVehicle(AutoPtr<CVehicle, int>* out, tVehicleModelUID tVehicle, float x, float y, float z, float w, float fRotation, bool snapToTerrain, bool snapToPhysics, bool networkPublish)
{
    return ((AutoPtr<CVehicle, int>* (__thiscall*)(CVehicleManager*, AutoPtr<CVehicle, int>*, tVehicleModelUID, float, float, float, float, float, bool, bool, bool))0x5B154B)(this, out, tVehicle, x, y, z, w, fRotation, snapToTerrain, snapToPhysics, networkPublish);
}

void CVehicleManager::FreeVehicle(AutoPtr<CVehicle, int>* vehiclePtr) {
    ((void(__thiscall*)(CVehicleManager*, AutoPtr<CVehicle, int>*))0x5b10e9)(this, vehiclePtr);
}