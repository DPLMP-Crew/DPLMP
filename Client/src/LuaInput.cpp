#include "LuaInput.h"
#include <Windows.h>

static bool wasKeyJustPressed(int vk)
{
	static SHORT lastState[256]{};

	SHORT state = GetAsyncKeyState(vk);
	bool pressed = (state & 0x8000) && !(lastState[vk] & 0x8000);

	lastState[vk] = state;
	return pressed;
}

void LuaInput::Register(sol::state* lua) {
	sol::table inputTable = lua->create_named_table("Input");
	inputTable.set_function("KeyPressed",
		[](int keyCode) {
			return wasKeyJustPressed(keyCode);
		}
	);
}