#pragma once
#include <sol/sol.hpp>
#include "IdManager.h"

class LuaTask {
public:
	sol::thread m_Thread;
	sol::coroutine m_Coroutine;
	float m_Remaining;
	Id64 m_Id;
};

class LuaScheduler {
public:
	void Update(float delta);
	template <typename... Args>
	Id64 AddTask(sol::coroutine co, sol::thread thread, Args&&... args) {
		sol::protected_function_result result = co(std::forward<Args>(args)...);
		if (result.status() == sol::call_status::yielded) {
			float wait = result.get<float>();
			std::unique_ptr task = std::make_unique<LuaTask>();
			task->m_Thread = std::move(thread);
			task->m_Coroutine = std::move(co);
			task->m_Remaining = wait;
			Id64 id = m_IdManager.Acquire();
			task->m_Id = id;
			m_Tasks.emplace_back(std::move(task));
			return id;
		}
		return Id64();
	}

private:
	std::vector<std::unique_ptr<LuaTask>> m_Tasks;
	IdManager m_IdManager;
};