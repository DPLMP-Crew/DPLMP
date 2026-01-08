#pragma once
#include "dpl/CPlayerDriverBehaviour.h"

class CLifePlayer {
public:
	int field0;
	int field4;
	CPlayerDriverBehaviour* pDriverBehaviour;

	CPlayerDriverBehaviour* GetDriverBehaviour();
};