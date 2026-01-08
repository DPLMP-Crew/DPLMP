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

	static bool DeveloperMode = false;
	static bool EnableNormalGameLoop = false;
	static bool EnableTraffic = false;
	static bool EnableCopTraffic = false;
	static int DeveloperEra = ERA_THEN;

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

	static void PrintGlmMatrix(const glm::mat4& matrix) {
		printf("%f %f %f %f\n", matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]);
		printf("%f %f %f %f\n", matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]);
		printf("%f %f %f %f\n", matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]);
		printf("%f %f %f %f\n", matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
	}

	static void PrintGameMatrix(const Matrix& matrix) {
		printf("%f %f %f %f\n", matrix.right.X, matrix.right.Y, matrix.right.Z, matrix.right.W);
		printf("%f %f %f %f\n", matrix.up.X, matrix.up.Y, matrix.up.Z, matrix.up.W);
		printf("%f %f %f %f\n", matrix.forward.X, matrix.forward.Y, matrix.forward.Z, matrix.forward.W);
		printf("%f %f %f %f\n", matrix.pos.X, matrix.pos.Y, matrix.pos.Z, matrix.pos.W);
	}

	static void DebugFrameUpdate() {

		static AutoPtr<CCharacter, int>* createdChar = new AutoPtr<CCharacter, int>();
		static AutoPtr<CVehicle, int>* createdVehicle = new AutoPtr<CVehicle, int>();

		if (wasKeyJustPressed(VK_F6)) {

			GameCamera* gameCam = GameCamera::GetInstance();
			if (gameCam != nullptr) {
				glm::mat4 toGlmMatrix = toGLM(gameCam->m_Matrix);
				Matrix fromGlmMatrix = fromGLM(toGlmMatrix);

				printf("\n");

				printf("Camera Matrix (Game):\n");
				PrintGameMatrix(gameCam->m_Matrix);
				printf("\n");

				printf("Camera Matrix (Game -> GLM):\n");
				PrintGlmMatrix(toGlmMatrix);
				printf("\n");

				printf("Camera Matrix (Game -> GLM -> Game):\n");
				PrintGameMatrix(fromGlmMatrix);
				printf("\n");

				printf("Position Matrix (GLM):\n");
				PrintGlmMatrix(glm::translate(glm::mat4(1.0), glm::vec3(toGLM(gameCam->m_Matrix.pos))));
				printf("\n");
				
				/*
				D3DXVECTOR3 from(gameCam->m_Matrix.pos.X + 10.0, gameCam->m_Matrix.pos.Y + 10.0, gameCam->m_Matrix.pos.Z);
				D3DXVECTOR3 at(gameCam->m_Matrix.pos.X, gameCam->m_Matrix.pos.Y, gameCam->m_Matrix.pos.Z);
				D3DXVECTOR3 up(0.0, 1.0, 0.0);

				D3DXMATRIX lookAt;
				D3DXMatrixLookAtLH(&lookAt, &from, &at, &up);

				printf("Regular Matrix: %p\n\n", gameCam);
				printf("%f %f %f %f\n", gameCam->m_Matrix.right.X, gameCam->m_Matrix.right.Y, gameCam->m_Matrix.right.Z, gameCam->m_Matrix.right.W);
				printf("%f %f %f %f\n", gameCam->m_Matrix.up.X, gameCam->m_Matrix.up.Y, gameCam->m_Matrix.up.Z, gameCam->m_Matrix.up.W);
				printf("%f %f %f %f\n", gameCam->m_Matrix.forward.X, gameCam->m_Matrix.forward.Y, gameCam->m_Matrix.forward.Z, gameCam->m_Matrix.forward.W);
				printf("%f %f %f %f\n", gameCam->m_Matrix.pos.X, gameCam->m_Matrix.pos.Y, gameCam->m_Matrix.pos.Z, gameCam->m_Matrix.pos.W);

				printf("LookAt Matrix: %p\n\n", gameCam);
				printf("%f %f %f %f\n", lookAt._11, lookAt._12, lookAt._13, lookAt._14);
				printf("%f %f %f %f\n", lookAt._21, lookAt._22, lookAt._23, lookAt._24);
				printf("%f %f %f %f\n", lookAt._31, lookAt._32, lookAt._33, lookAt._34);
				printf("%f %f %f %f\n", lookAt._41, lookAt._42, lookAt._43, lookAt._44);*/
			}
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
	/*
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

		Matrix* playerMatrix = playerCharacter->GetMatrix();

		D3DXVECTOR3 from(charPos.X + 10.0, charPos.Y + 10.0, charPos.Z);
		D3DXVECTOR3 at(charPos.X, charPos.Y, charPos.Z);
		D3DXVECTOR3 up(0.0, 1.0, 0.0);

		D3DXMATRIX lookAtView;
		D3DXMATRIX lookAtWorld;
		D3DXMatrixLookAtRH(&lookAtView, &from, &at, &up);
		D3DXMatrixInverse(&lookAtWorld, nullptr, &lookAtView);

		gameCam->m_Matrix.right = Vector4(lookAtWorld._11, lookAtWorld._12, lookAtWorld._13, lookAtWorld._14);
		gameCam->m_Matrix.up = Vector4(lookAtWorld._21, lookAtWorld._22, lookAtWorld._23, lookAtWorld._24);
		gameCam->m_Matrix.forward = Vector4(lookAtWorld._31, lookAtWorld._32, lookAtWorld._33, lookAtWorld._34);
		gameCam->m_Matrix.pos = Vector4(lookAtWorld._41, lookAtWorld._42, lookAtWorld._43, lookAtWorld._44);
	}*/

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

		// Don't pull up game over menu - respawning should be handled via Lua. Just have to re-set health, force stand up and wipe the screen.
		handlePlayerDeadHook = safetyhook::create_inline(0x45b315, HandlePlayerDead_Hook);

		setCameraMatrixHook = safetyhook::create_inline(0x55a646, SetCameraMatrix_Hook);

		// Below causes a crash due to a buffer overflow in the character pool.
		// The pool is in CCharacterManager, offset 0x318, right before free character count. The char count is overriden if we go any higher.
		// To truly fix this, we would have to heap allocate the pool (or allocate it in this dll space), and hook CCharacterManager::GetFreeCharacter and whatever else uses the pool. Which would take work.
		//char pplAmount = 48;
		//WriteToMemory((DWORD)0x4f764e, &pplAmount, 1);
	}
}