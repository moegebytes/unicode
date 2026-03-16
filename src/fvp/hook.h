#pragma once

#include "..\pch.h"

namespace FVP {
	class Hook {
	public:
		static void Install();
	private:
		class MoviePlayer {
		public:
			static bool __thiscall InitFilter(void* instance, const char* fname);
		};

#if FVP_GAME_ID >= HOSHINOMEMORIA
		class Window {
		public:
			static HWND __thiscall InitWindow(void* instance);
			static LRESULT __thiscall WndProc(void* instance, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		private:
			static constexpr UINT WM_RESTORE_PLACEMENT = WM_USER + 0x3BFF;
			static bool bIsResizing;
		};
#endif
	};
}