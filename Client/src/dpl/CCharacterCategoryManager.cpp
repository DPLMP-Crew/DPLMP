#include "dpl/CCharacterCategoryManager.h"

CCharacterCategoryManager* CCharacterCategoryManager::GetInstance() {
	return *(CCharacterCategoryManager**)0x70C6E4;
}

void CCharacterCategoryManager::SetPlayerAvatar(int id) {
	((void(__thiscall*)(CCharacterCategoryManager*, int))0x4F31F5)(this, id);
}