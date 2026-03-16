#pragma once

#include "..\pch.h"

namespace FVP {
	class Window {
	public:
#if FVP_GAME_ID >= HOSHINOMEMORIA
		static void UpdateScreen(void* instance, int w, int h);
		static void SavePlacement(void* instance, HWND hWnd);
		static bool RestorePlacement(void* instance, HWND hWnd, DWORD dwStyle);
		static SIZE GetDefaultClientSize(int gameW, int gameH);
	private:
		struct PlacementData {
			LONG width;
			LONG height;
		};

		static std::filesystem::path GetPlacementPath(void* instance);
#endif
	};
}