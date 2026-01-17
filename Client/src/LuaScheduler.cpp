#include "LuaScheduler.h"

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