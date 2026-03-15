#pragma once

#include "..\pch.h"

namespace FVP {
	class Window {
	public:
#if FVP_ENGINE_VER >= 10002
		static void UpdateScreen(void* instance, int w, int h);
		static void SavePlacement(void* instance, HWND hWnd);
		static bool RestorePlacement(void* instance, HWND hWnd, DWORD dwStyle);
	private:
		struct PlacementData {
			LONG width;
			LONG height;
		};

		static std::filesystem::path GetPlacementPath(void* instance);
#endif
	};
}