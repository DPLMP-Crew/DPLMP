#include "dpl/CLifeSystem.h"

CLifePlayer* CLifeSystem::GetPlayer()
{
    return pPlayer;
}

CLifeSystem* CLifeSystem::GetInstance()
{
    return *(CLifeSystem**)0x70C7C4;
}