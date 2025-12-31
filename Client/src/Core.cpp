#include "Core.h"
#include <safetyhook.hpp>

namespace Core {

	static SafetyHookInline getForcedLevelHook{};

	static int __stdcall GetForcedLevel_Hook()
	{
		return 1;
	}

	void Initialize() {
		// Purpose: Steam EXE gets unpacked on boot, so hooking before this crashes the game.
		// TODO - Find a better way to do this.
		uint8_t sig[] = { 0x8B, 0x41, 0x20, 0xC3 };
		while (memcmp((void*)0x46746d, sig, sizeof(sig)) != 0)
		{
			//Wait
		}
		printf("Sig matched, hooking.\n");
		getForcedLevelHook = safetyhook::create_inline(0x46746d, GetForcedLevel_Hook);
	}
}