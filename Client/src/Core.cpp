#include "Core.h"
#include <safetyhook.hpp>
#include "dpl/CLifeEventDataManager.h"
#include "dpl/SpoolableResourceManager.h"
#include "dpl/CLoadingScreen.h"
#include "scan.h"
#include <shellapi.h>

namespace Core {

	static SafetyHookInline getForcedLevelHook{};
	static SafetyHookInline nodeOnEnableHook{};
	static SafetyHookInline cStateGameRunningHook{};
	static SafetyHookInline frameUpdateHook{};
	static SafetyHookInline setGameSpeedMuliplierHook{};
	static SafetyHookInline createTrafficPedestrianHook{};
	static SafetyHookInline createScriptedPedestrianHook{};
	static SafetyHookInline createTeleportTrafficHook{};
	static SafetyHookInline trafficVehicleHook{};
	static SafetyHookInline createCopVehicleHook{};

	static int __stdcall GetForcedLevel_Hook()
	{
		// For THEN: 7
		// For NOW: 24
		return 7;
	}

	static void __stdcall Stub_Hook() {
		return;
	}

	static void __fastcall CreateTeleportTraffic_Hook(void* me) {
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

	static void __fastcall TrafficVehicle_Hook(void* me, void* _, void* unk1, void* unk2) {
		return;
	}

	static void __stdcall SetGameSpeedMultiplier_Hook(float speed) {
		setGameSpeedMuliplierHook.stdcall(1.0f);
	}

	static bool __fastcall CreateTrafficPedestrian_Hook(void* me, void* _, void* unk) {
		return false;
	}

	static void __fastcall CreateScriptedPedestrian_Hook(void* me) {
		return;
	}

	static void* __fastcall CreateCopVehicle_Hook(void* me, void* _, void* unk1, void* unk2, void* unk3, void* unk4, void* unk5, void* unk6)
	{
		return nullptr;
	}

	static void WaitForSteamUnpack() {
		// Purpose: Steam EXE gets unpacked on boot, so hooking before this crashes the game or doesn't work.
		// TODO - Find a better way to do this.
		uint8_t sig[] = { 0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x10 };
		while (memcmp((void*)0x634fbd, sig, sizeof(sig)) != 0)
		{
			//Wait
		}
	}

	void Initialize() {
		WaitForSteamUnpack();
		printf("EXE unpacked, hooking.\n");
		printf("Command line:\n");
		int argCount = 0;
		LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &argCount);
		if (args) {
			for (int i = 0; i < argCount; i++) {
				wprintf(L"%s\n", args[i]);
			}
			LocalFree(args);
		}
		
		// Force in-game.
		getForcedLevelHook = safetyhook::create_inline(0x46746d, GetForcedLevel_Hook);
		// Stub out CStatusControlNode::OnEnable to disable life events.
		nodeOnEnableHook = safetyhook::create_inline(0x48719a, Stub_Hook);
		// hook into Game Running state constructor to stop all life events.
		cStateGameRunningHook = safetyhook::create_inline(0x45ad10, CStateGameRunning_Hook);
		// Called every frame.
		frameUpdateHook = safetyhook::create_inline(0x570f27, FrameUpdate_Hook);
		// Lock timescale to full speed.
		setGameSpeedMuliplierHook = safetyhook::create_inline(0x45ace8, SetGameSpeedMultiplier_Hook);

		// REMOVE TRAFFIC HOOKS
		// 
		// Removes traffic peds.
		createTrafficPedestrianHook = safetyhook::create_inline(0x44884B, CreateTrafficPedestrian_Hook);
		// Removes scripted ped spawns (hobos around barrels, food stands, etc.)
		createScriptedPedestrianHook = safetyhook::create_inline(0x449388, CreateScriptedPedestrian_Hook);
		// Removes ped and car traffic generation on teleport.
		createTeleportTrafficHook = safetyhook::create_inline(0x412803, CreateTeleportTraffic_Hook);
		// Stops creation of AI traffic vehicles.
		trafficVehicleHook = safetyhook::create_inline(0x410bab, TrafficVehicle_Hook);
		// Stops creation of cop vehicles.
		createCopVehicleHook = safetyhook::create_inline(0x40fbee, CreateCopVehicle_Hook);
		/*
		// creates traffic on teleport
		Hooking::MakeJMP((BYTE*)0x00412803, (DWORD)ReturnEarly, 5);
		// ai vehicles
		Hooking::MakeJMP((BYTE*)0x00412C1A, (DWORD)AIVehicleManagerHook, 5);
		// cop vehicles
		Hooking::MakeJMP((BYTE*)0x0040FC04, (DWORD)CreateCopVehicleHook, 5);
		// scripted peds (?
		Hooking::MakeJMP((BYTE*)0x004488D0, (DWORD)CreatePedsHook, 5);
		// jump over other vehicle creation
		Nop((BYTE*)0x42d007, 2);
		MakeJMP((BYTE*)0x42D010, 0x42EA8D, 6);*/
	}
}