#pragma once
#include "dpl/CLifePlayer.h"

class CLifeSystem {
public:
	int field0;
	int field4;
	CLifePlayer* pPlayer;

	CLifePlayer* GetPlayer();

	static CLifeSystem* GetInstance();
};