#pragma once

class CCharacterCategoryManager {
public:
	static CCharacterCategoryManager* GetInstance();
	void SetPlayerAvatar(int id);
};