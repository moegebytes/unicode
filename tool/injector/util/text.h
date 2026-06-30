#pragma once

#include "..\pch.h"

namespace Util {
	inline std::unique_ptr<wchar_t[]> NarrowToWide(const char* narrowText) {
		int wideLength = MultiByteToWideChar(CP_ACP, 0, narrowText, -1, NULL, 0);
		auto wideText = std::make_unique<wchar_t[]>(wideLength);
		MultiByteToWideChar(CP_ACP, 0, narrowText, -1, wideText.get(), wideLength);
		return wideText;
	}

	inline std::unique_ptr<char[]> WideToNarrow(const wchar_t* wideText) {
		int narrowLength = WideCharToMultiByte(CP_ACP, 0, wideText, -1, NULL, 0, NULL, NULL);
		auto narrowText = std::make_unique<char[]>(narrowLength);
		WideCharToMultiByte(CP_ACP, 0, wideText, -1, narrowText.get(), narrowLength, NULL, NULL);
		return narrowText;
	}
}