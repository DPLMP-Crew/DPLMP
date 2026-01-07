#include "dpl/CharacterAIManager.h"

CharacterAIManager* CharacterAIManager::GetInstance()
{
	return ((CharacterAIManager**)0x70C7EC)[0];
}