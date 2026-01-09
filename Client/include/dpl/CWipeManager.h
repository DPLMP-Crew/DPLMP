#pragma once

class CWipeManager {
public:
	static CWipeManager* GetInstance();
	void ResetWipes();
};