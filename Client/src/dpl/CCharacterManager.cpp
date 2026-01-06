#include "dpl/CCharacterManager.h"

CCharacter* CCharacterManager::CreateCharacter
(AutoPtr<CCharacter, int>* outChar, ECharacterType eType, Vector4* v4Position, float fHeading,
	EnumAnimCondition eAnimCondition, ECharacterRoles eRole, Vector4* velocity, bool strafe,
	AutoPtr<CWeapon, int> weaponPtr, Vector4* targetPosition, bool crouch, float health)
{
	return ((CCharacter * (__thiscall*)(
		CCharacterManager*,
		AutoPtr<CCharacter, int> *, // it doesn't have in the PS2 preview build but it's kind of pointless when it can just be returned sometimes
		ECharacterType,
		Vector4*,
		float,
		EnumAnimCondition,
		ECharacterRoles,
		Vector4*,
		bool,
		AutoPtr<CWeapon, int>,
		Vector4*,
		bool,
		float))0x4F723B)(
			this, outChar, eType, v4Position, fHeading, eAnimCondition, eRole, velocity, strafe, weaponPtr, targetPosition, crouch, health);
}

CCharacterManager* CCharacterManager::GetInstance()
{
	return (CCharacterManager*)(*(int*)0x70C6E0); // 0x0070C6A4
}

void CCharacterManager::FreeCharacter(AutoPtr<CCharacter, int>* characterPtr) {
	((void(__thiscall*)(CCharacterManager*, AutoPtr<CCharacter, int>*))0x4f6ee8)(this, characterPtr);
}

int CCharacterManager::GetFreeCharacterCount() {
	return *(float*)(this + 0x3DC);
}