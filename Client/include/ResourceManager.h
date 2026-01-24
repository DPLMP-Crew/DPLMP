#pragma once
#include <unordered_map>
#include <cstdint>
#include "dpl/SpoolableResourceManager.h"

class ResourceHandle {
public:
	int m_References = 1;
};

class ResourceManager {
public:
	void Acquire(SpooledPackageType package, int id);
	void Release(SpooledPackageType package, int id);
	bool IsLoaded(SpooledPackageType package, int id);
	bool IsAlive(SpooledPackageType package, int id);

private:
	std::unordered_map<uint64_t, ResourceHandle> m_Handles;
};