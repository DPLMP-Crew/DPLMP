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
#include "D3DHooks.h"
#include "dpl/GameCamera.h"
#include "dpl/CLifeSystem.h"
#include "mathutil.h"
#include "dpl/CWipeManager.h"
#include "LuaEngine.h"
#include "dpl/UINotification.h"
#include <chrono>
#include "LuaHooks.h"
#include "dpl/CHandling.h"
#include "dpl/CPlaylist.h"

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
	static SafetyHookInline handlePlayerDeadHook{};
	static SafetyHookInline setCameraMatrixHook{};
	static SafetyHookInline updateOverheadMapHook{};
	static SafetyHookInline isCheatActiveHook{};
	static SafetyHookInline playerDriverBehaviourStepHook{};
	static SafetyHookInline sendManipulationPacketHook{};
	static SafetyHookInline pauseSoundManagerHook{};
	static SafetyHookInline pauseMusicManagerHook{};
	static SafetyHookInline renderCharactersHook{};
	static SafetyHookInline createFileHook{};
	static SafetyHookInline spoolableResManagerIsRequiredHook{};

	static bool DeveloperMode = false;
	static bool EnableNormalGameLoop = false;
	static bool EnableTraffic = false;
	static bool EnableCopTraffic = false;
	static bool ShaderExtractMode = false;
	static int DeveloperEra = ERA_THEN;

	static std::unique_ptr<LuaEngine> Lua;

	std::wstring GameDirectory;
	std::wstring Directory;
	bool PlayerSpawned = true;
	float DeltaTime = 0.0;
	std::unique_ptr<ResourceManager> ResourceMgr;
	std::unique_ptr<VehicleSpawner> VehSpawner;

	static bool PlayerDead = false;

	bool IsGamePaused() {
		return (((char*)0x6e73e2)[0] == 1);
	}

	static void __fastcall PauseMusicManager_Hook(void* me) {
		return;
	}

	static void __fastcall PauseSoundManager_Hook(void* me) {
		return;
	}

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

	static void __fastcall RenderCharacters_Hook(void* me, void* _, void* viewport) {
		renderCharactersHook.thiscall(me, viewport);
	}

	static void __fastcall Handling_SendManipulationPacket_Hook(float* me, void* _, float* packet) {
		/*
		if (IsGamePaused() || !PlayerSpawned) {
			packet[0] = 0.0;
			packet[1] = 0.0;
			packet[2] = 0.0;
		}*/
		//printf("Sending manipulation packet 0: %f, 1: %f, 2: %f\n", packet[0], packet[1], packet[2]);
		sendManipulationPacketHook.thiscall(me, packet);
	}

	static void __fastcall PlayerDriverBehaviour_Step_Hook(CPlayerDriverBehaviour* me) {
		if (IsGamePaused() || !PlayerSpawned) {
			CVehicle* veh = me->GetCharacter()->GetVehicle();
			if (veh != nullptr) {
				CHandling* handling = nullptr;
				veh->GetHandling(&handling);
				float manip[] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
				handling->SendManipulationPacket(manip);
			}
			me->AquaticCollisionCheck();
			return;
		}
		playerDriverBehaviourStepHook.thiscall(me);
	}

	static bool __fastcall IsCheatActive_Hook(void* me, void* _, int cheatId) {
		return false;
	}

	static void __fastcall UpdateOverheadMap_Hook(void* me) {
		if (IsGamePaused()) return;
		updateOverheadMapHook.thiscall(me);
	}

	static void __fastcall HandlePlayerDead_Hook(void* me) {
		return;
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
		if (!EnableNormalGameLoop)
			CLifeEventDataManager::GetInstance()->EndAllLifeEvents();
		printf("Starting up Lua virtual machine.\n");
		Lua = std::make_unique<LuaEngine>();
		printf("Starting up ResourceManager.\n");
		ResourceMgr = std::make_unique<ResourceManager>();
		printf("Starting up VehicleSpawner.\n");
		VehSpawner = std::make_unique<VehicleSpawner>();
		if (DeveloperMode)
		{
			std::wstring luaScriptPath = Directory + L"\\data\\scripts\\dev.lua";
			Lua->RunScriptFromFile(luaScriptPath);
		}
		CPlaylist* musicPlaylist = (CPlaylist*)0x6cfd08;
		musicPlaylist->Shuffle();
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

		if (wasKeyJustPressed(VK_F6)) {
			// Revive if dead
			CLifeSystem* lifeSystem = CLifeSystem::GetInstance();
			CLifePlayer* play = lifeSystem->GetPlayer();
			CPlayerDriverBehaviour* bhav = play->GetDriverBehaviour();
			CCharacter* character = bhav->GetCharacter();

			if (character->GetHealth() <= 0.0f) {
				character->SetHealth(1.0f);
				character->ForceIdleMode();
				CWipeManager::GetInstance()->ResetWipes();
				character->SetDeadState(0);
			}
		}
		//D3DHooks::ExtractMode = false;
		if (wasKeyJustPressed(VK_F5)) {
			D3DHooks::ExtractMode = true;
			/*
			UINotification::SetCustomString("Car Spawned!");
			UINotification::GetInstance()->Clear();
			UINotification::GetInstance()->Show(0, 0, 999, 5.0f, true);
			VehSpawner->RequestVehicle(tVehicleModelUID::tVehicleModelUID_Andec, 2465.992f, 39.996f, -4370.734f, 0.0f, true);*/
			/*
			UINotification::SetCustomString("Car Spawned!");
			UINotification::GetInstance()->Show(0, 0, 999, 5.0f, true);
			printf("Creating a car!\n");
			CVehicleManager* vmgr = CVehicleManager::GetInstance();
			LifeAcquirableVehicleManager* amgr = LifeAcquirableVehicleManager::GetInstance();
			if (createdVehicle != nullptr && createdVehicle->m_pPointer != nullptr) {
				vmgr->FreeVehicle(createdVehicle);
			}
			vmgr->CreateVehicle(createdVehicle, tVehicleModelUID::tVehicleModelUID_Andec, 2465.992f, 39.996f, -4370.734f, 1.0f, 0.0f, true, true, false);
			amgr->AddVehicle(createdVehicle->m_pPointer, 1);*/
		}

		if (wasKeyJustPressed(VK_F3))
		{
			printf("CharacterManager: %p\nVehicleManager: %p\nLifeAcquirableVehicleManager: %p\nAIManager: %p\nCharacterAIManager: %p\nPlayerDriverBehaviour: %p\n", CCharacterManager::GetInstance(), CVehicleManager::GetInstance(), LifeAcquirableVehicleManager::GetInstance(), AIManager::GetInstance(), CharacterAIManager::GetInstance(), CLifeSystem::GetInstance()->GetPlayer()->GetDriverBehaviour());
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

	static CCharacter* GetPlayerCharacter() {
		CLifeSystem* lifeSystem = CLifeSystem::GetInstance();
		if (lifeSystem == nullptr) return nullptr;
		CLifePlayer* lifePlayer = lifeSystem->GetPlayer();
		if (lifePlayer == nullptr) return nullptr;
		CPlayerDriverBehaviour* bhav = lifePlayer->GetDriverBehaviour();
		if (bhav == nullptr) return nullptr;
		CCharacter* ch = bhav->GetCharacter();
		return ch;
	}

	static std::chrono::steady_clock::time_point deltaTimePoint;

	static void __fastcall FrameUpdate_Hook(void* me) {
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		std::chrono::duration<float> delta = now - deltaTimePoint;
		DeltaTime = delta.count();
		deltaTimePoint = now;

		frameUpdateHook.thiscall(me);
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

		if (VehSpawner != nullptr) {
			VehSpawner->Update();
		}

		if (Lua != nullptr) {
			CCharacter* playerCharacter = GetPlayerCharacter();

			if (playerCharacter != nullptr) {
				if (playerCharacter->GetHealth() > 0.0f)
				{
					PlayerDead = false;
				}
				else
				{
					if (!PlayerDead) {
						Lua->m_LuaHooks.FireHook(HookType::OnLocalPlayerDead);
						PlayerDead = true;
					}
				}
			}
			Lua->m_LuaHooks.FireHook(HookType::OnFrameUpdate, DeltaTime);
			Lua->Update(DeltaTime);
		}
	}
	
	static void DoLookAtCam() {
		GameCamera* gameCam = GameCamera::GetInstance();
		CLifeSystem* lifeSystem = CLifeSystem::GetInstance();
		if (gameCam == nullptr || lifeSystem == nullptr) return;
		CLifePlayer* lifePlayer = lifeSystem->GetPlayer();
		if (lifePlayer == nullptr) return;
		CPlayerDriverBehaviour* bhav = lifePlayer->GetDriverBehaviour();
		if (bhav == nullptr) return;
		CCharacter* playerCharacter = bhav->GetCharacter();
		if (playerCharacter == nullptr) return;

		Vector4 charPos;

		playerCharacter->GetPosition(&charPos);

		glm::vec3 from(charPos.X + 10.0, charPos.Y + 10.0, charPos.Z);
		glm::vec3 at(charPos.X, charPos.Y, charPos.Z);

		glm::vec3 heading = glm::normalize(from - at);
		glm::vec3 right = glm::normalize(glm::cross(VEC3_UP, heading));
		glm::vec3 up = glm::normalize(glm::cross(heading, right));

		gameCam->m_Matrix.right = fromGLM(glm::vec4(right, 0.0));
		gameCam->m_Matrix.up = fromGLM(glm::vec4(up, 0.0));
		gameCam->m_Matrix.forward = fromGLM(glm::vec4(heading, 0.0));
		gameCam->m_Matrix.pos = fromGLM(glm::vec4(from, 1.0));
	}

	static void __fastcall SetCameraMatrix_Hook(void* me) {
		//DoLookAtCam();
		setCameraMatrixHook.thiscall(me);
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

	static bool __fastcall SpoolableResourceManager_IsRequired_Hook(void* me, void* _, SpooledPackageType packageType, int id)
	{
		if (ResourceMgr != nullptr && ResourceMgr->IsAlive(packageType, id)) return true;
		return spoolableResManagerIsRequiredHook.thiscall<bool>(me, packageType, id);
	}

	static LPCWSTR GetPrefix(LPCWSTR str, const std::wstring& prefix)
	{
		if (!str)
			return nullptr;

		size_t prefixLen = prefix.size();
		size_t strLen = wcslen(str);

		if (strLen < prefixLen)
			return nullptr;

		if (wcsncmp(str, prefix.c_str(), prefixLen) == 0)
			return str + prefixLen;
		return nullptr;
	}

	static HANDLE __stdcall CreateFileW_Hook(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
		if (dwDesiredAccess == GENERIC_READ && dwCreationDisposition == OPEN_EXISTING)
		{
			LPCWSTR prefix = GetPrefix(lpFileName, GameDirectory);
			if (prefix != nullptr) {
				std::wstring vfsPath = Core::Directory + L"\\data\\game" + prefix;
				
				HANDLE vfsHandle = createFileHook.stdcall<HANDLE>(vfsPath.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

				if (vfsHandle == INVALID_HANDLE_VALUE)
					return createFileHook.stdcall<HANDLE>(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
				else
					return vfsHandle;
			}
		}
		return createFileHook.stdcall<HANDLE>(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
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

		D3DHooks::Initialize();

		// Lock timescale to full speed.
		setGameSpeedMuliplierHook = safetyhook::create_inline(0x45ace8, SetGameSpeedMultiplier_Hook);

		// Called every frame.
		frameUpdateHook = safetyhook::create_inline(0x45ac50, FrameUpdate_Hook);

		if (!EnableNormalGameLoop) {
			// Force in-game.
			getForcedLevelHook = safetyhook::create_inline(0x46746d, GetForcedLevel_Hook);
			// Stub out CStatusControlNode::OnEnable to disable life events.
			nodeOnEnableHook = safetyhook::create_inline(0x48719a, Stub_Hook);
		}

		// hook into Game Running state constructor to stop all life events.
		cStateGameRunningHook = safetyhook::create_inline(0x45ad10, CStateGameRunning_Hook);

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

		// Don't pull up game over menu - respawning should be handled via Lua. Just have to re-set health, force stand up and wipe the screen.
		handlePlayerDeadHook = safetyhook::create_inline(0x45b315, HandlePlayerDead_Hook);

		setCameraMatrixHook = safetyhook::create_inline(0x55a646, SetCameraMatrix_Hook);

		playerDriverBehaviourStepHook = safetyhook::create_inline(0x4a27aa, PlayerDriverBehaviour_Step_Hook);

		// TODO: maybe only disable the follow player code, so we get realtime changes while in TAB map
		updateOverheadMapHook = safetyhook::create_inline(0x4730f1, UpdateOverheadMap_Hook);

		// No cheats ma'am
		isCheatActiveHook = safetyhook::create_inline(0x4c2ab6, IsCheatActive_Hook);

		sendManipulationPacketHook = safetyhook::create_inline(0x58cb57, Handling_SendManipulationPacket_Hook);

		// DONT PAUSE AUDIO IN PAUSE MENUS
		
		pauseSoundManagerHook = safetyhook::create_inline(0x4e24cf, PauseSoundManager_Hook);

		pauseMusicManagerHook = safetyhook::create_inline(0x4e20eb, PauseMusicManager_Hook);

		char jmp = 0xEB;
		WriteToMemory((DWORD)0x45b7f5, &jmp, 1);

		// END

		// Simple VFS for game overrides
		HMODULE kernelBase = GetModuleHandle("KERNELBASE.DLL");
		FARPROC fileCreateProc = GetProcAddress(kernelBase, "CreateFileW");
		createFileHook = safetyhook::create_inline(fileCreateProc, CreateFileW_Hook);

		// Prevent game from unloading assets that are in use by the mod ( LIKELY NOT NECESSARY, HANDLED BY THE SPOOL PRIORITY INGAME )
		// spoolableResManagerIsRequiredHook = safetyhook::create_inline(0x548973, SpoolableResourceManager_IsRequired_Hook);

		// Don't freeze game while paused.
		Nop((BYTE*)0x45ac8c, 2);

		static char thenFont[] = "LANG:FONTS\\70S\\FONT.BNK";
		static char nowFont[] = "LANG:FONTS\\90S\\FONT.BNK";

		char* fontPtr = nowFont;

		if (DeveloperMode && DeveloperEra != ERA_NOW)
			fontPtr = thenFont;

		WriteToMemory((DWORD)0x45c30f, &fontPtr, 4);
		WriteToMemory((DWORD)0x4a9491, &fontPtr, 4);
		WriteToMemory((DWORD)0x4a898c, &fontPtr, 4);

		renderCharactersHook = safetyhook::create_inline(0x4f6e49, RenderCharacters_Hook);

		// Below causes a crash due to a buffer overflow in the character pool.
		// The pool is in CCharacterManager, offset 0x318, right before free character count. The char count is overriden if we go any higher.
		// To truly fix this, we would have to heap allocate the pool (or allocate it in this dll space), and hook CCharacterManager::GetFreeCharacter and whatever else uses the pool. Which would take work.
		//char pplAmount = 48;
		//WriteToMemory((DWORD)0x4f764e, &pplAmount, 1);
	}
}