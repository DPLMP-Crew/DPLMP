#include "LuaEngine.h"
#include <fstream>
#include <filesystem>

LuaEngine::LuaEngine() {
	m_LuaState = sol::state{};
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