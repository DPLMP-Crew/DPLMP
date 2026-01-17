#include "LuaUI.h"
#include "dpl/UINotification.h"
#include "dpl/CWipeManager.h"

void LuaUI::Register(sol::state* lua) {
	sol::table uiTable = lua->create_named_table("UI");
	uiTable.set_function("ShowNotification",
		[](const std::string& text, float duration, bool fadeOut) {
			UINotification* uiNotif = UINotification::GetInstance();
			if (uiNotif == nullptr) return;
			uiNotif->Clear();
			UINotification::SetCustomString(text.c_str());
			uiNotif->Show(0, 0, 137, duration, fadeOut);
		}
	);
	uiTable.set_function("ClearNotification",
		[] {
			UINotification* uiNotif = UINotification::GetInstance();
			if (uiNotif == nullptr) return;
			uiNotif->Clear();
		}
	);
	uiTable.set_function("ResetWipes",
		[] {
			CWipeManager* wipeMgr = CWipeManager::GetInstance();
			if (wipeMgr == nullptr) return;
			wipeMgr->ResetWipes();
		}
	);
}