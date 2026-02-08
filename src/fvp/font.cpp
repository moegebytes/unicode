#include "..\pch.h"
#include "font.h"
#include "..\util\process.h"

namespace FVP {
	void Font::Load() {
		const std::filesystem::path directory{Util::GetProcessParentPath()/"font"};

		if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
			return;
		}

		for (const auto& entry: std::filesystem::directory_iterator(directory)) {
			if (!std::filesystem::is_regular_file(entry.path())) continue;
			if (entry.path().extension() != ".ttf" && entry.path().extension() != ".otf") continue;

			DbgPrintVerbose("Loading additional font with path=" << entry.path());
			AddFontResourceEx(entry.path().c_str(), FR_PRIVATE, 0);
		}
	}
}