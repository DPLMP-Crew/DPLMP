#pragma once
#include <sol/sol.hpp>

class LuaTask {
public:
	sol::thread m_Thread;
	sol::coroutine m_Coroutine;
	float m_Remaining;
};

class LuaScheduler {
public:
	void Update(float delta);
	template <typename... Args>
	void AddTask(sol::coroutine co, sol::thread thread, Args&&... args) {
		sol::protected_function_result result = co(std::forward<Args>(args)...);
		if (result.status() == sol::call_status::yielded) {
			float wait = result.get<float>();
			LuaTask task;
			task.m_Thread = std::move(thread);
			task.m_Coroutine = std::move(co);
			task.m_Remaining = wait;
			m_Tasks.emplace_back(std::move(task));
		}
	}

private:
	std::vector<LuaTask> m_Tasks;
};