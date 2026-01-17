#pragma once

// NOT REAL SOURCE/SYMBOL NAMES - COMPLETELY GUESSED
class UINotification {
public:
	static UINotification* GetInstance();
	static void SetCustomString(const char* str);
	void Clear();
	void Show(int unk, int unk2, int stringId, float duration, bool unk3);
};