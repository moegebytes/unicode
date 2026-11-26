#pragma once

#include "..\pch.h"
#include <span>

namespace Util {
	std::wstring GetFontFamilyName(HDC hDC, LANGID langId);
}