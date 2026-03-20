#include "..\pch.h"
#include "window.h"
#include "..\util\describe.h"
#include "..\util\monitor.h"

namespace FVP {
	SIZE Window::GetOptimalWindowSize(HWND hWnd, int gameW, int gameH) {
		SIZE monitor = Util::GetMonitorSize(hWnd);
		double scaleW = static_cast<double>(monitor.cx) * 4.0 / 5.0 / static_cast<double>(gameW);
		double scaleH = static_cast<double>(monitor.cy) * 4.0 / 5.0 / static_cast<double>(gameH);

		auto scale = (scaleW < scaleH) ? scaleW : scaleH;
		if (scale >= 1.0) {
			return { gameW, gameH }; // Clamp to native game resolution to avoid upscaling
		}

		return { static_cast<LONG>(gameW * scale), static_cast<LONG>(gameH * scale) };
	}

	SIZE Window::GetMinimumWindowSize(HWND hWnd, int gameW, int gameH) {
		SIZE monitor = Util::GetMonitorSize(hWnd);
		double scaleW = static_cast<double>(monitor.cx) * 2.0 / 3.0 / static_cast<double>(gameW);
		double scaleH = static_cast<double>(monitor.cy) * 2.0 / 3.0 / static_cast<double>(gameH);

		auto scale = (scaleW < scaleH) ? scaleW : scaleH;
		if (scale >= 1.0) {
			return { gameW, gameH }; // Clamp to native game resolution to avoid upscaling
		}

		return { static_cast<LONG>(gameW * scale), static_cast<LONG>(gameH * scale) };
	}

#if FVP_GAME_ID >= HOSHINOMEMORIA
	void Window::UpdateScreen(void* engine, int w, int h) {
		// Engine uses these values to perform downscaling against game size. Normally, these values
		// only store screen resolution, but we hijack them and adjust to window size, effectively
		// lying to game about monitor resolution it's being displayed on.
		FAVS::Field<DWORD>(engine, FAVS::Engine::Fields::ScreenW) = w;
		FAVS::Field<DWORD>(engine, FAVS::Engine::Fields::ScreenH) = h;

		HWND hGameWnd = FAVS::Field<HWND>(engine, FAVS::Engine::Fields::GameWindowHnd);
		if (hGameWnd) {
			SetWindowPos(hGameWnd, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
		}
	}

	void Window::SaveDimensions(void* engine, HWND hWnd) {
		RECT rc;
		if (!GetClientRect(hWnd, &rc)) {
			DbgPrintVerbose("Unable to retrieve window dimensions; code=" << GetLastError());
			return;
		}

		PlacementData data = {
			rc.right,
			rc.bottom
		};

		auto path = GetPersistPath(engine).string();
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			DbgPrint("Failed to persist window state; code=" << GetLastError());
			return;
		}

		DWORD nBytesWritten;
		WriteFile(hFile, &data, sizeof(data), &nBytesWritten, NULL);
		CloseHandle(hFile);

		if (nBytesWritten != sizeof(data)) {
			DbgPrint("Failed to persist window state; code=" << GetLastError());
		}
	}

	bool Window::RestoreDimensions(void* engine, HWND hWnd, DWORD dwStyle) {
		auto path = GetPersistPath(engine).string();
		HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			DbgPrint("Failed to restore window state; code=" << GetLastError());
			return false;
		}

		PlacementData data;

		DWORD nBytesRead;
		BOOL bResult = ReadFile(hFile, &data, sizeof(data), &nBytesRead, NULL);
		CloseHandle(hFile);

		if (!bResult || nBytesRead != sizeof(data) || data.width <= 0 || data.height <= 0) {
			DbgPrint("Failed to restore window state; code=" << GetLastError());
			return false;
		}

		RECT rc = { 0, 0, data.width, data.height };
		AdjustWindowRectEx(&rc, dwStyle, FALSE, 0);

		SetWindowPos(
			hWnd,
			NULL,
			0, 0,
			rc.right - rc.left,
			rc.bottom - rc.top,
			SWP_NOMOVE | SWP_NOZORDER
		);

		DbgPrintVerbose("Restored window state=" << Util::Describe(data));
		return true;
	}
#endif
}