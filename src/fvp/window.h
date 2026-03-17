#pragma once

#include "..\pch.h"
#include "engine.h"

namespace FVP {
	class Window {
	public:
		static SIZE GetDefaultClientSize(int gameW, int gameH);

#if FVP_GAME_ID >= HOSHINOMEMORIA
		static void UpdateScreen(void* engine, int w, int h);
		static void SavePlacement(void* engine, HWND hWnd);
		static bool RestorePlacement(void* engine, HWND hWnd, DWORD dwStyle);
#endif
	private:
#if FVP_GAME_ID >= HOSHINOMEMORIA
		struct PlacementData {
			LONG width;
			LONG height;
		};

		static inline std::filesystem::path GetPlacementPath(void* engine) {
			const char* base = FAVS::Field<const char*>(engine, FAVS::Engine::Fields::LocalDataPath);
			return std::filesystem::path(base) / "window.bin";
		}
#endif
	};
}