#include "LuaUI.h"
#include <sol/sol.hpp>
#include "dpl/UINotification.h"

namespace LuaUI {
	void Register(LuaEngine* lua) {
		sol::table uiTable = lua->m_LuaState.create_named_table("UI");
		uiTable.set_function("ShowNotification",
			[](const std::string& text, float duration) {
				UINotification* uiNotif = UINotification::GetInstance();
				uiNotif->Clear();
				UINotification::SetCustomString(text.c_str());
				uiNotif->Show(0, 0, 999, duration, true);
			}
		);
		uiTable.set_function("ClearNotification",
			[] {
				UINotification::GetInstance()->Clear();
			}
		);
	}
}