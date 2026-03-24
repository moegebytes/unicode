#pragma once

#include "..\pch.h"

namespace Util {
	inline std::unique_ptr<wchar_t[]> NarrowToWide(unsigned int codePage, const char* narrowText) {
		int wideLength = MultiByteToWideChar(codePage, 0, narrowText, -1, NULL, 0);
		auto wideText = std::make_unique<wchar_t[]>(wideLength);
		MultiByteToWideChar(codePage, 0, narrowText, -1, wideText.get(), wideLength);
		return wideText;
	}

	inline std::unique_ptr<char[]> WideToNarrow(unsigned int codePage, const wchar_t* wideText) {
		int narrowLength = WideCharToMultiByte(codePage, 0, wideText, -1, NULL, 0, NULL, NULL);
		auto narrowText = std::make_unique<char[]>(narrowLength);
		WideCharToMultiByte(codePage, 0, wideText, -1, narrowText.get(), narrowLength, NULL, NULL);
		return narrowText;
	}
}