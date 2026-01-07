#include <iostream>
#include "dpl/CCharacter.h"

void CCharacter::GetPosition(Vector4* pos)
{
	((void(__thiscall*)(CCharacter*, Vector4*))0x4F2A5E)(this, pos);
}

void CCharacter::Crouch(bool a1, bool a2)
{
	((void(__thiscall*)(CCharacter*, bool, bool))0x4E7ADC)(this, a1, a2);
}

int CCharacter::GetCharacterType()
{
	return ((int(__thiscall*)(CCharacter*, CVehicle*))0x4E7990)(this, this->GetVehicle());
}

void CCharacter::SetArmour(float armour)
{
	((void(__thiscall*)(CCharacter*, float, bool))0x4E7A1E)(this, armour, false);
}

bool CCharacter::IsCrouching()
{
	((void(__thiscall*)(CCharacter*, bool, bool))0x4E7ADC)(this, false, false);
	return false;
}

void CCharacter::SetScaredOfBeingRunnedOver(int scareType)
{
	((void(__thiscall*)(CCharacter*, int))0x4E7AF7)(this, scareType);
}

void CCharacter::Draw(CPCViewport viewport)
{
	((void(__thiscall*)(CCharacter*, CPCViewport))0x4F09AD)(this, viewport);
}

void CCharacter::SetArrested()
{
	((void(__thiscall*)(CCharacter*))0x4F2A32)(this);
}

void CCharacter::ForceIdleMode()
{
	((void(__thiscall*)(CCharacter*))0x4E9E0A)(this);
}

void CCharacter::PlayMissionAnimation(unsigned int animHash, bool unk)
{
	((void(__thiscall*)(CCharacter*, unsigned int, bool))0x4E9E24)(this, animHash, unk);
}

void CCharacter::KillMissionAnimations()
{
	((void(__thiscall*)(CCharacter*))0x4E863F)(this);
}

void CCharacter::EnterVehicle(CVehicle* pVehicle, int nDoor, bool hijack, bool unk2)
{
	((void(__thiscall*)(CCharacter*, CVehicle*, int, bool, bool))0x4EBF46)(this, pVehicle, nDoor, hijack, unk2);
}

void CCharacter::ExitVehicle(int Reason, int unk1, int unk2)
{
	((void(__thiscall*)(CCharacter*, int, int, int))0x4EB560)(this, Reason, unk1, unk2);
}

// the true EnterVehicleImmediate
void CCharacter::TeleportToVehicle(CVehicle* pVehicle, int seat, bool unk1)
{
	((void(__thiscall*)(CCharacter*, CVehicle*, int, bool))0x4EF9EE)(this, pVehicle, seat, unk1);
}

void CCharacter::GetAnimationRootMatrix(Matrix* m, HAnimClip* clip, float time)
{
	((void(__thiscall*)(CCharacter*, Matrix*, HAnimClip*, float))0x4E7FC7)(this, m, clip, time);
}

void CCharacter::EjectFromVehicle()
{
	((void(__thiscall*)(CCharacter*))0x4EA603)(this);
}

void CCharacter::AllowVehicleExit(bool enable)
{
	((void(__thiscall*)(CCharacter*, bool))0x4F29CA)(this, enable);
}

void CCharacter::Melee(CCharacter* target)
{
	((void(__thiscall*)(CCharacter*, CCharacter*))0x4E7B11)(this, target);
}

void CCharacter::ReloadWeapon()
{
	((void(__thiscall*)(CCharacter*))0x4E8D64)(this);
}

void CCharacter::SetTarget(Vector4 target)
{
	((void(__thiscall*)(CCharacter*, Vector4))0x4E8750)(this, target);
}

void CCharacter::Walk(Vector4 velocity, bool strafe, bool scared)
{
	((void(__thiscall*)(CCharacter*, Vector4*, bool, bool))0x4E7990)(this, &velocity, strafe, scared);
}

void CCharacter::Walk(Vector4* velocity, bool strafe, bool scared)
{
	((void(__thiscall*)(CCharacter*, Vector4*, bool, bool))0x4E7990)(this, velocity, strafe, scared);
}

void CCharacter::FaceDirection(Vector4 worldVec)
{
	((void(__thiscall*)(CCharacter*, Vector4*))0x4E7A1E)(this, &worldVec);
}

void CCharacter::FaceDirection(Vector4* worldVec)
{
	((void(__thiscall*)(CCharacter*, Vector4*))0x4E7A1E)(this, worldVec);
}

void CCharacter::DrawWeapon(AutoPtr<CWeapon, int> weapon)
{
	((void(__thiscall*)(CCharacter*, AutoPtr<CWeapon, int>))0x4E8C9D)(this, weapon);
}

void CCharacter::HolsterWeapon()
{
	((void(__thiscall*)(CCharacter*))0x4E8CCC)(this);
}

void CCharacter::ShootWeapon(bool hitTarget, bool checkTarget, float angularInaccuracy)
{
	((void(__thiscall*)(CCharacter*, bool, bool, float))0x4E8D22)(this, hitTarget, checkTarget, angularInaccuracy);
}

// I'm not sure what this is
// looks like AI functions?
void CCharacter::Unknown(CVehicle* pVehicle, CCharacter* pTarget) {
	((void(__thiscall*)(CCharacter*, CVehicle*, CCharacter*))0x4EC2F1)(this, pVehicle, pTarget);
}

void CCharacter::Unknown2(CVehicle* pVehicle, CVehicle* pTarget) {
	((void(__thiscall*)(CCharacter*, CVehicle*, CVehicle*))0x4E8700)(this, pVehicle, pTarget);
}

// this one feels like it returns something.......
void CCharacter::Unknown3() {
	((void(__thiscall*)(CCharacter*, CVehicle*))0x4E8750)(this, this->GetVehicle());
}

// another useless function yet again?
// or am I not calling corresponding parameters?
void CCharacter::Unknown4()
{
	((void(__thiscall*)(CCharacter*, CVehicle*, bool))0x4E9E24)(this, this->GetVehicle(), false);
}

void CCharacter::SetAllowGetOutOfVehicle(bool allow)
{
	((char*)(this + 0x7A6))[0] = allow ? 1 : 0;
}

bool CCharacter::GetAllowGetOutOfVehicle()
{
	return (((char*)(this + 0x7A6))[0] != 0);
}

bool CCharacter::ValidWeapon()
{
	return 	((bool(__thiscall*)(CCharacter*))0x4E8163)(this);
}

float CCharacter::GetAngle()
{
	Vector4 fwd = GetMatrix()->forward;
	return atan2(fwd.X, fwd.Y);
}

void CCharacter::SetPosition(Vector4* pos)
{
	GetMatrix()->pos = { pos->X, pos->Y, pos->Z, 1.0 };
}

void CCharacter::SetModel(int model)
{
	*(int*)((int)this + 0x744) = model;
	*(int*)((int)this + 0x764) = model;
}

int CCharacter::GetModel()
{
	return *(int*)((int)this + 0x744);
}

void CCharacter::SetAnimType(int type)
{
	*(int*)((int)this + 0x63C) = type;
}

int* CCharacter::GetAnimTypePointer()
{
	return (int*)((int)this + 0x63C);
}

void CCharacter::DrawCrosshair(int Viewport, int unk)
{
	((void(__thiscall*)(CCharacter*, int, int))0x4EB954)(this, Viewport, unk);
}

CCharacterRendering* CCharacter::GetRendering()
{
	return (CCharacterRendering*)((int)this + 1680);
}

CVehicle* CCharacter::GetVehicle()
{
	return *(CVehicle**)((int)this + 0x688);
}

CVehicle* CCharacter::GetEnteringVehicle()
{
	return *(CVehicle**)((int)this + 0x5C8);
}

Matrix* CCharacter::GetMatrix()
{
	return (Matrix*)((int)this + 16);
}

Vector4* CCharacter::GetVelocity()
{
	return (Vector4*)((int)this + 0x534);
}

HAnimInstance* CCharacter::GetAnimInstance()
{
	return  (HAnimInstance*)((int)this + 0x5FC);
}

CWeapon* CCharacter::GetWeapon()
{
	if (!ValidWeapon())
		return nullptr;
	return (CWeapon*)((int)this + 208);
}

int CCharacter::GetPointer()
{
	return (int)(this);
}

int CCharacter::GetVTableAddress()
{
	return *(int*)(this);
}

bool CCharacter::operator==(CCharacter* other) {
	if (this == other)
		return true;
	return false;
}

bool CCharacter::operator!=(CCharacter* other) {
	return !(*this == other);
}

EVehicleDoor CCharacter::GetVehicleDoor()
{
	return ((EVehicleDoor(__thiscall*)(CCharacter*))0x4E86B4)(this);
}

float CCharacter::GetHealth()
{
	return *(float*)(this + 0x684);
}

void CCharacter::SetHealth(float health)
{
	*(float*)(this + 0x684) = health;
}

// ld: this seems to be lod instead.
void CCharacter::SetRole(int role)
{
	*(int*)(this + 0x770) = role; // old: 0x3E0
}

int CCharacter::GetRole()
{
	return *(int*)(this + 0x770);
}

bool CCharacter::GetIsSpawned()
{
	//return (((int*)(address + 0x680))[0] != 0 && ((int*)(address + 0x674))[0] != 0);
	return (((int*)(this + 0x674))[0] != 0);
}

void CCharacter::SetIsSpawned(bool spawned)
{
	//return (((int*)(address + 0x680))[0] != 0 && ((int*)(address + 0x674))[0] != 0);
	((char*)(this + 0x674))[0] = spawned ? 1 : 0;
}

bool CCharacter::GetShadowVisible() {
	return (((char*)(this + 0x74c))[0] != 0);
}

void CCharacter::SetShadowVisible(bool set)
{
	((char*)(this + 0x74c))[0] = set ? 1 : 0;
}

float CCharacter::GetAlpha() {
	return ((float*)(this + 0x5e4))[0];
}

void CCharacter::SetAlpha(float alpha)
{
	((float*)(this + 0x5e4))[0] = alpha;
}

int CCharacter::GetRenderMode() {
	return ((int*)(this + 0x5e8))[0];
}

void CCharacter::SetRenderMode(int rendermode)
{
	((int*)(this + 0x5e8))[0] = rendermode;
}