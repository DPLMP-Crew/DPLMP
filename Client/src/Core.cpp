#include "Core.h"
#include <safetyhook.hpp>
#include "dpl/CLifeEventDataManager.h"
#include "dpl/SpoolableResourceManager.h"
#include "dpl/CLoadingScreen.h"
#include "scan.h"

namespace Core {

	static SafetyHookInline getForcedLevelHook{};
	static SafetyHookInline nodeOnEnableHook{};
	static SafetyHookInline cStateGameRunningHook{};
	static SafetyHookInline frameUpdateHook{};

	static int __stdcall GetForcedLevel_Hook()
	{
		// For THEN: 7
		// For NOW: 24
		return 24;
	}

	static void __stdcall Stub_Hook() {
		return;
	}

	static void* __fastcall CStateGameRunning_Hook(void* cState) {
		void* retVal = cStateGameRunningHook.thiscall<void*>(cState);
		CLifeEventDataManager::GetInstance()->EndAllLifeEvents();
		return retVal;
	}

	static void __fastcall FrameUpdate_Hook(void* me, void* _, void* unk1, void* unk2) {
		frameUpdateHook.thiscall(me, unk1, unk2);
		// Disable loading screen if nothing is being loaded.
		SpoolableResourceManager* resManager = SpoolableResourceManager::GetInstance();
		CLoadingScreen* loadingScreen = CLoadingScreen::GetInstance();
		if (resManager != nullptr && loadingScreen != nullptr)
		{
			int spoolStatus = resManager->GetStatus();
			if (spoolStatus != 0)
				loadingScreen->Deactivate();
		}
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
		// hook into Game Running state constructor to stop all life events.
		cStateGameRunningHook = safetyhook::create_inline(0x45ad10, CStateGameRunning_Hook);
		// Called every frame.
		frameUpdateHook = safetyhook::create_inline(0x570f27, FrameUpdate_Hook);
	}
}