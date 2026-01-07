#include "dpl/CLifeInstance_Character.h"

CLifeInstance_Character::CLifeInstance_Character()
{
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
	Matrix* _zeroMatrix,
	Matrix* __zeroMatrix,
	Matrix* ___zeroMatrix,
	CWeapon* weapon,
	void* _punk,
	void* __punk,
	void* ___punk,
	int __iunk,
	int ___iunk,
	void* ____punk,
	Vector4* rotation,
	Matrix* ____zeroMatrix,
	float funk;
)
{
	((void(__thiscall*)(
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
		_zeroMatrix, 
		__zeroMatrix, 
		___zeroMatrix, 
		weapon,
		_punk,
		__punk,
		___punk,
		__iunk,
		___iunk,
		____punk,
		rotation,
		____zeroMatrix,
		funk
		);
}