#include "dpl/UINotification.h"
#include <string>
#include "scan.h"

static std::string customString;

UINotification* UINotification::GetInstance() {
	return ((UINotification**)0x70C7F8)[0];
}

void UINotification::Show(int unk, int unk2, int stringId, float duration, bool unk3) {
	((void(__thiscall*)(UINotification*, int, int, int, float, bool))0x472C34)(this, unk, unk2, stringId, duration, unk3);
}

void UINotification::Clear() {
	((void(__thiscall*)(UINotification*))0x472759)(this);
}

void UINotification::SetCustomString(const char* str) {
	customString = str;
	const char* pStr = customString.c_str();
	WriteToMemory((DWORD)0x472B63, &pStr, 4);
}