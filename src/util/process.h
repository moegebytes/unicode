#pragma once

#include "..\pch.h"

namespace Util {
	inline std::filesystem::path GetProcessParentPath() {
		wchar_t szProcessPath[MAX_PATH] = {};
		GetModuleFileName(NULL, szProcessPath, MAX_PATH);

		return std::filesystem::path(szProcessPath).parent_path();
	}
}