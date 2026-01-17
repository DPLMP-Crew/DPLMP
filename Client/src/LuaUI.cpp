#include "LuaUI.h"
#include "dpl/UINotification.h"
#include "dpl/CWipeManager.h"

void LuaUI::Register(sol::state* lua) {
	sol::table uiTable = lua->create_named_table("UI");
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
	uiTable.set_function("ResetWipes",
		[] {
			CWipeManager::GetInstance()->ResetWipes();
		}
	);
}