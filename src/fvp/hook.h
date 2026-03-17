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
			static constexpr UINT WM_RESTORE_PLACEMENT = WM_USER + 0x3BFF;
			static bool bIsResizing;
#endif
		};
	};
}