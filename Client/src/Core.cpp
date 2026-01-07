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
#include <vector>
#include <fstream>
#include <codecvt>
#include <filesystem>
#include "dpl/CLifeInstance_Character.h"

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
	static SafetyHookInline characterStepRenderingOptionsHook{};
	static SafetyHookInline characterInitialiseHook{};

	static bool DeveloperMode = false;
	static bool EnableNormalGameLoop = false;
	static bool EnableTraffic = false;
	static bool EnableCopTraffic = false;
	static int DeveloperEra = ERA_THEN;

	static CWeapon* WeaponCopy = nullptr;

	std::wstring Directory;

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

	static void __fastcall CLifeInstance_Character_Initialise_Hook(void* me, void* _,
		void* _1,
		void* _2,
		void* _3,
		void* _4,
		void* _5,
		void* _6,
		void* _7,
		void* _8,
		void* _9,
		void* _10,
		void* _11,
		void* _12,
		void* _13,
		void* _14,
		void* _15,
		void* _16,
		CWeapon* weapon,
		void* _18,
		void* _19,
		void* _20,
		void* _21,
		void* _22,
		void* _23,
		void* _24,
		void* _25,
		void* _26) {

		if (WeaponCopy == nullptr) {
			WeaponCopy = new CWeapon();
			memcpy(WeaponCopy, weapon, sizeof(CWeapon));
			printf("Copied weapon to {%p}!\n", WeaponCopy);
		}

		characterInitialiseHook.thiscall(me,
			_1,
			_2,
			_3,
			_4,
			_5,
			_6,
			_7,
			_8,
			_9,
			_10,
			_11,
			_12,
			_13,
			_14,
			_15,
			_16,
			weapon,
			_18,
			_19,
			_20,
			_21,
			_22,
			_23,
			_24,
			_25,
			_26
		);
	}

	static void __stdcall Stub_Hook() {
		return;
	}

	static void __fastcall CreateTeleportTraffic_Hook(void* me) {
		return;
	}

	static void __fastcall StepRenderingOptions_Hook(CCharacter* character) {
		characterStepRenderingOptionsHook.thiscall(character);
		character->SetShadowVisible(true);
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

		static AutoPtr<CLifeInstance_Character, int>* createdLifeChar = new AutoPtr<CLifeInstance_Character, int>();

		if (wasKeyJustPressed(VK_F6)) {

			Matrix* posMat = new Matrix();
			posMat->right = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
			posMat->up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
			posMat->forward = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
			posMat->pos = Vector4(2465.992f, 39.996f, -4370.734f, 1.0f);

			Vector4* zeroVec = new Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			Vector4* zeroVec2 = new Vector4(0.0f, 0.0f, 0.0f, 1.0f);

			Matrix* emptyMat = new Matrix();

			CLifeInstance_Character* lifeChar = new CLifeInstance_Character();
			printf("Created LifeCharacter (%p)\n", lifeChar);
			
			lifeChar->Initialise(
				createdLifeChar,
				0xefcdab0,
				ECharacterType::TheKid_Then,
				posMat,
				nullptr,
				0,
				0,
				&posMat->pos,
				&posMat->pos,
				zeroVec,
				emptyMat,
				0.0f,
				0,
				emptyMat,
				emptyMat,
				emptyMat,
				WeaponCopy,
				0,
				0,
				0,
				0,
				0,
				nullptr,
				zeroVec2,
				emptyMat,
				1.0f
			);

			lifeChar->CoreCreate();
		}

		if (wasKeyJustPressed(VK_F5)) {
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

	static void ProcessCommand(const std::wstring& cmd)
	{
		if (wcscmp(cmd.c_str(), L"-dev") == 0) {
			DeveloperMode = true;
		}
		else if (DeveloperMode) {
			if (wcscmp(cmd.c_str(), L"-cops") == 0)
				EnableCopTraffic = true;
			else if (wcscmp(cmd.c_str(), L"-traffic") == 0)
				EnableTraffic = true;
			else if (wcscmp(cmd.c_str(), L"-sp") == 0)
				EnableNormalGameLoop = true;
			else if (wcscmp(cmd.c_str(), L"-now") == 0)
				DeveloperEra = ERA_NOW;
		}
	}

	static void ParseCommandLine() {
		std::wstring cmdPath = Directory + L"\\cmd_cache.bin";
		std::ifstream file(cmdPath);

		if (!file.good()) {
			printf("No command line.\n");
			return;
		}

		std::string line;

		while (std::getline(file, line)) {
			int count = MultiByteToWideChar(CP_UTF8, 0, line.data(), line.size(), nullptr, 0);

			if (count <= 0)
				continue;

			std::wstring result(count, L'\0');

			MultiByteToWideChar(CP_UTF8, 0, line.data(), line.size(), result.data(), count);

			ProcessCommand(result);
		}

		file.close();

		try {
			std::filesystem::remove(cmdPath);
		}
		catch (const std::filesystem::filesystem_error& err) {
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

		// Small graphical improvement, shadows on all characters + while on vehicle (for better bike visuals)
		characterStepRenderingOptionsHook = safetyhook::create_inline(0x4e9d31, StepRenderingOptions_Hook);

		characterInitialiseHook = safetyhook::create_inline(0x47e1a6, CLifeInstance_Character_Initialise_Hook);

		// Below causes a crash due to a buffer overflow in the character pool.
		// The pool is in CCharacterManager, offset 0x318, right before free character count. The char count is overriden if we go any higher.
		// To truly fix this, we would have to heap allocate the pool (or allocate it in this dll space), and hook CCharacterManager::GetFreeCharacter and whatever else uses the pool. Which would take work.
		//char pplAmount = 48;
		//WriteToMemory((DWORD)0x4f764e, &pplAmount, 1);
	}
}