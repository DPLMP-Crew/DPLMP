#include "dpl/CWipeManager.h"

CWipeManager* CWipeManager::GetInstance()
{
	return *(CWipeManager**)0x70c79c;
}

void CWipeManager::ResetWipes()
{
	((void(__thiscall*)(CWipeManager*))0x55dd29)(this);
}