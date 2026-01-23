#include "LuaCharacter.h"
#include "dpl/CLifeSystem.h"
#include "dpl/CLifePlayer.h"
#include "dpl/CPlayerDriverBehaviour.h"
#include "dpl/CWipeManager.h"
#include "dpl/CCharacterCategoryManager.h"
#include "mathutil.h"
#include "Core.h"

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

void GameCharacter::Revive() {
	m_Handle->SetHealth(1.0f);
	m_Handle->ForceIdleMode();
	m_Handle->SetDeadState(0);

	CCharacter* player = GetPlayerCharacter();

	if (player == m_Handle)
		CWipeManager::GetInstance()->ResetWipes();
}

void GameCharacter::SetPosition(float x, float y, float z) {
	Vector4 pos(x, y, z, 1.0);
	m_Handle->SetPosition(&pos);
}

void GameCharacter::SetRotation(float x, float y, float z, float w) {
	Vector4 pos;
	m_Handle->GetPosition(&pos);

	glm::vec3 newPos = glm::vec3(pos.X, pos.Y, pos.Z);
	glm::quat newRot = glm::normalize(glm::quat(w, x, y, z));

	glm::mat4 tf = glm::translate(glm::mat4(1.0f), newPos) * glm::mat4_cast(newRot);

	Matrix* charMatrix = m_Handle->GetMatrix();

	*charMatrix = fromGLM(tf);
}

void GameCharacter::SetHealth(float health) {
	m_Handle->SetHealth(health);
}

float GameCharacter::GetHealth() {
	return m_Handle->GetHealth();
}

std::tuple<float, float, float, float> GameCharacter::GetRotation() {
	Matrix* charMatrix = m_Handle->GetMatrix();
	glm::quat glmQuat = glm::quat_cast(toGLM(*charMatrix));
	return { glmQuat.x, glmQuat.y, glmQuat.z, glmQuat.w };
}

std::tuple<float, float, float> GameCharacter::GetPosition() {
	Vector4 pos;
	m_Handle->GetPosition(&pos);
	return { pos.X, pos.Y, pos.Z };
}

void GameCharacter::SwapSkin(int animations, int skin) {
	Core::ResourceMgr->Acquire(SpooledPackageType::Characters, skin);
	Core::ResourceMgr->Release(SpooledPackageType::Characters, m_Handle->GetModel());
	m_Handle->SwapSkin(animations, skin);
	CCharacter* player = GetPlayerCharacter();
	if (m_Handle == player)
		CCharacterCategoryManager::GetInstance()->SetPlayerAvatar(skin);
}

void LuaCharacter::Register(sol::state* lua) {

	sol::usertype<GameCharacter> characterType = lua->new_usertype<GameCharacter>(
		"Character",
		sol::no_constructor,
		"Revive", &GameCharacter::Revive,
		"SetPosition", &GameCharacter::SetPosition,
		"SetRotation", &GameCharacter::SetRotation,
		"SetHealth", &GameCharacter::SetHealth,
		"GetHealth", &GameCharacter::GetHealth,
		"GetPosition", &GameCharacter::GetPosition,
		"GetRotation", &GameCharacter::GetRotation,
		"SwapSkin", &GameCharacter::SwapSkin
	);

	sol::table charManagerTable = lua->create_named_table("CharacterManager");

	charManagerTable.set_function("GetPlayerCharacter",
		[]() {
			return GameCharacter(GetPlayerCharacter());
		}
	);
}