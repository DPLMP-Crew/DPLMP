#include "Core.h"
#include <safetyhook.hpp>
#include "dpl/CLifeEventDataManager.h"
#include "scan.h"

namespace Core {

	static SafetyHookInline getForcedLevelHook{};
	static SafetyHookInline nodeOnEnableHook{};
	static SafetyHookInline lifeSystemInitialiseHook{};

	static int __stdcall GetForcedLevel_Hook()
	{
		// For THEN: 7
		// For NOW: 24
		return 24;
	}

	static void __stdcall Stub_Hook() {
		return;
	}

	static void __fastcall LifeSystemInitialise_Hook(void* cLifeSystem) {
		lifeSystemInitialiseHook.thiscall(cLifeSystem);
		CLifeEventDataManager::GetInstance()->EndAllLifeEvents();
	}

	static void WaitForSteamUnpack() {
		// Purpose: Steam EXE gets unpacked on boot, so hooking before this crashes the game or doesn't work.
		// TODO - Find a better way to do this.
		//uint8_t sig[] = { 0x8B, 0x41, 0x20, 0xC3 };
		//while (memcmp((void*)0x46746d, sig, sizeof(sig)) != 0)
		uint8_t sig[] = { 0x6A, 0x01, 0xFF, 0x71, 0x30 };
		while (memcmp((void*)0x48719a, sig, sizeof(sig)) != 0)
		{
			//Wait
		}
	}

	void Initialize() {
		WaitForSteamUnpack();
		printf("EXE unpacked, hooking.\n");
		
		// Force in-game.
		getForcedLevelHook = safetyhook::create_inline(0x46746d, GetForcedLevel_Hook);
		// Stub out CStatusControlNode::OnEnable to disable life events.
		nodeOnEnableHook = safetyhook::create_inline(0x48719a, Stub_Hook);
		// hook into CLifeSystem::Initialise to end all life events.
		lifeSystemInitialiseHook = safetyhook::create_inline(0x471e82, LifeSystemInitialise_Hook);
		// MakeJMP((BYTE*)0x004720bf, (DWORD)EnterInGameStateHook, 5);
	}
}