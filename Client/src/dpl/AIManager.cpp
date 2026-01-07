#include "dpl/AIManager.h"

AIManager* AIManager::GetInstance()
{
	return ((AIManager**)0x70c5c0)[0];
}