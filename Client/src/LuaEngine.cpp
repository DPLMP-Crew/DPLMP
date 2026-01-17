#include "LuaEngine.h"
#include <fstream>
#include <filesystem>
#include <windows.h>
#define IncludeAPI(name) namespace name { void Register(LuaEngine* lua); }
#define RegisterAPI(name) name::Register(this);

IncludeAPI(LuaUI);
IncludeAPI(LuaHooks);

void LuaScheduler::Update(float delta) {
	for (auto it = m_Tasks.begin(); it != m_Tasks.end(); ) {
		LuaTask& task = *it;
		task.m_Remaining -= delta;

		if (task.m_Remaining > 0.0f) {
			++it;
			continue;
		}

		sol::protected_function_result result = task.m_Coroutine();

		if (task.m_Coroutine.status() == sol::call_status::yielded) {
			task.m_Remaining = result.get<float>();
			++it;
		}
		else
		{
			it = m_Tasks.erase(it);
		}
	}
}

void LuaEngine::Update(float delta) {
	m_Scheduler.Update(delta);
}

LuaEngine::LuaEngine() {
	m_LuaState.open_libraries(sol::lib::base, sol::lib::bit32, sol::lib::coroutine, sol::lib::math, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::utf8);
	RegisterAPI(LuaUI);
	RegisterAPI(LuaHooks);
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