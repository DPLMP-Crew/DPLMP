#include "ResourceManager.h"
#include "dpl/ECharacterType.h"

static uint64_t MakeKey(SpooledPackageType package, int id) {
	return (uint64_t(package) << 32) | uint64_t(id);
}

void ResourceManager::Acquire(SpooledPackageType package, int id) {
	if (package == SpooledPackageType::Characters && (id == ECharacterType::TheKid_Then || id == ECharacterType::TheDriver_Now)) return;

	SpoolableResourceManager* spool = SpoolableResourceManager::GetInstance();

	uint64_t key = MakeKey(package, id);
	auto it = m_Handles.find(key);

	if (it != m_Handles.end())
	{
		it->second.m_References++;
	}
	else
	{
		m_Handles[key] = ResourceHandle();
		printf("Loading asset id %i, package %i\n", id, package);
		spool->SetEntityPriority(package, id, SpoolPriority::Request);
	}
}

void ResourceManager::Release(SpooledPackageType package, int id) {
	if (package == SpooledPackageType::Characters && (id == ECharacterType::TheKid_Then || id == ECharacterType::TheDriver_Now)) return;

	SpoolableResourceManager* spool = SpoolableResourceManager::GetInstance();

	uint64_t key = MakeKey(package, id);
	auto it = m_Handles.find(key);

	if (it != m_Handles.end()) 
	{
		it->second.m_References--;
		if (it->second.m_References <= 0) {
			printf("Releasing asset id %i, package %i\n", id, package);
			spool->SetEntityPriority(package, id, SpoolPriority::Release);
			m_Handles.erase(it);
		}
	}
	else
	{
		printf("Tried to free resource that wasn't acquired! (id %i, package %i)\n", id, package);
	}
}

bool ResourceManager::IsLoaded(SpooledPackageType package, int id) {
	SpoolableResourceManager* spool = SpoolableResourceManager::GetInstance();
	return spool->IsEntityLoaded(package, id);
}

bool ResourceManager::IsAlive(SpooledPackageType package, int id) {
	if (package == SpooledPackageType::Characters && (id == ECharacterType::TheKid_Then || id == ECharacterType::TheDriver_Now)) return true;
	return (m_Handles.find(MakeKey(package, id)) != m_Handles.end());
}