#pragma once

#include "..\pch.h"
#include "engine.h"

namespace FVP {
	class Window {
	public:
		static SIZE GetOptimalWindowSize(int gameW, int gameH);
		static SIZE GetMinimumWindowSize(int gameW, int gameH);

#if FVP_GAME_ID >= HOSHINOMEMORIA
		static void UpdateScreen(void* engine, int w, int h);
		static void SaveDimensions(void* engine, HWND hWnd);
		static bool RestoreDimensions(void* engine, HWND hWnd, DWORD dwStyle);
#endif
	private:
#if FVP_GAME_ID >= HOSHINOMEMORIA
		struct PlacementData {
			LONG width;
			LONG height;
		};

		inline static std::filesystem::path GetPersistPath(void* engine) {
			const char* base = FAVS::Field<const char*>(engine, FAVS::Engine::Fields::LocalDataPath);
			return std::filesystem::path(base) / "window.bin";
		}
#endif
	};
}