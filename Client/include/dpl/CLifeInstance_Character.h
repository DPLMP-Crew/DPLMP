#pragma once
#include "dpl/AutoPtr.h"
#include "dpl/ECharacterType.h"
#include "dpl/Matrix.h"
#include "dpl/Vector4.h"
#include "dpl/CWeapon.h"

class CLifeInstance_Character {
public:
	CLifeInstance_Character();
	void Initialise(
		AutoPtr<CLifeInstance_Character, int>* outPtr,
		int playerEnum,
		ECharacterType characterType,
		Matrix* posMatrix,
		void* punk,
		int iunk,
		int _iunk,
		Vector4* pos,
		Vector4* _pos,
		Vector4* velocitymaybe,
		Matrix* zeroMatrix,
		float heading,
		int __iunk,
		Matrix* _zeroMatrix,
		Matrix* __zeroMatrix,
		Matrix* ___zeroMatrix,
		CWeapon* weapon,
		void* _punk,
		void* __punk,
		void* ___punk,
		int ___iunk,
		int ____iunk,
		void* ____punk,
		Vector4* rotation,
		Matrix* ____zeroMatrix,
		float funk
	);
	void CoreCreate();
private:
	char _pad[0x1f4];
};