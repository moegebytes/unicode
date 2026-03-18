#pragma once

#include "..\pch.h"

namespace FVP {
	class Hook {
	public:
		static void Install();
	private:
		class MoviePlayer {
		public:
			static bool __thiscall InitFilter(void* self, const char* fname);
		};

		class Engine {
		public:
#if FVP_GAME_ID >= HOSHINOMEMORIA
			static HWND __thiscall InitWindow(void* self);
			static LRESULT __thiscall PrimaryWindowProcA(void* self, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#endif
		private:
#if FVP_GAME_ID >= HOSHINOMEMORIA
			// Custom message for PrimaryWindowProcA to trigger placement restore.
			// Engine already occupies WM_USER (0x0400) itself; to be extra careful, we occupy
			// last message possible (0x7FFF).
			static constexpr unsigned int WM_RESTORE_SIZE = WM_USER + 0x3BFF;
#endif
		};
	};
}