#include "Core.h"
#include <safetyhook.hpp>
#include "dpl/CLifeEventDataManager.h"
#include "dpl/SpoolableResourceManager.h"
#include "dpl/CLoadingScreen.h"
#include "dpl/CCharacterManager.h"
#include "dpl/CVehicleManager.h"
#include "dpl/LifeAcquirableVehicleManager.h"
#include "dpl/AIManager.h"
#include "dpl/CharacterAIManager.h"
#include "scan.h"
#include <shellapi.h>

#define ERA_NOW 1
#define ERA_THEN 0

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

	static bool DeveloperMode = false;
	static bool EnableNormalGameLoop = false;
	static bool EnableTraffic = false;
	static bool EnableCopTraffic = false;
	static int DeveloperEra = ERA_THEN;

	static int __stdcall GetForcedLevel_Hook()
	{
		// For THEN: 7
		// For NOW: 24
		if (DeveloperMode) {
			if (DeveloperEra == ERA_NOW)
				return 24;
			else
				return 7;
		}
		return 24;
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

	static bool wasKeyJustPressed(int vk)
	{
		static SHORT lastState[256]{};

		SHORT state = GetAsyncKeyState(vk);
		bool pressed = (state & 0x8000) && !(lastState[vk] & 0x8000);

		lastState[vk] = state;
		return pressed;
	}

	static void DebugFrameUpdate() {

		static AutoPtr<CCharacter, int>* createdChar = new AutoPtr<CCharacter, int>();
		static AutoPtr<CVehicle, int>* createdVehicle = new AutoPtr<CVehicle, int>();

		if (wasKeyJustPressed(VK_F4)) {
			printf("Creating a car!\n");
			CVehicleManager* vmgr = CVehicleManager::GetInstance();
			LifeAcquirableVehicleManager* amgr = LifeAcquirableVehicleManager::GetInstance();
			if (createdVehicle != nullptr && createdVehicle->m_pPointer != nullptr) {
				vmgr->FreeVehicle(createdVehicle);
			}
			vmgr->CreateVehicle(createdVehicle, tVehicleModelUID::tVehicleModelUID_Andec, 2465.992f, 39.996f, -4370.734f, 1.0f, 0.0f, true, true, false);
			amgr->AddVehicle(createdVehicle->m_pPointer, 1);
		}

		if (wasKeyJustPressed(VK_F3))
		{
			printf("CharacterManager: %p\nVehicleManager: %p\nLifeAcquirableVehicleManager: %p\nAIManager: %p\nCharacterAIManager: %p\n", CCharacterManager::GetInstance(), CVehicleManager::GetInstance(), LifeAcquirableVehicleManager::GetInstance(), AIManager::GetInstance(), CharacterAIManager::GetInstance());
		}

		if (wasKeyJustPressed(VK_F2))
		{
			printf("Creating a guy!\n");
			CCharacterManager* cmgr = CCharacterManager::GetInstance();
			if (createdChar != nullptr && createdChar->m_pPointer != nullptr) {
				cmgr->FreeCharacter(createdChar);
			}
			AutoPtr<CWeapon, int> weaponPtr = AutoPtr<CWeapon, int>();
			Vector4 pos = Vector4();
			pos.X = 2461.992f;
			pos.Y = 39.996f;
			pos.Z = -4368.734;
			pos.W = 1.0f;
			Vector4 vel = Vector4();
			vel.X = 0.0f;
			vel.Y = 0.0f;
			vel.Z = 0.0f;
			vel.W = 0.0f;
			cmgr->CreateCharacter(createdChar, ECharacterType::TheDriver_Now, &pos, 0.0f, EnumAnimCondition::eACond_Default, ECharacterRoles::eCharacterRole_Network, &vel, false, weaponPtr, &vel, false, 200.0f);
			cmgr->CharacterSetNoFelon(createdChar->m_pPointer, 2);
			//printf("Created character %p! Rendermode: %i\n", createdChar, createdChar->GetRole());
		}
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
		if (DeveloperMode) {
			DebugFrameUpdate();
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

	static void ParseCommandLine() {
		printf("Command line:\n");
		int argCount = 0;
		LPWSTR* args = CommandLineToArgvW(GetCommandLineW(), &argCount);
		if (args) {
			for (int i = 0; i < argCount; i++) {
				LPWSTR arg = args[i];

				wprintf(L"%s\n", arg);

				if (wcscmp(arg, L"-dev") == 0)
				{
					DeveloperMode = true;
				}
				else if (DeveloperMode)
				{
					if (wcscmp(arg, L"-cops") == 0)
						EnableCopTraffic = true;
					else if (wcscmp(arg, L"-traffic") == 0)
						EnableTraffic = true;
					else if (wcscmp(arg, L"-sp") == 0)
						EnableNormalGameLoop = true;
					else if (wcscmp(arg, L"-now") == 0)
						DeveloperEra = ERA_NOW;
				}
			}
			LocalFree(args);
		}
	}

	void Initialize() {
		WaitForSteamUnpack();
		printf("EXE unpacked, hooking.\n");
		ParseCommandLine();

		// Lock timescale to full speed.
		setGameSpeedMuliplierHook = safetyhook::create_inline(0x45ace8, SetGameSpeedMultiplier_Hook);

		// Called every frame.
		frameUpdateHook = safetyhook::create_inline(0x570f27, FrameUpdate_Hook);

		if (!EnableNormalGameLoop) {
			// Force in-game.
			getForcedLevelHook = safetyhook::create_inline(0x46746d, GetForcedLevel_Hook);
			// Stub out CStatusControlNode::OnEnable to disable life events.
			nodeOnEnableHook = safetyhook::create_inline(0x48719a, Stub_Hook);
			// hook into Game Running state constructor to stop all life events.
			cStateGameRunningHook = safetyhook::create_inline(0x45ad10, CStateGameRunning_Hook);
		}

		if (!EnableTraffic) {
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
		}

		if (!EnableCopTraffic) {
			// Stops creation of cop vehicles.
			createCopVehicleHook = safetyhook::create_inline(0x40fbee, CreateCopVehicle_Hook);
		}

		// Below causes a crash due to a buffer overflow in the character pool.
		// The pool is in CCharacterManager, offset 0x318, right before free character count. The char count is overriden if we go any higher.
		// To truly fix this, we would have to heap allocate the pool (or allocate it in this dll space), and hook CCharacterManager::GetFreeCharacter and whatever else uses the pool. Which would take work.
		//char pplAmount = 48;
		//WriteToMemory((DWORD)0x4f764e, &pplAmount, 1);
	}
}