#include "LuaEngine.h"
#include <fstream>
#include <filesystem>

namespace LuaUI {
	void Register(LuaEngine* lua);
}

LuaEngine::LuaEngine() {
	m_LuaState.open_libraries(sol::lib::base, sol::lib::bit32, sol::lib::coroutine, sol::lib::math, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::utf8);
	LuaUI::Register(this);
}

void LuaEngine::RunScriptFromFile(const wchar_t* filename) {
	std::string finalText;
	std::ifstream file(filename);

	if (!file.good()) {
		return;
	}

	std::string line;

	while (std::getline(file, line)) {
		finalText += line + "\n";
	}

	file.close();

	m_LuaState.script(finalText, "script", sol::load_mode::text);
}