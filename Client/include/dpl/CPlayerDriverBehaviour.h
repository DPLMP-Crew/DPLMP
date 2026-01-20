#pragma once
#include "dpl/CCharacter.h"

class CPlayerDriverBehaviour {
public:
	CCharacter* GetCharacter();
	void AquaticCollisionCheck();
};