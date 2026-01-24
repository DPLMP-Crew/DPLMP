#pragma once

enum SpooledPackageType
{
	Vehicles = 0,
	UnknownPackage,
	Characters
};

enum SpoolPriority
{
	Release = 0,
	Request
};

class SpoolableResourceManager {
public:
	void SetEntityPriority(SpooledPackageType packageType, int ID, SpoolPriority priority);
	void RequestEntity(SpooledPackageType packageType, int ID, int unk1, SpoolPriority priority);
	bool IsEntityLoaded(SpooledPackageType packageType, int ID);
	bool IsEntityPresent(SpooledPackageType packageType, int ID);
	int GetStatus();
	static SpoolableResourceManager* GetInstance();
};