#pragma once
#include <sol/sol.hpp>
#include <string>
#include <vector>

class LuaTask {
public:
	sol::coroutine m_Coroutine;
	float m_Remaining;
};

class LuaScheduler {
public:
	void Update(float delta);
	template <typename... Args>
	void AddTask(sol::coroutine co, Args&&... args) {
		sol::protected_function_result result = co(std::forward<Args>(args)...);
		if (result.status() == sol::call_status::yielded) {
			float wait = result.get<float>();
			LuaTask task;
			task.m_Coroutine = std::move(co);
			task.m_Remaining = wait;
			m_Tasks.emplace_back(std::move(task));
		}
	}

private:
	std::vector<LuaTask> m_Tasks;
};

class LuaEngine {
public:
	LuaEngine();
	void Update(float delta);
	void RunScriptFromFile(const std::wstring& filename);
	sol::state m_LuaState;
	LuaScheduler m_Scheduler;
};