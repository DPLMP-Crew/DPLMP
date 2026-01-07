#include "dpl/CLifeInstance_Character.h"
#include <iostream>

CLifeInstance_Character::CLifeInstance_Character()
{
	memset(this, 0, sizeof(CLifeInstance_Character));
	((void(__thiscall*)(CLifeInstance_Character*))0x47f97e)(this);
}

void CLifeInstance_Character::CoreCreate() {
	((void(__thiscall*)(CLifeInstance_Character*))0x47f8c3)(this);
}

void CLifeInstance_Character::Initialise(
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
)
{
	((void(__thiscall*)(
		CLifeInstance_Character*,
		AutoPtr<CLifeInstance_Character, int>*,
		int,
		ECharacterType,
		Matrix *,
		void*,
		int,
		int,
		Vector4*,
		Vector4*,
		Vector4*,
		Matrix*,
		float,
		int,
		Matrix*,
		Matrix*,
		Matrix*,
		CWeapon*,
		void*,
		void*,
		void*,
		int,
		int,
		void*,
		Vector4*,
		Matrix*,
		float
	))0x47e1a6)(
		this, 
		outPtr, 
		playerEnum, 
		characterType, 
		posMatrix, 
		punk, 
		iunk, 
		_iunk, 
		pos, 
		_pos, 
		velocitymaybe, 
		zeroMatrix,
		heading, 
		__iunk,
		_zeroMatrix, 
		__zeroMatrix, 
		___zeroMatrix, 
		weapon,
		_punk,
		__punk,
		___punk,
		___iunk,
		____iunk,
		____punk,
		rotation,
		____zeroMatrix,
		funk
		);
}