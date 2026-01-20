#include "dpl/CHandling.h"

void CHandling::SendManipulationPacket(float* packet) {
	//45
	int vTableAddr = *(int*)this;
	((void(__thiscall*)(CHandling*, float*)) * (int*)(vTableAddr + 180))(this, packet);
}