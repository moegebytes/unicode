#include "..\pch.h"
#include "hook.h"
#include "engine.h"
#include "..\hook\manager.h"
#include "..\util\env.h"

namespace FVP {
	void Hook::Install() {
		const auto hEngine = (uintptr_t)GetModuleHandle(NULL);
		HookManager::Install((bool(__thiscall *) (void*, const char*))(hEngine + FAVS::MoviePlayer::InitFilter),
			Hook::MoviePlayer::InitFilter);
	}
}

namespace FVP {
	bool Hook::MoviePlayer::InitFilter(void* instance, const char* fname) {
		DbgPrintVerbose("Intercepted MoviePlayer::InitFilter with fname=" << fname);

		if (!Util::IsWineEnvironment()) {
			return HookManager::Call(InitFilter, instance, fname);
		}

		std::filesystem::path path(fname);
		if (path.filename().extension() == ".wmv" || path.filename().extension() == ".mpg") {
			path.replace_extension(".webm");
		}

		DbgPrintVerbose("Adjusting MoviePlayer::InitFilter to fname=" << path);

		return HookManager::Call(InitFilter, instance, path.string().c_str());
	}
}