#pragma once
#include <map>
#include <iostream>
#include <list>
#include "dpl/CWeapon.h"
#include "dpl/AutoPtr.h"
#include "dpl/Vector4.h"
#include "dpl/Matrix.h"
#include "dpl/HAnimInstance.h"
#include "dpl/EVehicleDoor.h"
#include "dpl/CPCViewport.h"

class CVehicle;
class CCharacter;
class CPCViewport;

class CCharacterRendering {
public:

};


class CCharacter {
public:

	int GetCharacterType();

	void GetPosition(Vector4* pos);
	Vector4* GetVelocity();

	void SetArmour(float armour);

	bool IsCrouching();
	void Crouch(bool a1, bool a2);

	void SetScaredOfBeingRunnedOver(int scareType);

	void Draw(CPCViewport viewport);

	void SetArrested();

	void ForceIdleMode();
	void PlayMissionAnimation(unsigned int animHash, bool unk);
	void KillMissionAnimations();

	void EnterVehicle(CVehicle* pVehicle, int nDoor, bool hijack, bool unk2);
	void ExitVehicle(int Reason, int unk1, int unk2);
	///<summary>
	///Ejects the character from the vehicle.
	///</summary>
	void EjectFromVehicle();

	void GetAnimationRootMatrix(Matrix* m, HAnimClip* clip, float time);
	bool ValidWeapon();

	void SetPosition(Vector4* pos);
	void SetModel(int model);
	void SetAnimType(int type);

	int* GetAnimTypePointer();
	int GetPointer();
	int GetVTableAddress();

	void DrawCrosshair(int Viewport, int unk);

	void Melee(CCharacter* target);

	void SetTarget(Vector4 target);

	void Walk(Vector4 velocity, bool strafe, bool scared);
	void Walk(Vector4* velocity, bool strafe, bool scared);
	void FaceDirection(Vector4 worldVec);
	void FaceDirection(Vector4* worldVec);

	void HolsterWeapon();
	void DrawWeapon(AutoPtr<CWeapon, int> weapon);
	void ReloadWeapon();
	void ShootWeapon(bool hitTarget, bool checkTarget, float angularInaccuracy);

	CCharacterRendering* GetRendering();
	CVehicle* GetVehicle();
	CVehicle* GetEnteringVehicle();
	float GetAngle();
	Matrix* GetMatrix();
	HAnimInstance* GetAnimInstance();
	CWeapon* GetWeapon();

	bool operator==(CCharacter* other);
	bool operator!=(CCharacter* other);

	///<summary>
	///Teleports the player instantly into a vehicle
	///</summary>
	void TeleportToVehicle(CVehicle* pVehicle, int seat, bool unk1);

	EVehicleDoor GetVehicleDoor();

	///<summary>
	///Sets the character's health to a certain value.
	///</summary>
	void SetHealth(float health);
	///<summary>
	 ///Returns the amount of health of this character.
	///</summary>
	float GetHealth();

	void SetRole(int role);
	int GetRole();

	void SetAllowGetOutOfVehicle(bool allow);
	bool GetAllowGetOutOfVehicle();
	void AllowVehicleExit(bool enable);

	bool GetIsSpawned();
	void SetIsSpawned(bool spawned);

	int GetModel();

	bool GetShadowVisible();
	void SetShadowVisible(bool set);

	// actually no clue
	float GetAlpha();
	void SetAlpha(float alpha);

	int GetRenderMode();
	void SetRenderMode(int rendermode);
	// --------------------------

	// junkyard
	void Unknown(CVehicle* pVehicle, CCharacter* pTarget);
	void Unknown2(CVehicle* pVehicle, CVehicle* pTarget);
	void Unknown3();
	void Unknown4();

};