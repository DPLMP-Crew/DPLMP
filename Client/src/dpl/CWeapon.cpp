#include "dpl/CWeapon.h"

void CWeapon::Shoot()
{
	((void(__thiscall*)(CWeapon*))0x525268)(this);
}

void CWeapon::Reload()
{
	((void(__thiscall*)(CWeapon*))0x5253B4)(this);
}

bool CWeapon::IsAutomatic()
{
	return 	((bool(__thiscall*)(CWeapon*))0x525312)(this);
}