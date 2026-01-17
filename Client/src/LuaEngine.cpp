#include "LuaEngine.h"
#include <fstream>
#include <filesystem>
#include <windows.h>

namespace LuaUI {
	void Register(LuaEngine* lua);
}

LuaEngine::LuaEngine() {
	m_LuaState.open_libraries(sol::lib::base, sol::lib::bit32, sol::lib::coroutine, sol::lib::math, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::utf8);
	LuaUI::Register(this);
}

void LuaEngine::RunScriptFromFile(const std::wstring& filename) {
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

	std::string result("<invalid script name>");
	
	if (!filename.empty()) {
		int size = WideCharToMultiByte(
			CP_UTF8, 0,
			filename.data(), filename.size(),
			nullptr, 0,
			nullptr, nullptr
		);

		if (size > 0)
		{
			result = std::string(size, '\0');

			WideCharToMultiByte(
				CP_UTF8, 0,
				filename.data(), filename.size(),
				result.data(), size,
				nullptr, nullptr
			);
		}
	}

	m_LuaState.script(finalText, result, sol::load_mode::text);
}