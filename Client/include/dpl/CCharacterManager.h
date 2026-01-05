#pragma once
#include "dpl/AutoPtr.h"
#include "dpl/CWeapon.h"
#include "dpl/CCharacter.h"

class CCharacterManager {
public:
	static CCharacterManager* GetInstance();
	CCharacter* CreateCharacter(AutoPtr<CCharacter, int>* outChar, int eType, Vector4* v4Position, float fHeading,
		EnumAnimCondition eAnimCondition, ECharacterRoles eRole, Vector4* velocity, bool strafe,
		AutoPtr<CWeapon, int> weaponPtr, Vector4* targetPosition, bool crouch, float health);
};